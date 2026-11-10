"""Actual impact setter/shake controller, full owners and original/linked MIPS.

Angular, sine, haptic and audio services are instrumented boundaries. The two
production controllers and their combined flow run as real C without field,
address or arithmetic rewriting; only the existing MIPS register name is
removed for the host compiler. Old fixtures and their models are untouched.
"""
import ctypes as C
from pathlib import Path
import random
import re
import struct
import subprocess
from tempfile import TemporaryDirectory

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN

from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
CASES, STATE, MAX_EVENTS = 4096, 3476, 8
EVENT = STATE+20
OUTPUT = 4+MAX_EVENTS*EVENT+STATE
RECORDS = (0x80200010, 0x80201010)
SINES = (-4096, -3072, -1, 0, 1, 3072, 4096)
SPEEDS = (-32768, -1, 0, 319, 320, 321, 799, 800, 801, 32767)
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "vehicle_wire.h"
#include "game/car/carstate.h"
#include "game/course/trkintp.h"
#include "game/system/pad.h"
#include "game/sound/sndvab.h"
#include "rr/trig.h"
static struct {u8 before[16];RaceVehicle cars[2];u8 after[16];} players;
static struct {u8 before[16];RaceVehicle car;u8 after[16];} extra;
#define PlayerCars players.cars
int ReplayState;
static unsigned scenario,events;
static RaceVehicle *selected;
static u8 *trace;
static const s32 sines[]={-4096,-3072,-1,0,1,3072,4096};
static const s16 speeds[]={-32768,-1,0,319,320,321,799,800,801,32767};
enum {STATE=3476,EVENT=3496,MAX_EVENTS=8};
static void snapshot(u8 *out) {
    unsigned i;
    rr_vehicle_wire_store(out,&PlayerCars[0]);
    rr_vehicle_wire_store(out+800,&PlayerCars[1]);
    rr_vehicle_wire_store(out+1600,&extra.car);
    for(i=0;i<2;i++)memcpy(out+2400+i*0x218,&rr_test_records[i].record,0x218);
    memcpy(out+3472,&ReplayState,4);
}
static unsigned record(u32 kind,u32 a,u32 b,u32 c,u32 d) {
    u32 words[5]={kind,a,b,c,d};unsigned serial=events++;
    assert(serial<MAX_EVENTS);memcpy(trace+serial*EVENT,words,20);
    snapshot(trace+serial*EVENT+20);
    if((scenario&1)&&kind==1)selected->input=speeds[(scenario+serial)%10];
    if((scenario&2)&&serial==0)ReplayState=(scenario&16)?2:0;
    if((scenario&4)&&kind==2) {
        selected->contactTimer1A7=(scenario&16)?0:255;
        selected->input=speeds[(scenario+serial)%10];
    }
    if((scenario&8)&&kind==3) {
        static const u8 states[]={0,1,2,3,5,8,9,255};
        selected->state1A4=states[(scenario+serial)%8];
        selected->stateTimer1A5=(scenario&16)?29:0;
        selected->steeringLock1B8^=1;
    }
    if((scenario&4)&&kind==4) {
        selected->renderAngleX1AC=(s16)(scenario*977U);
        selected->renderAngleZ1AE=(s16)(scenario*293U);
    }
    return serial;
}
int AngleDiffAbs(int first,int second) {
    u32 distance=((u32)first-(u32)second)&4095U;
    record(1,(u32)first,(u32)second,0,0);
    return distance<=2048?distance:4096-distance;
}
void PadStartPulse(int channel,int kind,int strength) {
    record(2,(u32)channel,(u32)kind,(u32)strength,0);
}
s32 rsin(s32 angle) {
    unsigned serial=record(3,(u32)angle,0,0,0);
    return sines[(scenario+serial+((u32)angle&7))%7];
}
void PlaySfxVolPan(int sound,s16 volume,s16 pan,int voice) {
    record(4,(u32)sound,(u32)(s32)volume,(u32)(s32)pan,(u32)voice);
}
'''
TAIL = r'''
void run_case(unsigned number,unsigned operation,unsigned index,s32 state,
              const u8 *input,u8 *output) {
    unsigned i;scenario=number;events=0;trace=output+4;
    assert(operation<3&&index<3);rr_vehicle_wire_reset();
    memset(&players,0xA6,sizeof(players));memset(&extra,0xA6,sizeof(extra));
    rr_vehicle_wire_load(&PlayerCars[0],input);
    rr_vehicle_wire_load(&PlayerCars[1],input+800);
    rr_vehicle_wire_load(&extra.car,input+1600);
    assert(rr_test_record_count==2&&rr_test_handling_count==0);
    for(i=0;i<2;i++)memcpy(&rr_test_records[i].record,input+2400+i*0x218,0x218);
    memcpy(&ReplayState,input+3472,4);
    selected=index==2?&extra.car:&PlayerCars[index];
    assert(sizeof(void*)==8&&(uintptr_t)selected>UINT32_MAX);
    assert((uintptr_t)selected->control.record18>UINT32_MAX);
    assert(selected->control.handling1C==&selected->control.record18->handling1C8);
    assert(PlayerCars[0].control.record18==PlayerCars[1].control.record18);
    assert(PlayerCars[0].control.handling1C==PlayerCars[1].control.handling1C);
    memset(output,0,4+MAX_EVENTS*EVENT+STATE);
    if(operation!=1)SetCarImpactState(state,selected);
    if(operation!=0)UpdateCarShake(selected);
    memcpy(output,&events,4);snapshot(output+4+MAX_EVENTS*EVENT);
    for(i=0;i<16;i++) {
        assert(players.before[i]==0xA6&&players.after[i]==0xA6);
        assert(extra.before[i]==0xA6&&extra.after[i]==0xA6);
    }
    assert(rr_test_record_count==2&&rr_test_handling_count==0);
}
'''


def signed_half(value):
    value &= 65535
    return value if value < 32768 else value-65536


def truncated(first, second):
    quotient = abs(first)//abs(second)
    return -quotient if (first < 0) != (second < 0) else quotient


def main():
    owners = source_owners(ROOT/'src/main/PAL')
    names = ('SetCarImpactState', 'UpdateCarShake')
    bodies = '\n'.join(definition_source(owners[name].read_text(), name) for name in names)
    bodies = re.sub(r'\s+asm\("\$[0-9]+"\)', '', bodies)
    with TemporaryDirectory() as temp:
        source, library = Path(temp)/'native.c', Path(temp)/'native.dylib'
        source.write_text(PREFIX+bodies+TAIL)
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC',
                        '-Wall', '-Wextra', '-Werror', '-fsanitize=undefined,bounds',
                        '-fno-sanitize-recover=all', '-I'+str(ROOT/'include'),
                        '-I'+str(ROOT/'tools/scripts'), str(source), '-o', str(library)], check=True)
        native = C.CDLL(str(library));native.run_case.argtypes = [C.c_uint, C.c_uint, C.c_uint,
                                                                C.c_int32, C.c_void_p, C.c_void_p]
        executable = (ROOT/'assets/PAL/main.exe').read_bytes()
        base = struct.unpack_from('<I', executable, 24)[0]
        starts = tuple(address_of(name) for name in names)
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN);memory.mem_map(0, 0x400000)
        write = lambda address, data: memory.mem_write(address & 0x1FFFFFFF, bytes(data))
        read = lambda address, size: bytes(memory.mem_read(address & 0x1FFFFFFF, size))
        put = lambda address, value, size: write(address, (value & ((1 << (8*size))-1)).to_bytes(size, 'little'))
        write(base, executable[0x800:])
        cars = (address_of('PlayerCars'), address_of('PlayerCars')+800, 0x80180010)
        replay, stack, stop = address_of('ReplayState'), 0x803F0000, 0x803FF000
        services = {address_of(name): i+1 for i, name in enumerate(('AngleDiffAbs', 'PadStartPulse', 'rsin', 'PlaySfxVolPan'))}
        snapshot = lambda: b''.join(read(a, 800) for a in cars)+b''.join(read(a, 0x218) for a in RECORDS)+read(replay, 4)
        rng = random.Random(0x2C1D42C328)
        words = (-2147483648, -65537, -1, 0, 1, 2, 3, 5, 8, 9, 255, 256, 257, 258, 264, 65537, 2147483647)
        states = (0, 1, 2, 3, 5, 8, 9, 255)
        halves = (-32768, -32767, -301, -300, -100, -1, 0, 1, 99, 100, 299, 300, 32766, 32767)
        cases = []
        for case in range(CASES):
            data = bytearray(rng.randbytes(STATE))
            for index in range(3):
                at = index*800;record = RECORDS[index//2]
                struct.pack_into('<II', data, at+0x20C, record, record+0x1C8)
                data[at+0x1A4] = states[(case//3+index) % len(states)]
                data[at+0x1A5] = (case+index) % 256
                data[at+0x1A7] = (case//7+index) % 256
                struct.pack_into('<h', data, at+0x1A2, halves[(case+index) % len(halves)])
                struct.pack_into('<h', data, at+0x1BA, halves[(case//7+index) % len(halves)])
                struct.pack_into('<h', data, at+0x1D8, SPEEDS[(case//3+index) % len(SPEEDS)])
                struct.pack_into('<h', data, at+0x1B8, (-32768, -1, 0, 1, 32767)[(case+index) % 5])
                struct.pack_into('<h', data, at+0x1AC, halves[(case//9+index) % len(halves)])
                struct.pack_into('<h', data, at+0x1AE, halves[(case//11+index) % len(halves)])
            struct.pack_into('<i', data, 3472, (-2147483648, -1, 0, 1, 2, 2147483647)[case % 6])
            cases.append((data, case % 3, words[(case//3) % len(words)]))

        for version in ('original', 'linked'):
            for name, entry, size in zip(names, starts, (0x154, 0x310)):
                code = executable[0x800+entry-base:0x800+entry-base+size]
                if version == 'linked':
                    with (ROOT/'build/PAL/main.elf').open('rb') as stream:
                        elf = ELFFile(stream);symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(name)[0]
                        assert symbol['st_value'] == entry and symbol['st_size'] == size
                        section = elf.get_section(symbol['st_shndx']);offset = entry-section['sh_addr']
                        code = section.data()[offset:offset+size]
                        rodata = next(s for s in elf.iter_sections() if s['sh_type'] == 'SHT_PROGBITS' and
                                      s['sh_addr'] <= 0x80010468 and 0x80010488 <= s['sh_addr']+s['sh_size'])
                        offset = 0x80010468-rodata['sh_addr'];jump_table = rodata.data()[offset:offset+32]
                        assert jump_table == executable[0x800+0x80010468-base:0x800+0x80010488-base]
                        write(0x80010468, jump_table)
                write(entry, code)
            covered, expiration_cases, wrapped_contacts, oracle_cases = set(), 0, 0, 0
            for operation in range(3):
                for case, (data, index, state) in enumerate(cases):
                    output = (C.c_ubyte*OUTPUT)()
                    native.run_case(case, operation, index, state, C.create_string_buffer(bytes(data)), output)
                    expected = bytes(output);events, = struct.unpack_from('<I', expected)
                    for i, address in enumerate(cars):write(address, data[i*800:(i+1)*800])
                    for i, address in enumerate(RECORDS):write(address-16, b'\xA6'*16+data[2400+i*0x218:2400+(i+1)*0x218]+b'\x6A'*16)
                    write(replay, data[3472:]);write(stack-256, b'\xA6'*512)
                    selected = cars[index];event = 0;writes = []
                    if operation == 0 and case % 16 == 0:
                        oracle = bytearray(data);at = index*800;oracle[at+0x1A4] = state & 255
                        if state in (1, 2, 8):
                            oracle[at+0x1A5] = 30
                            if state == 1:angle = struct.unpack_from('<h', data, at+0x1BA)[0]*8
                            else:
                                heading = struct.unpack_from('<h', data, at+0x1EC)[0]
                                rotation = struct.unpack_from('<i', data, at+0x54)[0]
                                delta = (heading-rotation) & 4095;magnitude = min(delta, 4096-delta) if state == 2 else 1024
                                if magnitude > 1024:magnitude = 2048-magnitude
                                speed = struct.unpack_from('<h', data, at+0x1D8)[0]
                                angle = -truncated((speed-320)*magnitude, 4096) if speed >= 320 else 0
                            struct.pack_into('<h', oracle, at+0x1A2, signed_half(angle))
                        assert expected[4+MAX_EVENTS*EVENT:] == oracle, (version, case, 'independent setter oracle')
                        oracle_cases += 1
                    for subject in ((0,) if operation == 0 else (1,) if operation == 1 else (0, 1)):
                        def store(cpu, access, address, size, value, user):
                            if stack-0x28 <= address and address+size <= stack:return
                            offset = address-selected
                            allowed = {0x1A4:1, 0x1A5:1, 0x1A2:2} if subject == 0 else {
                                0x1A4:1, 0x1A5:1, 0x1A7:1, 0x1AC:2, 0x1AE:2, 0x1B0:2, 0x1B2:2, 0x1B4:2, 0x1B6:2}
                            assert allowed.get(offset) == size, (hex(address), size, subject)
                            writes.append((subject, offset, size, value & ((1 << (8*size))-1)))
                        cpu = Cpu(memory, 0, 100, 101, 102, store)
                        cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
                        cpu.r[4:6] = [state & 0xFFFFFFFF, selected] if subject == 0 else [selected, 0xBEEF0005]
                        cpu.r[29] = stack;cpu.r[31] = stop;cpu.pc = starts[subject]
                        for instruction in range(1500):
                            if cpu.pc == stop:break
                            if cpu.pc not in services:cpu.step();continue
                            assert cpu.delay is None and event < events
                            kind = services[cpu.pc];covered.add(kind)
                            args = [0]*4
                            for i in range((2, 3, 1, 4)[kind-1]):args[i] = cpu.r[4+i]
                            actual = struct.pack('<5I', kind, *args)+snapshot()
                            assert actual == expected[4+event*EVENT:4+(event+1)*EVENT], (version, case, operation, subject, event, kind)
                            if kind == 1:
                                distance = (args[0]-args[1]) & 4095;result = min(distance, 4096-distance)
                            elif kind == 3:result = SINES[(case+event+(args[0]&7)) % 7]
                            else:result = 0
                            if case & 1 and kind == 1:put(selected+0x1D8, SPEEDS[(case+event) % 10], 2)
                            if case & 2 and event == 0:put(replay, 2 if case & 16 else 0, 4)
                            if case & 4 and kind == 2:
                                put(selected+0x1A7, 0 if case & 16 else 255, 1)
                                put(selected+0x1D8, SPEEDS[(case+event) % 10], 2)
                            if case & 8 and kind == 3:
                                put(selected+0x1A4, states[(case+event) % 8], 1)
                                put(selected+0x1A5, 29 if case & 16 else 0, 1)
                                put(selected+0x1B8, int.from_bytes(read(selected+0x1B8, 2), 'little') ^ 1, 2)
                            if case & 4 and kind == 4:
                                put(selected+0x1AC, case*977, 2);put(selected+0x1AE, case*293, 2)
                            event += 1;resume = cpu.r[31]
                            for register in (2, 3, *range(4, 16), 24, 25):cpu.r[register] = 0xDEAD0000+register
                            cpu.r[2] = result & 0xFFFFFFFF;cpu.pc = resume;cpu.hi = 0x12345678;cpu.lo = 0x87654321
                        assert cpu.pc == stop
                        assert cpu.r[29] == stack and cpu.r[31] == stop
                        for register in (*range(16, 24), 28, 30):assert cpu.r[register] == 0xBEEF0000+register
                        assert read(stack-256, 256-0x28) == b'\xA6'*(256-0x28) and read(stack, 256) == b'\xA6'*256
                    assert event == events and snapshot() == expected[4+MAX_EVENTS*EVENT:], (version, case, operation, 'final')
                    if (1, 0x1A5, 1, 0) in writes:expiration_cases += 1
                    if (1, 0x1A7, 1, 255) in writes:wrapped_contacts += 1
                    for address in RECORDS:assert read(address-16, 16) == b'\xA6'*16 and read(address+0x218, 16) == b'\x6A'*16
            assert covered == {1, 2, 3, 4} and expiration_cases and wrapped_contacts and oracle_cases == 256
            print('PASS', version, CASES*3, 'actual impact/shake flows; setter oracle', oracle_cases,
                  'expiration', expiration_cases, 'contact wrap', wrapped_contacts, 'full owners/pointees and live service traces')


if __name__ == '__main__':
    main()
