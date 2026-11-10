"""Real player-update caller and reverse-heading helper, native versus PAL MIPS."""
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
STATE_SIZE = 3 * 800 + 20 + 4 * 60
EVENT_SIZE = STATE_SIZE + 12
CASES = 2048
STATES = (-2147483648, -1, 0, 1, 2, 3, 4, 2147483647)
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "vehicle_wire.h"
#include "game/ui/hudmap.h"
#include "game/ui/uipart1.h"
#include "game/car/carsetup.h"
#include "game/car/aipath.h"
#include "game/car/carparam.h"
#include "game/car/carsteer.h"
#include "game/car/aipath2.h"
#include "game/race/racemai2.h"
#include "game/course/trktext.h"
#include "game/sound/sndbgm3.h"
#include "rr/track_sample_types.h"
RaceVehicle PlayerCars[2];
static struct {u8 before[16]; RaceVehicle vehicle; u8 after[16];} extra;
static TrackSample3C nodes[4];
TrackSample3C *TrackNodes=nodes;
int RaceState, TrackEventSfxEnabled, ReplayState, ChaseViewEnabled;
short LinkHostFlag;
static RaceVehicle *selected;
static unsigned scenario, events;
static u8 *trace;
static const s32 states[]={INT32_MIN,-1,0,1,2,3,4,INT32_MAX};
static void snapshot(u8 *out) {
    s32 globals[5]={RaceState,TrackEventSfxEnabled,ReplayState,ChaseViewEnabled,LinkHostFlag};
    rr_vehicle_wire_store(out,&PlayerCars[0]);
    rr_vehicle_wire_store(out+800,&PlayerCars[1]);
    rr_vehicle_wire_store(out+1600,&extra.vehicle);
    memcpy(out+2400,globals,20);memcpy(out+2420,nodes,sizeof(nodes));
}
static void event(u32 kind,u32 a,u32 b) {
    u32 words[3]={kind,a,b};assert(events<8);
    memcpy(trace,words,12);snapshot(trace+12);trace+=2672;events++;
}
static void change(RaceVehicle *vehicle,u32 kind) {
    assert(vehicle==selected);
    vehicle->first=(s32)((u32)vehicle->first^(scenario*0x9E3779B9U+kind));
}
void UpdateCarGear(RaceVehicle *vehicle,int player) {
    assert(vehicle==selected);event(1,(u32)player,0);change(vehicle,1);
    if(scenario&1)RaceState=states[(scenario>>3)&7];
}
void UpdatePlayerSteering(RaceVehicle *vehicle,int player) {
    assert(vehicle==selected);event(2,(u32)player,0);change(vehicle,2);
    if(scenario&2)TrackEventSfxEnabled=(scenario>>5)&3;
}
void UpdateCarEngine(RaceVehicle *vehicle) {
    assert(vehicle==selected);event(3,0,0);change(vehicle,3);
    if(scenario&4)ReplayState=(scenario>>7)&3;
}
void PlayTrackEventSfx(RaceVehicle *vehicle) {
    assert(vehicle==selected);event(4,0,0);change(vehicle,4);
    if(scenario&8)ReplayState=2;
}
void UpdateRumbleStrip(RaceVehicle *vehicle,int player) {
    assert(vehicle==selected);event(5,(u32)player,0);change(vehicle,5);
}
void SetLoopVoiceVolumes(int player,int left,int right) {
    assert(left==0&&right==0);event(6,(u32)player,0);
}
int GetMappedPadButtons(int player) {
    event(7,(u32)player,0);
    if(scenario&16)RaceState=states[(scenario>>8)&7];
    return scenario%3==0?0:scenario%3==1?0x10000:-1;
}
/* Capture the ignored incoming word at this service boundary. The real
 * ResetCarSlotBuffers has no parameters; its caller's extra word is old debt. */
