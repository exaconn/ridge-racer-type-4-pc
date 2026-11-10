"""Current intro camera glue, real native scratch owner and original MIPS.

BuildCameraMatrices is an instrumented service. Every halfword pattern in the
three doubled Y-row cells is covered; this does not model the service's trig.
"""
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
NAME = 'BuildScaledCameraMatrices'
SCRATCH, PIXELS, ORDER = 0x1F800000, 0x80250010, 0x80260010
STACK, STOP, CASES = 0x803F0000, 0x803FF000, 65536
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "rr/projection_state.h"
#include "game/gfx/gtemodel.h"
#include "game/math/rotmat.h"
static struct {
 u8 before[16];ViewMatrixScratch48 scratch;u8 after[16];
 u8 pixels[2][16];u32 order[2][4];
} world;
#define SCRATCHPAD ((u8*)&world.scratch)
static unsigned scenario,calls;static u8 *trace;
static void put32(u8 **p,u32 value){memcpy(*p,&value,4);*p+=4;}
static void snapshot(u8 *p){
 unsigned cursor=world.scratch.projection.pointers.cursor==world.pixels[1];
 unsigned order=world.scratch.projection.pointers.order==world.order[1];
 assert(world.scratch.projection.pointers.cursor==world.pixels[cursor]);
 assert(world.scratch.projection.pointers.order==world.order[order]);
 put32(&p,0x80250010U+cursor*16);put32(&p,0x80260010U+order*16);
 memcpy(p,(u8*)&world.scratch.projection+offsetof(ProjectionScratchPrefix,pose08.position00),28);p+=28;
 memcpy(p,&world.scratch.projection.pose08.angles10.words[3],4);p+=4;memcpy(p,&world.scratch.matrix28,32);
}
void BuildCameraMatrices(void){
 unsigned i;assert(calls++==0);snapshot(trace);
 world.scratch.projection.pointers.cursor=world.pixels[(scenario>>1)&1];
 world.scratch.projection.pointers.order=world.order[(scenario>>2)&1];
 world.scratch.projection.pose08.position00.words[0]=(s32)(scenario^0x17171717U);
 for(i=0;i<16;i++)world.scratch.matrix28.halves[i]=(u16)(scenario+i*0x1111U);
}
'''
TAIL = r'''
void run_case(unsigned number,const u8 *input,u8 *output){
 unsigned i;u32 x;memset(&world,0xA5,sizeof(world));scenario=number;calls=0;trace=output;
 assert(sizeof(void*)==8&&(uintptr_t)&world>UINT32_MAX);
 assert(offsetof(ProjectionScratchPrefix,pose08.position00)==2*sizeof(void*));
 memcpy(&x,input,4);assert(x==0x80250010U+(number&1)*16);
 memcpy(&x,input+4,4);assert(x==0x80260010U+((number>>1)&1)*16);
 world.scratch.projection.pointers.cursor=world.pixels[number&1];
 world.scratch.projection.pointers.order=world.order[(number>>1)&1];
 memcpy((u8*)&world.scratch.projection+offsetof(ProjectionScratchPrefix,pose08.position00),input+8,28);
 memcpy(&world.scratch.projection.pose08.angles10.words[3],input+36,4);memcpy(&world.scratch.matrix28,input+40,32);
 BuildScaledCameraMatrices();assert(calls==1);snapshot(output+72);
 assert(!memcmp(&world.scratch.projection.pose08.angles10.words[3],input+36,4));
 for(i=0;i<16;i++)assert(world.before[i]==0xA5&&world.after[i]==0xA5);
 for(i=0;i<sizeof(world.pixels);i++)assert(((u8*)world.pixels)[i]==0xA5);
 for(i=0;i<sizeof(world.order);i++)assert(((u8*)world.order)[i]==0xA5);
 for(i=offsetof(ProjectionScratchPrefix,pose08.angles10)+sizeof(VectorWords16);i<sizeof(ProjectionScratchPrefix);i++)assert(((u8*)&world.scratch.projection)[i]==0xA5);
 for(i=offsetof(ViewMatrixScratch48,matrix28)+sizeof(MatrixWords32);i<sizeof(ViewMatrixScratch48);i++)assert(((u8*)&world.scratch)[i]==0xA5);
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
        native.run_case.argtypes = [C.c_uint, C.c_void_p, C.c_void_p]
        memory = Uc(UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN | UC_MODE_MIPS32)
        memory.mem_map(0, 0x400000);memory.mem_map(0x1F7FF000, 0x3000)
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
            assert (SCRATCH+46 <= address and address+size <= SCRATCH+52 and size == 2
                    or (STACK & 0x1FFFFFFF)-32 <= address and address+size <= (STACK & 0x1FFFFFFF))

        cpu = Cpu(memory, 0, 100, 101, 102, store);rng = random.Random(0x1B640)
        for case in range(CASES):
            initial = bytearray(rng.randbytes(72))
            struct.pack_into('<2I', initial, 0, PIXELS+(case&1)*16, ORDER+((case>>1)&1)*16)
            output = (C.c_ubyte*144)();native.run_case(case, C.create_string_buffer(bytes(initial)), output)
            expected = bytes(output);write(SCRATCH-16, b'\xA5'*16+initial+b'\xA5'*16)
            cpu.reset();cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
            cpu.r[29] = STACK;cpu.r[31] = STOP;cpu.pc = address_of(NAME);calls = 0;trace = b''
            service_state = bytearray(initial)
            struct.pack_into('<3I', service_state, 0, PIXELS+((case>>1)&1)*16, ORDER+((case>>2)&1)*16, case^0x17171717)
            struct.pack_into('<16H', service_state, 40, *((case+i*0x1111)&65535 for i in range(16)))
            for instruction in range(200):
                if cpu.pc == STOP:break
                if cpu.pc == address_of('BuildCameraMatrices'):
                    assert calls == 0;calls += 1;trace = read(SCRATCH, 72)
                    write(SCRATCH, service_state);cpu.pc = cpu.r[31]
                else:cpu.step()
            assert cpu.pc == STOP and calls == 1, (case, hex(cpu.pc))
            actual = read(SCRATCH, 72)
            assert trace+actual == expected, (case, 'native/MIPS state and service entry')
            independent = bytearray(service_state)
            # Byte offsets are the console's second rotation row, not host offsets.
            for offset in (46, 48, 50):
                value = int.from_bytes(service_state[offset:offset+2], 'little')
                independent[offset:offset+2] = ((value*2)&65535).to_bytes(2, 'little')
            assert actual == independent, (case, 'Y row and untouched state')
            assert cpu.r[29] == STACK and cpu.r[31] == STOP
            for reg in (*range(16, 24), 28, 30):assert cpu.r[reg] == 0xBEEF0000+reg
            assert read(SCRATCH-16, 16) == b'\xA5'*16 and read(SCRATCH+72, 16) == b'\xA5'*16
        print('PASS', CASES, 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
              'MIPS/native intro camera cases; exhaustive Y-row halfwords, current C, real native scratch/pointers, callback entry, complete state, padding, guards, ABI and UBSan')


if __name__ == '__main__':
    main()
