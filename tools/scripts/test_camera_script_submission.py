"""Current camera script: complete native owners, DMA maps and original MIPS.

Motion, geometry and drawing services are instrumented. The native path runs
the actual mask constructor and matrix scaler; the reference uses byte models.
All service-entry snapshots and final owners are compared without normalizing
native pointer widths. A separate wire serializer checks original/linked MIPS.
"""
import ctypes as C
import os
from pathlib import Path
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32

from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
NAME = 'StepCameraScript'
SCRATCH, ALT, SCRIPTS = 0x1F800000, 0x802C0010, (0x802C1000, 0x802C1100)
STACK, STOP = 0x803F0000, 0x803FF000
SERVICES = ['InitCameraMotion', 'InitMotionPanel', 'InitScrollTrack', 'InitMotionTrack',
            'UpdateCameraMotion', 'UpdateMotionPanel', 'UpdateScrollTrack', 'UpdateMotionTrack',
            'BuildCameraMatrices', 'SetGeomScreen', 'SetupViewport', 'SetGeomOffset',
            'ScaleMatrixRow1', 'DrawPresentationMotionModel', 'DrawMotionControllerPanel',
            'DrawRingEffects', 'DrawPresentationTrackModel', 'SetDrawStp']
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "rr/rro_model.h"
#include "rr/presentation_types.h"
#include "rr/gpu_address.h"
#include "rr/frame_storage_types.h"
#include "game/car/carbound.h"
#include "game/camera/camrun.h"
#include "game/camera/cammove.h"
#include "game/camera/campath.h"
#include "game/gfx/motion1.h"
#include "game/gfx/motion4.h"
#include "game/gfx/ring.h"
#include "game/gfx/ring2.h"
#include "game/camera/viewcam.h"
#include "game/math/rotmat.h"
#include "psyq/libgpu/gpudisp.h"
enum {WIRE_SIZE=0x568+0x188+40+64+0x7C+2*0x22778+8192+8,MAX_EVENTS=16};
typedef struct {
 u32 before[4];PresentationContextPrefix root;PresentationController alternate;
 MotionClock20 clocks[2];CameraScriptEntry8 scripts[2][4];
 RroSceneScratchPrefix scratch;FrameStorage frames[2];u32 gpu[2048];
 u32 frame;s32 threshold;u32 after[4];
} State;
typedef struct {u32 kind,args[2];State entry;} Event;
typedef struct {unsigned calls;Event events[MAX_EVENTS];State final;} Capture;
typedef struct {u32 kind,args[2];u8 before[WIRE_SIZE],after[WIRE_SIZE];} WireEvent;
static State live,initial;
static Capture actual,expected,*capture;
static WireEvent wireEvents[MAX_EVENTS];
static u8 firstWire[WIRE_SIZE],lastWire[WIRE_SIZE];
static GpuDmaRegion regions[3];
static unsigned scenario,mapProfile,mutation,modelRun,recordWire,serial;
static unsigned serviceSeen[19],mapsSeen[2],aliasSeen[4],changedSeen[2],wrapSeen;
static unsigned clockSeen[2][2],viewportSeen[2],drawSeen[2],stencilSeen[2],dmaDifferent;
static unsigned sentinelSeen,fourthWordSeen,aliasExecuted[4];
#define RaceContext live.root
#define CurrentDrawBuf (&live.frames[live.frame])
#define SecondaryMotionClock live.clocks[0]
#define PrimaryMotionClock live.clocks[1]
#define VSyncThreshold live.threshold
#define SCRATCHPAD ((u8*)&live.scratch)
#define SCRATCH_ROT_MATRIX ((u8*)&live.scratch.view.matrix28)
static void native_stp(GpuDrawModePacket *,int);
static void native_scale(s32,MatrixWords32 *);
static u32 get32(const void *p){u32 v;memcpy(&v,p,4);return v;}
static void put32(void *p,u32 v){memcpy(p,&v,4);}
static void put16(void *p,u32 v){u16 h=(u16)v;memcpy(p,&h,2);}
static s32 word(u32 v){s32 s;memcpy(&s,&v,4);return s;}
static u32 frame_address(unsigned bank){return (mapProfile?0x80300000U:0x80200000U)+bank*0x40000U;}
static u32 gpu_address(void){return mapProfile?0x80380000U:0x80280000U;}
static u32 wire(const void *p){
 uintptr_t n=(uintptr_t)p,b;
 for(unsigned bank=0;bank<2;bank++){
  b=(uintptr_t)&live.frames[bank];
  if(n>=b&&n-b<sizeof(FrameStorage))return frame_address(bank)+(u32)(n-b);
 }
 b=(uintptr_t)live.gpu;
 if(n>=b&&n-b<sizeof(live.gpu))return gpu_address()+(u32)(n-b);
 assert(!"GPU pointer outside complete owners");return 0;
}
static PresentationController *controller(unsigned index){return index?&live.alternate:&live.root.presentation250;}
static u32 controller_address(const PresentationController *p){
 if(p==controller(0))return @RACE@+0x250U;
 assert(p==controller(1));return 0x802C0010U;
}
static void store_controller(u8 *p,const PresentationController *c){
 memset(p,0,0x188);memcpy(p,&c->position,16);memcpy(p+16,&c->target,16);
 put32(p+0x48,(u32)c->viewDistance48);put32(p+0x4C,(u32)c->scale4C);
 put32(p+0x50,(u32)c->fullViewport50);put32(p+0x54,(u32)c->drawEnabled54);
 put32(p+0x58,(u32)c->stencilEnabled58);put32(p+0x5C,(u32)c->alternateClock5C);
 put32(p+0x60,(u32)c->flags60);
 assert(c->script64==live.scripts[0]||c->script64==live.scripts[1]);
 put32(p+0x64,c->script64==live.scripts[0]?0x802C1000U:0x802C1100U);
 put16(p+0x68,(u16)c->scriptIndex68);put16(p+0x6A,(u16)c->scriptTimer6A);
 put32(p+0x6C,(u32)c->scriptOperation6C);put32(p+0x70,(u32)c->scriptArgument70);
}
static void snapshot_wire(u8 *out){
 u8 *p=out;memset(p,0,0x568);memcpy(p,live.root.reserved00,0x250);
 store_controller(p+0x250,controller(0));put32(p+0x3D8,controller_address(live.root.controller3D8));
 memcpy(p+0x3DC,live.root.reserved3DC,sizeof(live.root.reserved3DC));
 put32(p+0x564,(u32)live.root.ringModel564);p+=0x568;
 store_controller(p,controller(1));p+=0x188;
 memcpy(p,live.clocks,40);p+=40;memcpy(p,live.scripts,64);p+=64;
 memset(p,0,0x7C);put32(p,wire(live.scratch.view.projection.pointers.cursor));
 put32(p+4,wire(live.scratch.view.projection.pointers.order));
 memcpy(p+8,&live.scratch.view.projection.pose08.position00,16);
 memcpy(p+0x18,&live.scratch.view.projection.pose08.angles10,16);
 memcpy(p+0x28,&live.scratch.view.matrix28,32);
 put32(p+0x4C,live.scratch.reserved4C);put32(p+0x5C,live.scratch.reserved5C);
 put32(p+0x60,(u32)live.scratch.depthShift60);put32(p+0x64,(u32)live.scratch.renderFlags64);
 put32(p+0x68,live.scratch.reserved68);memcpy(p+0x6C,&live.scratch.viewport6C,8);
 memcpy(p+0x74,live.scratch.draw74.words,8);p+=0x7C;
 memcpy(p,live.frames,sizeof(live.frames));p+=sizeof(live.frames);
 memcpy(p,live.gpu,sizeof(live.gpu));p+=sizeof(live.gpu);
 put32(p,frame_address(live.frame));put32(p+4,(u32)live.threshold);p+=8;
 assert(p-out==WIRE_SIZE);
}
static void event(unsigned kind,u32 first,u32 second){
 unsigned n=capture->calls++;assert(n<MAX_EVENTS);
 Event *e=&capture->events[n];e->kind=kind;e->args[0]=first;e->args[1]=second;
 memcpy(&e->entry,&live,sizeof(live));serviceSeen[kind]++;
 if(recordWire){WireEvent *v=&wireEvents[n];v->kind=kind;v->args[0]=first;v->args[1]=second;snapshot_wire(v->before);}
}
static void complete_event(void){if(recordWire)snapshot_wire(wireEvents[capture->calls-1].after);}
static void mutate(unsigned phase){
 unsigned n=serial++;if(!mutation)return;
 u32 v=scenario*0x17171717U+n*0x1234567U+phase;
 live.frame^=1;live.root.controller3D8=controller((scenario+n+phase)&1);
 PresentationController *c=live.root.controller3D8;
 c->viewDistance48=word(v);c->fullViewport50=(s32)((n+scenario)>>1)&1;
 c->drawEnabled54=(s32)((n+scenario+1)>>2)&1;
 c->stencilEnabled58=(s32)((n+scenario)>>3)&1;c->scriptOperation6C=(s32)((n+scenario)%7)-1;
 c->scriptArgument70=word(v^0xA55A55AAU);
 for(unsigned i=0;i<4;i++){
  put32((u8*)&c->position+i*4,v+i*73U);put32((u8*)&c->target+i*4,~v+i*313U);
 }
 live.scratch.view.projection.pointers.cursor=(scenario&1)?live.frames[live.frame].bytes+0xB6C:(u8*)live.gpu+1024;
 live.scratch.view.projection.pointers.order=(scenario&2)?live.frames[live.frame^1].buffer.secondaryOrderingB70.words:live.gpu+64;
 for(unsigned bank=0;bank<2;bank++){
  live.frames[bank].buffer.ordering70.words[702]^=v;
  live.frames[bank].buffer.ordering70.words[703]^=v>>3;
 }
}
static void call_scalar(unsigned kind,s32 first,s32 second){event(kind,(u32)first,(u32)second);mutate(kind);complete_event();}
void InitCameraMotion(int selection){call_scalar(1,selection,0);}
void InitMotionPanel(s32 selection){call_scalar(2,selection,0);}
void InitScrollTrack(int selection){call_scalar(3,selection,0);}
void InitMotionTrack(int selection){call_scalar(4,selection,0);}
static void call_clock(unsigned kind,MotionClock20 *clock){
 assert(clock==&live.clocks[0]||clock==&live.clocks[1]);
 event(kind,clock==&live.clocks[0]?@CLOCK0@:@CLOCK1@,0);mutate(kind);complete_event();
}
void UpdateCameraMotion(MotionClock20 *clock){call_clock(5,clock);}
void UpdateMotionPanel(MotionClock20 *clock){call_clock(6,clock);}
void UpdateScrollTrack(MotionClock20 *clock){call_clock(7,clock);}
void UpdateMotionTrack(MotionClock20 *clock){call_clock(8,clock);}
void BuildCameraMatrices(void){
 event(9,0,0);
 for(unsigned i=0;i<16;i++)live.scratch.view.matrix28.halves[i]=(u16)(scenario+i*0x1717U);
 mutate(9);complete_event();
}
void SetGeomScreen(s32 distance){call_scalar(10,distance,0);}
void SetupViewport(s32 mode){assert(mode==6);call_scalar(11,mode,0);}
void SetGeomOffset(s32 x,s32 y){assert(x==160&&y==232);call_scalar(12,x,y);}
void ScaleMatrixRow1(s32 scale,MatrixWords32 *matrix){
 assert(scale==2&&matrix==&live.scratch.view.matrix28);event(13,(u32)scale,0x1F800028U);
 if(modelRun){
  u8 *p=(u8*)matrix;
  for(unsigned i=0;i<3;i++){u16 h;memcpy(&h,p+6+i*2,2);put16(p+6+i*2,(u32)h*2);}
 }else native_scale(scale,matrix);
 mutate(13);complete_event();
}
void DrawPresentationMotionModel(void){call_scalar(14,0,0);}
void DrawMotionControllerPanel(void){call_scalar(15,0,0);}
void DrawRingEffects(void){call_scalar(16,0,0);}
void DrawPresentationTrackModel(void){call_scalar(17,0,0);}
void SetDrawStp(GpuDrawModePacket *packet,int enabled){
 event(18,wire(packet),(u32)enabled);
 if(modelRun){u8 *p=(u8*)packet;p[3]=2;put32(p+4,0xE6000000U|(enabled!=0));put32(p+8,0);}
 else native_stp(packet,enabled);
 mutate(18);complete_event();
}
'''
REFERENCE = r'''
static u32 *head(unsigned index){return live.frames[live.frame].buffer.ordering70.words+index;}
static void low24(void *destination,u32 value){put32(destination,(get32(destination)&0xFF000000U)|(value&0xFFFFFFU));}
static void attach_packet(u8 *packet){low24(packet,get32(head(703)));low24(head(703),wire(packet));}
static void attach_subtable(unsigned index){
 u32 *table=live.scratch.view.projection.pointers.order;
 low24(table,get32(head(index)));low24(head(index),wire(table+703));
}
static void reference(void){
 PresentationController *c=live.root.controller3D8;
 c->scale4C=live.threshold==128?2:1;
 unsigned clock=c->alternateClock5C?0:1;
 clockSeen[clock][live.clocks[clock].active!=0]++;
 unsigned changed=0;
 if(live.clocks[clock].active){
  s16 before=c->scriptTimer6A;u16 bits=(u16)((u16)before+1);memcpy(&c->scriptTimer6A,&bits,2);
  if(before==INT16_MAX)wrapSeen++;
  if(c->scriptTimer6A>=c->script64[c->scriptIndex68].duration){
   unsigned next=(unsigned)c->scriptIndex68+1;
   if(c->script64[next].operation<0){next=0;sentinelSeen++;}
   c->scriptIndex68=(s16)next;c->scriptOperation6C=c->script64[next].operation;
   c->scriptArgument70=c->script64[next].argument;c->scriptTimer6A=0;changed=1;
  }
 }
 changedSeen[changed]++;
 s32 operation=c->scriptOperation6C;
 if(operation>=0&&operation<4){
  if(changed)call_scalar(1+(unsigned)operation,c->scriptArgument70,0);
  call_clock(5+(unsigned)operation,&live.clocks[clock]);
 }
 c=live.root.controller3D8;drawSeen[c->drawEnabled54!=0]++;
 if(!c->drawEnabled54)return;
 fourthWordSeen+=live.scratch.view.projection.pose08.angles10.vector.pad!=c->target.pad;
 memcpy(&live.scratch.view.projection.pose08.position00,&c->position,16);
 memcpy(&live.scratch.view.projection.pose08.angles10,&c->target,16);
 BuildCameraMatrices();SetGeomScreen(live.root.controller3D8->viewDistance48);
 unsigned full=live.root.controller3D8->fullViewport50!=0;viewportSeen[full]++;
 if(!full){SetupViewport(6);SetGeomOffset(160,232);ScaleMatrixRow1(2,&live.scratch.view.matrix28);}
 live.scratch.depthShift60=5;
 live.scratch.view.projection.pointers.order=live.frames[live.frame].buffer.secondaryOrderingB70.words;
 c=live.root.controller3D8;
 if(c->drawEnabled54&&c->scriptOperation6C>=0&&c->scriptOperation6C<4)call_scalar(14+(unsigned)c->scriptOperation6C,0,0);
 unsigned stencil=live.root.controller3D8->stencilEnabled58!=0;stencilSeen[stencil]++;
 if(!stencil)attach_subtable(702);
 else{
  if(!mutation)aliasExecuted[(scenario>>10)&3]++;
  u8 *packet=live.scratch.view.projection.pointers.cursor;
  SetDrawStp((GpuDrawModePacket*)packet,0);put32(packet+4,get32(packet+4)|2);
  attach_packet(packet);live.scratch.view.projection.pointers.cursor=packet+12;
  attach_subtable(703);packet=live.scratch.view.projection.pointers.cursor;
  SetDrawStp((GpuDrawModePacket*)packet,0);attach_packet(packet);packet+=12;
  live.scratch.view.projection.pointers.cursor=packet;
  packet[3]=3;packet[4]=packet[5]=packet[6]=0;packet[7]=0x60;
  put16(packet+8,0);put16(packet+10,0);put16(packet+12,320);put16(packet+14,240);
  attach_packet(packet);packet+=16;live.scratch.view.projection.pointers.cursor=packet;
  SetDrawStp((GpuDrawModePacket*)packet,1);attach_packet(packet);
  live.scratch.view.projection.pointers.cursor=packet+12;
 }
 live.scratch.view.projection.pointers.order=live.frames[live.frame].buffer.secondaryOrderingB70.words;
}
static void initialize(unsigned number){
 scenario=number;mapProfile=(number>>4)&1;mutation=(number>>3)&1;serial=0;recordWire=0;
 memset(&live,0,sizeof(live));u32 random=0x725A193BU^number;
 for(unsigned i=0;i<sizeof(live.frames);i++){
  random^=random<<13;random^=random>>17;random^=random<<5;((u8*)live.frames)[i]=(u8)random;
 }
 for(unsigned i=0;i<sizeof(live.gpu);i++)((u8*)live.gpu)[i]=(u8)(number+i*73U);
 for(unsigned i=0;i<sizeof(live.root.reserved00);i++)live.root.reserved00[i]=(u8)(number+i*17U);
 for(unsigned i=0;i<sizeof(live.root.reserved3DC);i++)live.root.reserved3DC[i]=(u8)(number+i*19U);
 static const s16 timers[]={INT16_MIN,-1,0,1,INT16_MAX,127,128,-128};
 static const s32 durations[]={INT32_MIN,-1,0,1,INT32_MAX,127,128,-128};
 for(unsigned bank=0;bank<2;bank++){
  PresentationController *c=controller(bank);
  for(unsigned i=0;i<4;i++){
   put32((u8*)&c->position+i*4,random+i*0x17171717U+bank);
   put32((u8*)&c->target+i*4,~random+i*0x13131313U+bank);
  }
  c->viewDistance48=word(random);c->scale4C=word(~random);
  c->fullViewport50=(number>>5)&1;c->drawEnabled54=(number>>6)&1;
  c->stencilEnabled58=(number>>7)&1;c->alternateClock5C=(number>>1)&1;
  c->flags60=word(random^bank);c->script64=live.scripts[bank];
  c->scriptIndex68=(s16)((number>>2)%3);c->scriptTimer6A=timers[(number>>8)%8];
  c->scriptOperation6C=(s32)((number>>2)%7)-1;c->scriptArgument70=word(random);
  live.clocks[bank].active=((number>>9)^(number>>11))&1;live.clocks[bank].tempo=random+bank;
  for(unsigned i=0;i<4;i++){
   live.scripts[bank][i].operation=(s16)((number+i+bank)%4);
   live.scripts[bank][i].argument=(s16)(random+i*731U);
   live.scripts[bank][i].duration=durations[(number+i*3)%8];
  }
  live.scripts[bank][3].operation=-1;
 }
 live.root.controller3D8=controller(number&1);live.frame=(number>>2)&1;
 live.threshold=(number&2)?128:word(random);live.root.ringModel564=word(random);
 for(unsigned i=0;i<4;i++)live.before[i]=random+i,live.after[i]=~random+i;
 for(unsigned i=0;i<8;i++){
  live.scratch.view.projection.pose08.position00.words[i%4]=word(random+i*17U);
  live.scratch.view.projection.pose08.angles10.words[i%4]=word(~random+i*19U);
  live.scratch.view.matrix28.w[i]=word(random+i*31U);
 }
 unsigned alias=(number>>10)&3;aliasSeen[alias]++;
 static const unsigned offsets[]={0,0x70,0x98,0xB6C};
 live.scratch.view.projection.pointers.cursor=alias?live.frames[live.frame].bytes+offsets[alias]:(u8*)live.gpu+128;
 live.scratch.view.projection.pointers.order=live.frames[live.frame^1].buffer.secondaryOrderingB70.words;
 live.scratch.depthShift60=word(random);live.scratch.renderFlags64=word(~random);
 live.scratch.reserved4C=random;live.scratch.reserved5C=~random;live.scratch.reserved68=random+1;
 live.scratch.viewport6C.words[0]=random+2;live.scratch.viewport6C.words[1]=random+3;
 live.scratch.draw74.words[0]=random+4;live.scratch.draw74.words[1]=random+5;
 for(unsigned bank=0;bank<2;bank++)regions[bank]=(GpuDmaRegion){&live.frames[bank],frame_address(bank),sizeof(FrameStorage)};
 regions[2]=(GpuDmaRegion){live.gpu,gpu_address(),sizeof(live.gpu)};
 assert(GpuDmaSetRegions(regions,3));
 assert((uintptr_t)&live>UINT32_MAX);
 assert(offsetof(ProjectionScratchPrefix,pose08.position00)==2*sizeof(void*));
 assert(offsetof(ViewMatrixScratch48,matrix28)==sizeof(ProjectionScratchPrefix));
 assert(offsetof(ProjectionScratchPrefix,pose08.angles10)+sizeof(VectorWords16)==sizeof(ProjectionScratchPrefix));
 dmaDifferent+=(wire(live.scratch.view.projection.pointers.cursor)&0xFFFFFFU)!=((uintptr_t)live.scratch.view.projection.pointers.cursor&0xFFFFFFU);
 mapsSeen[mapProfile]++;memcpy(&initial,&live,sizeof(live));
}
unsigned exercise_native(void){
 for(unsigned n=0;n<4096;n++){
  initialize(n);capture=&expected;capture->calls=0;modelRun=1;reference();memcpy(&expected.final,&live,sizeof(live));
  memcpy(&live,&initial,sizeof(live));serial=0;capture=&actual;capture->calls=0;modelRun=0;
  StepCameraScript();memcpy(&actual.final,&live,sizeof(live));
  assert(actual.calls==expected.calls);
  for(unsigned i=0;i<actual.calls;i++)assert(!memcmp(&actual.events[i],&expected.events[i],sizeof(Event)));
  assert(!memcmp(&actual.final,&expected.final,sizeof(State)));
 }
 for(unsigned i=1;i<=18;i++)assert(serviceSeen[i]);
 for(unsigned i=0;i<2;i++)assert(mapsSeen[i]&&changedSeen[i]&&viewportSeen[i]&&drawSeen[i]&&stencilSeen[i]&&clockSeen[i][0]&&clockSeen[i][1]);
 for(unsigned i=0;i<4;i++)assert(aliasSeen[i]&&aliasExecuted[i]);
 assert(wrapSeen&&dmaDifferent&&sentinelSeen&&fourthWordSeen);
 return 4096;
}
unsigned prepare_trace(unsigned number){
 initialize(number);snapshot_wire(firstWire);capture=&expected;capture->calls=0;
 serial=0;modelRun=1;recordWire=1;reference();snapshot_wire(lastWire);recordWire=0;return capture->calls;
}
const void *initial_wire(void){return firstWire;}
const void *final_wire(void){return lastWire;}
const void *event_wire(unsigned index,unsigned after){assert(index<capture->calls);return after?wireEvents[index].after:wireEvents[index].before;}
u32 event_value(unsigned index,unsigned field){assert(index<capture->calls&&field<3);return field?wireEvents[index].args[field-1]:wireEvents[index].kind;}
unsigned wire_size(void){return WIRE_SIZE;}
'''


class CameraScriptSubmissionTest(unittest.TestCase):
    def test_current_full_owners_and_original_mips(self):
        owners = source_owners(ROOT/'src/main/PAL')
        body = definition_source(owners[NAME].read_text(), NAME)
        stp = definition_source(owners['SetDrawStp'].read_text(), 'SetDrawStp').replace('SetDrawStp', 'native_stp')
        scale = definition_source(owners['ScaleMatrixRow1'].read_text(), 'ScaleMatrixRow1').replace('ScaleMatrixRow1', 'native_scale')
        prefix = PREFIX.replace('@RACE@', hex(address_of('RaceContext'))+'U')
        prefix = prefix.replace('@CLOCK0@', hex(address_of('SecondaryMotionClock'))+'U')
        prefix = prefix.replace('@CLOCK1@', hex(address_of('PrimaryMotionClock'))+'U')
        with TemporaryDirectory() as temporary:
            source, library = Path(temporary)/'native.c', Path(temporary)/'native.dylib'
            source.write_text(prefix+stp+scale+body+REFERENCE)
            subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                            '-fno-strict-aliasing',
                            '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
                            '-I', str(ROOT/'include'), str(source), str(ROOT/'src/native/gpu_address.c'),
                            '-o', str(library)], check=True)
            native = C.CDLL(str(library))
            native.exercise_native.restype = C.c_uint
            self.assertEqual(native.exercise_native(), 4096)
            native.prepare_trace.argtypes = [C.c_uint];native.prepare_trace.restype = C.c_uint
            native.wire_size.restype = C.c_uint
            native.initial_wire.restype = native.final_wire.restype = C.c_void_p
            native.event_wire.argtypes = [C.c_uint, C.c_uint];native.event_wire.restype = C.c_void_p
            native.event_value.argtypes = [C.c_uint, C.c_uint];native.event_value.restype = C.c_uint32
            size = native.wire_size()
            memory = Uc(UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN | UC_MODE_MIPS32)
            memory.mem_map(0, 0x400000);memory.mem_map(SCRATCH, 4096)
            write = lambda address, data: memory.mem_write(address & 0x1FFFFFFF, bytes(data))
            read = lambda address, count: bytes(memory.mem_read(address & 0x1FFFFFFF, count))
            original = (ROOT/'assets/PAL/main.exe').read_bytes()
            base = struct.unpack_from('<I', original, 0x18)[0];write(base, original[0x800:])
            if os.getenv('RR_COMPILED_ELF'):
                with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                    elf = ELFFile(stream);symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(NAME)[0]
                    section = elf.get_section(symbol['st_shndx']);offset = symbol['st_value']-section['sh_addr']
                    self.assertEqual(symbol['st_value'], address_of(NAME))
                    write(symbol['st_value'], section.data()[offset:offset+symbol['st_size']])
            services = {address_of(name): index+1 for index, name in enumerate(SERVICES)}
            controllers = (address_of('RaceContext')+0x250, ALT)

            def store(cpu, access, address, length, value, user):
                address &= 0x1FFFFFFF
                allowed = [(STACK-48, 48), (SCRATCH, 0x28), (SCRATCH+0x60, 4),
                           (frame, 0x22778), (frame+0x40000, 0x22778), (gpu, 8192)]
                for controller in controllers:
                    allowed.extend([(controller+0x4C, 4), (controller+0x68, 12)])
                self.assertIn(length, (1, 2, 4))
                if length == 1:
                    self.assertTrue(any((start & 0x1FFFFFFF) <= address <
                                        (start & 0x1FFFFFFF)+span
                                        for start, span in ((frame, 0x22778),
                                                            (frame+0x40000, 0x22778),
                                                            (gpu, 8192))), hex(address))
                self.assertTrue(any((start & 0x1FFFFFFF) <= address and
                                    address+length <= (start & 0x1FFFFFFF)+span
                                    for start, span in allowed), (hex(address), length))

            cpu = Cpu(memory, 0, 100, 101, 102, store)
            for case in range(512):
                number = (case*7919) & 4095
                events = native.prepare_trace(number)
                frame = 0x80300000 if (number >> 4) & 1 else 0x80200000
                gpu = 0x80380000 if (number >> 4) & 1 else 0x80280000
                regions = [(address_of('RaceContext'), 0x568), (ALT, 0x188),
                           (address_of('SecondaryMotionClock'), 20), (address_of('PrimaryMotionClock'), 20),
                           (SCRIPTS[0], 32), (SCRIPTS[1], 32), (SCRATCH, 0x7C),
                           (frame, 0x22778), (frame+0x40000, 0x22778), (gpu, 8192),
                           (address_of('CurrentDrawBuf'), 4), (address_of('VSyncThreshold'), 4)]
                self.assertEqual(sum(length for _, length in regions), size)

                def write_state(data):
                    offset = 0
                    for address, length in regions:
                        write(address, data[offset:offset+length]);offset += length

                def state():
                    return b''.join(read(address, length) for address, length in regions)

                write_state(C.string_at(native.initial_wire(), size))
                write(STACK-128, b'\xA5'*256)
                cpu.reset();cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
                cpu.r[29], cpu.r[31], cpu.pc = STACK, STOP, address_of(NAME)
                index = 0
                for instruction in range(2000):
                    if cpu.pc == STOP:
                        break
                    if cpu.pc in services:
                        self.assertLess(index, events, (number, 'unexpected service'))
                        kind = services[cpu.pc]
                        self.assertEqual(kind, native.event_value(index, 0), (number, index))
                        used = (0 if kind in (9, 14, 15, 16, 17) else 2 if kind in (12, 13, 18) else 1)
                        for argument in range(used):
                            self.assertEqual(cpu.r[4+argument], native.event_value(index, argument+1),
                                             (number, index, 'argument', argument))
                        self.assertEqual(state(), C.string_at(native.event_wire(index, 0), size),
                                         (number, index, 'service entry'))
                        write_state(C.string_at(native.event_wire(index, 1), size))
                        for register in (1, 2, 3, *range(4, 16), 24, 25):
                            cpu.r[register] = (0xA1230000+index*0x101+register) & 0xFFFFFFFF
                        cpu.pc = cpu.r[31];index += 1
                    else:
                        cpu.step()
                self.assertEqual(cpu.pc, STOP, number)
                self.assertEqual(index, events, number)
                self.assertEqual(state(), C.string_at(native.final_wire(), size), (number, 'final state'))
                self.assertEqual(cpu.r[29], STACK)
                self.assertEqual(cpu.r[31], STOP)
                for register in (*range(16, 24), 28, 30):
                    self.assertEqual(cpu.r[register], 0xBEEF0000+register)
                self.assertEqual(read(STACK-128, 64), b'\xA5'*64)
                self.assertEqual(read(STACK, 128), b'\xA5'*128)
            print('PASS4096 native and512', 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
                  'MIPS camera cases;full owners/pointers, complete service states, two DMA maps, aliases and UBSan')


if __name__ == '__main__':
    unittest.main()
