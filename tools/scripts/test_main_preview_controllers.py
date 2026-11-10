"""Current main preview controllers: public RGB ABI and complete native owners.

Stateful service doubles compare every entry and final state against an
independent transition model. Real matrix/reset helpers run in the native path.
Wire traces also check original or linked MIPS, including the shared selector
alias, full native arena pointers and the three defined color bytes.
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
NAMES = ['CarPreviewModeUpdate', 'GarageCarPreviewModeUpdate']
SERVICES = ['SetupViewport', 'StepCarShowcaseScene', 'CdAudioFade', 'InitCarPreviewScene',
            'BuildCameraMatrices', 'ScaleMatrixRow1', 'UpdateCarWorldTransform', 'DrawPreviewCar',
            'UpdateImageRow', 'SeqStartTrack', 'UpdateScreenFade', 'SetMenuFrameOptions',
            'LoadCarRangeStep', 'PlaySfx', 'StepMenuValue', 'ResetScreenCounters',
            'InitMotionController', 'SetupMenuCarCamera', 'DrawUiRow', 'DrawSelectionIconGrid',
            'UpdateBadgeRowAnim']
STACK, STOP = 0x803F0000, 0x803FF000
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "vehicle_wire.h"
#include "rr/current_selection_types.h"
#include "rr/menu_arena_state.h"
#include "rr/projection_state.h"
#include "rr/ui.h"
#include "rr/badge_row_types.h"
#include "game/ui/uitext.h"
#include "game/car/carwheel.h"
#include "game/car/carbound.h"
#include "game/menu/menuwork.h"
_Static_assert(sizeof(ImageRow20)==20&&sizeof(BadgeRow1C)==28,"wire row storage");
enum {WIRE_SIZE=1600+80+288+112+44+32+72+6144+8+16+4+2+4+2+2+2+4+1+1+4,MAX_EVENTS=24};
typedef struct {
 u32 before[4];RaceVehicle cars[2];ImageRow20 images[2][2];UiRow24 rows[2][4];
 BadgeRow1C badges[2][2];MenuSettings2C settings;CurrentSelection choice;
 ViewMatrixScratch48 scratch;u32 buffers[3][512];MenuArenas8 arenas;void *cursor;
 s32 frame,accepted,screen;s16 group,next,fade;u16 phase;
 u8 load,step,table[4];unsigned imageRoot,rowRoot,badgeRoot;u32 after[4];
} State;
typedef struct {u32 kind,args[5],result;State entry;} Event;
typedef struct {unsigned calls;Event events[MAX_EVENTS];State final;} Capture;
typedef struct {u32 kind,args[5],result;u8 before[WIRE_SIZE],after[WIRE_SIZE];} WireEvent;
static State live,initial;static Capture actual,expected,*capture;
static WireEvent trace[MAX_EVENTS];static u8 firstWire[WIRE_SIZE],lastWire[WIRE_SIZE];
static unsigned scenario,kind,mutation,modelRun,serial,recordWire;
static unsigned serviceSeen[22],phaseSeen[6],readySeen[6],drawSeen[2],frameWrapSeen,rotateWrapSeen,publicationSeen;
static u32 timersSeen[8];
static const u16 phases[]={0,1,2,3,32768,65535};
static const s32 statuses[]={-1,0,1,2,INT32_MIN,INT32_MAX};
static const s32 frames[]={INT32_MIN,-1,0,1,INT32_MAX};
static const s8 modes[]={INT8_MIN,-1,0,1,2,3,INT8_MAX};
#define PlayerCars live.cars
#define ImageRows live.images[live.imageRoot]
#define UiRows live.rows[live.rowRoot]
#define BadgeRows live.badges[live.badgeRoot]
#define SelectionState ((u8*)&live.settings)
#define SelectionRows ((u8*)&live.choice)
#define MenuArenas live.arenas
#define LoadCursor live.cursor
#define ModeFrame live.frame
#define D_800F3A60 live.accepted
#define SubmodeState live.phase
#define ModeGroup live.group
#define ModeIndex live.next
#define FadeCountdown live.fade
#define GpStageShowcaseModes live.table
#define LoadState live.load
#define MenuScriptStep live.step
#define ScreenFrame live.screen
#define SCRATCH_ROT_MATRIX ((u8*)&live.scratch.matrix28)
static void native_scale(s32,MatrixWords32 *);
static void native_reset(void);
static void put32(void *p,u32 v){memcpy(p,&v,4);}
static s32 word(u32 v){s32 s;memcpy(&s,&v,4);return s;}
static u32 pointer(const void *p){
 uintptr_t n=(uintptr_t)p,b;
 struct {const void *p;u32 address,size;} regions[]={
  {live.images,0x80210000,sizeof(live.images)},{live.rows,0x80211000,sizeof(live.rows)},
  {live.badges,0x80212000,sizeof(live.badges)},{&live.choice,@CHOICE@,sizeof(live.choice)},
  {&live.scratch.matrix28,0x1F800028,32},{live.buffers,0x80200000,sizeof(live.buffers)}};
 if(p==live.cars)return @CARS@;
 for(unsigned i=0;i<sizeof(regions)/sizeof(regions[0]);i++){
  b=(uintptr_t)regions[i].p;if(n>=b&&n-b<regions[i].size)return regions[i].address+(u32)(n-b);
 }
 assert(!"pointer outside complete fixture owners");return 0;
}
static void wire(u8 *out){
 u8 *p=out;
 for(unsigned i=0;i<2;i++){rr_vehicle_wire_store_fields(p,&live.cars[i]);p+=800;}
 memcpy(p,live.images,80);p+=80;memcpy(p,live.rows,288);p+=288;memcpy(p,live.badges,112);p+=112;
 memcpy(p,&live.settings,44);p+=44;memcpy(p,&live.choice,32);p+=32;
 put32(p,pointer(live.scratch.projection.pointers.cursor));put32(p+4,pointer(live.scratch.projection.pointers.order));p+=8;
 memcpy(p,&live.scratch.projection.pose08.position00,16);p+=16;memcpy(p,&live.scratch.projection.pose08.angles10,16);p+=16;
 memcpy(p,&live.scratch.matrix28,32);p+=32;memcpy(p,live.buffers,6144);p+=6144;
 put32(p,pointer(live.arenas.first));put32(p+4,pointer(live.arenas.second));p+=8;
 put32(p,pointer(live.cursor));put32(p+4,pointer(ImageRows));put32(p+8,pointer(UiRows));put32(p+12,pointer(BadgeRows));p+=16;
 memcpy(p,&live.frame,4);p+=4;memcpy(p,&live.phase,2);p+=2;memcpy(p,&live.accepted,4);p+=4;
 memcpy(p,&live.group,2);p+=2;memcpy(p,&live.next,2);p+=2;memcpy(p,&live.fade,2);p+=2;
 memcpy(p,&live.screen,4);p+=4;*p++=live.load;*p++=live.step;memcpy(p,live.table,4);p+=4;
 assert(p-out==WIRE_SIZE);
}
static void mutate(unsigned operation){
 unsigned n=serial++;if(!mutation)return;
 u32 v=scenario*0x17171717U+n*0x1234567U+operation;
 live.imageRoot=(scenario+n)&1;live.rowRoot=(scenario+n+1)&1;live.badgeRoot=(scenario/3+n)&1;
 live.phase=phases[(scenario+n)%6];live.accepted=word((n&1)?v:0);
 live.frame=word(v);live.group=(s16)v;live.next=(s16)~v;live.fade=(s16)(v>>7);
 live.settings.phase17=kind?(s8)v:(s8)(((scenario+n)%4)*2);
 live.settings.mode1A=(s16)v;live.choice.choice.bytes[3]=(u8)((scenario+n)&1);
 live.cursor=(u8*)live.buffers[(scenario+n)%3]+(v&255);
 live.arenas.first=(u8*)live.buffers[(scenario+n+1)%3]+(v&127);
 live.arenas.second=(u8*)live.buffers[(scenario+n+2)%3]+(v&63);
 live.scratch.projection.pointers.cursor=(u8*)live.buffers[n%3]+128;
 live.scratch.projection.pointers.order=live.buffers[(n+1)%3]+16;
 for(unsigned bank=0;bank<2;bank++){
  live.images[bank][0].f1=modes[(scenario+n+bank)%7];live.images[bank][0].f2=(u8)(v+bank*73U);
  live.rows[bank][0].byte14=(u8)((scenario+n+bank)%6);live.rows[bank][1].byte14=(u8)((scenario+n+bank+1)%6);
  live.badges[bank][0].half02=(s16)v;live.badges[bank][0].state10=word(v);
  live.cars[bank].rot54=word(v+bank*0x1234567U);
 }
}
static u32 service(unsigned operation,u32 a,u32 b,u32 c,u32 d,u32 e){
 unsigned index=capture->calls++;assert(index<MAX_EVENTS);Event *event=&capture->events[index];
 event->kind=operation;event->args[0]=a;event->args[1]=b;event->args[2]=c;event->args[3]=d;event->args[4]=e;
 unsigned status=(scenario/4+serial)%6;u32 result=(u32)statuses[status];
 if(operation==13||operation==2)readySeen[status]++;
 event->result=result;memcpy(&event->entry,&live,sizeof(live));serviceSeen[operation]++;
 if(recordWire){trace[index].kind=operation;memcpy(trace[index].args,event->args,20);trace[index].result=result;wire(trace[index].before);}
 if(operation==6){
  if(modelRun){for(unsigned i=0;i<3;i++){u16 h;memcpy(&h,(u8*)&live.scratch.matrix28+6+i*2,2);h=(u16)((u32)h*2);memcpy((u8*)&live.scratch.matrix28+6+i*2,&h,2);}}
  else native_scale(2,&live.scratch.matrix28);
 }else if(operation==16){
  if(modelRun){live.load=0;live.step=0;live.screen=0;}else native_reset();
 }else if(operation==15){live.choice.choice.bytes[3]=(u8)((scenario+serial)&1);}
 else if(operation==5){for(unsigned i=0;i<8;i++)live.scratch.matrix28.w[i]=word(scenario+i*0x17171717U);}
 mutate(operation);if(recordWire)wire(trace[index].after);return result;
}
void SetupViewport(s32 a){assert(a==6);(void)service(1,(u32)a,0,0,0,0);}
int StepCarShowcaseScene(int a){return word(service(2,(u32)a,0,0,0,0));}
void CdAudioFade(s32 a,s32 b){(void)service(3,(u32)a,(u32)b,0,0,0);}
void InitCarPreviewScene(void){(void)service(4,0,0,0,0,0);}
void BuildCameraMatrices(void){(void)service(5,0,0,0,0,0);}
void ScaleMatrixRow1(s32 a,MatrixWords32 *p){assert(a==2&&p==&live.scratch.matrix28);(void)service(6,(u32)a,pointer(p),0,0,0);}
void UpdateCarWorldTransform(RaceVehicle *p){assert(p==live.cars);(void)service(7,pointer(p),0,0,0,0);}
void DrawPreviewCar(RaceVehicle *p,PreviewRgb3 rgb,s32 scale,s32 flags){
 assert(p==live.cars&&(uintptr_t)p>UINT32_MAX);u32 color=(u32)rgb.r|((u32)rgb.g<<8)|((u32)rgb.b<<16);
 assert(color==(kind?0x808080U:0U));(void)service(8,pointer(p),color,(u32)scale,(u32)flags,0);
}
int UpdateImageRow(int a){return word(service(9,(u32)a,0,0,0,0));}
void SeqStartTrack(s32 a){(void)service(10,(u32)a,0,0,0,0);}
void UpdateScreenFade(s32 a){(void)service(11,(u32)a,0,0,0,0);}
void SetMenuFrameOptions(s32 a,s32 b,s32 c){(void)service(12,(u32)a,(u32)b,(u32)c,0,0);}
int LoadCarRangeStep(s32 a,s32 b){assert(a==1&&b==3);return word(service(13,(u32)a,(u32)b,0,0,0));}
void PlaySfx(s32 a){(void)service(14,(u32)a,0,0,0,0);}
void StepMenuValue(s32 a,s32 b,char *p,s32 d,s32 e){assert(p==(char*)&live.choice.choice.bytes[3]);(void)service(15,(u32)a,(u32)b,pointer(p),(u32)d,(u32)e);}
void ResetScreenCounters(void){(void)service(16,0,0,0,0,0);}
void InitMotionController(void){(void)service(17,0,0,0,0,0);}
void SetupMenuCarCamera(s32 a){(void)service(18,(u32)a,0,0,0,0);}
int DrawUiRow(s32 a){return word(service(19,(u32)a,0,0,0,0));}
void DrawSelectionIconGrid(s32 a){(void)service(20,(u32)a,0,0,0,0);}
void UpdateBadgeRowAnim(s32 a){(void)service(21,(u32)a,0,0,0,0);}
'''
REFERENCE = r'''
static void reference(void){
 frameWrapSeen+=live.frame==INT32_MAX;live.frame=word((u32)live.frame+1);
 if(!kind){
  SetupViewport(6);
  if(!live.phase){
   if(StepCarShowcaseScene(live.table[live.settings.phase17/2])){
    live.fade=64;CdAudioFade(0,60);live.phase=1;InitCarPreviewScene();ImageRows->f1=0;
   }
  }else{
   BuildCameraMatrices();ScaleMatrixRow1(2,&live.scratch.matrix28);UpdateCarWorldTransform(live.cars);
   PreviewRgb3 black={0,0,0};DrawPreviewCar(live.cars,black,256,1);UpdateImageRow(0);
   if(ImageRows->f1==2){live.group=1;live.next=34;live.phase=0;SeqStartTrack(0);}
  }
  UpdateScreenFade(0x10028);SetMenuFrameOptions(0,0,0);return;
 }
 UpdateScreenFade(0x10022);s32 ready=LoadCarRangeStep(1,3);
 if(ready==1){
  live.arenas.first=live.cursor;assert((uintptr_t)live.arenas.first>UINT32_MAX);publicationSeen++;
  switch(live.phase){
  case 0:
   if(UiRows->byte14==2){
    SetMenuFrameOptions(1,1,0);UiRows->cells[0]=2;
    if(live.accepted){PlaySfx(1);UiRows->cells[0]=1;UiRows[1].byte14=0;UiRows[2].byte14=0;live.phase=1;}
   }break;
  case 1:{
   u8 status=UiRows[1].byte14;
   if(status==2){
    UiRows[live.choice.choice.bytes[3]+1].cells[0]=0;
    StepMenuValue(0,1,(char*)&live.choice.choice.bytes[3],2,0);UiRows[live.choice.choice.bytes[3]+1].cells[0]=2;
    if(live.accepted){PlaySfx(1);UiRows[live.choice.choice.bytes[3]+1].cells[0]=1;for(unsigned i=0;i<3;i++)UiRows[i].byte14=3;}
   }else if(status==5)live.phase=2;
  }break;
  case 2:
   live.frame=0;ImageRows->f1=3;BadgeRows->state10=4;ResetScreenCounters();
   memcpy(live.settings.copies[0].bytes,live.choice.choice.bytes,4);
   if(live.settings.phase17<=0)live.settings.mode1A=2;
   InitMotionController();live.group=1;live.next=11;live.phase=0;break;
  default:break;
  }
 }
 SetupMenuCarCamera(1);SetupViewport(6);
 unsigned draw=ImageRows->f1>0;drawSeen[draw]++;
 if(draw){
  rotateWrapSeen+=live.cars[0].rot54>INT32_MAX-8;
  live.cars[0].rot54=word(((u32)live.cars[0].rot54+8)&4095);
  unsigned timer=ImageRows->f2;s32 scale=ImageRows->f1==2?256:(s32)(timer*256/30);
  if(ImageRows->f1!=2)timersSeen[timer/32]|=1U<<(timer%32);
  BadgeRows->half02=1;DrawUiRow(0);DrawUiRow(1);DrawUiRow(2);DrawSelectionIconGrid(0);UpdateCarWorldTransform(live.cars);
  PreviewRgb3 gray={128,128,128};DrawPreviewCar(live.cars,gray,scale,1);
 }
 UpdateBadgeRowAnim(0);UpdateImageRow(0);
}
static void initialize(unsigned number){
 scenario=number;kind=number&1;mutation=(number>>1)&1;serial=recordWire=0;memset(&live,0,sizeof(live));
 rr_vehicle_wire_reset();CarRecord *record=rr_vehicle_record_resolve(0x80180010U);
 for(unsigned i=0;i<2;i++){
  live.cars[i].control.record18=record;live.cars[i].control.handling1C=&record->handling1C8;
  live.cars[i].rot54=frames[(number/256+i)%5];
 }
 for(unsigned bank=0;bank<2;bank++){
  live.images[bank][0].f1=modes[(number/4096+bank)%7];live.images[bank][0].f2=(u8)(number/4);
  live.rows[bank][0].byte14=(u8)((number/8+bank)%6);live.rows[bank][1].byte14=(u8)((number/16+bank)%6);
  for(unsigned i=0;i<4;i++)live.rows[bank][i].cells[0]=(u8)(number+i*17U);
 }
 live.imageRoot=(number>>2)&1;live.rowRoot=(number>>3)&1;live.badgeRoot=(number>>4)&1;
 live.phase=phases[(number/512)%6];phaseSeen[(number/512)%6]++;
 live.frame=frames[(number/128)%5];live.accepted=word((number&8)?number*0x1234567U:0);
 live.settings.phase17=kind?(s8)(number/32):(s8)(((number/8)%4)*2);
 live.choice.choice.bytes[3]=(u8)((number/8)&1);
 for(unsigned i=0;i<3;i++)live.choice.choice.bytes[i]=(u8)(number+i*73U);
 live.arenas.first=live.buffers[0];live.arenas.second=(u8*)live.buffers[1]+32;live.cursor=(u8*)live.buffers[2]+64;
 live.scratch.projection.pointers.cursor=(u8*)live.buffers[0]+128;live.scratch.projection.pointers.order=live.buffers[1]+16;
 for(unsigned i=0;i<8;i++)live.scratch.matrix28.w[i]=word(number+i*0x17171717U);
 live.screen=word(~number);live.load=(u8)number;live.step=(u8)~number;
 for(unsigned i=0;i<4;i++){live.before[i]=number+i;live.after[i]=~number+i;live.table[i]=(u8)(number+i*73U);}
 assert(sizeof(PreviewRgb3)==4&&__alignof__(PreviewRgb3)==4);
 assert(sizeof(void*)==8&&(uintptr_t)&live>UINT32_MAX&&sizeof(MenuArenas8)==16);
 memcpy(&initial,&live,sizeof(live));
}
unsigned exercise_native(void){
 for(unsigned n=0;n<32768;n++){
  initialize(n);capture=&expected;capture->calls=0;modelRun=1;reference();memcpy(&expected.final,&live,sizeof(live));
  memcpy(&live,&initial,sizeof(live));serial=0;capture=&actual;capture->calls=0;modelRun=0;
  if(kind)GarageCarPreviewModeUpdate();else CarPreviewModeUpdate();memcpy(&actual.final,&live,sizeof(live));
  assert(actual.calls==expected.calls);
  for(unsigned i=0;i<actual.calls;i++)assert(!memcmp(&actual.events[i],&expected.events[i],sizeof(Event)));
  assert(!memcmp(&actual.final,&expected.final,sizeof(State)));
 }
 for(unsigned i=1;i<=21;i++)assert(serviceSeen[i]);
 for(unsigned i=0;i<6;i++)assert(phaseSeen[i]&&readySeen[i]);
 for(unsigned i=0;i<8;i++)assert(timersSeen[i]==0xFFFFFFFFU);
 assert(drawSeen[0]&&drawSeen[1]&&frameWrapSeen&&rotateWrapSeen&&publicationSeen);return 32768;
}
unsigned prepare_trace(unsigned n){initialize(n);wire(firstWire);capture=&expected;capture->calls=0;modelRun=1;recordWire=1;reference();wire(lastWire);recordWire=0;return capture->calls;}
const void *initial_wire(void){return firstWire;}
const void *final_wire(void){return lastWire;}
const void *event_wire(unsigned i,unsigned after){assert(i<capture->calls);return after?trace[i].after:trace[i].before;}
u32 event_value(unsigned i,unsigned field){assert(i<capture->calls&&field<7);return field==0?trace[i].kind:field==6?trace[i].result:trace[i].args[field-1];}
unsigned wire_size(void){return WIRE_SIZE;}
'''


class MainPreviewControllerTest(unittest.TestCase):
    def test_full_native_entries_and_original_mips(self):
        owners = source_owners(ROOT/'src/main/PAL')
        body = '\n'.join(definition_source(owners[name].read_text(), name) for name in NAMES)
        scale = definition_source(owners['ScaleMatrixRow1'].read_text(), 'ScaleMatrixRow1').replace('ScaleMatrixRow1', 'native_scale')
        reset = definition_source(owners['ResetScreenCounters'].read_text(), 'ResetScreenCounters').replace('ResetScreenCounters', 'native_reset')
        prefix = PREFIX.replace('@CHOICE@', hex(address_of('SelectionRows'))+'U').replace('@CARS@', hex(address_of('PlayerCars'))+'U')
        self.assertEqual(address_of('D_800AD4EF'), address_of('SelectionState')+0x17)
        with TemporaryDirectory() as temporary:
            source, library = Path(temporary)/'native.c', Path(temporary)/'native.dylib'
            source.write_text(prefix+scale+reset+body+REFERENCE)
            subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                            '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-I', str(ROOT/'include'),
                            '-I', str(ROOT/'tools/scripts'), str(source), '-o', str(library)], check=True)
            native = C.CDLL(str(library));native.exercise_native.restype = C.c_uint
            self.assertEqual(native.exercise_native(), 32768)
            native.prepare_trace.argtypes = [C.c_uint];native.prepare_trace.restype = C.c_uint
            native.initial_wire.restype = native.final_wire.restype = C.c_void_p
            native.event_wire.argtypes = [C.c_uint, C.c_uint];native.event_wire.restype = C.c_void_p
            native.event_value.argtypes = [C.c_uint, C.c_uint];native.event_value.restype = C.c_uint32
            native.wire_size.restype = C.c_uint
            regions = [(address_of('PlayerCars'),1600),(0x80210000,80),(0x80211000,288),(0x80212000,112),
                       (address_of('SelectionState'),44),(address_of('SelectionRows'),32),(0x1F800000,72),(0x80200000,6144),
                       (address_of('MenuArenas'),8),(address_of('LoadCursor'),4),(address_of('ImageRows'),4),
                       (address_of('UiRows'),4),(address_of('BadgeRows'),4),(address_of('ModeFrame'),4),
                       (address_of('SubmodeState'),2),(address_of('D_800F3A60'),4),(address_of('ModeGroup'),2),
                       (address_of('ModeIndex'),2),(address_of('FadeCountdown'),2),(address_of('ScreenFrame'),4),
                       (address_of('LoadState'),1),(address_of('MenuScriptStep'),1),(address_of('GpStageShowcaseModes'),4)]
            size = native.wire_size();self.assertEqual(sum(n for _,n in regions),size)
            intervals = sorted((a,a+n) for a,n in regions)
            self.assertTrue(all(a[1]<=b[0] for a,b in zip(intervals,intervals[1:])))
            memory = Uc(UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN | UC_MODE_MIPS32)
            memory.mem_map(0,0x400000);memory.mem_map(0x1F800000,4096)
            write = lambda a,b:memory.mem_write(a&0x1FFFFFFF,bytes(b))
            read = lambda a,n:bytes(memory.mem_read(a&0x1FFFFFFF,n))
            original = (ROOT/'assets/PAL/main.exe').read_bytes();base=struct.unpack_from('<I',original,0x18)[0]
            write(base,original[0x800:])
            if os.getenv('RR_COMPILED_ELF'):
                with open(os.environ['RR_COMPILED_ELF'],'rb') as stream:
                    elf=ELFFile(stream)
                    for name in NAMES:
                        symbol=elf.get_section_by_name('.symtab').get_symbol_by_name(name)[0]
                        section=elf.get_section(symbol['st_shndx']);offset=symbol['st_value']-section['sh_addr']
                        self.assertEqual(symbol['st_value'],address_of(name));write(symbol['st_value'],section.data()[offset:offset+symbol['st_size']])
            services={address_of(n):i+1 for i,n in enumerate(SERVICES)}
            counts=[1,1,2,0,0,2,1,4,1,1,1,3,2,1,5,0,0,1,1,1,1]

            def store(cpu,access,address,length,value,user):
                address&=0x1FFFFFFF
                allowed=regions+[(STACK-64,64)]
                self.assertTrue(any((a&0x1FFFFFFF)<=address and address+length<=(a&0x1FFFFFFF)+n for a,n in allowed),(hex(address),length))

            cpu=Cpu(memory,0,100,101,102,store)
            for case in range(1024):
                number=(case*7919)&32767;events=native.prepare_trace(number)

                def write_state(data):
                    offset=0
                    for address,length in regions:write(address,data[offset:offset+length]);offset+=length

                def state():return b''.join(read(a,n) for a,n in regions)

                write_state(C.string_at(native.initial_wire(),size));write(STACK-128,b'\xA5'*256)
                cpu.reset();cpu.r=[0]+[0xBEEF0000+r for r in range(1,32)]
                cpu.r[29],cpu.r[31],cpu.pc=STACK,STOP,address_of(NAMES[number&1]);event=0
                for instruction in range(1500):
                    if cpu.pc==STOP:break
                    if cpu.pc in services:
                        self.assertLess(event,events);operation=services[cpu.pc]
                        self.assertEqual(operation,native.event_value(event,0),(number,event))
                        for index in range(counts[operation-1]):
                            value=cpu.r[4+index] if index<4 else int.from_bytes(read(cpu.r[29]+16,4),'little')
                            if operation==8 and index==1:value&=0xFFFFFF
                            self.assertEqual(value,native.event_value(event,index+1),(number,event,index))
                        self.assertEqual(state(),C.string_at(native.event_wire(event,0),size),(number,event,'entry'))
                        write_state(C.string_at(native.event_wire(event,1),size))
                        for register in (1,2,3,*range(4,16),24,25):cpu.r[register]=(0xA1230000+event*257+register)&0xFFFFFFFF
                        cpu.r[2]=native.event_value(event,6);cpu.pc=cpu.r[31];event+=1
                    else:cpu.step()
                self.assertEqual(cpu.pc,STOP,number);self.assertEqual(event,events,number)
                self.assertEqual(state(),C.string_at(native.final_wire(),size),(number,'final'))
                self.assertEqual(cpu.r[29],STACK);self.assertEqual(cpu.r[31],STOP)
                for register in (*range(16,24),28,30):self.assertEqual(cpu.r[register],0xBEEF0000+register)
                self.assertEqual(read(STACK-128,64),b'\xA5'*64);self.assertEqual(read(STACK,128),b'\xA5'*128)
            print('PASS32768 native and1024','compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
                  'MIPS main preview cases;RGB ABI, complete states, native pointers, aliases and UBSan')


if __name__=='__main__':unittest.main()
