"""Run the actual wrapped counter update, including callback polling and reloads."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest
from sequence_fixture_transport import wire_definition, field_only_body, transport, current_body, compile_library

Update = C.CFUNCTYPE(None, C.c_int32, C.c_int32)
wrap = lambda value: C.c_int32(value).value


class ChannelCounterUpdateTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        root = Path(__file__).resolve().parents[2]
        directory = Path(cls.temp.name)
        source = '#include \"rr/sequence_channel.h\"\n' + wire_definition() + r'''SequenceWire176 test_records[5][5];
SequenceWire176 *test_table[0x1100];
static void sequence_import(void),sequence_export(void);
void (*test_update)(s32,s32);
void test_initialize(void){int bank;for(bank=-2;bank<=2;bank++)test_table[0x1000-0xF98/4+bank]=&test_records[bank+2][2];}
'''
        source += transport(banks=5,rows=5,table_size=0x1100)
        source += r'''void _SsGetSeqData(s32 bank,s32 row){sequence_export();test_update(bank,row);sequence_import();}
#define _ss_score (sequence_native_table+0x1000-0xF98/4)
'''
        body=field_only_body(current_body(root,'800832A4'))
        name=__import__('c_functions').name_at('800832A4')
        source += re.sub(r'\b'+name+r'\b','native_'+name,body)
        source += 'void '+name+'(s32 bank,s32 row){sequence_import();native_'+name+'(bank,row);sequence_export();}\n'
        compile_library(root,directory,source,'function')
        cls.lib = C.CDLL(str(directory / 'function.so'))
        cls.records = (C.c_uint8 * (25 * 0xB0)).in_dll(cls.lib, 'test_records')
        cls.table = (C.c_void_p * 0x1100).in_dll(cls.lib, 'test_table')
        cls.function = cls.lib._SsSeqPlay
        cls.function.argtypes = [C.c_int32, C.c_int32]
        cls.function.restype = None


    def setUp(self):
        C.memset(C.addressof(self.records), 0xA5, C.sizeof(self.records))
        self.lib.test_initialize()
        self.events = []
        def unexpected(bank, row):
            self.events.append((bank, row))
            offset = self.offset(bank, row)
            C.c_int32.from_buffer(self.records, offset + 0x90).value = 0x7FFFFFFF
            C.c_int16.from_buffer(self.records, offset + 0x54).value = -32768
        self.hook(unexpected)

    def hook(self, callback):
        self.callback = Update(callback)
        C.c_void_p.in_dll(self.lib, 'test_update').value = C.cast(self.callback, C.c_void_p).value

    def offset(self, bank, row):
        return ((bank + 2) * 5 + row + 2) * 0xB0

    def prepare(self, bank, row, current, period, delay):
        offset = self.offset(bank, row)
        C.c_int32.from_buffer(self.records, offset + 0x90).value = current
        C.c_int16.from_buffer(self.records, offset + 0x54).value = period
        C.c_int16.from_buffer(self.records, offset + 0x52).value = delay
        return offset

    def test_fast_paths_and_overflow_skip_preserve_unrelated_fields(self):
        for bank, row in ((0, 0), (2, 2), (-2, -2)):
            for period in (-32768, -7, 0, 1, 32767):
                for current in (-0x80000000, -100, 0, 100, 0x7FFFFFFF):
                    delta = wrap(current - period)
                    if delta <= 0 and period >= current:
                        continue  # polling cases are exercised separately
                    for delay in (-32768, -1, 0, 1, 32767):
                        offset = self.prepare(bank, row, current, period, delay)
                        expected = bytearray(self.records)
                        if delta > 0:
                            if delay > 0:
                                expected[offset + 0x52:offset + 0x54] = (delay - 1).to_bytes(2, 'little', signed=True)
                            elif delay == 0:
                                expected[offset + 0x52:offset + 0x54] = period.to_bytes(2, 'little', signed=True)
                                expected[offset + 0x90:offset + 0x94] = wrap(current - 1).to_bytes(4, 'little', signed=True)
                            else:
                                expected[offset + 0x90:offset + 0x94] = delta.to_bytes(4, 'little', signed=True)
                        self.function(bank, row)
                        self.assertEqual(self.events, [])
                        self.assertEqual(bytes(self.records), bytes(expected))

    def test_polling_ignores_zero_updates_and_wraps_accumulation(self):
        cases = [(0, 7, [0, 0, 3, 0, 4]), (3, 7, [2, 2]), (7, 7, [1]),
                 (-10, 10, [0, 5, 5, 20]), (-0x80000000, 0, [0x7FFFFFFF, 1]),
                 (1, 2, [0x7FFFFFFF, 0x7FFFFFFF, 3])]
        for current, period, updates in cases:
            offset = self.prepare(0, 0, current, period, 17)
            self.events.clear()
            pending = iter(updates)
            def update(bank, row):
                self.events.append((bank, row))
                C.c_int32.from_buffer(self.records, offset + 0x90).value = next(pending, 0x7FFFFFFF)
            self.hook(update)
            expected = bytearray(self.records)
            accumulated = current
            for value in updates:
                if value:
                    accumulated = wrap(accumulated + value)
            result = wrap(accumulated - period)
            expected[offset + 0x90:offset + 0x94] = result.to_bytes(4, 'little', signed=True)
            self.function(0, 0)
            self.assertEqual(self.events, [(0, 0)] * len(updates))
            self.assertEqual(bytes(self.records), bytes(expected))

    def test_period_reload_and_callback_changes_survive_with_signed_arguments(self):
        offset = self.prepare(-1, -2, 0, 100, 5)
        updates = [(0, 123), (3, 10), (0, -2), (1, -2)]
        pending = iter(updates)
        def update(bank, row):
            self.events.append((bank, row))
            value, period = next(pending, (0x7FFFFFFF, -32768))
            C.c_int32.from_buffer(self.records, offset + 0x90).value = value
            C.c_int16.from_buffer(self.records, offset + 0x54).value = period
            C.c_int16.from_buffer(self.records, offset + 0x52).value = -99
            self.records[offset + 0x17] = 11
        self.hook(update)
        self.function(0x1234FFFF, 0x2345FFFE)
        self.assertEqual(self.events, [(-1, -2)] * 4)
        self.assertEqual(C.c_int32.from_buffer(self.records, offset + 0x90).value, 6)
        self.assertEqual(C.c_int16.from_buffer(self.records, offset + 0x54).value, -2)
        self.assertEqual(C.c_int16.from_buffer(self.records, offset + 0x52).value, -99)
        self.assertEqual(self.records[offset + 0x17], 11)

    def test_lookup_pointer_is_retained_across_callback(self):
        offset = self.prepare(0, 0, 0, 2, 0)
        original = self.table[0x1000 - 0xF98 // 4]
        replacement = C.addressof(self.records) + self.offset(1, 0)
        def update(bank, row):
            self.events.append((bank, row))
            self.table[0x1000 - 0xF98 // 4] = replacement
            C.c_int32.from_buffer(self.records, offset + 0x90).value = 2
        self.hook(update)
        self.function(0, 0)
        self.assertEqual(self.events, [(0, 0)])
        self.assertEqual(self.table[0x1000 - 0xF98 // 4], replacement)
        self.assertNotEqual(original, replacement)
        self.assertEqual(C.c_int32.from_buffer(self.records, offset + 0x90).value, 0)
        self.assertEqual(C.c_int32.from_buffer(self.records, self.offset(1, 0) + 0x90).value, wrap(0xA5A5A5A5))


if __name__ == '__main__':
    unittest.main()
