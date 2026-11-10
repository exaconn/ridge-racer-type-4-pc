"""Current shared matrix scaler: full native owners and original MIPS.

The native byte oracle exhausts all halfword patterns for16 scale boundaries,
plus random scales. Original MIPS independently checks65,536 complete states,
the three permitted stores, callee-saved registers and the public typed API.
"""
import ctypes as C
import os
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32

from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
NAME = 'ScaleMatrixRow1'
OWNER, STACK, STOP = 0x801C0010, 0x803F0000, 0x803FF000
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "game/car/carbound.h"
typedef struct {u8 before[16];MatrixWords32 matrix;u8 after[16];} State;
static State state;
'''
TAIL = r'''
void run_case(s32 scale,const u8 *input,u8 *output){
 _Static_assert(sizeof(State)==64,"complete matrix owner and guards");
 _Static_assert(offsetof(State,matrix)==16,"matrix offset");
 assert(sizeof(void*)==8&&(uintptr_t)&state.matrix>UINT32_MAX);
 memcpy(&state,input,sizeof(state));
 ScaleMatrixRow1(scale,&state.matrix);
 memcpy(output,&state,sizeof(state));
}
unsigned exercise_native(void){
 static const s32 scales[]={INT32_MIN,INT32_MIN+1,-65537,-65536,-32768,-2,-1,
                           0,1,2,32767,32768,65535,65536,65537,INT32_MAX};
 u32 random=0x725A193BU;unsigned calls=0;
 for(unsigned n=0;n<65536;n++)for(unsigned k=0;k<17;k++){
  u8 input[64],expected[64],output[64];s32 scale;
  for(unsigned j=0;j<64;j++){
   random^=random<<13;random^=random>>17;random^=random<<5;input[j]=(u8)random;
  }
  if(k<16)scale=scales[k];else memcpy(&scale,&random,4);
  for(unsigned j=0;j<3;j++){
   u16 bits=(u16)(n*(2*j+1)+j*0x1717U);memcpy(input+22+2*j,&bits,2);
  }
  memcpy(expected,input,64);
  for(unsigned j=0;j<3;j++){
   u16 bits;memcpy(&bits,input+22+2*j,2);
   bits=(u16)((uint64_t)bits*(uint32_t)scale);memcpy(expected+22+2*j,&bits,2);
  }
  run_case(scale,input,output);assert(!memcmp(output,expected,64));calls++;
 }
 return calls;
}
'''


class MatrixRowScaleTest(unittest.TestCase):
    def test_current_api_full_products_and_original_mips(self):
        source = source_owners(ROOT/'src/main/PAL')[NAME]
        body = definition_source(source.read_text(), NAME)
        with TemporaryDirectory() as temporary:
            directory = Path(temporary)
            native_source, library = directory/'native.c', directory/'native.dylib'
            native_source.write_text(PREFIX+body+TAIL)
            subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC',
                            '-Wall', '-Wextra', '-Werror', '-fsanitize=undefined,bounds',
                            '-fno-sanitize-recover=all', '-I', str(ROOT/'include'),
                            str(native_source), '-o', str(library)], check=True)
            native = C.CDLL(str(library))
            native.run_case.argtypes = [C.c_int32, C.c_void_p, C.c_void_p]
            native.run_case.restype = None
            native.exercise_native.restype = C.c_uint
            self.assertEqual(native.exercise_native(), 65536*17)

            memory = Uc(UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN | UC_MODE_MIPS32)
            memory.mem_map(0, 0x400000)
            write = lambda address, data: memory.mem_write(address & 0x1FFFFFFF, bytes(data))
            read = lambda address, size: bytes(memory.mem_read(address & 0x1FFFFFFF, size))
            original = (ROOT/'assets/PAL/main.exe').read_bytes()
            base = struct.unpack_from('<I', original, 0x18)[0]
            write(base, original[0x800:])
            if os.getenv('RR_COMPILED_ELF'):
                with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                    elf = ELFFile(stream)
                    symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(NAME)[0]
                    section = elf.get_section(symbol['st_shndx'])
                    offset = symbol['st_value']-section['sh_addr']
                    self.assertEqual(symbol['st_value'], address_of(NAME))
                    write(symbol['st_value'], section.data()[offset:offset+symbol['st_size']])
            stores = []

            def store(cpu, access, address, size, value, user):
                address &= 0x1FFFFFFF
                self.assertEqual(size, 2)
                self.assertIn(address, [(OWNER+offset) & 0x1FFFFFFF for offset in (22, 24, 26)])
                stores.append(address)

            cpu = Cpu(memory, 0, 100, 101, 102, store)
            rng = random.Random(0x725A193B)
            boundaries = [-2147483648, -2147483647, -65537, -65536, -32768,
                          -2, -1, 0, 1, 2, 32767, 32768, 65535, 65536, 65537, 2147483647]
            for case in range(65536):
                initial = bytearray(rng.randbytes(64))
                scale = boundaries[case % 17] if case % 17 < 16 else rng.randrange(-2**31, 2**31)
                for index in range(3):
                    struct.pack_into('<H', initial, 22+2*index,
                                     (case*(2*index+1)+index*0x1717) & 65535)
                expected = bytearray(initial)
                for offset in (22, 24, 26):
                    value = struct.unpack_from('<h', initial, offset)[0]
                    struct.pack_into('<H', expected, offset, (value*scale) & 65535)
                output = (C.c_ubyte*64)()
                native.run_case(scale, C.create_string_buffer(bytes(initial)), output)
                self.assertEqual(bytes(output), expected, case)
                write(OWNER, initial)
                cpu.reset();stores.clear()
                cpu.r = [0]+[0xBEEF0000+register for register in range(1, 32)]
                cpu.r[4] = scale & 0xFFFFFFFF
                cpu.r[5] = OWNER+16
                cpu.r[29], cpu.r[31], cpu.pc = STACK, STOP, address_of(NAME)
                for instruction in range(80):
                    if cpu.pc == STOP:
                        break
                    cpu.step()
                self.assertEqual(cpu.pc, STOP, case)
                self.assertEqual(read(OWNER, 64), expected, case)
                self.assertEqual(sorted(stores), [((OWNER+offset) & 0x1FFFFFFF) for offset in (22, 24, 26)])
                self.assertEqual(cpu.r[29], STACK)
                self.assertEqual(cpu.r[31], STOP)
                for register in (*range(16, 24), 28, 30):
                    self.assertEqual(cpu.r[register], 0xBEEF0000+register)
            print('PASS1114112 native and65536',
                  'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
                  'MIPS matrix cases;complete owners, guards, exact stores, ABI and UBSan')


if __name__ == '__main__':
    unittest.main()
