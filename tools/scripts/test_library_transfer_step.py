"""Real transfer callback: wrapped counters, restoration order and late notifications."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

Restore = C.CFUNCTYPE(None, C.c_void_p)
Command = C.CFUNCTYPE(None, C.c_int32, C.c_int32)
Notify = C.CFUNCTYPE(None, C.c_int32, C.c_int32)


class State(C.Structure):
    _fields_ = [('pad00', C.c_uint8 * 8), ('position', C.c_uint32),
                ('pad0C', C.c_uint8 * 4), ('step', C.c_uint32), ('remaining', C.c_uint32),
                ('pad18', C.c_uint8 * 8), ('completed', C.c_uint32), ('finished', C.c_uint32),
                ('pad28', C.c_uint8 * 4), ('restore_first', C.c_void_p),
                ('restore_second', C.c_void_p), ('argument', C.c_int32)]


class NotificationPage(C.Structure):
    _fields_ = [('prefix', C.c_uint8 * 0x63F0), ('callback', C.c_void_p),
                ('suffix', C.c_uint8 * (0x10000 - 0x63F0 - C.sizeof(C.c_void_p)))]


class LibraryTransferStepTest(unittest.TestCase):
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
RrLibTransferState38 CdReadState;
u32 CD_read_dma_mode;
struct { u8 prefix[0x63F0]; RrLibNotificationCallback callback;
         u8 suffix[0x10000 - 0x63F0 - sizeof(RrLibNotificationCallback)]; } test_page;
void (*test_first)(RrLibCompletionCallback);
void (*test_second)(RrLibCompletionCallback);
void (*test_control)(RrLibCompletionCallback);
void (*test_command)(s32, s32);
s32 test_unexpected_calls;
void CdReadyCallback(RrLibCompletionCallback callback) { test_first(callback); }
void CdDataCallback(RrLibCompletionCallback callback) { test_second(callback); }
void CdSyncCallback(RrLibCompletionCallback callback) { test_control(callback); }
s32 CdControlF(s32 command, const u8 *value) { test_command(command, (s32)(unsigned long)value); return 0; }
void cd_read_restore_sync(void) { test_unexpected_calls++; }
void test_callback_first(void) { test_unexpected_calls++; }
void test_callback_second(void) { test_unexpected_calls++; }
'''
        body = (root / 'tools/scripts/fixtures/legacy/func_8008EA9C.c').read_text()
        body = re.sub(r'^#include[^\n]*\n', '', body, flags=re.M)
        body = re.sub(r'\s+__asm__\("\$[0-9]+"\)', '', body)
        body = re.sub(r'"\$[0-9]+", ', '', body)
        body = re.sub(r' : "\$[0-9]+"', '', body)
        body = body.replace('(u8 *)0x800A0000U', '((u8 *)&test_page)')
        source += body
        (directory / 'function.c').write_text(source)
        subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory / 'function.c'),
                        '-o', str(directory / 'function.so')], check=True, capture_output=True)
        cls.library = C.CDLL(str(directory / 'function.so'))
        cls.state = State.in_dll(cls.library, 'CdReadState')
        cls.flags = C.c_uint32.in_dll(cls.library, 'CD_read_dma_mode')
        cls.page = NotificationPage.in_dll(cls.library, 'test_page')
        cls.unexpected = C.c_int32.in_dll(cls.library, 'test_unexpected_calls')
        cls.first_address = C.cast(cls.library.test_callback_first, C.c_void_p).value
        cls.second_address = C.cast(cls.library.test_callback_second, C.c_void_p).value
        cls.control_address = C.cast(cls.library.cd_read_restore_sync, C.c_void_p).value
        cls.function = cls.library.cb_data
        cls.function.argtypes = []
        cls.function.restype = None

    def setUp(self):
        C.memset(C.addressof(self.state), 0xA5, C.sizeof(self.state))
        C.memset(C.addressof(self.page), 0x5A, C.sizeof(self.page))
        self.state.position, self.state.step, self.state.remaining = 100, 3, 1
        self.state.completed, self.state.finished = 0, 0xDEADBEEF
        self.state.restore_first, self.state.restore_second = self.first_address, self.second_address
        self.state.argument = -123
        self.flags.value = 1
        self.unexpected.value = 0
        self.events = []
        self.callbacks = []
        for name, label in [('test_first', 'first'), ('test_second', 'second'), ('test_control', 'control')]:
            self.hook(name, Restore, lambda callback, label=label: self.events.append((label, callback, bytes(self.state))))
        self.hook('test_command', Command, lambda command, value: self.events.append(('command', command, value, bytes(self.state))))
        self.notification = Notify(lambda event, argument: self.events.append(('notify', event, argument, bytes(self.state))))
        self.page.callback = C.cast(self.notification, C.c_void_p).value

    def hook(self, name, kind, function):
        callback = kind(function)
        self.callbacks.append(callback)
        C.c_void_p.in_dll(self.library, name).value = C.cast(callback, C.c_void_p).value

    def advanced(self):
        expected = State.from_buffer_copy(self.state)
        expected.position = expected.position + expected.step * 4
        expected.remaining -= 1
        expected.completed += 1
        return expected

    def test_nonfinal_steps_wrap_unsigned_arithmetic_without_callbacks(self):
        for remaining in (0, 2, 0x80000000, 0xFFFFFFFF):
            for position in (0, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF):
                for step in (0, 1, 0x40000000, 0xFFFFFFFF):
                    self.state.remaining, self.state.position, self.state.step = remaining, position, step
                    self.state.completed = 0xFFFFFFFF
                    self.events.clear()
                    expected = self.advanced()
                    page = bytes(self.page)
                    self.function()
                    self.assertEqual(bytes(self.state), bytes(expected))
                    self.assertEqual(bytes(self.page), page)
                    self.assertEqual(self.events, [])
        self.assertEqual(self.unexpected.value, 0)

    def test_completion_restoration_order_and_finished_publication(self):
        for flags in (0, 1, 2, 0xFFFFFFFF):
            for notify in (False, True):
                self.state.remaining = 1
                self.state.finished = 0xDEADBEEF
                self.flags.value = flags
                self.page.callback = C.cast(self.notification, C.c_void_p).value if notify else None
                self.events.clear()
                expected = self.advanced()
                pending = bytes(expected)
                expected.finished = 1
                events = [('first', self.first_address, pending)]
                if flags & 1:
                    events.append(('second', self.second_address, pending))
                events += [('control', self.control_address, pending), ('command', 9, 0, pending)]
                if notify:
                    events.append(('notify', 2, -123, bytes(expected)))
                self.function()
                self.assertEqual(bytes(self.state), bytes(expected))
                self.assertEqual(self.events, events)
        self.assertEqual(self.unexpected.value, 0)

    def test_later_callback_fields_and_flags_are_reloaded_after_first_restore(self):
        def first(callback):
            self.events.append(('first', callback))
            self.flags.value = 1
            self.state.restore_second = self.first_address
        self.hook('test_first', Restore, first)
        self.flags.value = 0
        self.function()
        self.assertEqual(self.events[0], ('first', self.first_address))
        self.assertEqual(self.events[1][:2], ('second', self.first_address))
        self.assertEqual(self.state.finished, 1)
        self.assertEqual(self.unexpected.value, 0)

    def test_notification_pointer_and_argument_are_loaded_after_command(self):
        self.page.callback = None
        def command(command, value):
            self.events.append(('command', command, value, bytes(self.state)))
            self.page.callback = C.cast(self.notification, C.c_void_p).value
            self.state.argument = -2147483648
        self.hook('test_command', Command, command)
        self.function()
        self.assertEqual(self.events[-1][:3], ('notify', 2, -2147483648))
        self.assertEqual(self.state.finished, 1)
        self.assertEqual(self.events[-1][3], bytes(self.state))


if __name__ == '__main__':
    unittest.main()
