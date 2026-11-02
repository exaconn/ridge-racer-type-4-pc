from pathlib import Path
from tempfile import TemporaryDirectory
import re,subprocess,sys,os,json,hashlib
root=next(p for p in Path(__file__).resolve().parents if (p/'tools/scripts/cc.sh').is_file());stage=Path(__file__).resolve().parent
path=Path(sys.argv[1]) if len(sys.argv)>1 else root/'src/main/PAL/func_8005AC0C.c'
body=path.read_text();body=body[body.index('void func_8005AC0C'):];body=re.sub(r' __asm__\("\$\d+"\)','',body)
src=r'''
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "rr/game_motion.h"
_Static_assert(sizeof(RrMotionVector8)==8,"complete snapshot");
_Static_assert(sizeof(RrMotionKey32)==32,"complete key");
typedef struct {u32 guard[4];RrMotionControllerPrefix controller;u32 end[4];} State;
static State actual,reference;static RrMotionKey32 keys[4];static RrMotionClock20 clock;
RrMotionControllerPrefix *D_800ACA28;
''' + body + r'''
static s32 word(u32 u){s32 s;memcpy(&s,&u,4);return s;}
static s16 half(u16 u){s16 s;memcpy(&s,&u,2);return s;}
static s32 product(s32 a,s32 b){return word((u32)((uint64_t)(u32)a*(u32)b));}
static s32 diff(s32 a,s32 b){return word((u32)a-(u32)b);}
static s32 divide(s32 n,s16 d){assert(d&&!(n==INT32_MIN&&d==-1));return (s32)((int64_t)n/d);}
static s32 arithmetic8(s32 s){return (s32)((int64_t)s/256-(s<0&&((u32)s&255)!=0));}
static int valid(const RrMotionTrack44 *t,const RrMotionClock20 *c){if(t->disabled||t->state!=1)return 1;const RrMotionKey32 *k=t->key;return t->duration&&!(t->duration==-1&&product(diff(k->toScale,k->fromScale),t->elapsed)==INT32_MIN);}
static void expected(RrMotionControllerPrefix *p,const RrMotionClock20 *c){RrMotionTrack44 *t=&p->track;RrMotionKey32 *k=t->key;if(t->disabled)return;
 switch(t->state){case 0:if(!c->active)return;memcpy(&t->position,&k->from,8);t->scale=k->fromScale;{s32 unit=(arithmetic8(k->timeCode)*12+(k->timeCode&255))*2;u32 n=(u32)((uint64_t)c->tempo*(u32)unit);t->elapsed=0;t->duration=half(n/1200);}t->state=word((u32)t->state+1);
 case 1:{s32 n=t->elapsed; t->position.x=half((u16)k->from.x+(u32)divide(product((s32)k->to.x-k->from.x,n),t->duration));t->position.y=half((u16)k->from.y+(u32)divide(product((s32)k->to.y-k->from.y,n),t->duration));t->position.z=half((u16)k->from.z+(u32)divide(product((s32)k->to.z-k->from.z,n),t->duration));s32 step=divide(product(diff(k->toScale,k->fromScale),t->elapsed),t->duration);t->elapsed=half((u16)t->elapsed+1);t->scale=word((u32)step+(u32)k->fromScale);if(t->elapsed>t->duration){t->position.x=k->to.x;t->position.y=k->to.y;t->position.z=k->to.z;t->scale=k->toScale;t->state=k->continueAfter?2:3;if(k->continueAfter)t->velocity=800;}break;}
 case 2:{s32 v=t->velocity;t->scale=word((u32)t->scale+(u32)v);t->velocity=(s32)((int64_t)product(v,31)/32);t->position.x=half((u16)t->position.x+16);t->position.z=half((u16)t->position.z+8);t->position.y=half((u16)t->position.y+8);break;}}
}
int main(void){unsigned cases=0,states[6]={0};for(unsigned i=0;i<1048576;i++){u32 seed=i*0x9e3779b9U;memset(&actual,0xA5,sizeof(actual));memset(&clock,0x5A,sizeof(clock));RrMotionTrack44 *t=&actual.controller.track;RrMotionKey32 *k=&keys[i%4];k->from=(RrMotionVector8){half(seed),half(seed>>9),half(seed>>17),half(~seed)};k->to=(RrMotionVector8){half(~seed),half(seed>>3),half(seed>>11),half(seed>>1)};k->fromScale=word(seed);k->toScale=word(seed^0x80000000U);k->continueAfter=i&1;k->timeCode=word(seed);clock.active=(s16)((i>>1)&1);clock.tempo=seed^0x5A5A5A5AU;t->key=k;t->state=(s32)(i%6)-1;t->disabled=((i>>3)&7)==0;t->elapsed=half(i);t->duration=half(seed>>16);t->velocity=word(seed);t->scale=word(~seed);
 if(t->state==0&&clock.active&&!t->disabled){s32 unit=(arithmetic8(k->timeCode)*12+(k->timeCode&255))*2;if(!half((u32)((uint64_t)clock.tempo*(u32)unit)/1200))continue;}
 if(!valid(t,&clock))continue;reference=actual;RrMotionClock20 oldClock=clock;RrMotionKey32 oldKey=*k;D_800ACA28=&actual.controller;assert((uintptr_t)D_800ACA28>UINT32_MAX&&(uintptr_t)k>UINT32_MAX);expected(&reference.controller,&clock);func_8005AC0C(&clock);assert(!memcmp(&actual,&reference,sizeof(actual)));assert(!memcmp(&clock,&oldClock,sizeof(clock))&&!memcmp(k,&oldKey,sizeof(*k)));states[i%6]++;cases++;}for(unsigned j=0;j<6;j++)assert(states[j]);printf("native cases %u; all states covered\n",cases);}
'''
with TemporaryDirectory() as td:
 p=Path(td);(p/'test.c').write_text(src);subprocess.run(['cc','-std=gnu11','-O2','-fsanitize=address,undefined','-fno-sanitize-recover=all','-I'+str(root/'include'),'-I'+str(stage),str(p/'test.c'),'-o',str(p/'test')],check=True);subprocess.run([str(p/'test')],check=True)

if os.getenv('RR_AUDIT_JSON'):
 Path(os.environ['RR_AUDIT_JSON']).write_text(json.dumps(dict(source=str(path),source_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),test_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),header_sha256=hashlib.sha256((root/'include/rr/game_motion.h').read_bytes()).hexdigest(),engine='native vs independent arithmetic oracle',serial_cases=1048576,sanitizers=['address','undefined'],full_host_pointers=True,guarded_controller=True),indent=2)+'\n')
