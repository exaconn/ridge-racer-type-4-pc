"""Actual race-frame controller with native owners and original/linked MIPS.

Physics/input/trigonometry and the eleven stage algorithms are instrumented
boundaries. This tests dispatch, live owner/global reads and impact/contact
controller arithmetic, not the algorithms behind those service boundaries.
Inputs use real course indices and bounded physical denominators/products.
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
from mips_preview_engine import signed
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
CASES, STATE, MAX_EVENTS = 4096, 4580, 64
EVENT = 12 + STATE
OUTPUT = 4 + MAX_EVENTS * EVENT + STATE
SCRATCH, RECORDS = 0x1F800000, (0x80200010, 0x80201010)
STACK, STOP = 0x803F0000, 0x803FF000
SINES = (-4096, -3072, -1, 0, 1, 3072, 4096)
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "vehicle_wire.h"
#include "rr/vehicle_table.h"
#include "game/race/racemai2.h"
#include "game/race/racesave.h"
#include "game/car/carsetup.h"
#include "game/car/carai.h"
#include "game/car/carstate.h"
#include "game/ui/uipart1.h"
#include "game/course/trkintp.h"
#include "rr/steering.h"
#include "rr/trig.h"
static struct {u8 before[16];RaceVehicle cars[2];u8 after[16];} players;
static struct {u8 before[16];RaceVehicle car;u8 after[16];} extra;
static struct {u8 before[16];u32 words[256];u8 after[16];} scratch;
#define PlayerCars players.cars
#define SCRATCHPAD ((u8*)scratch.words)
RaceVehicle *CarTable[8];
short NumCars;
unsigned short CourseIndex;
u8 CourseStartSteeringBias[4];
static unsigned scenario,events;
static u8 *trace;
static const s32 sines[]={-4096,-3072,-1,0,1,3072,4096};
enum {STATE=4580,EVENT=4592,MAX_EVENTS=64};
static RaceVehicle *car(unsigned index) {
    assert(index<3);return index==2?&extra.car:&PlayerCars[index];
}
static u32 carAddress(const RaceVehicle *value) {
    unsigned i;for(i=0;i<3;i++)if(value==car(i))return carAddresses[i];
    assert(!"unknown vehicle pointer");return 0;
}
static void snapshot(u8 *out) {
    unsigned i;u32 word;
    for(i=0;i<3;i++)rr_vehicle_wire_store(out+i*800,car(i));
    memcpy(out+2400,scratch.words,1024);
    for(i=0;i<2;i++)memcpy(out+3424+i*0x218,&rr_test_records[i].record,0x218);
    for(i=0;i<8;i++){word=carAddress(CarTable[i]);memcpy(out+4496+i*4,&word,4);}
    for(i=0;i<11;i++) {
        unsigned j;for(j=0;j<11;j++)if(CarUpdateStages[i]==handlers[j])break;
        assert(j<11);word=stageAddresses[j];memcpy(out+4528+i*4,&word,4);
    }
    memcpy(out+4574,&CourseIndex,2);
    memcpy(out+4576,CourseStartSteeringBias,4);
    memcpy(out+4572,&NumCars,2);
}
static unsigned record(u32 kind,u32 a,u32 b) {
    u32 words[3]={kind,a,b};unsigned serial=events++;
    assert(serial<MAX_EVENTS);memcpy(trace+serial*EVENT,words,12);
    snapshot(trace+serial*EVENT+12);return serial;
}
static void changeTable(unsigned serial) {
    unsigned slot=(scenario>>4)%3;
    CarTable[slot]=car((scenario+serial+1)%3);
}
void ReadCarInput(RaceVehicle *vehicle,s32 player) {
    unsigned serial=record(1,carAddress(vehicle),(u32)player);
    if(scenario&1)CourseIndex=(u16)((CourseIndex+3)%(scenario<2048?6:8));
    if((scenario&2)&&serial==0)changeTable(serial);
}
void UpdatePlayerCar(RaceVehicle *vehicle,int player) {
    unsigned serial=record(2,carAddress(vehicle),(u32)player);
    if(scenario&4)vehicle->contactTimer1A7^=1;
    if((scenario&8)&&serial==1)NumCars=(s16)((scenario>>6)%4);
}
void UpdateComCars(void) {
    unsigned serial=record(3,0,0);
    if(scenario&8)NumCars=(s16)((scenario>>6)%4);
    if(scenario&2)changeTable(serial);
}
void SetCarImpactState(int state,RaceVehicle *vehicle) {
    record(4,(u32)state,carAddress(vehicle));vehicle->state1A4=(u8)state;
}
void StartCollisionShake(RaceVehicle *vehicle,s32 kind) {
    unsigned serial=record(5,carAddress(vehicle),(u32)kind);
    if(scenario&16)changeTable(serial);
}
int AngleDiffAbs(int first,int second) {
    unsigned serial=record(6,(u32)first,(u32)second);
    int distance=(first-second)&4095;
    if(scenario&32)changeTable(serial);
    return distance<=2048?distance:4096-distance;
}
s32 rcos(s32 angle) {
    unsigned serial=record(7,(u32)angle,0);
    if(scenario&64)changeTable(serial);
    return sines[(scenario+serial+((u32)angle&7))%7];
}
void UpdateComCarTrackProjections(void) {record(8,0,0);}
static void stage(unsigned index,RaceVehicle *vehicle) {
    unsigned serial=record(100+index,carAddress(vehicle),0);
    if(scenario&128)vehicle->contactTimer1A7^=1;
    if((scenario&256)&&serial<8)NumCars=(s16)((scenario>>6)%4);
    if(scenario&512)CarUpdateStages[(scenario+serial)%11]=handlers[(index+3)%11];
    if(scenario&1024)changeTable(serial);
}
'''
TAIL = r'''
void run_case(unsigned number,u32 mode,const u8 *input,u8 *output) {
    unsigned i;scenario=number;events=0;trace=output+4;
    rr_vehicle_wire_reset();memset(&players,0xA6,sizeof(players));
    memset(&extra,0xA6,sizeof(extra));memset(&scratch,0xA6,sizeof(scratch));
    for(i=0;i<3;i++)rr_vehicle_wire_load(car(i),input+i*800);
    assert(rr_test_record_count==2&&rr_test_handling_count==0);
    memcpy(scratch.words,input+2400,1024);
    for(i=0;i<2;i++)memcpy(&rr_test_records[i].record,input+3424+i*0x218,0x218);
    for(i=0;i<8;i++)CarTable[i]=car((number+i)%3);
    for(i=0;i<11;i++)CarUpdateStages[i]=handlers[i];
    NumCars=(s16)((number>>2)%4);CourseIndex=(u16)((number>>4)%(number<2048?6:8));
    memcpy(CourseStartSteeringBias,input+4576,4);
    assert(sizeof(void*)==8&&(uintptr_t)SCRATCHPAD>UINT32_MAX);
    for(i=0;i<3;i++) {
        assert((uintptr_t)car(i)>UINT32_MAX);
        assert((uintptr_t)car(i)->control.record18>UINT32_MAX);
        assert(car(i)->control.handling1C==&car(i)->control.record18->handling1C8);
    }
    assert(car(0)->control.record18==car(1)->control.record18);
    for(i=0;i<11;i++)assert((uintptr_t)handlers[i]>UINT32_MAX);
    memset(output,0,4+MAX_EVENTS*EVENT+STATE);StepRaceFrame(mode);
    memcpy(output,&events,4);snapshot(output+4+MAX_EVENTS*EVENT);
    for(i=0;i<16;i++) {
        assert(players.before[i]==0xA6&&players.after[i]==0xA6);
        assert(extra.before[i]==0xA6&&extra.after[i]==0xA6);
        assert(scratch.before[i]==0xA6&&scratch.after[i]==0xA6);
    }
    assert(rr_test_record_count==2&&rr_test_handling_count==0);
}
'''


class FrameCpu(Cpu):
    def step(self):
        word = self.load(self.pc, 4)
        if word >> 26 == 0 and (word & 63) in (0x1A, 0x1B):
            assert (word >> 6) & 1023 == 0
            first, second = self.r[(word >> 21) & 31], self.r[(word >> 16) & 31]
            if (word & 63) == 0x1A:first, second = signed(first), signed(second)
            assert second != 0 and not (first == -2147483648 and second == -1)
            quotient = abs(first)//abs(second)
            if (first < 0) != (second < 0):quotient = -quotient
            self.lo = quotient & 0xFFFFFFFF;self.hi = (first-quotient*second) & 0xFFFFFFFF
            target = self.delay;self.delay = None
            self.pc = target if target is not None else self.pc+4
            return
        super().step()


def main():
    owners = source_owners(ROOT/'src/main/PAL')
    body = definition_source(owners['StepRaceFrame'].read_text(), 'StepRaceFrame')
    # Production body and argument/field operations are compiled unchanged.
    executable = (ROOT/'assets/PAL/main.exe').read_bytes()
    base = struct.unpack_from('<I', executable, 24)[0]
    entry = address_of('StepRaceFrame');table = address_of('CarUpdateStages')
    stage_addresses = struct.unpack_from('<11I', executable, 0x800+table-base)
    cars = (address_of('PlayerCars'), address_of('PlayerCars')+800, 0x80180010)
    stage_functions = '\n'.join('static void stage%d(RaceVehicle *vehicle){stage(%d,vehicle);}' % (i, i) for i in range(11))
    declarations = '\n'.join('static void stage%d(RaceVehicle *);' % i for i in range(11))
    declarations += '\nstatic void stage(unsigned,RaceVehicle *);\n'
    declarations += 'static const u32 carAddresses[3]={'+','.join(hex(a) for a in cars)+'};\n'
    declarations += 'static const u32 stageAddresses[11]={'+','.join(hex(a) for a in stage_addresses)+'};\n'
    declarations += 'static CarUpdateStage const handlers[11]={'+','.join('stage%d' % i for i in range(11))+'};\n'
    declarations += 'CarUpdateStage CarUpdateStages[11];\n'
    prefix = PREFIX.replace('static unsigned scenario,events;', declarations+'static unsigned scenario,events;')
    with TemporaryDirectory() as temp:
        source, library = Path(temp)/'native.c', Path(temp)/'native.dylib'
        source.write_text(prefix+stage_functions+body+TAIL)
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC',
                        '-Wall', '-Wextra', '-Werror', '-fsanitize=undefined,bounds',
                        '-fno-sanitize-recover=all', '-I'+str(ROOT/'include'),
                        '-I'+str(ROOT/'tools/scripts'), str(source), '-o', str(library)], check=True)
        native = C.CDLL(str(library));native.run_case.argtypes = [C.c_uint, C.c_uint32, C.c_void_p, C.c_void_p]
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0, 0x400000);memory.mem_map(SCRATCH, 0x1000)
        write = lambda address, data: memory.mem_write(address & 0x1FFFFFFF, bytes(data))
        read = lambda address, size: bytes(memory.mem_read(address & 0x1FFFFFFF, size))
        put = lambda address, value, size=4: write(address, (value & ((1 << (8*size))-1)).to_bytes(size, 'little'))
        write(base, executable[0x800:])  # Includes the real race-mode jump table.
        count, course, friction, car_table = map(address_of, ('NumCars', 'CourseIndex', 'CourseStartSteeringBias', 'CarTable'))
        def snapshot():
            return (b''.join(read(a, 800) for a in cars)+read(SCRATCH, 1024)+
                    b''.join(read(a, 0x218) for a in RECORDS)+read(car_table, 32)+
                    read(table, 44)+read(count, 2)+read(course, 2)+read(friction, 4))
        services = {address_of(name): i+1 for i, name in enumerate(('ReadCarInput', 'UpdatePlayerCar', 'UpdateComCars',
                    'SetCarImpactState', 'StartCollisionShake', 'AngleDiffAbs', 'rcos', 'UpdateComCarTrackProjections'))}
        services.update({address: 100+i for i, address in enumerate(stage_addresses)})
        rng = random.Random(0x37D889F7A0)
        speeds = (-32768, -1, 0, 160, 161, 760, 761, 800, 801, 1000, 2500, 32767)
        impacts = (0, 1, 2, 7, 8, 0x8000, 0xFFFF)
        cases = []
        for case in range(CASES):
            # Preserve every original2048 case byte-for-byte; the extra table
            # byte consumes no RNG output. Further cases cover all eight courses.
            data = bytearray(rng.randbytes(4579))+bytearray(1)
            for index in range(3):
                at = index*800;record = RECORDS[index//2]
                struct.pack_into('<II', data, at+0x20C, record, record+0x1C8)
                struct.pack_into('<h', data, at+0x1E8, -1 if (case+index) % 5 == 0 else index)
                struct.pack_into('<h', data, at+0x1D8, speeds[(case//3+index) % len(speeds)])
                struct.pack_into('<H', data, at+0x1A0, impacts[(case//7+index) % len(impacts)])
                struct.pack_into('<H', data, at+0x1E4, 0x10 if (case//11+index) & 1 else 0)
                struct.pack_into('<h', data, at+0x1B8, 0 if (case+index) & 1 else 1)
                data[at+0x1A7] = (0, 1, 10, 255)[(case+index) % 4]
                for wheel in range(4):data[at+0x1F0+wheel] = (case+index+wheel) % 4
                struct.pack_into('<i', data, at+0x28C, (4096, 8192, 32767)[index])
                struct.pack_into('<i', data, at+0x294, rng.randrange(-10000, 10001))
                struct.pack_into('<i', data, at+0x24C, rng.randrange(-100000, 100001))
                struct.pack_into('<i', data, at+0x2EC, (4096+index*4096)*256)
            data[4576:4579] = bytes((case % 256, (case*13) % 256, (case*79) % 256))
            data[4579] = (case*101) % 256
            mode = (0, 1, 2, 3, 4, 5, 6, 7, 0xFFFFFFFF)[case % 9]
            cases.append((data, mode))

        for version in ('original', 'linked'):
            size = 0x680;code = executable[0x800+entry-base:0x800+entry-base+size]
            if version == 'linked':
                with (ROOT/'build/PAL/main.elf').open('rb') as stream:
                    elf = ELFFile(stream);symbol = elf.get_section_by_name('.symtab').get_symbol_by_name('StepRaceFrame')[0]
                    assert symbol['st_value'] == entry and symbol['st_size'] == size
                    section = elf.get_section(symbol['st_shndx']);offset = entry-section['sh_addr']
                    code = section.data()[offset:offset+size]
                    rodata = next(s for s in elf.iter_sections()
                                  if s['sh_type'] == 'SHT_PROGBITS' and
                                  s['sh_addr'] <= 0x80010740 and
                                  0x80010740+0x1C <= s['sh_addr']+s['sh_size'])
                    offset = 0x80010740-rodata['sh_addr']
                    jump_table = rodata.data()[offset:offset+0x1C]
                    assert jump_table == executable[0x800+0x80010740-base:0x800+0x80010740-base+0x1C]
                    write(0x80010740, jump_table)
            write(entry, code);covered = set();cosine_sites = set();cosine_calls = negative_impacts = 0
            for case, (data, mode) in enumerate(cases):
                output = (C.c_ubyte*OUTPUT)();native.run_case(case, mode, C.create_string_buffer(bytes(data)), output)
                expected = bytes(output);events, = struct.unpack_from('<I', expected)
                for index, address in enumerate(cars):write(address, data[index*800:(index+1)*800])
                write(SCRATCH, data[2400:3424])
                for index, address in enumerate(RECORDS):write(address-16, b'\xA6'*16+data[3424+index*0x218:3424+(index+1)*0x218]+b'\x6A'*16)
                write(car_table, struct.pack('<8I', *(cars[(case+i) % 3] for i in range(8))))
                write(table, struct.pack('<11I', *stage_addresses));put(count, (case>>2) % 4, 2);put(course, (case>>4) % (6 if case<2048 else 8), 2)
                write(friction, data[4576:4580]);write(STACK-256, b'\xA6'*512)
                def store(cpu, access, address, size, value, user):
                    if STACK-0x48 <= address and address+size <= STACK:return
                    if address == SCRATCH+0x7C:assert size == 4;return
                    for car_address in cars:
                        if address-car_address in (0x224, 0x1D8, 0x1A7, 0x24C, 0x294):
                            assert size == (4 if address-car_address in (0x24C, 0x294) else 1 if address-car_address == 0x1A7 else 2)
                            return
                    raise AssertionError(('controller store outside real fields', hex(address), size))
                cpu = FrameCpu(memory, 0, 100, 101, 102, store)
                cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
                cpu.r[4] = mode;cpu.r[29] = STACK;cpu.r[31] = STOP;cpu.pc = entry;event = 0
                for instruction in range(10000):
                    if cpu.pc == STOP:break
                    if cpu.pc not in services:cpu.step();continue
                    assert cpu.delay is None and event < events
                    kind = services[cpu.pc];covered.add(kind)
                    args = cpu.r[4:6] if kind in (1, 2, 4, 5, 6) else [cpu.r[4], 0] if kind == 7 or kind >= 100 else [0, 0]
                    actual = struct.pack('<3I', kind, *args)+snapshot()
                    assert actual == expected[4+event*EVENT:4+(event+1)*EVENT], (version, case, mode, event, kind)
                    result = 0
                    def change_table():put(car_table+((case>>4) % 3)*4, cars[(case+event+1) % 3])
                    if kind == 1:
                        if case & 1:put(course, (int.from_bytes(read(course, 2), 'little')+3) % (6 if case<2048 else 8), 2)
                        if case & 2 and event == 0:change_table()
                    elif kind == 2:
                        if case & 4:put(args[0]+0x1A7, int.from_bytes(read(args[0]+0x1A7, 1), 'little') ^ 1, 1)
                        if case & 8 and event == 1:put(count, (case>>6) % 4, 2)
                    elif kind == 3:
                        if case & 8:put(count, (case>>6) % 4, 2)
                        if case & 2:change_table()
                    elif kind == 4:put(args[1]+0x1A4, args[0], 1)
                    elif kind == 5:
                        if signed(args[1]) < 0:negative_impacts += 1
                        if case & 16:change_table()
                    elif kind == 6:
                        distance = (signed(args[0])-signed(args[1])) & 4095
                        result = min(distance, 4096-distance)
                        if case & 32:change_table()
                    elif kind == 7:
                        cosine_sites.add(cpu.r[31]);cosine_calls += 1;result = SINES[(case+event+(args[0]&7)) % 7]
                        if case & 64:change_table()
                    elif kind >= 100:
                        if case & 128:put(args[0]+0x1A7, int.from_bytes(read(args[0]+0x1A7, 1), 'little') ^ 1, 1)
                        if case & 256 and event < 8:put(count, (case>>6) % 4, 2)
                        if case & 512:put(table+((case+event) % 11)*4, stage_addresses[(kind-100+3) % 11])
                        if case & 1024:change_table()
                    event += 1;resume = cpu.r[31]
                    for register in (2, 3, *range(4, 16), 24, 25):cpu.r[register] = 0xDEAD0000+register
                    cpu.r[2] = result & 0xFFFFFFFF;cpu.pc = resume;cpu.hi = 0x12345678;cpu.lo = 0x87654321
                assert cpu.pc == STOP and event == events, (version, case, event, events)
                assert snapshot() == expected[4+MAX_EVENTS*EVENT:], (version, case, 'final')
                assert cpu.r[29] == STACK and cpu.r[31] == STOP
                for register in (*range(16, 24), 28, 30):assert cpu.r[register] == 0xBEEF0000+register
                assert read(STACK-256, 256-0x48) == b'\xA6'*(256-0x48) and read(STACK, 256) == b'\xA6'*256
                for address in RECORDS:assert read(address-16, 16) == b'\xA6'*16 and read(address+0x218, 16) == b'\x6A'*16
            assert covered == {*range(1, 9), *range(100, 111)}, covered
            assert cosine_calls and negative_impacts and len(cosine_sites) == 2, cosine_sites
            print('PASS', version, CASES, 'real race-frame flows; cosine calls', cosine_calls,
                  'signed impacts', negative_impacts, 'all eleven stage boundaries and full native owners')


if __name__ == '__main__':
    main()
