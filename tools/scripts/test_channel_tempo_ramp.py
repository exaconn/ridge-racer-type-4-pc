"""Actual tempo ramp vs independent wrapped-integer state oracle, under UBSan."""
from pathlib import Path
from tempfile import TemporaryDirectory
import re, subprocess, unittest
from sequence_fixture_transport import current_body
ROOT=Path(__file__).resolve().parents[2]

class ChannelTempoRampTest(unittest.TestCase):
 def test_signed_intervals_wrap_and_full_stream_owners(self):
  body=current_body(ROOT,'80084314')
  body=re.sub(r'^#include[^\n]*\n','',body,flags=re.M)
  body=re.sub(r' __asm__\("\$\d+"\)','',body)
  body=re.sub(r'__asm__\(""[^;]*;', '',body)
  code=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "rr/layout.h"
#include "rr/channel_stream.h"
#define BANKS 32
#define ROWS 8
typedef struct {u32 guard[4];ChannelStreamB0 records[BANKS][ROWS];
 ChannelStreamB0 *slots[BANKS];u32 rate;u32 tail[4];} World;
static World actual,expected;static u8 buffers[5][64];
static u32 seed=0x84314;static unsigned paths[14];
static u32 next(void){u32 x=seed;x^=x<<13;x^=x>>17;x^=x<<5;return seed=x;}
static s32 signed32(u32 x){s32 y;memcpy(&y,&x,4);return y;}
static s16 signed16(u16 x){s16 y;memcpy(&y,&x,2);return y;}
static s32 wrap(int64_t x){return signed32((u32)x);}
#define _ss_score (actual.slots)
#define VBLANK_MINUS (actual.rate)
'''+body+r'''
static void model(unsigned bank,unsigned row){ChannelStreamB0 *r=&expected.records[bank][row];
 s32 left=wrap((int64_t)r->wa8-1);int step=r->h48[3];u32 tempo=r->w94,target=r->wac;
 uint64_t product,dividend,denominator,quotient;s32 pitch;r->wa8=left;
 if(left<0){r->w98&=~192;paths[0]++;return;}
 if(step>0){
  if(left%step){paths[1]++;return;}
  if(tempo<target){tempo=(u32)((uint64_t)tempo+1);paths[2]++;}
  else if(tempo>target){tempo=(u32)((uint64_t)tempo-1);paths[3]++;}
  else paths[4]++;
 }else{
  if(tempo<target){tempo=(u32)((int64_t)tempo-step);paths[5]++;
   if(tempo>target){tempo=target;paths[7]++;}}
  else if(tempo>target){tempo=(u32)((int64_t)tempo+step);paths[6]++;
   if(tempo<target){tempo=target;paths[7]++;}}
  else paths[4]++;
 }
 r->w94=tempo;
 product=(u32)((int64_t)r->h48[4]*(int64_t)tempo);
 dividend=(u32)(product*10);denominator=(u32)((uint64_t)expected.rate*60);assert(denominator);
 quotient=dividend/denominator;pitch=(quotient&32768)?(s32)(quotient&65535)-65536:(s32)(quotient&65535);
 if(pitch<1){pitch=1;paths[8]++;}else paths[9]++;
 r->h48[6]=(s16)pitch;
 if(!left||tempo==target){r->w98&=~192;paths[10]++;}else paths[11]++;
 if(step==0)paths[12]++;if(step==-32768)paths[13]++;
}
static void compare(void){unsigned b;
 assert(!memcmp(actual.guard,expected.guard,sizeof(actual.guard)));
 assert(!memcmp(actual.records,expected.records,sizeof(actual.records)));
 assert(!memcmp(actual.tail,expected.tail,sizeof(actual.tail)));assert(actual.rate==expected.rate);
 for(b=0;b<BANKS;b++)assert(actual.slots[b]==actual.records[b]&&expected.slots[b]==expected.records[b]);
}
int main(void){unsigned n,b,row,i,phase;uint64_t cases=0;unsigned seen[65536]={0};
 static const u32 rates[9]={0,1,50,60,70,0x40000000,0x80000000,0xc0000000,0xffffffff};
 static const u32 edges[8]={0,1,2,127,32768,0x7fffffff,0x80000000,0xffffffff};
 _Static_assert(sizeof(ChannelStreamB0)==200,"native full owner");
 _Static_assert(offsetof(ChannelStreamB0,h48[3])==78+5*(sizeof(void*)-4),"signed interval");
 _Static_assert(offsetof(ChannelStreamB0,h48[4])==80+5*(sizeof(void*)-4),"signed scale");
 _Static_assert(offsetof(ChannelStreamB0,w94)==148+5*(sizeof(void*)-4),"tempo");
 _Static_assert(offsetof(ChannelStreamB0,wa8)==168+5*(sizeof(void*)-4),"remaining");
 _Static_assert(offsetof(ChannelStreamB0,wac)==172+5*(sizeof(void*)-4),"target");
 assert((uintptr_t)&actual>UINT32_MAX);assert((uintptr_t)buffers>UINT32_MAX);
 {unsigned char *p=(unsigned char*)&actual;for(i=0;i<sizeof(actual);i++)p[i]=(u8)next();}
 for(b=0;b<BANKS;b++){
  actual.slots[b]=actual.records[b];
  for(row=0;row<ROWS;row++){ChannelStreamB0 *r=&actual.records[b][row];
   r->c0=buffers[0]+row;r->c4=buffers[1]+b;r->c8=buffers[2]+row;
   r->cC=buffers[3]+b;r->p10=buffers[4]+row;}
 }
 for(n=0;n<524288;n++){
  ChannelStreamB0 *r;s32 remaining;int interval,willDivide;
  b=n%BANKS;row=(n/BANKS)%ROWS;phase=n/65536;r=&actual.records[b][row];
  interval=signed16((u16)n);r->h48[3]=(s16)interval;r->h48[4]=signed16((u16)(n/8));
  r->w98=signed32(next());r->h48[6]=signed16((u16)next());r->w94=next();r->wac=next();
  actual.rate=(n&1)?next():rates[(n>>1)%9];
  switch(phase){
  case 0:r->wa8=0;break;
  case 1:r->wa8=INT32_MIN;break;
  case 2:r->wa8=INT32_MAX;break;
  case 3:r->wa8=1;break;
  case 4:r->wa8=signed32(next());break;
  case 5:r->wa8=interval>0?interval*(s32)(next()%4)+1:2;break;
  case 6:r->wa8=interval>1?interval+2:17;break;
  default:r->wa8=2;r->wac=r->w94;break;
  }
  if(phase==3||phase==5){r->w94=edges[(n>>3)&7];r->wac=edges[(n>>6)&7];}
  remaining=wrap((int64_t)r->wa8-1);
  willDivide=remaining>=0&&(interval<=0||remaining%interval==0);
  /* The original traps if the executed unsigned division has denominator0.
   * Keep zero/wrapped-zero rates in expiry and skipped-interval paths. */
  if(willDivide&&(u32)((uint64_t)actual.rate*60)==0)actual.rate=1;
  memcpy(&expected,&actual,sizeof(actual));for(i=0;i<BANKS;i++)expected.slots[i]=expected.records[i];
  model(b,row);
  _SsSndTempo(signed32((next()&0xffff0000U)|b),signed32((next()&0xffff0000U)|row));
  compare();seen[(u16)interval]++;cases++;
 }
 for(i=0;i<65536;i++)assert(seen[i]==8);for(i=0;i<14;i++)assert(paths[i]);
 printf("%llu cases\n",(unsigned long long)cases);return 0;
}
'''
  with TemporaryDirectory() as d:
   p=Path(d);(p/'test.c').write_text(code)
   r=subprocess.run(['clang','-std=gnu11','-O2','-fsanitize=undefined','-fno-sanitize-recover=all','-I'+str(ROOT/'include'),str(p/'test.c'),'-o',str(p/'test')],capture_output=True,text=True)
   self.assertEqual(r.returncode,0,r.stderr)
   r=subprocess.run([str(p/'test')],capture_output=True,text=True)
   self.assertEqual(r.returncode,0,r.stderr)
   self.assertEqual(r.stdout.strip(),'524288 cases')

if __name__=='__main__':unittest.main()
