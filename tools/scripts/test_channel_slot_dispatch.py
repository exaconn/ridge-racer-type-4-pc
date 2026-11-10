"""Exercise real slot dispatch, signed record selection and both callback ABIs."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest
from sequence_fixture_transport import wire_definition, field_only_body, transport, current_body, compile_library

Active = C.CFUNCTYPE(None, *([C.c_int32] * 6))
Inactive = C.CFUNCTYPE(None, *([C.c_int32] * 4))


class ChannelSlotDispatchTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        root = Path(__file__).resolve().parents[2]
        directory = Path(cls.temp.name)
        source = '#include \"rr/sequence_channel.h\"\n' + wire_definition() + r'''SequenceWire176 test_records[5][5];
SequenceWire176 *test_table[0x1100];
static void sequence_import(void),sequence_export(void);
void (*test_active)(s32,s32,s32,s32,s32,s32);
void (*test_inactive)(s32,s32,s32,s32);
void test_initialize(void){int bank;for(bank=-2;bank<=2;bank++)test_table[0x1000-0xF98/4+bank]=&test_records[bank+2][2];}
'''
        source += transport(banks=5,rows=5,table_size=0x1100)
        source += r'''void _SsVmKeyOn(s32 a,s32 b,s32 c,s32 d,s32 e,s32 f){sequence_export();test_active(a,b,c,d,e,f);sequence_import();}
void _SsVmKeyOff(s32 a,s32 b,s32 c,s32 d){sequence_export();test_inactive(a,b,c,d);sequence_import();}
#define _ss_score (sequence_native_table+0x1000-0xF98/4)
'''
        body=field_only_body(current_body(root,'80081CB4'))
        name=__import__('c_functions').name_at('80081CB4')
        source += re.sub(r'\b'+name+r'\b','native_'+name,body)
        source += 'void '+name+'(s32 bank,s32 row,s32 value,s32 mode){sequence_import();native_'+name+'(bank,row,value,mode);sequence_export();}\n'
        compile_library(root,directory,source,'function')
        cls.lib = C.CDLL(str(directory / 'function.so'))
        cls.records = (C.c_uint8 * (25 * 0xB0)).in_dll(cls.lib, 'test_records')
        cls.lib.test_initialize()
        cls.function = cls.lib._SsNoteOn
        cls.function.argtypes = [C.c_int32] * 4
        cls.function.restype = None


    def setUp(self):
        C.memset(C.addressof(self.records), 0xA5, C.sizeof(self.records))
        self.events = []
        self.active = Active(lambda *args: self.events.append(('active', args)))
        self.inactive = Inactive(lambda *args: self.events.append(('inactive', args)))
        C.c_void_p.in_dll(self.lib, 'test_active').value = C.cast(self.active, C.c_void_p).value
        C.c_void_p.in_dll(self.lib, 'test_inactive').value = C.cast(self.inactive, C.c_void_p).value

    def offset(self, bank, row):
        return ((bank + 2) * 5 + row + 2) * 0xB0

    def prepare(self, bank, row, slot, mask):
        offset = self.offset(bank, row)
        self.records[offset + 0x17] = slot
        self.records[offset + 0x26] = 0x80 + slot
        for i in range(16):
            self.records[offset + 0x27 + i] = (i * 13 + 7) & 255
            self.records[offset + 0x37 + i] = (i * 17 + 3) & 255
        C.c_uint16.from_buffer(self.records, offset + 0x80).value = mask
        return C.c_int8(0x80 + slot).value, (slot * 17 + 3) & 255, (slot * 13 + 7) & 255

    def test_modes_masks_and_every_slot_preserve_records(self):
        for bank, row in ((0, 0), (2, 2), (-2, -2)):
            encoded = C.c_int16(bank | (row << 8)).value
            for slot in range(16):
                for blocked in (False, True):
                    signed, second, first = self.prepare(bank, row, slot, (1 << slot) if blocked else 0)
                    for mode in (0, 1, 255, 256, -1, -256):
                        self.events.clear()
                        before = bytes(self.records)
                        self.function(bank, row, 0x123456AB, mode)
                        if mode & 255:
                            expected = [] if blocked else [('active', (encoded, signed, second, 0xAB, mode & 255, first))]
                        else:
                            expected = [('inactive', (encoded, signed, second, 0xAB))]
                        self.assertEqual(self.events, expected)
                        self.assertEqual(bytes(self.records), before)

    def test_raw_bank_row_high_bits_and_value_truncation(self):
        for bank, row, raw_bank, raw_row in ((1, 2, 0x12340001, 0x43210002),
                                           (-1, -2, 0x1234FFFF, -2)):
            signed, second, first = self.prepare(bank, row, 15, 0)
            for value in (0, -1, -0x80000000, 0x7FFFFFFF, 256):
                self.events.clear()
                self.function(raw_bank, raw_row, value, 0x123401AB)
                encoded = C.c_int16(raw_bank | (raw_row << 8)).value
                self.assertEqual(self.events, [('active', (encoded, signed, second, value & 255, 0xAB, first))])

    def test_callback_changes_are_not_overwritten(self):
        offset = self.offset(0, 0)
        self.prepare(0, 0, 0, 0)
        def active(*args):
            self.events.append(args)
            C.c_uint16.from_buffer(self.records, offset + 0x80).value = 1
            self.records[offset + 0x27] = 99
        self.active = Active(active)
        C.c_void_p.in_dll(self.lib, 'test_active').value = C.cast(self.active, C.c_void_p).value
        self.function(0, 0, -1, 1)
        self.assertEqual(len(self.events), 1)
        self.assertEqual(self.records[offset + 0x27], 99)
        self.function(0, 0, -1, 1)
        self.assertEqual(len(self.events), 1)


if __name__ == '__main__':
    unittest.main()
