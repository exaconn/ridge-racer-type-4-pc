"""Pitch callback: snapshot slot, store after voice, then decode the live table."""
import ctypes as C
import struct
import unittest

from test_channel_patch_voice_update import ChannelVoiceFixture, Voice


class ChannelPitchUpdateTest(ChannelVoiceFixture, unittest.TestCase):
    handler = '80080434'

    def test_all_slots_values_and_raw_selectors_with_real_decoder(self):
        table_before = bytes(self.table)
        for bank, row in [(-4, -8), (0, 0), (4, 8)]:
            raw_bank = 0x12340000 | (bank & 65535)
            raw_row = 0x43210000 | (row & 65535)
            encoded = C.c_int16(raw_bank | (raw_row << 8)).value
            for slot in range(16):
                for value in (-0x80000000, -1, 0, 127, 128, 255, 256, 0x7FFFFFFF):
                    self.events.clear()
                    offset, stream, device, channel, previous, _ = self.prepare_record(
                        bank, row, slot, [0x81, 0], 0xFFFFFFFD)
                    initial = bytes(self.records)
                    expected = bytearray(initial)
                    self.write_expected(expected, offset, 'pitch', struct.pack('<H', value & 255), slot)
                    self.write_expected(expected, offset, 'cursor', struct.pack('P', stream + 2))
                    self.write_expected(expected, offset, 'accumulator', struct.pack('<I', 1277))
                    self.write_expected(expected, offset, 'counter', struct.pack('<i', 1280))
                    streams_before = bytes(self.streams)
                    self.function(raw_bank, raw_row, value)
                    self.assertEqual(self.events, [('voice', (encoded, device, channel, value & 255, previous), initial)])
                    self.assertEqual(bytes(self.records), bytes(expected))
                    self.assertEqual(bytes(self.streams), streams_before)
        self.assertEqual(bytes(self.table), table_before)

    def test_voice_changes_slot_cursor_accumulator_and_pitch_before_store(self):
        offset, stream, device, channel, previous, _ = self.prepare_record(0, 0, 5, [0, 0x81, 0], 99)
        initial = bytes(self.records)
        def voice(*args):
            self.events.append(('voice', args, bytes(self.records)))
            self.field(offset, 'slot', C.c_uint8).value = 9
            self.field(offset, 'cursor', C.c_void_p).value = stream + 1
            self.field(offset, 'accumulator', C.c_uint32).value = 0xFFFFFFFC
            self.field(offset, 'pitch', C.c_uint16, 5).value = 0xFEDC
            self.field(offset, 'pitch', C.c_uint16, 9).value = 0xDEAD
        self.hook('test_voice', Voice, voice)
        expected = bytearray(initial)
        self.write_expected(expected, offset, 'slot', bytes([9]))
        self.write_expected(expected, offset, 'pitch', struct.pack('<H', 0xAB), 5)
        self.write_expected(expected, offset, 'pitch', struct.pack('<H', 0xDEAD), 9)
        self.write_expected(expected, offset, 'cursor', struct.pack('P', stream + 3))
        self.write_expected(expected, offset, 'accumulator', struct.pack('<I', 1276))
        self.write_expected(expected, offset, 'counter', struct.pack('<i', 1280))
        self.function(0, 0, 0x123456AB)
        self.assertEqual(self.events, [('voice', (0, device, channel, 0xAB, previous), initial)])
        self.assertEqual(bytes(self.records), bytes(expected))

    def test_table_replacement_redirects_decoder_but_pitch_and_counter_stay_in_original_record(self):
        offset, _, device, channel, previous, _ = self.prepare_record(0, 0, 3, [0], 55)
        other, other_stream, *_ = self.prepare_record(1, 0, 1, [0x81, 0], 100)
        replacement = C.addressof(self.records) + other
        initial = bytes(self.records)
        def voice(*args):
            self.events.append(('voice', args, bytes(self.records)))
            self.table[0x4000 - 0xF98 // 4] = replacement
        self.hook('test_voice', Voice, voice)
        expected = bytearray(initial)
        self.write_expected(expected, offset, 'pitch', struct.pack('<H', 255), 3)
        self.write_expected(expected, other, 'cursor', struct.pack('P', other_stream + 2))
        self.write_expected(expected, other, 'accumulator', struct.pack('<I', 1380))
        self.write_expected(expected, offset, 'counter', struct.pack('<i', 1280))
        self.function(0, 0, 255)
        self.assertEqual(self.events, [('voice', (0, device, channel, 255, previous), initial)])
        self.assertEqual(bytes(self.records), bytes(expected))
        self.assertEqual(self.table[0x4000 - 0xF98 // 4], replacement)

    def test_real_controller_calls_pitch_handler_and_decodes_once(self):
        offset, stream, device, channel, previous, _ = self.prepare_record(1, 2, 5, [0xAB, 0x81, 0], 99)
        published = bytearray(self.records)
        self.write_expected(published, offset, 'cursor', struct.pack('P', stream + 1))
        expected = bytearray(published)
        self.write_expected(expected, offset, 'pitch', struct.pack('<H', 0xAB), 5)
        self.write_expected(expected, offset, 'cursor', struct.pack('P', stream + 3))
        self.write_expected(expected, offset, 'accumulator', struct.pack('<I', 1379))
        self.write_expected(expected, offset, 'counter', struct.pack('<i', 1280))
        self.controller(0x12340001, 0x43210002, 0x12340007)
        self.assertEqual(self.events, [('voice', (0x201, device, channel, 0xAB, previous), bytes(published))])
        self.assertEqual(bytes(self.records), bytes(expected))


if __name__ == '__main__':
    unittest.main()
