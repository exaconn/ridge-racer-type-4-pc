"""Graphics reset against a byte-level state and service-call model.

The BIOS and graphics services are simulated; this does not run GPU hardware.
"""
from pathlib import Path
from tempfile import TemporaryDirectory
import subprocess, unittest

from c_functions import definition_source, source_owners

ROOT = Path(__file__).resolve().parents[2]

class GpuResetTest(unittest.TestCase):
    def test_modes_debug_levels_and_mutable_services(self):
        owner = source_owners(ROOT/'src/main/PAL')['ResetGraph']
        body = definition_source(owner.read_text(),'ResetGraph')
        code = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include "rr/layout.h"
#include "rr/gpu_reset.h"
typedef struct {u32 code;unsigned long a,b,c;u8 state[136];u32 root;GpuDimension4 widths[3];GpuHeightTable14 heights;} Event;
typedef struct {
 u32 guards[4];
 union {GpuResetState80 state;u8 raw[136];} data;
 u32 tails[4];
 GpuResetDriver40 *root;
 GpuDimension4 widths[3];GpuHeightTable14 heights;
 Event events[12];u32 count,seed,phase;
} Environment;
static Environment actual,expected,*g;
GpuResetDriver40 GpuDriverTable;
static GpuResetDriver40 alternate;
const char ResetGraphFmt[]="init",StrResetGraphFmt[]="debug";
#define GpuResetState (g->data.state)
#define D_800ABAC2 (g->data.state.debug02)
#define GpuFuncTable (g->root)
#define D_800ABB40 (g->widths)
#define D_800ABB4C (g->heights)
static u32 next(u32 *p){u32 n=*p;n^=n<<13;n^=n>>17;n^=n<<5;return *p=n;}
static void completion0(void){assert(!"reset must not execute completion");}
static void completion1(void){assert(!"reset must not execute completion");}
/* Preserve the original128-byte random input stream and mutation choices.
 * Its callback bytes now select real functions; native pointer alignment adds
 * eight bytes before the unchanged112-byte draw/display cache payload. */