void ResetCarSlotBuffers(int incomingWord) {event(8,(u32)incomingWord,0);}
void UpdateCarSteering(RaceVehicle *v) {assert(v==selected);event(9,0,0);change(v,9);}
void IntegrateCarMotion(RaceVehicle *v) {assert(v==selected);event(10,0,0);change(v,10);}
void UpdateCarTrackNode(RaceVehicle *v) {assert(v==selected);event(11,0,0);change(v,11);}
void ComputeWheelOffsets(RaceVehicle *v) {assert(v==selected);event(12,0,0);change(v,12);}
void ConstrainCarToTrack(RaceVehicle *v) {assert(v==selected);event(13,0,0);change(v,13);}
'''
TAIL = r'''
unsigned run_case(unsigned number,unsigned operation,unsigned index,int player,const u8 *input,u8 *output) {
    s32 globals[5];unsigned i;scenario=number;events=0;trace=output;
    assert(index<3);rr_vehicle_wire_reset();
    memset(&extra,0xA6,sizeof(extra));
    rr_vehicle_wire_load(&PlayerCars[0],input);
    rr_vehicle_wire_load(&PlayerCars[1],input+800);
    rr_vehicle_wire_load(&extra.vehicle,input+1600);
    memcpy(globals,input+2400,20);memcpy(nodes,input+2420,sizeof(nodes));
    RaceState=globals[0];TrackEventSfxEnabled=globals[1];ReplayState=globals[2];
    ChaseViewEnabled=globals[3];LinkHostFlag=(s16)globals[4];
    selected=index==2?&extra.vehicle:&PlayerCars[index];
    assert((uintptr_t)selected>UINT32_MAX&&(uintptr_t)TrackNodes>UINT32_MAX);
    if(operation)StepCarPhysics(selected);else UpdatePlayerCar(selected,player);
    snapshot(trace);
    for(i=0;i<16;i++)assert(extra.before[i]==0xA6&&extra.after[i]==0xA6);
    assert(rr_test_handling_count==0&&rr_test_record_count==0);
    return events;
}
int run_heading(s16 heading,s16 trackHeading) {
    RaceVehicle before;TrackSample3C beforeNode;
    memset(&PlayerCars[0],0xA5,sizeof(PlayerCars[0]));memset(nodes,0xA6,sizeof(nodes));
    PlayerCars[0].trackNodeF0=0;PlayerCars[0].heading1D4=heading;
    nodes[0].road.heading=trackHeading;before=PlayerCars[0];beforeNode=nodes[0];
    {int result=IsCarFacingReverse(&PlayerCars[0]);
     assert(!memcmp(&before,&PlayerCars[0],sizeof(before)));
     assert(!memcmp(&beforeNode,&nodes[0],sizeof(beforeNode)));return result;}
}
void run_reset(const u8 *input,u8 *output) {
    unsigned i;rr_vehicle_wire_reset();memset(&extra,0xA6,sizeof(extra));
    rr_vehicle_wire_load(&extra.vehicle,input);
    assert((uintptr_t)&extra.vehicle>UINT32_MAX);
    ResetVehicleShake(&extra.vehicle);
    rr_vehicle_wire_store(output,&extra.vehicle);
    for(i=0;i<16;i++)assert(extra.before[i]==0xA6&&extra.after[i]==0xA6);
    assert(rr_test_handling_count==0&&rr_test_record_count==0);
}
'''


def main():
    owners = source_owners(ROOT/'src/main/PAL')
    bodies = '\n'.join(definition_source(owners[name].read_text(), name)
                       for name in ('IsCarFacingReverse', 'UpdatePlayerCar',
                                    'StepCarPhysics', 'ResetVehicleShake'))
    with TemporaryDirectory() as temp:
        directory = Path(temp)
        source, library = directory/'native.c', directory/'native.dylib'
        source.write_text(PREFIX + bodies + TAIL)
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
                        '-I'+str(ROOT/'include'), '-I'+str(ROOT/'tools/scripts'),
                        str(source), '-o', str(library)], check=True)
        native = C.CDLL(str(library))
        native.run_case.argtypes = [C.c_uint, C.c_uint, C.c_uint, C.c_int32, C.c_void_p, C.c_void_p]
        native.run_case.restype = C.c_uint
        native.run_heading.argtypes = [C.c_int16, C.c_int16]
        native.run_heading.restype = C.c_int
        native.run_reset.argtypes = [C.c_void_p, C.c_void_p]
        # Every signed vehicle heading, plus every signed track heading. The
        # interval oracle expresses the 12-bit angular window independently.
        for raw in range(65536):
            heading = C.c_int16(raw).value
            for car, track in ((heading, 0), (123, heading)):
                delta = (car + track - 2048) % 4096
                assert native.run_heading(car, track) == int(1025 <= delta < 3072)

        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0, 0x400000)
        write = lambda a, data: memory.mem_write(a & 0x1FFFFFFF, bytes(data))
        read = lambda a, size: bytes(memory.mem_read(a & 0x1FFFFFFF, size))
        put32 = lambda a, v: write(a, struct.pack('<I', v & 0xFFFFFFFF))
        player_root = address_of('PlayerCars')
        vehicles = (player_root, player_root+800, 0x80180010)
        nodes, stack, stop = 0x80190010, 0x803F0000, 0x803FF000
        globals_ = tuple(address_of(n) for n in
                         ('RaceState', 'TrackEventSfxEnabled', 'ReplayState', 'ChaseViewEnabled', 'LinkHostFlag'))
        services = {address_of(n): i+1 for i, n in enumerate((
            'UpdateCarGear', 'UpdatePlayerSteering', 'UpdateCarEngine',
            'PlayTrackEventSfx', 'UpdateRumbleStrip', 'SetLoopVoiceVolumes',
            'GetMappedPadButtons', 'ResetCarSlotBuffers', 'UpdateCarSteering',
            'IntegrateCarMotion', 'UpdateCarTrackNode', 'ComputeWheelOffsets', 'ConstrainCarToTrack'))}
        executable = (ROOT/'assets/PAL/main.exe').read_bytes()
        base = struct.unpack_from('<I', executable, 24)[0]
        starts = {n: address_of(n) for n in ('UpdatePlayerCar', 'IsCarFacingReverse',
                                            'StepCarPhysics', 'ResetVehicleShake')}
        spans = {'UpdatePlayerCar': 0x12C, 'IsCarFacingReverse': 0x40,
                 'StepCarPhysics': 0x44, 'ResetVehicleShake': 0x3C}
        randomizer = random.Random(0x2284822B64)
        cases = []
        for case in range(CASES):
            data = bytearray(randomizer.randbytes(STATE_SIZE))
            for index in range(3):
                struct.pack_into('<II', data, index*800+0x20C, 0, 0)
                struct.pack_into('<i', data, index*800+0xF0, (case+index) % 4)
            state = STATES[case % 8]
            values = (state, (-1, 0, 1, 2)[(case//8) % 4],
                      (-1, 0, 1, 2, 3)[(case//32) % 5],
                      randomizer.randrange(-2147483648, 2147483648),
                      (-32768, -1, 0, 32767)[(case//160) % 4])
            struct.pack_into('<5i', data, 2400, *values)
            player = (-2147483648, -1, 0, 1, 2, 3, 2147483647)[(case//3) % 7]
            cases.append((data, case % 3, player))

        def snapshot():
            values = [int.from_bytes(read(a, 2 if i == 4 else 4), 'little', signed=True)
                      for i, a in enumerate(globals_)]
            return b''.join(read(a, 800) for a in vehicles)+struct.pack('<5i', *values)+read(nodes, 240)

        for version in ('original', 'linked'):
            for name, start in starts.items():
                code = executable[0x800+start-base:0x800+start-base+spans[name]]
                if version == 'linked':
                    with (ROOT/'build/PAL/main.elf').open('rb') as stream:
                        elf = ELFFile(stream)
                        symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(name)[0]
                        section = elf.get_section(symbol['st_shndx'])
                        offset = symbol['st_value']-section['sh_addr']
                        assert symbol['st_value'] == start and symbol['st_size'] == spans[name]
                        code = section.data()[offset:offset+symbol['st_size']]
                write(start, code)
            counts = [0]*13
            for number, (data, index, player) in enumerate(cases+cases):
                operation, case = divmod(number, CASES)
                output = (C.c_ubyte*(EVENT_SIZE*9))()
                expected_events = native.run_case(case, operation, index, player, C.create_string_buffer(bytes(data)), output)
                expected = bytes(output)
                for i, address in enumerate(vehicles):
                    write(address, data[i*800:(i+1)*800])
                for i, address in enumerate(globals_):
                    write(address, data[2400+i*4:2400+i*4+(2 if i == 4 else 4)])
                write(nodes, data[2420:]);put32(address_of('TrackNodes'), nodes)
                write(stack-0x108, b'\xA6'*0x308)
                def store(cpu, access, address, size, value, user):
                    # Only the selected car's reverse flag, global toggle and
                    # callee frame are written by these actual C bodies.
                    frame_size = 0x18 if operation else 0x68
                    allowed = ((vehicles[index]+0x1EA, 2), (vehicles[index]+0x1E4, 2),
                               (globals_[3], 4), (stack-frame_size, frame_size))
                    assert any(a <= address and address+size <= a+n for a, n in allowed), hex(address)
                cpu = Cpu(memory, 0, 100, 101, 102, store)
                cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
                cpu.r[4:6] = [vehicles[index], player & 0xFFFFFFFF]
                cpu.r[29] = stack;cpu.r[31] = stop;cpu.pc = starts['StepCarPhysics' if operation else 'UpdatePlayerCar']
                event = 0
                for instruction in range(1000):
                    if cpu.pc == stop:
                        break
                    if cpu.pc not in services:
                        cpu.step();continue
                    kind = services[cpu.pc];counts[kind-1] += 1
                    assert cpu.delay is None and event < expected_events
                    if kind <= 5 or kind >= 9:
                        assert cpu.r[4] == vehicles[index]
                    if kind in (1, 2, 5):a = cpu.r[5]
                    elif 6 <= kind <= 8:a = cpu.r[4]
                    else:a = 0
                    if kind == 6:assert cpu.r[5] == cpu.r[6] == 0
                    entry = struct.pack('<III', kind, a, 0)+snapshot()
                    assert entry == expected[event*EVENT_SIZE:(event+1)*EVENT_SIZE], (version, case, kind)
                    event += 1
                    if kind <= 5 or kind >= 9:
                        address = vehicles[index]+0x184
                        put32(address, int.from_bytes(read(address, 4), 'little') ^
                              ((case*0x9E3779B9+kind) & 0xFFFFFFFF))
                    if kind == 1 and case & 1:put32(globals_[0], STATES[(case>>3)&7])
                    if kind == 2 and case & 2:put32(globals_[1], (case>>5)&3)
                    if kind == 3 and case & 4:put32(globals_[2], (case>>7)&3)
                    if kind == 4 and case & 8:put32(globals_[2], 2)
                    if kind == 7 and case & 16:put32(globals_[0], STATES[(case>>8)&7])
                    result = (0, 0x10000, 0xFFFFFFFF)[case % 3] if kind == 7 else 0xDEAD0002
                    resume = cpu.r[31]
                    for register in (2, 3, *range(4, 16), 24, 25):cpu.r[register] = 0xDEAD0000+register
                    cpu.r[2] = result;cpu.hi = 0x12345678;cpu.lo = 0x87654321;cpu.pc = resume
                assert cpu.pc == stop and event == expected_events, (version, case)
                assert snapshot() == expected[event*EVENT_SIZE:event*EVENT_SIZE+STATE_SIZE], (version, case, 'final')
                assert cpu.r[29] == stack and cpu.r[31] == stop
                for register in (*range(16, 24), 28, 30):assert cpu.r[register] == 0xBEEF0000+register
                guard_size = 0x108-(0x18 if operation else 0x68)
                assert read(stack-0x108, guard_size) == b'\xA6'*guard_size
                assert read(stack, 0x200) == b'\xA6'*0x200
            assert all(counts), counts
            print('PASS', version, CASES*2, 'whole-caller traces; service calls', counts)
            # Independent PAL store extents/order, including the two byte
            # fields between the reset halfwords. All other owner bytes survive.
            stores = ((0x1A0, 2), (0x1A4, 1), (0x1A5, 1), (0x1A2, 2),
                      (0x19C, 2), (0x19E, 2), (0x198, 2), (0x19A, 2),
                      (0x1B4, 2), (0x1B6, 2), (0x1B0, 2), (0x1B2, 2),
                      (0x1B8, 2), (0x1BA, 2))
            reset_random = random.Random(0x2BEF8198)
            for case in range(4096):
                before = bytearray(reset_random.randbytes(800))
                struct.pack_into('<II', before, 0x20C, 0, 0)
                wanted = bytearray(before)
                for offset, size in stores:wanted[offset:offset+size] = bytes(size)
                output = (C.c_ubyte*800)()
                native.run_reset(C.create_string_buffer(bytes(before)), output)
                assert bytes(output) == wanted, (version, case, 'native reset')
                address = vehicles[2];seen = []
                write(address-16, b'\xA6'*16+before+b'\x6A'*16)
                def reset_store(cpu, access, at, size, value, user):
                    seen.append((at-address, size))
                    assert seen == list(stores[:len(seen)]) and value == 0
                cpu = Cpu(memory, 0, 100, 101, 102, reset_store)
                cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
                cpu.r[4] = address;cpu.r[29] = stack;cpu.r[31] = stop
                cpu.pc = starts['ResetVehicleShake']
                for instruction in range(30):
                    if cpu.pc == stop:break
                    cpu.step()
                assert cpu.pc == stop and seen == list(stores)
                assert read(address, 800) == wanted
                assert read(address-16, 16) == b'\xA6'*16 and read(address+800, 16) == b'\x6A'*16
                assert cpu.r[29] == stack and cpu.r[31] == stop
                for register in (*range(16, 24), 28, 30):assert cpu.r[register] == 0xBEEF0000+register
            print('PASS', version, '4096 typed shake resets; all PAL stores and whole-owner guards')
        print('PASS 131072 signed heading cases; actual caller/helper, full native pointers, UBSan')


if __name__ == '__main__':
    main()
