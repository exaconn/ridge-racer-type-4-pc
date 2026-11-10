"""Actual course-light setter and consumers, native owners and original MIPS."""
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
ALIAS, TABLES = 0x80200010, 0x80210010
STATE_SIZE = 44+2*8*44+4+12+24+4
EVENT_SIZE = 16+STATE_SIZE+18
CASES = 4096
FUNCTIONS = ('SetLightTable', 'SetLightColorsScaled', 'SetLightColorsFromRow')
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "game/data/bankrec.h"
#include "game/gfx/gfxmtx.h"
#include "rr/matrix_workspace.h"
static struct {
 u8 before[16];union {PreviewMatrixWorkspace basis;TrackLighting2C row;} alias;
 TrackLighting2C tables[2][8];u8 ambient[4];PreviewColor4 colors[3];
 LightDirectionBits8 directions[3];TrackLighting2C *table;u8 after[16];
} world;
#define LightMatrix world.alias.basis
#define TrackLightTable world.table
#define D_8009F924 world.ambient
#define D_8009F928 world.colors
#define D_8009F934 world.directions
static u8 *result;static unsigned scenario,event_count;
enum {STATE_SIZE=44+2*8*44+4+12+24+4};
static u32 address(const void *p) {
 unsigned i;if(!p)return 0;if(p==&world.alias.row)return 0x80200010;
 for(i=0;i<16;i++)if(p==&world.tables[i/8][i%8])return 0x80210010+i*44;
 assert(!"unmapped track-light root");return 0;
}
static TrackLighting2C *resolve(u32 p) {
 unsigned i;if(!p)return 0;if(p==0x80200010)return &world.alias.row;
 for(i=0;i<16;i++)if(p==0x80210010U+i*44)return &world.tables[i/8][i%8];
 assert(!"unknown light-row address");return 0;
}
static void snapshot(u8 *p) {
 u32 pointer=address(world.table);
 memcpy(p,&world.alias,44);p+=44;memcpy(p,world.tables,704);p+=704;
 memcpy(p,world.ambient,4);p+=4;memcpy(p,world.colors,12);p+=12;
 memcpy(p,world.directions,24);p+=24;memcpy(p,&pointer,4);
}
static void event(u32 kind,u32 red,u32 green,u32 blue,const void *colors) {
 u32 words[4]={kind,red,green,blue};assert(event_count++<3);
 memcpy(result,words,16);snapshot(result+16);
 if(colors)memcpy(result+16+STATE_SIZE,colors,18);result+=16+STATE_SIZE+18;
}
void SetBackColor(s32 red,s32 green,s32 blue) {
 event(1,red,green,blue,0);LightMatrix.words[7]^=scenario*0x17171717U;
 SetLightTable(&world.tables[(scenario>>2)&1][4]);
 world.colors[scenario%3].red^=(u8)(scenario+37);
 world.directions[(scenario>>2)%3].x^=(u16)(scenario*17);
}
void SetColorMatrix(const MatrixWords32 *matrix) {
 assert((uintptr_t)matrix>UINT32_MAX&&((uintptr_t)matrix&3)==0);
 event(2,1,0,0,matrix);LightMatrix.words[6]^=scenario*0x1234567U;
 TrackLightTable->lighting04.ambient[3]^=0x5A;
}
'''
TAIL = r'''
void run_case(unsigned number,unsigned kind,s32 argument,const u8 *input,u8 *output) {
 u32 pointer;unsigned i;const u8 *p=input;TrackLighting2C *root;
 memset(&world,0xA5,sizeof(world));memset(output,0,3*(16+STATE_SIZE+18));
 scenario=number;event_count=0;result=output;
 assert(sizeof(void*)==8&&sizeof(unsigned long)==sizeof(void*));
 memcpy(&world.alias,p,44);p+=44;memcpy(world.tables,p,704);p+=704;
 memcpy(world.ambient,p,4);p+=4;memcpy(world.colors,p,12);p+=12;
 memcpy(world.directions,p,24);p+=24;memcpy(&pointer,p,4);p+=4;assert(p-input==STATE_SIZE);
 root=resolve(pointer);SetLightTable(root);assert(TrackLightTable==root);
 assert(!root||(uintptr_t)root>UINT32_MAX);
 if(kind)SetLightColorsFromRow(argument);else SetLightColorsScaled(argument);
 event(0,0,0,0,0);assert(event_count==3);
 for(i=0;i<16;i++)assert(world.before[i]==0xA5&&world.after[i]==0xA5);
}
'''


def main():
    os.chdir(ROOT);owners = source_owners(ROOT/'src/main/PAL')
    bodies = '\n'.join(definition_source(owners[n].read_text(), n) for n in FUNCTIONS)
    with TemporaryDirectory() as temporary:
        source, library = Path(temporary)/'native.c', Path(temporary)/'native.dylib'
        source.write_text(PREFIX+bodies+TAIL)
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-Iinclude',
                        str(source), '-o', str(library)], check=True)
        native = C.CDLL(str(library));native.run_case.restype = None
        native.run_case.argtypes = [C.c_uint, C.c_uint, C.c_int32, C.c_void_p, C.c_void_p]
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN);memory.mem_map(0, 0x400000)
        write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b))
        read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
        put = lambda a, x: write(a, struct.pack('<I', x & 0xFFFFFFFF))
        original = (ROOT/'assets/PAL/main.exe').read_bytes();base = struct.unpack_from('<I', original, 0x18)[0]
        write(base, original[0x800:])
        if os.getenv('RR_COMPILED_ELF'):
            with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                elf = ELFFile(stream)
                for name in FUNCTIONS:
                    symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(name)[0]
                    section = elf.get_section(symbol['st_shndx']);offset = symbol['st_value']-section['sh_addr']
                    assert symbol['st_value'] == address_of(name)
                    write(symbol['st_value'], section.data()[offset:offset+symbol['st_size']])
        light, table = address_of('LightMatrix'), address_of('TrackLightTable')
        ambient, colors, directions = [address_of(n) for n in ('D_8009F924', 'D_8009F928', 'D_8009F934')]
        stack, stop = 0x803F0000, 0x803FF000
        services = {address_of('SetBackColor'): 1, address_of('SetColorMatrix'): 2}
        regions = ((ALIAS, 44), (TABLES, 704), (ambient, 4), (colors, 12), (directions, 24), (table, 4))
        # The alias row occupies the very same storage as the real global matrix.
        def snapshot():
            return read(light, 32)+read(ALIAS+32, 12)+b''.join(read(a, n) for a, n in regions[1:])

        def store(cpu, access, address, size, value, user):
            address &= 0x1FFFFFFF
            allowed = ((light, 32), (table, 4), (stack-80, 80))
            assert any((a & 0x1FFFFFFF) <= address and address+size <= (a & 0x1FFFFFFF)+n for a, n in allowed), (hex(address), size)

        class LightingCpu(Cpu):
            def mem_read(self, address, size):
                # A real typed row may alias LightMatrix in the native owner;
                # translate only that alternate fixture address, not target code.
                if ALIAS <= address < ALIAS+32:return read(light+address-ALIAS, size)
                return super().mem_read(address, size)

        cpu = LightingCpu(memory, 0, 100, 101, 102, store)
        rng = random.Random(0x3D01C);scales = (-8421504, -65536, -257, -1, 0, 1, 255, 256, 65535, 8421504)
        counts = set();alias_cases = 0
        for kind in (0, 1):
            for case in range(CASES):
                initial = bytearray(rng.randbytes(STATE_SIZE));overlap = (case//8)%4 == 0
                root = ALIAS if overlap else TABLES+((case>>2)&1)*8*44+4*44
                if not kind and case%7 == 0:root = 0
                argument = 0 if kind and overlap else (case%8)-4 if kind else scales[case%len(scales)]
                if kind:counts.add(argument);alias_cases += overlap
                struct.pack_into('<I', initial, STATE_SIZE-4, root)
                input_buffer = C.create_string_buffer(bytes(initial));output = (C.c_ubyte*(3*EVENT_SIZE))()
                native.run_case(case, kind, argument, input_buffer, output);expected = bytes(output)
                p = 0
                for address, size in regions:
                    write(address, initial[p:p+size]);p += size
                write(light, initial[:32]);write(ALIAS+32, initial[32:44]);write(light-16, b'\xA5'*16)
                def setup(function, arg):
                    cpu.reset();cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
                    cpu.pc = address_of(function);cpu.r[4] = arg & 0xFFFFFFFF;cpu.r[29] = stack;cpu.r[31] = stop
                setup('SetLightTable', root)
                for _ in range(20):
                    if cpu.pc == stop:break
                    cpu.step()
                assert cpu.pc == stop and int.from_bytes(read(table, 4), 'little') == root
                setup('SetLightColorsFromRow' if kind else 'SetLightColorsScaled', argument);event = 0
                for instruction in range(1000):
                    if cpu.pc == stop:break
                    if cpu.pc not in services:cpu.step();continue
                    call = services[cpu.pc];a, b, c = cpu.r[4:7]
                    captured = bytes(18)
                    if call == 2:
                        assert stack-80 <= a and a+18 <= stack and a%4 == 0
                        captured = read(a, 18);a, b, c = 1, 0, 0
                    payload = struct.pack('<4I', call, a, b, c)+snapshot()+captured
                    assert payload == expected[event*EVENT_SIZE:(event+1)*EVENT_SIZE], (kind, case, call, 'entry')
                    if call == 1:
                        at = light+28;put(at, int.from_bytes(read(at, 4), 'little') ^ (case*0x17171717))
                        put(table, TABLES+((case>>2)&1)*8*44+4*44)
                        at = colors+(case%3)*4;write(at, bytes([read(at, 1)[0] ^ ((case+37)&255)]))
                        at = directions+((case>>2)%3)*8
                        write(at, struct.pack('<H', int.from_bytes(read(at, 2), 'little') ^ ((case*17)&65535)))
                    else:
                        at = light+24;put(at, int.from_bytes(read(at, 4), 'little') ^ (case*0x1234567))
                        at = int.from_bytes(read(table, 4), 'little')+43;write(at, bytes([read(at, 1)[0] ^ 0x5A]))
                    event += 1;resume = cpu.r[31]
                    for register in (2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 24, 25):cpu.r[register] = 0xDEAD0000+register
                    cpu.pc = resume;cpu.hi = 0x12345678;cpu.lo = 0x87654321
                assert cpu.pc == stop and event == 2, (kind, case, hex(cpu.pc), event)
                assert struct.pack('<4I', 0, 0, 0, 0)+snapshot()+bytes(18) == expected[2*EVENT_SIZE:], (kind, case, 'final')
                assert cpu.r[29] == stack and cpu.r[31] == stop
                for register in (*range(16, 24), 28, 30):assert cpu.r[register] == 0xBEEF0000+register
                assert read(light-16, 16) == b'\xA5'*16
        assert counts == set(range(-4, 4)) and alias_cases == CASES//4
        print('PASS', CASES*2, ('compiled' if os.getenv('RR_COMPILED_ELF') else 'original'),
              'MIPS/native lighting cases; real setter/full pointers, negative row indices, aliased lighting row, complete owners, callback traces, ABI, guards and UBSan')


if __name__ == '__main__':
    main()
