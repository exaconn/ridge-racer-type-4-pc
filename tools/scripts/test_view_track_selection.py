"""Camera track selection through actual C interfaces and complete native owners.

Runs the real node/segment/octant/table chain against original and linked MIPS.
Only the geometric track search is a service double; it may replace the shared
camera yaw, track bank and visibility bank before the caller resumes.
"""
import ctypes as C
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
NAMES = ['GetViewTrackNode', 'GetViewTrackSegment', 'YawToOctant',
         'GetSegmentViewTable', 'GetTrackSegmentAndYaw']
SCRATCH, NODES, SLOTS, PAYLOAD = 0x1F800000, 0x80210000, 0x80220000, 0x80230000
STACK, STOP = 0x803F0000, 0x803FF000
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "rr/player_cars.h"
#include "rr/track_sample_types.h"
#include "rr/relative_file.h"
#include "rr/rro_model.h"
#include "game/gfx/model.h"
#include "game/camera/viewcam.h"
#include "game/course/trkclip.h"
typedef struct {
 u8 before[32];RroModelScratch scratch;RaceVehicle cars[2];
 TrackSample3C nodes[2][80];RelativeSlot slots[2][128];u32 payload[2][128][2];
 s32 mode;short segmentYaw[2];u8 after[32];
} World;
static World live,initial,expected;
static TrackSample3C *trackRoot;
static RelativeSlot *slotRoot;
static unsigned scenario,calls,player;
static s32 node;
#define SCRATCHPAD ((u8*)&live.scratch)
#define G ((RroSceneScratchPrefix *)SCRATCHPAD)
#define PlayerCars live.cars
#define TrackNodes trackRoot
#define ViewUsesCarTrackNode live.mode
#define RelFileSegmentViewSlots slotRoot
#define D_800F4C58 live.segmentYaw
static s32 signed_word(u32 value){s32 out;memcpy(&out,&value,4);return out;}
static s32 changed_yaw(void){return signed_word(0x81726354U+scenario*0x12345U);}
int FindTrackNodeAtXZ(int x,int z) {
 assert(calls++==0);
 assert(x==initial.scratch.scene.view.projection.pose08.position00.xyz.x);
 assert(z==initial.scratch.scene.view.projection.pose08.position00.xyz.z);
 if(scenario&64U) {
  live.scratch.scene.view.projection.pose08.angles10.xyz.y=changed_yaw();
  live.mode=-1;trackRoot=live.nodes[1]+40;slotRoot=live.slots[1]+64;
 }
 return node;
}
'''
TAIL = r'''
void setup(unsigned number,s32 mode,unsigned selected,s32 returned,s32 car0,s32 car1,s32 yaw,s32 x,s32 z) {
 scenario=number;player=selected;node=returned;calls=0;
 memset(&live,0xA5,sizeof(live));
 live.mode=mode;live.cars[0].trackNodeF0=car0;live.cars[1].trackNodeF0=car1;
 live.scratch.scene.view.projection.pose08.position00.xyz.x=x;
 live.scratch.scene.view.projection.pose08.position00.xyz.z=z;
 live.scratch.scene.view.projection.pose08.angles10.xyz.y=yaw;
 for(unsigned bank=0;bank<2;bank++)for(unsigned i=0;i<80;i++)
  live.nodes[bank][i].heading12=(s16)((i*1031U+number*17U+bank*13007U)&65535U);
 for(unsigned bank=0;bank<2;bank++)for(unsigned i=0;i<128;i++) {
  unsigned target=(i*37U+number+bank*13U)&127U;
  live.slots[bank][i].absolute=(u8*)live.payload[bank][target];
 }
 trackRoot=live.nodes[0]+40;slotRoot=live.slots[0]+64;
 memcpy(&initial,&live,sizeof(live));
 assert(sizeof(void*)==8&&(uintptr_t)&live>UINT32_MAX);
 assert(offsetof(ProjectionScratchPrefix,pose08)==2*sizeof(void*));
 assert(sizeof(ProjectionView32)==32&&offsetof(ProjectionView32,angles10)==16);
}
u32 invoke(unsigned operation) {
 const ProjectionView32 *view=&live.scratch.scene.view.projection.pose08;
 switch(operation) {
 case 0:return (u32)GetViewTrackNode(view->position00.words,player);
 case 1:return (u32)GetViewTrackSegment(view,player);
 case 2:return (u32)YawToOctant(view);
 case 3:{
  const u8 *result=(const u8*)GetSegmentViewTable(player);
  uintptr_t offset=(uintptr_t)result-(uintptr_t)live.payload;
  assert(offset<sizeof(live.payload)&&offset%8==0);
  return 0x80230000U+(u32)offset;
 }
 default:assert(0);return 0;
 }
}
void check(unsigned operation,unsigned searched,unsigned bank,s32 mode,s32 yaw,s32 segment,s32 relativeYaw) {
 assert(calls==searched&&calls<=1);
 memcpy(&expected,&initial,sizeof(expected));
 if(searched&&(scenario&64U)) {
  expected.scratch.scene.view.projection.pose08.angles10.xyz.y=yaw;
  expected.mode=mode;
 }
 if((operation==1||operation==3)&&initial.mode==0) {
  expected.segmentYaw[0]=(s16)segment;expected.segmentYaw[1]=(s16)relativeYaw;
 }
 assert(trackRoot==live.nodes[bank]+40&&slotRoot==live.slots[bank]+64);
 assert(!memcmp(&live,&expected,sizeof(live)));
 for(unsigned b=0;b<2;b++)for(unsigned i=0;i<128;i++) {
  unsigned target=(i*37U+scenario+b*13U)&127U;
  assert(live.slots[b][i].absolute==(u8*)live.payload[b][target]);
  assert((uintptr_t)live.slots[b][i].absolute>UINT32_MAX);
 }
}
'''


def signed(value):
    return C.c_int32(value).value


def quotient(value):
    return abs(value)//5 * (-1 if value < 0 else 1)


class ViewTrackSelectionTest(unittest.TestCase):
    def test_native_chain_against_original_and_compiled_mips(self):
        owners = source_owners(ROOT/'src/main/PAL')
        bodies = '\n'.join(definition_source(owners[n].read_text(), n) for n in NAMES)
        with TemporaryDirectory() as directory:
            source, library = Path(directory)/'native.c', Path(directory)/'native.dylib'
            source.write_text(PREFIX+bodies+TAIL)
            subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                            '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
                            '-I', str(ROOT/'include'), str(source), '-o', str(library)], check=True)
            native = C.CDLL(str(library))
            native.setup.argtypes = [C.c_uint, C.c_int32, C.c_uint, *([C.c_int32]*6)]
            native.invoke.argtypes = [C.c_uint];native.invoke.restype = C.c_uint32
            native.check.argtypes = [C.c_uint, C.c_uint, C.c_uint, *([C.c_int32]*4)]
            memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
            memory.mem_map(0, 0x400000);memory.mem_map(SCRATCH, 0x1000)
            read = lambda a,n: bytes(memory.mem_read(a & 0x1FFFFFFF,n))
            write = lambda a,data: memory.mem_write(a & 0x1FFFFFFF,bytes(data))
            put = lambda a,v: write(a,struct.pack('<I',v & 0xFFFFFFFF))
            globals_ = {n:address_of(n) for n in ['ViewUsesCarTrackNode','PlayerCars','TrackNodes','RelFileSegmentViewSlots','D_800F4C58']}
            spans = {n:(int(a,16),int(s,16)) for n,a,s in map(str.split,(ROOT/'configs/PAL/functions.txt').read_text().splitlines())}
            exe = (ROOT/'assets/PAL/main.exe').read_bytes();base = struct.unpack_from('<I',exe,24)[0]
            variants = {'original':{n:exe[0x800+spans[n][0]-base:0x800+sum(spans[n])-base] for n in NAMES}}
            with (ROOT/'build/PAL/main.elf').open('rb') as stream:
                elf = ELFFile(stream);table = elf.get_section_by_name('.symtab');compiled = {}
                for name in NAMES:
                    symbol=table.get_symbol_by_name(name)[0];section=elf.get_section(symbol['st_shndx'])
                    self.assertEqual(symbol['st_value'],spans[name][0]);offset=symbol['st_value']-section['sh_addr']
                    compiled[name]=section.data()[offset:offset+symbol['st_size']]
                variants['compiled']=compiled
            expected_stores=[];seen=set();service=address_of('FindTrackNodeAtXZ')
            def store(cpu,access,address,size,value,user):
                a=address & 0x1FFFFFFF
                stack=(STACK-128)&0x1FFFFFFF;result=globals_['D_800F4C58']&0x1FFFFFFF
                self.assertTrue(stack<=a and a+size<=stack+128 or result<=a and a+size<=result+4,(hex(address),size))
                if result<=a<result+4:expected_stores.append((a-result,size,value & ((1<<(size*8))-1)))
            cpu=Cpu(memory,0,100,101,102,store)
            rng=random.Random(0x6F1EC)
            edges=[-2147483648,-2147483647,-65537,-4097,-1,0,1,255,256,4095,4096,65537,2147483647]
            modes=[0,1,-1,2,-2147483648]
            for scenario in range(4096):
                operation=scenario%4;player=(scenario//4)%2;mode=modes[(scenario//8)%5]
                yaw=edges[(scenario//40)%len(edges)] if scenario<2080 else signed(rng.getrandbits(32))
                node=scenario%80-40;car0=(scenario*17)%80-40;car1=(scenario*31)%80-40
                if operation<2 and mode!=0:
                    car0=edges[(scenario//3)%len(edges)];car1=edges[(scenario//7)%len(edges)]
                x,z=signed(rng.getrandbits(32)),signed(rng.getrandbits(32))
                searched=int(operation!=2 and mode==0);bank=int(bool(searched and scenario&64))
                final_yaw=signed(0x81726354+scenario*0x12345) if bank else yaw
                final_mode=-1 if bank else mode
                selected=node if mode==0 else (car0,car1)[player];segment=quotient(selected)
                heading=C.c_int16(((selected+40)*1031+scenario*17+bank*13007)&65535).value
                relative=(final_yaw-(2048-heading))&4095
                octant=((final_yaw&4095)+256)>>9&7
                if operation==0:result=selected & 0xFFFFFFFF
                elif operation==1:result=segment & 0xFFFFFFFF
                elif operation==2:result=octant
                else:
                    index=segment*8+octant+64;self.assertTrue(0<=index<128)
                    result=PAYLOAD+bank*1024+((index*37+scenario+bank*13)&127)*8
                native.setup(scenario,mode,player,node,car0,car1,yaw,x,z)
                self.assertEqual(native.invoke(operation),result,scenario)
                native.check(operation,searched,bank,final_mode,final_yaw,segment,relative)
                seen.add((operation,mode,player,bank))
                scratch=bytearray(b'\xA5'*0xDC);struct.pack_into('<i',scratch,8,x);struct.pack_into('<i',scratch,16,z);struct.pack_into('<i',scratch,28,yaw)
                cars=bytearray(b'\xA5'*1600);struct.pack_into('<i',cars,0xF0,car0);struct.pack_into('<i',cars,800+0xF0,car1)
                nodes=bytearray(b'\xA5'*(2*80*60));slots=bytearray(1024)
                for b in range(2):
                    for i in range(80):struct.pack_into('<H',nodes,(b*80+i)*60+18,(i*1031+scenario*17+b*13007)&65535)
                    for i in range(128):struct.pack_into('<I',slots,(b*128+i)*4,PAYLOAD+b*1024+((i*37+scenario+b*13)&127)*8)
                for variant,code in variants.items():
                    for name,data in code.items():write(spans[name][0],data)
                    write(SCRATCH,b'\xA5'*0x1000);write(SCRATCH,scratch)
                    # PlayerCars is the actual resident array, not a pointer slot.
                    write(globals_['PlayerCars']-16,b'\x6A'*16+cars+b'\xA6'*16)
                    write(NODES-16,b'\x6A'*16+nodes+b'\xA6'*16);write(SLOTS-16,b'\x6A'*16+slots+b'\xA6'*16)
                    put(globals_['ViewUsesCarTrackNode'],mode);put(globals_['TrackNodes'],NODES+40*60);put(globals_['RelFileSegmentViewSlots'],SLOTS+64*4)
                    write(globals_['D_800F4C58']-4,b'\x6A'*4+b'\xA5'*4+b'\xA6'*4)
                    write(STACK-256,b'\xA5'*512)
                    cpu.reset();cpu.r=[0]+[0xBEEF0000+r for r in range(1,32)];cpu.r[29]=STACK;cpu.r[31]=STOP
                    cpu.r[4:6]=[player,0x12345678] if operation==3 else [SCRATCH+8,player]
                    cpu.pc=spans[NAMES[operation]][0];expected_stores.clear();search_calls=0
                    for step in range(500):
                        if cpu.pc==STOP:break
                        if cpu.pc==service:
                            self.assertIsNone(cpu.delay);self.assertEqual(search_calls,0);search_calls+=1
                            self.assertEqual(cpu.r[4:6],[x&0xFFFFFFFF,z&0xFFFFFFFF])
                            if bank:
                                put(SCRATCH+28,final_yaw);put(globals_['ViewUsesCarTrackNode'],final_mode)
                                put(globals_['TrackNodes'],NODES+80*60+40*60);put(globals_['RelFileSegmentViewSlots'],SLOTS+128*4+64*4)
                            resume=cpu.r[31]
                            for reg in (1,2,3,*range(4,16),24,25):cpu.r[reg]=0xDEAD0000+reg
                            cpu.r[2]=node&0xFFFFFFFF;cpu.pc=resume;cpu.hi=0x12345678;cpu.lo=0x87654321
                        else:cpu.step()
                    self.assertEqual(cpu.pc,STOP,(variant,scenario));self.assertEqual(cpu.r[2],result,(variant,scenario))
                    self.assertEqual(cpu.r[29],STACK);self.assertEqual(cpu.r[31],STOP)
                    for reg in (*range(16,24),28,30):self.assertEqual(cpu.r[reg],0xBEEF0000+reg)
                    self.assertEqual(search_calls,searched)
                    expected_pair=struct.pack('<hh',segment,relative) if operation in (1,3) and mode==0 else b'\xA5'*4
                    self.assertEqual(read(globals_['D_800F4C58']-4,12),b'\x6A'*4+expected_pair+b'\xA6'*4)
                    self.assertEqual(expected_stores,[(2,2,relative),(0,2,segment&65535)] if operation in (1,3) and mode==0 else [])
                    final_scratch=bytearray(scratch)
                    if bank:struct.pack_into('<i',final_scratch,28,final_yaw)
                    self.assertEqual(read(SCRATCH,len(scratch)),final_scratch)
                    self.assertEqual(read(globals_['PlayerCars']-16,1632),b'\x6A'*16+cars+b'\xA6'*16)
                    self.assertEqual(read(NODES-16,len(nodes)+32),b'\x6A'*16+nodes+b'\xA6'*16)
                    self.assertEqual(read(SLOTS-16,1056),b'\x6A'*16+slots+b'\xA6'*16)
                    self.assertEqual(read(STACK-256,128),b'\xA5'*128);self.assertEqual(read(STACK,256),b'\xA5'*256)
                    self.assertEqual(read(globals_['ViewUsesCarTrackNode'],4),struct.pack('<i',final_mode))
                    self.assertEqual(read(globals_['TrackNodes'],4),struct.pack('<I',NODES+(bank*80+40)*60))
                    self.assertEqual(read(globals_['RelFileSegmentViewSlots'],4),struct.pack('<I',SLOTS+(bank*128+64)*4))
            self.assertEqual({(op,mode,player) for op,mode,player,bank in seen},{(op,mode,player) for op in range(4) for mode in modes for player in range(2)})
            self.assertTrue(any(bank for op,mode,player,bank in seen))
            print('4096 native / 8192 original-and-compiled MIPS cases: actual camera selection chain, both players, signed yaw/node edges, mutable roots, full pointer slots, ordered stores, full owner guards and UBSan')
