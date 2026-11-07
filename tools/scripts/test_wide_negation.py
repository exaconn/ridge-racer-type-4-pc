"""Actual two-word negation through the real shared addition helper."""
import ctypes as C
import random
import unittest
from test_wide_and_counter_mode import ArithmeticFixture,Wide,Box

MASK=(1<<64)-1


class WideNegationTest(ArithmeticFixture,unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        super().setUpClass()
        cls.negate=cls.lib._mainasu;cls.observed=cls.lib.test_neg_observed
        for f in (cls.negate,cls.observed):f.argtypes=[C.POINTER(Wide),Wide];f.restype=C.c_void_p

    def check(self,bits,f=None):
        if f is None:f=self.negate
        box=Box();C.memset(C.addressof(box),0xA5,C.sizeof(box))
        source=Wide.from_bits(bits);before=bytes(source)
        self.assertEqual(f(C.byref(box.value),source),C.addressof(box)+Box.value.offset)
        self.assertEqual(box.value.bits(),(-bits)&MASK)
        self.assertEqual(bytes(source),before)
        self.assertEqual(list(box.before),[0xA5A5A5A5]*2);self.assertEqual(list(box.after),[0xA5A5A5A5]*2)
        return box.value.bits()

    def test_zero_signed_extrema_and_all_power_of_two_neighbors(self):
        values=[0,1,MASK,1<<63,(1<<63)-1,0xFFFFFFFF,0x100000000,0xFFFFFFFF00000000]
        values += [(value+delta)&MASK for bit in range(64) for value in (1<<bit,MASK^(1<<bit)) for delta in (-1,0,1)]
        for bits in values:self.check(bits)

    def test_every_low_halfword_across_carry_and_high_sign_boundaries(self):
        for high in (0,1,0x7FFFFFFF,0x80000000,0xFFFFFFFF):
            for low in range(65536):self.check((high<<32)|low)

    def test_random_double_negation_and_in_place_output(self):
        rng=random.Random(0x7E494)
        for _ in range(5000):
            bits=rng.getrandbits(64);negated=self.check(bits)
            self.assertEqual(self.check(negated),bits)
            box=Box();C.memset(C.addressof(box),0x5A,C.sizeof(box));box.value=Wide.from_bits(bits)
            self.assertEqual(self.negate(C.byref(box.value),box.value),C.addressof(box)+Box.value.offset)
            self.assertEqual(box.value.bits(),(-bits)&MASK)
            self.assertEqual(list(box.before),[0x5A5A5A5A]*2);self.assertEqual(list(box.after),[0x5A5A5A5A]*2)

    def test_exact_addition_operands_and_private_result_storage(self):
        calls=C.c_int32.in_dll(self.lib,'test_neg_calls')
        left=Wide.in_dll(self.lib,'test_neg_left');right=Wide.in_dll(self.lib,'test_neg_right')
        scratch=C.c_void_p.in_dll(self.lib,'test_neg_output')
        for bits in (0,1,MASK,1<<63,0x80000000FFFFFFFF,0x123456789ABCDEF0):
            calls.value=0
            box=Box();box.value=Wide.from_bits(0xA5A5A5A5A5A5A5A5)
            out=C.addressof(box)+Box.value.offset
            self.assertEqual(self.observed(C.byref(box.value),Wide.from_bits(bits)),out)
            self.assertEqual(box.value.bits(),(-bits)&MASK)
            self.assertEqual((calls.value,left.bits(),right.bits()),(1,bits^MASK,1))
            self.assertTrue(scratch.value);self.assertNotEqual(scratch.value,out)
            if C.sizeof(C.c_void_p)>4:
                self.assertGreater(out,0xFFFFFFFF);self.assertGreater(scratch.value,0xFFFFFFFF)
