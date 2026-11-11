"""Real TIM construction, backup, save construction and all import branches.

Current C bodies and original/linked MIPS share a serialized-state oracle.
Only the audio application boundary is controlled; string copying is real C.
"""
import ctypes as C
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu
from test_save_record_writer import expected_world as writer_model, OFFSETS as WRITER_OFFSETS, SIZE as WRITER_SIZE, PARTS as WRITER_PARTS

ROOT=Path(__file__).resolve().parents[2]
BUFFERS=(0x80200010,0x80205010)
ASSETS=(0x80210010,0x80211010)
STACK,STOP=0x803F0000,0x803FF000
PARTS=(('before',16),('buffers[0]',16384),('buffers[1]',16384),('custom',4224),('work',2112),
       ('progress',124),('options',64),('choices',144),('selection',44),('snapshots',132),('slots',132),
       ('times',192),('laps',1280),('races',1280),('bestLap',64),('bestTotal',64),('versus',144),('link',144),
       ('versusNames',24),('linkNames',48),('assets',4224),('frames',4),('extraFrames',12),('controls',16),('after',16))
OFFSETS={};SIZE=0
for name,length in PARTS:OFFSETS[name]=SIZE;SIZE+=length
CALLS=7;FRAMES=8
EVENTS_AT=4+CALLS*8+FRAMES*(4+SIZE)
NAMES=('InitTimTemplate','BackupSaveData','RestoreSaveData','BuildSaveData','ApplySaveData','CopyStringClearTail')

