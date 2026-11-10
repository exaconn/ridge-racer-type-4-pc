"""Exercise the recovered C on the host; fd.py checks the actual PSX layout/code.

Only target layout assertions and the fixed-width type include are replaced for
the host build. The function body and its structure declaration are unchanged.
"""
import ctypes as C
from pathlib import Path
import random
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest


U8 = C.c_uint8


from channel_native_types import PadChannel, Output, OutputFields
from c_functions import source_owners, definition_source


class SlotAssignmentTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        directory = Path(cls.temp.name)
        root = Path(__file__).resolve().parents[2]
        owners=source_owners(root/'src/main/PAL')
        source=definition_source(owners['_padActAlignComplete'].read_text(),'_padActAlignComplete')
        source = re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n', '', source, flags=re.M)
        source = '#include \"rr/channels.h\"\n' + source
        (directory / 'function.c').write_text(source)
        subprocess.run(['cc', '-shared', '-fPIC', '-O2', '-I', str(root/'include'), str(directory / 'function.c'),
                        '-o', str(directory / 'function.so')], check=True, capture_output=True)
        cls.library = C.CDLL(str(directory / 'function.so'))
        cls.function = cls.library._padActAlignComplete
        cls.function.argtypes = [C.POINTER(PadChannel)]
        cls.function.restype = C.c_int32

    def check_assignment(self, slots, limits):
        state = PadChannel()
        C.memset(C.addressof(state), 0xA5, C.sizeof(state))
        groups = (U8 * max(1, len(limits) * 5))()
        for group, limit in enumerate(limits):
            groups[group * 5 + 2] = limit
        slot_buffer = (U8 * 6)(*slots)
        state.entries04 = C.addressof(groups)
        state.layout20 = C.addressof(slot_buffer)
        state.entry_countE9 = len(limits)
        original = [0x90 + i for i in range(6)]
        state.output54.fields.payload[:] = original
        before = C.string_at(C.addressof(state), C.sizeof(state))

        # Independent group-level specification, rather than the recovered loops.
        expected = [original[i] if group >= len(limits) else
                    (group if slots.count(group) >= max(1, limits[group]) else 0xFF)
                    for i, group in enumerate(slots)]
        self.assertEqual(self.function(C.byref(state)), 0)
        self.assertEqual(list(state.output54.fields.payload), expected)
        self.assertEqual(state.phase, 0xFE)
        self.assertEqual(list(slot_buffer), slots)
        self.assertEqual([groups[g * 5 + 2] for g in range(len(limits))], limits)
        after = C.string_at(C.addressof(state), C.sizeof(state))
        allowed = {PadChannel.phase.offset, *range((PadChannel.output54.offset+OutputFields.payload.offset),
                                             (PadChannel.output54.offset+OutputFields.payload.offset) + 6)}
        self.assertTrue(all(a == b for i, (a, b) in enumerate(zip(before, after))
                            if i not in allowed), 'unexpected state write')

    def test_zero_groups_does_not_dereference_inputs(self):
        state = PadChannel()
        state.output54.fields.payload[:] = range(6)
        self.assertEqual(self.function(C.byref(state)), 0)
        self.assertEqual(list(state.output54.fields.payload), list(range(6)))
        self.assertEqual(state.phase, 0xFE)

    def test_threshold_boundaries_and_unrecognised_slots(self):
        for minimum in (0, 1, 5, 6, 7, 255):
            self.check_assignment([0] * 6, [minimum])
        self.check_assignment([0, 0, 1, 1, 1, 2], [3, 2, 0])
        self.check_assignment([0, 1, 2, 3, 254, 255], [0, 2, 1])
        self.check_assignment([254] * 6, [255] * 254 + [6])

    def test_deterministic_random_inputs(self):
        randomizer = random.Random(99404)
        for _ in range(1000):
            count = randomizer.choice([0, 1, 2, 6, 24, 255])
            slots = [randomizer.randrange(max(1, count + 1)) for _ in range(6)]
            limits = [randomizer.randrange(9) for _ in range(count)]
            self.check_assignment(slots, limits)


if __name__ == '__main__':
    unittest.main()
