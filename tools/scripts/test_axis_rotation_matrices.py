"""Actual axis constructors and trig helpers on native owners and both MIPS images.

The independent oracle uses the ROM quarter-wave table and mathematical axis
layouts. All phases, signed-word boundaries and high-half patterns are covered;
the matrix's alignment halfword, translation and exterior guards must survive.
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
BASE, STACK, STOP = 0x80200010, 0x803F0000, 0x803FF000
NAMES = ('BuildRotMatrixX', 'BuildRotMatrixY', 'BuildRotMatrixZ')
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "game/math/rotmat.h"
#include "rr/trig.h"
static s16 table[4097];
#define D_800A50C0 table
#define D_800A58C0 (table+0x400)
#define D_800A60C0 (table+0x800)
#define SinTable (table+0xC00)
static struct {u8 before[16];MatrixWords32 matrix;u8 after[16];} owner;
void set_table(const void *input){memcpy(table,input,sizeof(table));}
'''
TAIL = r'''
void run_axis(unsigned axis,s32 angle,const void *input,void *output){
 assert((uintptr_t)&owner>UINT32_MAX);
 memset(&owner,0xA7,sizeof(owner));memcpy(&owner.matrix,input,32);
 switch(axis){
 case 0:BuildRotMatrixX(&owner.matrix,angle);break;
 case 1:BuildRotMatrixY(&owner.matrix,angle);break;
 case 2:BuildRotMatrixZ(&owner.matrix,angle);break;
 default:assert(!"invalid axis");
 }
 memcpy(output,&owner,sizeof(owner));
}
'''


class AxisRotationMatricesTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temporary = TemporaryDirectory()
        cls.addClassCleanup(cls.temporary.cleanup)
        directory = Path(cls.temporary.name)
        owners = source_owners(ROOT / 'src/main/PAL')
        source = PREFIX
        for name in ('sin_1', 'rsin', 'rcos', *NAMES):
            source += definition_source(owners[name].read_text(), name)
        source += TAIL
        path, binary = directory / 'axis.c', directory / 'axis.so'
        path.write_text(source)
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC',
                        '-Wall', '-Wextra', '-Werror', '-fsanitize=undefined,bounds',
                        '-fno-sanitize-recover=all', '-I', str(ROOT / 'include'),
                        str(path), '-o', str(binary)], check=True, capture_output=True)
        cls.native = C.CDLL(str(binary))
        cls.native.set_table.argtypes = [C.c_void_p]
        cls.native.run_axis.argtypes = [C.c_uint, C.c_int32, C.c_void_p, C.c_void_p]
        cls.native.run_axis.restype = None
        executable = (ROOT / 'assets/PAL/main.exe').read_bytes()
        cls.original = (struct.unpack_from('<I', executable, 0x18)[0], executable[0x800:])
        at = 0x800 + address_of('D_800A50C0') - cls.original[0]
        cls.table = executable[at:at + 4097 * 2]
        assert len(cls.table) == 4097 * 2
        cls.quarter = struct.unpack_from('<1025h', cls.table, 0xC00 * 2)
        cls.native.set_table(C.create_string_buffer(cls.table))
        with (ROOT / 'build/PAL/main.elf').open('rb') as stream:
            elf = ELFFile(stream)
            cls.linked = [(s['sh_addr'], s.data()) for s in elf.iter_sections()
                          if s['sh_flags'] & 2 and s['sh_type'] == 'SHT_PROGBITS']

    def sine(self, angle):
        quadrant, index = divmod(angle & 4095, 1024)
        return (self.quarter[index], self.quarter[1024 - index],
                -self.quarter[index], -self.quarter[1024 - index])[quadrant]

    def expected(self, axis, angle, initial):
        sine, cosine = self.sine(angle), self.sine(angle + 1024)
        rotations = ((4096, 0, 0, 0, cosine, -sine, 0, sine, cosine),
                     (cosine, 0, -sine, 0, 4096, 0, sine, 0, cosine),
                     (cosine, -sine, 0, sine, cosine, 0, 0, 0, 4096))
        return b'\xA7' * 16 + struct.pack('<9h', *rotations[axis]) + initial[18:] + b'\xA7' * 16

    def test_actual_constructors_all_phases_and_signed_word_selectors(self):
        randomizer = random.Random(0x41584953)
        angles = list(range(4096))
        angles += [-2147483648, -2147483647, -65537, -65536, -32769,
                   -32768, -4097, -4096, -1, 32767, 32768, 65535,
                   65536, 2147483646, 2147483647]
        angles += [C.c_int32(high | low).value
                   for high in (0x12340000, 0x80000000, 0xFFFF0000)
                   for low in (0, 1, 1023, 1024, 1025, 2047, 2048, 3072, 4095)]
        angles += [randomizer.randint(-2147483648, 2147483647) for _ in range(128)]
        inputs = [randomizer.randbytes(32) for _ in angles]
        for version in ('original', 'linked'):
            memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
            memory.mem_map(0, 0x400000)
            write = lambda address, data: memory.mem_write(address & 0x1FFFFFFF, bytes(data))
            read = lambda address, size: bytes(memory.mem_read(address & 0x1FFFFFFF, size))
            write(*self.original)
            if version == 'linked':
                for address, data in self.linked:
                    write(address, data)

            def store(cpu, access, address, size, value, user):
                address &= 0x1FFFFFFF
                self.assertTrue((BASE & 0x1FFFFFFF) + 16 <= address and
                                address + size <= (BASE & 0x1FFFFFFF) + 48 or
                                (STACK & 0x1FFFFFFF) - 1024 <= address and
                                address + size <= (STACK & 0x1FFFFFFF),
                                (hex(address), size))

            cpu = Cpu(memory, 0, 100, 101, 102, store)
            for axis, name in enumerate(NAMES):
                for angle, initial in zip(angles, inputs):
                    expected = self.expected(axis, angle, initial)
                    native = C.create_string_buffer(64)
                    self.native.run_axis(axis, angle, C.create_string_buffer(initial), native)
                    self.assertEqual(native.raw, expected, (name, angle, 'native'))
                    write(BASE, b'\xA7' * 16 + initial + b'\xA7' * 16)
                    cpu.reset()
                    cpu.r = [0] + [0xBEEF0000 + i for i in range(1, 32)]
                    cpu.r[4], cpu.r[5] = BASE + 16, angle & 0xFFFFFFFF
                    cpu.r[29], cpu.r[31], cpu.pc = STACK, STOP, address_of(name)
                    for _ in range(1000):
                        if cpu.pc == STOP:
                            break
                        cpu.step()
                    self.assertEqual(cpu.pc, STOP, (version, name, angle))
                    for reg in (*range(16, 24), 28, 29, 30):
                        self.assertEqual(cpu.r[reg], STACK if reg == 29 else 0xBEEF0000 + reg)
                    self.assertEqual(read(BASE, 64), expected, (version, name, angle, 'MIPS'))
            self.assertEqual(read(address_of('D_800A50C0'), len(self.table)), self.table)


if __name__ == '__main__':
    unittest.main()
