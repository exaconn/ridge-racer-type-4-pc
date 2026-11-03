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


class State(C.Structure):
    _fields_ = [
        ('pad0', C.c_char * 4), ('group_data', C.POINTER(U8)),
        ('pad8', C.c_char * (0x20 - 8)), ('slot_groups', C.POINTER(U8)),
        ('pad24', C.c_char * (0x46 - 0x24)), ('status', U8),
        ('pad47', C.c_char * (0x5D - 0x47)), ('assignments', U8 * 6),
        ('pad63', C.c_char * (0xE9 - 0x63)), ('group_count', U8),
    ]


class SlotAssignmentTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        directory = Path(cls.temp.name)
        root = Path(__file__).resolve().parents[2]
        source = (root / 'tools/scripts/fixtures/views/func_80099404.c').read_text()
        source = re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n', '', source, flags=re.M)
        source = '#include <stdint.h>\ntypedef uint8_t u8;\ntypedef int32_t s32;\n' + source
        (directory / 'function.c').write_text(source)
        subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory / 'function.c'),
                        '-o', str(directory / 'function.so')], check=True, capture_output=True)
        cls.library = C.CDLL(str(directory / 'function.so'))
        cls.function = cls.library.func_80099404
        cls.function.argtypes = [C.POINTER(State)]
        cls.function.restype = C.c_int32

    def check_assignment(self, slots, limits):
        state = State()
        C.memset(C.addressof(state), 0xA5, C.sizeof(state))
        groups = (U8 * max(1, len(limits) * 5))()
        for group, limit in enumerate(limits):
            groups[group * 5 + 2] = limit
        slot_buffer = (U8 * 6)(*slots)
        state.group_data = groups
        state.slot_groups = slot_buffer
        state.group_count = len(limits)
        original = [0x90 + i for i in range(6)]
        state.assignments[:] = original
        before = C.string_at(C.addressof(state), C.sizeof(state))

        # Independent group-level specification, rather than the recovered loops.
        expected = [original[i] if group >= len(limits) else
                    (group if slots.count(group) >= max(1, limits[group]) else 0xFF)
                    for i, group in enumerate(slots)]
        self.assertEqual(self.function(C.byref(state)), 0)
        self.assertEqual(list(state.assignments), expected)
        self.assertEqual(state.status, 0xFE)
        self.assertEqual(list(slot_buffer), slots)
        self.assertEqual([groups[g * 5 + 2] for g in range(len(limits))], limits)
        after = C.string_at(C.addressof(state), C.sizeof(state))
        allowed = {State.status.offset, *range(State.assignments.offset,
                                             State.assignments.offset + 6)}
        self.assertTrue(all(a == b for i, (a, b) in enumerate(zip(before, after))
                            if i not in allowed), 'unexpected state write')

    def test_zero_groups_does_not_dereference_inputs(self):
        state = State()
        state.assignments[:] = range(6)
        self.assertEqual(self.function(C.byref(state)), 0)
        self.assertEqual(list(state.assignments), list(range(6)))
        self.assertEqual(state.status, 0xFE)

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
