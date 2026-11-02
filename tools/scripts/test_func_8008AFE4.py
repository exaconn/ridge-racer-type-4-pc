"""Host behavioral checks; fd.py checks the target instructions."""
import ctypes as C
from pathlib import Path
import subprocess
from tempfile import TemporaryDirectory
import unittest


Convert = C.CFUNCTYPE(C.c_int32, C.c_int32, C.c_int32)


class AddressConversionTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        directory = Path(cls.temp.name)
        root = Path(__file__).resolve().parents[2]
        source = (root / 'src/main/PAL/func_8008AFE4.c').read_text()
        source = source.replace('#include "types.h"', '''#include <stdint.h>
typedef uint16_t u16;
typedef uint32_t u32;
typedef int32_t s32;''')
        source += '''
volatile u16 D_800A47F0;
s32 D_800A4800;
s32 (*test_convert)(s32, s32);
s32 func_8007F414(s32 mode, s32 value) {
    return test_convert(mode, value);
}
'''
        (directory / 'function.c').write_text(source)
        subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory / 'function.c'),
                        '-o', str(directory / 'function.so')], check=True, capture_output=True)
        cls.library = C.CDLL(str(directory / 'function.so'))
        cls.function = cls.library.func_8008AFE4
        cls.function.argtypes = [C.c_int32]
        cls.function.restype = C.c_uint32

    def check_conversion(self, value, converted, shift=3):
        calls = []
        stored = C.c_uint16.in_dll(self.library, 'D_800A47F0')
        stored.value = 0xA55A
        C.c_int32.in_dll(self.library, 'D_800A4800').value = shift

        @Convert
        def convert(mode, argument):
            calls.append((mode, argument))
            return converted

        C.c_void_p.in_dll(self.library, 'test_convert').value = C.cast(convert, C.c_void_p).value
        result = self.function(value)
        if 0x1010 <= value <= 0x7FFF8:
            self.assertEqual(calls, [(-1, value)])
            self.assertEqual(stored.value, converted & 0xFFFF)
            self.assertEqual(result, ((converted & 0xFFFF) * (2 ** shift)) & 0xFFFFFFFF)
        else:
            self.assertEqual(calls, [])
            self.assertEqual(stored.value, 0xA55A)
            self.assertEqual(result, 0)

    def test_range_boundaries(self):
        for value in (-(1 << 31), -1, 0, 0x100F, 0x1010, 0x1011,
                      0x7FFF7, 0x7FFF8, 0x7FFF9, (1 << 31) - 1):
            self.check_conversion(value, 0x1234)

    def test_halfword_truncation_and_unsigned_shift(self):
        for converted in (-(1 << 31), -1, 0, 1, 0xFFFF, 0x10000, (1 << 31) - 1):
            for shift in range(32):
                self.check_conversion(0x2000, converted, shift)


if __name__ == '__main__':
    unittest.main()
