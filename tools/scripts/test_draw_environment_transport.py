"""Current SDK draw descriptor APIs with real owners and DMA address mapping.

Service-entry traces and complete memory are checked against an independent
byte/protocol model. Drawing-command arithmetic is tested separately; this test
scripts the builder, GPU queue, logger, video mode and BIOS copy boundaries.
"""
from pathlib import Path
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
#include "psyq/libgpu/gpuenv.h"
#include "rr/draw_environment.h"
#include "rr/gpu_reset.h"
#include "rr/gpu_address.h"
typedef struct {u32 kind,args[4],hash;} Event;
typedef struct {
 u32 before[4];GpuResetState80 gpu;DrawEnvironment env[2];u32 ordering[32];
 u32 root,calls;Event events[8];u32 after[4];
} World;
static World actual,expected,*w;
static GpuDriver40 drivers[2];
static GpuDmaRegion region;
static unsigned scenario,profile;
static void unused_completion(void){assert(!"drawing transport must not invoke completion");}
#define GpuResetState w->gpu
#define GpuFuncTable (&drivers[w->root])
static const char PutDrawEnvFmt[]="put",StrDrawOTagEnvFmt[]="tag";
static u32 offset(const void *p){uintptr_t n=(uintptr_t)p-(uintptr_t)w;assert(n<sizeof(*w));return (u32)n;}
static void event(u32 kind,u32 a,u32 b,u32 c,u32 d){
 assert(w->calls<8);Event *e=&w->events[w->calls++];e->kind=kind;
 e->args[0]=a;e->args[1]=b;e->args[2]=c;e->args[3]=d;
 u32 hash=2166136261U;const u8 *p=(const u8*)w;
 for(unsigned i=0;i<offsetof(World,root);i++)hash=(hash^p[i])*16777619U;
 e->hash=hash^w->root;
}
static void mutate(unsigned phase){
 if(!profile)return;
 u32 mask=scenario*0x31415927U+phase*0x17171717U;
 w->root=(w->root+1)&1;w->gpu.debug02.level=(u8)mask;
 for(unsigned i=0;i<2;i++){
  w->env[i].r^=(u8)mask;w->env[i].background^=(u8)(mask>>8);
  w->env[i].draw_packet1C[(scenario+phase+i)&15]^=mask;
 }
 w->gpu.draw10.r^=(u8)(mask>>16);
 w->gpu.draw10.draw_packet1C[(scenario+phase)&15]^=mask;
}
static s32 debug(const char *format,...){
 va_list ap;va_start(ap,format);u32 kind,first=0;DrawEnvironment *env;
 if(format==PutDrawEnvFmt){kind=1;env=va_arg(ap,DrawEnvironment*);}
 else {assert(format==StrDrawOTagEnvFmt);kind=2;first=offset(va_arg(ap,u32*));env=va_arg(ap,DrawEnvironment*);}
 va_end(ap);event(kind,first,offset(env),0,0);mutate(kind);return -1;
}
s32 (*GPU_printf)(const char*,...)=debug;
s32 GetVideoMode(void){
 static const s32 modes[]={0,1,-1,(-2147483647-1),2147483647};
 s32 result=modes[(scenario>>3)%5];event(3,(u32)result,0,0,0);mutate(3);return result;
}
void SetDrawEnv2(u32 *packet,DrawEnvironment *env){
 assert(packet==env->draw_packet1C&&(uintptr_t)env>UINT32_MAX);
 event(4,offset(packet),offset(env),0,0);
 unsigned count=env->background?9:6;
 for(unsigned i=1;i<=count;i++)packet[i]=scenario*0x1234567U+i*0x17171717U;
 ((u8*)packet)[3]=(u8)count;mutate(4);
}
static void draw0(unsigned long p){(void)p;assert(!"queue must not execute GPU hardware");}
static void draw1(unsigned long p){(void)p;assert(!"queue must not execute GPU hardware");}
static s32 dispatch(unsigned which,GpuOpFunc op,void *packet,s32 size,unsigned long parameter){
 assert(op==(GpuOpFunc)drivers[which].draw_ordering_table&&size==64&&parameter==0);
 event(5+which,offset(packet),(u32)size,(u32)parameter,0);mutate(5+which);return -17;
}
static s32 dispatch0(GpuOpFunc op,void *p,s32 n,unsigned long a){return dispatch(0,op,p,n,a);}
static s32 dispatch1(GpuOpFunc op,void *p,s32 n,unsigned long a){return dispatch(1,op,p,n,a);}
void *PsyqMemcpy(void *dst,const void *src,s32 count){
 assert(count==92);event(7,offset(dst),offset(src),(u32)count,0);
 /* All exercised copies are disjoint complete descriptors or exact self-copy. */
 u8 bytes[92];memcpy(bytes,src,92);memcpy(dst,bytes,92);mutate(7);return dst;
}
'''
TAIL = r'''
static void put16(u8 *p,u32 n){p[0]=(u8)n;p[1]=(u8)(n>>8);}
static u32 get32(const u8 *p){u32 n;memcpy(&n,p,4);return n;}
static void put32(u8 *p,u32 n){memcpy(p,&n,4);}
static void model_builder(DrawEnvironment *env){
 u8 *p=(u8*)env;unsigned n=p[24]?9:6;event(4,offset(p+28),offset(env),0,0);
 for(unsigned i=1;i<=n;i++)put32(p+28+i*4,scenario*0x1234567U+i*0x17171717U);
 p[31]=(u8)n;mutate(4);
}
static DrawEnvironment *reference(unsigned operation,DrawEnvironment *env,u32 *ordering,s32 x,s32 y,s32 width,s32 height){
 u8 *bytes=(u8*)env;u8 *cache=(u8*)&w->gpu+24;
 if(operation==3){
  s32 mode=GetVideoMode();
  put16(bytes,(u32)x);put16(bytes+2,(u32)y);put16(bytes+4,(u32)width);put16(bytes+6,(u32)height);
  put16(bytes+8,(u32)x);put16(bytes+10,(u32)y);memset(bytes+12,0,8);put16(bytes+20,10);
  bytes[22]=1;bytes[23]=(u8)(height<(mode?289:257));memset(bytes+24,0,4);return env;
 }
 if(operation==2){
  PsyqMemcpy(bytes,cache,92);return env;
 }
 if(((u8*)&w->gpu)[2]>=2){
  if(operation==0)debug(PutDrawEnvFmt,env);else debug(StrDrawOTagEnvFmt,ordering,env);
 }
 model_builder(env);
 u32 tag=get32(bytes+28);
 if(operation==0)tag|=0xFFFFFFU;
 else tag=(tag&0xFF000000U)|((0x80180000U+offset(ordering))&0xFFFFFFU);
 put32(bytes+28,tag);
 unsigned root=w->root;dispatch(root,(GpuOpFunc)drivers[root].draw_ordering_table,bytes+28,64,0);
 PsyqMemcpy(cache,bytes,92);return env;
}
static u32 seed=0x92EA4;
static u32 next(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
int main(void){
 const s32 extremes[]={(-2147483647-1),-65537,-32769,-32768,-1,0,1,255,256,257,288,289,32767,65535,2147483647};
 unsigned seen[4]={0},levels[256]={0},roots[3]={0},aliases[16]={0};
 assert(sizeof(void*)==8&&(uintptr_t)&actual>UINT32_MAX);
 assert(sizeof(DrawEnvironment)==92&&sizeof(DisplayEnvironment)==20&&sizeof(GpuResetState80)==136);
 assert(offsetof(GpuResetState80,completion0C)==16&&offsetof(GpuResetState80,draw10)==24&&offsetof(GpuResetState80,display6C)==116);
 drivers[0].dispatch=dispatch0;drivers[0].draw_ordering_table=draw0;
 drivers[1].dispatch=dispatch1;drivers[1].draw_ordering_table=draw1;
 region=(GpuDmaRegion){&actual,0x80180000U,sizeof(actual)};assert(GpuDmaSetRegions(&region,1));
 for(scenario=0;scenario<8192;scenario++){
  for(unsigned i=0;i<sizeof(actual);i++)((u8*)&actual)[i]=(u8)next();
  actual.gpu.completion0C=(scenario&1)?unused_completion:0;
  actual.gpu.debug02.level=(u8)(scenario>>2);levels[actual.gpu.debug02.level]++;
  actual.calls=0;memset(actual.events,0,sizeof(actual.events));actual.root=(scenario>>1)&1;
  expected=actual;profile=(scenario>>4)&1;unsigned op=scenario&3,which=(scenario>>5)%3,alias=(scenario>>2)&15;
  s32 x=extremes[(scenario>>2)%15],y=extremes[(scenario>>3)%15],width=extremes[(scenario>>4)%15],height=extremes[(scenario>>5)%15];
  w=&expected;DrawEnvironment *env=which<2?&w->env[which]:&w->gpu.draw10;
  u32 *head=(scenario&512)?env->draw_packet1C+alias:w->ordering+alias;
  DrawEnvironment *wanted=reference(op,env,head,x,y,width,height);u32 result=offset(wanted);
  w=&actual;env=which<2?&w->env[which]:&w->gpu.draw10;head=(scenario&512)?env->draw_packet1C+alias:w->ordering+alias;
  DrawEnvironment *returned;
  if(op==0)returned=PutDrawEnv(env);
  else if(op==1){DrawOTagEnv(head,env);returned=env;}
  else if(op==2)returned=GetDrawEnv(env);
  else returned=SetDefDrawEnv(env,x,y,width,height);
  assert(offset(returned)==result&&!memcmp(&actual,&expected,sizeof(actual)));
  seen[op]++;roots[which]++;if(op==1&&(scenario&512))aliases[alias]++;
 }
 for(unsigned i=0;i<4;i++)assert(seen[i]);for(unsigned i=0;i<256;i++)assert(levels[i]);
 for(unsigned i=0;i<3;i++)assert(roots[i]);for(unsigned i=0;i<16;i++)assert(aliases[i]);
 return 0;
}
'''


class DrawEnvironmentTransportTest(unittest.TestCase):
    def test_actual_sdk_entries_with_live_callbacks_and_cache_aliases(self):
        owners = source_owners(ROOT / 'src/main/PAL')
        bodies = '\n'.join(definition_source(owners[name].read_text(), name)
                           for name in ('SetDefDrawEnv', 'PutDrawEnv', 'DrawOTagEnv', 'GetDrawEnv'))
        with TemporaryDirectory() as directory:
            source = Path(directory) / 'transport.c'
            executable = source.with_suffix('')
            source.write_text(PREFIX + bodies + TAIL)
            # The driver retains its existing cross-signature operation-pointer
            # transport. This fixture compares it but never invokes GPU ops.
            result = subprocess.run(['cc', '-std=c11', '-O2', '-Wall', '-Wextra', '-Werror',
                                     '-Wno-unused-parameter', '-Wno-error=cast-function-type-mismatch',
                                     '-fsanitize=undefined,bounds',
                                     '-fno-sanitize-recover=all', '-I', str(ROOT / 'include'),
                                     str(source), str(ROOT / 'src/native/gpu_address.c'),
                                     '-o', str(executable)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            result = subprocess.run([str(executable)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)


if __name__ == '__main__':
    unittest.main()
