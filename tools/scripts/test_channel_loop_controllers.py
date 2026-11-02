"""Loop controller callbacks, live mode reloads, saved cursor and repeat limits."""
import ctypes as C
import struct
import unittest

from test_channel_patch_voice_update import ChannelVoiceFixture

Callback = C.CFUNCTYPE(None, C.c_int32, C.c_int32, C.c_int32)


class ChannelLoopValueTest(ChannelVoiceFixture, unittest.TestCase):
    handler = '80080804'

    def setUp(self):
        super().setUp()
        self.loop_callbacks = (C.c_void_p * 0x200).in_dll(self.lib, 'test_loop_callbacks')

    def bind_callback(self, bank, row, function):
        callback = Callback(function)
        self.callbacks.append(callback)
        self.loop_callbacks[0x100 + bank * 16 + row] = C.cast(callback, C.c_void_p).value

    def test_all_modes_null_callback_raw_values_and_signed_selectors(self):
        table_before = bytes(self.table)
        for bank, row in [(-4, -8), (0, 0), (4, 8)]:
            for mode in range(256):
                for payload in (-1, 0, 128, 255, 0x123456AB):
                    offset, stream, *_ = self.prepare_record(bank, row, 5, [0x81, 0], 0xFFFFFFFC)
                    self.field(offset, 'mode', C.c_uint8).value = mode
                    self.field(offset, 'serial', C.c_uint8).value = 255
                    expected = bytearray(self.records)
                    if mode not in (20, 30, 40):
                        self.write_expected(expected, offset, 'control_value', bytes([payload & 255]))
                        self.write_expected(expected, offset, 'pending', b'\0')
                        self.write_expected(expected, offset, 'serial', b'\0')
                    self.write_expected(expected, offset, 'cursor', struct.pack('P', stream + 2))
                    self.write_expected(expected, offset, 'accumulator', struct.pack('<I', 1276))
                    self.write_expected(expected, offset, 'counter', struct.pack('<i', 1280))
                    streams_before = bytes(self.streams)
                    self.function(0x12340000 | (bank & 65535), 0x43210000 | (row & 65535), payload)
                    self.assertEqual(bytes(self.records), bytes(expected))
                    self.assertEqual(bytes(self.streams), streams_before)
        self.assertEqual(self.events, [])
        self.assertEqual(bytes(self.table), table_before)

    def test_registered_hook_runs_only_in_mode_40_and_uses_exact_bank_row(self):
        bank, row = 3, 7
        self.bind_callback(bank, row, lambda *args: self.events.append(('selected', args)))
        self.bind_callback(bank, row - 1, lambda *args: self.events.append(('wrong-row', args)))
        self.bind_callback(bank - 1, row, lambda *args: self.events.append(('wrong-bank', args)))
        for mode in range(256):
            self.events.clear()
            offset, _, *_ = self.prepare_record(bank, row, 5, [0], 99)
            self.field(offset, 'mode', C.c_uint8).value = mode
            self.function(bank, row, 0x123456AB)
            self.assertEqual(self.events, [('selected', (bank, row, 0xAB))] if mode == 40 else [])

    def test_callback_receives_canonical_args_and_changed_mode_controls_late_stores(self):
        bank, row = -1, 2
        for mode in (0, 20, 30, 40, 99, 255):
            self.events.clear()
            offset, stream, *_ = self.prepare_record(bank, row, 5, [0, 0x81, 0], 99)
            self.field(offset, 'mode', C.c_uint8).value = 40
            self.field(offset, 'serial', C.c_uint8).value = 255
            initial = bytes(self.records)
            def callback(*args):
                self.events.append((args, bytes(self.records)))
                self.field(offset, 'mode', C.c_uint8).value = mode
                self.field(offset, 'cursor', C.c_void_p).value = stream + 1
                self.field(offset, 'accumulator', C.c_uint32).value = 0xFFFFFFFC
            self.bind_callback(bank, row, callback)
            expected = bytearray(initial)
            self.write_expected(expected, offset, 'mode', bytes([mode]))
            if mode not in (20, 30, 40):
                self.write_expected(expected, offset, 'control_value', b'\xAB')
                self.write_expected(expected, offset, 'pending', b'\0')
                self.write_expected(expected, offset, 'serial', b'\0')
            self.write_expected(expected, offset, 'cursor', struct.pack('P', stream + 3))
            self.write_expected(expected, offset, 'accumulator', struct.pack('<I', 1276))
            self.write_expected(expected, offset, 'counter', struct.pack('<i', 1280))
            self.function(0x1234FFFF, 0x43210002, 0x123456AB)
            self.assertEqual(self.events, [((-1, 2, 0xAB), initial)])
            self.assertEqual(bytes(self.records), bytes(expected))

    def test_callback_table_replacement_redirects_decoder_but_updates_original_record(self):
        offset, _, *_ = self.prepare_record(0, 0, 3, [0], 55)
        other, other_stream, *_ = self.prepare_record(1, 0, 1, [0x81, 0], 100)
        self.field(offset, 'mode', C.c_uint8).value = 40
        self.field(offset, 'serial', C.c_uint8).value = 254
        replacement = C.addressof(self.records) + other
        initial = bytes(self.records)
        def callback(*args):
            self.events.append((args, bytes(self.records)))
            self.field(offset, 'mode', C.c_uint8).value = 0
            self.table[0x4000 - 0xF98 // 4] = replacement
        self.bind_callback(0, 0, callback)
        expected = bytearray(initial)
        self.write_expected(expected, offset, 'mode', b'\0')
        self.write_expected(expected, offset, 'control_value', b'\xFE')
        self.write_expected(expected, offset, 'pending', b'\0')
        self.write_expected(expected, offset, 'serial', b'\xFF')
        self.write_expected(expected, other, 'cursor', struct.pack('P', other_stream + 2))
        self.write_expected(expected, other, 'accumulator', struct.pack('<I', 1380))
        self.write_expected(expected, offset, 'counter', struct.pack('<i', 1280))
        self.function(0, 0, 254)
        self.assertEqual(self.events, [((0, 0, 254), initial)])
        self.assertEqual(bytes(self.records), bytes(expected))
        self.assertEqual(self.table[0x4000 - 0xF98 // 4], replacement)

    def test_real_controller_calls_optional_hook_after_payload_then_decodes_once(self):
        offset, stream, *_ = self.prepare_record(1, 2, 5, [0xAB, 0x81, 0], 99)
        self.field(offset, 'mode', C.c_uint8).value = 40
        self.bind_callback(1, 2, lambda *args: self.events.append((args, bytes(self.records))))
        published = bytearray(self.records)
        self.write_expected(published, offset, 'cursor', struct.pack('P', stream + 1))
        expected = bytearray(published)
        self.write_expected(expected, offset, 'cursor', struct.pack('P', stream + 3))
        self.write_expected(expected, offset, 'accumulator', struct.pack('<I', 1379))
        self.write_expected(expected, offset, 'counter', struct.pack('<i', 1280))
        self.controller(0x12340001, 0x43210002, 0x12340062)
        self.assertEqual(self.events, [((1, 2, 0xAB), bytes(published))])
        self.assertEqual(bytes(self.records), bytes(expected))


class ChannelLoopModeTest(ChannelVoiceFixture, unittest.TestCase):
    handler = '80080904'

    def check_transition(self, bank, row, payload, count, through_controller=False):
        data = [payload & 255, 0x81, 0] if through_controller else [0x81, 0]
        consumed = 3 if through_controller else 2
        offset, stream, *_ = self.prepare_record(bank, row, 5, data, 0xFFFFFFFC)
        saved = stream + 16
        self.field(offset, 'saved_cursor', C.c_void_p).value = saved
        self.field(offset, 'repeat', C.c_uint8).value = count
        self.field(offset, 'serial', C.c_uint8).value = 255
        expected = bytearray(self.records)
        mode = payload & 255
        self.write_expected(expected, offset, 'mode', bytes([mode]))
        self.write_expected(expected, offset, 'accumulator', struct.pack('<I', 1276))
        self.write_expected(expected, offset, 'counter', struct.pack('<i', 1280))
        cursor = stream + consumed
        if mode == 20:
            self.write_expected(expected, offset, 'pending', b'\1')
            self.write_expected(expected, offset, 'saved_cursor', struct.pack('P', cursor))
        elif mode == 30:
            if count == 0:
                self.write_expected(expected, offset, 'enabled', b'\0')
            elif count < 127:
                self.write_expected(expected, offset, 'repeat', bytes([count - 1]))
                if count == 1:
                    self.write_expected(expected, offset, 'enabled', b'\0')
                else:
                    cursor = saved
            else:
                cursor = saved
                self.write_expected(expected, offset, 'counter', struct.pack('<i', 0))
        else:
            self.write_expected(expected, offset, 'serial', b'\0')
        self.write_expected(expected, offset, 'cursor', struct.pack('P', cursor))
        streams_before = bytes(self.streams)
        if through_controller:
            self.controller(0x12340000 | (bank & 65535), 0x43210000 | (row & 65535), 0x12340063)
        else:
            self.function(0x12340000 | (bank & 65535), 0x43210000 | (row & 65535), payload)
        self.assertEqual(bytes(self.records), bytes(expected), (bank, row, payload, count))
        self.assertEqual(bytes(self.streams), streams_before)

    def test_all_modes_raw_high_bits_and_signed_selectors(self):
        table_before = bytes(self.table)
        for bank, row in [(-4, -8), (0, 0), (4, 8)]:
            for mode in range(256):
                self.check_transition(bank, row, C.c_int32(0xFEDCBA00 | mode).value, 2)
        self.assertEqual(bytes(self.table), table_before)
        self.assertEqual(self.events, [])

    def test_all_repeat_counts_finite_and_infinite_with_real_decoder(self):
        for bank, row in [(-4, -8), (0, 0), (4, 8)]:
            for count in range(256):
                self.check_transition(bank, row, 0x1234001E, count)

    def test_marker_then_finite_repeats_reuse_the_actual_saved_stream_pointer(self):
        offset, stream, *_ = self.prepare_record(0, 0, 5, [1, 2, 3, 4], 0)
        self.field(offset, 'enabled', C.c_uint8).value = 1
        self.field(offset, 'repeat', C.c_uint8).value = 2
        expected = bytearray(self.records)
        for mode, count, consumed, accumulator, delta, enabled in [
                (20, 2, 1, 10, 10, 1), (30, 1, 1, 30, 20, 1),
                (30, 0, 2, 50, 20, 0), (30, 0, 3, 80, 30, 0)]:
            self.write_expected(expected, offset, 'mode', bytes([mode]))
            self.write_expected(expected, offset, 'pending', b'\1')
            self.write_expected(expected, offset, 'repeat', bytes([count]))
            self.write_expected(expected, offset, 'enabled', bytes([enabled]))
            self.write_expected(expected, offset, 'saved_cursor', struct.pack('P', stream + 1))
            self.write_expected(expected, offset, 'cursor', struct.pack('P', stream + consumed))
            self.write_expected(expected, offset, 'accumulator', struct.pack('<I', accumulator))
            self.write_expected(expected, offset, 'counter', struct.pack('<i', delta))
            self.function(0, 0, mode)
            self.assertEqual(bytes(self.records), bytes(expected))

    def test_real_controller_preserves_loop_marker_and_repeat_cursor_semantics(self):
        for mode, count in [(20, 0), (30, 0), (30, 1), (30, 2), (30, 126),
                            (30, 127), (30, 255), (40, 255), (255, 255)]:
            self.check_transition(1, 2, mode, count, through_controller=True)


if __name__ == '__main__':
    unittest.main()
