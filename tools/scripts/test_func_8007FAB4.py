"""Exercise transfer sequencing on the host; fd.py checks target code.

The host build replaces fixed-width typedefs and removes the empty MIPS
register-clobber annotation. It retains the recovered algorithm.
"""
import ctypes as C
from pathlib import Path
import subprocess
from tempfile import TemporaryDirectory
import unittest


Validate = C.CFUNCTYPE(C.c_int32, C.c_uint32)
Transfer = C.CFUNCTYPE(None, C.c_int32, C.c_uint32, C.c_void_p, C.c_uint32)
Wait = C.CFUNCTYPE(C.c_int32, C.c_int32)


class ChunkedTransferTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        directory = Path(cls.temp.name)
        root = Path(__file__).resolve().parents[2]
        source = (root / 'src/main/PAL/func_8007FAB4.c').read_text()
        source = source.replace('#include "types.h"', '''#include <stdint.h>
#include <stdarg.h>
typedef uint8_t u8;
typedef uint32_t u32;
typedef int32_t s32;''')
        source = source.replace('__asm__ volatile("" : : : "$2");', '')
        source += '''
s32 D_800A47F4, D_800A4800, D_800A4840;
volatile s32 D_800A4810;
u32 D_800A48C8[10];
u8 D_800A4938[0x400];
s32 (*test_validate)(u32);
void (*test_transfer)(s32, u32, void *, u32);
s32 (*test_wait)(s32);
s32 func_8007FCD4(u32 address) { return test_validate(address); }
s32 func_80096E94(s32 event) { return test_wait(event); }
s32 func_8007F068(s32 mode, ...) {
    va_list args;
    u32 address = 0, size = 0;
    void *buffer = 0;
    va_start(args, mode);
    if (mode == 2) address = va_arg(args, u32);
    if (mode == 3) {
        buffer = va_arg(args, u8 *);
        size = va_arg(args, u32);
    }
    va_end(args);
    test_transfer(mode, address, buffer, size);
    return -2; /* These return values are intentionally ignored. */
}
'''
        (directory / 'function.c').write_text(source)
        subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory / 'function.c'),
                        '-o', str(directory / 'function.so')], check=True, capture_output=True)
        cls.library = C.CDLL(str(directory / 'function.so'))
        cls.function = cls.library.func_8007FAB4
        cls.function.argtypes = [C.c_uint32]
        cls.function.restype = C.c_int32

    def global_int(self, name):
        return C.c_int32.in_dll(self.library, name)

    def check_transfer(self, preset=1, address=0xFF00, mode=1, callback=123,
                       validation=0, mutate_state=False, validated_address=None):
        table = (C.c_uint32 * 10).in_dll(self.library, 'D_800A48C8')
        table[:] = [address] * 10
        self.global_int('D_800A4800').value = 3
        self.global_int('D_800A47F4').value = mode
        self.global_int('D_800A4810').value = callback
        self.global_int('D_800A4840').value = 0x123456
        buffer = (C.c_uint8 * 0x400).in_dll(self.library, 'D_800A4938')
        buffer[:] = [i & 255 for i in range(0x400)]
        before_buffer = bytes(buffer)
        calls, observed_state = [], []

        @Validate
        def validate(argument):
            calls.append(('validate', argument))
            if validated_address is not None:
                table[preset] = validated_address
            return validation

        @Transfer
        def transfer(command, destination, pointer, size):
            calls.append(('transfer', command, destination, pointer, size))
            observed_state.append((self.global_int('D_800A47F4').value,
                                   self.global_int('D_800A4810').value))

        @Wait
        def wait(event):
            calls.append(('wait', event))
            if mutate_state:
                self.global_int('D_800A47F4').value = 55
                self.global_int('D_800A4810').value = 66
            return -1

        for name, function in [('test_validate', validate), ('test_transfer', transfer),
                               ('test_wait', wait)]:
            C.c_void_p.in_dll(self.library, name).value = C.cast(function, C.c_void_p).value

        result = self.function(preset)
        if preset >= 10 or validation != 0:
            self.assertEqual(result, -1)
            self.assertEqual(calls, [] if preset >= 10 else [('validate', address)])
            self.assertEqual(self.global_int('D_800A47F4').value, mode)
            self.assertEqual(self.global_int('D_800A4810').value, callback)
        else:
            self.assertEqual(result, 0)
            start = 0xFFF0 if preset == 0 else (
                address if validated_address is None else validated_address)
            start *= 8
            length = 0x80000 - start
            # Independent interval partition: even an empty interval is submitted once.
            segments = [(start + offset, min(0x400, length - offset))
                        for offset in range(0, length, 0x400)] or [(start, 0)]
            expected = [('validate', address)]
            for destination, size in segments:
                expected.extend([('transfer', 2, destination, None, 0),
                                 ('transfer', 1, 0, None, 0),
                                 ('transfer', 3, 0, C.addressof(buffer), size),
                                 ('wait', 0x123456)])
            self.assertEqual(calls, expected)
            self.assertEqual(observed_state[:3], [(0 if mode == 1 else mode, 0)] * 3)
            expected_mode = mode if mode == 1 or not mutate_state else 55
            expected_callback = callback if callback != 0 or not mutate_state else 66
            self.assertEqual(self.global_int('D_800A47F4').value, expected_mode)
            self.assertEqual(self.global_int('D_800A4810').value, expected_callback)
        self.assertEqual(bytes(buffer), before_buffer)

    def test_invalid_presets_and_validation_failure(self):
        for preset in (10, 11, 0xFFFFFFFF):
            self.check_transfer(preset=preset)
        for validation in (-1, 1, 2, -(1 << 31), (1 << 31) - 1):
            self.check_transfer(validation=validation)

    def test_chunk_boundaries_and_all_presets(self):
        for length in (0, 8, 0x3F8, 0x400, 0x408, 0x800, 0x808, 0x80000):
            self.check_transfer(address=0x10000 - length // 8)
        for preset in range(10):
            self.check_transfer(preset=preset)

    def test_state_restoration_after_helpers_change_globals(self):
        for mode in (0, 1, 2, -1):
            for callback in (0, 1, -1, -(1 << 31)):
                self.check_transfer(mode=mode, callback=callback, mutate_state=True)

    def test_start_address_is_reloaded_after_validation(self):
        self.check_transfer(address=0xFFFF, validated_address=0xFEFF)


if __name__ == '__main__':
    unittest.main()
