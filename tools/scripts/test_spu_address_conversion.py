"""Native SPU address conversion against independent unsigned arithmetic."""
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest


class SpuAddressConversionTest(unittest.TestCase):
    def test_rounding_selectors_and_complete_register_owner(self):
        root = Path(__file__).resolve().parents[2]
        body = (root / 'src/main/PAL/func_8007F414.c').read_text()
        # Target register-allocation annotations have no host equivalent.
        body = re.sub(r' __asm__\("\$[23]"\)', '', body)
        source = r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include "rr/layout.h"
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_OFFSET(t,f,n) _Static_assert(offsetof(t,f)==(n), "offset")
#include "rr/spu.h"
s32 D_800A47FC, D_800A4800;
u32 D_800A4804, D_800A4808;
RrSpuRegisterBank *D_800A47D8;
static struct {
    u32 before[8];
    RrSpuRegisterStorage bank;
    u32 after[8];
} world;
''' + body + r'''
int main(void) {
    u32 seed = 1, k, j;
    for (k = 0; k < 131072; ++k) {
        u16 expected[256];
        u32 address, rounded, units, result;
        s32 index;
        seed = seed * 1664525U + 1013904223U;
        address = seed;
        /* Exercise every shift, both query selectors, all256 write indices,
         * unrounded and wrapped rounded addresses, and independent masks. */
        D_800A4800 = (k >> 1) & 31;
        D_800A47FC = (k & 1) ? -1 : 0;
        D_800A4804 = (k & 4) ? (1U << (k % 32)) : (k % 65535) + 1;
        D_800A4808 = (k & 8) ? D_800A4804 - 1 : ~seed;
        index = (s32)(k % 258) - 2;
        if ((k & 127) == 0) address = 0;
        if ((k & 127) == 1) address = UINT32_MAX;
        if ((k & 127) == 2) address = 0x80000000U;
        for (j = 0; j < 8; ++j) {
            world.before[j] = seed ^ j;
            world.after[j] = ~(seed ^ j);
        }
        for (j = 0; j < 256; ++j)
            expected[j] = world.bank.halves[j] = (u16)(seed + j * 37U);
        D_800A47D8 = &world.bank.registers;
        assert((uintptr_t)D_800A47D8 > UINT32_MAX);
        rounded = address;
        if (D_800A47FC && (uint64_t)address % D_800A4804) {
            uint64_t sum = (uint64_t)address + D_800A4804;
            rounded = (u32)sum & (D_800A4808 ^ UINT32_MAX);
        }
        units = (u32)((uint64_t)rounded / (UINT64_C(1) << D_800A4800));
        result = index == -1 ? (u16)units : rounded;
        if (index >= 0) expected[index] = (u16)units;
        assert((u32)func_8007F414(index, (s32)address) == result);
        for (j = 0; j < 256; ++j) assert(world.bank.halves[j] == expected[j]);
        for (j = 0; j < 8; ++j) {
            assert(world.before[j] == (seed ^ j));
            assert(world.after[j] == ~(seed ^ j));
        }
        assert(D_800A47D8 == &world.bank.registers);
    }
    return 0;
}
'''
        with TemporaryDirectory() as directory:
            path = Path(directory)
            (path / 'test.c').write_text(source)
            subprocess.run(['cc', '-std=gnu11', '-O2', '-fsanitize=undefined',
                            '-fno-sanitize-recover=all', '-I', str(root / 'include'),
                            str(path / 'test.c'), '-o', str(path / 'test')], check=True)
            subprocess.run([str(path / 'test')], check=True)


if __name__ == '__main__':
    unittest.main()
