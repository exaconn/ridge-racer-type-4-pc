"""Actual software addition, shared helper interfaces and original PAL semantics.

The integer oracle uses mathematical alignment, bit lengths and rounding.
It deliberately preserves the executable's encoding behavior for subnormals,
NaNs and negative exponents rather than substituting host floating arithmetic.
"""
import ctypes as C
from collections import Counter
from pathlib import Path
import random
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32
from c_functions import address_of, definition_source, source_owners
from test_cd_disc_probe import native_body
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
MASK = (1 << 64) - 1
FRACTION = (1 << 52) - 1
STACK, STOP = 0x803F0000, 0x803FF000


def oracle(left, right):
    flags = []
    if left & (MASK >> 1) == 0:
        return right, (0, 0, 0), ['left_zero']
    if right & (MASK >> 1) == 0:
        return left, (0, 0, 0), ['right_zero']
    a, b = (left >> 52) & 2047, (right >> 52) & 2047
    if a - b > 54:
        return left, (0, 0, 0), ['left_gap']
    if b - a > 54:
        return right, (0, 0, 0), ['right_gap']
    exponent = max(a, b)
    terms = []
    for bits, power in [(left, a), (right, b)]:
        term = ((bits & FRACTION) | (1 << 52)) * 512
        if bits >> 63:
            term = -term
        terms.append(term >> (exponent - power))
    total = sum(terms)
    if total == 0:
        return 0, (0, 0, 0), ['cancel']
    sign = (1 << 63) if total < 0 else 0
    magnitude = abs(total)
    scale = 62 - magnitude.bit_length()
    magnitude = magnitude << scale if scale >= 0 else magnitude >> -scale
    exponent -= scale
    if scale > 0:
        flags.append('normalize_left')
    elif scale < 0:
        flags.append('normalize_right')
    magnitude += 256 if (magnitude // 512) & 1 else 255
    if magnitude >= 1 << 62:
        magnitude //= 2
        exponent += 1
        flags.append('round_carry')
    if exponent >= 2047:
        return sign | 0x7FF0000000000000, (1, 34, 11), flags + ['overflow']
    if exponent < 0:
        flags.append('negative_exponent')
    fraction = (magnitude // 512) & FRACTION
    return (sign | (exponent << 52) | fraction) & MASK, (0, 0, 0), flags


class SoftfloatAdditionTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temporary = TemporaryDirectory()
        cls.addClassCleanup(cls.temporary.cleanup)
        owners = source_owners(ROOT / 'src/main/PAL')
        helpers = '\n'.join(definition_source(owners[name].read_text(), name)
                            for name in ['_add_mant_d', '_mainasu'])
        helpers += '\n' + owners['_dbl_shift'].read_text()
        addition = owners['__adddf3'].read_text()
        body = native_body(helpers + '\n' + addition)
        body = re.sub(r'\s+(?:__asm__|asm)\("\$[0-9]+"\)', '', body)
        source = r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "rr/softfloat.h"
#include "psyq/other/dpadd.h"
static u32 errors,category,operation;
s32 _err_math(s32 a,s32 b) {
    errors++;category=(u32)a;operation=(u32)b;return 0x13579BDF;
}
''' + body + r'''
u64 run_case(u64 a,u64 b,u32 *trace) {
    struct {u64 before[2],left,right,after[2];} input,saved;
    union {u64 bits;s64 argument;} left,right,result;
    memset(&input,0xA5,sizeof(input));input.left=a;input.right=b;saved=input;
    assert((uintptr_t)&input>UINT32_MAX);
    left.bits=input.left;right.bits=input.right;
    errors=category=operation=0;
    result.argument=__adddf3(left.argument,right.argument);
    assert(memcmp(&input,&saved,sizeof(input))==0);
    trace[0]=errors;trace[1]=category;trace[2]=operation;return result.bits;
}
'''
        directory = Path(cls.temporary.name)
        c, library = directory / 'addition.c', directory / 'addition.dylib'
        c.write_text(source)
        result = subprocess.run([
            'cc', '-std=gnu11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
            '-Wno-unused-parameter', '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
            '-I', str(ROOT / 'include'), str(c), '-o', str(library),
        ], capture_output=True, text=True)
        if result.returncode:
            raise AssertionError(result.stderr)
        cls.library = C.CDLL(str(library))
        cls.add = cls.library.run_case
        cls.add.argtypes = [C.c_uint64, C.c_uint64, C.POINTER(C.c_uint32)]
        cls.add.restype = C.c_uint64

    def native(self, left, right):
        trace = (C.c_uint32 * 3)()
        return self.add(left, right, trace), tuple(trace)

    def test_all_exponents_signs_alignment_rounding_and_cancellation(self):
        coverage = Counter()
        fractions = [0, 1, 0xFF, 0x100, 0x1FF, 0x200, (1 << 51) - 1, FRACTION]
        cases = 0

        def check(left, right):
            nonlocal cases
            expected, trace, flags = oracle(left, right)
            actual = self.native(left, right)
            if actual != (expected, trace):
                self.fail(f'{left:016X}+{right:016X}: {actual!r} != {(expected, trace)!r}')
            coverage.update(flags)
            cases += 1

        for exponent in range(2048):
            for signs in range(4):
                for delta in [-55, -54, -1, 0, 1, 54, 55]:
                    other = min(2047, max(0, exponent + delta))
                    for index, fraction in enumerate(fractions):
                        left = (signs & 1) << 63 | exponent << 52 | fraction
                        right = (signs >> 1) << 63 | other << 52 | fractions[(index * 5 + 3) % 8]
                        check(left, right)
        rng = random.Random(0x7E494)
        for _ in range(131072):
            check(rng.getrandbits(64), rng.getrandbits(64))
        for exponent in [0, 1, 2, 1022, 1023, 2046, 2047]:
            for fraction in fractions:
                bits = exponent << 52 | fraction
                for sign in [0, 1 << 63]:
                    check(bits | sign, bits | (sign ^ (1 << 63)))
                    check(bits | sign, (bits + 1) | (sign ^ (1 << 63)))
                    check(bits | sign, bits | sign)
        required = {'left_zero', 'right_zero', 'left_gap', 'right_gap', 'cancel',
                    'normalize_left', 'normalize_right', 'round_carry', 'overflow', 'negative_exponent'}
        self.assertTrue(all(coverage[key] for key in required), coverage)
        self.assertEqual(cases, 590160)
        print(f'{cases} native addition cases under UBSan; all exponent/sign classes and error boundaries passed')

    def test_original_mips_helpers_results_error_calls_and_abi(self):
        memory = Uc(UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN | UC_MODE_MIPS32)
        memory.mem_map(0, 0x400000)
        read = lambda address, count: bytes(memory.mem_read(address & 0x1FFFFFFF, count))
        write = lambda address, data: memory.mem_write(address & 0x1FFFFFFF, bytes(data))
        image = (ROOT / 'assets/PAL/main.exe').read_bytes()
        base, = struct.unpack_from('<I', image, 24)
        write(base, image[0x800:])
        functions = {name: (int(address, 16), int(size, 16))
                     for name, address, size in map(str.split, (ROOT / 'configs/PAL/functions.txt').read_text().splitlines())}
        spans = [functions[name] for name in ['__adddf3', '_dbl_shift', '_add_mant_d', '_mainasu']]

        def store(cpu, access, address, size, value, user):
            self.assertTrue(STACK - 2048 <= address and address + size <= STACK + 16,
                            (hex(cpu.pc), hex(address), size))

        cpu = Cpu(memory, 0, 100, 101, 102, store)
        edges = [0, 1, 0x000FFFFFFFFFFFFF, 0x0010000000000000,
                 0x0010000000000001, 0x3FF0000000000000, 0x3FF0000000000001,
                 0x3FEFFFFFFFFFFFFF, 0x7FEFFFFFFFFFFFFF, 0x7FF0000000000000,
                 0x7FFFFFFFFFFFFFFF]
        pairs = [(a | sign, b | other) for a, b in zip(edges, reversed(edges))
                 for sign in [0, 1 << 63] for other in [0, 1 << 63]]
        pairs += [(a, a ^ (1 << 63)) for a in edges]
        pairs += [(a, (a + 1) ^ (1 << 63)) for a in edges]
        rng = random.Random(0x7E494)
        for _ in range(128):
            exponent = rng.randrange(2048)
            other = min(2047, max(0, exponent + rng.choice([-55, -54, -1, 0, 1, 54, 55])))
            pairs.append((rng.getrandbits(1) << 63 | exponent << 52 | rng.getrandbits(52),
                          rng.getrandbits(1) << 63 | other << 52 | rng.getrandbits(52)))
        for left, right in pairs:
            write(STACK - 2112, b'\xA5' * 2256)
            cpu.reset()
            cpu.r = [0] + [0xBEEF0000 + index for index in range(1, 32)]
            cpu.r[4:8] = [left & 0xFFFFFFFF, left >> 32, right & 0xFFFFFFFF, right >> 32]
            cpu.r[29], cpu.r[31], cpu.pc = STACK, STOP, address_of('__adddf3')
            saved = {index: cpu.r[index] for index in [*range(16, 24), 28, 30]}
            errors = []
            for _ in range(20000):
                if cpu.pc == STOP:
                    break
                if cpu.pc == address_of('_err_math'):
                    self.assertIsNone(cpu.delay)
                    errors.append(tuple(cpu.r[4:6]))
                    resume = cpu.r[31]
                    for index in [*range(1, 16), 24, 25]:
                        cpu.r[index] = 0xA1230000 + index
                    cpu.r[2], cpu.hi, cpu.lo = 0x13579BDF, 0xABCD, 0xDCBA
                    cpu.pc = resume
                    continue
                self.assertTrue(any(start <= cpu.pc < start + size for start, size in spans), hex(cpu.pc))
                cpu.step()
            else:
                self.fail('Original software addition did not return')
            result = cpu.r[2] | cpu.r[3] << 32
            trace = (len(errors), *errors[0]) if errors else (0, 0, 0)
            expected, error, _ = oracle(left, right)
            self.assertEqual((result, trace), (expected, error), (hex(left), hex(right)))
            self.assertEqual(self.native(left, right), (result, trace))
            self.assertEqual(cpu.r[29], STACK)
            self.assertEqual({index: cpu.r[index] for index in saved}, saved)
            self.assertEqual(read(STACK - 2112, 64), b'\xA5' * 64)
            self.assertEqual(read(STACK + 16, 128), b'\xA5' * 128)
        print(f'{len(pairs)} original-MIPS addition flows; actual helpers, native results, error boundaries and ABI guards passed')