NATIVE=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "game/data/savetype.h"
#include "game/data/savedat.h"
#include "game/data/savebld.h"
#include "game/gfx/model.h"
#include "game/race/ranking.h"
#include "rr/emblem_images.h"
#include "rr/save_slots.h"
#include "rr/string_tail_copy.h"
typedef union {CardSaveBlock2000 block;u8 bytes[0x4000];} Arena;
typedef struct {
 s32 active;s16 gameMode;u16 cursor;
 u8 buffer,activeA,activeB,savedA,savedB,audio,corrupt,padding;
} Controls;
typedef struct {
 u32 before[4];Arena buffers[2];CustomEmblemImages1080 custom;EmblemTimImage840 work;
 SaveProgressRecord7C progress;RuntimeSettings options;CourseChoice4 choices[4][9];
 SavedMenuSettings selection,snapshots[3],slots[3];s32 times[2][8][3];
 TimeRecord16 laps[2][8][5],races[2][8][5];s32 bestLap[2][8],bestTotal[2][8];
 CourseChoice4 versus[2][2][9],link[2][2][9];char versusNames[2][12],linkNames[4][12];
 EmblemTim840 assets[2];s32 frames,extraFrames[3];Controls controls;u32 after[4];
} World;
static World actual;
typedef struct {u32 before[4];u8 *buffer;EmblemTim840 *activeA,*activeB,*savedA,*savedB;u32 after[4];} Roots;
static Roots roots;
static const u8 *trace;static unsigned eventCount,eventCursor;
#define SaveBuffer roots.buffer
#define CustomEmblemBuffers actual.custom
#define SaveDataWork actual.work
#define SaveProgress actual.progress
#define Options actual.options
#define CourseChoiceTable actual.choices
#define SelectionState actual.selection
#define SaveSlotSnapshots actual.snapshots
#define SaveSlotSelections actual.slots
#define CourseBestLapTimes actual.times
#define LapRecords actual.laps
#define RaceRecords actual.races
#define CourseBestLap actual.bestLap
#define CourseBestTotal actual.bestTotal
#define VsCarChoiceTable actual.versus
#define LinkCarChoiceTable actual.link
#define VsPlayerNames actual.versusNames
#define LinkPlayerNames actual.linkNames
#define EmblemEditFrames actual.frames
#define GarageExtraDefaultTimes actual.extraFrames
#define GameModeType actual.controls.gameMode
#define ActivePlayerIndex actual.controls.active
#define GpSaveSlotCursor actual.controls.cursor
#define GarageActiveDecalTimA roots.activeA
#define GarageActiveDecalTimB roots.activeB
#define GarageSavedDecalTimA roots.savedA
#define GarageSavedDecalTimB roots.savedB
static u32 get(const u8*p){u32 v;memcpy(&v,p,4);return v;}
static EmblemTim840 *source(unsigned id){
 assert(id<9);
 if(id<2)return &actual.custom.images[id].file;
 if(id==2)return &actual.work.file;
 if(id<7)return &actual.buffers[(id-3)/2].block.emblem.decals300[(id-3)%2];
 return &actual.assets[id-7];
}
static void bind(void){
 memset(&roots,0xA5,sizeof(roots));assert(actual.controls.buffer<2&&actual.controls.cursor<3);
 roots.buffer=actual.buffers[actual.controls.buffer].bytes;
 roots.activeA=source(actual.controls.activeA);roots.activeB=source(actual.controls.activeB);
 roots.savedA=source(actual.controls.savedA);roots.savedB=source(actual.controls.savedB);
 assert(sizeof(void*)==8&&(uintptr_t)roots.buffer>UINT32_MAX&&(uintptr_t)roots.savedB>UINT32_MAX);
}
static void check(const u8 *expected){
 assert(!memcmp(&actual,expected,sizeof(actual)));Roots saved;memcpy(&saved,&roots,sizeof(saved));
 bind();assert(!memcmp(&saved,&roots,sizeof(roots)));
}
void ApplySoundVolumes(void){
 assert(eventCursor<eventCount);const u8 *event=trace+@EVENTS@+eventCursor*2*sizeof(World);check(event);
 if(actual.controls.audio){
  actual.options.bytes[0x31]^=0x5A;actual.frames=(s32)((u32)actual.frames^0x8F010203U);
  actual.controls.buffer^=1;actual.controls.active^=1;
  actual.extraFrames[0]=(s32)((u32)actual.extraFrames[0]^0x76543210U);
  actual.custom.images[1].words[16]^=0xA5A55A5AU;bind();
 }
 check(event+sizeof(World));eventCursor++;
}
'''

TAIL=r'''
void run_pipeline(const u8 *input){
 trace=input;eventCount=get(input);eventCursor=0;assert(eventCount<=1);
 const u8 *frames=input+4+7*8;memcpy(&actual,frames+4,sizeof(actual));bind();check(frames+4);
 for(unsigned i=0;i<7;i++){
  const u8 *call=input+4+i*8;u32 kind=get(call);s32 argument=(s32)get(call+4),result=0;
  switch(kind){
  case 0:assert(argument>=0&&argument<3);InitTimTemplate(argument==2?&actual.work:&actual.custom.images[argument]);break;
  case 1:BackupSaveData();break;
  case 2:RestoreSaveData();break;
  case 3:BuildSaveData(argument);break;
  case 4:
   if(argument==0x10){u32 frame;memcpy(&frame,SaveBuffer+0x280,4);actual.controls.buffer^=1;bind();memcpy(SaveBuffer,&frame,4);}
   else if(argument!=0x11&&actual.controls.corrupt)SaveBuffer[0x1AA4]^=0x40;
   break;
  case 5:result=ApplySaveData(argument);break;
  default:assert(!"unknown action");
  }
  const u8 *frame=frames+(i+1)*(4+sizeof(World));assert(result==(s32)get(frame));check(frame+4);
 }
 assert(eventCursor==eventCount);
}
'''


def source_offset(identifier):
    if identifier<2:return OFFSETS['custom']+identifier*0x840
    if identifier==2:return OFFSETS['work']
    if identifier<7:return OFFSETS['buffers[0]']+((identifier-3)//2)*0x4000+0x300+((identifier-3)%2)*0x840
    return OFFSETS['assets']+(identifier-7)*0x840


class Model:
    def __init__(self,state):self.state=bytearray(state);self.events=[]
    def copy(self,to,source,length):self.state[to:to+length]=self.state[source:source+length]
    def root(self):return OFFSETS['buffers[0]']+self.state[OFFSETS['controls']+8]*0x4000
    def template(self,index):
        at=OFFSETS['work'] if index==2 else OFFSETS['custom']+index*0x840
        header=(16,8,44,0x00400280,0x00010010,0x7FFF0000,0x7C1F03FF,0x7FE0001F,
                0x7C0003E0,0x42100000,0x40100210,0x42000010,0x40000200,0x80C,0x280,0x00400010)
        self.state[at:at+0x840]=struct.pack('<16I',*header)+bytes(2048)
    def build(self,mode):
        # Transport into the established independent writer model; its model,
        # cases and PRNG are unchanged. Non-buffer TIM sources are immutable.
        previous=bytearray(WRITER_SIZE)
        mapping=('buffers[0]','buffers[1]','progress','options','choices','selection','snapshots','times','laps','races','frames')
        lengths=dict(WRITER_PARTS)
        for name in mapping:previous[WRITER_OFFSETS[name]:WRITER_OFFSETS[name]+lengths[name]]=self.state[OFFSETS[name]:OFFSETS[name]+lengths[name]]
        previous[WRITER_OFFSETS['emblem']:WRITER_OFFSETS['emblem']+0x840]=self.state[OFFSETS['custom']:OFFSETS['custom']+0x840]
        control=OFFSETS['controls'];old_control=WRITER_OFFSETS['controls']
        previous[old_control:old_control+2]=self.state[control+6:control+8];previous[old_control+2]=self.state[control+8]
        for n,identifier in enumerate(self.state[control+9:control+11]):
            if 3<=identifier<7:encoded=identifier
            else:
                encoded=n;source=source_offset(identifier)
                previous[WRITER_OFFSETS['decals']+n*0x840:WRITER_OFFSETS['decals']+(n+1)*0x840]=self.state[source:source+0x840]
            previous[old_control+3+n]=encoded
        expected=writer_model(previous,mode)
        for name in mapping:self.state[OFFSETS[name]:OFFSETS[name]+lengths[name]]=expected[WRITER_OFFSETS[name]:WRITER_OFFSETS[name]+lengths[name]]
    def audio(self):
        before=bytes(self.state);at=OFFSETS['controls']
        if self.state[at+13]:
            self.state[OFFSETS['options']+0x31]^=0x5A
            for position,value in ((OFFSETS['frames'],0x8F010203),(at,1),(OFFSETS['extraFrames'],0x76543210),(OFFSETS['custom']+0x840+64,0xA5A55A5A)):
                current=struct.unpack_from('<I',self.state,position)[0];struct.pack_into('<I',self.state,position,current^value)
            self.state[at+8]^=1
        self.events.append(before+bytes(self.state))
    def clear_tail(self,destination,source):
        value=bytes(self.state[source:source+12]).split(b'\0',1)[0]
        self.state[destination:destination+len(value)]=value
        at=destination+len(value)
        while self.state[at]:self.state[at]=0;at+=1
    def apply(self,mode):
        start=self.root();control=OFFSETS['controls'];cursor=struct.unpack_from('<H',self.state,control+6)[0]
        if mode not in (0x10,0x11):
            wanted=struct.unpack_from('<I',self.state,start+0x1AA4)[0]
            if sum(struct.unpack_from('<1705I',self.state,start))&0xFFFFFFFF!=wanted:return 0
            if mode==2:
                active,game_mode=struct.unpack_from('<ih',self.state,control)
                for index in range(9):
                    if game_mode in (2,3):
                        self.copy(OFFSETS['versus']+(active&1)*72+36+index*4,start+0x33C+index*4,4)
                        self.clear_tail(OFFSETS['versusNames']+(active&1)*12,start+0x280)
                    elif game_mode==4:
                        self.copy(OFFSETS['link']+(active&1)*72+36+index*4,start+0x33C+index*4,4)
                        self.clear_tail(OFFSETS['linkNames']+active*12,start+0x280)
                self.copy(OFFSETS['custom']+(active&1)*0x840,start+0xEA4,0x840)
            else:
                for destination,source,length in (('progress',0x280,124),('options',0x2FC,64),('slots' if mode==0x30 else 'snapshots',0x360,132),
                                                  ('times',0x3E4,192),('laps',0x4A4,1280),('races',0x9A4,1280),('custom',0xEA4,0x840)):
                    self.copy(OFFSETS[destination],start+source,length)
                self.copy(OFFSETS['choices']+36,start+0x33C,36)
                for index in range(16):
                    self.copy(OFFSETS['bestLap']+index*4,OFFSETS['laps']+index*80+12,4)
                    self.copy(OFFSETS['bestTotal']+index*4,OFFSETS['races']+index*80+12,4)
                self.audio()
        elif mode==0x10:self.copy(OFFSETS['extraFrames']+cursor*4,start,4)
        else:
            self.copy(OFFSETS['frames'],start+0x280,4);self.copy(OFFSETS['extraFrames']+cursor*4,start+0x280,4)
            for frame,identifier in enumerate(self.state[control+11:control+13]):
                destination=source_offset(identifier)
                for index in range(0x840):self.state[destination+index]=self.state[start+0x300+frame*0x840+index]
        return 1
    def action(self,kind,argument):
        if kind==0:self.template(argument)
        elif kind==1:self.copy(OFFSETS['custom'],OFFSETS['work'],0x840)
        elif kind==2:self.copy(OFFSETS['work'],OFFSETS['custom'],0x840)
        elif kind==3:self.build(argument)
        elif kind==4:
            control=OFFSETS['controls']
            if argument==0x10:
                source=self.root()+0x280;value=bytes(self.state[source:source+4]);self.state[control+8]^=1
                self.state[self.root():self.root()+4]=value
            elif argument!=0x11 and self.state[control+14]:self.state[self.root()+0x1AA4]^=0x40
        else:return self.apply(argument)
        return 0


class SaveImportPipelineTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup);source=Path(cls.temp.name)/'pipeline.c';library=source.with_suffix('.dylib')
        owners=source_owners(ROOT/'src/main/PAL');order=(*NAMES[:-2],NAMES[-1],NAMES[-2])
        bodies='\n'.join(definition_source(owners[n].read_text(),n) for n in order)
        checks='\n'.join('_Static_assert(offsetof(World,%s)==%d,"wire %s");'%(n,OFFSETS[n],n) for n,_ in PARTS)
        checks+='\n_Static_assert(sizeof(World)==%d,"complete world");\n'%SIZE
        source.write_text((NATIVE+checks+bodies+TAIL).replace('@EVENTS@',str(EVENTS_AT)))
        result=subprocess.run(['cc','-std=gnu11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror','-fsanitize=undefined,bounds',
                               '-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(source),'-o',str(library)],capture_output=True,text=True)
        if result.returncode:raise AssertionError(result.stderr)
        cls.native=C.CDLL(str(library));cls.native.run_pipeline.argtypes=[C.c_void_p]

    def test_full_pipeline_all_import_modes_aliases_and_audio_boundary(self):
        bindings=[(BUFFERS[0],OFFSETS['buffers[0]'],0x4000),(BUFFERS[1],OFFSETS['buffers[1]'],0x4000)]
        for symbol,name,length in (('CustomEmblemBuffers','custom',4224),('SaveDataWork','work',2112),('SaveProgress','progress',124),('Options','options',64),
            ('CourseChoiceTable','choices',72),('SelectionState','selection',44),('SaveSlotSnapshots','snapshots',132),('SaveSlotSelections','slots',132),
            ('CourseBestLapTimes','times',192),('LapRecords','laps',1280),('RaceRecords','races',1280),('CourseBestLap','bestLap',64),('CourseBestTotal','bestTotal',64),
            ('VsCarChoiceTable','versus',144),('LinkCarChoiceTable','link',144),('VsPlayerNames','versusNames',24),('LinkPlayerNames','linkNames',48),
            ('EmblemEditFrames','frames',4),('GarageExtraDefaultTimes','extraFrames',12)):
            bindings.append((address_of(symbol),OFFSETS[name],length))
        for index,address in enumerate(ASSETS):bindings.append((address,OFFSETS['assets']+index*0x840,0x840))
        pointers=[address_of(n) for n in ('SaveBuffer','GarageActiveDecalTimA','GarageActiveDecalTimB','GarageSavedDecalTimA','GarageSavedDecalTimB')]
        scalar_addresses=[address_of(n) for n in ('ActivePlayerIndex','GameModeType','GpSaveSlotCursor')];scalar_widths=(4,2,2)
        machines=[]
        for label,path in [('original',ROOT/'assets/PAL/main.exe'),('linked',ROOT/'build/PAL/main.exe')]:
            memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000)
            image=path.read_bytes();memory.mem_write(struct.unpack_from('<I',image,24)[0]&0x1FFFFFFF,image[0x800:]);machines.append((label,memory))
        audio_address=address_of('ApplySoundVolumes')
        rng=random.Random(0x54494D53);coverage=set();modes=(0,1,0x30,2,0x10,0x11,4,-1)
        for number in range(256):
            initial=bytearray(rng.randbytes(SIZE));control=OFFSETS['controls'];game_mode=(0,2,3,4,5)[(number//8)%5]
            active=number%4 if game_mode==4 else (-2147483648,-1,0,1,2,3,2147483647)[(number//3)%7]
            struct.pack_into('<ihH8B',initial,control,active,game_mode,number%3,number&1,(number//2)%9,(number//18)%9,(number//3)%9,(number//27)%9,(number>>4)&1,(number>>3)&1,0)
            for n in range(7):
                at=(OFFSETS['progress'] if n==0 else OFFSETS['versusNames']+(n-1)*12 if n<3 else OFFSETS['linkNames']+(n-3)*12)
                text=('R4_%d_%d'%(number,n)).encode();initial[at:at+12]=text+b'\0'+bytes(11-len(text))
            mode=modes[number%8];build_mode=(6 if number&16 else 17) if mode in (0x10,0x11) else (3 if number&16 else 0)
            calls=((0,number&1),(1,0),(0,2),(2,0),(3,build_mode),(4,mode),(5,mode));model=Model(initial);frames=[struct.pack('<I',0)+bytes(initial)]
            for kind,arg in calls:
                result=model.action(kind,arg);frames.append(struct.pack('<I',result)+bytes(model.state))
                if kind==5:coverage.add((mode,result))
            trace=struct.pack('<I',len(model.events))+b''.join(struct.pack('<Ii',*call) for call in calls)+b''.join(frames)+b''.join(model.events)
            self.native.run_pipeline(C.create_string_buffer(trace))
            def pointer_values(state):
                c=OFFSETS['controls'];values=[BUFFERS[state[c+8]]]
                for identifier in state[c+9:c+13]:
                    if identifier<2:value=address_of('CustomEmblemBuffers')+identifier*0x840
                    elif identifier==2:value=address_of('SaveDataWork')
                    elif identifier<7:value=BUFFERS[(identifier-3)//2]+0x300+((identifier-3)%2)*0x840
                    else:value=ASSETS[identifier-7]
                    values.append(value)
                return values
            for label,memory in machines:
                read=lambda p,n:bytes(memory.mem_read(p&0x1FFFFFFF,n))
                write=lambda p,b:memory.mem_write(p&0x1FFFFFFF,bytes(b))
                def load(state):
                    for p,at,length in bindings:write(p,state[at:at+length])
                    for p,value in zip(pointers,pointer_values(state)):write(p,struct.pack('<I',value))
                    at=OFFSETS['controls']
                    for p,width,offset in zip(scalar_addresses,scalar_widths,(0,4,6)):write(p,state[at+offset:at+offset+width])
                def check(state):
                    for p,at,length in bindings:self.assertEqual(read(p,length),state[at:at+length],(number,label,hex(p)))
                    for p,value in zip(pointers,pointer_values(state)):self.assertEqual(read(p,4),struct.pack('<I',value))
                    at=OFFSETS['controls']
                    for p,width,offset in zip(scalar_addresses,scalar_widths,(0,4,6)):self.assertEqual(read(p,width),state[at+offset:at+offset+width])
                load(initial);event_cursor=0
                for frame,(kind,arg) in enumerate(calls,1):
                    if kind==4:load(frames[frame][4:]);continue
                    def store(cpu,access,address,length,value,user):
                        address&=0x1FFFFFFF;allowed=[(p,n) for p,_,n in bindings]+[(STACK-0x800,0x800)]
                        self.assertTrue(any((p&0x1FFFFFFF)<=address and address+length<=(p&0x1FFFFFFF)+n for p,n in allowed),(number,label,hex(cpu.pc),hex(address)))
                    cpu=Cpu(memory,0,100,101,102,store);cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)]
                    name={0:'InitTimTemplate',1:'BackupSaveData',2:'RestoreSaveData',3:'BuildSaveData',5:'ApplySaveData'}[kind]
                    cpu.r[4]=(address_of('SaveDataWork') if arg==2 else address_of('CustomEmblemBuffers')+arg*0x840) if kind==0 else arg&0xFFFFFFFF
                    cpu.r[29],cpu.r[31],cpu.pc=STACK,STOP,address_of(name);write(STACK-0x810,b'\xA5'*0x850)
                    for _ in range(400000):
                        if cpu.pc==STOP:break
                        if cpu.pc==audio_address:
                            event=model.events[event_cursor];check(event[:SIZE]);load(event[SIZE:]);event_cursor+=1
                            for register in (2,3,*range(4,16),24,25):cpu.r[register]=0xBAD00000+register
                            cpu.pc=cpu.r[31];continue
                        cpu.step()
                    self.assertEqual((cpu.pc,cpu.r[29]),(STOP,STACK),(number,label,name))
                    if kind==5:self.assertEqual(cpu.r[2],struct.unpack_from('<I',frames[frame])[0],(number,label,mode))
                    check(frames[frame][4:]);self.assertEqual(read(STACK-0x810,16),b'\xA5'*16)
                    for register in (*range(16,24),28,30):self.assertEqual(cpu.r[register],0xBEEF0000+register)
                self.assertEqual(event_cursor,len(model.events))
        self.assertEqual(coverage,{(m,result) for m in modes for result in ((1,) if m in (0x10,0x11) else (0,1))})


if __name__=='__main__':unittest.main()
