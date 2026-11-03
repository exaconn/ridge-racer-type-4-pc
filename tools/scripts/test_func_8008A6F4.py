"""Host specification checks for marking a SPU allocation free.

Only the target register annotations and layout assertions are omitted on the
host. fd.py verifies those separately against the original MIPS instructions.
"""
import ctypes as C
from pathlib import Path
import random
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest


class Allocation(C.Structure):
    _fields_ = [('address_flags', C.c_uint32), ('size', C.c_uint32)]


Consolidate = C.CFUNCTYPE(None)


class FreeAllocationTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        directory = Path(cls.temp.name)
        root = Path(__file__).resolve().parents[2]
        header = (root / 'include/rr/spu.h').read_text()
        header = re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n', '', header, flags=re.M)
        source = (root / 'build/legacy/func_8008A6F4.c').read_text()
        source = source.replace('#include "rr/spu.h"', '''#include <stdint.h>
typedef uint8_t u8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
''' + header)
        source = re.sub(r'\s+__asm__\("\$[0-9]+"\)', '', source)
        source += '''
s32 D_800A48B8;
RrSpuAllocation *D_800A48C0;
void (*test_consolidate)(void);
void func_8008A774(void) { test_consolidate(); }
'''
        (directory / 'function.c').write_text(source)
        subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory / 'function.c'),
                        '-o', str(directory / 'function.so')], check=True, capture_output=True)
        cls.library = C.CDLL(str(directory / 'function.so'))
        cls.function = cls.library.func_8008A6F4
        cls.function.argtypes = [C.c_uint32]
        cls.function.restype = None

    def check_free(self, entries, address, limit=None):
        if limit is None:
            limit = len(entries)
        table = (Allocation * len(entries))(*(Allocation(*entry) for entry in entries))
        C.c_int32.in_dll(self.library, 'D_800A48B8').value = limit
        C.c_void_p.in_dll(self.library, 'D_800A48C0').value = (
            C.addressof(table) if limit > 0 else None)
        expected = list(entries)
        prefix = [value for value, _ in entries[:max(0, limit)]]
        sentinel = next((i for i, value in enumerate(prefix) if value & 0x40000000), len(prefix))
        candidates = [i for i, value in enumerate(prefix[:sentinel]) if value == address]
        if candidates:
            index = candidates[0]
            expected[index] = (address | 0x80000000, entries[index][1])
        snapshots = []

        @Consolidate
        def consolidate():
            snapshots.append([(entry.address_flags, entry.size) for entry in table])

        C.c_void_p.in_dll(self.library, 'test_consolidate').value = C.cast(consolidate, C.c_void_p).value
        self.function(address)
        self.assertEqual([(entry.address_flags, entry.size) for entry in table], expected)
        self.assertEqual(snapshots, [expected], 'consolidation must run once, after the update')

    def test_empty_or_disabled_table_does_not_dereference_null(self):
        for count in (0, -1, -(1 << 31)):
            self.check_free([], 0x1000, count)

    def test_first_match_count_limit_and_end_marker(self):
        entries = [(0x1000, 7), (0x2000, 8), (0x2000, 9), (0x40000000, 0), (0x3000, 10)]
        for address in (0, 0x1000, 0x2000, 0x3000, 0x40000000, 0xFFFFFFFF):
            self.check_free(entries, address)
        self.check_free(entries, 0x2000, 1)
        self.check_free(entries, 0x2000, 2)
        self.check_free([(0xC0001000, 1), (0x2000, 2)], 0x2000)
        self.check_free([(0x80001000, 1), (0x2000, 2)], 0x80001000)

    def test_deterministic_allocation_tables(self):
        rng = random.Random(0x8A6F4)
        for _ in range(1000):
            entries = [(rng.getrandbits(32) & ~0x40000000, rng.getrandbits(32))
                       for _ in range(rng.randrange(1, 33))]
            if rng.randrange(2):
                index = rng.randrange(len(entries))
                entries[index] = (entries[index][0] | 0x40000000, entries[index][1])
            address = rng.choice(entries)[0] if rng.randrange(2) else rng.getrandbits(32)
            self.check_free(entries, address, rng.randrange(len(entries) + 1))


if __name__ == '__main__':
    unittest.main()
