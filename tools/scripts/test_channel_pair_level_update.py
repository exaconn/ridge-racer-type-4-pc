"""Paired level controller using the real command builder and delta decoder."""
import ctypes as C
import struct
import unittest

from test_channel_patch_voice_update import ChannelVoiceFixture

Sink = C.CFUNCTYPE(C.c_int32, C.c_void_p)


class ChannelPairLevelUpdateTest(ChannelVoiceFixture, unittest.TestCase):
    handler = '80080774'

    def setUp(self):
        super().setUp()
        self.commands = (C.c_uint8 * 128).in_dll(self.lib, 'test_commands')
        C.memset(C.addressof(self.commands), 0xA5, len(self.commands))
        def sink(pointer):
            self.events.append(('command', C.string_at(pointer, 128), bytes(self.records)))
            return -1
        self.hook('test_command_sink', Sink, sink)

    def expected_command(self, value):
        expected = bytearray(self.commands)
        expected[0:4] = struct.pack('<i', 6)
        level = ((value & 255) * 0x7FFF // 127) & 65535
        expected[8:12] = struct.pack('<HH', level, level)
        return bytes(expected)

    def test_all_payload_bytes_raw_high_bits_and_signed_selectors(self):
        table_before = bytes(self.table)
        for bank, row in [(-4, -8), (0, 0), (4, 8)]:
            for value in range(256):
                self.events.clear()
                offset, stream, *_ = self.prepare_record(bank, row, 5, [0x81, 0], 0xFFFFFFFC)
                initial = bytes(self.records)
                expected = bytearray(initial)
                self.write_expected(expected, offset, 'cursor', struct.pack('P', stream + 2))
                self.write_expected(expected, offset, 'accumulator', struct.pack('<I', 1276))
                self.write_expected(expected, offset, 'counter', struct.pack('<i', 1280))
                command = self.expected_command(value)
                self.function(0x12340000 | (bank & 65535), 0x43210000 | (row & 65535),
                              C.c_int32(0xFEDCBA00 | value).value)
                self.assertEqual(self.events, [('command', command, initial)])
                self.assertEqual(bytes(self.records), bytes(expected))
                self.assertEqual(bytes(self.commands), command)
        self.assertEqual(bytes(self.table), table_before)

    def test_sink_changes_stream_and_accumulator_before_decoder(self):
        offset, stream, *_ = self.prepare_record(0, 0, 5, [0, 0x81, 0], 99)
        initial = bytes(self.records)
        def sink(pointer):
            self.events.append(('command', C.string_at(pointer, 128), bytes(self.records)))
            self.field(offset, 'cursor', C.c_void_p).value = stream + 1
            self.field(offset, 'accumulator', C.c_uint32).value = 0xFFFFFFFC
            return -1
        self.hook('test_command_sink', Sink, sink)
        command = self.expected_command(255)
        expected = bytearray(initial)
        self.write_expected(expected, offset, 'cursor', struct.pack('P', stream + 3))
        self.write_expected(expected, offset, 'accumulator', struct.pack('<I', 1276))
        self.write_expected(expected, offset, 'counter', struct.pack('<i', 1280))
        self.function(0, 0, -1)
        self.assertEqual(self.events, [('command', command, initial)])
        self.assertEqual(bytes(self.records), bytes(expected))

    def test_sink_table_replacement_redirects_decoder_but_not_final_counter_store(self):
        offset, _, *_ = self.prepare_record(0, 0, 3, [0], 55)
        other, other_stream, *_ = self.prepare_record(1, 0, 1, [0x81, 0], 100)
        replacement = C.addressof(self.records) + other
        initial = bytes(self.records)
        def sink(pointer):
            self.events.append(('command', C.string_at(pointer, 128), bytes(self.records)))
            self.table[0x4000 - 0xF98 // 4] = replacement
            return 0
        self.hook('test_command_sink', Sink, sink)
        command = self.expected_command(128)
        expected = bytearray(initial)
        self.write_expected(expected, other, 'cursor', struct.pack('P', other_stream + 2))
        self.write_expected(expected, other, 'accumulator', struct.pack('<I', 1380))
        self.write_expected(expected, offset, 'counter', struct.pack('<i', 1280))
        self.function(0, 0, 128)
        self.assertEqual(self.events, [('command', command, initial)])
        self.assertEqual(bytes(self.records), bytes(expected))
        self.assertEqual(self.table[0x4000 - 0xF98 // 4], replacement)

    def test_public_pair_setter_narrows_full_register_arguments_locally(self):
        setter = self.lib.SsUtSetReverbDepth
        setter.argtypes = [C.c_int32, C.c_int32]
        setter.restype = None
        for left, right in [(0x12348000, 0x43217FFF), (-1, 0),
                            (0x12340080, 0x432100FF), (-0x80000000, 0x7FFFFFFF)]:
            self.events.clear()
            expected = bytearray(self.commands)
            expected[0:4] = struct.pack('<i', 6)
            levels = []
            for raw in (left, right):
                product = C.c_int16(raw).value * 0x7FFF
                quotient = abs(product) // 127
                levels.append((-quotient if product < 0 else quotient) & 65535)
            expected[8:12] = struct.pack('<HH', *levels)
            records_before = bytes(self.records)
            setter(left, right)
            self.assertEqual(self.events, [('command', bytes(expected), records_before)])
            self.assertEqual(bytes(self.commands), bytes(expected))
            self.assertEqual(bytes(self.records), records_before)

    def test_real_controller_calls_real_pair_setter_and_decodes_once(self):
        offset, stream, *_ = self.prepare_record(1, 2, 5, [0xAB, 0x81, 0], 99)
        published = bytearray(self.records)
        self.write_expected(published, offset, 'cursor', struct.pack('P', stream + 1))
        expected = bytearray(published)
        self.write_expected(expected, offset, 'cursor', struct.pack('P', stream + 3))
        self.write_expected(expected, offset, 'accumulator', struct.pack('<I', 1379))
        self.write_expected(expected, offset, 'counter', struct.pack('<i', 1280))
        command = self.expected_command(0xAB)
        self.controller(0x12340001, 0x43210002, 0x1234005B)
        self.assertEqual(self.events, [('command', command, bytes(published))])
        self.assertEqual(bytes(self.records), bytes(expected))


if __name__ == '__main__':
    unittest.main()
