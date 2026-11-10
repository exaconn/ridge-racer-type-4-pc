"""Actual SPU key commands vs an independent mask/cache oracle, under UBSan."""
from pathlib import Path
from tempfile import TemporaryDirectory
import re, subprocess, unittest
ROOT=Path(__file__).resolve().parents[2]

class SpuKeyTest(unittest.TestCase):
 def test_direct_and_queued_masks_with_complete_bank_owners(self):
  from c_functions import definition_source, source_owners
  owner=source_owners(ROOT/'src/main/PAL')['SpuSetKey'].read_text()
  body=definition_source(owner,'SpuSetKey')
  assert 'asm' not in body
  code=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "rr/spu_key.h"
typedef struct {u32 guard[4];SpuRegisterStorage bank;u32 tail[4];} GuardedBank;
typedef struct {u32 guard[4];u16 pending[10];u32 flags,dirty,queued,direct;
 SpuRegisterBank *root;u32 tail[4];} Environment;
static GuardedBank banks[2];static u16 expectedBank[2][256];
static u32 bankGuard[2][4],bankTail[2][4];static Environment actual,expected;
static u32 seed=0x87814;static unsigned paths[10];
static u32 next(void){u32 x=seed;x^=x<<13;x^=x>>17;x^=x<<5;return seed=x;}
static s32 signed32(u32 x){s32 y;memcpy(&y,&x,4);return y;}
#define _spu_RQ (actual.pending)
#define _spu_env (actual.flags)
#define _spu_RQmask (actual.dirty)
#define _spu_RQvoice (actual.queued)
#define _spu_keystat (actual.direct)
#define _spu_RXX (actual.root)
'''+body+r'''
static void model(s32 mode,u32 input){u32 mask=input&0xffffffU;unsigned low=(unsigned)(mask%65536),high=(unsigned)(mask/65536),bank,on,other;
 if(mode!=0&&mode!=1){paths[0]++;return;}
 on=mode==1?0:2;other=2-on;
 if(expected.flags&1){
  expected.pending[on]=(u16)low;expected.pending[on+1]=(u16)high;
  expected.dirty=expected.dirty|1U;
  expected.queued=mode?expected.queued|mask:expected.queued&(0xffffffffU-mask);
  if(expected.pending[other]&low)paths[3]++;else paths[4]++;
  if(expected.pending[other+1]&high)paths[5]++;else paths[6]++;
  expected.pending[other]=(u16)(expected.pending[other]&(65535U-low));
  expected.pending[other+1]=(u16)(expected.pending[other+1]&(65535U-high));
  paths[1]++;
 }else{
  assert(expected.root==&banks[0].bank.registers||expected.root==&banks[1].bank.registers);
  bank=expected.root==&banks[1].bank.registers;
  expectedBank[bank][196+on]=(u16)low;expectedBank[bank][197+on]=(u16)high;
  expected.direct=mode?expected.direct|mask:expected.direct&(0xffffffffU-mask);paths[2]++;
 }
 if(!mask)paths[7]++;if(mode==0)paths[8]++;else paths[9]++;
}
static void compare(void){unsigned b,i;assert(!memcmp(&actual,&expected,sizeof(actual)));
 for(b=0;b<2;b++){
  assert(!memcmp(banks[b].guard,bankGuard[b],sizeof(bankGuard[b])));
  assert(!memcmp(banks[b].tail,bankTail[b],sizeof(bankTail[b])));
  /* Hardware fields are read through their volatile-qualified interface.
   * Oracle storage and metadata snapshots are ordinary RAM objects. */
  for(i=0;i<256;i++)assert(banks[b].bank.halves[i]==expectedBank[b][i]);
 }
}
int main(void){unsigned n,phase,b,i;uint64_t cases=0;s32 mode;u32 mask;u8 high;
 static const u8 highs[6]={0,255,128,1,85,170};
 _Static_assert(sizeof(SpuRegisterStorage)==512,"complete bank owner");
 _Static_assert(sizeof(actual.pending)==20,"complete ten-halfword cache");
 _Static_assert(offsetof(SpuRegisterBank,transfer_address)==0x1a6,"hardware layout");
 assert((uintptr_t)&banks[0].bank>UINT32_MAX);assert((uintptr_t)&actual>UINT32_MAX);
 for(b=0;b<2;b++){
  for(i=0;i<4;i++){banks[b].guard[i]=bankGuard[b][i]=next();banks[b].tail[i]=bankTail[b][i]=next();}
  for(i=0;i<256;i++){u16 value=(u16)next();banks[b].bank.halves[i]=value;expectedBank[b][i]=value;}
 }
 for(n=0;n<589824;n++){
  unsigned char *p=(unsigned char*)&actual;phase=n/65536;
  for(i=0;i<sizeof(actual);i++)p[i]=(u8)next();
  high=phase<6?highs[phase]:(u8)next();mask=(next()&0xff000000U)|((u32)high<<16)|(n&65535);
  if(phase<8){mode=(s32)(phase&1);actual.flags=(next()&~1U)|((phase>>1)&1);}
  else{switch(n&7){case 0:mode=-1;break;case 1:mode=INT32_MIN;break;case 2:mode=INT32_MAX;break;
   case 3:mode=2;break;case 4:mode=65536;break;case 5:mode=-65536;break;default:mode=signed32(next());if(mode==0||mode==1)mode=3;}
   actual.flags=next();}
  actual.root=&banks[(n>>4)&1].bank.registers;
  if(phase==8||((actual.flags&1)&&(n&8)))actual.root=0;
  memcpy(&expected,&actual,sizeof(actual));model(mode,mask);SpuSetKey(mode,mask);compare();cases++;
 }
 for(i=0;i<10;i++)assert(paths[i]);printf("%llu cases\n",(unsigned long long)cases);return 0;
}
'''
  with TemporaryDirectory() as d:
   p=Path(d);(p/'test.c').write_text(code)
   r=subprocess.run(['clang','-std=gnu11','-O2','-fsanitize=undefined','-fno-sanitize-recover=all','-I'+str(ROOT/'include'),str(p/'test.c'),'-o',str(p/'test')],capture_output=True,text=True)
   self.assertEqual(r.returncode,0,r.stderr)
   r=subprocess.run([str(p/'test')],capture_output=True,text=True)
   self.assertEqual(r.returncode,0,r.stderr)
   self.assertEqual(r.stdout.strip(),'589824 cases')

if __name__=='__main__':unittest.main()
