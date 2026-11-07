"""Actual signed command builder with the real zero/reset callee and observed sink."""
import ctypes as C
from pathlib import Path
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

Reset = C.CFUNCTYPE(None)
Sink = C.CFUNCTYPE(C.c_int32, C.c_void_p)


class SignedLibraryCommandTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        root = Path(__file__).resolve().parents[2]
        directory = Path(cls.temp.name)
        header = (root / 'include/rr/library.h').read_text()
        header = re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n', '', header, flags=re.M)
        source = '''#include <stdint.h>
#include <string.h>
typedef uint8_t u8; typedef int8_t s8;
typedef uint16_t u16; typedef int16_t s16;
typedef uint32_t u32; typedef int32_t s32;
''' + header + '''
s32 test_command_page[16384];
#define _svm_rattr (test_command_page+2)
s32 (*test_sink)(s32 *);
void (*test_reset_hook)(void);
s32 SpuSetReverbModeParam(s32 *command) {return test_sink(command);}
s32 test_unexpected_probe_calls;
s32 _SpuIsInAllocateArea_(s32 ignored) {test_unexpected_probe_calls++;return 0;}
'''
        reset = (root / 'tools/scripts/fixtures/views/func_80086694.c').read_text()
        reset = reset.replace('SpuSetReverb', 'test_actual_reset')
        source += reset + '''
struct S test_device;
struct S *_spu_RXX=&test_device;
int _spu_rev_flag, _spu_rev_reserve_wa, _spu_rev_offsetaddr;
u16 *test_device_flags=&test_device.f;
s32 SpuSetReverb(s32 value) {test_reset_hook();return test_actual_reset(value);}
void test_initialize(void) {
 memset(test_command_page,0xA5,64);test_device.f=0xFFFF;_spu_rev_flag=99;
 _spu_rev_reserve_wa=7;_spu_rev_offsetaddr=-123;test_unexpected_probe_calls=0;
}
'''
        body = (root / 'tools/scripts/fixtures/views/func_800865E4.c').read_text()
        body = body.replace('#include "rr/library.h"', '')
        body = body.replace('extern int _svm_rattr[];', '')
        if body.count('(s32 *)0x80110000U') != 1:
            raise AssertionError('Command page adapter must cover exactly one fixed page')
        body = body.replace('(s32 *)0x80110000U', 'test_command_page')
        body = re.sub(r'\s+__asm__\("\$[0-9]+"\)', '', body)
        body = re.sub(r':\s*"\$[0-9]+"(?:,\s*"\$[0-9]+")*', '', body)
        source += body
        (directory / 'command.c').write_text(source)
        result = subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory / 'command.c'),
                                 '-o', str(directory / 'command.so')], capture_output=True)
        if result.returncode:
            raise AssertionError(result.stderr.decode())
        cls.lib = C.CDLL(str(directory / 'command.so'))
        cls.command = (C.c_ubyte * 64).in_dll(cls.lib, 'test_command_page')
        cls.state = C.c_int32.in_dll(cls.lib, '_spu_rev_flag')
        cls.probes = C.c_int32.in_dll(cls.lib, 'test_unexpected_probe_calls')
        flags = C.c_void_p.in_dll(cls.lib, 'test_device_flags').value
        cls.flags = C.c_uint16.from_address(flags)
        cls.function = cls.lib.SsUtSetReverbType
        cls.function.argtypes = [C.c_int32]
        cls.function.restype = C.c_int32

    def setUp(self):
        self.events = []
        self.reset_hook = Reset(lambda: self.events.append(('reset', bytes(self.command), self.state.value, self.flags.value)))
        self.sink_hook = Sink(self.observe_sink)
        C.c_void_p.in_dll(self.lib, 'test_reset_hook').value = C.cast(self.reset_hook, C.c_void_p).value
        C.c_void_p.in_dll(self.lib, 'test_sink').value = C.cast(self.sink_hook, C.c_void_p).value

    def observe_sink(self, pointer):
        self.events.append(('sink', pointer, bytes(self.command), self.state.value, self.flags.value))
        return -1

    def check_value(self, raw):
        self.lib.test_initialize()
        self.events.clear()
        low = C.c_int16(raw).value
        magnitude = abs(low)
        before = bytes(self.command)
        expected = bytearray(before)
        if magnitude < 10:
            struct.pack_into('=ii', expected, 8, 1, magnitude | (0x100 if low < 0 else 0))
        result = self.function(C.c_int32(raw).value)
        if magnitude >= 10:
            self.assertEqual(result, -1)
            self.assertEqual(self.events, [])
            self.assertEqual((self.state.value, self.flags.value), (99, 0xFFFF))
        else:
            self.assertEqual(result, magnitude)
            events = []
            if magnitude == 0:
                events.append(('reset', bytes(expected), 99, 0xFFFF))
            events.append(('sink', C.addressof(self.command)+8, bytes(expected),
                           0 if magnitude == 0 else 99, 0xFF7F if magnitude == 0 else 0xFFFF))
            self.assertEqual(self.events, events)
        self.assertEqual(bytes(self.command), bytes(expected))
        self.assertEqual(self.probes.value, 0)

    def test_every_low_halfword_and_signed_boundaries(self):
        for value in range(65536):
            self.check_value(0x76540000 | value)

    def test_high_register_bits_preserve_magnitude_direction_and_zero_reset(self):
        for high in (0, 1, 0x7FFF, 0x8000, 0xFFFF):
            for low in (0, 1, 9, 10, 32767, 32768, 65526, 65527, 65535):
                self.check_value((high << 16) | low)

    def test_dispatch_changes_do_not_replace_the_saved_return_value(self):
        self.lib.test_initialize()
        def sink(pointer):
            self.events.append(('sink', pointer))
            C.c_int32.from_address(pointer+4).value = -12345
            self.state.value = 72
            return -999
        self.sink_hook = Sink(sink)
        C.c_void_p.in_dll(self.lib, 'test_sink').value = C.cast(self.sink_hook, C.c_void_p).value
        self.assertEqual(self.function(-9), 9)
        self.assertEqual(self.events, [('sink', C.addressof(self.command)+8)])
        self.assertEqual(C.c_int32.from_buffer(self.command, 12).value, -12345)
        self.assertEqual(self.state.value, 72)


if __name__ == '__main__':
    unittest.main()
