"""Controller mode update with the real mode setters and delta decoder."""
import ctypes as C
import struct
import unittest

from test_channel_patch_voice_update import ChannelVoiceFixture


class ChannelModeUpdateTest(ChannelVoiceFixture, unittest.TestCase):
    handler = '800806C4'

    def setUp(self):
        super().setUp()
        self.mode = (C.c_uint8 * 65536).in_dll(self.lib, 'test_mode_page')
        C.memset(C.addressof(self.mode), 0xA5, len(self.mode))

    def test_all_control_bytes_high_bits_and_signed_selectors(self):
        table_before = bytes(self.table)
        for bank, row in [(-4, -8), (0, 0), (4, 8)]:
            for value in range(256):
                offset, stream, *_ = self.prepare_record(bank, row, 5, [0x81, 0], 0xFFFFFFFC)
                expected = bytearray(self.records)
                self.write_expected(expected, offset, 'cursor', struct.pack('P', stream + 2))
                self.write_expected(expected, offset, 'accumulator', struct.pack('<I', 1276))
                self.write_expected(expected, offset, 'counter', struct.pack('<i', 1280))
                expected_mode = bytearray(self.mode)
                expected_mode[0xA0:0xA2] = struct.pack('<h', 0 if value < 64 else 2)
                streams_before = bytes(self.streams)
                self.function(0x12340000 | (bank & 65535), 0x43210000 | (row & 65535),
                              C.c_int32(0xFEDCBA00 | value).value)
                self.assertEqual(bytes(self.records), bytes(expected))
                self.assertEqual(bytes(self.mode), bytes(expected_mode))
                self.assertEqual(bytes(self.streams), streams_before)
        self.assertEqual(bytes(self.table), table_before)
        self.assertEqual(self.events, [])

    def test_real_controller_consumes_payload_then_decodes_once_for_both_modes(self):
        for value in (0, 63, 64, 255):
            offset, stream, *_ = self.prepare_record(1, 2, 5, [value, 0x81, 0], 99)
            expected = bytearray(self.records)
            self.write_expected(expected, offset, 'cursor', struct.pack('P', stream + 3))
            self.write_expected(expected, offset, 'accumulator', struct.pack('<I', 1379))
            self.write_expected(expected, offset, 'counter', struct.pack('<i', 1280))
            expected_mode = bytearray(self.mode)
            expected_mode[0xA0:0xA2] = struct.pack('<h', 0 if value < 64 else 2)
            self.controller(0x12340001, 0x43210002, 0x12340040)
            self.assertEqual(bytes(self.records), bytes(expected))
            self.assertEqual(bytes(self.mode), bytes(expected_mode))
        self.assertEqual(self.events, [])


if __name__ == '__main__':
    unittest.main()
