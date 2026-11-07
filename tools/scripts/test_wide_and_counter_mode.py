"""Real two-word addition and mode publication with exact PAL ABI values."""
import ctypes as C
from pathlib import Path
import random
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from c_functions import source_owners


class Wide(C.Structure):
    _fields_ = [('lo', C.c_uint32), ('hi', C.c_int32)]

    @classmethod
    def from_bits(cls, bits):
        return cls(bits & 0xFFFFFFFF, C.c_int32(bits >> 32).value)

    def bits(self):
        return self.lo | ((self.hi & 0xFFFFFFFF) << 32)


class Box(C.Structure):
    _fields_ = [('before', C.c_uint32 * 2), ('value', Wide), ('after', C.c_uint32 * 2)]


Counter = C.CFUNCTYPE(C.c_int32, C.c_uint32, C.c_uint32)


class ArithmeticFixture:
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        root = Path(__file__).resolve().parents[2]
        directory = Path(cls.temp.name)
        header = (root / 'include/rr/wide.h').read_text()
        header = re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n', '', header, flags=re.M)
        source = '''#include <stdint.h>
#include <stddef.h>
typedef uint32_t u32; typedef int32_t s32; typedef uint64_t u64; typedef int64_t s64;
''' + header + '''
u32 test_wide_layout[]={sizeof(RrWide64),offsetof(RrWide64,lo),offsetof(RrWide64,hi)};
RrWide64 test_values[3];
RrWide64 test_neg_left,test_neg_right;
s32 test_neg_calls;void *test_neg_output;
RrWide64 *test_neg_add(RrWide64 *out,RrWide64 left,RrWide64 right) {
 test_neg_calls++;test_neg_left=left;test_neg_right=right;test_neg_output=out;
 return _add_mant_d(out,left,right);
}
RrWide64 *test_alias(s32 index,RrWide64 left,RrWide64 right) {
 test_values[0]=left; test_values[1]=right;
 test_values[2].lo=0xA5A5A5A5U; test_values[2].hi=(s32)0xA5A5A5A5U;
 return _add_mant_d(&test_values[index],test_values[0],test_values[1]);
}
s32 test_page[0x4000];
s32 (*test_counter)(u32,u32);
s32 DeliverEvent(u32 counter,u32 mode) { return test_counter(counter,mode); }
'''
        owners = {'func_'+n: root / 'tools/scripts/fixtures/legacy' / ('func_'+n+'.c') for n in ('8007DB54', '8007DC04', '8007E9C4', '8007DFA4', '8007DFF4', '8007E1B4', '8007E904', '8007E494', '8007E818')}
        sources = dict.fromkeys(owners['func_' + name] for name in
                                ('8007DB54', '8007DC04', '8007E9C4', '8007DFA4', '8007DFF4', '8007E1B4', '8007E904', '8007E494', '8007E818'))
        for path in {q.read_text(): q for q in sources}.values():
            body = path.read_text()
            body = re.sub(r'^#include[^\n]*\n', '', body, flags=re.M)
            body = body.replace('extern s32 D_800A47D0, D_800A47D4;', '')
            body = body.replace('D_800A47D0', 'test_page[0x47D0/4]')
            body = body.replace('(s32 *)0x800A0000U', 'test_page')
            body = re.sub(r'\s+(?:__asm__|asm)\("\$[0-9]+"\)', '', body)
            body = re.sub(r'"\$[0-9]+",\s*', '', body)
            body = re.sub(r':\s*"\$[0-9]+"(?:,\s*"\$[0-9]+")*(?=\))', '', body)
            source += body
            if '_mainasu(' in body:
                observed=body.replace('_mainasu','test_neg_observed')
                assert observed.count('_add_mant_d(')==1
                source+=observed.replace('_add_mant_d(','test_neg_add(')
        (directory / 'function.c').write_text(source)
        result = subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory / 'function.c'),
                                 '-o', str(directory / 'function.so')], capture_output=True)
        if result.returncode:
            raise AssertionError(result.stderr.decode())
        cls.lib = C.CDLL(str(directory / 'function.so'))
        cls.add = cls.lib._add_mant_d
        cls.add.argtypes = [C.POINTER(Wide), Wide, Wide]
        cls.add.restype = C.POINTER(Wide)
        cls.shift = cls.lib._dbl_shift
        cls.shift.argtypes = [C.c_int32, Wide, C.c_int32]
        cls.shift.restype = Wide
        cls.alias = cls.lib.test_alias
        cls.alias.argtypes = [C.c_int32, Wide, Wide]
        cls.alias.restype = C.POINTER(Wide)
        cls.compare = cls.lib._comp_mant
        cls.compare.argtypes = [Wide, Wide]
        cls.compare.restype = C.c_int32
        cls.logical_shift = cls.lib._dbl_shift_us
        cls.logical_shift.argtypes = [C.POINTER(Wide), C.c_int32, Wide, C.c_int32]
        cls.logical_shift.restype = C.POINTER(Wide)
        cls.integer_to_binary64 = getattr(cls.lib, '__floatsidf')
        cls.integer_to_binary64.argtypes = [C.c_int32]
        cls.integer_to_binary64.restype = C.c_uint64
        cls.negate_binary64 = cls.lib.func_8007E904
        cls.negate_binary64.argtypes = [C.c_uint64]
        cls.negate_binary64.restype = C.c_uint64
        cls.values = (Wide * 3).in_dll(cls.lib, 'test_values')
        cls.function = cls.lib._err_math
        cls.function.argtypes = [C.c_int32, C.c_int32]
        cls.function.restype = C.c_int32
        cls.page = (C.c_uint8 * 65536).in_dll(cls.lib, 'test_page')


