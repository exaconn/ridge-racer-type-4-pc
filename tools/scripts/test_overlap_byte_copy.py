"""Actual overlap-safe copy and its original direction-dependent return cursor."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest


class OverlapByteCopyTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        root = Path(__file__).resolve().parents[2]
        directory = Path(cls.temp.name)
        header = (root / 'include/rr/memory.h').read_text().replace('#include "types.h"', '')
        body = (root / 'src/main/PAL/func_80096D34.c').read_text()
        body = body.replace('#include "rr/memory.h"', '')
        body = re.sub(r'\s+__asm__\("\$[0-9]+"\)', '', body)
        body = re.sub(r':\s*"\$[0-9]+"(?:,\s*"\$[0-9]+")*', '', body)
        source = '#include <stdint.h>\ntypedef uint8_t u8; typedef uint32_t u32; typedef int32_t s32;\n' + header + body
        (directory / 'function.c').write_text(source)
        result = subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory / 'function.c'),
                                 '-o', str(directory / 'function.so')], capture_output=True)
        if result.returncode:
            raise AssertionError(result.stderr.decode())
        cls.lib = C.CDLL(str(directory / 'function.so'))
        cls.function = cls.lib.func_80096D34
        cls.function.argtypes = [C.c_void_p, C.c_void_p, C.c_int32]
        cls.function.restype = C.c_void_p

    def test_overlapping_identical_and_disjoint_ranges_preserve_all_other_bytes(self):
        initial = bytes((i * 73 + 19) & 255 for i in range(128))
        buffer = (C.c_uint8 * 128)()
        base = C.addressof(buffer)
        for source in range(33):
            for destination in range(33):
                for length in (0, 1, 2, 3, 7, 16, 31, 32, 63, 64):
                    C.memmove(base, initial, len(initial))
                    expected = bytearray(initial)
                    expected[destination:destination + length] = initial[source:source + length]
                    cursor = destination + length if destination < source and length > 0 else destination
                    result = self.function(base + destination, base + source, length)
                    self.assertEqual(result, base + cursor, (destination, source, length))
                    self.assertEqual(bytes(buffer), bytes(expected), (destination, source, length))

    def test_nonpositive_lengths_do_not_dereference_even_null_inputs(self):
        buffer = (C.c_uint8 * 16)(*range(16))
        base = C.addressof(buffer)
        for destination, source in [(0, 0), (0, base), (base, 0), (base, base), (base, base + 8)]:
            for length in (-0x80000000, -0x7FFFFFFF, -77, -1, 0):
                before = bytes(buffer)
                self.assertEqual(self.function(destination, source, length), destination or None)
                self.assertEqual(bytes(buffer), before)

    def test_separate_objects_use_full_native_addresses_and_leave_source_unchanged(self):
        source = (C.c_uint8 * 80)(*((i * 31 + 7) & 255 for i in range(80)))
        destination = (C.c_uint8 * 80)(*([0xA5] * 80))
        source_base, destination_base = C.addressof(source), C.addressof(destination)
        for length in (1, 3, 16, 31, 64):
            before_source = bytes(source)
            C.memset(destination_base, 0xA5, 80)
            expected = bytearray(destination)
            expected[8:8 + length] = before_source[4:4 + length]
            result = self.function(destination_base + 8, source_base + 4, length)
            expected_cursor = destination_base + 8 + (length if destination_base + 8 < source_base + 4 else 0)
            self.assertEqual(result, expected_cursor)
            self.assertEqual(bytes(destination), bytes(expected))
            self.assertEqual(bytes(source), before_source)


if __name__ == '__main__':
    unittest.main()
