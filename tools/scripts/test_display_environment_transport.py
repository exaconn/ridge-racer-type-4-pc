"""Current display submission/readback, complete owners and a byte/protocol oracle.

Driver/video/log/copy callbacks deliberately mutate live input/cache/timing data.
Physical register bindings are normalized; the existing memory barrier and cache
load volatility remain. GPU execution and interrupt concurrency are not modeled.
"""
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

from c_functions import definition_source, source_owners

ROOT = Path(__file__).resolve().parents[2]
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include "psyq/libgpu/gpudisp.h"
#include "rr/gpu_reset.h"
typedef struct {
 GpuResetState80 state;DisplayEnvironment input[2];GpuHorizontalRange ranges[2][5];
 u8 divisors[5],reserved[3];u32 root;
} Data;
typedef struct {u32 kind,args[4];Data before;} Event;
typedef struct {u32 before[4];Data data;u32 calls,serial;Event events[16];u32 after[4];} World;
static World actual,expected,*world;
static GpuDriver40 drivers[2];
static unsigned scenario,profile;
static void unused_completion(void){assert(!"display transport must not invoke completion");}
const char StrPutDispEnvFmt[]="display";
#define GpuResetState (world->data.state)
#define GpuFuncTable (&drivers[world->data.root])
#define DispHRangeTable (world->data.ranges)
#define DispDotClockDivisors (world->data.divisors)
static const s16 widths[]={-32768,-1,0,1,280,281,352,353,400,401,560,561,32767};
static const s16 heights[]={-32768,-1,0,1,256,257,288,289,32767};
static const s16 coordinates[]={-32768,-513,-256,-1,0,1,255,256,257,32767};
static u32 offset(const void *p){uintptr_t n=(uintptr_t)p-(uintptr_t)&world->data;assert(n<sizeof(Data));return (u32)n;}
static u16 get16(const void *p){u16 n;memcpy(&n,p,2);return n;}
static u32 get32(const void *p){u32 n;memcpy(&n,p,4);return n;}
static s32 half(const void *p){u16 n=get16(p);return n&32768?(s32)n-65536:n;}
static void put16(void *p,u32 n){u16 bits=(u16)n;memcpy(p,&bits,2);}
static DisplayEnvironment *environment(unsigned which){return which<2?&world->data.input[which]:&world->data.state.display6C;}
static void event(unsigned kind,u32 a,u32 b,u32 c,u32 d){
 assert(world->calls<16);Event *e=&world->events[world->calls++];e->kind=kind;
 e->args[0]=a;e->args[1]=b;e->args[2]=c;e->args[3]=d;e->before=world->data;
}
static void mutate(unsigned phase){
 unsigned serial=world->serial++;
 if(!profile)return;
 u32 bits=scenario*0x31415927U+serial*0x17171717U+phase;
 world->data.root^=1;world->data.state.debug02.level=(u8)(bits>>8);world->data.state.debug02.mode=(u8)bits;
 for(unsigned i=0;i<3;i++){
  u8 *p=(u8*)environment(i);
  put16(p,(u32)(s32)coordinates[(scenario+serial+i)%10]);put16(p+2,(u32)(s32)coordinates[(scenario/3+serial+i)%10]);
  put16(p+4,(u32)(s32)widths[(scenario+serial+i)%13]);put16(p+6,(u32)(s32)heights[(scenario/7+serial+i)%9]);
  for(unsigned j=0;j<4;j++)put16(p+8+j*2,(u32)(s32)coordinates[(scenario/5+serial+i+j)%10]);
  p[16]=(u8)(bits+i);p[17]=(u8)(bits>>8);p[18]=(serial+i)&1?8:(u8)bits;p[19]=(u8)(bits>>16);
 }
 for(unsigned row=0;row<2;row++)for(unsigned col=0;col<5;col++){
  world->data.ranges[row][col].start=(u16)(bits+row*0x1234U+col*0x1717U);
  world->data.ranges[row][col].end=(u16)((bits>>7)+row*0x9876U+col*0x3131U);
 }
 for(unsigned i=0;i<5;i++)world->data.divisors[i]=(u8)((bits>>i)+i);
}
static s32 debug(const char *format,...){
 assert(format==StrPutDispEnvFmt);va_list ap;va_start(ap,format);DisplayEnvironment *env=va_arg(ap,DisplayEnvironment*);va_end(ap);
 event(1,offset(env),0,0,0);mutate(1);return -1;
}
s32 (*GPU_printf)(const char*,...)=debug;
static void control(unsigned which,u32 command){assert(which==world->data.root);event(2,which,command,0,0);mutate(2);}
static void control0(u32 command){control(0,command);}
static void control1(u32 command){control(1,command);}
s32 GetVideoMode(void){s32 mode=(scenario+world->serial)&1;event(3,(u32)mode,0,0,0);mutate(3);return mode;}
void PsyqMemcpy(void *destination,void *source,s32 count){
 assert(count==20);event(4,offset(destination),offset(source),(u32)count,0);
 u8 bytes[20];memcpy(bytes,source,20);memcpy(destination,bytes,20);mutate(4);
}
'''
TAIL = r'''
static unsigned width_class(s32 width){
 const s32 boundaries[]={281,353,401,561};unsigned index=0;
 while(index<4&&width>=boundaries[index])index++;return index;
}
static s32 clamp(s32 n,s32 low,s32 high){if(n<low)return low;if(n>high)return high;return n;}
static u32 paths[4],classes[5],standards[2],modeBranches[2],rangeBranches[2],defaults[2],aliases[3],debugLevels[256];
static DisplayEnvironment *reference(unsigned readback,DisplayEnvironment *env){
 u8 *p=(u8*)env,*cache=(u8*)&world->data.state+116;
 if(readback){PsyqMemcpy(p,cache,20);return env;}
 if(((u8*)&world->data.state)[2]>=2)debug(StrPutDispEnvFmt,env);
 control(world->data.root,0x05000000U|(get16(p)&1023U)|((get16(p+2)&1023U)<<10));
 unsigned mode_changed=get32(cache+16)!=get32(p+16)||memcmp(cache,p,8)!=0;
 modeBranches[mode_changed]++;
 if(mode_changed){
  p[18]=(u8)GetVideoMode();
  static const u32 resolution[]={0,1,64,2,3};
  u32 command=0x08000000U|resolution[width_class(half(p+4))];
  if(p[18]==1)command|=8;if(p[17])command|=16;if(p[16])command|=32;
  if(((u8*)&world->data.state)[3])command|=128;
  if(half(p+6)>=(p[18]?289:257))command|=36;
  control(world->data.root,command);p[18]=8;
 }
 unsigned timing_changed=memcmp(cache+8,p+8,8)!=0||p[18]==8;
 rangeBranches[timing_changed]++;paths[mode_changed*2+timing_changed]++;
 if(timing_changed){
  p[18]=(u8)GetVideoMode();unsigned mode=p[18],index=width_class(half(p+4));assert(mode<2);
  standards[mode]++;classes[index]++;
  s32 y0=half(p+10)+(mode?19:16),height=half(p+14);defaults[height==0]++;
  s32 y1=y0+(height?height:240);
  const u8 *range=(const u8*)world->data.ranges+(mode*5+index)*4;
  s32 clock=world->data.divisors[index];
  s32 x0=get16(range)+half(p+8)*clock;
  s32 distance=(s32)get16(range+2)-(s32)get16(range);
  s32 width=half(p+12);if(width)distance=(distance*width)>>8;
  s32 x1=x0+distance;
  if(p[18]){x0=clamp(x0,540,3220);x1=clamp(x1,x0+clock*4,3260);y0=clamp(y0,19,303);y1=clamp(y1,y0+2,305);}
  else {x0=clamp(x0,500,3250);x1=clamp(x1,x0+clock*4,3290);y0=clamp(y0,16,257);y1=clamp(y1,y0+2,258);}
  control(world->data.root,0x06000000U|(((u32)x1&4095U)<<12)|((u32)x0&4095U));
  /* This command uses the already-calculated vertical window after control's callback. */
  control(world->data.root,0x07000000U|(((u32)y1&1023U)<<10)|((u32)y0&1023U));
 }
 PsyqMemcpy(cache,p,20);return env;
}
static u32 seed=0x92998;
static u32 next(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
int main(void){
 assert(sizeof(void*)==8&&(uintptr_t)&actual>UINT32_MAX);
 assert(sizeof(DisplayEnvironment)==20&&_Alignof(DisplayEnvironment)>=4);
 assert(sizeof(GpuResetState80)==136&&offsetof(GpuResetState80,completion0C)==16&&offsetof(GpuResetState80,display6C)==116);
 assert(offsetof(DisplayEnvironment,flags10)==16&&sizeof(GpuHorizontalRange)==4);
 DisplayFlags flags;flags.word=0x78563412U;assert(flags.bytes.interlace==0x12&&flags.bytes.rgb24==0x34&&flags.bytes.mode==0x56&&flags.bytes.reserved==0x78);
 drivers[0].control10=control0;drivers[1].control10=control1;
 for(scenario=0;scenario<16384;scenario++){
  for(unsigned i=0;i<sizeof(actual);i+=4){u32 value=next();memcpy((u8*)&actual+i,&value,4);}
  actual.data.state.completion0C=(scenario&2)?unused_completion:0;
  actual.data.root=(scenario>>2)&1;actual.data.state.debug02.level=(u8)(scenario>>5);debugLevels[actual.data.state.debug02.level]++;
  actual.calls=actual.serial=0;memset(actual.events,0,sizeof(actual.events));
  profile=(scenario>>3)&1;unsigned which=(scenario>>1)%3,readback=scenario&1;
  world=&actual;DisplayEnvironment *env=environment(which);u8 *p=(u8*)env;
  put16(p+4,(u32)(s32)widths[(scenario>>4)%13]);put16(p+6,(u32)(s32)heights[(scenario>>6)%9]);
  for(unsigned i=0;i<4;i++)put16(p+8+i*2,(u32)(s32)coordinates[(scenario/11+i)%10]);
  p[16]=(scenario>>4)&1;p[17]=(scenario>>5)&1;p[18]=(scenario&4096)?8:(u8)((scenario>>6)&1);
  if(scenario&512)memcpy(&actual.data.state.display6C,env,20);
  if((scenario&1024)&&which<2)((u8*)&actual.data.state.display6C)[19]^=1;
  if((scenario&2048)&&which<2)((u8*)&actual.data.state.display6C)[8]^=1;
  expected=actual;world=&expected;env=environment(which);u32 wanted=offset(reference(readback,env));
  world=&actual;env=environment(which);DisplayEnvironment *result=readback?GetDispEnv(env):PutDispEnv(env);
  assert(offset(result)==wanted&&!memcmp(&actual,&expected,sizeof(actual)));aliases[which]++;
 }
 for(unsigned i=0;i<256;i++)assert(debugLevels[i]);for(unsigned i=0;i<3;i++)assert(aliases[i]);
 assert(paths[0]&&paths[1]&&paths[3]);/* A mode change always forces timing rebuild. */
 assert(!paths[2]);for(unsigned i=0;i<5;i++)assert(classes[i]);
 assert(standards[0]&&standards[1]&&modeBranches[0]&&modeBranches[1]&&rangeBranches[0]&&rangeBranches[1]&&defaults[0]&&defaults[1]);
 return 0;
}
'''


class DisplayEnvironmentTransportTest(unittest.TestCase):
    def test_actual_entries_with_live_cache_timing_and_driver_callbacks(self):
        owners = source_owners(ROOT / 'src/main/PAL')
        source_text = owners['PutDispEnv'].read_text()
        clamp_macro = re.search(r'^#define CLAMP[^\n]*', source_text, re.M).group(0)
        bodies = '\n'.join(definition_source(owners[name].read_text(), name)
                           for name in ('PutDispEnv', 'GetDispEnv'))
        bodies = re.sub(r'\s+(?:__asm__|asm)\("\$\d+"\)', '', bodies)
        with TemporaryDirectory() as directory:
            source = Path(directory) / 'display.c'
            binary = source.with_suffix('')
            source.write_text(PREFIX + '\n' + clamp_macro + '\n' + bodies + TAIL)
            result = subprocess.run(['cc', '-std=c11', '-O2', '-Wall', '-Wextra', '-Werror',
                                     '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
                                     '-I', str(ROOT / 'include'), str(source), '-o', str(binary)],
                                    capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            result = subprocess.run([str(binary)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)


if __name__ == '__main__':
    unittest.main()
