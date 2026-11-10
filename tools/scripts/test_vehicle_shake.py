"""Actual shake setters/controllers: complete native owners and PAL call traces.

Trigonometry and audio are instrumented service boundaries. Their algorithms
are outside this fixture; the actual vehicle functions run unchanged.
"""
import ctypes as C
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN

from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
STATE = 2404
EVENT = STATE + 20
MAX_EVENTS = 8
OUTPUT = 4 + MAX_EVENTS * EVENT + STATE
CASES = 4096
SINES = (-4096, -3072, -2048, -1, 0, 1, 2048, 3072, 4096)
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "vehicle_wire.h"
#include "game/ui/uipart1.h"
#include "rr/trig.h"
#include "game/course/trkintp.h"
#include "game/sound/sndvab.h"
static struct {u8 before[16];RaceVehicle cars[2];u8 after[16];} players;
static struct {u8 before[16];RaceVehicle car;u8 after[16];} extra;
#define PlayerCars players.cars
int ReplayState;
static unsigned scenario,events;
static RaceVehicle *selected;
static u8 *trace;
static const s32 sines[]={-4096,-3072,-2048,-1,0,1,2048,3072,4096};
enum {STATE=2404,EVENT=2424,MAX_EVENTS=8};
static void snapshot(u8 *out) {
    rr_vehicle_wire_store(out,&PlayerCars[0]);
    rr_vehicle_wire_store(out+800,&PlayerCars[1]);
    rr_vehicle_wire_store(out+1600,&extra.car);
    memcpy(out+2400,&ReplayState,4);
}
static unsigned record(u32 kind,u32 a,u32 b,u32 c,u32 d) {
    unsigned serial=events;u32 words[5]={kind,a,b,c,d};assert(events<MAX_EVENTS);
    memcpy(trace+events*EVENT,words,20);snapshot(trace+events*EVENT+20);events++;
    if((scenario&1)&&serial==0) {
        selected->flags1E4^=0x10;selected->lateralVelF4=-(s32)(scenario|1);
    }
    if((scenario&2)&&serial==1)ReplayState=(scenario&8)?2:0;
    if((scenario&4)&&kind==3) {
        selected->input=(s16)(scenario*293U);
        selected->heading1D4=(s16)(scenario*977U);
    }
    if(kind==4)selected->shakeTimer19E^=7;
    return serial;
}
s32 rsin(s32 angle) {
    unsigned serial=record(1,(u32)angle,0,0,0);
    return sines[(scenario+serial+((u32)angle&7))%9];
}
s32 rcos(s32 angle) {
    unsigned serial=record(2,(u32)angle,0,0,0);
    return sines[(scenario+serial+((u32)angle&7)+3)%9];
}
int AngleDiffAbs(int first,int second) {
    s32 distance=(first-second)&4095;record(3,(u32)first,(u32)second,0,0);
    return distance<=2048?distance:4096-distance;
}
void PlaySfxVolPan(int sound,s16 volume,s16 pan,int voice) {
    record(4,(u32)sound,(u32)volume,(u32)pan,(u32)voice);
}
'''
TAIL = r'''
void run_case(unsigned number,unsigned operation,unsigned index,s32 first,s32 second,
              const u8 *input,u8 *output) {
    unsigned i;scenario=number;events=0;trace=output+4;assert(index<3);
    memset(output,0,4+MAX_EVENTS*EVENT+STATE);rr_vehicle_wire_reset();
    memset(&players,0xA6,sizeof(players));memset(&extra,0xA6,sizeof(extra));
    rr_vehicle_wire_load(&PlayerCars[0],input);
    rr_vehicle_wire_load(&PlayerCars[1],input+800);
    rr_vehicle_wire_load(&extra.car,input+1600);memcpy(&ReplayState,input+2400,4);
    selected=index==2?&extra.car:&PlayerCars[index];
    assert((uintptr_t)selected>UINT32_MAX);
    if(operation)StartCollisionShake(selected,first);
    else SetVehicleShake(selected,first,second);
    memcpy(output,&events,4);snapshot(output+4+MAX_EVENTS*EVENT);
    for(i=0;i<16;i++) {
        assert(players.before[i]==0xA6&&players.after[i]==0xA6);
        assert(extra.before[i]==0xA6&&extra.after[i]==0xA6);
    }
    assert(rr_test_record_count==0&&rr_test_handling_count==0);
}
'''


def main():
    owners = source_owners(ROOT/'src/main/PAL')
    bodies = '\n'.join(definition_source(owners[n].read_text(), n)
                       for n in ('StartCollisionShake', 'SetVehicleShake'))
    with TemporaryDirectory() as temp:
        directory = Path(temp)
        source, library = directory/'native.c', directory/'native.dylib'
        source.write_text(PREFIX+bodies+TAIL)
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC',
                        '-Wall', '-Wextra', '-Werror', '-fsanitize=undefined,bounds',
                        '-fno-sanitize-recover=all', '-I'+str(ROOT/'include'),
                        '-I'+str(ROOT/'tools/scripts'), str(source), '-o', str(library)], check=True)
        native = C.CDLL(str(library))
        native.run_case.argtypes = [C.c_uint, C.c_uint, C.c_uint, C.c_int32,
                                   C.c_int32, C.c_void_p, C.c_void_p]
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0, 0x400000)
        write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b))
        read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
        put32 = lambda a, v: write(a, struct.pack('<I', v & 0xFFFFFFFF))
        roots = (address_of('PlayerCars'), address_of('PlayerCars')+800, 0x80180010)
        replay, stack, stop = address_of('ReplayState'), 0x803F0000, 0x803FF000
        functions = ('SetVehicleShake', 'StartCollisionShake')
        starts = tuple(address_of(n) for n in functions)
        services = {address_of(n): i+1 for i, n in enumerate(('rsin', 'rcos', 'AngleDiffAbs', 'PlaySfxVolPan'))}
        executable = (ROOT/'assets/PAL/main.exe').read_bytes()
        base = struct.unpack_from('<I', executable, 24)[0]
        rng = random.Random(0x2BF342C11C)
        inputs = []
        words = (-2147483648, -65537, -32768, -1, 0, 1, 32767, 65535, 65536, 2147483647)
        impacts = (-2147483648, -1, 0, 1, 2, 3, 4, 32767, 2147483647)
        speeds = (-32768, -1, 0, 239, 240, 241, 799, 800, 801, 1799, 1800, 1801, 32767)
        for case in range(CASES):
            data = bytearray(rng.randbytes(STATE))
            for index in range(3):
                struct.pack_into('<II', data, index*800+0x20C, 0, 0)
                struct.pack_into('<h', data, index*800+0x1D8, speeds[(case+index) % len(speeds)])
                struct.pack_into('<H', data, index*800+0x1E4, 0x10 if (case//13+index) & 1 else 0)
            struct.pack_into('<i', data, 2400, (-2147483648, -1, 0, 1, 2, 2147483647)[(case//26) % 6])
            first = words[case % len(words)] if case < 2048 else rng.randrange(-2147483648, 2147483648)
            second = words[(case//len(words)) % len(words)] if case < 2048 else rng.randrange(-2147483648, 2147483648)
            inputs.append((data, case % 3, first, second))

        def snapshot():
            return b''.join(read(a, 800) for a in roots)+read(replay, 4)

        for version in ('original', 'linked'):
            for name, start, size in zip(functions, starts, (0x1C, 0x1E8)):
                code = executable[0x800+start-base:0x800+start-base+size]
                if version == 'linked':
                    with (ROOT/'build/PAL/main.elf').open('rb') as stream:
                        elf = ELFFile(stream)
                        symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(name)[0]
                        section = elf.get_section(symbol['st_shndx']);offset = symbol['st_value']-section['sh_addr']
                        assert symbol['st_value'] == start and symbol['st_size'] == size
                        code = section.data()[offset:offset+size]
                write(start, code)
            counts = [0]*4
            for operation in (0, 1):
                for case, (data, index, first, second) in enumerate(inputs):
                    argument = impacts[(case//3) % len(impacts)] if operation else first
                    output = (C.c_ubyte*OUTPUT)()
                    native.run_case(case, operation, index, argument, second, C.create_string_buffer(bytes(data)), output)
                    expected = bytes(output);events, = struct.unpack_from('<I', expected)
                    for i, address in enumerate(roots):write(address, data[i*800:(i+1)*800])
                    write(replay, data[2400:]);write(stack-0x100, b'\xA6'*0x200)
                    address = roots[index];writes = []
                    def store(cpu, access, at, size, value, user):
                        if stack-0x28 <= at and at+size <= stack:
                            assert operation == 1;return
                        assert size == 2 and at-address in (0x198, 0x19A, 0x19C, 0x19E), hex(at)
                        writes.append(at-address)
                    cpu = Cpu(memory, 0, 100, 101, 102, store)
                    cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
                    cpu.r[4:7] = [address, argument & 0xFFFFFFFF, second & 0xFFFFFFFF]
                    cpu.r[29] = stack;cpu.r[31] = stop;cpu.pc = starts[operation]
                    event = 0
                    for instruction in range(1000):
                        if cpu.pc == stop:break
                        if cpu.pc not in services:cpu.step();continue
                        assert operation == 1 and cpu.delay is None and event < events
                        kind = services[cpu.pc];counts[kind-1] += 1
                        args = [0]*4
                        for i in range(4 if kind == 4 else 2 if kind == 3 else 1):args[i] = cpu.r[4+i]
                        entry = struct.pack('<5I', kind, *args)+snapshot()
                        assert entry == expected[4+event*EVENT:4+(event+1)*EVENT], (version, case, kind, event)
                        if kind < 3:result = SINES[(case+event+(args[0]&7)+(3 if kind == 2 else 0)) % 9]
                        elif kind == 3:
                            distance = (args[0]-args[1]) & 4095
                            result = min(distance, 4096-distance)
                        else:result = 0
                        if case & 1 and event == 0:
                            value = int.from_bytes(read(address+0x1E4, 2), 'little') ^ 0x10
                            write(address+0x1E4, struct.pack('<H', value));put32(address+0xF4, -(case | 1))
                        if case & 2 and event == 1:put32(replay, 2 if case & 8 else 0)
                        if case & 4 and kind == 3:
                            write(address+0x1D8, struct.pack('<H', (case*293) & 65535))
                            write(address+0x1D4, struct.pack('<H', (case*977) & 65535))
                        if kind == 4:
                            value = int.from_bytes(read(address+0x19E, 2), 'little') ^ 7
                            write(address+0x19E, struct.pack('<H', value))
                        event += 1;resume = cpu.r[31]
                        for register in (2, 3, *range(4, 16), 24, 25):cpu.r[register] = 0xDEAD0000+register
                        cpu.r[2] = result & 0xFFFFFFFF;cpu.hi = 0x12345678;cpu.lo = 0x87654321;cpu.pc = resume
                    assert cpu.pc == stop and event == events
                    assert snapshot() == expected[4+MAX_EVENTS*EVENT:], (version, case, operation, 'final')
                    if operation == 0:
                        wanted = bytearray(data)
                        struct.pack_into('<4H', wanted, index*800+0x198, first & 65535, second & 65535, 2, 15)
                        assert snapshot() == wanted and writes == [0x19C, 0x198, 0x19A, 0x19E]
                    assert cpu.r[29] == stack and cpu.r[31] == stop
                    for register in (*range(16, 24), 28, 30):assert cpu.r[register] == 0xBEEF0000+register
                    guard = 0xD8 if operation else 0x100
                    assert read(stack-0x100, guard) == b'\xA6'*guard and read(stack, 0x100) == b'\xA6'*0x100
            assert all(counts), counts
            print('PASS', version, CASES*2, 'whole-owner shake flows; service calls', counts)


if __name__ == '__main__':
    main()
