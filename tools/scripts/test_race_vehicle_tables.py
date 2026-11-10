"""Real race-table bodies against original MIPS, complete native vehicles and services."""
import ctypes as C
import os
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32

from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
CASES = 2048
CARS, PLAYERS, SNAPSHOTS = 0x80200010, address_of('PlayerCars'), 0x80210010
BANKS = ((CARS, 8), (PLAYERS, 2), (SNAPSHOTS, 8))
RECORDS = (0x80220010, 0x80230010)
GLOBALS = (
    ('NumCars', 2), ('TrackLength', 4), ('D_800AC3FC', 2), ('D_800ABEC4', 4),
    ('D_80107178', 2), ('SavedRaceState', 4), ('TotalRaceTime', 4),
    ('ReplayState', 4), ('RaceState', 4),
)
STATE_SIZE = 18*800+2*0x218+64+9*4+4
EVENT_SIZE = STATE_SIZE+16
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "vehicle_wire.h"
#include "game/race/raceinit.h"
#include "game/race/racesave.h"
#include "game/car/carsetup.h"
#include "game/car/carlane.h"
#include "game/race/racesamp.h"
#include "rr/bios_devices.h"
#include "rr/player_status.h"
static struct {u8 before[16];RaceVehicle values[8];u8 after[16];} cars,snapshots;
static struct {u8 before[16];RaceVehicle values[2];u8 after[16];} players;
RaceVehicle *CarTable[8],*CarSnapshots[8];
static struct {
 s16 count; s32 length; s32 marker; s16 participants;
 s32 saved,time,replay,race;
} state;
static s16 *count_address=&state.count;
static s8 links[2][2];
#define PlayerCars players.values
#define NumCars (*count_address)
#define TrackLength state.length
#define D_800ABEC4 state.marker
#define D_80107178 state.participants
#define SavedRaceState state.saved
#define TotalRaceTime state.time
#define ReplayState state.replay
#define RaceState state.race
#define LinkEntryState links
static unsigned scenario,events;
static s32 race_mode;
static int cleared;
static u8 *output;
enum {STATE_SIZE=18*800+2*0x218+64+9*4+4};
static void put32(u8 **p,u32 x){memcpy(*p,&x,4);*p+=4;}
static u32 get32(const u8 **p){u32 x;memcpy(&x,*p,4);*p+=4;return x;}
static RaceVehicle *resolve(u32 address) {
 unsigned i;
 for(i=0;i<8;i++) {
  if(address==0x80200010U+i*800)return &cars.values[i];
  if(address==0x80210010U+i*800)return &snapshots.values[i];
 }
 for(i=0;i<2;i++)if(address==RR_PLAYERS+i*800)return &players.values[i];
 assert(!"unknown vehicle address");return 0;
}
static u32 address(const RaceVehicle *v) {
 unsigned i;
 for(i=0;i<8;i++) {
  if(v==&cars.values[i])return 0x80200010U+i*800;
  if(v==&snapshots.values[i])return 0x80210010U+i*800;
 }
 for(i=0;i<2;i++)if(v==&players.values[i])return RR_PLAYERS+i*800;
 assert(!"unmapped native vehicle");return 0;
}
static void snapshot(u8 *p) {
 unsigned i;u8 *start=p;
 for(i=0;i<18;i++) {
  RaceVehicle *v=i<8?&cars.values[i]:i<10?&players.values[i-8]:&snapshots.values[i-10];
  rr_vehicle_wire_padding_value(v,cleared&&i==8?0:0xA5);
  rr_vehicle_wire_store_fields(p,v);p+=800;
 }
 for(i=0;i<2;i++){memcpy(p,&rr_test_records[i].record,0x218);p+=0x218;}
 for(i=0;i<8;i++)put32(&p,address(CarTable[i]));
 for(i=0;i<8;i++)put32(&p,address(CarSnapshots[i]));
 put32(&p,NumCars);put32(&p,TrackLength);put32(&p,(s16)players.values[1].flags1E4);put32(&p,D_800ABEC4);
 put32(&p,D_80107178);put32(&p,SavedRaceState);put32(&p,TotalRaceTime);
 put32(&p,ReplayState);put32(&p,RaceState);memcpy(p,links,4);p+=4;
 assert(p-start==STATE_SIZE);
}
static void event(u32 kind,u32 a,u32 b,u32 c) {
 assert(events++<7);put32(&output,kind);put32(&output,a);put32(&output,b);put32(&output,c);
 snapshot(output);output+=STATE_SIZE;
}
void InitCarTable(int mode) {
 assert(mode==race_mode);
 unsigned i;event(1,0,0,0);
 for(i=0;i<8;i++)CarTable[i]=&cars.values[(scenario+i)%8];
 if(scenario&1){CarTable[0]=&players.values[0];CarTable[1]=&players.values[1];}
 if(scenario&4)NumCars=(scenario>>3)%(race_mode==4?5:9);
}
void InitPlayerCar(RaceVehicle *v,s32 slot,s32 grid) {
 unsigned record=((unsigned)slot^scenario)&1;event(2,address(v),slot,grid);
 v->first=(s32)((u32)v->first^scenario*0x9E3779B9U);
 v->control.record18=&rr_test_records[record].record;
 v->control.handling1C=&v->control.record18->handling1C8;
 if(race_mode==4) {
  if(scenario&8)((s8*)links)[(slot+1)%4]^=1;
  if(scenario&16)NumCars=(scenario>>6)%5;
 }
 if(scenario&32)CarTable[(slot+1)%8]=&cars.values[(slot+2)%8];
}
void InitComCars(s32 count) {
 event(3,count,0,0);cars.values[7].first=(s32)((u32)cars.values[7].first^(u32)count);
 D_800ABEC4=(s32)((u32)D_800ABEC4^(u32)count);
}
void SetupRaceMemoryArena(s32 layout,s32 mode) {
 event(4,layout,mode,0);SavedRaceState=(s32)(scenario^0x1234);TotalRaceTime=scenario+13;
}
void PsyqBzero(void *buffer,s32 size) {
 assert(buffer==&players.values[0]&&size==sizeof(players.values[0]));
 event(5,address(buffer),800,0);memset(buffer,0,(size_t)size);cleared=1;
}
'''
TAIL = r'''
int run_case(unsigned sc,int init,s32 mode,const u8 *input,u8 *result) {
 unsigned i;const u8 *p=input;scenario=sc;race_mode=mode;events=0;output=result;cleared=0;
 count_address=&state.count;
 assert(sizeof(void*)==8&&(uintptr_t)cars.values>UINT32_MAX);
 rr_vehicle_wire_reset();memset(&cars,0xA5,sizeof(cars));memset(&players,0xA5,sizeof(players));
 memset(&snapshots,0xA5,sizeof(snapshots));
 rr_vehicle_record_resolve(0x80220010);rr_vehicle_record_resolve(0x80230010);
 for(i=0;i<18;i++) {
  RaceVehicle *v=i<8?&cars.values[i]:i<10?&players.values[i-8]:&snapshots.values[i-10];
  rr_vehicle_wire_load(v,p);p+=800;
 }
 for(i=0;i<2;i++){memcpy(&rr_test_records[i].record,p,0x218);p+=0x218;}
 for(i=0;i<8;i++)CarTable[i]=resolve(get32(&p));
 for(i=0;i<8;i++)CarSnapshots[i]=resolve(get32(&p));
 NumCars=(s16)get32(&p);TrackLength=(s32)get32(&p);players.values[1].flags1E4=(u16)get32(&p);
 D_800ABEC4=(s32)get32(&p);D_80107178=(s16)get32(&p);SavedRaceState=(s32)get32(&p);
 TotalRaceTime=(s32)get32(&p);ReplayState=(s32)get32(&p);RaceState=(s32)get32(&p);
 memcpy(links,p,4);p+=4;assert(p-input==STATE_SIZE);
 if(init)InitRaceCars(mode);else SaveCarSnapshots(mode);
 event(0,0,0,0);
 for(i=0;i<16;i++)assert(cars.before[i]==0xA5&&cars.after[i]==0xA5&&
  players.before[i]==0xA5&&players.after[i]==0xA5&&snapshots.before[i]==0xA5&&snapshots.after[i]==0xA5);
 assert(rr_test_record_count==2&&rr_test_handling_count==0);
 return events;
}
void run_snapshot_count_aliases(void) {
 RaceVehicle expected[8],sources[8];unsigned raw,i;
 /* The first destination aliases the live count. Its new signed flags determine
  * how many later slots are visited; an initially positive count guarantees one. */
 for(raw=0;raw<65536;raw++) {
  s16 bound;u16 bits=(u16)raw;unsigned copies;
  memcpy(&bound,&bits,2);if(bound>8)continue;
  rr_vehicle_wire_reset();rr_vehicle_record_resolve(0x80220010);rr_vehicle_record_resolve(0x80230010);
  memset(&cars,0xA5,sizeof(cars));memset(&snapshots,0x6A,sizeof(snapshots));
  for(i=0;i<8;i++) {
   cars.values[i].control.record18=&rr_test_records[i&1].record;
   cars.values[i].control.handling1C=&rr_test_records[i&1].record.handling1C8;
   snapshots.values[i].control.record18=&rr_test_records[1-(i&1)].record;
   snapshots.values[i].control.handling1C=&rr_test_records[1-(i&1)].record.handling1C8;
   CarTable[i]=&cars.values[i];CarSnapshots[i]=&snapshots.values[i];
  }
  cars.values[0].flags1E4=bits;snapshots.values[0].flags1E4=8;
  count_address=(s16*)&snapshots.values[0].flags1E4;
  memcpy(expected,snapshots.values,sizeof(expected));memcpy(sources,cars.values,sizeof(sources));
  copies=bound<=1?1:(unsigned)bound;
  memcpy(expected,cars.values,copies*sizeof(expected[0]));
  RaceState=-37;ReplayState=0;SavedRaceState=42;SaveCarSnapshots(-123);
  assert(SavedRaceState==-37&&ReplayState==1);
  assert(!memcmp(expected,snapshots.values,sizeof(expected))&&!memcmp(sources,cars.values,sizeof(sources)));
  for(i=0;i<16;i++)assert(cars.before[i]==0xA5&&cars.after[i]==0xA5&&
   snapshots.before[i]==0x6A&&snapshots.after[i]==0x6A);
 }
 count_address=&state.count;
}
'''


def main():
    os.chdir(ROOT)
    owners = source_owners(ROOT/'src/main/PAL')
    bodies = ''.join(definition_source(owners[n].read_text(), n) for n in ('InitRaceCars', 'SaveCarSnapshots'))
    with TemporaryDirectory() as temporary:
        directory = Path(temporary)
        source, library = directory/'native.c', directory/'native.dylib'
        source.write_text(PREFIX.replace('RR_PLAYERS', hex(PLAYERS)+'U')+bodies+TAIL)
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-Wno-unused-parameter', '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
                        '-Iinclude', '-Itools/scripts', str(source), '-o', str(library)], check=True)
        native = C.CDLL(str(library))
        native.run_case.argtypes = [C.c_uint, C.c_int, C.c_int32, C.c_void_p, C.c_void_p]
        native.run_case.restype = C.c_int
        native.run_snapshot_count_aliases.argtypes = []
        native.run_snapshot_count_aliases.restype = None
        native.run_snapshot_count_aliases()
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0, 0x400000)
        write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b))
        read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
        put = lambda a, v, n: write(a, (v & ((1 << (n*8))-1)).to_bytes(n, 'little'))
        original = (ROOT/'assets/PAL/main.exe').read_bytes()
        base = struct.unpack_from('<I', original, 0x18)[0]
        write(base, original[0x800:])
        if os.getenv('RR_COMPILED_ELF'):
            with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                elf = ELFFile(stream)
                for name in ('InitRaceCars', 'SaveCarSnapshots'):
                    symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(name)[0]
                    section = elf.get_section(symbol['st_shndx'])
                    offset = symbol['st_value']-section['sh_addr']
                    assert symbol['st_value'] == address_of(name)
                    write(symbol['st_value'], section.data()[offset:offset+symbol['st_size']])
        table, snapshots = address_of('CarTable'), address_of('CarSnapshots')
        globals_ = [(address_of(n), size) for n, size in GLOBALS]
        link = address_of('LinkEntryState')
        stack, stop = 0x803F0000, 0x803FF000
        services = {address_of(n): i+1 for i, n in enumerate(
            ('InitCarTable', 'InitPlayerCar', 'InitComCars', 'SetupRaceMemoryArena', 'PsyqBzero'))}
        allowed = list(BANKS)+[(a, 0x218) for a in RECORDS]
        allowed = [(a, n*800) if (a, n) in BANKS else (a, n) for a, n in allowed]
        allowed += globals_+[(table, 32), (snapshots, 32), (link, 4), (stack-64, 64)]

        def store(cpu, access, address, size, value, user):
            address &= 0x1FFFFFFF
            assert any((a & 0x1FFFFFFF) <= address and address+size <= (a & 0x1FFFFFFF)+n
                       for a, n in allowed), (hex(address), size)

        cpu = Cpu(memory, 0, 100, 101, 102, store)

        def snapshot():
            state = b''.join(read(a, n*800) for a, n in BANKS)
            state += b''.join(read(a, 0x218) for a in RECORDS)
            state += read(table, 32)+read(snapshots, 32)
            state += b''.join(struct.pack('<i', int.from_bytes(read(a, n), 'little', signed=True))
                              for a, n in globals_)
            return state+read(link, 4)

        rng = random.Random(0x37B3C)
        seen_modes, seen_counts, seen_events = set(), set(), set()
        counts = (-32768, -7, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8)
        for init in (True, False):
            for case in range(CASES):
                mode = (0, 1, 2, 3, 4, 5, 6, -1, -2147483648, 2147483647)[case % 10]
                count = case % 5 if init and mode == 4 else counts[(case//10) % len(counts)]
                length = (0, 1, 17, 12345, 100000)[case % 5]
                data = bytearray(rng.randbytes(STATE_SIZE))
                for i in range(18):
                    record = RECORDS[(i+case) & 1]
                    struct.pack_into('<II', data, i*800+0x20C, record, record+0x1C8)
                sources = [CARS+((i+case) % 8)*800 for i in range(8)]
                destinations = [SNAPSHOTS+i*800 for i in range(8)]
                if case % 4 == 1:
                    destinations = sources[:]
                elif case % 4 == 2:
                    destinations = sources[1:]+sources[:1]
                elif case % 4 == 3:
                    destinations = [SNAPSHOTS+(case % 8)*800]*8
                at = 18*800+2*0x218
                struct.pack_into('<16I', data, at, *sources, *destinations)
                replay = (0, 1, 2, -1, -2147483648, 2147483647)[(case//4) % 6]
                global_values = (count, length, -137, 0x12345678, -291, -7, 233, replay, -1234567)
                struct.pack_into('<9i', data, at+64, *global_values)
                data[-4:] = bytes((1, 0, 0xFF, 1)[(i+(case//10)) % 4] for i in range(4))
                output = (C.c_ubyte*(EVENT_SIZE*8))()
                source_buffer = C.create_string_buffer(bytes(data))
                total = native.run_case(case, init, mode, source_buffer, output)
                expected = bytes(output)[:total*EVENT_SIZE]
                p = 0
                for address, n in BANKS:
                    write(address-16, b'\xA5'*16+data[p:p+n*800]+b'\xA5'*16)
                    p += n*800
                for address in RECORDS:
                    write(address-16, b'\xA5'*16+data[p:p+0x218]+b'\xA5'*16)
                    p += 0x218
                write(table, data[p:p+32]);write(snapshots, data[p+32:p+64]);p += 64
                for (address, size), value in zip(globals_, global_values):
                    put(address, value, size)
                write(link, data[-4:])
                cpu.reset();cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
                cpu.r[4] = mode & 0xFFFFFFFF;cpu.r[29] = stack;cpu.r[31] = stop
                cpu.pc = address_of('InitRaceCars' if init else 'SaveCarSnapshots')
                event = 0
                for instruction in range(10000):
                    if cpu.pc == stop:
                        break
                    if cpu.pc not in services:
                        cpu.step();continue
                    kind = services[cpu.pc];seen_events.add(kind)
                    a, b, c = cpu.r[4:7]
                    args = (kind, a, b, c) if kind == 2 else (kind, a, b, 0) if kind == 4 else \
                           (kind, a, 800, 0) if kind == 5 else (kind, a, 0, 0) if kind == 3 else (kind, 0, 0, 0)
                    position = event*EVENT_SIZE
                    assert struct.pack('<4I', *args)+snapshot() == expected[position:position+EVENT_SIZE], \
                        (init, case, mode, kind, event, 'service entry')
                    if kind == 1:
                        for i in range(8):
                            put(table+i*4, CARS+((case+i) % 8)*800, 4)
                        if case & 1:
                            put(table, PLAYERS, 4);put(table+4, PLAYERS+800, 4)
                        if case & 4:
                            put(globals_[0][0], (case >> 3) % (5 if mode == 4 else 9), 2)
                    elif kind == 2:
                        assert 0 <= b < 8
                        at = a+0x184;put(at, int.from_bytes(read(at, 4), 'little')^(case*0x9E3779B9), 4)
                        record = RECORDS[(b ^ case) & 1]
                        put(a+0x20C, record, 4);put(a+0x210, record+0x1C8, 4)
                        if mode == 4:
                            if case & 8:
                                at = link+(b+1) % 4;write(at, bytes([read(at, 1)[0] ^ 1]))
                            if case & 16:
                                put(globals_[0][0], (case >> 6) % 5, 2)
                        if case & 32:
                            put(table+((b+1) % 8)*4, CARS+((b+2) % 8)*800, 4)
                    elif kind == 3:
                        at = CARS+7*800+0x184;put(at, int.from_bytes(read(at, 4), 'little') ^ a, 4)
                        at = globals_[3][0];put(at, int.from_bytes(read(at, 4), 'little') ^ a, 4)
                    elif kind == 4:
                        put(globals_[5][0], case ^ 0x1234, 4);put(globals_[6][0], case+13, 4)
                    else:
                        assert a == PLAYERS and b == 800
                        write(a, bytes(800))
                    event += 1;resume = cpu.r[31]
                    for register in (2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 24, 25):
                        cpu.r[register] = 0xDEAD0000+register
                    cpu.pc = resume;cpu.hi = 0x12345678;cpu.lo = 0x87654321
                assert cpu.pc == stop and event == total-1, (init, case, mode, hex(cpu.pc), event)
                assert struct.pack('<4I', 0, 0, 0, 0)+snapshot() == expected[event*EVENT_SIZE:], \
                    (init, case, mode, 'final state')
                assert cpu.r[29] == stack and cpu.r[31] == stop
                for register in (*range(16, 24), 28, 30):
                    assert cpu.r[register] == 0xBEEF0000+register
                for address, size in [(a, n*800) for a, n in BANKS]+[(a, 0x218) for a in RECORDS]:
                    assert read(address-16, 16) == b'\xA5'*16 and read(address+size, 16) == b'\xA5'*16
                if init:
                    seen_modes.add(mode)
                else:
                    seen_counts.add(count)
        assert len(seen_modes) == 10 and seen_counts == set(counts) and seen_events == set(range(1, 6))
        print('PASS', CASES*2, ('compiled' if os.getenv('RR_COMPILED_ELF') else 'original'),
              'MIPS/native cases; 32777 native snapshot/count aliases; complete owners, mutable services, guards, ABI and UBSan')


if __name__ == '__main__':
    main()