class WideAdditionTest(ArithmeticFixture, unittest.TestCase):
    def check_sum(self, left_bits, right_bits):
        left, right = Wide.from_bits(left_bits), Wide.from_bits(right_bits)
        before_left, before_right = bytes(left), bytes(right)
        box = Box()
        C.memset(C.addressof(box), 0xA5, C.sizeof(box))
        expected = bytearray(box)
        total = (left_bits + right_bits) & ((1 << 64) - 1)
        struct.pack_into('<Q', expected, Box.value.offset, total)
        result = self.add(C.byref(box.value), left, right)
        self.assertEqual(C.addressof(result.contents), C.addressof(box.value))
        self.assertEqual(bytes(box), bytes(expected))
        self.assertEqual(bytes(left), before_left)
        self.assertEqual(bytes(right), before_right)

    def test_native_and_target_word_layout(self):
        self.assertEqual(list((C.c_uint32 * 3).in_dll(self.lib, 'test_wide_layout')), [8, 0, 4])
        self.assertEqual((C.sizeof(Wide), Wide.lo.offset, Wide.hi.offset), (8, 0, 4))

    def test_carries_and_signed_high_word_boundaries(self):
        edges = [0, 1, 0x7FFF, 0x8000, 0xFFFF, 0x10000, 0x7FFFFFFF, 0x80000000,
                 0xFFFFFFFF, 0x100000000, 0x7FFFFFFF00000000, 0x7FFFFFFFFFFFFFFF,
                 0x8000000000000000, 0xFFFFFFFF00000000, 0xFFFFFFFFFFFFFFFF]
        for left in edges:
            for right in edges:
                self.check_sum(left, right)

    def test_random_full_width_sums_against_independent_integer_arithmetic(self):
        rng = random.Random(0x7DB54)
        for _ in range(10000):
            self.check_sum(rng.getrandbits(64), rng.getrandbits(64))

    def test_shared_word_layout_shift_helper_matches_signed_and_wrapped_shifts(self):
        values = [0, 1, 0xFFFFFFFF, 0x100000000, 0x7FFFFFFFFFFFFFFF,
                  0x8000000000000000, 0xFFFFFFFFFFFFFFFF, 0x123456789ABCDEF0]
        for bits in values:
            for direction in (0, 1, -1, 0x7FFFFFFF):
                for count in (-1, 0, 1, 4, 15, 16, 31, 32, 33, 63, 64, 65, 127):
                    value = Wide.from_bits(bits)
                    initial = bytes(value)
                    if count <= 0:
                        expected = bits
                    elif direction == 0:
                        expected = (bits << count) & ((1 << 64) - 1)
                    else:
                        signed = bits - (1 << 64) if bits & (1 << 63) else bits
                        expected = (signed >> count) & ((1 << 64) - 1)
                    self.assertEqual(self.shift(direction, value, count).bits(), expected)
                    self.assertEqual(bytes(value), initial)

    def test_result_can_alias_either_input_or_a_separate_word_pair(self):
        for left, right in [(0xFFFFFFFF, 1), (0x7FFFFFFFFFFFFFFF, 1),
                            (0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF), (0, 0)]:
            for index in range(3):
                expected = [left, right, 0xA5A5A5A5A5A5A5A5]
                expected[index] = (left + right) & ((1 << 64) - 1)
                result = self.alias(index, Wide.from_bits(left), Wide.from_bits(right))
                self.assertEqual(C.addressof(result.contents), C.addressof(self.values) + index * 8)
                self.assertEqual([value.bits() for value in self.values], expected)


