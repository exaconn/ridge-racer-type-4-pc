"""Real SPU stop body with ordered MMIO adapter and bounded busy status."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest


class SpuTransferStopTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        root = Path(__file__).resolve().parents[2]
        directory = Path(cls.temp.name)
        header = (root / 'include/rr/spu.h').read_text()
        header = re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n', '', header, flags=re.M)
        source = '''#include <stdint.h>
#include <stddef.h>
#include <string.h>
typedef uint8_t u8; typedef int16_t s16; typedef uint16_t u16;
typedef uint32_t u32; typedef int32_t s32;
''' + header + '''
SpuRegisterBank test_banks[2];
SpuRegisterBank *_spu_RXX=&test_banks[0];
s32 _spu_dmaIsRead;
void (*volatile _spu_transferCallback)(void);
u32 test_count, test_log[4000][3], test_busy, test_reads, test_written;
u32 test_replace_after_delay, test_callback_mutation;
u32 test_layout[]={sizeof(SpuRegisterBank),offsetof(SpuRegisterBank,spu_control)};
static void record(u32 op,u32 value,u32 bank) {
 if(test_count<4000) {test_log[test_count][0]=op;test_log[test_count][1]=value;test_log[test_count][2]=bank;}
 test_count++;
}
u32 test_read_control(SpuRegisterBank *bank) {
 u32 value=bank->spu_control;
 if(test_written && test_reads++<test_busy) value|=0x30;
 record(1,value,(u32)(bank-test_banks));return value;
}
void test_write_control(SpuRegisterBank *bank,u32 value) {
 record(2,value,(u32)(bank-test_banks));bank->spu_control=value;test_written=1;
}
void test_callback(void) {
 record(4,0,(u32)(_spu_RXX-test_banks));
 if(test_callback_mutation) {_spu_RXX->spu_control=0xBEEF;_spu_dmaIsRead=72;}
}
s32 DeliverEvent(u32 first,s32 second) {record(5,first,(u32)second);return -1;}
'''
        delay = (root / 'tools/scripts/fixtures/legacy/func_8007F59C.c').read_text()
        delay = delay.replace('#include "types.h"', '').replace('_spu_Fw1ts', 'test_actual_delay')
        delay = re.sub(r'\s+__asm__\("\$[0-9]+"\)', '', delay)
        source += delay + '''
void _spu_Fw1ts(void) {
 record(3,0,0);test_actual_delay();
 if(test_replace_after_delay)_spu_RXX=&test_banks[1];
}
void test_initialize(u32 initial,u32 active,u32 callback,u32 busy) {
 memset(test_banks,0xA5,sizeof(test_banks));
 test_banks[0].spu_control=initial;test_banks[1].spu_control=initial;
 _spu_RXX=&test_banks[0];_spu_dmaIsRead=(s32)active;
 _spu_transferCallback=callback?test_callback:0;
 test_count=test_reads=test_written=test_replace_after_delay=test_callback_mutation=0;
 test_busy=busy;memset(test_log,0xA5,sizeof(test_log));
}
'''
        body = (root / 'tools/scripts/fixtures/legacy/func_8007EF04.c').read_text()
        body = body.replace('#include "rr/spu.h"', '')
        body = re.sub(r'\s+__asm__\("\$[0-9]+"\)', '', body)
        body = re.sub(r':\s*"\$[0-9]+"(?:,\s*"\$[0-9]+")*(?:,\s*"memory")?', '', body)
        source += body.replace('_spu_FiDMA', 'test_native_stop')
        if body.count('bank->spu_control') != 3:
            raise AssertionError('Adapter must cover the actual RMW plus both status-read sites')
        body = body.replace('bank->spu_control &= 0xFFCF;',
                            'test_write_control(bank,test_read_control(bank) & 0xFFCF);')
        body = body.replace('bank->spu_control', 'test_read_control(bank)')
        source += body
        (directory / 'stop.c').write_text(source)
        result = subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory / 'stop.c'),
                                 '-o', str(directory / 'stop.so')], capture_output=True)
        if result.returncode:
            raise AssertionError(result.stderr.decode())
        cls.lib = C.CDLL(str(directory / 'stop.so'))
        cls.size, cls.control_offset = (C.c_uint32 * 2).in_dll(cls.lib, 'test_layout')
        cls.banks = (C.c_uint8 * (2*cls.size)).in_dll(cls.lib, 'test_banks')
        cls.count = C.c_uint32.in_dll(cls.lib, 'test_count')
        cls.log = (C.c_uint32 * 12000).in_dll(cls.lib, 'test_log')
        cls.replace = C.c_uint32.in_dll(cls.lib, 'test_replace_after_delay')
        cls.mutate = C.c_uint32.in_dll(cls.lib, 'test_callback_mutation')
        cls.active = C.c_int32.in_dll(cls.lib, '_spu_dmaIsRead')
        cls.initialize = cls.lib.test_initialize
        cls.initialize.argtypes = [C.c_uint32]*4
        cls.stop, cls.native_stop = cls.lib._spu_FiDMA, cls.lib.test_native_stop
        for function in (cls.stop, cls.native_stop):
            function.argtypes = []
            function.restype = None

    def trace(self):
        self.assertLess(self.count.value, 4000)
        return [tuple(self.log[3*i:3*i+3]) for i in range(self.count.value)]

    def check(self, initial, active, callback, busy, native=False, replace=False, mutate=False):
        self.initialize(initial, active, callback, busy)
        self.replace.value = replace
        self.mutate.value = mutate
        before = bytes(self.banks)
        expected = bytearray(before)
        bank = int(replace and active == 0)
        final = 0xBEEF if mutate and callback else initial & 0xFFCF
        offset = bank*self.size+self.control_offset
        expected[offset:offset+2] = final.to_bytes(2, 'little')
        (self.native_stop if native else self.stop)()
        trace = [(3, 0, 0)] if active == 0 else []
        if not native:
            trace += [(1, initial, bank), (2, initial & 0xFFCF, bank)]
            reads = min(busy+1, 3841)
            trace += [(1, (initial & 0xFFCF) | (0x30 if i < busy else 0), bank)
                      for i in range(reads)]
        trace += [(4, 0, bank)] if callback else [(5, 0xF0000009, 0x20)]
        self.assertEqual(self.trace(), trace)
        self.assertEqual(bytes(self.banks), bytes(expected))
        self.assertEqual(self.active.value, 72 if mutate and callback else C.c_int32(active).value)

    def test_busy_status_poll_limit_and_both_completion_routes(self):
        for initial in (0, 0x30, 0xFFFF, 0xA5C3):
            for active in (0, 1, 0xFFFFFFFF):
                for callback in (0, 1):
                    for busy in (0, 1, 2, 7, 3840, 3841, 9000):
                        self.check(initial, active, callback, busy)

    def test_delay_can_replace_bank_before_first_control_access(self):
        for callback in (0, 1):
            self.check(0xFFFF, 0, callback, 7, replace=True)

    def test_callback_state_changes_are_preserved_after_polling(self):
        for active in (0, 1):
            self.check(0xFFFF, active, 1, 5, mutate=True)

    def test_unchanged_native_body_agrees_when_control_write_clears_busy_bits(self):
        for initial in (0, 0x30, 0xFFFF, 0xA5C3):
            for active in (0, 1, 0xFFFFFFFF):
                for callback in (0, 1):
                    self.check(initial, active, callback, 0, native=True)


if __name__ == '__main__':
    unittest.main()
