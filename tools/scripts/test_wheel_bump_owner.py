"""Current wheel suspension body against original and linked MIPS.

The native harness uses complete vehicle/contact owners above 4 GiB. Scratchpad
addresses are adapted to those owners; the five existing compiler constraints
are removed only for host compilation. Trigger/impact/angle services have named,
stateful adapters, so this tests the suspension body, not those three services.
Inputs keep products/sums in signed range and use nonzero wheelbase divisors.
"""
import ctypes as C
from pathlib import Path
import random
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from c_functions import address_of, definition_source, source_owners
from mips_preview_engine import Mips, signed

ROOT = Path(__file__).resolve().parents[2]
VEHICLE = 0x80180010
CONTACT = 0x1F800080
PREFIX_SIZE = 0x1F4
CONTACT_SIZE = 0x260
STATE_SIZE = PREFIX_SIZE + CONTACT_SIZE + 4

NATIVE = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "game/car/carsp.h"
#include "game/car/carmove1.h"
#include "game/car/carwheel.h"
#include "game/car/carstate.h"
#include "game/math/rotmat.h"
enum {PREFIX=0x1F4, CONTACT=0x260, STATE=PREFIX+CONTACT+4};
static struct {
    u32 beforeVehicle[4]; RaceVehicle vehicle; u32 afterVehicle[4];
    u32 beforeContact[4]; CarTrackWork contact; u32 afterContact[4];
    s32 bank;
} world;
static unsigned scenario, calls;
static u8 *trace;
#define SCRATCHPAD ((uintptr_t)&world.bank - 0x7CU)
#define SCRATCH_CAR_WORK (&world.contact)
static void snapshot(u8 *out) {
    u32 pointer=0x80180010;
    assert(world.contact.vehicle00==&world.vehicle);
    memcpy(out,&world.vehicle,PREFIX);out+=PREFIX;
    memcpy(out,&pointer,4);out+=4;
    memcpy(out,&world.contact.w4,CONTACT-4);out+=CONTACT-4;
    memcpy(out,&world.bank,4);
}
static void event(u32 kind,s32 first,s32 second) {
    u32 words[3]={kind,(u32)first,(u32)second};
    assert(calls<16);memcpy(trace,words,12);snapshot(trace+12);
    trace+=12+STATE;calls++;
}
int TriggerWheelBump(RaceVehicle *vehicle,int wheel) {
    unsigned active=(scenario>>(wheel+1))&1;
    assert(vehicle==&world.vehicle&&wheel>=0&&wheel<4);
    event(1,wheel,0);
    if(active){vehicle->wheelBumpState1BC[wheel]=1;
               vehicle->wheelBumpVel1C4[wheel]=-(s16)(5+wheel);}
    return active;
}
void SetCarImpactState(int state,RaceVehicle *vehicle) {
    assert(state==1&&vehicle==&world.vehicle);event(2,state,0);
    vehicle->shakeActive19C=state;
}
s32 Atan2Table(int first,int second) {
    event(3,first,second);
    return (((u32)first*13U)^((u32)second*7U))&0x7FF;
}
'''

TAIL = r'''
void run_wheels(unsigned number,const u8 *input,u8 *output) {
    unsigned i;RaceVehicle before;CarTrackWork oldContact;
    scenario=number;calls=0;trace=output+STATE+4;
    memset(&world,0xA5,sizeof(world));
    memcpy(&world.vehicle,input,PREFIX);input+=PREFIX;
    world.vehicle.control.record18=NULL;world.vehicle.control.handling1C=NULL;
    memcpy(&world.contact.w4,input+4,CONTACT-4);input+=CONTACT;
    world.contact.vehicle00=&world.vehicle;memcpy(&world.bank,input,4);
    assert(sizeof(void*)==8&&(uintptr_t)&world.vehicle>UINT32_MAX);
    assert((uintptr_t)&world.contact>UINT32_MAX);
    assert(offsetof(CarTrackWork,positions110)==0x114);
    assert(offsetof(CarTrackWork,wheelHeights250)==0x254);
    memcpy(&before,&world.vehicle,sizeof(before));
    memcpy(&oldContact,&world.contact,sizeof(oldContact));
    UpdateWheelBumps(&world.vehicle);
    assert(!memcmp((u8*)&world.vehicle+PREFIX,(u8*)&before+PREFIX,sizeof(before)-PREFIX));
    assert(!memcmp((u8*)&world.contact+0x264,(u8*)&oldContact+0x264,sizeof(oldContact)-0x264));
    for(i=0;i<4;i++){
        assert(world.beforeVehicle[i]==0xA5A5A5A5&&world.afterVehicle[i]==0xA5A5A5A5);
        assert(world.beforeContact[i]==0xA5A5A5A5&&world.afterContact[i]==0xA5A5A5A5);
    }
    snapshot(output);memcpy(output+STATE,&calls,4);
}
'''


class Cpu(Mips):
    def step(self):
        word = self.load(self.pc, 4)
        if word >> 26 == 0 and word & 63 == 0x1A:
            self.assert_division(word)
            return
        if word >> 26 == 1:
            assert self.delay is None
            rs, rt = (word >> 21) & 31, (word >> 16) & 31
            assert rt in (0, 1)
            take = signed(self.r[rs]) < 0 if rt == 0 else signed(self.r[rs]) >= 0
            self.delay = self.pc + 4 + (signed(word, 16) << 2) if take else self.pc + 8
            self.pc += 4
            return
        super().step()

    def assert_division(self, word):
        first, second = signed(self.r[(word >> 21) & 31]), signed(self.r[(word >> 16) & 31])
        assert second and (first, second) != (-0x80000000, -1)
        quotient = abs(first) // abs(second)
        if (first < 0) != (second < 0):
            quotient = -quotient
        self.lo, self.hi = quotient & 0xFFFFFFFF, (first - quotient * second) & 0xFFFFFFFF
        old, self.delay = self.delay, None
        self.pc = old if old is not None else self.pc + 4


class WheelBumpOwnerTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        owners = source_owners(ROOT/'src/main/PAL')
        body = definition_source(owners['UpdateWheelBumps'].read_text(), 'UpdateWheelBumps')
        body, pins = re.subn(r'\bregister int w78 asm\("\$2"\)', 'int w78', body)
        body, barriers = re.subn(r'__asm__\s*(?:volatile)?\s*\([^;]*\);', '', body)
        if (pins, barriers) != (1, 4):
            raise AssertionError(('review changed production constraints', pins, barriers))
        cls.temporary = TemporaryDirectory()
        cls.addClassCleanup(cls.temporary.cleanup)
        directory = Path(cls.temporary.name)
        source, library = directory/'native.c', directory/'native.dylib'
        source.write_text(NATIVE + body + TAIL)
        built = subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                                '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
                                '-I', str(ROOT/'include'), str(source), '-o', str(library)],
                               capture_output=True, text=True)
        if built.returncode:
            raise AssertionError(built.stderr)
        cls.lib = C.CDLL(str(library))
        cls.lib.run_wheels.argtypes = [C.c_uint, C.c_void_p, C.c_void_p]

    def test_four_wheels_both_height_banks_and_live_service_boundaries(self):
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0, 0x400000)
        memory.mem_map(0x1F800000, 0x1000)
        read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
        write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b))
        put16 = lambda a, v: write(a, struct.pack('<H', v & 0xFFFF))
        start = address_of('UpdateWheelBumps')
        services = {address_of(name): number for number, name in
                    enumerate(('TriggerWheelBump', 'SetCarImpactState', 'Atan2Table'), 1)}
        original = (ROOT/'assets/PAL/main.exe').read_bytes()
        linked = (ROOT/'build/PAL/main.exe').read_bytes()
        base = struct.unpack_from('<I', original, 0x18)[0]
        size = next(int(row.split()[2], 16) for row in (ROOT/'configs/PAL/functions.txt').read_text().splitlines()
                    if row.split()[0] == 'UpdateWheelBumps')
        stop, stack = 0x803FF000, 0x803F0000
        randomizer = random.Random(0xCA4B00)
        seen, trigger_outcomes, landings = set(), set(), set()

        def snapshot():
            return read(VEHICLE, PREFIX_SIZE) + read(CONTACT, CONTACT_SIZE) + read(CONTACT-4, 4)

        def store(cpu, access, address, width, value, user):
            address &= 0x1FFFFFFF
            self.assertTrue(any(first <= address and address + width <= last for first, last in
                                ((VEHICLE & 0x1FFFFFFF, (VEHICLE & 0x1FFFFFFF) + PREFIX_SIZE),
                                 (CONTACT, CONTACT + CONTACT_SIZE),
                                 ((stack & 0x1FFFFFFF)-0x100, stack & 0x1FFFFFFF))),
                            (case, hex(cpu.pc), hex(address), width))

        cpu = Cpu(memory, 0, 100, 101, 102, store)
        for case in range(768):
            vehicle = bytearray(randomizer.randbytes(PREFIX_SIZE))
            contact = bytearray(randomizer.randbytes(CONTACT_SIZE))
            struct.pack_into('<I', contact, 0, VEHICLE)
            # All live 32-bit arithmetic remains in range; the C function's
            # separate full-range wrapping debt is outside this owner change.
            for offset in (*range(0x10, 0x1C, 4), *range(0x40, 0x4C, 4),
                           *range(0x70, 0x80, 4), *range(0x88, 0xD8, 4)):
                struct.pack_into('<i', vehicle, offset, randomizer.randrange(-1000, 1001))
            struct.pack_into('<hh', vehicle, 0x10C, 120 + case % 31, 200 + case % 47)
            for offset in range(0x124, 0x16C, 2):
                struct.pack_into('<h', vehicle, offset, randomizer.randrange(-100, 101))
            struct.pack_into('<i', vehicle, 0x16C, (-1, 0, 1, 7, 15, 16, 255)[case % 7])
            for offset in range(0x1A8, 0x1B8, 2):
                struct.pack_into('<h', vehicle, offset, randomizer.randrange(-100, 101))
            for wheel in range(4):
                node = (case + wheel) % 8
                struct.pack_into('<h', vehicle, 0xF8 + 2*wheel, node)
                struct.pack_into('<h', vehicle, 0x100 + 2*wheel, node if (case >> wheel) & 1 else (node+1) % 8)
                state = (-2, 0, 1, 2, 3, 7, 12)[(case//2 + wheel) % 7]
                struct.pack_into('<h', vehicle, 0x1BC + 2*wheel, state)
                struct.pack_into('<h', vehicle, 0x1C4 + 2*wheel, (-80, -5, 0, 5, 80)[(case+wheel) % 5])
                struct.pack_into('<h', vehicle, 0x1CC + 2*wheel, randomizer.randrange(-200, 201))
                for bank in range(2):
                    struct.pack_into('<h', contact, 0x250 + bank*8 + wheel*2,
                                     randomizer.randrange(-200, 201))
                seen.add((wheel, state, case & 1))
            data = bytes(vehicle + contact + struct.pack('<i', case & 1))
            native_input = C.create_string_buffer(data)
            output = C.create_string_buffer(STATE_SIZE + 4 + 16*(12 + STATE_SIZE))
            self.lib.run_wheels(case, native_input, output)
            expected = output.raw
            count = struct.unpack_from('<I', expected, STATE_SIZE)[0]
            for wheel in range(4):
                initial = struct.unpack_from('<h', vehicle, 0x1BC+2*wheel)[0]
                final = struct.unpack_from('<h', expected, 0x1BC+2*wheel)[0]
                if initial > 0 and final == 0:
                    landings.add(wheel)
            for image in (original, linked):
                write(start, image[0x800+start-base:0x800+start-base+size])
                write(VEHICLE-16, b'\xA6'*16 + vehicle + b'\x6A'*16)
                write(CONTACT-16, b'\xA6'*12 + struct.pack('<i', case & 1) + contact + b'\x6A'*16)
                write(stack-0x110, b'\xA5'*0x120)
                cpu.reset();cpu.r = [0] + [0xBEEF0000+i for i in range(1, 32)]
                cpu.r[4], cpu.r[29], cpu.r[31], cpu.pc = VEHICLE, stack, stop, start
                events = 0
                for instruction in range(5000):
                    if cpu.pc == stop:
                        break
                    if cpu.pc not in services:
                        cpu.step();continue
                    kind = services[cpu.pc]
                    first, second = signed(cpu.r[4]), signed(cpu.r[5])
                    if kind == 1:
                        self.assertEqual(cpu.r[4], VEHICLE);wheel = second
                        event = (1, wheel, 0)
                    elif kind == 2:
                        self.assertEqual(cpu.r[5], VEHICLE);event = (2, first, 0)
                    else:
                        event = (3, first, second)
                    pos = STATE_SIZE + 4 + events*(12 + STATE_SIZE)
                    self.assertEqual(struct.pack('<3I', *(v & 0xFFFFFFFF for v in event)), expected[pos:pos+12], (case, events))
                    self.assertEqual(snapshot(), expected[pos+12:pos+12+STATE_SIZE], (case, events, 'service boundary'))
                    result = 0
                    if kind == 1:
                        result = (case >> (wheel+1)) & 1
                        trigger_outcomes.add((wheel, result))
                        if result:
                            put16(VEHICLE+0x1BC+2*wheel, 1)
                            put16(VEHICLE+0x1C4+2*wheel, -(5+wheel))
                    elif kind == 2:
                        put16(VEHICLE+0x19C, 1)
                    else:
                        result = ((first*13) ^ (second*7)) & 0x7FF
                    resume = cpu.r[31]
                    for register in (2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 24, 25):
                        cpu.r[register] = 0xDEAD0000 + register
                    cpu.r[2], cpu.hi, cpu.lo, cpu.pc = result, 0x12345678, 0x87654321, resume
                    events += 1
                self.assertEqual(cpu.pc, stop, case)
                self.assertEqual(events, count, case)
                self.assertEqual(snapshot(), expected[:STATE_SIZE], (case, 'final owners'))
                self.assertEqual(cpu.r[29], stack);self.assertEqual(cpu.r[31], stop)
                for register in (*range(16, 24), 28, 30):
                    self.assertEqual(cpu.r[register], 0xBEEF0000+register)
                self.assertEqual(read(VEHICLE-16, 16), b'\xA6'*16)
                self.assertEqual(read(VEHICLE+PREFIX_SIZE, 16), b'\x6A'*16)
                self.assertEqual(read(CONTACT-16, 12), b'\xA6'*12)
                self.assertEqual(read(CONTACT+CONTACT_SIZE, 16), b'\x6A'*16)
                self.assertEqual(read(stack-0x110, 16), b'\xA5'*16)
                self.assertEqual(read(stack, 16), b'\xA5'*16)
        self.assertEqual(seen, {(wheel, state, bank) for wheel in range(4)
                               for state in (-2, 0, 1, 2, 3, 7, 12) for bank in range(2)})
        self.assertEqual(trigger_outcomes, {(wheel, result) for wheel in range(4) for result in (0, 1)})
        self.assertEqual(landings, set(range(4)))


if __name__ == '__main__':
    unittest.main()