class CounterModeTest(ArithmeticFixture, unittest.TestCase):
    def setUp(self):
        C.memset(C.addressof(self.page), 0xA5, len(self.page))
        self.events = []
        self.callbacks = []
        self.install(lambda counter, mode: self.events.append((counter, mode, bytes(self.page))) or -1)

    def install(self, function):
        callback = Counter(function)
        self.callbacks.append(callback)
        C.c_void_p.in_dll(self.lib, 'test_counter').value = C.cast(callback, C.c_void_p).value

    def test_raw_modes_and_parameters_are_published_before_exact_mode_dispatch(self):
        modes = [-0x80000000, -1, *range(256), 0x12340021, 0x12340022, 0x7FFFFFFF]
        for mode in modes:
            for parameter in (-0x80000000, -1, 0, 1, 0x12345678, 0x7FFFFFFF):
                self.events.clear()
                expected = bytearray(self.page)
                struct.pack_into('<ii', expected, 0x47D0, mode, parameter)
                self.assertEqual(self.function(mode, parameter), 0)
                events = [(0xF4000002, 0x301 if mode == 33 else 0x302, bytes(expected))] if mode in (33, 34) else []
                self.assertEqual(self.events, events)
                self.assertEqual(bytes(self.page), bytes(expected))

    def test_callback_error_and_mutations_are_retained(self):
        for result in (-0x80000000, -1, 0, 1, 0x7FFFFFFF):
            for mode in (33, 34):
                self.events.clear()
                published = bytearray(self.page)
                struct.pack_into('<ii', published, 0x47D0, mode, -77)
                expected = bytearray(published)
                struct.pack_into('<iii', expected, 0x47D0, -1, 123, 456)
                def callback(counter, command):
                    self.events.append((counter, command, bytes(self.page)))
                    self.page[0x47D0:0x47DC] = expected[0x47D0:0x47DC]
                    return result
                self.install(callback)
                self.assertEqual(self.function(mode, -77), 0)
                self.assertEqual(self.events, [(0xF4000002, 0x301 if mode == 33 else 0x302, bytes(published))])
                self.assertEqual(bytes(self.page), bytes(expected))


class WideUnsignedComparisonTest(ArithmeticFixture, unittest.TestCase):
    def check_comparison(self, left_bits, right_bits):
        left, right = Wide.from_bits(left_bits), Wide.from_bits(right_bits)
        before = bytes(left), bytes(right), bytes(self.values), bytes(self.page)
        expected = (left_bits > right_bits) - (left_bits < right_bits)
        self.assertEqual(self.compare(left, right), expected)
        self.assertEqual((bytes(left), bytes(right), bytes(self.values), bytes(self.page)), before)

    def test_word_boundaries_signed_high_bits_and_equal_values(self):
        words = (0, 1, 2, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFE, 0xFFFFFFFF)
        values = [(hi << 32) | lo for hi in words for lo in words]
        for left in values:
            for right in values:
                self.check_comparison(left, right)

    def test_random_full_width_values_and_equal_high_words(self):
        rng = random.Random(0x7DFA4)
        for _ in range(10000):
            self.check_comparison(rng.getrandbits(64), rng.getrandbits(64))
        for _ in range(3000):
            hi = rng.getrandbits(32) << 32
            self.check_comparison(hi | rng.getrandbits(32), hi | rng.getrandbits(32))


