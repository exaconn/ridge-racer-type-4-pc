"""Actual vehicle response with the canonical race record and exact integer rounding."""
import ctypes as C
from itertools import product
from pathlib import Path
import random
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

from test_race_distance import Distance


def divide(left, right):
    assert right
    quotient = abs(left)//abs(right)
    return -quotient if (left < 0) != (right < 0) else quotient


def expected_response(raw, maximum, span, blend, divisor):
    if raw == 0:
        return 0
    fraction = divide(raw, 8)
    if span < fraction:
        value = 16
    else:
        quotient = divide((maximum-16)*4096, span)
        wrapped = C.c_int32(quotient*fraction).value
        value = ((maximum & 65535)-divide(wrapped, 4096)) & 0xFFFFFFFF
    if blend > 0:
        delta = maximum-C.c_int16(value).value
        value = (value+divide(delta*blend, abs(divisor))) & 0xFFFFFFFF
    return C.c_int16(value).value


class VehicleResponseTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        root = Path(__file__).resolve().parents[2]
        header = (root/'include/rr/race_types.h').read_text() + (root/'include/rr/race.h').read_text()
        header = re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n', '', header, flags=re.M)
        body = (root/'tools/scripts/fixtures/views/func_80047CE0.c').read_text()
        body = re.sub(r'^#include[^\n]*\n', '', body, flags=re.M)
        body = re.sub(r'\s+__asm__\("\$[0-9]+"\)', '', body)
        body = re.sub(r'"\$[0-9]+",\s*', '', body)
        body = re.sub(r':\s*"\$[0-9]+"(?:,\s*"\$[0-9]+")*(?=\))', '', body)
        source = '''#include <stdint.h>
typedef uint8_t u8;typedef uint16_t u16;typedef int16_t s16;
typedef uint32_t u32;typedef int32_t s32;typedef int64_t s64;
''' + header + body
        directory = Path(cls.temp.name)
        (directory/'function.c').write_text(source)
        result = subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory/'function.c'),
                                 '-o', str(directory/'function.so')], capture_output=True)
        if result.returncode:
            raise AssertionError(result.stderr.decode())
        cls.lib = C.CDLL(str(directory/'function.so'))
        cls.response = cls.lib.func_80047CE0
        cls.response.argtypes = [C.POINTER(Distance)]
        cls.response.restype = C.c_int32

    def setUp(self):
        self.records = (Distance*3)()
        C.memset(C.addressof(self.records), 0xA5, C.sizeof(self.records))
        self.vehicle = self.records[1]

    def check(self, raw, maximum, span, blend, divisor):
        self.vehicle.input = raw
        self.vehicle.maximum = maximum
        self.vehicle.span = span
        self.vehicle.blend = blend
        self.vehicle.divisor = divisor
        expected = expected_response(raw, maximum, span, blend, divisor)
        self.assertEqual(self.response(C.byref(self.vehicle)), expected,
                         (raw,maximum,span,blend,divisor))

    def test_canonical_offsets_and_zero_input_bypasses_both_divisions(self):
        for name, offset in (('input',0x1D8),('blend',0x238),('maximum',0x30C),
                             ('span',0x30E),('divisor',0x310)):
            self.assertEqual(getattr(Distance,name).offset, offset)
        self.assertEqual(C.sizeof(Distance),0x320)
        for maximum, blend in product((-32768,-1,0,1,32767), repeat=2):
            self.check(0,maximum,0,blend,0)
        before = bytes(self.records)
        self.assertEqual(self.response(C.byref(self.vehicle)),0)
        self.assertEqual(bytes(self.records), before)

    def test_all_signed_input_values_with_and_without_blend_and_wrapped_products(self):
        configurations = ((160,32767,0,0),(160,32767,100,100),
                          (-32768,1,32767,-32768),(32767,-1,0,0))
        for maximum, span, blend, divisor in configurations:
            for raw in range(-32768,32768):
                self.check(raw,maximum,span,blend,divisor)
        # The complete record and both neighboring records remain read-only.
        before = bytes(self.records)
        self.response(C.byref(self.vehicle))
        self.assertEqual(bytes(self.records), before)

    def test_signed_field_boundaries_rounding_and_interpolation_threshold(self):
        boundaries = (-32768,-32767,-17,-16,-9,-8,-7,-1,0,1,7,8,9,16,17,32766,32767)
        for raw, maximum, span in product(boundaries, repeat=3):
            if raw and divide(raw,8) <= span and span == 0:
                continue  # Invalid record: original target traps in DIV's zero guard.
            for blend, divisor in ((0,0),(-32768,0),(1,-1),(32767,32767),(32767,-32768)):
                self.check(raw,maximum,span,blend,divisor)
        for raw in boundaries:
            fraction = divide(raw,8)
            for span in (fraction-1,fraction,fraction+1):
                if raw and fraction <= span and span == 0:
                    continue
                self.check(raw,-32768,span,32767,1)

    def test_independent_random_fields_and_guarded_bypassed_divisions(self):
        rng = random.Random(0x47CE0)
        for _ in range(12000):
            raw,maximum,span,blend,divisor = (rng.randrange(-32768,32768) for _ in range(5))
            if raw and divide(raw,8) <= span and span == 0:
                continue
            if raw and blend > 0 and divisor == 0:
                continue
            self.check(raw,maximum,span,blend,divisor)
        for raw in (8,9,32767):
            self.check(raw,32767,0,0,0)
        expected = bytearray(b'\xA5'*C.sizeof(self.records))
        offset = C.sizeof(Distance)
        for field in ('input','maximum','span','blend','divisor'):
            start = offset+getattr(Distance,field).offset
            expected[start:start+2] = (getattr(self.vehicle,field)&65535).to_bytes(2,'little')
        self.assertEqual(bytes(self.records), bytes(expected))
