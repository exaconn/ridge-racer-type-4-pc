"""Real record logic: VLQ decoding, wrapped arithmetic and mode-based updates."""
import ctypes as C
from pathlib import Path
import random
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest


ValueUpdate = C.CFUNCTYPE(None, C.c_int32, C.c_int32, C.c_int32, C.c_int32)
StreamUpdate = C.CFUNCTYPE(C.c_int32, C.c_int32, C.c_int32, C.c_int32, C.c_int32)


class ChannelDeltaDecodeTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        directory = Path(cls.temp.name)
        root = Path(__file__).resolve().parents[2]
        header = (root / 'include/rr/library.h').read_text()
        header = re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n', '', header, flags=re.M)
        source = '''#include <stdint.h>
#include <stddef.h>
#include <string.h>
typedef uint8_t u8; typedef int8_t s8;
typedef uint16_t u16; typedef int16_t s16;
typedef uint32_t u32; typedef int32_t s32;
''' + header + '''
RrLibStateB0 *test_table[0x4100];
RrLibStateB0 test_records[9][17];
u8 test_streams[9][17][32];
u32 test_record_size = sizeof(RrLibStateB0);
u32 test_accumulator_offset = offsetof(RrLibStateB0, w88);
u32 test_mode_offset = offsetof(RrLibStateB0, w98);
u32 test_values_offset = offsetof(RrLibStateB0, h58);
u32 test_byte_offset = offsetof(RrLibStateB0, b26);
u32 test_counter_offset = offsetof(RrLibStateB0, w90);
u32 test_slot_offset = offsetof(RrLibStateB0, b17);
u32 test_slot_values_offset = offsetof(RrLibStateB0, b37);
s32 (*test_stream_update)(s32, s32, s32, s32);
s32 func_80089540(s32 a, s32 b, s32 c, s32 d) { return test_stream_update(a,b,c,d); }
void (*test_value_update)(s32, s32, s32, s32);
void func_800897A4(s32 channel, s32 left, s32 right, s32 mode) {
    test_value_update(channel, left, right, mode);
}
void test_initialize(void) {
    s32 bank;
    memset(test_records, 0xA5, sizeof(test_records));
    memset(test_streams, 0x5A, sizeof(test_streams));
    memset(test_table, 0, sizeof(test_table));
    for (bank = -4; bank <= 4; bank++)
        test_table[0x4000 - 0xF98 / 4 + bank] = &test_records[bank + 4][8];
}
void test_prepare(s32 bank, s32 row, const u8 *input, u32 length, u32 accumulator) {
    u8 *stream = test_streams[bank + 4][row + 8];
    memcpy(stream, input, length);
    test_records[bank + 4][row + 8].c0 = stream;
    test_records[bank + 4][row + 8].w88 = accumulator;
}
'''
        body = '\n'.join((root / ('src/main/PAL/func_' + name + '.c')).read_text()
                         for name in ('80081E04', '8008424C', '80080034', '80081804'))
        body = body.replace('#include "rr/library.h"', '')
        body = body.replace('extern RrLibStateB0 *D_8010F068[];', '')
        body = body.replace('D_8010F068', '(test_table + 0x4000 - 0xF98 / 4)')
        body = re.sub(r'\s+__asm__\("\$[0-9]+"\)', '', body)
        body = re.sub(r'"\$[0-9]+", ', '', body)
        body = re.sub(r' : "\$[0-9]+"', '', body)
        body = body.replace('(RrLibStateB0 **)0x80110000U', '(test_table + 0x4000)')
        source += body
        (directory / 'function.c').write_text(source)
        subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory / 'function.c'),
                        '-o', str(directory / 'function.so')], check=True, capture_output=True)
        cls.library = C.CDLL(str(directory / 'function.so'))
        cls.stride = C.c_uint32.in_dll(cls.library, 'test_record_size').value
        cls.accumulator_offset = C.c_uint32.in_dll(cls.library, 'test_accumulator_offset').value
        cls.records = (C.c_uint8 * (9 * 17 * cls.stride)).in_dll(cls.library, 'test_records')
        cls.streams = (C.c_uint8 * (9 * 17 * 32)).in_dll(cls.library, 'test_streams')
        cls.table = (C.c_void_p * 0x4100).in_dll(cls.library, 'test_table')
        cls.prepare = cls.library.test_prepare
        cls.prepare.argtypes = [C.c_int32, C.c_int32, C.c_void_p, C.c_uint32, C.c_uint32]
        cls.function = cls.library.func_80081E04
        cls.function.argtypes = [C.c_int32, C.c_int32]
        cls.function.restype = C.c_int32
        cls.mode_offset = C.c_uint32.in_dll(cls.library, 'test_mode_offset').value
        cls.values_offset = C.c_uint32.in_dll(cls.library, 'test_values_offset').value
        cls.set_values = cls.library.func_8008424C
        cls.set_values.argtypes = [C.c_int32] * 4
        cls.set_values.restype = None
        cls.read_event = cls.library.func_80080034
        cls.read_event.argtypes = [C.c_int16, C.c_int16]
        cls.read_event.restype = None
        cls.byte_offset = C.c_uint32.in_dll(cls.library, 'test_byte_offset').value
        cls.counter_offset = C.c_uint32.in_dll(cls.library, 'test_counter_offset').value
        cls.dispatch_event = cls.library.func_80081804
        cls.dispatch_event.argtypes = [C.c_int32, C.c_int32]
        cls.dispatch_event.restype = None
        cls.slot_offset = C.c_uint32.in_dll(cls.library, 'test_slot_offset').value
        cls.slot_values_offset = C.c_uint32.in_dll(cls.library, 'test_slot_values_offset').value

    def setUp(self):
        self.library.test_initialize()
        self.events = []
        self.stream_events = []
        def stream_update(*args):
            self.stream_events.append((args, bytes(self.records)))
            return -12345
        self.stream_update = StreamUpdate(stream_update)
        C.c_void_p.in_dll(self.library, 'test_stream_update').value = C.cast(self.stream_update, C.c_void_p).value
        self.value_update = ValueUpdate(lambda *args: self.events.append(args))
        C.c_void_p.in_dll(self.library, 'test_value_update').value = C.cast(self.value_update, C.c_void_p).value

    @staticmethod
    def decode(data):
        value = data[0]
        consumed = 1
        if value & 128:
            value &= 127
            while True:
                next_byte = data[consumed]
                consumed += 1
                value = ((value << 7) + (next_byte & 127)) & 0xFFFFFFFF
                if not next_byte & 128:
                    break
        return (value * 10) & 0xFFFFFFFF, consumed

    def check(self, data, bank, row, accumulator):
        input_buffer = (C.c_uint8 * len(data))(*data)
        self.prepare(bank, row, input_buffer, len(data), accumulator)
        record_index = (bank + 4) * 17 + row + 8
        offset = record_index * self.stride
        stream = C.addressof(self.streams) + record_index * 32
        expected = bytearray(self.records)
        streams_before = bytes(self.streams)
        result, consumed = self.decode(data)
        expected[offset:offset + C.sizeof(C.c_void_p)] = struct.pack('P', stream + consumed)
        expected[offset + self.accumulator_offset:offset + self.accumulator_offset + 4] = struct.pack('<I', (accumulator + result) & 0xFFFFFFFF)
        self.assertEqual(self.function(bank, row), C.c_int32(result).value)
        self.assertEqual(bytes(self.records), bytes(expected))
        self.assertEqual(bytes(self.streams), streams_before)

    def test_direct_mode_updates_only_two_halfwords(self):
        values = [-2147483648, -65537, -32768, -1, 0, 1, 32767, 65535, 2147483647]
        table_before = bytes(self.table)
        for bank, row in [(-4, -8), (0, 0), (4, 8)]:
            offset = ((bank + 4) * 17 + row + 8) * self.stride
            mode = C.c_int32.from_address(C.addressof(self.records) + offset + self.mode_offset)
            for mode_value in (-2147483648, -1, 0, 2, 2147483647):
                mode.value = mode_value
                for left in values:
                    for right in values:
                        expected = bytearray(self.records)
                        expected[offset + self.values_offset:offset + self.values_offset + 4] = struct.pack('<HH', left & 65535, right & 65535)
                        self.set_values(bank + 0x20000, row + 0x30000, left, right)
                        self.assertEqual(bytes(self.records), bytes(expected))
        self.assertEqual(self.events, [])
        self.assertEqual(bytes(self.table), table_before)

    def test_callback_mode_masks_arguments_and_does_not_write_record(self):
        values = [-2147483648, -65537, -1, 0, 1, 65535, 2147483647]
        for bank, row in [(-4, -8), (0, 0), (4, 8)]:
            offset = ((bank + 4) * 17 + row + 8) * self.stride
            C.c_int32.from_address(C.addressof(self.records) + offset + self.mode_offset).value = 1
            for left in values:
                for right in values:
                    self.events.clear()
                    before = bytes(self.records)
                    self.set_values(bank + 0x20000, row + 0x30000, left, right)
                    self.assertEqual(bytes(self.records), before)
                    self.assertEqual(self.events, [(C.c_int16(bank | (row << 8)).value,
                                                   left & 65535, right & 65535, 1)])

    def test_callback_changes_are_not_overwritten_after_return(self):
        offset = (4 * 17 + 8) * self.stride
        record = C.addressof(self.records) + offset
        C.c_int32.from_address(record + self.mode_offset).value = 1
        self.value_update = ValueUpdate(lambda *args: C.memset(record + self.values_offset, 0x12, 4))
        C.c_void_p.in_dll(self.library, 'test_value_update').value = C.cast(self.value_update, C.c_void_p).value
        before = bytearray(self.records)
        before[offset + self.values_offset:offset + self.values_offset + 4] = bytes([0x12]) * 4
        self.set_values(0, 0, -1, -2)
        self.assertEqual(bytes(self.records), bytes(before))

    def test_every_first_byte_and_terminating_second_byte(self):
        table_before = bytes(self.table)
        for first in range(256):
            for second in range(128):
                bank, row = [(-4, -8), (0, 0), (4, 8)][first % 3]
                accumulator = [0, 0x7FFFFFFF, 0xFFFFFFFF][second % 3]
                self.check([first, second, 0x5A], bank, row, accumulator)
        self.assertEqual(bytes(self.table), table_before)

    def test_long_sequences_and_32_bit_overflow(self):
        rng = random.Random(0x81E04)
        for count in range(2, 12):
            for _ in range(100):
                data = [rng.randrange(128, 256) for _ in range(count - 1)] + [rng.randrange(128)]
                self.check(data, rng.randrange(-4, 5), rng.randrange(-8, 9), rng.getrandbits(32))
        self.check([0xFF] * 10 + [0x7F], -4, 8, 0xFFFFFFFF)

    def test_consecutive_values_advance_the_same_cursor(self):
        data = [0, 1, 0x81, 0, 0xFF, 0x7F, 0x80, 0]
        buffer = (C.c_uint8 * len(data))(*data)
        self.prepare(0, 0, buffer, len(data), 0xFFFFFFFE)
        index = 4 * 17 + 8
        record = C.addressof(self.records) + index * self.stride
        stream = C.addressof(self.streams) + index * 32
        accumulator = 0xFFFFFFFE
        consumed = 0
        for _ in range(5):
            result, length = self.decode(data[consumed:])
            self.assertEqual(self.function(0, 0), C.c_int32(result).value)
            consumed += length
            accumulator = (accumulator + result) & 0xFFFFFFFF
            self.assertEqual(C.c_void_p.from_address(record).value, stream + consumed)
            self.assertEqual(C.c_uint32.from_address(record + self.accumulator_offset).value, accumulator)
        self.assertEqual(consumed, len(data))


    def test_event_byte_then_real_delta_decoder_preserve_other_fields(self):
        payloads = [[0], [0x81, 0], [0xFF] * 10 + [0x7F]]
        table_before = bytes(self.table)
        for metadata in range(256):
            for bank, row in [(-4, -8), (0, 0), (4, 8)]:
                payload = payloads[metadata % len(payloads)]
                data = [metadata] + payload
                accumulator = [0, 0x7FFFFFFF, 0xFFFFFFFF][metadata % 3]
                buffer = (C.c_uint8 * len(data))(*data)
                self.prepare(bank, row, buffer, len(data), accumulator)
                index = (bank + 4) * 17 + row + 8
                offset = index * self.stride
                stream = C.addressof(self.streams) + index * 32
                expected = bytearray(self.records)
                streams_before = bytes(self.streams)
                result, consumed = self.decode(payload)
                expected[offset:offset + C.sizeof(C.c_void_p)] = struct.pack('P', stream + 1 + consumed)
                expected[offset + self.byte_offset] = metadata
                expected[offset + self.counter_offset:offset + self.counter_offset + 4] = struct.pack('<I', result)
                expected[offset + self.accumulator_offset:offset + self.accumulator_offset + 4] = struct.pack('<I', (accumulator + result) & 0xFFFFFFFF)
                self.read_event(bank, row)
                self.assertEqual(bytes(self.records), bytes(expected))
                self.assertEqual(bytes(self.streams), streams_before)
        self.assertEqual(bytes(self.table), table_before)
        self.assertEqual(self.events, [])

    def test_consecutive_events_share_one_stream_cursor(self):
        data = [0x80, 1, 0x7F, 0x81, 0, 0xFF, 0]
        buffer = (C.c_uint8 * len(data))(*data)
        self.prepare(0, 0, buffer, len(data), 0xFFFFFFFC)
        index = 4 * 17 + 8
        offset = index * self.stride
        stream = C.addressof(self.streams) + index * 32
        consumed, accumulator = 0, 0xFFFFFFFC
        for metadata, result, length in [(0x80, 10, 2), (0x7F, 1280, 3), (0xFF, 0, 2)]:
            expected = bytearray(self.records)
            consumed += length
            accumulator = (accumulator + result) & 0xFFFFFFFF
            expected[offset:offset + C.sizeof(C.c_void_p)] = struct.pack('P', stream + consumed)
            expected[offset + self.byte_offset] = metadata
            expected[offset + self.counter_offset:offset + self.counter_offset + 4] = struct.pack('<I', result)
            expected[offset + self.accumulator_offset:offset + self.accumulator_offset + 4] = struct.pack('<I', accumulator)
            self.read_event(0, 0)
            self.assertEqual(bytes(self.records), bytes(expected))
        self.assertEqual(consumed, len(data))


    def test_stream_dispatch_publishes_cursor_before_callback_and_decodes_after(self):
        payloads = [[0], [0x81, 0], [0xFF] * 10 + [0x7F]]
        for value in range(256):
            for bank, row, raw_bank, raw_row in [(0, 0, 0x12340000, 0x43210000),
                                                 (-4, -8, -4, -8), (4, 8, 4, 8)]:
                self.stream_events.clear()
                payload = payloads[value % 3]
                data = [value] + payload
                buffer = (C.c_uint8 * len(data))(*data)
                accumulator = 0xFFFFFFFE
                self.prepare(bank, row, buffer, len(data), accumulator)
                index = (bank + 4) * 17 + row + 8
                offset = index * self.stride
                stream = C.addressof(self.streams) + index * 32
                slot = value % 16
                metadata = [-128, -1, 0, 127][value % 4]
                self.records[offset + self.byte_offset] = metadata & 255
                self.records[offset + self.slot_offset] = slot
                for i in range(16):
                    self.records[offset + self.slot_values_offset + i] = (i * 17 + 5) & 255
                selected = (slot * 17 + 5) & 255
                published = bytearray(self.records)
                published[offset:offset + C.sizeof(C.c_void_p)] = struct.pack('P', stream + 1)
                expected = bytearray(published)
                result, consumed = self.decode(payload)
                expected[offset:offset + C.sizeof(C.c_void_p)] = struct.pack('P', stream + 1 + consumed)
                expected[offset + self.counter_offset:offset + self.counter_offset + 4] = struct.pack('<I', result)
                expected[offset + self.accumulator_offset:offset + self.accumulator_offset + 4] = struct.pack('<I', (accumulator + result) & 0xFFFFFFFF)
                streams_before = bytes(self.streams)
                self.dispatch_event(raw_bank, raw_row)
                encoded = C.c_int16(raw_bank | (raw_row << 8)).value
                self.assertEqual(self.stream_events, [((encoded, metadata, selected, value), bytes(published))])
                self.assertEqual(bytes(self.records), bytes(expected))
                self.assertEqual(bytes(self.streams), streams_before)

    def test_stream_callback_changes_cursor_and_accumulator_before_real_decode(self):
        data = [0xAA, 0x7E, 0x81, 0]
        buffer = (C.c_uint8 * len(data))(*data)
        self.prepare(0, 0, buffer, len(data), 99)
        index = 4 * 17 + 8
        offset = index * self.stride
        stream = C.addressof(self.streams) + index * 32
        self.records[offset + self.byte_offset] = 255
        self.records[offset + self.slot_offset] = 7
        self.records[offset + self.slot_values_offset + 7] = 200
        def mutate(*args):
            self.stream_events.append((args, C.c_void_p.from_buffer(self.records, offset).value))
            C.c_void_p.from_buffer(self.records, offset).value = stream + 2
            C.c_uint32.from_buffer(self.records, offset + self.accumulator_offset).value = 0xFFFFFFFC
            C.c_int32.from_buffer(self.records, offset + self.counter_offset).value = -444
            self.records[offset + self.byte_offset] = 127
            return 0x7FFFFFFF
        self.stream_update = StreamUpdate(mutate)
        C.c_void_p.in_dll(self.library, 'test_stream_update').value = C.cast(self.stream_update, C.c_void_p).value
        expected = bytearray(self.records)
        expected[offset:offset + C.sizeof(C.c_void_p)] = struct.pack('P', stream + 4)
        expected[offset + self.byte_offset] = 127
        expected[offset + self.counter_offset:offset + self.counter_offset + 4] = struct.pack('<I', 1280)
        expected[offset + self.accumulator_offset:offset + self.accumulator_offset + 4] = struct.pack('<I', (0xFFFFFFFC + 1280) & 0xFFFFFFFF)
        self.dispatch_event(0, 0)
        self.assertEqual(self.stream_events, [((0, -1, 200, 0xAA), stream + 1)])
        self.assertEqual(bytes(self.records), bytes(expected))

    def test_stream_callback_replaces_table_for_decoder_but_preserves_output_record(self):
        first = (C.c_uint8 * 2)(0x55, 0)
        other = (C.c_uint8 * 2)(0x81, 0)
        self.prepare(0, 0, first, 2, 99)
        self.prepare(1, 0, other, 2, 55)
        old_index, new_index = 4 * 17 + 8, 5 * 17 + 8
        old_offset, new_offset = old_index * self.stride, new_index * self.stride
        self.records[old_offset + self.slot_offset] = 0
        replacement = C.addressof(self.records) + new_offset
        def replace(*args):
            self.stream_events.append(args)
            self.table[0x4000 - 0xF98 // 4] = replacement
            return -999
        self.stream_update = StreamUpdate(replace)
        C.c_void_p.in_dll(self.library, 'test_stream_update').value = C.cast(self.stream_update, C.c_void_p).value
        expected = bytearray(self.records)
        old_stream = C.addressof(self.streams) + old_index * 32
        new_stream = C.addressof(self.streams) + new_index * 32
        expected[old_offset:old_offset + C.sizeof(C.c_void_p)] = struct.pack('P', old_stream + 1)
        expected[new_offset:new_offset + C.sizeof(C.c_void_p)] = struct.pack('P', new_stream + 2)
        expected[old_offset + self.counter_offset:old_offset + self.counter_offset + 4] = struct.pack('<I', 1280)
        expected[new_offset + self.accumulator_offset:new_offset + self.accumulator_offset + 4] = struct.pack('<I', 55 + 1280)
        self.dispatch_event(0, 0)
        self.assertEqual(len(self.stream_events), 1)
        self.assertEqual(bytes(self.records), bytes(expected))
        self.assertEqual(self.table[0x4000 - 0xF98 // 4], replacement)


if __name__ == '__main__':
    unittest.main()
