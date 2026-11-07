"""Execute the real bank pair setter in both direct and delegated modes."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

Callback = C.CFUNCTYPE(None, C.c_int32, C.c_int32, C.c_int32, C.c_int32)


class ChannelPairUpdateTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        root = Path(__file__).resolve().parents[2]
        directory = Path(cls.temp.name)
        header = (root / 'include/rr/channels.h').read_text()
        header = re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n', '', header, flags=re.M)
        body = (root / 'tools/scripts/fixtures/views/func_800841E4.c').read_text()
        body = body.replace('#include "rr/channels.h"', '')
        body = re.sub(r'\s+__asm__\("\$[0-9]+"\)', '', body)
        body = re.sub(r':\s*"\$[0-9]+"(?:,\s*"\$[0-9]+")*', '', body)
        body = body.replace('(RrChannelValueRecordB0 **)0x80110000U', '(test_table + 0xF98 / 4)')
        source = '''#include <stdint.h>
typedef uint8_t u8; typedef int8_t s8;
typedef uint16_t u16; typedef int16_t s16;
typedef uint32_t u32; typedef int32_t s32;
''' + header + '''
RrChannelValueRecordB0 test_states[16];
RrChannelValueRecordB0 *test_table[16 + 0xF98 / 4];
void (*test_callback)(s32,s32,s32,s32);
void _SsVmSetSeqVol(s32 bank,s32 left,s32 right,s32 mode) {
 test_callback(bank,left,right,mode);
}
void test_initialize(void) {
 int i;
 for(i=0;i<16;i++) test_table[i]=&test_states[i];
}
''' + body
        (directory / 'function.c').write_text(source)
        subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory / 'function.c'),
                        '-o', str(directory / 'function.so')], check=True, capture_output=True)
        cls.lib = C.CDLL(str(directory / 'function.so'))
        cls.states = (C.c_uint8 * (16 * 0xB0)).in_dll(cls.lib, 'test_states')
        cls.lib.test_initialize()
        cls.function = cls.lib.SsSeqSetVol
        cls.function.argtypes = [C.c_int16, C.c_int32, C.c_int32]
        cls.function.restype = None

    def setUp(self):
        C.memset(C.addressof(self.states), 0xA5, C.sizeof(self.states))
        self.events = []
        self.callback = Callback(lambda *args: self.events.append((*args, bytes(self.states))))
        C.c_void_p.in_dll(self.lib, 'test_callback').value = C.cast(self.callback, C.c_void_p).value

    def mode(self, bank, value):
        C.c_int32.from_buffer(self.states, bank * 0xB0 + 0x98).value = value

    def test_direct_modes_truncate_both_values_and_preserve_other_fields(self):
        for bank in (0, 1, 15):
            for mode in (0, 2, -1, -0x80000000, 0x7FFFFFFF):
                for left, right in ((0, 0), (-1, 0x12345678), (-0x80000000, 0x7FFFFFFF), (65535, 65536)):
                    self.mode(bank, mode)
                    expected = bytearray(self.states)
                    offset = bank * 0xB0
                    expected[offset + 0x58:offset + 0x5A] = (left & 65535).to_bytes(2, 'little')
                    expected[offset + 0x5A:offset + 0x5C] = (right & 65535).to_bytes(2, 'little')
                    self.function(bank, left, right)
                    self.assertEqual(bytes(self.states), bytes(expected))
                    self.assertEqual(self.events, [])

    def test_delegated_mode_passes_unsigned_halfwords_without_direct_stores(self):
        for bank in (0, 1, 15):
            for left, right in ((0, 0), (-1, 0x12345678), (-0x80000000, 0x7FFFFFFF)):
                self.mode(bank, 1)
                self.events.clear()
                before = bytes(self.states)
                self.function(bank, left, right)
                self.assertEqual(self.events, [(bank, left & 65535, right & 65535, 1, before)])
                self.assertEqual(bytes(self.states), before)

    def test_callback_mutation_is_preserved(self):
        def callback(*args):
            self.events.append(args)
            self.states[0x58] = 0x42
            self.mode(0, 2)
        self.callback = Callback(callback)
        C.c_void_p.in_dll(self.lib, 'test_callback').value = C.cast(self.callback, C.c_void_p).value
        self.mode(0, 1)
        self.function(0, -1, -2)
        self.assertEqual(self.events, [(0, 65535, 65534, 1)])
        self.assertEqual(self.states[0x58], 0x42)
        self.assertEqual(C.c_int32.from_buffer(self.states, 0x98).value, 2)


if __name__ == '__main__':
    unittest.main()
