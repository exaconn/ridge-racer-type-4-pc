"""Actual title-mask emitter and DMA encoder with complete native packet owners.

The oracle checks packed words and live ordering-head aliases against native C,
original MIPS and fully linked MIPS. Hardware scratch binds real full pointers;
registered GPU wire addresses never come from truncated native pointer bits.
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
STACK,STOP,SCRATCH=0x803F0000,0x803FF000,0x1F800000
ARENA_BYTES=320
WORLD_BYTES=16+2*ARENA_BYTES+24+16
ARENAS=(16,16+ARENA_BYTES)
VERTICES=16+2*ARENA_BYTES

NATIVE=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "rr/gpu.h"
#include "rr/gpu_scratch_types.h"
#include "rr/gpu_address.h"
#include "game/effect/spfade.h"
typedef union {GpuSpriteCommand packets[16];u32 words[80];u8 bytes[320];} Arena;
typedef struct {u32 before[4];Arena arena[2];s16 vertices[11];u16 vertexGuard;u32 after[4];} World;
typedef struct {s32 mask,animation;u32 cursorArena,cursorIndex,orderArena,orderIndex,base0,base1,empty;} Plan;
typedef struct {u32 before[4];GpuScratchPointers scratch;u32 after[4];} Roots;
static World actual;static Roots roots;static GpuDmaRegion regions[2];
#define SCRATCHPAD (&roots.scratch)
#define SpinningWheelBaseVerts actual.vertices
_Static_assert(sizeof(World)==696,"whole world");
_Static_assert(offsetof(World,arena)==16,"wire arenas");
_Static_assert(offsetof(World,vertices)==656,"wire vertices");
_Static_assert(sizeof(Plan)==36,"wire plan");
'''

TAIL=r'''
void run_mask(const u8 *input,const u8 *script,const u8 *expected){
 Plan plan;Roots saved;memcpy(&actual,input,sizeof(actual));memcpy(&plan,script,sizeof(plan));
 assert(plan.cursorArena<2&&plan.orderArena<2&&plan.cursorIndex<=5&&plan.orderIndex<79);
 regions[0]=(GpuDmaRegion){&actual.arena[0],plan.base0,sizeof(Arena)};
 regions[1]=(GpuDmaRegion){&actual.arena[1],plan.base1,sizeof(Arena)};
 assert(GpuDmaSetRegions(regions,2));
 assert(sizeof(void*)==8&&(uintptr_t)&actual.arena[0]>UINT32_MAX);
 memset(&roots,0xA5,sizeof(roots));
 roots.scratch.cursor=plan.empty?NULL:(u8*)&actual.arena[plan.cursorArena].packets[plan.cursorIndex];
 roots.scratch.order=plan.empty?NULL:&actual.arena[plan.orderArena].words[plan.orderIndex];
 memcpy(&saved,&roots,sizeof(saved));
 EmitTitleMaskSprites(plan.mask,plan.animation);
 unsigned count=(unsigned)__builtin_popcount((u32)plan.mask&0x7FFU);
 assert(!plan.empty||count==0);
 saved.scratch.cursor=plan.empty?NULL:actual.arena[plan.cursorArena].bytes+(plan.cursorIndex+count)*sizeof(GpuSpriteCommand);
 assert(!memcmp(&roots,&saved,sizeof(roots))&&!memcmp(&actual,expected,sizeof(actual)));
 assert(GpuDmaSetRegions(NULL,0));
}
'''


def signed(value):return value-(1<<32) if value&(1<<31) else value


def expected_world(initial,mask,animation,cursor_arena,cursor_index,order_arena,order_index,bases,empty):
    state=bytearray(initial);offset=cursor_index*20
    head=ARENAS[order_arena]+(order_index+1)*4
    # The low five animation bits select four UV phases; Python arithmetic
    # is intentionally unbounded and does not duplicate a signed C negate.
    uv=0x1BA4F0D0|((-animation)%32)//8
    def word(at,value):struct.pack_into('<I',state,at,value&0xFFFFFFFF)
    for bit in range(11):
        if mask&(1<<bit):
            packet=ARENAS[cursor_arena]+offset
            x=struct.unpack_from('<h',state,VERTICES+bit*2)[0]
            word(packet+4,0x65000000);word(packet+8,x|0xA80000)
            word(packet+16,0x100004);word(packet+12,uv)
            word(packet,struct.unpack_from('<I',state,head)[0]);state[packet+3]=4
            word(head,bases[cursor_arena]+offset);state[head+3]=0
            offset+=20
    cursor=0 if empty else bases[cursor_arena]+offset
    order=0 if empty else bases[order_arena]+order_index*4
    return bytes(state),struct.pack('<2I',cursor,order)


class TitleMaskPacketsTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup)
        source=Path(cls.temp.name)/'mask.c';library=source.with_suffix('.dylib')
        owner=source_owners(ROOT/'src/main/PAL')['EmitTitleMaskSprites']
        body=definition_source(owner.read_text(),'EmitTitleMaskSprites')
        source.write_text(NATIVE+body+TAIL)
        result=subprocess.run(['cc','-std=gnu11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                               '-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I',str(ROOT/'include'),
                               str(source),str(ROOT/'src/native/gpu_address.c'),'-o',str(library)],capture_output=True,text=True)
        if result.returncode:raise AssertionError(result.stderr)
        cls.native=C.CDLL(str(library));cls.native.run_mask.argtypes=[C.c_void_p]*3

    def test_all_masks_animation_extremes_and_live_packet_head_aliases(self):
        entry=address_of('EmitTitleMaskSprites');vertices=address_of('SpinningWheelBaseVerts');machines=[]
        for label,path in (('original',ROOT/'assets/PAL/main.exe'),('linked',ROOT/'build/PAL/main.exe')):
            memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000);memory.mem_map(SCRATCH,0x1000)
            image=path.read_bytes();memory.mem_write(struct.unpack_from('<I',image,24)[0]&0x1FFFFFFF,image[0x800:]);machines.append((label,memory))
        rng=random.Random(0x4D41534B);coverage=set()
        frames=(-2147483648,2147483647,-33,-32,-17,-16,-9,-8,-1,0,1,7,8,15,16,23,24,31,32,33)
        high=(0,0x7FFFF800,0x80000000,0xFFFFF800)
        cpu_segments=(0,0x80000000,0xA0000000,0)
        for case in range(8192):
            low=case%2048;profile=case//2048;mask=signed(low|high[profile]);frame=frames[case%len(frames)]
            cursor_arena=(case>>3)&1;cursor_index=(case>>4)%6;kind=case&7
            if kind==1 and cursor_index==0:cursor_index=1
            order_arena=1-cursor_arena if kind==0 else cursor_arena
            order_index=(0,cursor_index*5-1,cursor_index*5,cursor_index*5+1,cursor_index*5+2,cursor_index*5+3,(cursor_index+1)*5-1,78)[kind]
            bases=(cpu_segments[profile]|0x00200000,cpu_segments[(profile+1)%4]|0x00201000)
            empty=int(low==0 and profile%2==1)
            initial=rng.randbytes(WORLD_BYTES);wanted,root=expected_world(initial,mask,frame,cursor_arena,cursor_index,order_arena,order_index,bases,empty)
            plan=struct.pack('<2i7I',mask,frame,cursor_arena,cursor_index,order_arena,order_index,*bases,empty)
            self.native.run_mask(C.create_string_buffer(initial),C.create_string_buffer(plan),C.create_string_buffer(wanted))
            coverage.add(('mask',low));coverage.add(('alias',kind));coverage.add(('empty',empty));coverage.add(('frame',frame))
            for label,memory in machines:
                read=lambda p,n:bytes(memory.mem_read(p&0x1FFFFFFF,n))
                write=lambda p,b:memory.mem_write(p&0x1FFFFFFF,bytes(b))
                for index,base in enumerate(bases):
                    write(base-16,b'\xA6'*16);write(base,initial[ARENAS[index]:ARENAS[index]+ARENA_BYTES]);write(base+ARENA_BYTES,b'\x6A'*16)
                write(vertices,initial[VERTICES:VERTICES+24]);write(SCRATCH,b'\xA5'*64)
                cursor=0 if empty else bases[cursor_arena]+cursor_index*20
                order=0 if empty else bases[order_arena]+order_index*4
                write(SCRATCH,struct.pack('<2I',cursor,order));write(STACK-0x210,b'\xA5'*0x250)
                allowed=[(base,ARENA_BYTES) for base in bases]+[(SCRATCH,4),(STACK-0x200,0x200)]
                def store(cpu,access,address,length,value,user):
                    address&=0x1FFFFFFF
                    self.assertTrue(any((base&0x1FFFFFFF)<=address and address+length<=(base&0x1FFFFFFF)+size for base,size in allowed),(case,label,hex(cpu.pc),hex(address)))
                cpu=Cpu(memory,0,100,101,102,store);cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)]
                cpu.r[4],cpu.r[5],cpu.r[29],cpu.r[31],cpu.pc=mask&0xFFFFFFFF,frame&0xFFFFFFFF,STACK,STOP,entry
                for _ in range(2000):
                    if cpu.pc==STOP:break
                    cpu.step()
                self.assertEqual((cpu.pc,cpu.r[29]),(STOP,STACK),(case,label))
                for index,base in enumerate(bases):
                    self.assertEqual(read(base,ARENA_BYTES),wanted[ARENAS[index]:ARENAS[index]+ARENA_BYTES],(case,label,kind,index))
                    self.assertEqual(read(base-16,16),b'\xA6'*16);self.assertEqual(read(base+ARENA_BYTES,16),b'\x6A'*16)
                self.assertEqual(read(vertices,24),initial[VERTICES:VERTICES+24]);self.assertEqual(read(SCRATCH,8),root)
                self.assertEqual(read(SCRATCH+8,56),b'\xA5'*56)
                for register in (*range(16,24),28,30):self.assertEqual(cpu.r[register],0xBEEF0000+register)
                self.assertEqual(read(STACK-0x210,16),b'\xA5'*16);self.assertEqual(read(STACK+0x10,16),b'\xA5'*16)
        self.assertTrue({('mask',i) for i in range(2048)}<=coverage)
        self.assertTrue({('alias',i) for i in range(8)}<=coverage)
        self.assertTrue({('empty',i) for i in range(2)}<=coverage)
        self.assertTrue({('frame',i) for i in frames}<=coverage)


if __name__=='__main__':unittest.main()
