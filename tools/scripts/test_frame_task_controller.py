"""Actual frame-task controllers and ramp; controlled IO/audio/GPU boundaries.

Full native owners, explicit DMA regions and independent original/linked MIPS
service effects preserve complete state and event images. GPU emission and TIM
parsing algorithms are outside this controller fixture.
"""
import ctypes as C
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest
from unicorn import Uc,UC_ARCH_MIPS,UC_MODE_MIPS32,UC_MODE_LITTLE_ENDIAN
from c_functions import address_of,definition_source,source_owners
from test_vehicle_initializer import Cpu

ROOT=Path(__file__).resolve().parents[2]
SCRIPT=(0x80240010,0x80241010,0x80242010)
FRAME=(0x80180010,0x801C0010)
SCRATCH,STACK,STOP=0x1F800000,0x803F0000,0x803FF000
FRAME_SIZE=0x22778
PREFIX=r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "game/system/frmtask.h"
#include "game/effect/spfade.h"
#include "game/gfx/drawlist.h"
#include "game/data/cdfile1.h"
#include "game/data/cdfile2.h"
#include "game/course/trkintp2.h"
#include "game/sound/cdpos.h"
#include "game/sound/cdplay.h"
#include "game/sound/cdstart.h"
#include "game/ui/uipart1.h"
#include "rr/frame_loop.h"
#include "rr/gpu_scratch_types.h"
#include "rr/gpu_address.h"
#include "rr/screen_tiles.h"
#include "rr/menu_background.h"
enum {WIRE=860,MAX_EVENTS=16};
typedef struct {u32 before[4];FrameTaskState value;u32 after[4];} ScriptOwner;
typedef struct {u32 before[4];FrameStorage value;u32 after[4];} FrameOwner;
typedef struct {u32 before[4],words[64],after[4];} Arena;
typedef struct {
 u32 before[4];FrameOwner frames[2];ScriptOwner scripts[3];Arena arenas[2];
 GpuScratchPointers scratch;FrameTaskState *script;u8 *load;FrameStorage *frame;
 s32 phase,accepted;s16 fade,submode;u16 group,index;u32 after[4];
} State;
typedef struct {u32 kind,args[3],result;u8 before[WIRE],after[WIRE];} Event;
typedef struct {u32 count;Event events[MAX_EVENTS];} Capture;
static State live,initial,wanted;static Capture actual,expected,*capture;
static unsigned scenario,kind;static u32 bases[2];static GpuDmaRegion regions[2];
static const s32 phases[16]={0,1,2,3,4,5,6,7,8,-1,INT32_MIN,INT32_MAX,9,65535,32768,-9};
static const s16 frames[10]={INT16_MIN,-1,0,7,8,31,32,63,64,INT16_MAX};
static const s16 fades[8]={INT16_MIN,-1,0,1,2,63,64,INT16_MAX};
static const s16 levels[10]={0,1,127,128,129,INT16_MAX,INT16_MIN,-1,2,64};
static const s16 steps[8]={0,1,8,4,-1,-8,INT16_MAX,INT16_MIN};
static u32 seen[14],stageSeen[16],aliasSeen[6],endSeen[2];
#define FrmTaskState live.phase
#define ActiveFrameTaskScript live.script
#define LoadCursor live.load
#define CurrentDrawBuf live.frame
#define FadeCountdown live.fade
#define MenuReturnSubmodeIndex live.accepted
#define ModeGroup live.group
#define ModeIndex live.index
#define SubmodeState live.submode
#define SCRATCHPAD (&live.scratch)
static s32 signed_word(u32 n){s32 x;memcpy(&x,&n,4);return x;}
static s16 signed_half(u16 n){s16 x;memcpy(&x,&n,2);return x;}
static u32 script_address(unsigned i){return 0x80240010U+i*0x1000U;}
static u32 frame_address(unsigned i){return 0x80180010U+i*0x40000U;}
static u32 pointer(const void *p){
 if(!p)return 0;uintptr_t address=(uintptr_t)p;
 for(unsigned i=0;i<3;i++){uintptr_t base=(uintptr_t)&live.scripts[i].value;if(address>=base&&address-base<=16)return script_address(i)+(u32)(address-base);}
 for(unsigned i=0;i<2;i++){
  uintptr_t base=(uintptr_t)&live.frames[i].value;if(address>=base&&address-base<=sizeof(FrameStorage))return frame_address(i)+(u32)(address-base);
  base=(uintptr_t)live.arenas[i].words;if(address>=base&&address-base<=256)return bases[i]+(u32)(address-base);
 }
 assert(!"pointer outside complete owners");return 0;
}
static void wire(u8 *out){
 u8 *p=out;memcpy(p,live.before,16);p+=16;memcpy(p,live.after,16);p+=16;
 memcpy(p,live.scripts,sizeof(live.scripts));p+=sizeof(live.scripts);
 memcpy(p,live.arenas,sizeof(live.arenas));p+=sizeof(live.arenas);
 for(unsigned i=0;i<2;i++){memcpy(p,live.frames[i].before,16);p+=16;memcpy(p,live.frames[i].after,16);p+=16;}
 for(unsigned i=0;i<2;i++){memcpy(p,live.frames[i].value.buffer.ordering70.words,4);p+=4;}
 u32 roots[5]={pointer(live.script),pointer(live.load),pointer(live.scratch.cursor),pointer(live.scratch.order),pointer(live.frame)};
 memcpy(p,roots,20);p+=20;memcpy(p,&live.phase,4);p+=4;memcpy(p,&live.accepted,4);p+=4;
 memcpy(p,&live.fade,2);p+=2;memcpy(p,&live.submode,2);p+=2;memcpy(p,&live.group,2);p+=2;memcpy(p,&live.index,2);p+=2;
 assert(p-out==WIRE);
}
static unsigned script_index(u32 token){for(unsigned i=0;i<3;i++)if(token==script_address(i))return i;assert(0);return 0;}
static u32 result(unsigned operation,unsigned serial){
 if(operation==2)return (scenario&1)?0:0x80000001U;
 if(operation==3){const u32 values[4]={0,1,0xFFFFFFFFU,0x7FFFFFFFU};return values[(scenario/16)%4];}
 return 0x80000000U^(scenario*17U+operation*257U+serial);
}
static void select_head(unsigned bank,unsigned offset){
 unsigned alias=scenario%6;aliasSeen[alias]++;
 unsigned at=alias==1?offset:alias==2?offset+4:alias==3?offset+8:alias==5?252:0;if(at>252)at=252;
 live.scratch.order=alias==4?live.frames[(scenario/4)&1].value.buffer.ordering70.words:
 (u32*)((u8*)live.arenas[alias?bank:1-bank].words+at);
}
static void effect(unsigned operation,unsigned serial,const u32 *args){
 if(operation==11||operation==12){
  u32 token=pointer(live.scratch.cursor);unsigned bank=(token&0x1FFFFFFFU)>=0x281000U;
  u32 offset=(token&0x1FFFFFFFU)-(bases[bank]&0x1FFFFFFFU);assert(offset+24<=256);
  live.arenas[bank].words[offset/4+1]=0x60000000U|(operation<<16)|(scenario&0xFFFF);
  live.scratch.cursor+=(operation==11?20:24);
  if(operation==12)live.scripts[script_index(args[1])].value.fields.spriteTick++;
 }
 if(scenario&32){
  unsigned slot=(scenario+serial)%3,bank=(scenario+serial)&1;
  live.script=&live.scripts[slot].value;live.load=(u8*)&live.scripts[(scenario+2*serial)%3].value;
  live.frame=&live.frames[bank].value;live.phase=signed_word(0x81230000U+scenario*31U+serial);
  live.fade=fades[(scenario+serial)%8];live.accepted=((scenario+serial)&1)?signed_word(0xFEDC0000U+serial):0;
  live.group=(u16)(scenario*37U+serial);live.index=(u16)(scenario*19U+serial);live.submode=signed_half((u16)(scenario*7U+serial));
  live.scripts[slot].value.fields.frame=frames[(scenario+serial)%10];
  live.scripts[slot].value.fields.intensity[serial%3]^=(s16)(0x1111U+serial);
  live.frames[bank].value.buffer.ordering70.words[0]^=0xA1230000U+scenario+serial;
  unsigned offset=16+((scenario+serial)%8)*4;live.scratch.cursor=(u8*)live.arenas[bank].words+offset;select_head(bank,offset);
 }
 if(operation==12&&(scenario&16)){
  unsigned bank=(scenario/4)&1;live.scratch.cursor=(u8*)live.arenas[bank].words+248;select_head(bank,248);
 }
}
static u32 service(unsigned operation,u32 a,u32 b,u32 c){
 unsigned serial=capture->count++;assert(serial<MAX_EVENTS);Event *event=&capture->events[serial];
 memset(event,0,sizeof(*event));event->kind=operation;event->args[0]=a;event->args[1]=b;event->args[2]=c;event->result=result(operation,serial);seen[operation]++;
 wire(event->before);effect(operation,serial,event->args);wire(event->after);return event->result;
}
int GetR4FileIndex(int category,int index){return signed_word(service(1,(u32)category,(u32)index,0));}
s32 CdFileReadStart(s32 index,void *buffer){return signed_word(service(2,(u32)index,pointer(buffer),0));}
s32 CdFileBytesLoaded(void){return signed_word(service(3,0,0,0));}
void CdAudioInit(void){(void)service(4,0,0,0);}
void CdAudioSelectTrack(int first,int second){(void)service(5,(u32)first,(u32)second,0);}
void CdAudioBeginPlay(int track){(void)service(6,(u32)track,0,0);}
void LoadTimList(int *words){(void)service(7,pointer(words),0,0);}
void SetMenuFrameOptions(s32 a,s32 b,s32 c){(void)service(8,(u32)a,(u32)b,(u32)c);}
void AddScreenFadeTiles(u32 *head,s32 shade,u32 mode){(void)service(9,pointer(head),(u32)shade,mode);}
void CdAudioFade(s32 target,s32 duration){(void)service(10,(u32)target,(u32)duration,0);}
void EmitPresetSprite(s32 intensity,s32 index){(void)service(11,(u32)intensity,(u32)index,0);}
void EmitSpriteGrid(u32 intensity,SpriteGridClock *state){(void)service(12,intensity,pointer(state),0);}
void UpdateScreenFade(s32 flags){(void)service(13,(u32)flags,0,0);}
'''
TAIL=r'''
static void ramp(unsigned index){
 FrameTaskFields *s=&live.script->fields;s32 step=s->ramp[index];u16 value=(u16)s->intensity[index];
 if(step){value=(u16)(value+(u16)step);s->intensity[index]=signed_half(value);
  if(step>0&&signed_half(value)>=128){s->intensity[index]=128;s->ramp[index]=0;}
  if(step<0&&signed_half(value)<=0){s->intensity[index]=0;s->ramp[index]=0;}
 }
}
static void reference(void){
 if(kind){live.phase=0;live.fade=-1;if(kind==1){live.group=0;live.index=6;live.submode=0;}return;}
 s32 phase=live.phase;
 if(phase==0){u32 index=service(1,0,21,0);if(service(2,index,pointer(live.load),0))live.phase=1;return;}
 if(phase==1){if(service(3,0,0,0))live.phase=2;return;}
 if(phase==2){
  service(4,0,0,0);service(5,0,36,0);service(6,0,0,0);service(7,pointer(live.load),0,0);
  live.phase=7;live.script=(FrameTaskState*)live.load;live.load+=16;
  memset(&live.script->fields,0,sizeof(FrameTaskFields));live.script->fields.ramp[2]=8;phase=7;
 }
 if(phase==7){
  s16 frame=live.script->fields.frame;
  if(frame<64){if(frame==8)live.script->fields.ramp[0]=4;if(frame==32)live.script->fields.ramp[1]=4;}
  else {live.phase=8;phase=8;}
 }
 if(phase==8){
  service(8,1,1,0);
  if(live.fade>0){live.fade--;service(9,pointer(live.frame->buffer.ordering70.words),(u32)((64-live.fade)*4),69);
   if(live.fade==0){live.group=1;live.index=3;live.submode=0;}
  }else if(live.accepted){live.fade=64;service(10,0,64,0);}
 }
 for(unsigned i=0;i<3;i++)ramp(i);
 service(11,(u32)(s32)live.script->fields.intensity[0],47,0);
 service(11,(u32)(s32)live.script->fields.intensity[1],48,0);
 service(12,(u32)(s32)live.script->fields.intensity[2],pointer(live.script),0);
 u8 *p=live.scratch.cursor;u32 value=0xE100022AU;memcpy(p+4,&value,4);
 memcpy(p,live.scratch.order,4);p[3]=1;
 value=pointer(live.scratch.cursor);memcpy(live.scratch.order,&value,4);((u8*)live.scratch.order)[3]=0;
 live.scratch.cursor+=8;endSeen[pointer(live.scratch.cursor)%4096==256]++;
 service(13,0x1000C,0,0);live.script->fields.frame=signed_half((u16)live.script->fields.frame+1U);
}
static void initialize(unsigned number,unsigned selector){
 scenario=number;kind=selector;u32 seed=number*0x9E3779B9U+selector*0x12345U+1;
 for(unsigned i=0;i<sizeof(live);i++){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;((u8*)&live)[i]=(u8)seed;}
 const u32 segments[3]={0,0x80000000U,0xA0000000U};
 for(unsigned i=0;i<2;i++){bases[i]=segments[(number/8+i)%3]|(0x280000U+i*4096U);regions[i]=(GpuDmaRegion){live.arenas[i].words,bases[i],256};}
 assert(GpuDmaSetRegions(regions,2));
 live.phase=phases[number%16];live.fade=fades[(number/16)%8];live.accepted=(number&8)?0:INT32_MIN;
 live.script=&live.scripts[(number/8)%3].value;live.load=(u8*)&live.scripts[(number/4)%3].value;live.frame=&live.frames[number&1].value;
 for(unsigned i=0;i<3;i++){
  live.scripts[i].value.fields.frame=frames[(number/128)%10];live.scripts[i].value.fields.spriteTick=(u16)(number*257U);
  for(unsigned j=0;j<3;j++){live.scripts[i].value.fields.intensity[j]=levels[(number/16+j*3)%10];live.scripts[i].value.fields.ramp[j]=steps[(number/64+j)%8];}
 }
 unsigned bank=(number/2)&1,offset=16+(number%8)*4;live.scratch.cursor=(u8*)live.arenas[bank].words+offset;select_head(bank,offset);
 if((live.phase==0||live.phase==1)&&(number&16)){live.script=0;live.load=0;live.frame=0;live.scratch.cursor=0;live.scratch.order=0;}
 memcpy(&initial,&live,sizeof(live));
 assert(sizeof(void*)==8&&(uintptr_t)&live.arenas[0]>UINT32_MAX);
}
unsigned exercise_native(void){
 unsigned total=0;
 for(unsigned selector=0;selector<3;selector++)for(unsigned n=0;n<(selector?64:4096);n++){
  initialize(n,selector);stageSeen[n%16]++;memset(&expected,0,sizeof(expected));capture=&expected;reference();memcpy(&wanted,&live,sizeof(wanted));
  memcpy(&live,&initial,sizeof(live));memset(&actual,0,sizeof(actual));capture=&actual;
  if(selector==0)FrmTaskModeUpdate();else if(selector==1)FrmTaskModeEnter();else ResetFrmTaskState();
  assert(!memcmp(&actual,&expected,sizeof(actual)));assert(!memcmp(&live,&wanted,sizeof(live)));total++;
 }
 for(unsigned i=1;i<14;i++)assert(seen[i]);for(unsigned i=0;i<16;i++)assert(stageSeen[i]);for(unsigned i=0;i<6;i++)assert(aliasSeen[i]);assert(endSeen[0]&&endSeen[1]);
 assert(GpuDmaSetRegions(0,0));return total;
}
unsigned prepare(unsigned n,unsigned selector){initialize(n,selector);memset(&expected,0,sizeof(expected));capture=&expected;reference();memcpy(&wanted,&live,sizeof(wanted));return expected.count;}
const void *initial_wire(void){static u8 out[WIRE];State saved;memcpy(&saved,&live,sizeof(saved));memcpy(&live,&initial,sizeof(live));wire(out);memcpy(&live,&saved,sizeof(live));return out;}
const void *final_wire(void){static u8 out[WIRE];wire(out);return out;}
const void *event_wire(unsigned index,unsigned after){assert(index<expected.count);return after?expected.events[index].after:expected.events[index].before;}
u32 event_value(unsigned index,unsigned field){assert(index<expected.count&&field<5);Event *e=&expected.events[index];return field==0?e->kind:field==4?e->result:e->args[field-1];}
const void *frame_image(unsigned bank,unsigned after){assert(bank<2);return after?(const void*)&wanted.frames[bank].value:(const void*)&initial.frames[bank].value;}
u32 arena_base(unsigned index){assert(index<2);return bases[index];}
'''


def signed(n,bits=32):
    n&=(1<<bits)-1
    return n-(1<<bits) if n&(1<<(bits-1)) else n


class FrameTaskControllerTest(unittest.TestCase):
    def test_actual_controllers_ramp_complete_owners_and_mips(self):
        owners=source_owners(ROOT/'src/main/PAL');names=('RampValueToLimit','FrmTaskModeEnter','ResetFrmTaskState','FrmTaskModeUpdate')
        bodies=[definition_source(owners[n].read_text(),n) for n in names]
        for body in bodies:self.assertNotRegex(body,r'\b(?:asm|__asm__)\s*\(')
        with TemporaryDirectory() as temporary:
            source=Path(temporary)/'task.c';library=source.with_suffix('.dylib');source.write_text(PREFIX+'\n'.join(bodies)+TAIL)
            built=subprocess.run(['cc','-std=c11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                                  '-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I',str(ROOT/'include'),
                                  str(source),str(ROOT/'src/native/gpu_address.c'),'-o',str(library)],capture_output=True,text=True)
            self.assertEqual(built.returncode,0,built.stderr);native=C.CDLL(str(library));native.exercise_native.restype=C.c_uint
            self.assertEqual(native.exercise_native(),4224)
            native.prepare.argtypes=[C.c_uint,C.c_uint];native.prepare.restype=C.c_uint
            native.initial_wire.restype=native.final_wire.restype=native.event_wire.restype=native.frame_image.restype=C.c_void_p
            native.event_wire.argtypes=native.frame_image.argtypes=[C.c_uint,C.c_uint];native.event_value.argtypes=[C.c_uint,C.c_uint];native.event_value.restype=C.c_uint32
            native.arena_base.argtypes=[C.c_uint];native.arena_base.restype=C.c_uint32
            services=['GetR4FileIndex','CdFileReadStart','CdFileBytesLoaded','CdAudioInit','CdAudioSelectTrack','CdAudioBeginPlay','LoadTimList',
                      'SetMenuFrameOptions','AddScreenFadeTiles','CdAudioFade','EmitPresetSprite','EmitSpriteGrid','UpdateScreenFade']
            service_addresses={address_of(n):i+1 for i,n in enumerate(services)};argc=(0,2,2,0,0,2,1,1,3,3,2,2,2,1)
            globals_=[('ActiveFrameTaskScript',4),('LoadCursor',4),('CurrentDrawBuf',4),('FrmTaskState',4),('MenuReturnSubmodeIndex',4),
                      ('FadeCountdown',2),('SubmodeState',2),('ModeGroup',2),('ModeIndex',2)]
            cells={n:address_of(n) for n,_ in globals_};phases=(0,1,2,3,4,5,6,7,8,-1,-2147483648,2147483647,9,65535,32768,-9)
            frame_edges=(-32768,-1,0,7,8,31,32,63,64,32767);fade_edges=(-32768,-1,0,1,2,63,64,32767)
            for label,path in (('original',ROOT/'assets/PAL/main.exe'),('linked',ROOT/'build/PAL/main.exe')):
                memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000);memory.mem_map(SCRATCH,4096)
                image=path.read_bytes();memory.mem_write(struct.unpack_from('<I',image,24)[0]&0x1FFFFFFF,image[0x800:])
                read=lambda p,n:bytes(memory.mem_read(p&0x1FFFFFFF,n))
                write=lambda p,b:memory.mem_write(p&0x1FFFFFFF,bytes(b))
                load=lambda p,n=4:int.from_bytes(read(p,n),'little')
                def put(p,n,size=4):write(p,(n&((1<<(size*8))-1)).to_bytes(size,'little'))
                plans=[(n*7919&4095,0) for n in range(1024)]+[(n,k) for k in (1,2) for n in range(64)]
                for scenario,kind in plans:
                    count=native.prepare(scenario,kind);initial=C.string_at(native.initial_wire(),860);wanted=C.string_at(native.final_wire(),860)
                    bases=[native.arena_base(i) for i in range(2)]
                    def state():
                        parts=[initial[:32]]+[read(p-16,48) for p in SCRIPT]+[read(p-16,288) for p in bases]
                        parts += [read(p-16,16)+read(p+FRAME_SIZE,16) for p in FRAME]
                        parts += [read(p+0x70,4) for p in FRAME]
                        parts += [read(cells[n],4) for n in ('ActiveFrameTaskScript','LoadCursor')]
                        parts += [read(SCRATCH,4),read(SCRATCH+4,4),read(cells['CurrentDrawBuf'],4)]
                        parts += [read(cells[n],size) for n,size in globals_[3:]]
                        return b''.join(parts)
                    at=32
                    for p in SCRIPT:write(p-16,initial[at:at+48]);at+=48
                    for p in bases:write(p-16,initial[at:at+288]);at+=288
                    for i,p in enumerate(FRAME):
                        write(p,C.string_at(native.frame_image(i,0),FRAME_SIZE));write(p-16,initial[at:at+16]);write(p+FRAME_SIZE,initial[at+16:at+32]);at+=32
                    for p in FRAME:write(p+0x70,initial[at:at+4]);at+=4
                    roots=struct.unpack_from('<5I',initial,at);at+=20
                    for name,value in zip(('ActiveFrameTaskScript','LoadCursor'),roots):put(cells[name],value)
                    write(SCRATCH,b'\xA5'*64);put(SCRATCH,roots[2]);put(SCRATCH+4,roots[3]);put(cells['CurrentDrawBuf'],roots[4])
                    for name,size in globals_[3:]:write(cells[name],initial[at:at+size]);at+=size
                    self.assertEqual(at,860);self.assertEqual(state(),initial)
                    write(STACK-256,b'\xA5'*384)
                    allowed=[(p,16) for p in SCRIPT]+[(p,256) for p in bases]+[(p+0x70,4) for p in FRAME]+[(SCRATCH,8),(STACK-240,240)]+[(cells[n],size) for n,size in globals_]
                    def store(machine,access,address,size,value,user):
                        address&=0x1FFFFFFF
                        self.assertTrue(any((p&0x1FFFFFFF)<=address and address+size<=(p&0x1FFFFFFF)+n for p,n in allowed),(label,scenario,hex(machine.pc),hex(address)))
                    cpu=Cpu(memory,0,100,101,102,store);cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)];cpu.r[29],cpu.r[31]=STACK,STOP
                    cpu.pc=address_of(('FrmTaskModeUpdate','FrmTaskModeEnter','ResetFrmTaskState')[kind]);event=0
                    def select_head(bank,offset):
                        alias=scenario%6;pos=offset if alias==1 else offset+4 if alias==2 else offset+8 if alias==3 else 252 if alias==5 else 0
                        pos=min(pos,252);put(SCRATCH+4,FRAME[(scenario//4)&1]+0x70 if alias==4 else bases[bank if alias else 1-bank]+pos)
                    for _ in range(3000):
                        if cpu.pc==STOP:break
                        if cpu.pc not in service_addresses:cpu.step();continue
                        op=service_addresses[cpu.pc];self.assertLess(event,count);self.assertEqual(op,native.event_value(event,0))
                        args=cpu.r[4:7]
                        for i in range(argc[op]):self.assertEqual(args[i],native.event_value(event,i+1),(label,scenario,event,op,i))
                        self.assertEqual(state(),C.string_at(native.event_wire(event,0),860),(label,scenario,event,'entry'))
                        if op in (11,12):
                            cursor=load(SCRATCH);bank=int((cursor&0x1FFFFFFF)>=0x281000);offset=(cursor&0x1FFFFFFF)-(bases[bank]&0x1FFFFFFF)
                            self.assertLessEqual(offset+24,256);put(cursor+4,0x60000000|(op<<16)|(scenario&0xFFFF));put(SCRATCH,cursor+(20 if op==11 else 24))
                            if op==12:put(args[1]+14,load(args[1]+14,2)+1,2)
                        if scenario&32:
                            slot=(scenario+event)%3;bank=(scenario+event)&1
                            put(cells['ActiveFrameTaskScript'],SCRIPT[slot]);put(cells['LoadCursor'],SCRIPT[(scenario+2*event)%3]);put(cells['CurrentDrawBuf'],FRAME[bank])
                            put(cells['FrmTaskState'],0x81230000+scenario*31+event);put(cells['FadeCountdown'],fade_edges[(scenario+event)%8],2)
                            put(cells['MenuReturnSubmodeIndex'],0xFEDC0000+event if (scenario+event)&1 else 0)
                            for name,value in [('ModeGroup',scenario*37+event),('ModeIndex',scenario*19+event),('SubmodeState',scenario*7+event)]:put(cells[name],value,2)
                            put(SCRIPT[slot],frame_edges[(scenario+event)%10],2);p=SCRIPT[slot]+2+(event%3)*2;put(p,load(p,2)^(0x1111+event),2)
                            p=FRAME[bank]+0x70;put(p,load(p)^(0xA1230000+scenario+event));offset=16+((scenario+event)%8)*4;put(SCRATCH,bases[bank]+offset);select_head(bank,offset)
                        if op==12 and scenario&16:
                            bank=(scenario//4)&1;put(SCRATCH,bases[bank]+248);select_head(bank,248)
                        self.assertEqual(state(),C.string_at(native.event_wire(event,1),860),(label,scenario,event,'effect'))
                        result=native.event_value(event,4)
                        for reg in (1,2,3,*range(4,16),24,25):cpu.r[reg]=(0xA1230000+event*257+reg)&0xFFFFFFFF
                        cpu.r[2]=result;cpu.pc=cpu.r[31];event+=1
                    self.assertEqual((cpu.pc,cpu.r[29],event),(STOP,STACK,count),(label,scenario,kind))
                    self.assertEqual(state(),wanted,(label,scenario,kind,'final'))
                    for i,p in enumerate(FRAME):self.assertEqual(read(p,FRAME_SIZE),C.string_at(native.frame_image(i,1),FRAME_SIZE))
                    self.assertEqual(read(SCRATCH+8,56),b'\xA5'*56);self.assertEqual(read(STACK-256,16),b'\xA5'*16);self.assertEqual(read(STACK,128),b'\xA5'*128)
                    for reg in (*range(16,24),28,30):self.assertEqual(cpu.r[reg],0xBEEF0000+reg)


if __name__=='__main__':unittest.main()
