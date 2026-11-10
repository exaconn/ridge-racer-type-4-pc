"""Actual once initializer and callback exchange wrappers with native function pointers."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

from test_interrupt_driver_setup import Driver

Exchange = C.CFUNCTYPE(C.c_void_p, C.c_int32, C.c_void_p)
Callback = C.CFUNCTYPE(None)
System = C.CFUNCTYPE(C.c_int32, C.c_int32, C.c_uint32, C.c_int32, C.c_int32, C.c_void_p)


class SpuEventInitializationTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        root = Path(__file__).resolve().parents[2]
        directory = Path(cls.temp.name)
        header = (root / 'include/rr/library.h').read_text()
        header = re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n', '', header, flags=re.M)
        source = '''#include <stdint.h>
typedef uint8_t u8; typedef int8_t s8;
typedef uint16_t u16; typedef int16_t s16;
typedef uint32_t u32; typedef int32_t s32;
''' + header + '''
s32 test_ram[16384];
#define _spu_isCalled test_ram[0x48A8/4]
#define _spu_EVdma test_ram[0x4840/4]
LibInterruptDriverPrefix test_driver,test_alternate_driver;
LibInterruptDriverPrefix *IntrDriverTablePtr=&test_driver;
s32 test_stop_calls;
void _spu_FiDMA(void) {test_stop_calls++;}
s32 (*test_system)(s32,u32,s32,s32,void *);
s32 EnterCriticalSection(void) {return test_system(1,0,0,0,0);}
s32 ExitCriticalSection(void) {return test_system(5,0,0,0,0);}
s32 OpenEvent(u32 first,s32 second,s32 third,LibInterruptCallback fourth) {
 return test_system(3,first,second,third,(void *)fourth);
}
s32 EnableEvent(s32 event) {return test_system(4,(u32)event,0,0,0);}
'''
        for name in ('8008B3C4', '8007F764', '8007F6EC'):
            body = (root / ('tools/scripts/fixtures/views/func_'+name+'.c')).read_text()
            body = re.sub(r'^#include[^\n]*\n', '', body, flags=re.M)
            body = body.replace('extern s32 _spu_isCalled,_spu_EVdma;', '')
            if name == '8007F6EC':
                if body.count('(s32 *)0x800A0000U') != 2 or body.count('unsigned long page=0x80080000U;') != 1:
                    raise AssertionError('Initializer page adapters must match the two stores and callback address')
                body = body.replace('(s32 *)0x800A0000U', 'test_ram')
                # Replace just the fixed callback page with a native address whose
                # signed low-page subtraction still gives the actual callback.
                body = body.replace('unsigned long page=0x80080000U;',
                                    'unsigned long page=(unsigned long)_spu_FiDMA+0x10FC;')
            body = re.sub(r'\s+__asm__\("\$[0-9]+"\)', '', body)
            body = re.sub(r'"\$[0-9]+", ', '', body)
            body = re.sub(r' : "\$[0-9]+"', '', body)
            source += body
        (directory / 'init.c').write_text(source)
        result = subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory / 'init.c'),
                                 '-o', str(directory / 'init.so')], capture_output=True)
        if result.returncode:
            raise AssertionError(result.stderr.decode())
        cls.lib = C.CDLL(str(directory / 'init.so'))
        cls.ram = (C.c_int32 * 16384).in_dll(cls.lib, 'test_ram')
        cls.flag = C.c_int32.from_buffer(cls.ram, 0x48A8)
        cls.event = C.c_int32.from_buffer(cls.ram, 0x4840)
        cls.driver = Driver.in_dll(cls.lib, 'test_driver')
        cls.alternate = Driver.in_dll(cls.lib, 'test_alternate_driver')
        cls.current = C.c_void_p.in_dll(cls.lib, 'IntrDriverTablePtr')
        cls.stop_calls = C.c_int32.in_dll(cls.lib, 'test_stop_calls')
        cls.stop = C.cast(cls.lib._spu_FiDMA, C.c_void_p).value
        cls.initialize = cls.lib.SpuStart
        cls.initialize.argtypes = []
        cls.initialize.restype = None
        cls.exchange = cls.lib.DMACallback
        cls.exchange.argtypes = [C.c_int32, C.c_void_p]
        cls.exchange.restype = C.c_void_p
        cls.spu_exchange = cls.lib._SpuDataCallback
        cls.spu_exchange.argtypes = [C.c_void_p]
        cls.spu_exchange.restype = C.c_void_p

    def setUp(self):
        C.memset(C.addressof(self.ram), 0xA5, C.sizeof(self.ram))
        self.flag.value = 0
        self.events = []
        self.open_result = -1234567
        self.system_action = None
        self.exchange_action = None
        self.old = Callback(lambda: self.events.append(('old_callback',)))
        self.old_address = C.cast(self.old, C.c_void_p).value
        self.system_hook = System(self.system)
        C.c_void_p.in_dll(self.lib, 'test_system').value = C.cast(self.system_hook, C.c_void_p).value
        self.exchange_hooks = []
        for label, driver in (('main', self.driver), ('alternate', self.alternate)):
            def exchange(channel, callback, label=label):
                self.events.append(('exchange', label, channel, callback, self.flag.value, self.event.value))
                if self.exchange_action:
                    self.exchange_action()
                return self.old_address
            hook = Exchange(exchange)
            self.exchange_hooks.append(hook)
            driver.reserved = driver.install = driver.reset = None
            driver.exchange = C.cast(hook, C.c_void_p).value
        self.current.value = C.addressof(self.driver)
        self.stop_calls.value = 0

    def system(self, phase, first, second, third, fourth):
        self.events.append(('system', phase, first, second, third, fourth,
                            self.flag.value, self.event.value))
        if self.system_action:
            self.system_action(phase)
        return self.open_result if phase == 3 else -99

    def test_initializer_publishes_flag_and_event_before_calls_then_skips_repeat(self):
        initial_event = self.event.value
        for opened in (0, 1, -1, -2147483648, 2147483647):
            self.flag.value = 0
            self.event.value = initial_event
            self.open_result = opened
            self.events.clear()
            before = bytearray(self.ram)
            self.initialize()
            self.assertEqual(self.events, [
                ('system', 1, 0, 0, 0, None, 1, initial_event),
                ('exchange', 'main', 4, self.stop, 1, initial_event),
                ('system', 3, 0xF0000009, 0x20, 0x2000, None, 1, initial_event),
                ('system', 4, opened & 0xFFFFFFFF, 0, 0, None, 1, opened),
                ('system', 5, 0, 0, 0, None, 1, opened)])
            C.c_int32.from_buffer(before, 0x48A8).value = 1
            C.c_int32.from_buffer(before, 0x4840).value = opened
            self.assertEqual(bytes(self.ram), bytes(before))
            events = list(self.events)
            self.initialize()
            self.assertEqual(self.events, events)
            self.assertEqual(bytes(self.ram), bytes(before))
            self.assertEqual(self.stop_calls.value, 0)

    def test_all_nonzero_initialized_states_are_inert(self):
        for flag in (-2147483648, -1, 1, 2, 2147483647):
            self.flag.value = flag
            before = bytes(self.ram)
            self.initialize()
            self.assertEqual(bytes(self.ram), before)
            self.assertEqual(self.events, [])

    def test_reentry_uses_published_flag_and_exchange_reloads_driver(self):
        def action(phase):
            if phase == 1:
                self.initialize()
                self.current.value = C.addressof(self.alternate)
            if phase == 4:
                self.event.value = 77
        self.system_action = action
        self.exchange_action = self.initialize
        self.initialize()
        self.assertEqual([event[0:2] for event in self.events],
                         [('system', 1), ('exchange', 'alternate'), ('system', 3), ('system', 4), ('system', 5)])
        self.assertEqual(self.events[-1][-1], 77)
        self.assertEqual(self.event.value, 77)
        self.assertEqual(self.flag.value, 1)

    def test_exchange_wrappers_forward_full_callback_and_return_previous_pointer(self):
        for channel in (-2147483648, -1, 0, 4, 2147483647):
            self.events.clear()
            before = bytes(self.ram)
            self.assertEqual(self.exchange(channel, self.stop), self.old_address)
            self.assertEqual(self.events, [('exchange', 'main', channel, self.stop, 0, self.event.value)])
            self.assertEqual(bytes(self.ram), before)
        self.events.clear()
        self.assertEqual(self.spu_exchange(self.old_address), self.old_address)
        self.assertEqual(self.events, [('exchange', 'main', 4, self.old_address, 0, self.event.value)])


if __name__ == '__main__':
    unittest.main()
