"""SPU tick selector against an independent integer/state model and real TV getter."""
from pathlib import Path
from tempfile import TemporaryDirectory
import re, subprocess, unittest
ROOT=Path(__file__).resolve().parents[2]

class SpuTickTest(unittest.TestCase):
 def test_modes_system_words_and_complete_timer_owner(self):
  body=(ROOT/'tools/scripts/fixtures/views/func_80084004.c').read_text()
  body=re.sub(r'^#include[^\n]*\n','',body,flags=re.M)
  body=re.sub(r' __asm__\("\$\d+"\)','',body)
  getter=(ROOT/'tools/scripts/fixtures/views/func_8008BDF8.c').read_text()
  getter=re.sub(r'^#include[^\n]*\n|^extern[^\n]*\n','',getter,flags=re.M).replace('GetVideoMode','actual_system')
  code=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>
#include "rr/layout.h"
#undef RR_ASSERT_SIZE
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_SIZE(t,n)
#define RR_ASSERT_OFFSET(t,f,n)
#include "rr/spu_tick.h"
typedef struct {u32 guard[4];RrSpuTickState20 timer;u32 tail[4];s32 rate,system;} Environment;
static Environment actual,expected;
static unsigned calls;
static u32 seed=0x84004;
static u32 next(void){u32 n=seed;n^=n<<13;n^=n>>17;n^=n<<5;return seed=n;}
static s32 signed_bits(u32 n){s32 r;memcpy(&r,&n,4);return r;}
static void callback0(void){}static void callback1(void){}
#define _snd_seq_tick_env (actual.timer)
#define VBLANK_MINUS (actual.rate)
#define D_800A63D0 (actual.system)
'''+getter+r'''
s32 GetVideoMode(void){calls++;assert(calls==1);assert(!memcmp(&actual,&expected,sizeof(actual)));return actual_system();}
'''+body+r'''
static void model(s32 mode){s32 selected;unsigned flag=((u32)mode/4096U)&1U;
 selected=flag?(s32)((u32)mode%4096U):mode;
 expected.timer.active=(s32)flag;expected.timer.selector=selected;
 if(selected>=6){expected.rate=selected;return;}
 if(selected<0){expected.rate=60;return;}
 switch(selected){
 case 0:case 5:expected.rate=expected.system==1?50:60;break;
 case 1:expected.rate=60;expected.timer.selector=expected.system==0?5:60;break;
 case 2:expected.rate=240;break;
 case 3:expected.rate=120;break;
 case 4:expected.rate=50;expected.timer.selector=expected.system==1?5:50;break;
 }
}
int main(void){u32 n,phase,i;uint64_t cases=0;unsigned flags[2]={0},selectors[8]={0},systems[4]={0};s32 mode,selected;
 _Static_assert(sizeof(RrSpuTickFlags4)==4,"flag owner");
 _Static_assert(offsetof(RrSpuTickState20,active)==4,"active alias");
 _Static_assert(offsetof(RrSpuTickState20,callback)==8,"callback");
 _Static_assert(offsetof(RrSpuTickState20,previous)==8+sizeof(void*),"native callback stride");
 _Static_assert(offsetof(RrSpuTickState20,flags)==8+2*sizeof(void*),"native flags");
 _Static_assert(sizeof(RrSpuTickState20)==24+2*sizeof(void*),"complete native timer");
 _Static_assert(sizeof(unsigned long)>=sizeof(void*),"address width");
 assert((uintptr_t)&actual.timer>UINT32_MAX);assert((uintptr_t)callback0>UINT32_MAX);
 for(n=0;n<65536;n++)for(phase=0;phase<10;phase++){
  memset(&actual,0,sizeof(actual));
  for(i=0;i<4;i++){actual.guard[i]=next();actual.tail[i]=next();}
  actual.timer.selector=signed_bits(next());actual.timer.active=signed_bits(next());for(i=0;i<3;i++)actual.timer.state[i]=signed_bits(next());
  actual.timer.callback=(n&1)?callback0:callback1;actual.timer.previous=(n&2)?callback1:callback0;
  {u8 *f=(u8*)&actual.timer.flags;for(i=0;i<4;i++)f[i]=(u8)next();}
  actual.rate=signed_bits(next());
  if(phase<2)mode=(s32)n;else if(phase<4)mode=signed_bits(0xFFFF0000U|n);else mode=signed_bits((next()&0xFFFF0000U)|n);
  switch(phase){case 0:case 2:actual.system=0;break;case 1:case 3:actual.system=1;break;case 4:actual.system=INT32_MIN;break;case 5:actual.system=INT32_MAX;break;case 6:actual.system=-1;break;case 7:actual.system=2;break;case 8:actual.system=(s16)n;break;default:actual.system=signed_bits(next());}
  flags[((u32)mode>>12)&1]++;selected=(mode&4096)?mode&4095:mode;selectors[selected<0?6:selected<6?(unsigned)selected:7]++;
  systems[actual.system==0?0:actual.system==1?1:actual.system<0?2:3]++;
  expected=actual;calls=0;SsSetTickMode(mode);assert(calls==1);model(mode);
  assert(!memcmp(&actual,&expected,sizeof(actual)));cases++;
 }
 assert(cases==655360);for(i=0;i<2;i++)assert(flags[i]);for(i=0;i<8;i++)assert(selectors[i]);for(i=0;i<4;i++)assert(systems[i]);return 0;
}
'''
  with TemporaryDirectory() as tmp:
   p=Path(tmp);(p/'test.c').write_text(code)
   r=subprocess.run(['cc','-std=gnu11','-O2','-fsanitize=undefined','-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(p/'test.c'),'-o',str(p/'test')],capture_output=True,text=True)
   self.assertEqual(r.returncode,0,r.stderr);subprocess.run([str(p/'test')],check=True)
if __name__=='__main__':unittest.main()
