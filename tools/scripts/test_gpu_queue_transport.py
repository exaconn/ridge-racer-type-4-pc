"""Current queue bodies with real native records and an independent service model.

IRQ masking, polling and draining are scripted services; hardware concurrency and
the backend operations' legacy unspecified-argument ABI are outside this test.
"""
from pathlib import Path
import subprocess
from tempfile import TemporaryDirectory

from c_functions import definition_source, source_owners

ROOT = Path(__file__).resolve().parents[2]
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "psyq/libgpu/sys_drain.h"
#include "rr/interrupts.h"
typedef struct {u32 kind,head,tail,hash;uint64_t first,second;} Event;
typedef struct {
 u8 before[16];GpuResetState80 state;struct GpuQueue queue[64];u32 payload[32];
 s32 head,tail,saved,mask;u32 dma,status;unsigned calls,drains;Event events[32];u8 after[16];
} World;
static World actual,expected,*g;static unsigned scenario;static u8 parameterBytes[64];
#define GpuResetState g->state
#define GpuCommandQueue g->queue
#define _qin g->head
#define _qout g->tail
#define GpuQueueSavedIntrMask g->saved
#define DMA2_CHCR (&g->dma)
#define GPU_STATUS (&g->status)
static int op0(void*,unsigned long),op1(void*,unsigned long);
static void completion(void){assert(!"queue publication fixture does not run completion");}
static u32 seed=0x94BC4U;
static u32 next(void){seed=seed*1664525U+1013904223U;return seed;}
static unsigned offset(const void *p){uintptr_t n=(uintptr_t)p-(uintptr_t)g;assert(n<sizeof(*g));return (unsigned)n;}
static void hashword(u32 *h,uint64_t value){unsigned i;for(i=0;i<8;i++)*h=(*h^(u8)(value>>(i*8)))*16777619U;}
static u32 hash(void){
 u32 h=2166136261U;unsigned i,j;
 for(i=0;i<sizeof(g->state);i++)hashword(&h,((u8*)&g->state)[i]);
 for(i=0;i<64;i++){
  struct GpuQueue *q=&g->queue[i];assert(q->op==op0||q->op==op1);
  hashword(&h,q->op==op1);hashword(&h,offset(q->p0));hashword(&h,q->p1);
  for(j=0;j<21;j++)hashword(&h,(u32)q->buf[j]);
 }
 for(i=0;i<32;i++)hashword(&h,g->payload[i]);
 hashword(&h,(u32)g->saved);hashword(&h,(u32)g->mask);hashword(&h,g->dma);hashword(&h,g->status);return h;
}
static void event(u32 kind,uint64_t first,uint64_t second){
 unsigned n=g->calls++;assert(n<32);Event *e=&g->events[n];
 e->kind=kind;e->head=g->head;e->tail=g->tail;e->hash=hash();e->first=first;e->second=second;
}
static int operation(unsigned which,void *payload,unsigned long parameter){
 unsigned where=offset(payload);event(7+which,where,parameter);
 assert((uintptr_t)payload>UINT32_MAX);*(u32*)payload^=(u32)parameter+which*0x17171717U;
 ((u8*)&g->state.draw10)[(scenario+which)%sizeof(g->state.draw10)]^=(u8)scenario;return -(int)which;
}
static int op0(void *p,unsigned long n){return operation(0,p,n);}
static int op1(void *p,unsigned long n){return operation(1,p,n);}
void set_alarm(void){event(1,0,0);((u8*)&g->state.display6C)[scenario%20]^=(u8)(scenario>>3);}
s32 get_alarm(void){event(2,0,0);return (scenario&2048)!=0;}
s32 SetIntrMask(s32 value){s32 previous=g->mask;event(3,(u32)value,0);g->mask=value&65535;return previous;}
int _exeque(void){
 event(4,0,0);g->drains++;
 if(((g->head+1)&63)==g->tail){g->tail=(g->tail+1)&63;return 1;}
 if((scenario&256)&&g->tail!=g->head){
  struct GpuQueue *q=&g->queue[g->tail];q->op(q->p0,q->p1);g->tail=(g->tail+1)&63;
 }
 return (g->head-g->tail)&63;
}
LibInterruptCallback DMACallback(s32 channel,LibInterruptCallback callback){
 assert(channel==2&&callback==(LibInterruptCallback)_exeque);event(5,(u32)channel,0);
 if(scenario&128)g->head=(g->head+1)&63;g->payload[(scenario>>2)&31]^=scenario*0x1717U;return 0;
}
'''
TAIL = r'''
/* Model the publication steps and copy bytes independently of the C body. */
static int reference(GpuOpFunc operation,void *source,int length,unsigned long parameter){
 int old;unsigned i,n;
 set_alarm();
 for(;;){if((unsigned)((g->head+1)&63)!=(unsigned)g->tail)break;if(get_alarm())return -1;_exeque();}
 old=SetIntrMask(0);g->saved=old;g->state.pending08=1;
 if(!g->state.enabled01||(g->head==g->tail&&!(g->dma&0x01000000)&&!g->state.completion0C)){
  assert(g->status&0x04000000);operation(source,parameter);SetIntrMask(g->saved);return 0;
 }
 DMACallback(2,(LibInterruptCallback)_exeque);
 n=(unsigned)length/4;
 for(i=0;i<n;i++){u32 captured;memcpy(&captured,(u8*)source+i*4,4);memcpy(&g->queue[g->head].buf[i],&captured,4);}
 g->queue[g->head].p0=length?(void*)g->queue[g->head].buf:source;
 g->queue[g->head].p1=parameter;g->queue[g->head].op=operation;
 g->head=(g->head+1)&63;SetIntrMask(g->saved);_exeque();return (g->head-g->tail)&63;
}
static void initialize(unsigned number){
 unsigned i,j,mode=number>>12;memset(&actual,0xA5,sizeof(actual));g=&actual;
 for(i=0;i<sizeof(g->state);i++)((u8*)&g->state)[i]=(u8)next();
 for(i=0;i<64;i++){
  struct GpuQueue *q=&g->queue[i];q->op=(i&1)?op1:op0;
  q->p0=(i&2)?(void*)q->buf:(void*)&g->payload[i&15];
  q->p1=(i&1)?(unsigned long)(parameterBytes+(i&31)):next();
  for(j=0;j<21;j++)q->buf[j]=(s32)next();
 }
 for(i=0;i<32;i++)g->payload[i]=next();
 g->head=number&63;g->tail=(number>>6)&63;g->saved=(s32)next();g->mask=next()&65535;
 g->dma=mode==1?0x01000000:0;g->status=0x04000000;
 g->state.enabled01=mode!=0;g->state.completion0C=mode==3?completion:0;
 g->calls=g->drains=0;memset(g->events,0,sizeof(g->events));
 expected=actual;
 for(i=0;i<64;i++)expected.queue[i].p0=(u8*)&expected+offset(actual.queue[i].p0);
}
static void *input(unsigned number,int length){
 unsigned which=(number>>3)%3;
 if(!which)return g->payload+((number>>4)&7);
 return g->queue[which==1?(unsigned)g->head:((unsigned)g->head+17)&63].buf+((length<=80)&&(number&32));
}
static void compare(void){
 unsigned i;g=&actual;u32 a=hash();g=&expected;assert(a==hash());
 assert(actual.head==expected.head&&actual.tail==expected.tail&&actual.saved==expected.saved&&actual.mask==expected.mask);
 assert(actual.drains==expected.drains&&actual.calls==expected.calls);
 assert(!memcmp(actual.events,expected.events,actual.calls*sizeof(Event)));
 for(i=0;i<64;i++){
  struct GpuQueue *a=&actual.queue[i],*b=&expected.queue[i];
  assert(a->op==b->op&&a->p1==b->p1);
  assert((uintptr_t)a->p0-(uintptr_t)&actual==(uintptr_t)b->p0-(uintptr_t)&expected);
  assert(!memcmp(a->buf,b->buf,sizeof(a->buf)));
  for(unsigned j=offsetof(struct GpuQueue,buf)+sizeof(a->buf);j<sizeof(*a);j++)assert(((u8*)a)[j]==0xA5&&((u8*)b)[j]==0xA5);
 }
 assert(!memcmp(&actual.state,&expected.state,sizeof(actual.state)));
 assert(!memcmp(actual.payload,expected.payload,sizeof(actual.payload)));
 for(i=0;i<16;i++)assert(actual.before[i]==0xA5&&actual.after[i]==0xA5&&expected.before[i]==0xA5&&expected.after[i]==0xA5);
}
int main(void){
 static const int lengths[]={0,1,2,3,4,5,8,12,40,80,83,84};
 unsigned number,seen[12]={0},immediate=0,queued=0,timeouts=0;int length,a,b;unsigned long parameter;
 assert(sizeof(void*)==8&&sizeof(unsigned long)==sizeof(void*));
 assert(sizeof(GpuResetState80)==136&&offsetof(GpuResetState80,completion0C)==16&&offsetof(GpuResetState80,draw10)==24);
 assert((uintptr_t)completion>UINT32_MAX);
 assert(offsetof(struct GpuQueue,p0)==sizeof(void*));
 assert(offsetof(struct GpuQueue,p1)==2*sizeof(void*));
 assert(offsetof(struct GpuQueue,buf)==3*sizeof(void*));
 assert((uintptr_t)&actual>UINT32_MAX&&(uintptr_t)parameterBytes>UINT32_MAX);
 for(number=0;number<16384;number++){
  scenario=number;initialize(number);length=lengths[(number/7)%12];
  if(number&1024)length=0;else seen[(number/7)%12]++;
  parameter=(number&1)?(unsigned long)(parameterBytes+(number&31)):next();
  g=&actual;
  if(number&1024){_addque((number&8)?op1:op0,input(number,length),parameter);a=0;}
  else a=_addque2((number&8)?op1:op0,input(number,length),length,parameter);
  g=&expected;b=reference((number&8)?op1:op0,input(number,length),length,parameter);if(number&1024)b=0;
  assert(a==b);compare();
  for(unsigned i=0;i<actual.calls;i++){immediate+=actual.events[i].kind>=7;queued+=actual.events[i].kind==5;}
  timeouts+=a==-1;
 }
 for(number=0;number<12;number++)assert(seen[number]);assert(immediate&&queued&&timeouts);
 puts("PASS 16384 current-C GPU queue cases; full native payload/parameter pointers, every ring head/tail in four modes, timeout/drain/immediate/queued paths, overlapping payloads, partial-word lengths, both entry points, service protocol, complete records, padding, guards and UBSan");
 return 0;
}
'''


def main():
    owners = source_owners(ROOT/'src/main/PAL')
    bodies = '\n'.join(definition_source(owners[n].read_text(), n) for n in ('_addque', '_addque2'))
    with TemporaryDirectory() as temporary:
        source, binary = Path(temporary)/'native.c', Path(temporary)/'native'
        source.write_text(PREFIX+bodies+TAIL)
        subprocess.run(['cc', '-std=c11', '-O2', '-Wall', '-Wextra', '-Werror=int-conversion',
                        '-Werror=incompatible-pointer-types', '-fsanitize=undefined,bounds',
                        '-fno-sanitize-recover=all', '-I'+str(ROOT/'include'), str(source), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)


if __name__ == '__main__':
    main()
