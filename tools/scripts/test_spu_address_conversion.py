"""Native SPU address conversion against independent unsigned arithmetic."""
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest


class SpuAddressConversionTest(unittest.TestCase):
    def test_rounding_selectors_and_complete_register_owner(self):
        root = Path(__file__).resolve().parents[2]
        body = (root / 'tools/scripts/fixtures/views/func_8007F414.c').read_text()
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
s32 _spu_mem_mode, _spu_mem_mode_plus;
u32 _spu_mem_mode_unit, _spu_mem_mode_unitM;
SpuRegisterBank *_spu_RXX;
static struct {
    u32 before[8];
    SpuRegisterStorage bank;
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
        _spu_mem_mode_plus = (k >> 1) & 31;
        _spu_mem_mode = (k & 1) ? -1 : 0;
        _spu_mem_mode_unit = (k & 4) ? (1U << (k % 32)) : (k % 65535) + 1;
        _spu_mem_mode_unitM = (k & 8) ? _spu_mem_mode_unit - 1 : ~seed;
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
        _spu_RXX = &world.bank.registers;
        assert((uintptr_t)_spu_RXX > UINT32_MAX);
        rounded = address;
        if (_spu_mem_mode && (uint64_t)address % _spu_mem_mode_unit) {
            uint64_t sum = (uint64_t)address + _spu_mem_mode_unit;
            rounded = (u32)sum & (_spu_mem_mode_unitM ^ UINT32_MAX);
        }
        units = (u32)((uint64_t)rounded / (UINT64_C(1) << _spu_mem_mode_plus));
        result = index == -1 ? (u16)units : rounded;
        if (index >= 0) expected[index] = (u16)units;
        assert((u32)_spu_FsetRXXa(index, (s32)address) == result);
        for (j = 0; j < 256; ++j) assert(world.bank.halves[j] == expected[j]);
        for (j = 0; j < 8; ++j) {
            assert(world.before[j] == (seed ^ j));
            assert(world.after[j] == ~(seed ^ j));
        }
        assert(_spu_RXX == &world.bank.registers);
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
