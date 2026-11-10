"""Actual bank release body: guarded lookup, callback order and counter reload."""
import ctypes as C
from pathlib import Path
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from c_functions import definition_source, source_owners

Free = C.CFUNCTYPE(None, C.c_uint32)


class Page(C.Structure):
    _fields_ = [('prefix', C.c_uint8 * 0x120), ('flags', C.c_uint8 * 16),
                ('pad130', C.c_uint8 * 0x48), ('count', C.c_uint16),
                ('pad17A', C.c_uint8 * 6), ('addresses', C.c_uint32 * 16),
                ('suffix', C.c_uint8 * (0x10000 - 0x1C0))]


class SpuBankReleaseTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        directory = Path(cls.temp.name)
        root = Path(__file__).resolve().parents[2]
        header = '#include "rr/spu.h"\n'
        source = '''#include <stdint.h>
#include <stddef.h>
typedef uint8_t u8; typedef uint16_t u16; typedef int16_t s16;
typedef uint32_t u32; typedef int32_t s32;
''' + header + '''
struct { u8 prefix[0x120]; u8 flags[16]; u8 pad130[0x48];
         u16 count; u8 pad17A[6]; u32 addresses[16];
         u8 suffix[0x10000 - 0x1C0]; } test_page;
_Static_assert(offsetof(__typeof__(test_page), count) == 0x178, "count offset");
_Static_assert(offsetof(__typeof__(test_page), addresses) == 0x180, "address offset");
#define _svm_vab_used test_page.flags
#define _svm_vab_count test_page.count
#define _svm_vab_start test_page.addresses
void (*test_free)(u32 address);
void SpuFree(u32 address) { test_free(address); }
'''
        owner = source_owners(root / 'src/main/PAL')['SsVabClose']
        body = definition_source(owner.read_text(), 'SsVabClose')
        source += body
        (directory / 'function.c').write_text(source)
        subprocess.run(['cc', '-I'+str(Path(__file__).resolve().parents[2]/'include'), '-shared', '-fPIC', '-O2', str(directory / 'function.c'),
                        '-o', str(directory / 'function.so')], check=True, capture_output=True)
        cls.library = C.CDLL(str(directory / 'function.so'))
        cls.page = Page.in_dll(cls.library, 'test_page')
        cls.function = cls.library.SsVabClose
        cls.function.argtypes = [C.c_int16]
        cls.function.restype = None

    def setUp(self):
        self.events = []
        self.callback = Free(lambda address: self.events.append((address, bytes(self.page))))
        C.c_void_p.in_dll(self.library, 'test_free').value = C.cast(self.callback, C.c_void_p).value
        self.initialize()

    def initialize(self):
        C.memset(C.addressof(self.page), 0xA5, C.sizeof(self.page))
        self.events.clear()

    def test_every_invalid_bank_is_rejected_without_callback_or_writes(self):
        before = bytes(self.page)
        for bank in range(16, 65536):
            self.function(bank)
        self.assertEqual(bytes(self.page), before)
        self.assertEqual(self.events, [])

    def test_only_flag_one_releases_and_count_wraps(self):
        for bank in range(16):
            for flag in range(256):
                self.initialize()
                self.page.flags[bank] = flag
                count = (0, 1, 65535)[flag % 3]
                self.page.count = count
                self.page.addresses[bank] = address = 0xFEDC0000 + bank * 16
                before = bytes(self.page)
                expected = bytearray(before)
                if flag == 1:
                    expected[0x120 + bank] = 0
                    expected[0x178:0x17A] = struct.pack('<H', (count - 1) & 65535)
                self.function(bank)
                self.assertEqual(bytes(self.page), bytes(expected))
                self.assertEqual(self.events, [(address, before)] if flag == 1 else [])
        for count in (0, 1, 65535):
            self.initialize()
            self.page.flags[7] = 1
            self.page.count = count
            self.function(7)
            self.assertEqual(self.page.count, (count - 1) & 65535)

    def test_callback_changes_are_observed_before_final_updates(self):
        self.page.flags[3] = 1
        self.page.count = 123
        self.page.addresses[3] = 0xFFFFFFFF
        before = bytes(self.page)
        def release(address):
            self.events.append((address, bytes(self.page)))
            self.page.flags[3] = 2
            self.page.count = 7
            self.page.addresses[3] = 0xDEADBEEF
        self.callback = Free(release)
        C.c_void_p.in_dll(self.library, 'test_free').value = C.cast(self.callback, C.c_void_p).value
        self.function(3)
        expected = Page.from_buffer_copy(before)
        expected.flags[3] = 0
        expected.count = 6
        expected.addresses[3] = 0xDEADBEEF
        self.assertEqual(bytes(self.page), bytes(expected))
        self.assertEqual(self.events, [(0xFFFFFFFF, before)])

    def test_argument_uses_low_halfword(self):
        for upper in (0, 0x10000, 0x7FFF0000, -0x80000000):
            self.initialize()
            self.page.flags[15] = 1
            self.page.addresses[15] = 0
            self.function(upper + 15)
            self.assertEqual(self.page.flags[15], 0)
            self.assertEqual(self.events[0][0], 0)


if __name__ == '__main__':
    unittest.main()
