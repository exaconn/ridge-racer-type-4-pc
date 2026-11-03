"""Host behavior checks; fd.py checks the target instruction sequence."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest


class SerialRequestTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        directory = Path(cls.temp.name)
        root = Path(__file__).resolve().parents[2]
        header = (root / 'include/rr/serial.h').read_text()
        header = re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n', '', header, flags=re.M)
        source = """#include <stdint.h>
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int32_t s32;
""" + header
        for name in ('8009A94C', '8009ABF8'):
            body = (root / ('build/legacy/func_' + name + '.c')).read_text()
            source += body.replace('#include "rr/serial.h"', '')
        source += '\nvolatile RrSerialRequest D_801146E8;\nRrSerialReceiveRequest D_801146D8;\nRrSerialPortRegisters *D_800ABD18;\n'
        (directory / 'function.c').write_text(source)
        subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory / 'function.c'),
                        '-o', str(directory / 'function.so')], check=True, capture_output=True)
        cls.library = C.CDLL(str(directory / 'function.so'))
        cls.function = cls.library.func_8009A94C
        cls.function.argtypes = [C.c_int32, C.c_int32]
        cls.function.restype = C.c_int32
        cls.receive = cls.library.func_8009ABF8
        cls.receive.argtypes = [C.c_int32, C.c_int32]
        cls.receive.restype = C.c_int32

    def setUp(self):
        self.request = (C.c_int32 * 3).in_dll(self.library, 'D_801146E8')
        self.receive_request = (C.c_int32 * 4).in_dll(self.library, 'D_801146D8')
        self.registers = (C.c_uint16 * 8)(*range(8))
        C.c_void_p.in_dll(self.library, 'D_800ABD18').value = C.addressof(self.registers)

    def test_busy_request_preserves_state_without_accessing_registers(self):
        C.c_void_p.in_dll(self.library, 'D_800ABD18').value = None
        for active in (1, 2, -1, -(1 << 31), (1 << 31) - 1):
            self.request[:] = [active, 123, -456]
            self.assertEqual(self.function(789, -987), -1)
            self.assertEqual(list(self.request), [active, 123, -456])

    def test_parameter_boundaries_and_preservation_of_other_registers(self):
        for first in (-(1 << 31), -1, 0, 1, (1 << 31) - 1):
            for second in (-(1 << 31), -1, 0, 1, (1 << 31) - 1):
                self.request[:] = [0, 123, 456]
                self.registers[:] = range(8)
                self.assertEqual(self.function(first, second), 0)
                self.assertEqual(list(self.request), [1, first, second])
                self.assertEqual(list(self.registers), [0, 1, 2, 3, 4, 0x825, 6, 7])

    def test_busy_receive_preserves_request_and_does_not_access_port(self):
        C.c_void_p.in_dll(self.library, 'D_800ABD18').value = None
        for active in (1, 2, -1, -(1 << 31), (1 << 31) - 1):
            self.receive_request[:] = [active, 123, -456, 128]
            self.assertEqual(self.receive(789, -987), -1)
            self.assertEqual(list(self.receive_request), [active, 123, -456, 128])

    def test_receive_parameters_and_status_capture(self):
        for first in (-(1 << 31), -1, 0, 1, (1 << 31) - 1):
            for second in (-(1 << 31), -1, 0, 1, (1 << 31) - 1):
                for status in (0, 0x80, 0xFF7F, 0xFFFF):
                    self.receive_request[:] = [0, 123, 456, 987]
                    self.registers[:] = range(8)
                    self.registers[2] = status
                    self.assertEqual(self.receive(first, second), 0)
                    self.assertEqual(list(self.receive_request), [1, first, second, status & 0x80])
                    self.assertEqual(list(self.registers), [0, 1, status, 3, 4, 0x405, 6, 7])

    def test_receive_all_initial_status_and_control_values(self):
        for control in range(0x10000):
            status = control ^ 0xA55A
            self.receive_request[:] = [0, -1, -2, -3]
            self.registers[:] = range(8)
            self.registers[2] = status
            self.registers[5] = control
            self.assertEqual(self.receive(12, 34), 0)
            self.assertEqual(list(self.receive_request), [1, 12, 34, status & 128])
            self.assertEqual(list(self.registers), [0, 1, status, 3, 4, control | 0x400, 6, 7])

    def test_all_initial_control_register_values(self):
        for control in range(0x10000):
            self.request[0] = 0
            self.registers[5] = control
            self.assertEqual(self.function(12, 34), 0)
            self.assertEqual(self.registers[5], control | 0x820)


if __name__ == '__main__':
    unittest.main()
