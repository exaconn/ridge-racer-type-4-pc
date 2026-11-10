"""Actual transform body, native scratch pointers and original-MIPS service traces.

GTE services are instrumented here; this checks the C glue and memory ownership,
not the numerical accuracy of the hardware matrix implementation.
"""
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
SCRATCH, OUTPUT, POSITION, MATRIX = 0x1F800000, 0x1F80007C, 0x80250010, 0x80260010
ROTATION = SCRATCH+0x28
PIXELS, ORDER = 0x80270010, 0x80280010
STATE_SIZE = 36+32+112+24+64+64
EVENT_SIZE = 16+STATE_SIZE
CASES = 4096
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "game/camera/viewcam.h"
#include "rr/projection_state.h"
#include "psyq/libgte/gtemat7.h"
static struct {
 u8 before[16];ProjectionScratchPrefix view;MatrixWords32 rotation;
 ObjectTransform38 output[2];s16 position[2][6];s32 matrices[2][8];
 u8 pixels[2][16];u32 order[2][4];u8 after[16];
} world;
#define SCRATCHPAD ((u8*)&world.view)
#define SCRATCH_ROT_MATRIX world.rotation.w
static unsigned scenario,events,selected;
static u8 *result;
enum {STATE_SIZE=36+32+112+24+64+64};
static s32 word(u32 x){s32 y;memcpy(&y,&x,4);return y;}
static u32 get32(const u8 **p){u32 x;memcpy(&x,*p,4);*p+=4;return x;}
static void put32(u8 **p,u32 x){memcpy(*p,&x,4);*p+=4;}
static u32 address(const void *p) {
 uintptr_t x=(uintptr_t)p,base;unsigned i;
 struct {const void *p;u32 address,size;} regions[]={
  {&world.rotation,0x1F800028,32},{world.output,0x1F80007C,112},
  {world.position,0x80250010,24},{world.matrices,0x80260010,64},
  {world.pixels,0x80270010,32},{world.order,0x80280010,32}};
 for(i=0;i<sizeof(regions)/sizeof(regions[0]);i++) {
  base=(uintptr_t)regions[i].p;
  if(x>=base&&x-base<regions[i].size)return regions[i].address+(u32)(x-base);
 }
 assert(!"unmapped native pointer");return 0;
}
static void snapshot(u8 *p) {
 u8 *start=p;
 put32(&p,address(world.view.pointers.cursor));put32(&p,address(world.view.pointers.order));
 memcpy(p,(const u8*)&world.view+offsetof(ProjectionScratchPrefix,pose08.position00),28);p+=28;memcpy(p,&world.rotation,32);p+=32;
 memcpy(p,world.output,112);p+=112;memcpy(p,world.position,24);p+=24;
 memcpy(p,world.matrices,64);p+=64;memcpy(p,world.pixels,32);p+=32;
 memcpy(p,world.order,32);p+=32;assert(p-start==STATE_SIZE);
}
static void event(u32 kind,const void *a,const void *b,const void *c) {
 assert(events++<4);put32(&result,kind);put32(&result,a?address(a):0);
 put32(&result,b?address(b):0);put32(&result,c?address(c):0);
 snapshot(result);result+=STATE_SIZE;
}
static void change(unsigned n) {
 unsigned i;u32 v=scenario*0x9E3779B9U+n*0x1234567U;
 world.view.pose08.position00.words[0]=word((u32)world.view.pose08.position00.words[0]^v);
 world.view.pointers.cursor=world.pixels[(scenario+n)&1];
 world.view.pointers.order=world.order[(scenario+n+1)&1];
 world.rotation.w[1]=word((u32)world.rotation.w[1]^v);
 for(i=0;i<16;i++)world.pixels[n&1][i]^=(u8)(v+i);
}
Vec3i *ApplyMatrix(const MatrixWords32 *matrix,const Vec3s8 *input,Vec3i *output) {
 unsigned i;u32 values[3];event(1,matrix,input,output);
 assert(matrix==&world.rotation&&input==&world.output[selected].fields.relative00);
 assert(output==&world.output[selected].fields.transformed08);
 for(i=0;i<3;i++)values[i]=(u32)(s32)input->halves[i]^(u32)matrix->w[i]^(scenario*0x17171717U+i);
 change(1);
 output->x=word(values[0]);output->y=word(values[1]);output->z=word(values[2]);
 world.output[selected].fields.matrix18.fields.translation14[2]=word(~scenario);
 return output;
}
void SetRotMatrix(s32 *matrix) {
 event(2,matrix,0,0);change(2);
 world.output[selected].fields.transformed08.x=word((u32)world.output[selected].fields.transformed08.x^0xA55AA55AU);
 world.output[selected].fields.matrix18.fields.translation14[2]=word((u32)world.output[selected].fields.matrix18.fields.translation14[2]^0x12345678U);
}
void SetTransMatrix(s32 *matrix) {
 event(3,matrix,0,0);assert(matrix==world.output[selected].fields.matrix18.w);change(3);
 world.view.pose08.angles10.xyz.z=word((u32)world.view.pose08.angles10.xyz.z^scenario);
}
'''
TAIL = r'''
void run_case(unsigned number,const u8 *input,u8 *output) {
 const u8 *p=input;unsigned i;u32 cursor,order;const s16 *position;s32 *matrix;
 memset(&world,0xA5,sizeof(world));scenario=number;selected=number&1;events=0;result=output;
 assert(sizeof(void*)==8&&(uintptr_t)&world>UINT32_MAX);
 assert(offsetof(ProjectionScratchPrefix,pose08.position00)==2*sizeof(void*));
 cursor=get32(&p);order=get32(&p);assert(cursor==0x80270010U+(number&1)*16);
 assert(order==0x80280010U+((number>>1)&1)*16);
 world.view.pointers.cursor=world.pixels[number&1];world.view.pointers.order=world.order[(number>>1)&1];
 memcpy((u8*)&world.view+offsetof(ProjectionScratchPrefix,pose08.position00),p,28);p+=28;memcpy(&world.rotation,p,32);p+=32;
 memcpy(world.output,p,112);p+=112;memcpy(world.position,p,24);p+=24;
 memcpy(world.matrices,p,64);p+=64;memcpy(world.pixels,p,32);p+=32;
 memcpy(world.order,p,32);p+=32;assert(p-input==STATE_SIZE);
 position=number%3==1?world.output[selected].halves:world.position[(number>>2)&1];
 matrix=number%3==2?world.output[selected].fields.matrix18.w:world.matrices[(number>>3)&1];
 SetGteObjectTransform(&world.output[selected],position,matrix);
 event(0,0,0,0);assert(events==4);
 for(i=0;i<16;i++)assert(world.before[i]==0xA5&&world.after[i]==0xA5);
 for(i=offsetof(ProjectionScratchPrefix,pose08.angles10)+sizeof(Vec3i);i<sizeof(world.view);i++)assert(((u8*)&world.view)[i]==0xA5);
}
'''


def main():
    os.chdir(ROOT)
    owner = source_owners(ROOT/'src/main/PAL')['SetGteObjectTransform']
    body = definition_source(owner.read_text(), 'SetGteObjectTransform')
    with TemporaryDirectory() as temporary:
        source, library = Path(temporary)/'native.c', Path(temporary)/'native.dylib'
        source.write_text(PREFIX+body+TAIL)
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
                        '-Iinclude', str(source), '-o', str(library)], check=True)
        native = C.CDLL(str(library))
        native.run_case.argtypes = [C.c_uint, C.c_void_p, C.c_void_p]
        native.run_case.restype = None
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0, 0x400000);memory.mem_map(SCRATCH, 4096)
        write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b))
        read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
        put = lambda a, x: write(a, struct.pack('<I', x & 0xFFFFFFFF))
        original = (ROOT/'assets/PAL/main.exe').read_bytes()
        base = struct.unpack_from('<I', original, 0x18)[0];write(base, original[0x800:])
        start = address_of('SetGteObjectTransform')
        if os.getenv('RR_COMPILED_ELF'):
            with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                elf = ELFFile(stream);symbol = elf.get_section_by_name('.symtab').get_symbol_by_name('SetGteObjectTransform')[0]
                section = elf.get_section(symbol['st_shndx']);offset = symbol['st_value']-section['sh_addr']
                assert symbol['st_value'] == start
                write(start, section.data()[offset:offset+symbol['st_size']])
        services = {address_of(n): i+1 for i, n in enumerate(('ApplyMatrix', 'SetRotMatrix', 'SetTransMatrix'))}
        stack, stop = 0x803F0000, 0x803FF000
        regions = ((SCRATCH, 36), (ROTATION, 32), (OUTPUT, 112), (POSITION, 24),
                   (MATRIX, 64), (PIXELS, 32), (ORDER, 32))

        def snapshot():
            return b''.join(read(a, n) for a, n in regions)

        def store(cpu, access, address, size, value, user):
            address &= 0x1FFFFFFF
            assert OUTPUT <= address and address+size <= OUTPUT+112 or \
                   (stack & 0x1FFFFFFF)-32 <= address and address+size <= (stack & 0x1FFFFFFF), (hex(address), size)

        cpu = Cpu(memory, 0, 100, 101, 102, store)
        rng = random.Random(0x6EE3C)
        seen = set()
        for case in range(CASES):
            data = bytearray(rng.randbytes(STATE_SIZE))
            struct.pack_into('<II', data, 0, PIXELS+(case & 1)*16, ORDER+((case >> 1) & 1)*16)
            if case < 2048:
                profiles = (-2147483648, -65536, -32768, -1, 0, 1, 32767, 65535, 2147483647)
                for i in range(3):
                    struct.pack_into('<i', data, 8+i*4, profiles[(case+i) % len(profiles)])
                for i in range(6):
                    struct.pack_into('<i', data, 36+32+112+i*4, profiles[(case//9+i) % len(profiles)])
            source_buffer = C.create_string_buffer(bytes(data));output = (C.c_ubyte*(4*EVENT_SIZE))()
            native.run_case(case, source_buffer, output);expected = bytes(output)
            write(SCRATCH, b'\xA5'*4096)
            p = 0
            for address, size in regions:
                write(address, data[p:p+size]);p += size
            out = OUTPUT+(case & 1)*56
            pos = out if case % 3 == 1 else POSITION+((case >> 2) & 1)*12
            matrix = out+24 if case % 3 == 2 else MATRIX+((case >> 3) & 1)*32
            seen.add((case & 1, case % 3))
            cpu.reset();cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
            cpu.pc = start;cpu.r[4:7] = [out, pos, matrix];cpu.r[29] = stack;cpu.r[31] = stop
            event = 0
            for instruction in range(1000):
                if cpu.pc == stop:
                    break
                if cpu.pc not in services:
                    cpu.step();continue
                kind = services[cpu.pc];a, b, c = cpu.r[4:7]
                args = (kind, a, b, c) if kind == 1 else (kind, a, 0, 0)
                at = event*EVENT_SIZE
                assert struct.pack('<4I', *args)+snapshot() == expected[at:at+EVENT_SIZE], (case, kind, 'entry')
                values = None
                if kind == 1:
                    values = [(int.from_bytes(read(b+i*2, 2), 'little', signed=True) & 0xFFFFFFFF) ^
                              int.from_bytes(read(a+i*4, 4), 'little') ^ ((case*0x17171717+i) & 0xFFFFFFFF)
                              for i in range(3)]
                value = (case*0x9E3779B9+kind*0x1234567) & 0xFFFFFFFF
                put(SCRATCH+8, int.from_bytes(read(SCRATCH+8, 4), 'little') ^ value)
                put(SCRATCH, PIXELS+((case+kind) & 1)*16)
                put(SCRATCH+4, ORDER+((case+kind+1) & 1)*16)
                put(ROTATION+4, int.from_bytes(read(ROTATION+4, 4), 'little') ^ value)
                pixels = PIXELS+(kind & 1)*16
                write(pixels, bytes(x ^ ((value+i) & 255) for i, x in enumerate(read(pixels, 16))))
                if kind == 1:
                    for i in range(3):put(c+i*4, values[i])
                    put(out+0x34, ~case)
                elif kind == 2:
                    put(out+8, int.from_bytes(read(out+8, 4), 'little') ^ 0xA55AA55A)
                    put(out+0x34, int.from_bytes(read(out+0x34, 4), 'little') ^ 0x12345678)
                else:
                    put(SCRATCH+0x20, int.from_bytes(read(SCRATCH+0x20, 4), 'little') ^ case)
                event += 1;resume = cpu.r[31]
                for register in (2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 24, 25):
                    cpu.r[register] = 0xDEAD0000+register
                cpu.pc = resume;cpu.hi = 0x12345678;cpu.lo = 0x87654321
            assert cpu.pc == stop and event == 3, (case, hex(cpu.pc), event)
            assert struct.pack('<4I', 0, 0, 0, 0)+snapshot() == expected[3*EVENT_SIZE:], (case, 'final')
            assert cpu.r[29] == stack and cpu.r[31] == stop
            for register in (*range(16, 24), 28, 30):assert cpu.r[register] == 0xBEEF0000+register
            for a, n in ((SCRATCH+36, 4), (SCRATCH+72, 52), (OUTPUT+112, 4096-236)):
                assert read(a, n) == b'\xA5'*n
        assert len(seen) == 6
        print('PASS', CASES, ('compiled' if os.getenv('RR_COMPILED_ELF') else 'original'),
              'MIPS/native transform cases; full owners/pointers, low-half coordinates, aliased position/matrix, mutable GTE services, ABI, guards and UBSan')


if __name__ == '__main__':
    main()
