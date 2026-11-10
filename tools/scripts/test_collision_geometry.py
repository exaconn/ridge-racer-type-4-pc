"""Actual collision geometry helpers: native owners and original/linked MIPS.

NormalClip is an instrumented GTE boundary. Its signed-word result, live
corner mutations and complete call traces are shared across the two engines;
this fixture does not emulate the GTE or the full collision controller.
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
NAMES = ('PointInQuad', 'PointInQuadPtr', 'ComputeQuadMidpoints')
CASES, STATE, EVENT = 4096, 1040, 12 + 1040
OUTPUT = 8 + 4 * EVENT + STATE
SCRATCH, EXTRA, STACK, STOP = 0x1F800000, 0x80200010, 0x803F0000, 0x803FF000
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "game/race/racemai2.h"
#include "psyq/libgte/gtemath3.h"
static struct {
    u8 before[16];
    union {
        u32 words[256];
        struct {
            u8 prefix[0x80];
            ScratchpadCollision collision;
            u8 suffix[0x400-0x80-sizeof(ScratchpadCollision)];
        } data;
    } scratch;
    CollisionPoint extra[4];
    u8 after[16];
} world;
#define SCRATCH_CAR_WORK (&world.scratch.data.collision)
static unsigned scenario, events;
static CollisionPoint *selected;
static u8 *trace;
enum {STATE=1040,EVENT=1052};
static void snapshot(u8 *out) {
    memcpy(out,&world.scratch,1024);memcpy(out+1024,world.extra,16);
}
int NormalClip(int first,int second,int point) {
    u32 words[3]={(u32)first,(u32)second,(u32)point};
    s16 x[3],y[3];s64 area;u32 bits;s32 result;unsigned serial=events++,i;
    assert(serial<4);
    for(i=0;i<3;i++) {
        u16 lo=(u16)words[i],hi=(u16)(words[i]>>16);
        memcpy(&x[i],&lo,2);memcpy(&y[i],&hi,2);
    }
    area=((s64)x[1]-x[0])*((s64)y[2]-y[0])-
         ((s64)y[1]-y[0])*((s64)x[2]-x[0]);
    bits=(u32)area;memcpy(&result,&bits,4);
    if(scenario&1) {
        unsigned index=(scenario+serial)%4;
        selected[index].word=(s32)((u32)selected[index].word^(0x13579BDFU+serial));
    }
    memcpy(trace+serial*EVENT,words,12);snapshot(trace+serial*EVENT+12);
    return result;
}
'''
TAIL = r'''
void run_case(unsigned number,unsigned operation,unsigned index,s32 point,
              const u8 *input,u8 *output) {
    unsigned i;u32 result=0;scenario=number;events=0;trace=output+8;
    assert(operation<3&&index<3);memset(&world,0xA6,sizeof(world));
    memcpy(&world.scratch,input,1024);memcpy(world.extra,input+1024,16);
    selected=index==2?world.extra:&world.scratch.data.collision.pts40[index][1];
    assert(sizeof(void*)==8&&(uintptr_t)selected>UINT32_MAX);
    assert((uintptr_t)SCRATCH_CAR_WORK>UINT32_MAX);
    memset(output,0,8+4*EVENT+STATE);
    if(operation==0)result=PointInQuad(selected[0].word,selected[1].word,
                                    selected[2].word,selected[3].word,point);
    else if(operation==1)result=PointInQuadPtr(selected,point);
    else {assert(index<2);ComputeQuadMidpoints((s32)index);}
    memcpy(output,&result,4);memcpy(output+4,&events,4);
    snapshot(output+8+4*EVENT);
    for(i=0;i<16;i++)assert(world.before[i]==0xA6&&world.after[i]==0xA6);
}
'''


def clip(words):
    a, b, p = (struct.unpack('<hh', struct.pack('<I', value)) for value in words)
    result = ((b[0]-a[0])*(p[1]-a[1]) - (b[1]-a[1])*(p[0]-a[0])) & 0xFFFFFFFF
    return result if result < 0x80000000 else result - 0x100000000


def main():
    owners = source_owners(ROOT/'src/main/PAL')
    bodies = '\n'.join(definition_source(owners[name].read_text(), name) for name in NAMES)
    with TemporaryDirectory() as temp:
        source, library = Path(temp)/'native.c', Path(temp)/'native.dylib'
        source.write_text(PREFIX+bodies+TAIL)
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC',
                        '-Wall', '-Wextra', '-Werror', '-fsanitize=undefined,bounds',
                        '-fno-sanitize-recover=all', '-I'+str(ROOT/'include'),
                        str(source), '-o', str(library)], check=True)
        native = C.CDLL(str(library))
        native.run_case.argtypes = [C.c_uint, C.c_uint, C.c_uint, C.c_int32, C.c_void_p, C.c_void_p]
        executable = (ROOT/'assets/PAL/main.exe').read_bytes()
        base = struct.unpack_from('<I', executable, 24)[0]
        spans = {name: (int(address, 16), int(size, 16)) for name, address, size in
                 map(str.split, (ROOT/'configs/PAL/functions.txt').read_text().splitlines())}
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0, 0x400000);memory.mem_map(SCRATCH, 0x1000)
        write = lambda address, data: memory.mem_write(address & 0x1FFFFFFF, bytes(data))
        read = lambda address, size: bytes(memory.mem_read(address & 0x1FFFFFFF, size))
        snapshot = lambda: read(SCRATCH, 1024)+read(EXTRA, 16)
        rng = random.Random(0x3840838550)
        limits = (-32768, -32767, -3, -1, 0, 1, 3, 32766, 32767)
        cases = []
        for case in range(CASES):
            data = bytearray(rng.randbytes(STATE))
            for index in range(2):
                for corner in range(4):
                    for axis in range(2):
                        coordinate = limits[(case//(9 if axis else 1)+index+corner) % 9]
                        if case >= 1024:coordinate = rng.randrange(-32768, 32768)
                        struct.pack_into('<h', data, 0xC4+index*36+corner*4+axis*2, coordinate)
            # Rectangle boundary, inside/outside and degenerate inputs complement
            # arbitrary signed-halfword polygons and determinant wrapping.
            if case < 512:
                side = 0 if case & 16 else 123
                corners = ((-side, -side), (side, -side), (-side, side), (side, side))
                for corner, xy in enumerate(corners):
                    struct.pack_into('<hh', data, 0xC4+corner*4, *xy)
            xy = (limits[case % 9], limits[(case//9) % 9])
            if case < 512:xy = ((-124, -123, 0, 123, 124)[case % 5], 0)
            point = struct.unpack('<i', struct.pack('<hh', *xy))[0]
            cases.append((data, point))

        for version in ('original', 'linked'):
            for name in NAMES:
                entry, size = spans[name]
                code = executable[0x800+entry-base:0x800+entry-base+size]
                if version == 'linked':
                    with (ROOT/'build/PAL/main.elf').open('rb') as stream:
                        elf = ELFFile(stream);symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(name)[0]
                        assert symbol['st_value'] == entry and symbol['st_size'] == size
                        section = elf.get_section(symbol['st_shndx']);offset = entry-section['sh_addr']
                        code = section.data()[offset:offset+size]
                write(entry, code)
            covered = [set(), set()]
            for case, (data, point) in enumerate(cases):
                for operation, name in enumerate(NAMES):
                    index = case % (2 if operation == 2 else 3)
                    corners = EXTRA if index == 2 else SCRATCH+0xC4+index*36
                    output = (C.c_ubyte*OUTPUT)()
                    native.run_case(case, operation, index, point, C.create_string_buffer(bytes(data)), output)
                    expected = bytes(output);result, events = struct.unpack_from('<II', expected)
                    write(SCRATCH, data[:1024]);write(EXTRA-16, b'\xA6'*16+data[1024:]+b'\x6A'*16)
                    write(STACK-256, b'\xA6'*512)
                    originals = struct.unpack('<4I', read(corners, 16));writes = []
                    def store(cpu, access, address, size, value, user):
                        if STACK-64 <= address and address+size <= STACK:return
                        assert operation == 2 and size == 2
                        relative = address-(SCRATCH+0xC0+index*36)
                        assert relative in (0, 2, *range(20, 36, 2)), hex(address)
                        writes.append(relative)
                    cpu = Cpu(memory, 0, 100, 101, 102, store)
                    cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
                    cpu.r[29] = STACK;cpu.r[31] = STOP;cpu.pc = spans[name][0]
                    if operation == 0:
                        cpu.r[4:8] = originals;write(STACK+16, struct.pack('<I', point & 0xFFFFFFFF))
                    elif operation == 1:cpu.r[4:6] = [corners, point & 0xFFFFFFFF]
                    else:cpu.r[4] = index
                    event = 0
                    for instruction in range(500):
                        if cpu.pc == STOP:break
                        if cpu.pc != address_of('NormalClip'):cpu.step();continue
                        assert cpu.delay is None and event < events and operation < 2
                        args = cpu.r[4:7];value = clip(args)
                        if case & 1:
                            at = corners+((case+event) % 4)*4
                            bits = int.from_bytes(read(at, 4), 'little') ^ (0x13579BDF+event)
                            write(at, struct.pack('<I', bits))
                        actual = struct.pack('<3I', *args)+snapshot()
                        assert actual == expected[8+event*EVENT:8+(event+1)*EVENT], (version, case, operation, event)
                        event += 1;resume = cpu.r[31]
                        for register in (2, 3, *range(4, 16), 24, 25):cpu.r[register] = 0xDEAD0000+register
                        cpu.r[2] = value & 0xFFFFFFFF;cpu.pc = resume;cpu.hi = 0x12345678;cpu.lo = 0x87654321
                    assert cpu.pc == STOP and event == events
                    assert snapshot() == expected[8+4*EVENT:], (version, case, operation, 'final')
                    if operation < 2:
                        assert cpu.r[2] == result and result in (0, 1)
                        covered[operation].add((events, result))
                    else:
                        oracle = bytearray(data);offset = 0xC0+index*36
                        xy = [struct.unpack_from('<hh', data, offset+4+i*4) for i in range(4)]
                        for midpoint, (first, second) in enumerate(((0, 1), (0, 2), (1, 3), (2, 3))):
                            struct.pack_into('<hh', oracle, offset+20+midpoint*4,
                                             *((xy[first][axis]+xy[second][axis])//2 for axis in range(2)))
                        struct.pack_into('<hh', oracle, offset, *(sum(p[axis] for p in xy)//4 for axis in range(2)))
                        assert snapshot() == oracle
                        assert writes == [20, 22, 24, 26, 28, 30, 32, 34, 0, 2]
                    assert cpu.r[29] == STACK and cpu.r[31] == STOP
                    for register in (*range(16, 24), 28, 30):assert cpu.r[register] == 0xBEEF0000+register
                    assert read(STACK-256, 192) == b'\xA6'*192
                    assert read(STACK+20, 236) == b'\xA6'*236
                    assert read(EXTRA-16, 16) == b'\xA6'*16 and read(EXTRA+16, 16) == b'\x6A'*16
            for outcomes in covered:assert outcomes == {(1, 0), (2, 0), (3, 0), (4, 0), (4, 1)}, outcomes
            print('PASS', version, CASES*3, 'actual geometry flows; both owners, whole scratch and live corner traces')


if __name__ == '__main__':
    main()
