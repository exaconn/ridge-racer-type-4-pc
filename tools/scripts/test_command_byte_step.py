"""Actual command byte step: callbacks, fresh state and captured MMIO reads."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

from test_serial_byte_exchange import Port

Poll = C.CFUNCTYPE(C.c_int32)
Unary = C.CFUNCTYPE(C.c_int32, C.c_int32)
Status = C.CFUNCTYPE(C.c_uint16, C.c_void_p)
Data = C.CFUNCTYPE(C.c_uint8, C.c_void_p)


class CommandByteStepTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        root = Path(__file__).resolve().parents[2]
        headers = ''
        for name in ('commands', 'serial'):
            text = (root / ('include/rr/' + name + '.h')).read_text()
            text = re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n', '', text, flags=re.M)
            text = text.replace(', D_80114730, D_80114870', ', D_80114870')
            headers += text
        source = '''#include <stdint.h>
#include <string.h>
typedef uint8_t u8;typedef uint16_t u16;typedef uint32_t u32;typedef int32_t s32;
s32 test_page[0x4000];
#define D_80114730 test_page[0x4730/4]
#define RR_NOP_DEP_ON(value) ((void)(value))
''' + headers + '''
s32 D_800ABDE4,D_800ABDE8,D_800ABDEC,D_800ABDF4,D_800ABDF8;
s32 D_80114728,D_80114870;
u8 D_80114740[160],D_801147D8[160];
RrCommandReadCallback D_80114734;
RrCommandWriteCallback D_80114738;
RrSerialPortRegisters test_ports[2];
RrSerialPortRegisters *volatile D_800ABDE0;
s32 (*test_select)(s32),(*test_exchange)(s32);
u16 (*test_status)(void *);u8 (*test_data)(void *);
s32 (*test_subtype)(void);
s32 func_8009BD44(s32 slot) {return test_select(slot);}
s32 func_8009BE18(s32 byte) {return test_exchange(byte);}
u16 test_status_read(RrSerialPortRegisters *p) {return test_status(p);}
u8 test_data_read(RrSerialPortRegisters *p) {return test_data(p);}
void test_reset(void) {
 memset(test_page,0xA5,sizeof(test_page));
 memset(D_80114740,0xA5,sizeof(D_80114740));
 memset(D_801147D8,0xA5,sizeof(D_801147D8));
 memset(test_ports,0,sizeof(test_ports));
 D_800ABDE4=3;D_800ABDE8=2;D_800ABDEC=0x123;
 D_800ABDF4=4;D_800ABDF8=6;D_80114730=9;
 D_80114728=0;D_80114870=0;
 test_ports[0].data=0xAB;test_ports[0].status=2;
 test_ports[1].data=0xCD;test_ports[1].status=2;
 D_800ABDE0=&test_ports[0];
}
'''
        body = (root/'src/main/PAL/func_8009C0F4.c').read_text()
        body = re.sub(r'^#include[^\n]*\n', '', body, flags=re.M)
        body = re.sub(r'\s+__asm__\("\$[0-9]+"\)', '', body)
        body = re.sub(r'"\$[0-9]+",\s*', '', body)
        body = re.sub(r':\s*"\$[0-9]+"(?:,\s*"\$[0-9]+")*(?=\))', '', body)
        replacements = {'(s32 *)0x80110000U': ('test_page', 1),
                        'port->status': ('test_status_read(port)', 1),
                        'port->data': ('test_data_read(port)', 1),
                        'D_80114870': ('test_subtype()', 2)}
        for old, (new, count) in replacements.items():
            assert body.count(old) == count, old
            body = body.replace(old, new)
        directory = Path(cls.temp.name)
        (directory/'function.c').write_text(source + body)
        result = subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory/'function.c'),
                                 '-o', str(directory/'function.so')], capture_output=True)
        if result.returncode:
            raise AssertionError(result.stderr.decode())
        cls.lib = C.CDLL(str(directory/'function.so'))
        cls.page = (C.c_int32 * 0x4000).in_dll(cls.lib, 'test_page')
        cls.rx = (C.c_uint8 * 160).in_dll(cls.lib, 'D_80114740')
        cls.tx = (C.c_uint8 * 160).in_dll(cls.lib, 'D_801147D8')
        cls.ports = (Port * 2).in_dll(cls.lib, 'test_ports')
        cls.step = cls.lib.func_8009C0F4
        cls.step.argtypes = []
        cls.step.restype = C.c_int32

    def setUp(self):
        self.lib.test_reset()
        self.callbacks = []
        self.errors = []
        self.install('test_select', Unary, lambda slot: 0)
        self.install('test_exchange', Unary, lambda byte: 0x82)
        self.install('D_80114734', Poll, lambda: 0x81)
        self.install('D_80114738', Unary, lambda byte: 0)
        self.install('test_status', Status, lambda address: 2)
        self.install('test_data', Data, lambda address: C.cast(address, C.POINTER(Port)).contents.data)
        self.install('test_subtype', Poll, lambda: self.get('D_80114870'))

    def install(self, name, kind, action):
        def checked(*args):
            try:
                return action(*args)
            except BaseException as error:
                self.errors.append(error)
                return 2  # Polling can always terminate before rethrowing.
        callback = kind(checked)
        self.callbacks.append(callback)
        C.c_void_p.in_dll(self.lib, name).value = C.cast(callback, C.c_void_p).value

    def get(self, name):
        if name == 'D_80114730':
            return self.page[0x4730//4]
        return C.c_int32.in_dll(self.lib, name).value

    def put(self, name, value):
        if name == 'D_80114730':
            self.page[0x4730//4] = value
        else:
            C.c_int32.in_dll(self.lib, name).value = value

    def run_step(self):
        result = self.step()
        if self.errors:
            raise self.errors[0]
        self.assertEqual(self.page[0x4730//4-1], C.c_int32(0xA5A5A5A5).value)
        self.assertEqual(self.page[0x4730//4+1], C.c_int32(0xA5A5A5A5).value)
        self.assertEqual(bytes(self.rx[152:]), b'\xA5'*8)
        self.assertEqual(bytes(self.tx[152:]), b'\xA5'*8)
        return result

    def forbidden(self, *args):
        self.fail('unexpected device/callback access')

    def test_initial_select_full_request_then_fresh_header_and_count(self):
        for request in (-0x80000000, -1, 0, 0x7FFFFFFF, 0x12345678):
            for nibble in range(16):
                self.setUp()
                self.put('D_800ABDE8', 1)
                self.put('D_800ABDEC', request)
                self.put('D_800ABDF4', 0x7FFFFFFF)
                events = []
                def select(slot):
                    events.append(('select', slot))
                    self.assertEqual(slot, request >> 4)
                    self.assertEqual(self.get('D_800ABDE4'), 4)
                    self.assertEqual(self.get('D_80114730'), 9)
                    self.put('D_800ABDEC', 0x2340 | nibble)
                    return -0x80000000
                def exchange(byte):
                    events.append(('exchange', byte))
                    self.assertEqual(byte, ~(0x81+nibble))
                    self.put('D_800ABDEC', 0x12345670 | (15-nibble))
                    return -1
                self.install('test_select', Unary, select)
                self.install('test_exchange', Unary, exchange)
                self.install('D_80114734', Poll, self.forbidden)
                self.install('D_80114738', Unary, self.forbidden)
                C.c_void_p.in_dll(self.lib, 'D_800ABDE0').value = None
                self.assertEqual(self.run_step(), 0)
                self.assertEqual(len(events), 2)
                self.assertEqual(bytes(self.tx), bytes([0x81+15-nibble])+b'\xA5'*159)
                self.assertEqual(bytes(self.rx), b'\xA5'*160)
                self.assertEqual(self.get('D_800ABDF4'), -0x80000000)

    def test_each_read_and_receive_byte_with_fresh_callback_state(self):
        for byte in range(256):
            self.setUp()
            def reader():
                self.assertEqual(self.get('D_80114730'), 8)
                self.put('D_800ABDF4', 9)
                return C.c_int32(0x80000000 | byte).value
            def exchange(argument):
                self.assertEqual(argument, byte)
                self.assertEqual(self.tx[9], byte)
                self.assertEqual(self.get('D_800ABDF4'), 10)
                self.put('D_800ABDF8', 12)
                self.install('D_80114738', Unary, writer)
                return 0x12340000 | (255-byte)
            def writer(argument):
                self.assertEqual(argument, 255-byte)
                self.assertEqual(self.rx[12], 255-byte)
                self.assertEqual(self.get('D_800ABDF8'), 13)
                return 0
            self.install('D_80114734', Poll, reader)
            self.install('test_exchange', Unary, exchange)
            self.install('D_80114738', Unary, self.forbidden)
            self.install('test_status', Status, self.forbidden)
            self.assertEqual(self.run_step(), 0)
            self.assertEqual(bytes(self.tx), b'\xA5'*9+bytes([byte])+b'\xA5'*150)
            self.assertEqual(bytes(self.rx), b'\xA5'*12+bytes([255-byte])+b'\xA5'*147)

    def test_signed_error_returns_and_wrapped_remaining_count(self):
        for remaining in (-0x80000000, -1, 0, 1, 2, 0x7FFFFFFF):
            for result in (-0x80000000, -2001, -1):
                self.setUp()
                self.put('D_80114730', remaining)
                self.install('test_exchange', Unary, lambda byte: result)
                self.install('D_80114738', Unary, self.forbidden)
                self.assertEqual(self.run_step(), C.c_int32(result-1000).value)
                self.assertEqual(self.get('D_80114730'), C.c_int32(remaining-1).value)
                self.assertEqual(bytes(self.rx), b'\xA5'*160)
        for result in (-0x80000000, -1, 1, 2000, 0x7FFFFFFF):
            self.setUp()
            self.install('D_80114738', Unary, lambda byte: result)
            self.assertEqual(self.run_step(), C.c_int32(result-2000).value)
            self.assertEqual(self.get('D_800ABDF8'), 7)

    def test_last_byte_skip_wait_subtypes_and_all_output_bytes(self):
        for subtype in (14, 15):
            for byte in range(256):
                self.setUp()
                calls = []
                self.put('D_80114870', subtype)
                self.put('D_80114728', 3)
                self.ports[0].data = byte
                def writer(value):
                    calls.append(value)
                    self.assertEqual(self.get('D_800ABDF8'), 6+len(calls))
                    self.put('D_80114730', 1 if len(calls) == 1 else 99)
                    return 0
                self.install('D_80114738', Unary, writer)
                self.install('test_status', Status, self.forbidden)
                self.assertEqual(self.run_step(), 1)
                self.assertEqual(calls, [0x82, byte])
                self.assertEqual(self.get('D_800ABDE8'), 3)
                self.assertEqual(self.get('D_80114730'), 99)
                self.assertEqual(bytes(self.rx), b'\xA5'*6+bytes([0x82,byte])+b'\xA5'*152)

    def test_wait_captures_port_then_final_read_reloads_pointer_and_index(self):
        for subtype in (-0x80000000, -1, 0, 13, 14, 15, 16, 0x7FFFFFFF):
            for final_result in (0, -0x80000000, -1, 1, 3000, 0x7FFFFFFF):
                self.setUp()
                self.put('D_80114870', subtype)
                self.put('D_800ABDE8', 0x7FFFFFFF)
                calls, statuses, types = [], [], []
                initial = C.addressof(self.ports)
                second = initial+C.sizeof(Port)
                def writer(value):
                    calls.append(value)
                    if len(calls) == 1:
                        self.put('D_80114730', 1)
                        self.install('D_80114738', Unary, final_writer)
                    return 0
                def final_writer(value):
                    calls.append(value)
                    self.assertEqual(self.get('D_800ABDF8'), 21)
                    self.assertEqual(self.rx[20], 0xCD)
                    return final_result
                def status(address):
                    statuses.append(address)
                    self.assertEqual(address, initial)
                    C.c_void_p.in_dll(self.lib, 'D_800ABDE0').value = second
                    return 2 if len(statuses) == 3 else 0
                def data(address):
                    self.assertEqual(address, second)
                    self.assertEqual(self.get('D_800ABDE8'), -0x80000000)
                    self.assertEqual(self.get('D_80114730'), 0)
                    self.put('D_800ABDF8', 20)
                    return 0xCD
                def read_type():
                    types.append(1)
                    return subtype
                self.install('D_80114738', Unary, writer)
                self.install('test_status', Status, status)
                self.install('test_data', Data, data)
                self.install('test_subtype', Poll, read_type)
                self.assertEqual(self.run_step(), C.c_int32(final_result-3000).value if final_result else 1)
                self.assertEqual(calls, [0x82, 0xCD])
                self.assertEqual(statuses, [initial]*3)
                self.assertEqual(len(types), 1 if subtype == 14 else 2)

    def test_separate_subtype_read_and_wrapped_special_length(self):
        self.put('D_80114728', -4)
        types, calls = [], []
        def subtype():
            types.append(1)
            return 0 if len(types) == 1 else 15
        def writer(value):
            calls.append(value)
            self.put('D_80114730', 1)
            self.put('D_800ABDF8', 0)
            return 0
        self.install('D_80114738', Unary, writer)
        self.install('test_subtype', Poll, subtype)
        self.install('test_status', Status, self.forbidden)
        self.assertEqual(self.run_step(), 1)
        self.assertEqual(calls, [0x82, 0xAB])
        self.assertEqual(len(types), 2)
        self.assertEqual(self.rx[0], 0xAB)
