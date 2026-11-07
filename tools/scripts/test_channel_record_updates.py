"""Real channel-update bodies, PSX record layout and native-width pointer slots."""
import ctypes as C
from pathlib import Path
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

Event = C.CFUNCTYPE(None)
Voice = C.CFUNCTYPE(None, C.c_int32, C.c_int32, C.c_int32, C.c_uint32, C.c_uint32)
Update = C.CFUNCTYPE(C.c_int32, C.c_int32, C.c_int32)


class ChannelRecordUpdatesTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        directory = Path(cls.temp.name)
        root = Path(__file__).resolve().parents[2]
        header = (root / 'include/rr/library.h').read_text()
        header = re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n', '', header, flags=re.M)
        # The functions never access c0; retain that PSX pointer's four-byte
        # representation so all subsequent fields keep their target offsets.
        header = header.replace('u8 *c0;', 'u32 c0;').replace('u8 *c8;', 'u32 c8;').replace('u8 *p10;', 'u32 p10;')
        source = '''#include <stdint.h>
typedef uint8_t u8; typedef int8_t s8;
typedef uint16_t u16; typedef int16_t s16;
typedef uint32_t u32; typedef int32_t s32;
''' + header + '''
_Static_assert(sizeof(RrLibStateB0) == 0xB0, "record stride");
_Static_assert(__builtin_offsetof(RrLibStateB0, b17) == 0x17, "slot offset");
_Static_assert(__builtin_offsetof(RrLibStateB0, w90) == 0x90, "result offset");
RrLibStateB0 *test_table[0x4100];
RrLibStateB0 test_records[9][17];
void (*test_first)(void);
void (*test_second)(void);
s32 (*test_update)(s32, s32);
void (*test_voice)(s32, s32, s32, u32, u32);
void _SsVmSetVol(s32 channel, s32 device, s32 slot, u32 pitch, u32 value) { test_voice(channel, device, slot, pitch, value); }
void SsUtReverbOff(void) { test_first(); }
void _SsVmDamperOff(void) { test_second(); }
s32 _SsReadDeltaValue(s32 bank, s32 row) { return test_update(bank, row); }
'''
        for name in ('80080A44', '80080AB4', '80081D94', '80080B24', '80080504'):
            body = (root / ('tools/scripts/fixtures/views/func_' + name + '.c')).read_text()
            body = body.replace('#include "rr/library.h"', '')
            body = re.sub(r'\s+__asm__\("\$[0-9]+"\)', '', body)
            body = re.sub(r'"\$[0-9]+", ', '', body)
            body = re.sub(r' : "\$[0-9]+"', '', body)
            body = body.replace('(RrLibStateB0 **)0x80110000U', '(test_table + 0x4000)')
            source += body
        (directory / 'functions.c').write_text(source)
        subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory / 'functions.c'),
                        '-o', str(directory / 'functions.so')], check=True, capture_output=True)
        cls.library = C.CDLL(str(directory / 'functions.so'))
        cls.records = (C.c_uint8 * (9 * 17 * 0xB0)).in_dll(cls.library, 'test_records')
        cls.table = (C.c_void_p * 0x4100).in_dll(cls.library, 'test_table')
        cls.functions = []
        for name in ('80080A44', '80080AB4', '80081D94'):
            fn = getattr(cls.library, __import__('c_functions').name_at(name))
            fn.argtypes = [C.c_int32] * 3
            fn.restype = None
            cls.functions.append(fn)
        cls.voice = cls.library._SsContPanpot
        cls.voice.argtypes = [C.c_int32] * 3
        cls.voice.restype = None
        cls.reset = cls.library._SsContResetAll
        cls.reset.argtypes = [C.c_int32] * 2
        cls.reset.restype = None

    def setUp(self):
        C.memset(C.addressof(self.records), 0xA5, C.sizeof(self.records))
        C.memset(C.addressof(self.table), 0, C.sizeof(self.table))
        for bank in range(-4, 5):
            self.table[0x4000 - 0xF98 // 4 + bank] = self.address(bank, 0)
        self.events = []
        self.callbacks = []
        self.hook('test_first', Event, lambda: self.events.append('first'))
        self.hook('test_second', Event, lambda: self.events.append('second'))
        def update(bank, row):
            self.events.append(('update', bank, row, self.read(bank, row)))
            return -12345
        self.hook('test_update', Update, update)
        self.current_record = (0, 0)
        self.hook('test_voice', Voice, lambda *args: self.events.append(('voice', args, self.read(*self.current_record))))

    def hook(self, name, kind, function):
        callback = kind(function)
        self.callbacks.append(callback)
        C.c_void_p.in_dll(self.library, name).value = C.cast(callback, C.c_void_p).value

    def offset(self, bank, row):
        return ((bank + 4) * 17 + row + 8) * 0xB0

    def address(self, bank, row):
        return C.addressof(self.records) + self.offset(bank, row)

    def read(self, bank, row):
        return C.string_at(self.address(bank, row), 0xB0)

    def prepare(self, bank, row, slot=15):
        self.current_record = (bank, row)
        address = self.address(bank, row)
        C.memset(address, 0xA5, 0xB0)
        C.c_uint8.from_address(address + 0x17).value = slot
        self.events.clear()
        return bytearray(self.records)

    def check_result(self, before, bank, row, changes):
        offset = self.offset(bank, row)
        for field, value in changes.items():
            before[offset + field] = value
        published = bytes(before[offset:offset + 0xB0])
        before[offset + 0x90:offset + 0x94] = struct.pack('<i', -12345)
        self.assertEqual(bytes(self.records), bytes(before))
        self.assertEqual(self.events[-1], ('update', bank, row, published))

    def test_byte_setters_increment_and_wrap_without_touching_neighbors(self):
        table_before = bytes(self.table)
        for function, field in zip(self.functions[:2], (0x18, 0x19)):
            for bank, row in [(-4, -8), (0, 0), (4, 8)]:
                for value in range(256):
                    before = self.prepare(bank, row)
                    counter = value ^ 0xFF
                    self.records[self.offset(bank, row) + 0x1E] = counter
                    before[self.offset(bank, row) + 0x1E] = counter
                    function(bank, row, value)
                    self.check_result(before, bank, row, {field: value, 0x1E: (counter + 1) & 255})
                    self.assertEqual(len(self.events), 1)
        self.assertEqual(bytes(self.table), table_before)

    def test_selected_byte_uses_current_slot_and_publishes_before_update(self):
        for slot in range(16):
            for value in range(256):
                before = self.prepare(-3, 7, slot)
                self.functions[2](-3, 7, value)
                self.check_result(before, -3, 7, {0x37 + slot: value})
                self.assertEqual(len(self.events), 1)

    def test_voice_update_arguments_and_store_order(self):
        for bank, row in [(-4, -8), (0, 0), (4, 8)]:
            for slot in range(16):
                for value in (0, 127, 128, 255):
                    before = self.prepare(bank, row, slot)
                    offset = self.offset(bank, row)
                    self.records[offset + 0x26] = value ^ 128
                    pitch = (value * 257) ^ 0xFFFF
                    self.records[offset + 0x60 + 2 * slot] = pitch & 255
                    self.records[offset + 0x61 + 2 * slot] = pitch >> 8
                    before = bytearray(self.records)
                    original = self.read(bank, row)
                    self.voice(bank, row, value)
                    self.check_result(before, bank, row, {0x27 + slot: value})
                    encoded = C.c_int16(bank | (row << 8)).value
                    self.assertEqual(self.events[0], ('voice',
                        (encoded, C.c_int8(value ^ 128).value, 0xA5, pitch, value), original))
                    self.assertEqual(len(self.events), 2)

    def test_voice_update_keeps_selected_pointer_across_callback_changes(self):
        before = self.prepare(0, 0, 1)
        address = self.address(0, 0)
        def voice(*args):
            self.events.append(('voice', args))
            C.c_uint8.from_address(address + 0x17).value = 15
            self.table[0x4000 - 0xF98 // 4] = self.address(1, 0)
        self.hook('test_voice', Voice, voice)
        self.voice(0, 0, 255)
        self.check_result(before, 0, 0, {0x17: 15, 0x28: 255})
        self.assertEqual(len(self.events), 2)

    def test_reset_callback_order_and_selected_defaults(self):
        for bank, row in [(-4, -8), (0, 0), (4, 8)]:
            for slot in range(16):
                before = self.prepare(bank, row, slot)
                self.reset(bank, row)
                changes = {0x18: 0, 0x19: 0, 0x37 + slot: slot,
                           0x60 + slot * 2: 127, 0x61 + slot * 2: 0,
                           0x27 + slot: 64}
                self.check_result(before, bank, row, changes)
                self.assertEqual(self.events[:2], ['first', 'second'])
                self.assertEqual(len(self.events), 3)

    def test_reset_retains_record_pointer_but_reads_slot_after_callbacks(self):
        before = self.prepare(0, 0, 1)
        target = self.address(0, 0)
        def first():
            self.events.append('first')
            self.table[0x4000 - 0xF98 // 4] = self.address(1, 0)
            C.c_uint8.from_address(target + 0x17).value = 15
        self.hook('test_first', Event, first)
        self.reset(0, 0)
        self.check_result(before, 0, 0, {0x17: 15, 0x18: 0, 0x19: 0,
                                      0x46: 15, 0x7E: 127, 0x7F: 0, 0x36: 64})
        self.assertEqual(self.events[:2], ['first', 'second'])


if __name__ == '__main__':
    unittest.main()
