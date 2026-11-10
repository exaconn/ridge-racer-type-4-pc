"""Actual car-render allocator/reset with full native owners and original MIPS."""
import ctypes as C
import os
from pathlib import Path
import struct
import subprocess
from tempfile import TemporaryDirectory

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32

from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
ARENAS, MODELS, BOUNDS = (0x80200010, 0x80220010), (0x80180010, 0x80190010), (0x801A0010, 0x801A1010)
STACK, STOP = 0x803F0000, 0x803FF000
CASES, SLOT_BYTES, STATE = 4096, 16*200, 16*200+4+16+24+16
OUTPUT_SIZE = 4+5*STATE+2*65536
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "car_render_wire.h"
#include "game/data/cdfile2.h"
#include "game/car/bounds.h"
#include "rr/load_cursor.h"
#include "rr/bios_devices.h"
static struct World {
 u8 before[16];CarSlotBufferC8 slots[16];CarModelHeader models[2];
 CarBounds bounds[2];CarBounds *table[4];void *cursor;
 _Alignas(16) u8 arenas[2][65536];u8 after[16];
} world,expected;
#define CarSlotBuffers world.slots
#define CarBoundsTable world.table
#define LoadCursor world.cursor
static unsigned scenario,car,selected,calls;
static u8 *trace;
static const u32 arenaAddress[2]={0x80200010,0x80220010};
static const u32 modelAddress[2]={0x80180010,0x80190010};
static const u32 boundsAddress[2]={0x801A0010,0x801A1010};
static void *resolve(u32 p) {
 for(unsigned i=0;i<2;i++){
  if(p==modelAddress[i])return &world.models[i];
  if(p>=arenaAddress[i]&&p-arenaAddress[i]<65536)return world.arenas[i]+(p-arenaAddress[i]);
 }
 assert(!"unmapped PAL input pointer");return 0;
}
static u32 address(const void *p) {
 uintptr_t x=(uintptr_t)p,b;
 for(unsigned i=0;i<2;i++){
  if(p==&world.models[i])return modelAddress[i];
  if(p==&world.bounds[i])return boundsAddress[i];
  b=(uintptr_t)world.arenas[i];
  if(x>=b&&x-b<65536)return arenaAddress[i]+(u32)(x-b);
 }
 assert(!"truncated or unmapped native pointer");return 0;
}
static void snapshot(u8 *p) {
 u8 *start=p;
 for(unsigned i=0;i<16;i++){car_slot_store(p,&world.slots[i],address);p+=200;}
 car_put32(&p,address(world.cursor));
 for(unsigned i=0;i<2;i++){
  car_put32(&p,world.models[i].primary10.vertexCount);car_put32(&p,world.models[i].history34.vertexCount);
 }
 memcpy(p,world.bounds,24);p+=24;
 for(unsigned i=0;i<4;i++)car_put32(&p,address(world.table[i]));
 assert(p-start==STATE_SIZE);
}
static void reference_initialize(void) {
 u8 *p=world.cursor;
 u32 a=((world.models[selected].primary10.vertexCount+2U)/3U)*24U;
 u32 b=((world.models[selected].history34.vertexCount+2U)/3U)*24U;
 assert(a<=480&&b<=480);
 for(unsigned i=car*4;i<car*4+4;i++) {
  CarSlotBufferC8 *s=&expected.slots[i];s->model=&world.models[selected];
  s->color08=s->mainCount0C=s->mainIndex10=0;
  s->secondCount34=s->secondIndex38=s->phase5C=0;
  s->intensity60=s->intensity64=s->historyCount68=s->historyIndex6C=s->overlay70=0;
  for(unsigned j=0;j<4;j++){s->main14[j].vertices=p;p+=a;}
  for(unsigned j=0;j<4;j++){s->second3C[j].vertices=p;p+=b;}
  for(unsigned j=0;j<8;j++){s->history84[j].vertices=p;p+=b;}
 }
 expected.cursor=p;
}
void PsyqBzero(void *p,s32 length) {
 assert(calls<4&&length==16*sizeof(RroSpark)&&p==expected.cursor);
 expected.slots[car*4+calls].sparksC4=expected.cursor;
 assert(!memcmp(&world,&expected,sizeof(world)));
 snapshot(trace);trace+=STATE_SIZE;
 memset(p,0,(size_t)length);
 size_t offset=(size_t)((u8*)p-(u8*)&world);memset((u8*)&expected+offset,0,(size_t)length);
 if(scenario&1){
  world.cursor=expected.cursor=world.arenas[(scenario+calls)&1]+0x8000+calls*0x200;
  world.table[car]=expected.table[car]=&world.bounds[1-(car&1)];
  world.bounds[car&1].wheelRotationAxis=expected.bounds[car&1].wheelRotationAxis=0;
  world.models[selected].primary10.vertexCount=expected.models[selected].primary10.vertexCount=0xFFFFFFFFU-calls;
 }
 expected.cursor=(u8*)expected.cursor+16*sizeof(RroSpark);
 calls++;
}
'''
TAIL = r'''
void run_case(unsigned number,unsigned carIndex,unsigned modelIndex,const u8 *input,u8 *output) {
 assert(sizeof(void*)==8&&(uintptr_t)&world>UINT32_MAX);
 assert(sizeof(CarTrailBuffer)==16&&sizeof(RroEffectMesh)==24&&sizeof(CarSlotBufferC8)>200);
 scenario=number;car=carIndex;selected=modelIndex;calls=0;trace=output+4;
 assert(car<4&&selected<2);memset(output,0,OUTPUT_SIZE);
 memset(&world,0xA5,sizeof(world));const u8 *p=input;
 for(unsigned i=0;i<16;i++){car_slot_load(&world.slots[i],p,resolve);p+=200;}
 for(unsigned i=0;i<2;i++){
  world.models[i].primary10.vertexCount=car_get32(&p);world.models[i].history34.vertexCount=car_get32(&p);
 }
 memcpy(world.bounds,p,24);p+=24;
 for(unsigned i=0;i<4;i++)world.table[i]=&world.bounds[i&1];
 world.cursor=resolve(car_get32(&p));
 for(unsigned i=0;i<sizeof(world.arenas);i++)((u8*)world.arenas)[i]=(u8)(number*19+i*29);
 expected=world;
 unsigned needsClear=world.table[car]->wheelRotationAxis==2;
 if(number&0x1000){
  for(unsigned i=0;i<16;i++)expected.slots[i].mainCount0C=expected.slots[i].secondCount34=expected.slots[i].phase5C=expected.slots[i].historyCount68=0;
  ResetCarSlotBuffers();
 }else{
  reference_initialize();AllocCarSlotBuffers(&world.models[selected],(s32)car);
  assert(calls==(needsClear?4U:0U));
 }
 assert(!memcmp(&world,&expected,sizeof(world)));
 memcpy(output,&calls,4);snapshot(output+4+4*STATE_SIZE);
 memcpy(output+4+5*STATE_SIZE,world.arenas,sizeof(world.arenas));
}
'''


def main():
    os.chdir(ROOT); owners = source_owners(ROOT/'src/main/PAL')
    names = ('AllocCarSlotBuffers', 'ResetCarSlotBuffers')
    bodies = '\n'.join(definition_source(owners[n].read_text(), n) for n in names)
    with TemporaryDirectory() as directory:
        source, library = Path(directory)/'native.c', Path(directory)/'native.dylib'
        source.write_text(PREFIX.replace('STATE_SIZE', str(STATE))+bodies+TAIL.replace('STATE_SIZE', str(STATE)).replace('OUTPUT_SIZE', str(OUTPUT_SIZE)))
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-Iinclude', '-Itools/scripts',
                        str(source), '-o', str(library)], check=True)
        native = C.CDLL(str(library)); native.run_case.argtypes = [C.c_uint, C.c_uint, C.c_uint, C.c_void_p, C.c_void_p]
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN); memory.mem_map(0, 0x400000)
        write = lambda a, value: memory.mem_write(a & 0x1FFFFFFF, bytes(value))
        read = lambda a, size: bytes(memory.mem_read(a & 0x1FFFFFFF, size))
        put = lambda a, value: write(a, struct.pack('<I', value & 0xFFFFFFFF))
        get = lambda a: int.from_bytes(read(a, 4), 'little')
        original = (ROOT/'assets/PAL/main.exe').read_bytes(); base, = struct.unpack_from('<I', original, 0x18)
        write(base, original[0x800:]); slots, bounds, cursor = (address_of(n) for n in ('CarSlotBuffers', 'CarBoundsTable', 'LoadCursor'))
        if os.getenv('RR_COMPILED_ELF'):
            for item in os.environ['RR_COMPILED_ELF'].split(os.pathsep):
                with open(item, 'rb') as stream:
                    elf = ELFFile(stream)
                    for name in names:
                        symbols = elf.get_section_by_name('.symtab').get_symbol_by_name(name)
                        if not symbols or symbols[0]['st_shndx'] == 'SHN_ABS': continue
                        symbol = symbols[0]; section = elf.get_section(symbol['st_shndx']); offset = symbol['st_value']-section['sh_addr']
                        assert symbol['st_value'] == address_of(name)
                        write(symbol['st_value'], section.data()[offset:offset+symbol['st_size']])
        clear = address_of('PsyqBzero')

        def store(cpu, access, address, size, value, user):
            address &= 0x1FFFFFFF
            assert any((a & 0x1FFFFFFF) <= address and address+size <= (a & 0x1FFFFFFF)+n
                       for a, n in [(slots, SLOT_BYTES), (cursor, 4), (STACK-64, 64)]), (hex(address), size)

        cpu = Cpu(memory, 0, 100, 101, 102, store); profiles = (0, 1, 2, 3, 4, 5, 59, 60, 0xFFFFFFFF, 0xFFFFFFFE)
        seen, modes = set(), set()
        for case in range(CASES*2):
            car, selected = (case >> 1)&3, (case >> 3)&1
            reset = bool(case & 0x1000); name = names[1 if reset else 0]
            data = bytearray((case*19+i*29)&255 for i in range(SLOT_BYTES))
            for i in range(16):
                at = i*200; struct.pack_into('<I', data, at, MODELS[i&1])
                for j, off in enumerate([*(0x18+k*8 for k in range(4)), *(0x40+k*8 for k in range(4)), *(0x88+k*8 for k in range(8)), 0xC4]):
                    struct.pack_into('<I', data, at+off, ARENAS[(i+j)&1]+((i*17+j*31)&255)*16)
            counts = [profiles[(case//16+j*3)%len(profiles)] for j in range(4)]
            data += struct.pack('<4I', *counts)
            flags = ((case >> 5)&3, (case >> 7)&3)
            records = bytearray(b'\xA5'*24); records[10], records[22] = flags
            data += records+struct.pack('<I', ARENAS[(case >> 4)&1]+64+((case >> 9)&15)*16)
            output = (C.c_ubyte*OUTPUT_SIZE)(); incoming = C.create_string_buffer(bytes(data))
            native.run_case(case, car, selected, incoming, output); expected = bytes(output)
            write(slots-16, b'\xA6'*16+data[:SLOT_BYTES]+b'\x6A'*16)
            for i in range(2):
                write(MODELS[i], b'\xA5'*0x94); put(MODELS[i]+0x14, counts[i*2]); put(MODELS[i]+0x38, counts[i*2+1])
                write(BOUNDS[i], records[i*12:i*12+12])
                write(ARENAS[i]-16, b'\xA6'*16+bytes((case*19+(i*65536+j)*29)&255 for j in range(65536))+b'\x6A'*16)
            write(bounds, struct.pack('<4I', *(BOUNDS[i&1] for i in range(4)))); put(cursor, int.from_bytes(data[-4:], 'little'))
            cpu.reset(); cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
            cpu.r[4:6] = [MODELS[selected], car]; cpu.r[29] = STACK; cpu.r[31] = STOP; cpu.pc = address_of(name)
            calls, traces = 0, []

            def snapshot():
                return (read(slots, SLOT_BYTES)+read(cursor, 4)+b''.join(read(a+0x14, 4)+read(a+0x38, 4) for a in MODELS)
                        +b''.join(read(a, 12) for a in BOUNDS)+read(bounds, 16))

            for _ in range(2500):
                if cpu.pc == STOP: break
                if cpu.pc != clear: cpu.step(); continue
                assert not reset and calls < 4 and cpu.r[5] == 256
                assert cpu.r[4] == get(slots+(car*4+calls)*200+0xC4) == get(cursor)
                traces.append(snapshot()); write(cpu.r[4], bytes(256))
                if case&1:
                    put(cursor, ARENAS[(case+calls)&1]+0x8000+calls*0x200); put(bounds+car*4, BOUNDS[1-(car&1)])
                    write(BOUNDS[car&1]+10, b'\0'); put(MODELS[selected]+0x14, 0xFFFFFFFF-calls)
                calls += 1; resume = cpu.r[31]
                for reg in (*range(2, 16), 24, 25): cpu.r[reg] = 0xDEAD0000+reg
                cpu.pc = resume
            assert cpu.pc == STOP
            trace = struct.pack('<I', calls)+b''.join(traces)+bytes((4-calls)*STATE)+snapshot()+b''.join(read(a, 65536) for a in ARENAS)
            assert trace == expected, (case, name, 'full owner / pointer publication / clear service protocol')
            assert cpu.r[29] == STACK and cpu.r[31] == STOP
            for reg in (*range(16, 24), 28, 30): assert cpu.r[reg] == 0xBEEF0000+reg
            for address, size in [(slots, SLOT_BYTES), *((a, 65536) for a in ARENAS)]:
                assert read(address-16, 16) == b'\xA6'*16 and read(address+size, 16) == b'\x6A'*16
            if not reset: seen.add((car, selected, counts[selected*2], counts[selected*2+1])); modes.add(calls)
        assert modes == {0, 4} and {v[0] for v in seen} == set(range(4)) and {v[2] for v in seen} == set(profiles)
        print('PASS', CASES*2, 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
              'MIPS/native allocator/reset cases; complete slots/models, full pointers/strides, count wrap, mutable clear roots, captured metadata, complete buffers, ABI, guards, UBSan')


if __name__ == '__main__':
    main()
