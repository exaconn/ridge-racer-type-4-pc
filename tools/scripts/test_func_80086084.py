"""Host checks for masked SPU register writes; fd.py checks target code/layout."""
import ctypes as C
from pathlib import Path
import random
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest


class Patch(C.Structure):
    _fields_ = [('mask', C.c_int32), ('values', C.c_uint16 * 32)]


class Bank(C.Structure):
    _fields_ = [('padding', C.c_uint8 * 0x1C0), ('control', C.c_uint16 * 32)]


class MaskedSpuWriteTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        directory = Path(cls.temp.name)
        root = Path(__file__).resolve().parents[2]
        header = (root / 'include/rr/spu.h').read_text()
        header = re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n', '', header, flags=re.M)
        source = (root / 'tools/scripts/fixtures/legacy/func_80086084.c').read_text()
        source = source.replace('#include "rr/spu.h"', '''#include <stdint.h>
typedef uint8_t u8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
''' + header)
        source += (root / 'tools/scripts/fixtures/legacy/func_80086B84.c').read_text().replace(
            '#include "rr/spu.h"', '')
        source += '\nRrSpuRegisterBank *_spu_RXX;\n'
        (directory / 'function.c').write_text(source)
        subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory / 'function.c'),
                        '-o', str(directory / 'function.so')], check=True, capture_output=True)
        cls.library = C.CDLL(str(directory / 'function.so'))
        cls.function = cls.library._spu_setReverbAttr
        cls.function.argtypes = [C.POINTER(Patch)]
        cls.function.restype = None
        cls.volume = cls.library.func_80086B84
        cls.volume.argtypes = [C.c_uint32, C.POINTER(C.c_uint16), C.POINTER(C.c_uint16)]
        cls.volume.restype = None

    def check_mask(self, mask, values):
        bank = Bank()
        C.memset(C.addressof(bank), 0xA5, C.sizeof(bank))
        previous = list(bank.control)
        patch = Patch(C.c_int32(mask).value, (C.c_uint16 * 32)(*values))
        before = C.string_at(C.addressof(patch), C.sizeof(patch))
        C.c_void_p.in_dll(self.library, '_spu_RXX').value = C.addressof(bank)
        self.function(C.byref(patch))
        expected = [values[i] if mask == 0 or mask & (1 << i) else previous[i]
                    for i in range(32)]
        self.assertEqual(list(bank.control), expected)
        self.assertEqual(bytes(bank.padding), b'\xA5' * 0x1C0)
        self.assertEqual(C.string_at(C.addressof(patch), C.sizeof(patch)), before)

    def test_zero_mask_all_bits_and_each_individual_bit(self):
        values = [i * 2048 + i for i in range(32)]
        for mask in (0, 0xFFFFFFFF, 0xAAAAAAAA, 0x55555555, *[1 << i for i in range(32)]):
            self.check_mask(mask, values)

    def test_deterministic_masks_and_halfword_values(self):
        rng = random.Random(86084)
        for _ in range(1000):
            self.check_mask(rng.getrandbits(32), [rng.getrandbits(16) for _ in range(32)])

    def test_volume_conversion_exhausts_both_halfword_inputs(self):
        bank = Bank()
        words = (C.c_uint16 * (C.sizeof(bank) // 2)).from_buffer(bank)
        C.c_void_p.in_dll(self.library, '_spu_RXX').value = C.addressof(bank)
        left, right = C.c_uint16(), C.c_uint16()

        def decoded(value):
            return (value - 0x8000) & 0xFFFF if value >= 0x4000 else value

        for value in range(0x10000):
            voice = value % 24
            other = value ^ 0xA55A
            words[voice * 8], words[voice * 8 + 1] = value, other
            self.volume(voice, C.byref(left), C.byref(right))
            self.assertEqual((left.value, right.value), (decoded(value), decoded(other)))

    def test_volume_reads_both_channels_before_writing_overlapping_outputs(self):
        bank = Bank()
        words = (C.c_uint16 * (C.sizeof(bank) // 2)).from_buffer(bank)
        C.c_void_p.in_dll(self.library, '_spu_RXX').value = C.addressof(bank)
        for voice in range(24):
            words[voice * 8], words[voice * 8 + 1] = 0x8001, 0x9002
            right_register = C.cast(C.byref(bank, voice * 16 + 2), C.POINTER(C.c_uint16))
            output = C.c_uint16()
            self.volume(voice, right_register, C.byref(output))
            self.assertEqual((words[voice * 8 + 1], output.value), (1, 0x1002))
            self.assertEqual(words[voice * 8], 0x8001)

            before = C.string_at(C.addressof(bank), C.sizeof(bank))
            self.volume(voice, C.byref(output), C.byref(output))
            self.assertEqual(output.value, 1)
            self.assertEqual(C.string_at(C.addressof(bank), C.sizeof(bank)), before)


if __name__ == '__main__':
    unittest.main()
