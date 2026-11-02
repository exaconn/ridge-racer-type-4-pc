"""Stream completion/restart against a byte/state protocol oracle, under UBSan.

Restart and notification services are simulated, with mutable roots and pitch.
The new C body and its native-width stream-record layout are used directly.
"""
from pathlib import Path
from tempfile import TemporaryDirectory
import re, subprocess, unittest
ROOT=Path(__file__).resolve().parents[2]

class ChannelStreamEndTest(unittest.TestCase):
 def test_signed_byte_boundaries_and_callback_mutations(self):
  body=(ROOT/'src/main/PAL/func_800833A0.c').read_text()
  body=re.sub(r'^#include[^\n]*\n','',body,flags=re.M)
  body=re.sub(r'__asm__\(""[^;]*;', '',body)
  code=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "rr/layout.h"
#undef RR_ASSERT_SIZE
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_SIZE(t,n)
#define RR_ASSERT_OFFSET(t,f,n)
#include "rr/channel_stream.h"
#define BANKS 2
#define ROWS 3
typedef struct {u32 guard[4];RrChannelStreamB0 records[2][BANKS][ROWS];
 u32 tail[4];RrChannelStreamB0 *slots[BANKS];} World;
static World actual,expected;
static u8 buffers[5][64];
typedef struct {World state;s32 first,second;unsigned kind;} Event;
static Event events[3];static unsigned eventCount,eventIndex,mutation,notifications;
static u32 seed=0x833a0;
static u32 next(void){u32 x=seed;x^=x<<13;x^=x>>17;x^=x<<5;return seed=x;}
static s32 signed32(u32 x){s32 y;memcpy(&y,&x,4);return y;}
static s16 signed16(u16 x){s16 y;memcpy(&y,&x,2);return y;}
static int signedByte(unsigned x){return x<128?(int)x:(int)x-256;}
static void compare(const World *a,const World *e){unsigned i;
 assert(!memcmp(a->guard,e->guard,sizeof(a->guard)));
 assert(!memcmp(a->records,e->records,sizeof(a->records)));
 assert(!memcmp(a->tail,e->tail,sizeof(a->tail)));
 for(i=0;i<BANKS;i++)assert((uintptr_t)a->slots[i]-(uintptr_t)&actual==(uintptr_t)e->slots[i]-(uintptr_t)&expected);
}
static void snapshot(unsigned kind,s32 first,s32 second){Event *e;assert(eventCount<3);e=&events[eventCount++];
 memcpy(&e->state,&expected,sizeof(expected));e->kind=kind;e->first=first;e->second=second;}
static void enter(unsigned kind,s32 first,s32 second){Event *e;assert(eventIndex<eventCount);e=&events[eventIndex++];
 assert(e->kind==kind&&e->first==first&&e->second==second);compare(&actual,&e->state);}
/* Known reset protocol, simulated through the typed full pointer owner.
 * Root changes and pitch writes model effects observable through callbacks. */
