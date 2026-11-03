"""Real row pair update with shared record view and exact packed selector bits."""
import ctypes as C
from pathlib import Path
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

Callback = C.CFUNCTYPE(None, C.c_int32, C.c_int32, C.c_int32, C.c_int32)


class ChannelRowPairUpdateTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        root = Path(__file__).resolve().parents[2]
        directory = Path(cls.temp.name)
        header = (root / 'include/rr/channels.h').read_text()
        header = re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n', '', header, flags=re.M)
        source = '''#include <stdint.h>
#include <string.h>
typedef uint8_t u8; typedef int8_t s8;
typedef uint16_t u16; typedef int16_t s16;
typedef uint32_t u32; typedef int32_t s32;
''' + header + '''
RrChannelValueRecordB0 test_records[9][17];
RrChannelValueRecordB0 *test_table[0x4100];
void (*test_callback)(s32,s32,s32,s32);
void func_800897A4(s32 packed,s32 left,s32 right,s32 mode) {
 test_callback(packed,left,right,mode);
}
void test_initialize(void) {
 s32 bank;
 memset(test_records,0xA5,sizeof(test_records));
 memset(test_table,0,sizeof(test_table));
 for(bank=-4;bank<=4;bank++) test_table[0x4000-0xF98/4+bank]=&test_records[bank+4][8];
}
'''
        body = (root / 'tools/scripts/fixtures/views/func_80084154.c').read_text()
        body = body.replace('#include "rr/channels.h"', '')
        body = body.replace('(RrChannelValueRecordB0 **)0x80110000U', '(test_table + 0x4000)')
        body = re.sub(r'\s+__asm__\("\$[0-9]+"\)', '', body)
        body = re.sub(r':\s*"\$[0-9]+"(?:,\s*"\$[0-9]+")*', '', body)
        source += body
        (directory / 'function.c').write_text(source)
        result = subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory / 'function.c'),
                                 '-o', str(directory / 'function.so')], capture_output=True)
        if result.returncode:
            raise AssertionError(result.stderr.decode())
        cls.lib = C.CDLL(str(directory / 'function.so'))
        cls.records = (C.c_uint8 * (9 * 17 * 0xB0)).in_dll(cls.lib, 'test_records')
        cls.table = (C.c_void_p * 0x4100).in_dll(cls.lib, 'test_table')
        cls.function = cls.lib.func_80084154
        cls.function.argtypes = [C.c_int32] * 4
        cls.function.restype = None

    def setUp(self):
        self.lib.test_initialize()
        self.events = []
        self.install(lambda *args: self.events.append((*args, bytes(self.records))))

    def install(self, function):
        self.callback = Callback(function)
        C.c_void_p.in_dll(self.lib, 'test_callback').value = C.cast(self.callback, C.c_void_p).value

    def offset(self, bank, row):
        return ((bank + 4) * 17 + row + 8) * 0xB0

    def mode(self, offset, value):
        C.c_int32.from_buffer(self.records, offset + 0x98).value = value

    def test_direct_modes_signed_selectors_and_full_word_truncation(self):
        table_before = bytes(self.table)
        for bank, row in [(-4, -8), (0, 0), (4, 8), (1, 2)]:
            raw_bank, raw_row = 0x12340000 | (bank & 65535), 0x43210000 | (row & 65535)
            offset = self.offset(bank, row)
            for mode in (-0x80000000, -1, 0, 2, 0x10001, 0x7FFFFFFF):
                for left, right in [(0, 0), (-1, 0x12345678), (65535, 65536),
                                    (-0x80000000, 0x7FFFFFFF), (-32768, 32767)]:
                    self.mode(offset, mode)
                    expected = bytearray(self.records)
                    struct.pack_into('<HH', expected, offset + 0x58, left & 65535, right & 65535)
                    self.function(raw_bank, raw_row, left, right)
                    self.assertEqual(bytes(self.records), bytes(expected))
                    self.assertEqual(self.events, [])
        self.assertEqual(bytes(self.table), table_before)

    def test_delegated_mode_packs_raw_selectors_and_preserves_records(self):
        table_before = bytes(self.table)
        for bank, row in [(-4, -8), (0, 0), (4, 8), (1, 2)]:
            offset = self.offset(bank, row)
            self.mode(offset, 1)
            for raw_bank in (bank, C.c_int32(0xFEDC0000 | (bank & 65535)).value):
                for raw_row in (row, 0x43210000 | (row & 65535)):
                    packed = C.c_int16(raw_bank | (raw_row << 8)).value
                    for left, right in [(-1, -2), (0, 0x12345678), (65535, 65536),
                                        (-0x80000000, 0x7FFFFFFF)]:
                        self.events.clear()
                        before = bytes(self.records)
                        self.function(raw_bank, raw_row, left, right)
                        self.assertEqual(self.events, [(packed, left & 65535, right & 65535, 1, before)])
                        self.assertEqual(bytes(self.records), before)
        self.assertEqual(bytes(self.table), table_before)

    def test_callback_record_and_table_changes_are_preserved_without_late_stores(self):
        offset, other = self.offset(1, 2), self.offset(0, 0)
        self.mode(offset, 1)
        initial = bytes(self.records)
        replacement = C.addressof(self.records) + other
        expected = bytearray(initial)
        struct.pack_into('<HH', expected, offset + 0x58, 0x1234, 0xFEDC)
        struct.pack_into('<i', expected, offset + 0x98, 2)
        def callback(*args):
            self.events.append((*args, bytes(self.records)))
            self.records[offset + 0x58:offset + 0x5C] = expected[offset + 0x58:offset + 0x5C]
            self.mode(offset, 2)
            self.table[0x4000 - 0xF98 // 4 + 1] = replacement
        self.install(callback)
        self.function(0x12340001, 0x43210002, -1, -2)
        self.assertEqual(self.events, [(0x201, 65535, 65534, 1, initial)])
        self.assertEqual(bytes(self.records), bytes(expected))
        self.assertEqual(self.table[0x4000 - 0xF98 // 4 + 1], replacement)


if __name__ == '__main__':
    unittest.main()
