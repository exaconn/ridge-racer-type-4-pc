"""Execute the real mode-setup body with observable command and callback hooks."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

Command = C.CFUNCTYPE(C.c_int32, C.c_int32, C.POINTER(C.c_uint8), C.c_void_p)
Register = C.CFUNCTYPE(None, C.c_void_p)


class LibraryModeSetupTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        directory = Path(cls.temp.name)
        root = Path(__file__).resolve().parents[2]
        header = (root / 'include/rr/library.h').read_text()
        header = re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n', '', header, flags=re.M)
        source = '''#include <stdint.h>
typedef uint8_t u8; typedef int8_t s8;
typedef uint16_t u16; typedef int16_t s16;
typedef uint32_t u32; typedef int32_t s32;
''' + header + '''
s32 test_page[0x4000];
s32 test_unexpected;
s32 (*test_command)(s32, const u8 *, u8 *);
void (*test_first)(RrLibCompletionCallback);
void (*test_second)(RrLibCompletionCallback);
s32 func_8008C140(s32 c, const u8 *p, u8 *r) { return test_command(c,p,r); }
void func_8008C55C(RrLibCompletionCallback c) { test_first(c); }
void func_8008C12C(RrLibCompletionCallback c) { test_second(c); }
void func_8008F5E4(void) { test_unexpected++; }
void func_8008F248(void) { test_unexpected++; }
'''
        body = (root / 'tools/scripts/fixtures/views/func_8008F1C4.c').read_text()
        body = body.replace('#include "rr/library.h"', '')
        body = re.sub(r'\s+__asm__\("\$[0-9]+"\)', '', body)
        body = re.sub(r'"\$[0-9]+", ', '', body)
        body = body.replace('(s32 *)0x80110000U', 'test_page')
        (directory / 'function.c').write_text(source + body)
        subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory / 'function.c'),
                        '-o', str(directory / 'function.so')], check=True, capture_output=True)
        cls.lib = C.CDLL(str(directory / 'function.so'))
        cls.page = (C.c_int32 * 0x4000).in_dll(cls.lib, 'test_page')
        cls.function = cls.lib.func_8008F1C4
        cls.function.argtypes = [C.c_int32]
        cls.function.restype = None
        cls.first = C.cast(cls.lib.func_8008F5E4, C.c_void_p).value
        cls.second = C.cast(cls.lib.func_8008F248, C.c_void_p).value

    def setUp(self):
        C.memset(C.addressof(self.page), 0x5A, C.sizeof(self.page))
        self.events = []
        self.hooks = []
        self.hook('test_command', Command, self.command)
        self.hook('test_first', Register, lambda p: self.events.append(('first', p, self.mode)))
        self.hook('test_second', Register, lambda p: self.events.append(('second', p, self.mode)))

    @property
    def mode(self):
        return self.page[0x348 // 4]

    def hook(self, name, kind, callback):
        value = kind(callback)
        self.hooks.append(value)
        C.c_void_p.in_dll(self.lib, name).value = C.cast(value, C.c_void_p).value

    def command(self, command, parameters, result):
        self.events.append(('command', command, parameters[0] if parameters else None, result, self.mode))
        return -1

    def test_flags_payload_registration_order_and_only_mode_store(self):
        for flags in (0, 0x20, 0x100, 0x120, 0x1FF, -1, -256, -512, 0x7FFFFFFF, -0x80000000):
            self.page[0x348 // 4] = 123
            self.events.clear()
            before = list(self.page)
            expected = [('command', 14, flags & 255, None, 123)]
            mode = 123
            if flags & 0x100:
                mode = 0 if flags & 0x20 else 1
                expected += [('first', self.first, mode), ('second', self.second, mode)]
            expected += [('command', 27, None, None, mode)]
            self.function(flags)
            before[0x348 // 4] = mode
            self.assertEqual(self.events, expected)
            self.assertEqual(list(self.page), before)
        self.assertEqual(C.c_int32.in_dll(self.lib, 'test_unexpected').value, 0)

    def test_command_mutation_does_not_replace_saved_flags(self):
        def command(c, p, r):
            self.command(c, p, r)
            if c == 14:
                p[0] = 0
                self.page[0x348 // 4] = 77
            return 0
        self.hook('test_command', Command, command)
        self.function(0x120)
        self.assertEqual(self.events[1:], [('first', self.first, 0), ('second', self.second, 0),
                                         ('command', 27, None, None, 0)])

    def test_registration_mutation_survives_until_final_command(self):
        def first(p):
            self.events.append(('first', p, self.mode))
            self.page[0x348 // 4] = -99
        self.hook('test_first', Register, first)
        self.function(0x100)
        self.assertEqual(self.events[1:], [('first', self.first, 1), ('second', self.second, -99),
                                         ('command', 27, None, None, -99)])
        self.assertEqual(self.mode, -99)


if __name__ == '__main__':
    unittest.main()
