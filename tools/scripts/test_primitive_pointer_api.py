"""Actual sprite/line emitters: full native cursors, aliases and original MIPS.

SDK setters and ordering insertion are instrumented services. Every service
entry and exit is compared, including callback mutations. The independent
reference publishes packet bytes rather than using the production structs.
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
NAMES = ['AddSpritePrim', 'GameQueueTileTrans', 'AddLineF2SemiTrans']
SERVICES = ['SetSprt', 'SetShadeTex', 'SetSemiTrans', 'AddPrim', 'SetLineF2']
STACK, STOP = 0x803F0000, 0x803FF000
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "rr/sprite_emit.h"
#include "rr/gpu_line.h"
#include "rr/animated_glyph.h"
#include "psyq/libgpu/primattr.h"
#include "psyq/libgpu/settrans.h"
#include "psyq/libgpu/primatt6.h"
typedef union {SpritePacket sprites[64];GpuLineF2Packet lines[80];u32 words[320];u8 bytes[1280];} Arena;
typedef struct {u32 before[4];Arena value;u32 after[4];} ArenaOwner;
typedef struct {u32 before[4],values[4],after[4];} HeadOwner;
typedef struct {u32 before[4];UiUvRect values[4];u32 after[4];} RegionOwner;
typedef struct {ArenaOwner arenas[3];HeadOwner heads;RegionOwner regions;} World;
typedef struct {World entry,after;u32 operation,args[2];} Event;
typedef struct {World final;unsigned count;Event events[4];u32 result;} Capture;
static World live,initial;static Capture actual,expected,*capture;
static unsigned scenario,kind,serial,arenaIndex,packetIndex,regionIndex,headIndex;
static unsigned branchSeen[2],colorSeen[8],aliasSeen[8][8],arenaSeen[3],serviceSeen[6],mutationSeen[2];
static u8 firstWire[4032],lastWire[4032];
static const s32 edges[]={INT32_MIN,INT32_MIN+1,-65537,-32768,-1,0,1,32767,65536,INT32_MAX};
static const s32 colors[]={INT32_MIN,-2,-1,0,1,2,255,INT32_MAX};
static u32 next(u32 *seed){u32 n=*seed;n^=n<<13;n^=n>>17;n^=n<<5;return *seed=n;}
static u8 *packet(void){return live.arenas[arenaIndex].value.bytes+packetIndex*(kind?16U:20U);}
static UiUvRect *region(void){
 return regionIndex<4?&live.regions.values[regionIndex]:(UiUvRect*)(packet()+(regionIndex-4)*4U);
}
static u32 *head(void){return headIndex<4?&live.heads.values[headIndex]:(u32*)(packet()+(headIndex-4)*4U);}
static u32 pointer(const void *p){
 uintptr_t address=(uintptr_t)p;
 for(unsigned i=0;i<3;i++){
  uintptr_t base=(uintptr_t)&live.arenas[i];
  if(address>=base&&address<base+sizeof(ArenaOwner))return 0x80180000U+i*65536U+(u32)(address-base);
 }
 uintptr_t base=(uintptr_t)&live.heads;if(address>=base&&address<base+sizeof(HeadOwner))return 0x801B0000U+(u32)(address-base);
 base=(uintptr_t)&live.regions;if(address>=base&&address<base+sizeof(RegionOwner))return 0x801C0000U+(u32)(address-base);
 assert(!"pointer outside complete native fixture owners");return 0;
}
static void put16(void *p,u32 value){u16 bits=(u16)value;memcpy(p,&bits,2);}
static Event *begin(unsigned operation,const void *a,u32 b){
 unsigned index=capture->count++;assert(index<4);Event *e=&capture->events[index];
 memcpy(&e->entry,&live,sizeof(live));e->operation=operation;e->args[0]=pointer(a);e->args[1]=b;serviceSeen[operation]++;return e;
}
static void finish(Event *e){
 unsigned n=serial++;if(scenario&1){
  /* Mutations use actual byte aliases and remain within the owned packet.
   * They occur after each device effect, before the caller resumes. */
  u8 *p=packet();p[(n*3U)%20]^=(u8)(scenario+n+0x51U);
  for(unsigned i=0;i<4;i++)((u8*)region())[i]=(u8)(scenario*37U+n*73U+i*11U);
  live.heads.values[n%4]^=scenario*0x17171717U+n;
 }
 memcpy(&e->after,&live,sizeof(live));
}
void SetSprt(void *p){Event *e=begin(1,p,0);((u8*)p)[3]=4;((u8*)p)[7]=0x64;finish(e);}
void SetShadeTex(void *p,s32 flag){Event *e=begin(2,p,(u32)flag);u8 *c=(u8*)p+7;*c=flag?*c|1U:*c&0xFEU;finish(e);}
void SetSemiTrans(void *p,s32 flag){Event *e=begin(3,p,(u32)flag);u8 *c=(u8*)p+7;*c=flag?*c|2U:*c&0xFDU;finish(e);}
void AddPrim(u32 *ordering,u32 *p){
 Event *e=begin(4,ordering,pointer(p));*p=(*p&0xFF000000U)|(*ordering&0xFFFFFFU);
 *ordering=(*ordering&0xFF000000U)|(pointer(p)&0xFFFFFFU);finish(e);
}
void SetLineF2(void *p){Event *e=begin(5,p,0);((u8*)p)[3]=3;((u8*)p)[7]=0x40;finish(e);}
'''
REFERENCE = r'''
static s32 argument(unsigned slot){
 if(slot<4)return edges[(scenario/(slot+1)+slot*3U)%10];
 return slot==4?(s32)(scenario*0x17171717U):slot==5?colors[(scenario/3)%8]:edges[(scenario/11)%10];
}
static void *reference(void){
 u8 *p=packet();
 if(kind){
  SetLineF2(p);SetShadeTex(p,1);put16(p+8,(u32)argument(0));put16(p+10,(u32)argument(1));
  if(kind==2)p[7]|=2;
  put16(p+12,(u32)argument(2));put16(p+14,(u32)argument(3));
  p[4]=(u8)argument(4);p[5]=(u8)argument(5);p[6]=(u8)argument(6);AddPrim(head(),(u32*)p);return p+16;
 }
 s32 color=argument(5);s32 brightness=color>>1;SetSprt(p);
 if(brightness==0)SetShadeTex(p,1);else p[4]=p[5]=p[6]=(u8)brightness;
 SetSemiTrans(p,color&1);put16(p+8,(u32)argument(0));put16(p+10,(u32)argument(1));
 /* Read each UV byte when consumed, so overlapping stores remain observable. */
 u8 *uv=(u8*)region();put16(p+16,uv[2]);put16(p+18,uv[3]);p[12]=uv[0];p[13]=uv[1];put16(p+14,(u32)argument(4));
 AddPrim(head(),(u32*)p);return p+20;
}
static void initialize(unsigned n){
 scenario=n;kind=n%3;serial=0;arenaIndex=(n/72)%3;packetIndex=(n/3)%24;regionIndex=(n/216)%8;headIndex=(n/1728)%8;
 u32 seed=n*0x9E3779B9U+0x1FF84U;u8 *p=(u8*)&live;for(unsigned i=0;i<sizeof(live);i++)p[i]=(u8)next(&seed);
 memcpy(&initial,&live,sizeof(live));arenaSeen[arenaIndex]++;aliasSeen[regionIndex][headIndex]++;colorSeen[(n/3)%8]++;mutationSeen[n&1]++;
 branchSeen[argument(5)>>1!=0]++;
}
static void *run(void){
 if(kind==0)return AddSpritePrim(head(),packet(),argument(0),argument(1),region(),(u16)argument(4),argument(5));
 if(kind==1)return GameQueueTileTrans(head(),(GpuLineF2Packet*)packet(),argument(0),argument(1),argument(2),argument(3),argument(4),argument(5),argument(6));
 return AddLineF2SemiTrans(head(),(GpuLineF2Packet*)packet(),argument(0),argument(1),argument(2),argument(3),argument(4),argument(5),argument(6));
}
unsigned exercise_native(void){
 _Static_assert(sizeof(World)==4032&&sizeof(ArenaOwner)==1312,"complete native arena and guards");
 for(unsigned n=0;n<65536;n++){
  initialize(n);assert((uintptr_t)packet()>UINT32_MAX&&(uintptr_t)head()>UINT32_MAX&&(uintptr_t)region()>UINT32_MAX);
  capture=&expected;capture->count=0;capture->result=pointer(reference());memcpy(&capture->final,&live,sizeof(live));
  memcpy(&live,&initial,sizeof(live));serial=0;capture=&actual;capture->count=0;capture->result=pointer(run());memcpy(&capture->final,&live,sizeof(live));
  assert(actual.count==expected.count&&actual.result==expected.result);
  for(unsigned i=0;i<actual.count;i++)assert(!memcmp(&actual.events[i],&expected.events[i],sizeof(Event)));
  assert(!memcmp(&actual.final,&expected.final,sizeof(World)));
 }
 for(unsigned i=0;i<8;i++){assert(colorSeen[i]);for(unsigned j=0;j<8;j++)assert(aliasSeen[i][j]);}
 for(unsigned i=1;i<6;i++)assert(serviceSeen[i]);for(unsigned i=0;i<3;i++)assert(arenaSeen[i]);
 assert(branchSeen[0]&&branchSeen[1]&&mutationSeen[0]&&mutationSeen[1]);return 65536;
}
unsigned prepare_trace(unsigned n){initialize(n);memcpy(firstWire,&live,sizeof(live));capture=&expected;capture->count=0;capture->result=pointer(reference());memcpy(lastWire,&live,sizeof(live));return capture->count;}
const void *initial_wire(void){return firstWire;}const void *final_wire(void){return lastWire;}
const void *event_wire(unsigned i,unsigned after){assert(i<capture->count);return after?(const void*)&capture->events[i].after:(const void*)&capture->events[i].entry;}
u32 event_value(unsigned i,unsigned field){Event *e=&capture->events[i];return field?e->args[field-1]:e->operation;}
u32 call_argument(unsigned slot){
 if(!slot)return pointer(head());if(slot==1)return pointer(packet());
 if(!kind)return slot==2?(u32)argument(0):slot==3?(u32)argument(1):slot==4?pointer(region()):slot==5?(u32)argument(4):(u32)argument(5);
 return (u32)argument(slot-2);
}
u32 result(void){return capture->result;}
'''


class PrimitivePointerApiTest(unittest.TestCase):
    def test_native_aliases_and_original_mips(self):
        owners = source_owners(ROOT/'src/main/PAL')
        bodies = '\n'.join(definition_source(owners[name].read_text(), name) for name in NAMES)
        with TemporaryDirectory() as temporary:
            source, library = Path(temporary)/'native.c', Path(temporary)/'native.dylib'
            source.write_text(PREFIX+bodies+REFERENCE)
            subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                            '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-I', str(ROOT/'include'),
                            str(source), '-o', str(library)], check=True)
            native = C.CDLL(str(library));native.exercise_native.restype = C.c_uint
            self.assertEqual(native.exercise_native(), 65536)
            for name in ['initial_wire', 'final_wire', 'event_wire']:
                getattr(native, name).restype = C.c_void_p
            for name, count in [('prepare_trace', 1), ('event_wire', 2), ('event_value', 2), ('call_argument', 1)]:
                getattr(native, name).argtypes = [C.c_uint]*count
            for name in ['prepare_trace', 'event_value', 'call_argument', 'result']:
                getattr(native, name).restype = C.c_uint32
            regions = [(0x80180000+i*65536, 1312) for i in range(3)]+[(0x801B0000, 48), (0x801C0000, 48)]
            memory = Uc(UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN | UC_MODE_MIPS32);memory.mem_map(0, 0x400000)
            write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b))
            read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
            original = (ROOT/'assets/PAL/main.exe').read_bytes();base = struct.unpack_from('<I', original, 24)[0];write(base, original[0x800:])
            if os.getenv('RR_COMPILED_ELF'):
                with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                    elf = ELFFile(stream)
                    for name in NAMES:
                        symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(name)[0]
                        section = elf.get_section(symbol['st_shndx']);offset = symbol['st_value']-section['sh_addr']
                        self.assertEqual(symbol['st_value'], address_of(name));write(symbol['st_value'], section.data()[offset:offset+symbol['st_size']])

            def store(cpu, access, address, length, value, user):
                address &= 0x1FFFFFFF
                self.assertTrue(any((a & 0x1FFFFFFF) <= address and address+length <= (a & 0x1FFFFFFF)+n
                                    for a, n in regions+[(STACK-64, 64)]), (hex(address), length))

            cpu = Cpu(memory, 0, 100, 101, 102, store)

            def write_state(data):
                offset = 0
                for address, length in regions:
                    write(address, data[offset:offset+length]);offset += length

            def state():
                return b''.join(read(a, n) for a, n in regions)

            services = {address_of(name): i+1 for i, name in enumerate(SERVICES)}
            for case in range(1536):
                number = (case*7919) & 65535;kind = number % 3;events = native.prepare_trace(number)
                arguments = [native.call_argument(i) for i in range(9 if kind else 7)]
                write_state(C.string_at(native.initial_wire(), 4032));write(STACK-128, b'\xA5'*256)
                for i, value in enumerate(arguments[4:]):write(STACK+16+i*4, struct.pack('<I', value))
                entry_stack = read(STACK, 128)
                cpu.reset();cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)];cpu.r[4:8] = arguments[:4]
                cpu.r[29], cpu.r[31], cpu.pc = STACK, STOP, address_of(NAMES[kind]);event = 0
                for instruction in range(1000):
                    if cpu.pc == STOP:break
                    if cpu.pc not in services:cpu.step();continue
                    operation = services[cpu.pc];self.assertLess(event, events)
                    self.assertEqual(operation, native.event_value(event, 0), (number, event))
                    self.assertEqual(cpu.r[4], native.event_value(event, 1), (number, event, 'pointer'))
                    if operation in (2, 3, 4):self.assertEqual(cpu.r[5], native.event_value(event, 2), (number, event, 'argument'))
                    self.assertEqual(state(), C.string_at(native.event_wire(event, 0), 4032), (number, event, 'entry'))
                    write_state(C.string_at(native.event_wire(event, 1), 4032));resume = cpu.r[31]
                    for register in (1, 2, 3, *range(4, 16), 24, 25):cpu.r[register] = (0xA1230000+event*257+register) & 0xFFFFFFFF
                    cpu.pc = resume;cpu.hi = 0x12345678;cpu.lo = 0x87654321;event += 1
                self.assertEqual(cpu.pc, STOP, number);self.assertEqual(event, events)
                self.assertEqual(state(), C.string_at(native.final_wire(), 4032), (number, 'final'));self.assertEqual(cpu.r[2], native.result())
                self.assertEqual(cpu.r[29], STACK);self.assertEqual(cpu.r[31], STOP)
                for register in (*range(16, 24), 28, 30):self.assertEqual(cpu.r[register], 0xBEEF0000+register)
                self.assertEqual(read(STACK-128, 64), b'\xA5'*64);self.assertEqual(read(STACK, 128), entry_stack)
            print('PASS 65536 native and 1536', 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
                  'MIPS primitive API cases; full pointers, all64 UV/head alias pairs, callback mutations, complete owners and UBSan')


if __name__ == '__main__':
    unittest.main()
