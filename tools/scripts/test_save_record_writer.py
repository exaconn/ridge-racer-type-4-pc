"""Actual save writer: complete records, wrapped checksums and native pointers.

An independent serialized-byte oracle checks the unchanged C body and both
original/linked MIPS. There are no substituted services or C body rewrites.
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

ROOT=Path(__file__).resolve().parents[2]
BUFFERS=(0x80200010,0x80205010)
DECALS=(0x80210010,0x80211010,0x80212010)
STACK,STOP=0x803F0000,0x803FF000
PARTS=(('before',16),('buffers[0]',16384),('buffers[1]',16384),
       ('progress',124),('options',64),('choices',72),('selection',44),
       ('snapshots',132),('times',192),('laps',1280),('races',1280),
       ('emblem',2112),('decals',6336),('frames',4),('controls',8),('after',16))
OFFSETS={}
SIZE=0
for name,length in PARTS:OFFSETS[name]=SIZE;SIZE+=length

NATIVE=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "game/data/savetype.h"
#include "game/data/savebld.h"
typedef union {CardSaveBlock2000 block;u8 bytes[0x4000];} Arena;
typedef struct {
 u32 before[4];Arena buffers[2];SaveProgressRecord7C progress;
 RuntimeSettings options;CourseChoice4 choices[2][9];SavedMenuSettings selection,snapshots[3];
 s32 times[2][8][3];TimeRecord16 laps[2][8][5],races[2][8][5];
 s32 emblem[0x210];EmblemTim840 decals[3];s32 frames;
 struct {u16 cursor;u8 root,sourceA,sourceB,padding[3];} controls;
 u32 after[4];
} World;
static World actual;
typedef struct {u32 before[4];CustomEmblemImages1080 value;u32 after[4];} EmblemOwner;
static EmblemOwner nativeEmblems;
typedef struct {u32 before[4];u8 *buffer;EmblemTim840 *first,*second;u32 after[4];} Roots;
static Roots roots;
#define SaveBuffer roots.buffer
#define GarageActiveDecalTimA roots.first
#define GarageActiveDecalTimB roots.second
#define SaveProgress actual.progress
#define Options actual.options
#define CourseChoiceTable actual.choices
#define SelectionState actual.selection
#define SaveSlotSnapshots actual.snapshots
#define CourseBestLapTimes actual.times
#define LapRecords actual.laps
#define RaceRecords actual.races
#define CustomEmblemBuffers nativeEmblems.value
#define EmblemEditFrames actual.frames
#define GpSaveSlotCursor actual.controls.cursor
static EmblemTim840 *source(unsigned id){
 assert(id<7);
 if(id<3)return &actual.decals[id];
 return &actual.buffers[(id-3)/2].block.emblem.decals300[(id-3)%2];
}
'''

TAIL=r'''
void run_writer(const u8 *input,const u8 *expected,s32 mode){
 memcpy(&actual,input,sizeof(actual));assert(actual.controls.root<2&&actual.controls.cursor<3);
 memset(&roots,0xA5,sizeof(roots));roots.buffer=actual.buffers[actual.controls.root].bytes;
 roots.first=source(actual.controls.sourceA);roots.second=source(actual.controls.sourceB);
 assert(sizeof(void*)==8&&(uintptr_t)roots.buffer>UINT32_MAX);
 assert((uintptr_t)roots.first>UINT32_MAX&&(uintptr_t)roots.second>UINT32_MAX);
 memset(&nativeEmblems,0xA7,sizeof(nativeEmblems));
 memcpy(nativeEmblems.value.words,actual.emblem,sizeof(actual.emblem));
 EmblemOwner savedEmblems;memcpy(&savedEmblems,&nativeEmblems,sizeof(savedEmblems));
 Roots saved;memcpy(&saved,&roots,sizeof(saved));
 BuildSaveData(mode);
 assert(!memcmp(&actual,expected,sizeof(actual))&&!memcmp(&roots,&saved,sizeof(roots)));
 assert(!memcmp(&nativeEmblems,&savedEmblems,sizeof(nativeEmblems)));
}
'''


def source_offset(state,identifier):
    if identifier<3:return OFFSETS['decals']+identifier*0x840
    return OFFSETS['buffers[0]']+((identifier-3)//2)*0x4000+0x300+((identifier-3)%2)*0x840


def expected_world(initial,mode):
    state=bytearray(initial);control=OFFSETS['controls'];root=state[control+2]
    start=OFFSETS['buffers[0]']+root*0x4000
    def checksum(at):
        struct.pack_into('<H',state,at+16,0)
        value=sum(struct.unpack_from('<22h',state,at))&65535
        struct.pack_into('<H',state,at+16,value)
    def copy(destination,source,length):state[destination:destination+length]=state[source:source+length]
    if mode not in (6,17):
        state[start+0x280:start+0x2000]=bytes(0x2000-0x280)
        for destination,name,length in ((0x280,'progress',124),(0x2FC,'options',64)):
            copy(start+destination,OFFSETS[name],length)
        copy(start+0x33C,OFFSETS['choices']+36,36)
        for slot in range(3):
            record=OFFSETS['snapshots']+slot*44;checksum(record);copy(start+0x360+slot*44,record,44)
        if mode==3:
            record=OFFSETS['selection'];checksum(record)
            cursor=struct.unpack_from('<H',state,control)[0]
            copy(start+0x360+cursor*44,record,44)
        for destination,name,length in ((0x3E4,'times',192),(0x4A4,'laps',1280),(0x9A4,'races',1280),(0xEA4,'emblem',2112)):
            copy(start+destination,OFFSETS[name],length)
        total=sum(struct.unpack_from('<1705I',state,start))&0xFFFFFFFF
        struct.pack_into('<I',state,start+0x1AA4,total)
    else:
        if mode==6:copy(start+0xEA4,OFFSETS['emblem'],2112)
        copy(start+0x280,OFFSETS['frames'],4)
        # The original performs forward byte copies; sources may be a frame
        # in either real buffer, including overlap with the destination frame.
        for frame in range(2):
            source=source_offset(state,state[control+3+frame])
            for index in range(0x840):state[start+0x300+frame*0x840+index]=state[source+index]
    return bytes(state)


class SaveRecordWriterTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup)
        source=Path(cls.temp.name)/'writer.c';library=source.with_suffix('.dylib')
        owner=source_owners(ROOT/'src/main/PAL')['BuildSaveData']
        body=definition_source(owner.read_text(),'BuildSaveData')
        assertions='\n'.join('_Static_assert(offsetof(World,%s)==%d,"wire %s");'%(name,OFFSETS[name],name) for name,_ in PARTS)
        assertions+='\n_Static_assert(sizeof(World)==%d,"complete world");\n'%SIZE
        source.write_text(NATIVE+assertions+body+TAIL)
        result=subprocess.run(['cc','-std=gnu11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                               '-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I',str(ROOT/'include'),
                               str(source),'-o',str(library)],capture_output=True,text=True)
        if result.returncode:raise AssertionError(result.stderr)
        cls.native=C.CDLL(str(library));cls.native.run_writer.argtypes=[C.c_void_p,C.c_void_p,C.c_int32]

    def test_actual_writer_original_and_linked_with_complete_owners(self):
        bindings=[(BUFFERS[0],OFFSETS['buffers[0]'],0x4000),(BUFFERS[1],OFFSETS['buffers[1]'],0x4000)]
        for symbol,name,length in (('SaveProgress','progress',124),('Options','options',64),('CourseChoiceTable','choices',72),
                                   ('SelectionState','selection',44),('SaveSlotSnapshots','snapshots',132),
                                   ('CourseBestLapTimes','times',192),('LapRecords','laps',1280),('RaceRecords','races',1280),
                                   ('CustomEmblemBuffers','emblem',2112),('EmblemEditFrames','frames',4)):
            bindings.append((address_of(symbol),OFFSETS[name],length))
        for index,address in enumerate(DECALS):bindings.append((address,OFFSETS['decals']+index*0x840,0x840))
        pointer_cells=[address_of(n) for n in ('SaveBuffer','GarageActiveDecalTimA','GarageActiveDecalTimB')]
        cursor_address=address_of('GpSaveSlotCursor');machines=[]
        for label,path in [('original',ROOT/'assets/PAL/main.exe'),('linked',ROOT/'build/PAL/main.exe')]:
            memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000)
            image=path.read_bytes();memory.mem_write(struct.unpack_from('<I',image,24)[0]&0x1FFFFFFF,image[0x800:]);machines.append((label,memory))
        rng=random.Random(0x53415645);modes=(0,2,4,3,6,17,1,-1,0x20);coverage=set()
        for number in range(256):
            initial=bytearray(rng.randbytes(SIZE));control=OFFSETS['controls']
            struct.pack_into('<H',initial,control,number%3)
            initial[control+2:control+5]=bytes((number&1,(number//2)%7,(number//14)%7))
            mode=modes[number%len(modes)];coverage.add(mode);wanted=expected_world(initial,mode)
            self.native.run_writer(C.create_string_buffer(bytes(initial)),C.create_string_buffer(wanted),mode)
            def source_address(identifier):
                return DECALS[identifier] if identifier<3 else BUFFERS[(identifier-3)//2]+0x300+((identifier-3)%2)*0x840
            pointers=(BUFFERS[number&1],source_address(initial[control+3]),source_address(initial[control+4]))
            for label,memory in machines:
                read=lambda p,n:bytes(memory.mem_read(p&0x1FFFFFFF,n))
                write=lambda p,b:memory.mem_write(p&0x1FFFFFFF,bytes(b))
                for address,offset,length in bindings:write(address,initial[offset:offset+length])
                write(address_of('CustomEmblemBuffers')+0x840,b'\xA7'*0x840)
                for address,value in zip(pointer_cells,pointers):write(address,struct.pack('<I',value))
                write(cursor_address,initial[control:control+2]);write(STACK-0x410,b'\xA5'*0x450)
                allowed=[(pointers[0],0x4000),(address_of('SaveSlotSnapshots')+16,2),
                         (address_of('SaveSlotSnapshots')+60,2),(address_of('SaveSlotSnapshots')+104,2),
                         (address_of('SelectionState')+16,2),(STACK-0x400,0x400)]
                def store(cpu,access,address,length,value,user):
                    address&=0x1FFFFFFF
                    self.assertTrue(any((p&0x1FFFFFFF)<=address and address+length<=(p&0x1FFFFFFF)+n for p,n in allowed),
                                    (number,label,hex(cpu.pc),hex(address),length))
                cpu=Cpu(memory,0,100,101,102,store);cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)]
                cpu.r[4],cpu.r[29],cpu.r[31],cpu.pc=mode&0xFFFFFFFF,STACK,STOP,address_of('BuildSaveData')
                for _ in range(300000):
                    if cpu.pc==STOP:break
                    cpu.step()
                self.assertEqual((cpu.pc,cpu.r[29]),(STOP,STACK),(number,label,mode))
                for address,offset,length in bindings:self.assertEqual(read(address,length),wanted[offset:offset+length],(number,label,mode,hex(address)))
                for address,value in zip(pointer_cells,pointers):self.assertEqual(read(address,4),struct.pack('<I',value))
                self.assertEqual(read(address_of('CustomEmblemBuffers')+0x840,0x840),b'\xA7'*0x840)
                self.assertEqual(read(cursor_address,2),initial[control:control+2])
                self.assertEqual(read(STACK-0x410,16),b'\xA5'*16)
                for register in (*range(16,24),28,30):self.assertEqual(cpu.r[register],0xBEEF0000+register)
        self.assertEqual(coverage,set(modes))


if __name__=='__main__':unittest.main()
