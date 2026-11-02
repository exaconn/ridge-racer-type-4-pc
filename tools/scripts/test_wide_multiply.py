"""Actual split-word multiplication through the recovered addition helper."""
import ctypes as C
import random
import unittest
from test_wide_and_counter_mode import ArithmeticFixture, Box


class WideMultiplyTest(ArithmeticFixture, unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        super().setUpClass()
        cls.multiply = cls.lib.func_8007E818
        cls.multiply.argtypes = [C.c_void_p, C.c_uint32, C.c_uint32]
        cls.multiply.restype = C.c_void_p

    def check(self, left, right):
        box = Box()
        C.memset(C.addressof(box), 0xA5, C.sizeof(box))
        out = C.addressof(box) + Box.value.offset
        self.assertEqual(self.multiply(out, left, right), out)
        self.assertEqual(box.value.bits(), left * right)
        self.assertEqual(list(box.before), [0xA5A5A5A5] * 2)
        self.assertEqual(list(box.after), [0xA5A5A5A5] * 2)

    def test_carries_halfword_boundaries_and_unsigned_extrema(self):
        edges = [0, 1, 0x7FFF, 0x8000, 0xFFFF, 0x10000, 0x10001,
                 0x7FFFFFFF, 0x80000000, 0xFFFF0000, 0xFFFFFFFE, 0xFFFFFFFF]
        edges += [((1 << n) + delta) & 0xFFFFFFFF
                  for n in range(32) for delta in (-1, 0, 1)]
        for left in edges:
            for right in edges:
                self.check(left, right)

    def test_every_halfword_with_maximal_cross_products(self):
        for half in range(65536):
            self.check(0xFFFF0000 | half, 0xFFFFFFFF)
            self.check((half << 16) | 0xFFFF, 0xFFFF0001)

    def test_random_full_width_products(self):
        rng = random.Random(0x7E818)
        for _ in range(20000):
            self.check(rng.getrandbits(32), rng.getrandbits(32))