class WideLogicalShiftTest(ArithmeticFixture, unittest.TestCase):
    def check_shift(self, bits, direction, count, alias=False):
        value = Wide.from_bits(bits)
        input_before = bytes(value)
        box = Box()
        C.memset(C.addressof(box), 0xA5, C.sizeof(box))
        if alias:
            box.value = value
            value = box.value
        expected = bytearray(box)
        if count <= 0:
            shifted = bits
        elif count >= 64:
            shifted = 0
        elif direction == 0:
            shifted = (bits << count) & ((1 << 64)-1)
        else:
            shifted = bits >> count
        struct.pack_into('<Q', expected, Box.value.offset, shifted)
        result = self.logical_shift(C.byref(box.value), direction, value, count)
        self.assertEqual(C.addressof(result.contents), C.addressof(box.value))
        self.assertEqual(bytes(box), bytes(expected))
        if not alias:
            self.assertEqual(bytes(value), input_before)

    def test_carry_boundaries_nonpositive_counts_and_counts_above_word_width(self):
        values = [0, 1, (1 << 64)-1, 0xAAAAAAAA55555555, 0xFFFFFFFF00000000]
        values += [1 << bit for bit in range(64)]
        for bits in values:
            for direction in (0, 1, -1, -2147483648):
                for count in (-2147483648, -1, 0, 1, 2, 31, 32, 33, 63, 64, 65, 127):
                    self.check_shift(bits, direction, count)
        for direction in (0, 1):
            self.check_shift(0x123456789ABCDEF0, direction, 65536)

    def test_random_values_and_direction_selectors(self):
        rng = random.Random(0x7DFF4)
        for _ in range(3000):
            self.check_shift(rng.getrandbits(64), rng.choice([0, 1, -1, 1234567]), rng.randrange(-3, 130))

    def test_output_can_alias_the_callers_input_value(self):
        for bits in (0, 1, 0x8000000000000001, 0xFEDCBA9876543210, (1 << 64)-1):
            for direction in (0, 1, -1):
                for count in (-1, 0, 1, 31, 32, 33, 63, 64, 65):
                    self.check_shift(bits, direction, count, alias=True)


class IntegerToBinary64Test(ArithmeticFixture, unittest.TestCase):
    def check_conversion(self, value):
        expected = struct.unpack('<Q', struct.pack('<d', float(value)))[0]
        self.assertEqual(self.integer_to_binary64(value), expected, value)

    def test_all_signed_halfword_values(self):
        for value in range(-32768, 32768):
            self.check_conversion(value)

    def test_zero_int_min_and_normalization_boundaries(self):
        values = {-2147483648, -2147483647, 0, 2147483647,
                  0xFFFFFF, 0x1000000, 0x3FFFFFFF, 0x40000000}
        for bit in range(31):
            for offset in (-1, 0, 1):
                value = (1 << bit) + offset
                if value <= 2147483647:
                    values.update((value, -value))
        for value in sorted(values):
            self.check_conversion(value)

    def test_random_signed_words_using_real_shift_callee(self):
        rng = random.Random(0x7E1B4)
        before = bytes(self.values), bytes(self.page)
        for _ in range(10000):
            self.check_conversion(C.c_int32(rng.getrandbits(32)).value)
        self.assertEqual((bytes(self.values), bytes(self.page)), before)


class Binary64SignToggleTest(ArithmeticFixture, unittest.TestCase):
    def check_bits(self, bits):
        result = self.negate_binary64(bits)
        magnitude = bits & ((1 << 63)-1)
        exponent, fraction = magnitude >> 52, magnitude & ((1 << 52)-1)
        self.assertEqual(result & ((1 << 63)-1), magnitude)
        if exponent == 0x7FF and fraction:
            # NaN is an uninterpreted payload here; no floating operation may
            # quiet a signaling NaN or change its payload.
            self.assertEqual(result >> 63, 1-(bits >> 63))
        else:
            value = struct.unpack('<d', struct.pack('<Q', bits))[0]
            expected = struct.unpack('<Q', struct.pack('<d', 0.0 if value == 0 else -value))[0]
            self.assertEqual(result, expected)
        return result

    def test_zero_subnormals_normals_infinity_and_nan_payloads(self):
        magnitudes = [0, 1, 2, (1 << 52)-1, 1 << 52, 0x3FF0000000000000,
                      0x7FEFFFFFFFFFFFFF, 0x7FF0000000000000,
                      0x7FF0000000000001, 0x7FF0000000001234,
                      0x7FF8000000000000, 0x7FFABCDEFEDCBA98, 0x7FFFFFFFFFFFFFFF]
        magnitudes += [1 << bit for bit in range(63)]
        for magnitude in magnitudes:
            for sign in (0, 1 << 63):
                self.check_bits(magnitude | sign)

    def test_random_bit_patterns_preserve_payload_and_match_finite_negation(self):
        rng = random.Random(0x7E904)
        before = bytes(self.values), bytes(self.page)
        for _ in range(10000):
            self.check_bits(rng.getrandbits(64))
        self.assertEqual((bytes(self.values), bytes(self.page)), before)

    def test_two_applications_restore_bits_except_canonicalized_negative_zero(self):
        rng = random.Random(0x7E9042)
        for bits in [0, 1 << 63, 0x7FF0000000000001, 0xFFF8000000000001] + [rng.getrandbits(64) for _ in range(2000)]:
            result = self.negate_binary64(self.negate_binary64(bits))
            self.assertEqual(result, 0 if bits == (1 << 63) else bits)


if __name__ == '__main__':
    unittest.main()
