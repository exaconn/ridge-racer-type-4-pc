"""Execute the real bank pair setter in both direct and delegated modes."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest
from sequence_fixture_transport import wire_definition, field_only_body, transport, current_body, compile_library

Callback = C.CFUNCTYPE(None, C.c_int32, C.c_int32, C.c_int32, C.c_int32)


class ChannelPairUpdateTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        root = Path(__file__).resolve().parents[2]
        directory = Path(cls.temp.name)
        source = '#include \"rr/sequence_channel.h\"\n' + wire_definition("value") + r'''SequenceWire176 test_states[16];
SequenceWire176 *test_table[16+0xF98/4];
void test_initialize(void){int i;for(i=0;i<16;i++)test_table[i]=&test_states[i];}
void (*test_callback)(s32,s32,s32,s32);
'''
        source += transport(wire_rows='test_states',banks=1,rows=16,table_size=1014)
        source += '#define _ss_score (sequence_native_table+0)\n'
        source += r'''s16 _SsVmSetSeqVol(s32 packed,s32 left,s32 right,s32 mode){sequence_export();test_callback(packed,left,right,mode);sequence_import();return (s16)packed;}
'''
        body=field_only_body(current_body(root,'800841E4'))
        body=re.sub(r'\s+(?:__asm__|asm)\(\"\$[0-9]+\"\)','',body)
        body=re.sub(r'__asm__(?: volatile)?\(\"\"[^;]*;','',body)
        body=body.replace('(ChannelValueRecordB0 **)0x80110000U','(sequence_native_table+0+0xF98/4)')
        name=__import__('c_functions').name_at('800841E4')
        source += re.sub(r'\b'+name+r'\b','native_'+name,body)
        source += 'void '+name+'(s16 bank,s32 left,s32 right){sequence_import();native_'+name+'(bank,left,right);sequence_export();}\n'
        compile_library(root,directory,source,'function')
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