static void legacyByte(unsigned position,u8 value){
 if(position<12)g->data.raw[position]=value;
 else if(position<16)g->data.state.completion0C=(value&1)?completion1:completion0;
 else g->data.raw[position+8]=value;
}
static u32 driverIndex(void){assert(g->root==&GpuDriverTable||g->root==&alternate);return g->root==&alternate;}
static void event(u32 code,unsigned long a,unsigned long b,unsigned long c){Event *e;assert(g->count<12);e=&g->events[g->count++];e->code=code;e->a=a;e->b=b;e->c=c;memcpy(e->state,g->data.raw,136);e->root=driverIndex();memcpy(e->widths,g->widths,sizeof(e->widths));e->heights=g->heights;}
static void mutate(u32 code){unsigned i;u32 r;
 if(!g->phase)return;
 r=next(&g->seed);g->root=(r&1)?&alternate:&GpuDriverTable;
 g->data.raw[0]=(u8)(r%3);g->data.raw[2]=(u8)(r>>8);
 legacyByte((r>>16)%128,(u8)(r>>24));
 /* The reset result must remain a valid dimension index when later used. */
 g->data.raw[0]%=3;
 for(i=0;i<3;i++){g->widths[i].value=(u16)next(&g->seed);g->heights.dimensions[i].value=(u16)next(&g->seed);}
 if(g->phase==2)memset(g->data.raw+24,(int)code,112);
}
s32 DebugPrintStub(const char *f,...){va_list ap;GpuResetDriver40 *a;GpuResetState80 *b;va_start(ap,f);a=va_arg(ap,GpuResetDriver40*);b=va_arg(ap,GpuResetState80*);va_end(ap);assert(f==ResetGraphFmt&&a==&GpuDriverTable&&b==&g->data.state);event(1,0,0,0);mutate(1);return -123;}
static s32 debug(const char *f,...){va_list ap;s32 mode;va_start(ap,f);mode=va_arg(ap,s32);va_end(ap);assert(f==StrResetGraphFmt);event(2,(u32)mode,0,0);mutate(2);return 456;}
s32 (*GPU_printf)(const char *,...)=debug;
void GPU_memset(s8 *p,s8 value,s32 n){uintptr_t offset=(uintptr_t)p-(uintptr_t)&g->data.state;assert(offset<=136&&n>=0&&(unsigned)n<=136-offset);event(3,offset,(u8)value,(u32)n);memset(p,(u8)value,(size_t)n);mutate(3);}
void ResetCallback(void){event(4,0,0,0);mutate(4);}
s32 GPU_cw(unsigned long address){assert(address==((uintptr_t)g->root&0xFFFFFF));event(5,address,0,0);mutate(5);return -999;}
s32 _reset(s32 mode){u32 result=(g->seed^((u32)mode>>3))%3;event(6,(u32)mode,result,0);mutate(6);return (s32)result;}
static s32 reset0(s32 mode){assert(mode==1);event(7,mode,0,0);mutate(7);return (s32)(g->seed^0x81234567U);}
static s32 reset1(s32 mode){assert(mode==1);event(8,mode,0,0);mutate(8);return (s32)(g->seed^0x76543210U);}
''' + body + r'''
static s32 model(s32 mode){unsigned low=(u32)mode%8;u16 w,h;
 if(low==0||low==3||low==5){
  if(low!=5)DebugPrintStub(ResetGraphFmt,&GpuDriverTable,&g->data.state);
  GPU_memset((s8*)g->data.raw,0,136);
  ResetCallback();
  GPU_cw((uintptr_t)g->root&0xFFFFFF);
  g->data.raw[0]=(u8)_reset(mode);g->data.raw[1]=1;
  w=g->widths[g->data.raw[0]].value;h=g->heights.dimensions[g->data.raw[0]].value;
  memcpy(g->data.raw+4,&w,2);memcpy(g->data.raw+6,&h,2);
  GPU_memset((s8*)g->data.raw+24,-1,92);
  GPU_memset((s8*)g->data.raw+116,-1,20);
  return g->data.raw[0];
 }
 if(g->data.raw[2]>=2)debug(StrResetGraphFmt,mode);
 if(g->root==&GpuDriverTable)return reset0(1);
 return reset1(1);
}
int main(void){u32 n,phase,seed=0x929D4,seen[8]={0},levels[256]={0};unsigned i;uint64_t cases=0;s32 mode,result,reference;
 _Static_assert(sizeof(GpuResetState80)==136,"complete native state");
 _Static_assert(offsetof(GpuResetState80,pending08)==8,"pending");
 _Static_assert(offsetof(GpuResetState80,completion0C)==16,"full native completion");
 _Static_assert(offsetof(GpuResetState80,draw10)==24,"draw cache");
 _Static_assert(offsetof(GpuResetState80,display6C)==116,"display cache");
 _Static_assert(sizeof(GpuResetDriver40)==16*sizeof(void*),"full native driver");
 _Static_assert(offsetof(GpuResetDriver40,reset34)==13*sizeof(void*),"native callback offset");
 _Static_assert(sizeof(unsigned long)>=sizeof(void*),"address width");
 assert((uintptr_t)&actual>UINT32_MAX&& (uintptr_t)&alternate>UINT32_MAX);
 GpuDriverTable.reset34=reset0;alternate.reset34=reset1;
 for(n=0;n<65536;n++)for(phase=0;phase<4;phase++){
  memset(&actual,0,sizeof(actual));actual.seed=next(&seed);actual.phase=phase;
  g=&actual;memset(actual.data.raw,0xA5,136);
  for(i=0;i<128;i++)legacyByte(i,(u8)next(&seed));
  actual.data.raw[2]=(u8)n;levels[(u8)n]++;
  for(i=0;i<4;i++){actual.guards[i]=next(&seed);actual.tails[i]=next(&seed);}
  for(i=0;i<3;i++){actual.widths[i].value=(u16)next(&seed);actual.widths[i].reserved=(u16)next(&seed);actual.heights.dimensions[i].value=(u16)next(&seed);actual.heights.dimensions[i].reserved=(u16)next(&seed);}
  actual.heights.commands0C[0]=next(&seed);actual.heights.commands0C[1]=next(&seed);
  actual.root=(n&1)?&alternate:&GpuDriverTable;expected=actual;
  if(phase==0)mode=(s32)n;else if(phase==1)mode=(s32)(n|0xFFFF0000U);else mode=(s32)((next(&seed)&0xFFFF0000U)|n);
  seen[(u32)mode&7]++;g=&expected;reference=model(mode);g=&actual;result=ResetGraph(mode);
  assert(result==reference);assert(!memcmp(&actual,&expected,sizeof(actual)));cases++;
 }
 assert(cases==262144);for(i=0;i<8;i++)assert(seen[i]);for(i=0;i<256;i++)assert(levels[i]);return 0;
}
'''
        with TemporaryDirectory() as tmp:
            p=Path(tmp); (p/'test.c').write_text(code)
            r=subprocess.run(['cc','-std=gnu11','-O2','-fsanitize=undefined','-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(p/'test.c'),'-o',str(p/'test')],capture_output=True,text=True)
            self.assertEqual(r.returncode,0,r.stderr)
            subprocess.run([str(p/'test')],check=True)

if __name__=='__main__': unittest.main()
