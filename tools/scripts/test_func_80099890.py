"""Host-side behavioral checks; fd.py verifies the PSX layout and instructions."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest


U8 = C.c_uint8


from channel_native_types import PadChannel
from c_functions import source_owners, definition_source


Callback = C.CFUNCTYPE(C.c_uint32, C.POINTER(PadChannel), C.c_int32)
Finish = C.CFUNCTYPE(C.c_int32, C.POINTER(PadChannel), C.c_int32)


class TransferResultTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        directory = Path(cls.temp.name)
        root = Path(__file__).resolve().parents[2]
        owners=source_owners(root/'src/main/PAL')
        body=definition_source(owners['_padRecvStage3'].read_text(),'_padRecvStage3')
        source="""#include "rr/channel_init.h"
ChannelByteCallback PadTxByteCallback;
s32 (*test_finish)(PadChannel *,s32);
s32 _padSioRW2(PadChannel *state,s32 value) {return test_finish(state,value);}
"""+body
        (directory / 'function.c').write_text(source)
        subprocess.run(['cc', '-shared', '-fPIC', '-O2', '-I', str(root/'include'), str(directory / 'function.c'),
                        '-o', str(directory / 'function.so')], check=True, capture_output=True)
        cls.library = C.CDLL(str(directory / 'function.so'))
        cls.function = cls.library._padRecvStage3
        cls.function.argtypes = [C.POINTER(PadChannel)]
        cls.function.restype = C.c_int32

    def check_transfer(self, header, flag, callback_result, finish_result):
        packet = (U8 * 1)(header)
        state = PadChannel()
        state.packet = C.addressof(packet)
        state.command37 = flag
        calls = []

        @Callback
        def callback(pointer, mode):
            calls.append(('callback', C.addressof(pointer.contents), mode))
            return callback_result

        @Finish
        def finish(pointer, value):
            calls.append(('finish', C.addressof(pointer.contents), value))
            return finish_result

        C.c_void_p.in_dll(self.library, 'PadTxByteCallback').value = C.cast(callback, C.c_void_p).value
        C.c_void_p.in_dll(self.library, 'test_finish').value = C.cast(finish, C.c_void_p).value
        before = C.string_at(C.addressof(state), C.sizeof(state))
        expected = finish_result if finish_result <= 0 or finish_result == 0x5A else -4
        self.assertEqual(self.function(C.byref(state)), expected)
        mode = int(0x80 <= header <= 0x8F and flag == 0)
        self.assertEqual(calls, [('callback', C.addressof(state), mode),
                                 ('finish', C.addressof(state), callback_result & 0xFF)])
        self.assertEqual(C.string_at(C.addressof(state), C.sizeof(state)), before)
        self.assertEqual(packet[0], header)

    def test_all_header_values_and_flag_states(self):
        for header in range(256):
            for flag in (0, 1, 255):
                self.check_transfer(header, flag, 0x12345634, 0)

    def test_byte_truncation_and_result_boundaries(self):
        for callback_result in (-(1 << 31), -257, -1, 0, 255, 256, (1 << 31) - 1):
            for finish_result in (-(1 << 31), -5, -4, -1, 0, 1, 89, 90, 91, (1 << 31) - 1):
                self.check_transfer(0x81, 0, callback_result, finish_result)


if __name__ == '__main__':
    unittest.main()
