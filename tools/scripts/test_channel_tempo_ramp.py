"""Actual tempo ramp vs independent wrapped-integer state oracle, under UBSan."""
from pathlib import Path
from tempfile import TemporaryDirectory
import re, subprocess, unittest
ROOT=Path(__file__).resolve().parents[2]

class ChannelTempoRampTest(unittest.TestCase):
 def test_signed_intervals_wrap_and_full_stream_owners(self):
  body=(ROOT/'build/legacy/func_80084314.c').read_text()
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
#undef RR_ASSERT_SIZE
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_SIZE(t,n)
#define RR_ASSERT_OFFSET(t,f,n)
#include "rr/channel_stream.h"
#define BANKS 32
#define ROWS 8
typedef struct {u32 guard[4];RrChannelStreamB0 records[BANKS][ROWS];
 RrChannelStreamB0 *slots[BANKS];u32 rate;u32 tail[4];} World;
static World actual,expected;static u8 buffers[5][64];
static u32 seed=0x84314;static unsigned paths[14];
static u32 next(void){u32 x=seed;x^=x<<13;x^=x>>17;x^=x<<5;return seed=x;}
static s32 signed32(u32 x){s32 y;memcpy(&y,&x,4);return y;}
static s16 signed16(u16 x){s16 y;memcpy(&y,&x,2);return y;}
static s32 wrap(int64_t x){return signed32((u32)x);}
#define D_8010F068 (actual.slots)
#define D_8010F064 (actual.rate)
'''+body+r'''
static void model(unsigned bank,unsigned row){RrChannelStreamB0 *r=&expected.records[bank][row];
 s32 left=wrap((int64_t)r->tempoRemainingA8-1);int step=r->interval4E;u32 tempo=r->tempo94,target=r->targetTempoAC;
 uint64_t product,dividend,denominator,quotient;s32 pitch;r->tempoRemainingA8=left;
 if(left<0){r->mode98&=~192;paths[0]++;return;}
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
 r->tempo94=tempo;
 product=(u32)((int64_t)r->scale50*(int64_t)tempo);
 dividend=(u32)(product*10);denominator=(u32)((uint64_t)expected.rate*60);assert(denominator);
 quotient=dividend/denominator;pitch=(quotient&32768)?(s32)(quotient&65535)-65536:(s32)(quotient&65535);
 if(pitch<1){pitch=1;paths[8]++;}else paths[9]++;
 r->pitch54=(s16)pitch;
 if(!left||tempo==target){r->mode98&=~192;paths[10]++;}else paths[11]++;
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
 _Static_assert(sizeof(RrChannelStreamB0)==200,"native full owner");
 _Static_assert(offsetof(RrChannelStreamB0,interval4E)==78+5*(sizeof(void*)-4),"signed interval");
 _Static_assert(offsetof(RrChannelStreamB0,scale50)==80+5*(sizeof(void*)-4),"signed scale");
 _Static_assert(offsetof(RrChannelStreamB0,tempo94)==148+5*(sizeof(void*)-4),"tempo");
 _Static_assert(offsetof(RrChannelStreamB0,tempoRemainingA8)==168+5*(sizeof(void*)-4),"remaining");
 _Static_assert(offsetof(RrChannelStreamB0,targetTempoAC)==172+5*(sizeof(void*)-4),"target");
 assert((uintptr_t)&actual>UINT32_MAX);assert((uintptr_t)buffers>UINT32_MAX);
 {unsigned char *p=(unsigned char*)&actual;for(i=0;i<sizeof(actual);i++)p[i]=(u8)next();}
 for(b=0;b<BANKS;b++){
  actual.slots[b]=actual.records[b];
  for(row=0;row<ROWS;row++){RrChannelStreamB0 *r=&actual.records[b][row];
   r->cursor00=buffers[0]+row;r->start04=buffers[1]+b;r->saved08=buffers[2]+row;
   r->alternate0C=buffers[3]+b;r->marker10=buffers[4]+row;}
 }
 for(n=0;n<524288;n++){
  RrChannelStreamB0 *r;s32 remaining;int interval,willDivide;
  b=n%BANKS;row=(n/BANKS)%ROWS;phase=n/65536;r=&actual.records[b][row];
  interval=signed16((u16)n);r->interval4E=(s16)interval;r->scale50=signed16((u16)(n/8));
  r->mode98=signed32(next());r->pitch54=signed16((u16)next());r->tempo94=next();r->targetTempoAC=next();
  actual.rate=(n&1)?next():rates[(n>>1)%9];
  switch(phase){
  case 0:r->tempoRemainingA8=0;break;
  case 1:r->tempoRemainingA8=INT32_MIN;break;
  case 2:r->tempoRemainingA8=INT32_MAX;break;
  case 3:r->tempoRemainingA8=1;break;
  case 4:r->tempoRemainingA8=signed32(next());break;
  case 5:r->tempoRemainingA8=interval>0?interval*(s32)(next()%4)+1:2;break;
  case 6:r->tempoRemainingA8=interval>1?interval+2:17;break;
  default:r->tempoRemainingA8=2;r->targetTempoAC=r->tempo94;break;
  }
  if(phase==3||phase==5){r->tempo94=edges[(n>>3)&7];r->targetTempoAC=edges[(n>>6)&7];}
  remaining=wrap((int64_t)r->tempoRemainingA8-1);
  willDivide=remaining>=0&&(interval<=0||remaining%interval==0);
  /* The original traps if the executed unsigned division has denominator0.
   * Keep zero/wrapped-zero rates in expiry and skipped-interval paths. */
  if(willDivide&&(u32)((uint64_t)actual.rate*60)==0)actual.rate=1;
  memcpy(&expected,&actual,sizeof(actual));for(i=0;i<BANKS;i++)expected.slots[i]=expected.records[i];
  model(b,row);
  func_80084314(signed32((next()&0xffff0000U)|b),signed32((next()&0xffff0000U)|row));
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
