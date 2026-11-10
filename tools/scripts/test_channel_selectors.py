"""Real SDK bank selectors with public headers and complete native records."""
from pathlib import Path
import subprocess
from tempfile import TemporaryDirectory
import unittest

ROOT = Path(__file__).resolve().parents[2]

HARNESS = r'''
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "psyq/libpad/padstop.h"
#include "rr/channel_init.h"

PadChannel PadChannelRecordArray[2];
static PadChannel unrelated;

int main(void) {
    static const u32 upper_words[] = {
        0, 1, 0x80, 0x100, 0x8000, 0xFFFF, 0x5555, 0xAAAA
    };
    unsigned char before[sizeof(PadChannelRecordArray)];
    ChannelRecordCallback lookup = _padPortToChannel;
    s32 (*reverse)(void *) = _padChannelToPort;
    u32 high, low;
    size_t offset;

    /* The old PAL byte step must not accidentally select bank one on LP64. */
    assert(sizeof(void *) > 4);
    assert(sizeof(PadChannel) > 0xF0);
    assert((u8 *)&PadChannelRecordArray[1] !=
           (u8 *)PadChannelRecordArray + 0xF0);
    memset(PadChannelRecordArray, 0xA5, sizeof(PadChannelRecordArray));
    memcpy(before, PadChannelRecordArray, sizeof(before));

    for (high = 0; high < sizeof(upper_words)/sizeof(upper_words[0]); high++) {
        for (low = 0; low < 65536; low++) {
            u32 bits = (upper_words[high] << 16) | low;
            s32 selector;
            PadChannel *expected;
            memcpy(&selector, &bits, sizeof(selector));
            expected = &PadChannelRecordArray[(low & 0xF0) != 0];
            assert(lookup(selector) == expected);
            assert(reverse(lookup(selector)) == ((low & 0xF0) ? 0x20 : 0x10));
        }
    }

    /* Only complete bank bases are accepted, including through the status prefix. */
    for (offset = 0; offset < sizeof(PadChannelRecordArray); offset++) {
        s32 expected = offset == 0 ? 0x10 :
                       offset == sizeof(PadChannel) ? 0x20 : 0xFF;
        assert(reverse((u8 *)PadChannelRecordArray + offset) == expected);
    }
    assert(reverse(&PadChannelRecordArray[0].modes00) == 0x10);
    assert(reverse(&PadChannelRecordArray[1].modes00) == 0x20);
    assert(reverse(PadChannelRecordArray + 2) == 0xFF);
    assert(reverse(&unrelated) == 0xFF);
    assert(reverse(0) == 0xFF);
    assert(!memcmp(before, PadChannelRecordArray, sizeof(before)));
    puts("524288 selector cases; every bank byte; native pointers and callbacks preserved");
    return 0;
}
'''


class ChannelSelectorsTest(unittest.TestCase):
    def test_actual_sdk_selectors_with_complete_native_banks(self):
        with TemporaryDirectory() as directory:
            source = Path(directory)/'selectors.c'
            executable = Path(directory)/'selectors'
            source.write_text(HARNESS)
            compiled = subprocess.run([
                'cc', '-std=gnu99', '-O2', '-Wall', '-Wextra', '-Werror',
                '-fsanitize=address,undefined', '-fno-sanitize-recover=all',
                '-I', str(ROOT/'include'), str(source),
                str(ROOT/'src/main/PAL/psyq/libpad/padstop.c'),
                '-o', str(executable)
            ], capture_output=True, text=True)
            self.assertEqual(compiled.returncode, 0, compiled.stderr)
            result = subprocess.run([str(executable)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertIn('524288 selector cases', result.stdout)
