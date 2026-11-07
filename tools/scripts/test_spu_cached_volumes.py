"""Behavior of the recovered software SPU volume setters, using their real C bodies."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest


class Entry(C.Structure):
    _fields_ = [('left', C.c_int16), ('right', C.c_int16), ('pad', C.c_uint8 * 12)]


class Cache(C.Structure):
    _fields_ = [('flags', C.c_uint8 * 32), ('voices', Entry * 24)]


class Holder(C.Structure):
    _fields_ = [('cache', Cache), ('extra', C.c_uint8 * 0x200)]


class CachedVolumesTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        directory = Path(cls.temp.name)
        root = Path(__file__).resolve().parents[2]
        header = (root / 'include/rr/spu.h').read_text()
        header = re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n', '', header, flags=re.M)
        source = '''#include <stdint.h>
typedef uint8_t u8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
''' + header + '''
struct { RrSpuVolumeCache cache; u8 extra[0x200]; } test_holder;
#define _svm_sreg_dirty test_holder.cache
'''
        for name in ('800869F0', '80086B00'):
            body = (root / ('tools/scripts/fixtures/views/func_' + name + '.c')).read_text()
            body = body.replace('#include "rr/spu.h"', '')
            body = re.sub(r'\s+__asm__\("\$[0-9]+"\)', '', body)
            body = re.sub(r'"\$[0-9]+", ', '', body)
            body = re.sub(r' : "\$[0-9]+"', '', body)
            body = body.replace('(u8 *)0x80110000U', '((u8 *)&test_holder + 0x1E8)')
            source += body
        (directory / 'functions.c').write_text(source)
        subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory / 'functions.c'),
                        '-o', str(directory / 'functions.so')], check=True, capture_output=True)
        cls.library = C.CDLL(str(directory / 'functions.so'))
        cls.holder = Holder.in_dll(cls.library, 'test_holder')
        cls.functions = [cls.library.func_800869F0, cls.library.SsUtSetVVol]
        for function in cls.functions:
            function.argtypes = [C.c_int32] * 3
            function.restype = C.c_int32

    def initialize(self):
        C.memset(C.addressof(self.holder), 0xA5, C.sizeof(self.holder))
        for i in range(32):
            self.holder.cache.flags[i] = (i * 37) & 255
        return C.string_at(C.addressof(self.holder), C.sizeof(self.holder))

    def test_valid_voices_truncation_scaling_and_write_boundaries(self):
        values = [-2147483648, -65537, -32768, -1, 0, 1, 32767,
                  32768, 65535, 65536, 2147483647]
        for function, scale in zip(self.functions, (1, 129)):
            for voice in range(24):
                for upper in (0, 0x10000, 0x7FFF0000, -0x80000000):
                    for left in values:
                        for right in values:
                            expected = Holder.from_buffer_copy(self.initialize())
                            expected.cache.voices[voice].left = C.c_int16(left).value * scale
                            expected.cache.voices[voice].right = C.c_int16(right).value * scale
                            expected.cache.flags[voice] |= 3
                            self.assertEqual(function(upper + voice, left, right), 0)
                            self.assertEqual(bytes(self.holder), bytes(expected))

    def test_every_invalid_low_halfword_leaves_cache_unchanged(self):
        for function in self.functions:
            before = self.initialize()
            for voice in range(24, 65536):
                self.assertEqual(function(voice, -32768, 32767), -1)
            for voice in (-1, -32768, 0x7FFFFFFF):
                self.assertEqual(function(voice, 1, 2), -1)
            self.assertEqual(bytes(self.holder), before)

    def test_all_dirty_flag_values_preserve_other_bits(self):
        for function in self.functions:
            for flags in range(256):
                self.initialize()
                self.holder.cache.flags[7] = flags
                self.assertEqual(function(7, 12, -34), 0)
                self.assertEqual(self.holder.cache.flags[7], flags | 3)


if __name__ == '__main__':
    unittest.main()
