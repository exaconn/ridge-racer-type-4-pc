"""Actual grid allocator: native pointers, clear-service trace and original MIPS."""
import ctypes as C
import os
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32

from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
NAME = 'AllocMenuGridRecords'
BASE, STACK, STOP = 0x80200010, 0x803F0000, 0x803FF000
SLOT, BYTES, CASES = 0x800F3A28, 4*8*56, 4096
OUTPUT_SIZE = 20+2*BYTES
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "game/menu/mnsel.h"
#include "rr/menu_grid.h"
#include "rr/bios_devices.h"
#include "game/gfx/skyrot1.h"
static struct {u8 before[16];MenuGrid38 records[4][8];u8 after[16];} arena;
MenuGrid38 *MenuGridRecords;
static unsigned scenario,calls;static u8 *trace;
static u32 address(const void *p){
 uintptr_t offset=(uintptr_t)p-(uintptr_t)arena.records;assert(offset<sizeof(arena.records));return 0x80200010U+(u32)offset;
}
static void put32(u8 **p,u32 x){memcpy(*p,&x,4);*p+=4;}
void PsyqBzero(void *buffer,s32 size){
 uintptr_t offset=(uintptr_t)buffer-(uintptr_t)arena.records;
 assert(calls++==0&&size>=0&&offset+size<=sizeof(arena.records));
 put32(&trace,address(buffer));put32(&trace,(u32)size);put32(&trace,address(MenuGridRecords));
 memcpy(trace,arena.records,sizeof(arena.records));trace+=sizeof(arena.records);
 memset(buffer,0,(size_t)size);
 /* A service may replace the published root. The allocator must retain it. */
 if(scenario&1)MenuGridRecords=&arena.records[(scenario>>3)&3][(scenario>>5)&7];
}
'''
TAIL = r'''
void run_case(unsigned number,unsigned owner,unsigned first,int count,const u8 *input,u8 *output){
 unsigned i;int size;assert(owner<4&&first<8&&count>=0&&(unsigned)count<=8-first);
 assert(sizeof(void*)==8&&(uintptr_t)&arena>UINT32_MAX);
 assert(offsetof(IntroSceneState,p40)==0x40);
 assert(offsetof(IntroSceneState,s44)==0x40+sizeof(void*));
 assert(offsetof(IntroSceneState,objectYawAA)==0xA6+sizeof(void*));
 assert(sizeof(((IntroSceneState*)0)->mat00)==32&&sizeof(((IntroSceneState*)0)->objectYawAA)==16);
 memset(&arena,0xA5,sizeof(arena));memcpy(arena.records,input,sizeof(arena.records));
 MenuGridRecords=&arena.records[(number>>2)&3][(number>>4)&7];scenario=number;calls=0;trace=output;
 size=AllocMenuGridRecords(&arena.records[owner][first],count);assert(calls==1);
 put32(&trace,(u32)size);put32(&trace,address(MenuGridRecords));memcpy(trace,arena.records,sizeof(arena.records));
 assert(trace+sizeof(arena.records)-output==20+2*sizeof(arena.records));
 for(i=0;i<16;i++)assert(arena.before[i]==0xA5&&arena.after[i]==0xA5);
}
'''


def main():
    os.chdir(ROOT)
    owner = source_owners(ROOT/'src/main/PAL')[NAME]
    body = definition_source(owner.read_text(), NAME)
    with TemporaryDirectory() as temporary:
        source, library = Path(temporary)/'native.c', Path(temporary)/'native.dylib'
        source.write_text(PREFIX+body+TAIL)
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-Iinclude',
                        str(source), '-o', str(library)], check=True)
        native = C.CDLL(str(library));native.run_case.restype = None
        native.run_case.argtypes = [C.c_uint]*3+[C.c_int, C.c_void_p, C.c_void_p]
        memory = Uc(UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN | UC_MODE_MIPS32);memory.mem_map(0, 0x400000)
        write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b))
        read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
        original = (ROOT/'assets/PAL/main.exe').read_bytes();base = struct.unpack_from('<I', original, 0x18)[0]
        write(base, original[0x800:])
        if os.getenv('RR_COMPILED_ELF'):
            with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                elf = ELFFile(stream);symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(NAME)[0]
                section = elf.get_section(symbol['st_shndx']);offset = symbol['st_value']-section['sh_addr']
                assert symbol['st_value'] == address_of(NAME)
                write(symbol['st_value'], section.data()[offset:offset+symbol['st_size']])

        def store(cpu, access, address, size, value, user):
            address &= 0x1FFFFFFF
            assert (address == (SLOT & 0x1FFFFFFF) and size == 4
                    or (STACK & 0x1FFFFFFF)-32 <= address and address+size <= (STACK & 0x1FFFFFFF))

        cpu = Cpu(memory, 0, 100, 101, 102, store);rng = random.Random(0x41A18);layouts = set()
        for case in range(CASES):
            initial = rng.randbytes(BYTES);count = case % 9;owner = (case//9) % 4
            first = (case//36) % (9-count) if count else (case//36) % 8
            target = BASE+(owner*8+first)*56;size = count*56
            output = (C.c_ubyte*OUTPUT_SIZE)()
            native.run_case(case, owner, first, count, C.create_string_buffer(initial), output);expected = bytes(output)
            write(BASE-16, b'\xA5'*16+initial+b'\xA5'*16);write(SLOT-8, b'\xA5'*20)
            write(SLOT, struct.pack('<I', BASE+(((case>>2)&3)*8+((case>>4)&7))*56))
            cpu.reset();cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
            cpu.r[4:6] = [target, count];cpu.r[29] = STACK;cpu.r[31] = STOP;cpu.pc = address_of(NAME)
            calls = 0;trace = b''
            for instruction in range(100):
                if cpu.pc == STOP:break
                if cpu.pc == address_of('PsyqBzero'):
                    calls += 1;assert calls == 1
                    assert cpu.r[4:6] == [target, size]
                    trace = struct.pack('<2I', cpu.r[4], cpu.r[5])+read(SLOT, 4)+read(BASE, BYTES)
                    write(target, b'\0'*size)
                    if case&1:write(SLOT, struct.pack('<I', BASE+(((case>>3)&3)*8+((case>>5)&7))*56))
                    cpu.pc = cpu.r[31]
                    for reg in (*range(2, 16), 24, 25):cpu.r[reg] = 0xDEAD0000+reg
                else:cpu.step()
            assert cpu.pc == STOP and calls == 1, (case, hex(cpu.pc))
            actual = trace+struct.pack('<I', cpu.r[2])+read(SLOT, 4)+read(BASE, BYTES)
            assert actual == expected, (case, 'native/MIPS protocol and complete state')
            independent = bytearray(initial);start = target-BASE;independent[start:start+size] = b'\0'*size
            final_root = BASE+(((case>>3)&3)*8+((case>>5)&7))*56 if case&1 else target
            assert trace == struct.pack('<3I', target, size, target)+initial
            assert read(BASE, BYTES) == independent and cpu.r[2] == size
            assert struct.unpack('<I', read(SLOT, 4))[0] == final_root
            assert cpu.r[29] == STACK and cpu.r[31] == STOP
            for reg in (*range(16, 24), 28, 30):assert cpu.r[reg] == 0xBEEF0000+reg
            assert read(BASE-16, 16) == b'\xA5'*16 and read(BASE+BYTES, 16) == b'\xA5'*16
            assert read(SLOT-8, 8) == b'\xA5'*8 and read(SLOT+4, 8) == b'\xA5'*8
            layouts.add((owner, first, count))
        assert len(layouts) == 4*(8+sum(range(1, 9)))
        print('PASS', CASES, 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
              'MIPS/native grid allocation cases; all valid starts/counts in four arenas, actual public API/high pointers, published root before clear, mutable service, exact footprint, whole owners, return size, ABI, guards and UBSan')


if __name__ == '__main__':
    main()
