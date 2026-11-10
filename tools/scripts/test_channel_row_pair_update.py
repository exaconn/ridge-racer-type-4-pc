"""Real row pair update with shared record view and exact packed selector bits."""
import ctypes as C
from pathlib import Path
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest
from sequence_fixture_transport import wire_definition, field_only_body, transport, current_body, compile_library

Callback = C.CFUNCTYPE(None, C.c_int32, C.c_int32, C.c_int32, C.c_int32)


class ChannelRowPairUpdateTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        root = Path(__file__).resolve().parents[2]
        directory = Path(cls.temp.name)
        source = '#include \"rr/sequence_channel.h\"\n' + wire_definition("value") + r'''#include <string.h>
SequenceWire176 test_records[9][17];
SequenceWire176 *test_table[0x4100];
void test_initialize(void){s32 bank;memset(test_records,0xA5,sizeof(test_records));memset(test_table,0,sizeof(test_table));for(bank=-4;bank<=4;bank++)test_table[0x4000-0xF98/4+bank]=&test_records[bank+4][8];}
void (*test_callback)(s32,s32,s32,s32);
'''
        source += transport(wire_rows='test_records',banks=9,rows=17,table_size=16640)
        source += '#define _ss_score (sequence_native_table+0x4000-0xF98/4)\n'
        source += r'''s16 _SsVmSetSeqVol(s32 packed,s32 left,s32 right,s32 mode){sequence_export();test_callback(packed,left,right,mode);sequence_import();return (s16)packed;}
'''
        body=field_only_body(current_body(root,'80084154'))
        body=re.sub(r'\s+(?:__asm__|asm)\(\"\$[0-9]+\"\)','',body)
        body=re.sub(r'__asm__(?: volatile)?\(\"\"[^;]*;','',body)
        body=body.replace('(ChannelValueRecordB0 **)0x80110000U','(sequence_native_table+0x4000-0xF98/4+0xF98/4)')
        name=__import__('c_functions').name_at('80084154')
        source += re.sub(r'\b'+name+r'\b','native_'+name,body)
        source += 'void '+name+'(s32 bank,s32 row,s32 left,s32 right){sequence_import();native_'+name+'(bank,row,left,right);sequence_export();}\n'
        compile_library(root,directory,source,'function')
        cls.lib = C.CDLL(str(directory / 'function.so'))
        cls.records = (C.c_uint8 * (9 * 17 * 0xB0)).in_dll(cls.lib, 'test_records')
        cls.table = (C.c_void_p * 0x4100).in_dll(cls.lib, 'test_table')
        cls.function = cls.lib._SsSndSetVol
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
