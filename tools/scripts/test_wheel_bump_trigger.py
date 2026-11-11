"""Wheel event dispatch against native C, original MIPS and the linked function.

The full vehicle owner lives above 4 GiB in the native harness. LookupTrackEvent
is the sole service adapter: it records signed selectors and may change vehicle
input before the bump calculation. Target multiplication wraps at 32 bits.
"""
import ctypes as C
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from c_functions import address_of, definition_source, source_owners
from mips_preview_engine import Mips, signed

ROOT = Path(__file__).resolve().parents[2]
PREFIX = 0x1F4
VEHICLE, STACK, STOP = 0x80180010, 0x803F0000, 0x803FF000

NATIVE = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "game/car/carwheel.h"
#include "game/course/trkevent.h"
static struct {
    u32 before[4]; RaceVehicle vehicle; u32 after[4];
} world;
static s32 lookupResult, replaceInput, nextInput;
static u32 calls;
static s32 arguments[2];
static u8 boundary[0x1F4];
s32 LookupTrackEvent(s32 selection, s32 key) {
    assert(calls++ == 0);
    arguments[0] = selection; arguments[1] = key;
    memcpy(boundary, &world.vehicle, sizeof(boundary));
    if (replaceInput) world.vehicle.input = nextInput;
    return lookupResult;
}
'''
TAIL = r'''
s32 run_trigger(const u8 *input, u8 *output, s32 wheel, s32 result,
                s32 replace, s32 changedInput, u32 *count, s32 *args, u8 *atCall) {
    RaceVehicle initial;
    u32 i;
    s32 active;
    memset(&world, 0xA5, sizeof(world));
    memcpy(&world.vehicle, input, 0x1F4);
    world.vehicle.control.record18 = NULL;
    world.vehicle.control.handling1C = NULL;
    memcpy(&initial, &world.vehicle, sizeof(initial));
    assert(sizeof(void *) == 8 && (uintptr_t)&world.vehicle > UINT32_MAX);
    assert(offsetof(RaceVehicle, prevWheelNodes100) == 0x100);
    assert(offsetof(RaceVehicle, wheelBumpState1BC) == 0x1BC);
    assert(offsetof(RaceVehicle, wheelBumpVel1C4) == 0x1C4);
    assert(offsetof(RaceVehicle, input) == 0x1D8);
    lookupResult = result; replaceInput = replace; nextInput = changedInput;
    calls = 0; memset(arguments, 0, sizeof(arguments));
    memset(boundary, 0, sizeof(boundary));
    active = TriggerWheelBump(&world.vehicle, wheel);
    memcpy(output, &world.vehicle, 0x1F4);
    memcpy(atCall, boundary, sizeof(boundary));
    memcpy(args, arguments, sizeof(arguments)); *count = calls;
    assert(!memcmp((u8 *)&world.vehicle + 0x1F4, (u8 *)&initial + 0x1F4,
                   sizeof(initial) - 0x1F4));
    for (i = 0; i < 4; i++)
        assert(world.before[i] == 0xA5A5A5A5 && world.after[i] == 0xA5A5A5A5);
    return active;
}
'''


class WheelBumpTriggerTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        owner = source_owners(ROOT/'src/main/PAL')['TriggerWheelBump']
        body = definition_source(owner.read_text(), 'TriggerWheelBump')
        if 'asm' in body:
            raise AssertionError('TriggerWheelBump must compile without host rewrites')
        cls.temporary = TemporaryDirectory()
        cls.addClassCleanup(cls.temporary.cleanup)
        directory = Path(cls.temporary.name)
        source, library = directory/'trigger.c', directory/'trigger.dylib'
        source.write_text(NATIVE + body + TAIL)
        built = subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra',
                                '-Werror', '-fsanitize=undefined,bounds',
                                '-fno-sanitize-recover=all', '-I', str(ROOT/'include'),
                                str(source), '-o', str(library)], capture_output=True, text=True)
        if built.returncode:
            raise AssertionError(built.stderr)
        cls.library = C.CDLL(str(library))
        cls.library.run_trigger.argtypes = [C.c_void_p, C.c_void_p, C.c_int32,
                                          C.c_int32, C.c_int32, C.c_int32,
                                          C.c_void_p, C.c_void_p, C.c_void_p]
        cls.library.run_trigger.restype = C.c_int32

    def test_wheel_indices_thresholds_signed_events_and_wrapping_products(self):
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0, 0x400000)
        read = lambda address, size: bytes(memory.mem_read(address & 0x1FFFFFFF, size))
        write = lambda address, data: memory.mem_write(address & 0x1FFFFFFF, bytes(data))
        start, lookup = address_of('TriggerWheelBump'), address_of('LookupTrackEvent')
        original = (ROOT/'assets/PAL/main.exe').read_bytes()
        linked = (ROOT/'build/PAL/main.exe').read_bytes()
        base = struct.unpack_from('<I', original, 0x18)[0]
        size = next(int(row.split()[2], 16) for row in
                    (ROOT/'configs/PAL/functions.txt').read_text().splitlines()
                    if row.split()[0] == 'TriggerWheelBump')
        images = [(name, image[0x800+start-base:0x800+start-base+size])
                  for name, image in [('original', original), ('linked', linked)]]
        randomizer = random.Random(0xB04F)
        speeds = (-32768, -1, 0, 1, 798, 799, 800, 801, 2800, 32767)
        events = (-2147483648, -1000000, -2801, -2, -1, 0, 1, 2, 2799,
                  2800, 2801, 1000000, 2147483647)
        cases = [(wheel, speed, event, replace) for wheel in range(4)
                 for speed in speeds for event in events for replace in (False, True)]
        cases += [(wheel, randomizer.randrange(-32768, 32768),
                   signed(randomizer.getrandbits(32)), bool(i & 1))
                  for wheel in range(4) for i in range(128)]
        outcomes = set()

        def store(cpu, access, address, width, value, user):
            address &= 0x1FFFFFFF
            vehicle = VEHICLE & 0x1FFFFFFF; stack = STACK & 0x1FFFFFFF
            self.assertTrue(any(first <= address and address + width <= last
                                for first, last in ((vehicle, vehicle+PREFIX),
                                                    (stack-0x100, stack))),
                            (case, hex(cpu.pc), hex(address), width))

        cpu = Mips(memory, 0, 100, 101, 102, store)
        for case, (wheel, speed, event, replace) in enumerate(cases):
            vehicle = bytearray(randomizer.randbytes(PREFIX))
            changed_input = speeds[(case//4) % len(speeds)]
            struct.pack_into('<h', vehicle, 0x1D8, speed)
            selector = (-32768, -1, 0, 6, 32767)[case % 5]
            struct.pack_into('<h', vehicle, 0x1EA, selector)
            nodes = (-32768, -1, 0, 32767)
            struct.pack_into('<4h', vehicle, 0x100, *nodes)
            model = bytearray(vehicle)
            count = int(speed >= 800)
            active = int(bool(count) and event != -1)
            if count and replace:
                struct.pack_into('<h', model, 0x1D8, changed_input)
            if active:
                product = signed(event * (changed_input if replace else speed))
                quotient = abs(product)//2800 * (-1 if product < 0 else 1)
                struct.pack_into('<H', model, 0x1C4+wheel*2, (-quotient) & 0xFFFF)
                struct.pack_into('<h', model, 0x1BC+wheel*2, 1)
                bits = struct.unpack_from('<H', model, 0x1B8)[0] ^ (1 << wheel)
                struct.pack_into('<H', model, 0x1B8, bits)
            expected_args = (selector, nodes[wheel]) if count else (0, 0)
            boundary = bytes(vehicle) if count else bytes(PREFIX)
            outcomes.add((wheel, count, active, replace))
            native_in, native_out = C.create_string_buffer(bytes(vehicle)), C.create_string_buffer(PREFIX)
            native_count, native_args = C.c_uint32(), (C.c_int32*2)()
            native_boundary = C.create_string_buffer(PREFIX)
            actual = self.library.run_trigger(native_in, native_out, wheel, event, replace,
                                             changed_input, C.byref(native_count), native_args,
                                             native_boundary)
            self.assertEqual((actual, native_count.value, tuple(native_args)),
                             (active, count, expected_args), case)
            self.assertEqual(native_out.raw, bytes(model), (case, 'native vehicle'))
            self.assertEqual(native_boundary.raw, boundary, (case, 'native service boundary'))
            for name, instructions in images:
                write(start, instructions)
                write(VEHICLE-16, b'\xA6'*16+vehicle+b'\x6A'*16)
                write(STACK-0x110, b'\xA5'*0x120)
                cpu.reset(); cpu.r = [0]+[0xBEEF0000+i for i in range(1, 32)]
                cpu.r[4], cpu.r[5], cpu.r[29], cpu.r[31], cpu.pc = VEHICLE, wheel, STACK, STOP, start
                calls = 0
                for step in range(200):
                    if cpu.pc == STOP:
                        break
                    if cpu.pc != lookup:
                        cpu.step(); continue
                    calls += 1
                    self.assertEqual(calls, count, (case, name))
                    self.assertEqual((signed(cpu.r[4]), signed(cpu.r[5])), expected_args, (case, name))
                    self.assertEqual(read(VEHICLE, PREFIX), boundary, (case, name, 'service boundary'))
                    if replace:
                        write(VEHICLE+0x1D8, struct.pack('<h', changed_input))
                    resume = cpu.r[31]
                    for register in (2, 3, *range(4, 16), 24, 25):
                        cpu.r[register] = 0xDEAD0000+register
                    cpu.r[2], cpu.hi, cpu.lo, cpu.pc = event & 0xFFFFFFFF, 0x12345678, 0x87654321, resume
                self.assertEqual((cpu.pc, cpu.r[2], calls), (STOP, active, count), (case, name))
                self.assertEqual(read(VEHICLE, PREFIX), bytes(model), (case, name, 'vehicle'))
                self.assertEqual((cpu.r[29], cpu.r[31]), (STACK, STOP), (case, name))
                for register in (*range(16, 24), 28, 30):
                    self.assertEqual(cpu.r[register], 0xBEEF0000+register, (case, name, register))
                self.assertEqual(read(VEHICLE-16, 16), b'\xA6'*16)
                self.assertEqual(read(VEHICLE+PREFIX, 16), b'\x6A'*16)
                self.assertEqual(read(STACK-0x110, 16), b'\xA5'*16)
                self.assertEqual(read(STACK, 16), b'\xA5'*16)
        self.assertEqual(outcomes, {(wheel, count, active, replace) for wheel in range(4)
                                   for count, active in ((0, 0), (1, 0), (1, 1))
                                   for replace in (False, True)})


if __name__ == '__main__':
    unittest.main()