static void restart(World *w,s32 bank,s32 row){RrChannelStreamB0 *r;
 assert(bank>=0&&bank<BANKS&&row>=0&&row<ROWS);r=&w->slots[bank][row];
 r->duration20=1;r->elapsed21=0;r->mode98=(r->mode98&~(0x100|8|2|4|0x200))|1;
 r->active14=1;r->cursor00=r->start04;
 if(mutation&1)w->slots[bank]=w->records[1][bank];
}
static void notify(World *w,s32 packed){unsigned b=(u32)packed&255,row=((u32)packed>>8)&255;
 assert(b<BANKS&&row<ROWS);notifications++;
 if(mutation&2)w->slots[b]=w->records[(notifications>>1)&1][b];
 if(mutation&4){w->records[0][b][row].pitch54=signed16((u16)(0x8123U+notifications*97));
  w->records[0][b][row].state1C=(u8)(17+notifications);}
}
#define D_8010F068 (actual.slots)
void func_80083994(s32 bank,s16 row){enter(0,bank,row);restart(&actual,bank,row);}
void func_80089E1C(s32 packed){enter(1,packed,0);notify(&actual,packed);}
'''+body+r'''
static unsigned branches[8],mutations[8];
static void model(unsigned bank,unsigned row){RrChannelStreamB0 *r=&expected.slots[bank][row];
 unsigned elapsed=((unsigned)r->elapsed21+1)%256;int duration=r->duration20;
 s32 packed=(s32)(bank+256*row);r->elapsed21=(u8)elapsed;
 if(!duration){r->accumulator88=0;r->state1C=0;r->accumulator90=0;
  r->cursor00=(r->mode98&1024)?r->alternate0C:r->start04;branches[0]++;
 }else if(signedByte(elapsed)<duration){r->accumulator88=0;r->state1C=0;r->accumulator90=0;
  r->cursor00=(r->mode98&1024)?r->alternate0C:r->start04;r->saved08=r->cursor00;branches[1]++;
 }else{r->mode98=(r->mode98&~11)|516;r->active14=0;
  r->saved08=(r->mode98&1024)?r->alternate0C:r->start04;
  if(r->restartBank22!=-1){snapshot(0,r->restartBank22,r->restartRow23);restart(&expected,r->restartBank22,r->restartRow23);
   snapshot(1,packed,0);notify(&expected,packed);branches[2]++;
  }else branches[3]++;
  snapshot(1,packed,0);notify(&expected,packed);r->accumulator90=r->pitch54;
 }
 if(elapsed==0)branches[4]++;if(elapsed==128)branches[5]++;if(duration<0)branches[6]++;if(mutation)branches[7]++;
}
int main(void){unsigned n,i,bank,row,set,b,r;uint64_t cases=0;
 _Static_assert(offsetof(RrChannelStreamB0,start04)==sizeof(void*),"start pointer");
 _Static_assert(offsetof(RrChannelStreamB0,marker10)==4*sizeof(void*),"marker pointer");
 _Static_assert(offsetof(RrChannelStreamB0,active14)==5*sizeof(void*),"full native pointer prefix");
 _Static_assert(offsetof(RrChannelStreamB0,duration20)==32+5*(sizeof(void*)-4),"signed duration");
 _Static_assert(offsetof(RrChannelStreamB0,mode98)==152+5*(sizeof(void*)-4),"mode word");
 _Static_assert(sizeof(RrChannelStreamB0)==200,"complete LP64 row and tail alignment");
 assert((uintptr_t)&actual>UINT32_MAX);assert((uintptr_t)buffers>UINT32_MAX);
 for(n=0;n<262144;n++){
  unsigned char *raw=(unsigned char*)&actual;
  for(i=0;i<sizeof(actual);i++)raw[i]=(u8)next();
  for(set=0;set<2;set++)for(b=0;b<BANKS;b++)for(r=0;r<ROWS;r++){
   RrChannelStreamB0 *p=&actual.records[set][b][r];
   p->cursor00=buffers[0]+next()%64;p->start04=buffers[1]+next()%64;p->saved08=buffers[2]+next()%64;
   p->alternate0C=buffers[3]+next()%64;p->marker10=buffers[4]+next()%64;
   if((next()&7)==0)p->start04=0;if((next()&7)==0)p->alternate0C=0;
   p->restartBank22=-1;p->restartRow23=(s8)next();
  }
  for(b=0;b<BANKS;b++)actual.slots[b]=actual.records[0][b];
  bank=(n/3)%BANKS;row=n%ROWS;mutation=next()&7;mutations[mutation]++;
  {RrChannelStreamB0 *p=&actual.records[0][bank][row];
   p->duration20=(s8)signedByte((n>>8)&255);p->elapsed21=(u8)n;
   p->mode98=(p->mode98&~1024)|((n>>16)&1)*1024;
   p->pitch54=signed16((u16)n);
   if(n&0x20000){p->restartBank22=(s8)(next()%BANKS);p->restartRow23=(s8)(next()%ROWS);}
  }
  memcpy(&expected,&actual,sizeof(actual));for(b=0;b<BANKS;b++)expected.slots[b]=expected.records[0][b];
  eventCount=eventIndex=notifications=0;model(bank,row);notifications=0;
  func_800833A0(signed32((next()&0xffff0000U)|bank),signed32((next()&0xffff0000U)|row));
  assert(eventCount==eventIndex);compare(&actual,&expected);cases++;
 }
 for(i=0;i<8;i++){assert(branches[i]);assert(mutations[i]);}printf("%llu cases\n",(unsigned long long)cases);return 0;
}
'''
  with TemporaryDirectory() as d:
   p=Path(d);(p/'test.c').write_text(code)
   r=subprocess.run(['clang','-std=gnu11','-O2','-fsanitize=undefined','-fno-sanitize-recover=all','-I'+str(ROOT/'include'),str(p/'test.c'),'-o',str(p/'test')],capture_output=True,text=True)
   self.assertEqual(r.returncode,0,r.stderr)
   r=subprocess.run([str(p/'test')],capture_output=True,text=True)
   self.assertEqual(r.returncode,0,r.stderr)
   self.assertEqual(r.stdout.strip(),'262144 cases')

if __name__=='__main__':unittest.main()
