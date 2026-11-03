"""Real interrupt setup and typed dispatch wrappers, including callback side effects."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

Reset = C.CFUNCTYPE(None)
Install = C.CFUNCTYPE(C.c_void_p, C.c_int32, C.c_void_p)


class Driver(C.Structure):
    _fields_ = [('reserved', C.c_void_p), ('exchange', C.c_void_p), ('install', C.c_void_p), ('reset', C.c_void_p)]


class InterruptDriverSetupTest(unittest.TestCase):
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
u32 test_ram[0x10000 / 4];
#define D_800A6488 test_ram[0x6488 / 4]
#define D_800A648C test_ram[0x648C / 4]
#define D_800A6498 test_ram[0x6498 / 4]
RrLibInterruptDriverPrefix test_driver, test_alternate_driver;
RrLibInterruptDriverPrefix *D_800A6350;
s32 test_handler_calls;
void func_8008DDAC(void) { test_handler_calls++; }
'''
        for name in ('8008B364', '8008B394', '8008D820'):
            body = (root / ('build/legacy/func_' + name + '.c')).read_text()
            body = body.replace('#include "rr/library.h"', '')
            body = re.sub(r'^extern s32 D_[^\n]*\n', '', body, flags=re.M)
            body = re.sub(r'\s+__asm__\("\$[0-9]+"\)', '', body)
            body = re.sub(r'"\$[0-9]+", ', '', body)
            body = re.sub(r' : "\$[0-9]+"', '', body)
            body = body.replace('(s32 *)0x800A0000U', '(s32 *)test_ram')
            source += body
        (directory / 'functions.c').write_text(source)
        subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory / 'functions.c'),
                        '-o', str(directory / 'functions.so')], check=True, capture_output=True)
        cls.library = C.CDLL(str(directory / 'functions.so'))
        cls.ram = (C.c_uint32 * (0x10000 // 4)).in_dll(cls.library, 'test_ram')
        cls.driver = Driver.in_dll(cls.library, 'test_driver')
        cls.alternate = Driver.in_dll(cls.library, 'test_alternate_driver')
        cls.current = C.c_void_p.in_dll(cls.library, 'D_800A6350')
        cls.handler_calls = C.c_int32.in_dll(cls.library, 'test_handler_calls')
        cls.handler = C.cast(cls.library.func_8008DDAC, C.c_void_p).value
        cls.initialize = cls.library.func_8008D820
        cls.initialize.argtypes = []
        cls.initialize.restype = None
        cls.reset = cls.library.func_8008B364
        cls.reset.argtypes = []
        cls.reset.restype = None
        cls.install = cls.library.func_8008B394
        cls.install.argtypes = [C.c_int32, C.c_void_p]
        cls.install.restype = C.c_void_p

    def setUp(self):
        C.memset(C.addressof(self.ram), 0xA5, C.sizeof(self.ram))
        self.events = []
        self.callbacks = []
        self.handler_calls.value = 0
        self.current.value = C.addressof(self.driver)
        self.set_reset(self.driver, lambda: self.events.append(('reset', bytes(self.ram))))
        self.set_install(self.driver, lambda channel, handler: self.events.append(('install', channel, handler, bytes(self.ram))))
        reserved = Reset(lambda: self.events.append('unexpected reserved callback'))
        self.callbacks.append(reserved)
        self.driver.reserved = C.cast(reserved, C.c_void_p).value
        self.driver.exchange = None

    def set_reset(self, driver, function):
        callback = Reset(function)
        self.callbacks.append(callback)
        driver.reset = C.cast(callback, C.c_void_p).value

    def set_install(self, driver, function):
        callback = Install(function)
        self.callbacks.append(callback)
        driver.install = C.cast(callback, C.c_void_p).value

    def cleared(self):
        expected = bytearray(self.ram)
        for address in (0x6488, 0x648C, 0x6494, 0x6498):
            expected[address:address + 4] = bytes(4)
        return bytes(expected)

    def test_clear_precedes_reset_and_registration(self):
        expected = self.cleared()
        self.initialize()
        self.assertEqual(bytes(self.ram), expected)
        self.assertEqual(self.events, [('reset', expected), ('install', 2, self.handler, expected)])
        self.assertEqual(self.handler_calls.value, 0)

    def test_reset_changes_survive_and_new_driver_is_used_for_registration(self):
        expected = bytearray(self.cleared())
        expected[0x6488:0x648C] = (0xFACE).to_bytes(4, 'little')
        def reset():
            self.events.append(('reset', bytes(self.ram)))
            self.ram[0x6488 // 4] = 0xFACE
            self.current.value = C.addressof(self.alternate)
        self.set_reset(self.driver, reset)
        self.set_install(self.alternate, lambda channel, handler: self.events.append(('alternate', channel, handler)))
        before_reset = self.cleared()
        self.initialize()
        self.assertEqual(self.events, [('reset', before_reset), ('alternate', 2, self.handler)])
        self.assertEqual(bytes(self.ram), bytes(expected))
        self.assertEqual(self.handler_calls.value, 0)

    def test_install_wrapper_forwards_both_arguments_without_state_changes(self):
        before = bytes(self.ram)
        for channel in (-2147483648, -1, 0, 2, 2147483647):
            for handler in (None, self.handler):
                self.events.clear()
                self.install(channel, handler)
                self.assertEqual(self.events, [('install', channel, handler, before)])
                self.assertEqual(bytes(self.ram), before)
        self.assertEqual(self.handler_calls.value, 0)

    def test_install_wrapper_returns_previous_native_callback(self):
        self.set_install(self.driver, lambda channel, handler: self.handler)
        self.assertEqual(self.install(2, None), self.handler)

    def test_reset_wrapper_uses_current_driver(self):
        self.set_reset(self.alternate, lambda: self.events.append('alternate reset'))
        self.current.value = C.addressof(self.alternate)
        before = bytes(self.ram)
        self.reset()
        self.assertEqual(self.events, ['alternate reset'])
        self.assertEqual(bytes(self.ram), before)


if __name__ == '__main__':
    unittest.main()
