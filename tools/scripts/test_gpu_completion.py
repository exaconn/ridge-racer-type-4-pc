"""Actual callback registration, queue draining and display-mask cache ownership.

Independent protocol/byte snapshots cover live service mutations and callback
execution with full native pointers. GPU readiness and interrupts are scripted;
the existing queue operation callback's unspecified-argument ABI remains debt.
"""
from pathlib import Path
import subprocess
from tempfile import TemporaryDirectory
import unittest

from c_functions import definition_source, source_owners

ROOT = Path(__file__).resolve().parents[2]
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "psyq/libgpu/gpuenv.h"
#include "psyq/libgpu/sys_drain.h"
#include "rr/interrupts.h"
#include "rr/driver_control.h"
typedef struct {
 u32 operation,payload;unsigned long parameter;u32 bytes[21];u8 padding[4];
} QueueImage;
typedef struct {
 u8 state[136];QueueImage queue[64];u32 payload[64];
 s32 head,tail,saved,mask;u32 dma,status,root,serial,operations,dmaCalls,completions;
} Image;
typedef struct {u32 kind;unsigned long first,second;Image before;} Event;
typedef struct {
 u32 before[4];GpuResetState80 state;volatile struct GpuQueue queue[64];u32 payload[64];
 s32 head,tail,saved,mask;u32 dma,status,root,serial,operations,dmaCalls,completions,calls;
 Event events[16];u32 after[4];
} World;
static World actual,expected,*g;
static GpuDriver40 drivers[2];
static unsigned scenario,profile;
static u8 parameters[64];
static u32 seenDebug[256],seenHead[64],seenDepth[4],seenPending[4],seenCallbacks[3],seenEvents[12];
static int operation0(void *,unsigned long),operation1(void *,unsigned long);
static void completion0(void),completion1(void);
const char DrawSyncCallbackFmt[]="callback",StrSetDispMaskFmt[]="mask";
#define GpuResetState (g->state)
#define GpuCommandQueue (g->queue)
#define _qin (g->head)
#define _qout (g->tail)
#define GpuSavedIntrMask (g->saved)
#define DMA2_CHCR (&g->dma)
#define GPU_STATUS (&g->status)
#define GpuFuncTable (&drivers[g->root])
static unsigned callback_code(GpuCompletionCallback callback){
 if(!callback)return 0;if(callback==completion0)return 1;assert(callback==completion1);return 2;
}
static u32 payload_index(const void *p){
 uintptr_t delta=(uintptr_t)p-(uintptr_t)g->payload;
 assert(delta<sizeof(g->payload)&&!(delta%4));return (u32)(delta/4);
}
static void image(Image *out){
 memset(out,0,sizeof(*out));memcpy(out->state,&g->state,136);
 for(unsigned i=0;i<64;i++){
  volatile struct GpuQueue *q=&g->queue[i];QueueImage *copy=&out->queue[i];
  assert(q->op==operation0||q->op==operation1);copy->operation=q->op==operation1;
  copy->payload=payload_index(q->p0);copy->parameter=q->p1;
  for(unsigned j=0;j<21;j++)copy->bytes[j]=(u32)q->buf[j];
  for(unsigned j=0;j<4;j++)copy->padding[j]=((volatile u8*)q)[108+j];
 }
 memcpy(out->payload,g->payload,sizeof(out->payload));
 out->head=g->head;out->tail=g->tail;out->saved=g->saved;out->mask=g->mask;
 out->dma=g->dma;out->status=g->status;out->root=g->root;out->serial=g->serial;
 out->operations=g->operations;out->dmaCalls=g->dmaCalls;out->completions=g->completions;
}
static void event(unsigned kind,unsigned long first,unsigned long second){
 assert(g->calls<16&&kind<12);Event *e=&g->events[g->calls++];
 e->kind=kind;e->first=first;e->second=second;image(&e->before);seenEvents[kind]++;
}
static void live_change(unsigned phase){
 u32 bits=scenario*0x31415927U+g->serial++*0x17171717U+phase;
 g->root^=1;g->state.debug02.level=(u8)(bits>>8);
 g->state.pending08=(bits&1)?0:0x80000000U;
 g->state.completion0C=(bits&2)?completion0:completion1;
 ((u8*)&g->state.draw10)[bits%92]^=(u8)bits;
 ((u8*)&g->state.display6C)[(bits>>8)%20]^=(u8)(bits>>16);
}
static s32 debug(const char *format,...){
 va_list ap;va_start(ap,format);
 if(format==DrawSyncCallbackFmt){GpuCompletionCallback callback=va_arg(ap,GpuCompletionCallback);event(1,callback_code(callback),0);}
 else {assert(format==StrSetDispMaskFmt);s32 enabled=va_arg(ap,s32);event(2,(u32)enabled,0);}
 va_end(ap);if(profile&1)live_change(1);return -1;
}
s32 (*GPU_printf)(const char *,...)=debug;
s32 SetIntrMask(s32 mask){
 s32 previous=g->mask;event(3,(u32)mask,0);g->mask=mask&65535;
 if(mask&&(profile&2))live_change(2);return previous;
}
LibInterruptCallback DMACallback(s32 channel,LibInterruptCallback callback){
 assert(channel==2&&!callback);event(4,(u32)channel,0);
 if((profile&16)&&!g->dmaCalls)g->head=(g->head+1)&63;
 g->dmaCalls++;g->payload[(scenario>>3)&63]^=0x12345678U;return 0;
}
static int operate(unsigned which,void *payload,unsigned long parameter){
 unsigned index=payload_index(payload);event(5+which,index,parameter);g->operations++;
 assert((uintptr_t)payload>UINT32_MAX);g->payload[index]^=(u32)parameter+which*0x17171717U;
 ((u8*)&g->state.draw10)[(scenario+which)%92]^=(u8)scenario;
 if(profile&4)g->dma|=0x01000000U;
 if(profile&32)g->state.completion0C=which?completion0:0;
 assert(g->operations<=4);return -(int)which;
}
static int operation0(void *payload,unsigned long parameter){return operate(0,payload,parameter);}
static int operation1(void *payload,unsigned long parameter){return operate(1,payload,parameter);}
static void complete(unsigned which){
 /* The pending flag must be cleared before entering user code. */
 assert(g->state.pending08==0);event(7+which,which,0);g->completions++;
 g->payload[(scenario+which)&63]^=0x91827364U;
 if(profile&8){g->state.pending08=1;g->state.completion0C=which?completion0:completion1;}
}
static void completion0(void){complete(0);}
static void completion1(void){complete(1);}
void GPU_memset(s8 *destination,s8 value,s32 length){
 assert(destination==(s8*)&g->state.display6C&&value==-1&&length==20);
 event(9,116,(u32)length);memset(destination,(u8)value,(size_t)length);
 if(profile&1)live_change(9);
}
static void control(unsigned driver,u32 command){
 assert(driver==g->root);event(10,driver,command);if(profile&2)live_change(10);
}
static void control0(u32 command){control(0,command);}
static void control1(u32 command){control(1,command);}
'''
TAIL = r'''
static GpuCompletionCallback model_register(GpuCompletionCallback callback){
 if(((u8*)&g->state)[2]>=2)debug(DrawSyncCallbackFmt,callback);
 GpuCompletionCallback previous;memcpy(&previous,(u8*)&g->state+16,sizeof(previous));
 memcpy((u8*)&g->state+16,&callback,sizeof(callback));return previous;
}
static void model_mask(s32 enabled){
 if(((u8*)&g->state)[2]>=2)debug(StrSetDispMaskFmt,enabled);
 if(!enabled)GPU_memset((s8*)&g->state+116,-1,20);
 control(g->root,0x03000000U|(!enabled));
}
static int model_drain(void){
 if(g->dma&0x01000000U)return 1;
 g->saved=SetIntrMask(0);
 for(;;){
  if(g->head==g->tail||g->dma&0x01000000U)break;
  GpuCompletionCallback registered;memcpy(&registered,(u8*)&g->state+16,sizeof(registered));
  if(((unsigned)(g->tail+1)%64)==(unsigned)g->head&&!registered)DMACallback(2,0);
  assert(g->status&0x04000000U);
  volatile struct GpuQueue *q=&g->queue[g->tail];
  if(q->op==operation0)operation0(q->p0,q->p1);else operation1(q->p0,q->p1);
  g->tail=(g->tail+1)%64;
 }
 SetIntrMask(g->saved);
 if(g->head==g->tail&&!(g->dma&0x01000000U)){
  u32 pending;memcpy(&pending,(u8*)&g->state+8,4);
  if(pending){
   GpuCompletionCallback registered;memcpy(&registered,(u8*)&g->state+16,sizeof(registered));
   if(registered){memset((u8*)&g->state+8,0,4);registered();}
  }
 }
 return (g->head-g->tail)&63;
}
static u32 seed=0x92378;
static u32 next(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
static void initialize(void){
 memset(&actual,0,sizeof(actual));g=&actual;
 for(unsigned i=0;i<4;i++)actual.before[i]=actual.after[i]=next();
 for(unsigned i=0;i<136;i++)((u8*)&actual.state)[i]=(u8)next();
 static const u32 pending[]={0,1,0x80000000U,0xFFFFFFFFU};
 unsigned depth=(scenario>>6)&3,registration=(scenario>>8)%3;
 actual.state.pending08=pending[(scenario>>10)&3];seenPending[(scenario>>10)&3]++;
 actual.state.completion0C=registration==0?0:registration==1?completion0:completion1;
 actual.state.debug02.level=(u8)(scenario>>4);seenDebug[(u8)(scenario>>4)]++;
 actual.tail=scenario&63;actual.head=(actual.tail+(int)depth)&63;
 seenHead[actual.tail]++;seenDepth[depth]++;seenCallbacks[registration]++;
 actual.mask=(s32)(next()&65535U)|1;actual.saved=(s32)next();
 actual.dma=(scenario&512)?0x01000000U:0;actual.status=0x04000000U;
 actual.root=scenario&1;
 for(unsigned i=0;i<64;i++){
  actual.payload[i]=next();volatile struct GpuQueue *q=&actual.queue[i];
  q->op=(i&1)?operation1:operation0;q->p0=&actual.payload[(i*7)&63];
  q->p1=(i&2)?(unsigned long)&parameters[i]:next();
  for(unsigned j=0;j<21;j++)q->buf[j]=(s32)next();
 }
 memcpy(&expected,&actual,sizeof(actual));
 for(unsigned i=0;i<64;i++)expected.queue[i].p0=&expected.payload[payload_index(actual.queue[i].p0)];
}
static void compare(void){
 static Image a,b;g=&actual;image(&a);g=&expected;image(&b);assert(!memcmp(&a,&b,sizeof(a)));
 assert(actual.calls==expected.calls);
 assert(!memcmp(actual.events,expected.events,sizeof(actual.events)));
 assert(!memcmp(actual.before,expected.before,sizeof(actual.before)));
 assert(!memcmp(actual.after,expected.after,sizeof(actual.after)));
}
int main(void){
 assert(sizeof(void*)==8&&sizeof(GpuCompletionCallback)==8&&sizeof(GpuResetState80)==136);
 assert(sizeof(struct GpuQueue)==112&&offsetof(struct GpuQueue,buf)==24);
 assert(offsetof(GpuResetState80,pending08)==8&&offsetof(GpuResetState80,completion0C)==16);
 assert(offsetof(GpuResetState80,draw10)==24&&offsetof(GpuResetState80,display6C)==116);
 assert((uintptr_t)completion0>UINT32_MAX&&(uintptr_t)completion1>UINT32_MAX);
 drivers[0].control10=control0;drivers[1].control10=control1;
 for(scenario=0;scenario<4096;scenario++){
  profile=scenario>>6;initialize();
  unsigned selection=(scenario>>2)%3;
  GpuCompletionCallback requested=selection==0?0:selection==1?completion0:completion1;
  g=&expected;GpuCompletionCallback wanted=model_register(requested);
  g=&actual;GpuCompletionCallback result=DrawSyncCallback(requested);
  assert(result==wanted);compare();
  static const s32 masks[]={0,1,-1,(-2147483647-1),2147483647};s32 enabled=masks[(scenario>>4)%5];
  g=&expected;model_mask(enabled);g=&actual;SetDispMask(enabled);compare();
  g=&expected;int reference=model_drain();g=&actual;int value=_exeque();assert(value==reference);compare();
  /* Simulate DMA completion, then check live callback replacement/rearming. */
  actual.dma=expected.dma=0;actual.calls=expected.calls=0;
  memset(actual.events,0,sizeof(actual.events));memset(expected.events,0,sizeof(expected.events));
  actual.operations=expected.operations=0;
  g=&expected;reference=model_drain();g=&actual;value=_exeque();assert(value==reference);compare();
 }
 for(unsigned i=0;i<256;i++)assert(seenDebug[i]);for(unsigned i=0;i<64;i++)assert(seenHead[i]);
 for(unsigned i=0;i<4;i++)assert(seenDepth[i]&&seenPending[i]);
 for(unsigned i=0;i<3;i++)assert(seenCallbacks[i]);for(unsigned i=1;i<=10;i++)assert(seenEvents[i]);
 return 0;
}
'''


class GpuCompletionTest(unittest.TestCase):
    def test_registration_draining_and_cache_ownership(self):
        owners = source_owners(ROOT / 'src/main/PAL')
        bodies = '\n'.join(definition_source(owners[name].read_text(), name)
                           for name in ('DrawSyncCallback', 'SetDispMask', '_exeque'))
        with TemporaryDirectory() as directory:
            source = Path(directory) / 'completion.c'
            binary = source.with_suffix('')
            source.write_text(PREFIX + bodies + TAIL)
            # _exeque still calls the inherited GpuOpFunc int(*)() ABI. Keep
            # that diagnostic visible; the completion callback has a prototype.
            result = subprocess.run(['cc', '-std=c11', '-O2', '-Wall', '-Wextra', '-Werror',
                                     '-Wno-error=deprecated-non-prototype',
                                     '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
                                     '-I', str(ROOT / 'include'), str(source), '-o', str(binary)],
                                    capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            if result.stderr:
                print(result.stderr, end='')
            result = subprocess.run([str(binary)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)


if __name__ == '__main__':
    unittest.main()
