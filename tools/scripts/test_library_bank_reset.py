"""Bank reset preserves unrelated state and observes both callbacks before resetting."""
import ctypes as C
from pathlib import Path
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

Hook = C.CFUNCTYPE(None, C.c_int32)


class BankResetTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        root = Path(__file__).resolve().parents[2]
        directory = Path(cls.temp.name)
        header = (root / 'include/rr/library.h').read_text()
        header = re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n', '', header, flags=re.M)
        source = '''#include <stdint.h>
#include <stddef.h>
#include <string.h>
typedef uint8_t u8; typedef int8_t s8;
typedef uint16_t u16; typedef int16_t s16;
typedef uint32_t u32; typedef int32_t s32;
''' + header + '''
LibStateB0 records[32][9], replacement[9];
LibStateB0 *_ss_score[32];
u32 _snd_openflag;
s16 _snd_seq_t_max;
void (*hook)(s32);
s32 arguments[5];
void _SsVmSetSeqVol(s32 bank,s32 a,s32 b,s32 c) {
 arguments[0]=bank;arguments[1]=a;arguments[2]=b;arguments[3]=c;hook(0);
}
void _SsVmSeqKeyOff(s32 bank) { arguments[4]=bank;hook(1); }
u32 layout[] = { sizeof(LibStateB0),
 offsetof(LibStateB0,w98),offsetof(LibStateB0,b22),
 offsetof(LibStateB0,h48),offsetof(LibStateB0,w9c),
 offsetof(LibStateB0,wa0),offsetof(LibStateB0,wac),
 offsetof(LibStateB0,wa8),offsetof(LibStateB0,wa4),
 offsetof(LibStateB0,h58)};
void initialize(void) {
 int i;memset(records,0xA5,sizeof(records));memset(replacement,0x69,sizeof(replacement));
 for(i=0;i<32;i++)_ss_score[i]=records[i];
}
void replace_bank(s32 bank) {_ss_score[bank]=replacement;}
'''
        for name in ('8007F794', '8007F910', '8007F934'):
            body = (root / f'tools/scripts/fixtures/views/func_{name}.c').read_text()
            body = re.sub(r'^#include[^\n]*\n', '', body, flags=re.M)
            body = re.sub(r'\s+__asm__\("\$[0-9]+"\)', '', body)
            body = re.sub(r':\s*"\$[0-9]+"(?:,\s*"\$[0-9]+")*', '', body)
            source += body
        (directory / 'reset.c').write_text(source)
        subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory / 'reset.c'),
                        '-o', str(directory / 'reset.so')], check=True, capture_output=True)
        cls.lib = C.CDLL(str(directory / 'reset.so'))
        cls.layout = list((C.c_uint32 * 10).in_dll(cls.lib, 'layout'))
        cls.size = cls.layout[0]
        cls.records = (C.c_ubyte * (cls.size * 32 * 9)).in_dll(cls.lib, 'records')
        cls.replacement = (C.c_ubyte * (cls.size * 9)).in_dll(cls.lib, 'replacement')
        cls.flags = C.c_uint32.in_dll(cls.lib, '_snd_openflag')
        cls.count = C.c_int16.in_dll(cls.lib, '_snd_seq_t_max')
        cls.arguments = (C.c_int32 * 5).in_dll(cls.lib, 'arguments')
        for name in ('8007F794', '8007F910', '8007F934'):
            function = getattr(cls.lib, __import__('c_functions').name_at(name))
            function.argtypes = [C.c_int32]
            function.restype = None

    def expected(self, initial, start, count):
        result = bytearray(initial)
        _, w98, b22, h48, w9c, wa0, wac, wa8, wa4, h58 = self.layout
        for row in range(max(count, 0)):
            base = start + row * self.size
            for offset in (w98, w9c, wa0, wac, wa8, wa4):
                result[base+offset:base+offset+4] = bytes(4)
            result[base+b22:base+b22+2] = b'\xff\x00'
            result[base+h48:base+h48+8] = bytes(8)
            result[base+h58:base+h58+4] = struct.pack('=HH', 127, 127)
        return bytes(result)

    def call(self, name, raw_bank, callback):
        hook = Hook(callback)
        C.c_void_p.in_dll(self.lib, 'hook').value = C.cast(hook, C.c_void_p).value
        getattr(self.lib, __import__('c_functions').name_at(name))(raw_bank)

    def test_rows_flags_and_wrapper_narrowing(self):
        for name in ('8007F794', '8007F910', '8007F934'):
            for bank in (0, 1, 15, 31):
                for count in (-32768, -1, 0, 1, 8, 9):
                    with self.subTest(name=name, bank=bank, count=count):
                        self.lib.initialize()
                        self.flags.value = 0xDEADBEEF
                        self.count.value = count
                        before = bytes(self.records)
                        events = []
                        def callback(stage):
                            events.append((stage, self.flags.value, bytes(self.records)))
                        self.call(name, 0x76540000 | bank, callback)
                        self.assertEqual([e[0] for e in events], [0, 1])
                        for _, flags, records in events:
                            self.assertEqual(flags, 0xDEADBEEF)
                            self.assertEqual(records, before)
                        self.assertEqual(list(self.arguments), [bank, 0, 0, 1, bank])
                        self.assertEqual(self.flags.value, 0xDEADBEEF & ~(1 << bank))
                        self.assertEqual(bytes(self.records), self.expected(before, bank*9*self.size, count))

    def test_callbacks_replace_table_count_and_flags(self):
        self.lib.initialize()
        bank = 31
        self.flags.value = 0
        self.count.value = 0
        before = bytes(self.records)
        replacement = bytes(self.replacement)
        events = []
        def callback(stage):
            events.append((stage, self.flags.value, self.count.value))
            if stage == 0:
                self.flags.value = 0x80000040
                self.count.value = 1
            else:
                self.lib.replace_bank(bank)
                self.flags.value = 0xFFFFFFFF
                self.count.value = 7
        self.call('8007F794', bank, callback)
        self.assertEqual(events, [(0, 0, 0), (1, 0x80000040, 1)])
        self.assertEqual(bytes(self.records), before)
        self.assertEqual(bytes(self.replacement), self.expected(replacement, 0, 7))
        self.assertEqual(self.flags.value, 0x7FFFFFFF)
        self.assertEqual(self.count.value, 7)


if __name__ == '__main__':
    unittest.main()
