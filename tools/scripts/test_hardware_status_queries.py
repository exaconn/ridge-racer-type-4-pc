"""Actual status predicates with MMIO access adapters and an unchanged RAM run."""
import ctypes as C
from pathlib import Path
import random
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

# Operation ids are part of this fixture's bus model, not game constants.
READ_DATA, READ_CONTROL, WRITE_DATA, WRITE_CONTROL, READ_MASK, READ_PENDING, READ_SERIAL_STATUS, READ_SERIAL_CONTROL, WRITE_SERIAL_CONTROL = range(1, 10)


class HardwareStatusQueriesTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        root = Path(__file__).resolve().parents[2]
        directory = Path(cls.temp.name)
        headers = ''
        for name in ('gpu.h', 'interrupts.h', 'serial.h'):
            header = (root / 'include/rr' / name).read_text()
            headers += re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n', '', header, flags=re.M)
        source = '''#include <stdint.h>
#include <string.h>
typedef uint8_t u8; typedef int8_t s8;
typedef uint16_t u16; typedef int16_t s16;
typedef uint32_t u32; typedef int32_t s32;
''' + headers + '''
RrInterruptRegisters test_irq;
RrInterruptRegisters *I_STAT_PadVsync=&test_irq, *I_STAT_Card=&test_irq, *I_STAT_Sio=&test_irq;
RrSerialReceiveRequest SioReceiveRequest;
RrSerialPortRegisters test_serial;
RrSerialPortRegisters *SIO1_REGS=&test_serial;
u32 test_info, test_control, test_readback;
u32 test_last_data, test_last_control, test_count;
u32 test_log[16][2];
u32 test_ram[2];
volatile u32 *GPU_DATA=&test_ram[0], *GPU_STATUS=&test_ram[1];
static void test_record(u32 operation,u32 value) {
 if(test_count<16) {test_log[test_count][0]=operation; test_log[test_count][1]=value;}
 test_count++;
}
u32 test_read_data(void) {
 u32 value;
 /* The first data read answers the query; a later read acknowledges publication. */
 value=test_count==1 ? test_info : test_readback;
 test_record(1,value); return value;
}
u32 test_read_control(void) { test_record(2,test_control); return test_control; }
void test_write_data(u32 value) { test_last_data=value; test_record(3,value); }
void test_write_control(u32 value) { test_last_control=value; test_record(4,value); }
u32 test_read_mask(void) { u32 value=test_irq.mask; test_record(5,value); return value; }
u32 test_read_pending(void) { u32 value=test_irq.pending; test_record(6,value); return value; }
u32 test_serial_status(void) {u32 v=test_serial.status;test_record(7,v);return v;}
u32 test_serial_control(void) {u32 v=test_serial.control;test_record(8,v);return v;}
void test_serial_write(u32 value) {test_record(9,value);test_serial.control=value;}
void test_reset(u32 info,u32 control,u32 readback) {
 test_info=info; test_control=control; test_readback=readback;
 test_last_data=test_last_control=0xA5A5A5A5U; test_count=0;
 memset(test_log,0xA5,sizeof(test_log));
 test_ram[0]=info; test_ram[1]=0xA5A5A5A5U;
}
'''
        for name in ('800954D8', '80097680', '8009BAD8', '8009BB18', '8009B380'):
            path = root / ('tools/scripts/fixtures/legacy/func_'+name+'.c')
            if name in ('8009BAD8', '8009BB18'):
                # Each actual definition is extracted from the shared, contiguous TU.
                tu = (root / 'tools/scripts/fixtures/legacy/interrupt_queries.c').read_text()
                start = tu.index('s32 ' + __import__('c_functions').name_at(int(name,16)|0x80000000) + '(')
                _m = re.search(r'\ns32 \w+\(', tu[start + 1:]); finish = start + 1 + _m.start() if _m else -1
                body = tu[start:finish if finish >= 0 else len(tu)]
            else:
                body = path.read_text()
            body = re.sub(r'^#include[^\n]*\n', '', body, flags=re.M)
            body = re.sub(r'\s+__asm__\("\$[0-9]+"\)', '', body)
            body = re.sub(r':\s*"\$[0-9]+"(?:,\s*"\$[0-9]+")*', '', body)
            # Retain a second, unchanged body that uses real volatile RAM accesses.
            native = body.replace(__import__('c_functions').name_at(int(name,16)|0x80000000), 'test_native_'+name)
            source += native
            if name == '800954D8':
                # Adapt only MMIO lvalues/reads; retain every condition, mask and return.
                body, ordinary = re.subn(r'\*\(u32 \*\)GPU_STATUS\s*=\s*([^;]+);',
                                         r'test_write_control(\1);', body)
                body, control = re.subn(r'\*GPU_STATUS\s*=\s*([^;]+);',
                                       r'test_write_control(\1);', body)
                body, data = re.subn(r'\*GPU_DATA\s*=\s*([^;]+);',
                                    r'test_write_data(\1);', body)
                if (ordinary, control, data) != (1, 1, 1):
                    raise AssertionError('MMIO adapter no longer covers the actual write sites')
                if body.count('*GPU_DATA') != 2 or body.count('*GPU_STATUS') != 1:
                    raise AssertionError('MMIO adapter no longer covers the actual read sites')
                body = body.replace('*GPU_DATA', 'test_read_data()')
                body = body.replace('*GPU_STATUS', 'test_read_control()')
            elif name == '8009B380':
                if body.count('interrupts->mask') != 1 or body.count('interrupts->pending') != 1:
                    raise AssertionError('Receive IRQ read sites changed')
                body = body.replace('interrupts->mask', 'test_read_mask()')
                body = body.replace('interrupts->pending', 'test_read_pending()')
                if body.count('port->status') != 1 or body.count('port->control') != 1:
                    raise AssertionError('Receive serial read/write sites changed')
                body = body.replace('port->status', 'test_serial_status()')
                body = body.replace('port->control &= 0xFBFF;',
                                    'test_serial_write(test_serial_control() & 0xFBFF);')
            else:
                if body.count('registers->mask') != 1 or body.count('registers->pending') != 1:
                    raise AssertionError('IRQ adapter no longer covers the actual read sites')
                body = body.replace('registers->mask', 'test_read_mask()')
                body = body.replace('registers->pending', 'test_read_pending()')
            source += body
        (directory / 'function.c').write_text(source)
        result = subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory / 'function.c'),
                                 '-o', str(directory / 'function.so')], capture_output=True)
        if result.returncode:
            raise AssertionError(result.stderr.decode())
        cls.lib = C.CDLL(str(directory / 'function.so'))
        cls.query, cls.native_query = cls.lib.GPU_CheckVersion, cls.lib.test_native_800954D8
        for function in (cls.query, cls.native_query):
            function.argtypes = [C.c_int32]
            function.restype = C.c_int32
        cls.irq_query, cls.native_irq = cls.lib._padVsyncHookCheck, cls.lib.test_native_80097680
        for function in (cls.irq_query, cls.native_irq):
            function.argtypes = []
            function.restype = C.c_int32
        cls.irq_variants = []
        for name, bit in (('8009BAD8', 1), ('8009BB18', 0x80)):
            query, native = getattr(cls.lib, __import__('c_functions').name_at(int(name,16)|0x80000000)), getattr(cls.lib, 'test_native_' + name)
            for function in (query, native):
                function.argtypes = []
                function.restype = C.c_int32
            cls.irq_variants.append((query, native, bit))
        cls.receive_query, cls.native_receive = cls.lib._sio_irq_check, cls.lib.test_native_8009B380
        for function in (cls.receive_query, cls.native_receive):
            function.argtypes = []
            function.restype = C.c_int32
        cls.serial = (C.c_uint8 * 16).in_dll(cls.lib, 'test_serial')
        cls.request = (C.c_int32 * 4).in_dll(cls.lib, 'SioReceiveRequest')
        cls.serial_pointer = C.c_void_p.in_dll(cls.lib, 'SIO1_REGS')
        cls.reset = cls.lib.test_reset
        cls.reset.argtypes = [C.c_uint32] * 3
        cls.irq = (C.c_uint32 * 2).in_dll(cls.lib, 'test_irq')
        cls.ram = (C.c_uint32 * 2).in_dll(cls.lib, 'test_ram')
        cls.count = C.c_uint32.in_dll(cls.lib, 'test_count')
        cls.log = (C.c_uint32 * 32).in_dll(cls.lib, 'test_log')
        cls.last_data = C.c_uint32.in_dll(cls.lib, 'test_last_data')
        cls.last_control = C.c_uint32.in_dll(cls.lib, 'test_last_control')

    def trace(self):
        count = self.count.value
        self.assertLessEqual(count, 16)
        self.assertEqual(list(self.log)[count * 2:], [0xA5A5A5A5] * (32 - count * 2))
        return [(self.log[2 * i], self.log[2 * i + 1]) for i in range(count)]

    def test_interrupt_mask_precedes_conditional_pending_read_and_preserves_words(self):
        edges = [0, 1, 2, 3, 0xFFFFFFFE, 0xFFFFFFFF, 0x80000000, 0x80000001]
        pairs = [(pending, mask) for pending in edges for mask in edges]
        rng = random.Random(0x97680)
        pairs += [(rng.getrandbits(32), rng.getrandbits(32)) for _ in range(2000)]
        for pending, mask in pairs:
            self.reset(0, 0, 0)
            self.irq[:] = pending, mask
            before = bytes(self.irq)
            expected = int(bool(pending & mask & 1))
            self.assertEqual(self.irq_query(), expected)
            trace = [(READ_MASK, mask)] + ([(READ_PENDING, pending)] if mask & 1 else [])
            self.assertEqual(self.trace(), trace)
            self.assertEqual(self.native_irq(), expected)
            self.assertEqual(bytes(self.irq), before)

    def test_command_interrupt_queries_read_mask_then_pending_for_selected_bit(self):
        edges = [0, 1, 2, 0x7F, 0x80, 0x81, 0xFF, 0xFFFFFF7F, 0xFFFFFFFE,
                 0xFFFFFFFF, 0x80000000, 0x80000080]
        pairs = [(pending, mask) for pending in edges for mask in edges]
        rng = random.Random(0x9BAD8)
        pairs += [(rng.getrandbits(32), rng.getrandbits(32)) for _ in range(2000)]
        for query, native, bit in self.irq_variants:
            for pending, mask in pairs:
                self.reset(0, 0, 0)
                self.irq[:] = pending, mask
                before = bytes(self.irq)
                expected = int(bool(pending & mask & bit))
                self.assertEqual(query(), expected)
                trace = [(READ_MASK, mask)] + ([(READ_PENDING, pending)] if mask & bit else [])
                self.assertEqual(self.trace(), trace)
                self.assertEqual(native(), expected)
                self.assertEqual(bytes(self.irq), before)

    def test_receive_query_irq_and_serial_access_order_and_control_bit_clear(self):
        pairs = [(0, 0), (0x100, 0), (0, 0x100), (0x100, 0x100),
                 (0xFFFFFFFF, 0xFFFFFFFF), (0xFFFFFFFF, 0xFFFFFEFF)]
        rng = random.Random(0x9B380)
        pairs += [(rng.getrandbits(32), rng.getrandbits(32)) for _ in range(300)]
        for pending, mask in pairs:
            for active in (0, 1, -1):
                for status in (0, 1, 2, 3, 0xFFFF):
                    control = rng.getrandbits(16)
                    self.reset(0, 0, 0)
                    self.irq[:] = pending, mask
                    self.request[:] = active, -123, 987, -1
                    C.memset(C.addressof(self.serial), 0xA5, 16)
                    C.c_uint16.from_buffer(self.serial, 4).value = status
                    C.c_uint16.from_buffer(self.serial, 10).value = control
                    self.serial_pointer.value = C.addressof(self.serial)
                    expected = int(bool(pending & mask & 0x100))
                    trace = [(READ_PENDING, pending), (READ_MASK, mask)]
                    before, request = bytes(self.serial), bytes(self.request)
                    after = bytearray(before)
                    if expected and active:
                        trace.append((READ_SERIAL_STATUS, status))
                        if status & 1:
                            trace += [(READ_SERIAL_CONTROL, control),
                                      (WRITE_SERIAL_CONTROL, control & 0xFBFF)]
                            after[10:12] = (control & 0xFBFF).to_bytes(2, 'little')
                    self.assertEqual(self.receive_query(), expected)
                    self.assertEqual(self.trace(), trace)
                    self.assertEqual(bytes(self.serial), bytes(after))
                    self.serial[:] = before
                    self.assertEqual(self.native_receive(), expected)
                    self.assertEqual(bytes(self.serial), bytes(after))
                    self.assertEqual(bytes(self.request), request)
                    self.assertEqual(list(self.irq), [pending, mask])

    def test_receive_query_does_not_access_port_without_active_enabled_interrupt(self):
        try:
            self.serial_pointer.value = None
            for pending, mask, active in ((0, 0x100, 1), (0x100, 0, 1),
                                           (0x100, 0x100, 0), (0x80, 0x80, -1)):
                self.irq[:] = pending, mask
                self.request[0] = active
                self.assertEqual(self.native_receive(), int(bool(pending & mask & 0x100)))
        finally:
            self.serial_pointer.value = C.addressof(self.serial)

    def test_other_gpu_versions_read_status_publish_fallback_and_acknowledge(self):
        for info in (0, 1, 3, 0xFFFFFF, 0x80000000, 0x7FFFFFFF, 0xFFFFFFFF):
            for status in (0, 1, 0x3FFF, 0x4000, 0xFFFF, 0xFFFFFFFF, 0x12345678):
                for mode in (-0x80000000, -1, 0, 1, 8, 255, 0x7FFFFFFF):
                    for readback in (0, 2, 0xFFFFFFFF):
                        self.reset(info, status, readback)
                        command = (status & 0x3FFF) | 0xE1001000
                        self.assertEqual(self.query(mode), 0)
                        self.assertEqual(self.trace(), [(WRITE_CONTROL, 0x10000007), (READ_DATA, info),
                                                        (READ_CONTROL, status), (WRITE_DATA, command),
                                                        (READ_DATA, readback)])
                        self.assertEqual(self.last_data.value, command)
                        self.assertEqual(self.last_control.value, 0x10000007)

    def test_gpu_version_two_masks_high_byte_and_uses_only_mode_bit_eight(self):
        cases = [(info, mode) for info in (2, 0x01000002, 0x80000002, 0xFF000002)
                 for mode in [-0x80000000, -1, *range(256), 0x12340000, 0x12340008, 0x7FFFFFFF]]
        cases += [((high << 24) | 2, mode) for high in range(256) for mode in (0, 8)]
        for info, mode in cases:
            self.reset(info, 0xFFFFFFFF, 0xDEADBEEF)
            expected = 2 if mode & 8 else 1
            self.assertEqual(self.query(mode), expected)
            trace = [(WRITE_CONTROL, 0x10000007), (READ_DATA, info)]
            if mode & 8:
                trace.append((WRITE_CONTROL, 0x09000001))
            self.assertEqual(self.trace(), trace)
            self.assertEqual(self.last_data.value, 0xA5A5A5A5)
            self.assertEqual(self.last_control.value, 0x09000001 if mode & 8 else 0x10000007)

    def test_unchanged_native_body_agrees_with_bus_model_for_ram_readback(self):
        for info in (0, 1, 2, 3, 0xFFFFFF, 0xFF000002, 0xFFFFFFFF):
            for mode in (-1, 0, 1, 8, 16, 0x12340008):
                fallback = 0xE1001007
                self.reset(info, 0x10000007, fallback)
                expected = self.query(mode)
                command = self.last_control.value
                expected_data = self.last_data.value if (info & 0xFFFFFF) != 2 else info
                self.assertEqual(self.native_query(mode), expected)
                self.assertEqual(list(self.ram), [expected_data, command])


if __name__ == '__main__':
    unittest.main()
