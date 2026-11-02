"""Validate the real channel-count setter, including raw 32-bit arguments."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest


class ChannelCountTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        root = Path(__file__).resolve().parents[2]
        directory = Path(cls.temp.name)
        header = (root / 'include/rr/channels.h').read_text()
        header = re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n', '', header, flags=re.M)
        body = (root / 'src/main/PAL/func_8008A644.c').read_text().replace('#include "rr/channels.h"', '')
        source = '''#include <stdint.h>
typedef uint8_t u8; typedef int8_t s8;
typedef uint16_t u16; typedef int16_t s16;
typedef uint32_t u32; typedef int32_t s32;
''' + header + '\ns8 D_801100FC;\n' + body
        (directory / 'function.c').write_text(source)
        subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory / 'function.c'),
                        '-o', str(directory / 'function.so')], check=True, capture_output=True)
        cls.lib = C.CDLL(str(directory / 'function.so'))
        cls.count = C.c_int8.in_dll(cls.lib, 'D_801100FC')
        cls.function = cls.lib.func_8008A644
        cls.function.argtypes = [C.c_uint32]
        cls.function.restype = C.c_int32

    def test_every_low_byte_and_high_bit_pattern(self):
        for high in (0, 0x100, 0x7FFFFF00, 0x80000000, 0xFFFFFF00):
            for low in range(256):
                for previous in (-128, -1, 0, 1, 24, 127):
                    self.count.value = previous
                    result = self.function(high | low)
                    accepted = 1 <= low <= 24
                    self.assertEqual(result, low if accepted else -1)
                    self.assertEqual(self.count.value, low if accepted else previous)

    def test_rejected_call_preserves_last_successful_count(self):
        for accepted in (1, 24, 0xFFFFFF04):
            self.assertEqual(self.function(accepted), accepted & 255)
            for rejected in (0, 25, 255, 256, 0x80000000, 0xFFFFFFFF):
                self.assertEqual(self.function(rejected), -1)
                self.assertEqual(self.count.value, accepted & 255)


if __name__ == '__main__':
    unittest.main()
