"""Actual RRO dispatcher against original/compiled MIPS and full native owners.

Transform and primitive emitters are instrumented services, not GTE emulation.
Their mutations check captured command cursors and live scratch/texture inputs.
"""
import ctypes as C
import os
from pathlib import Path
import re
import struct
import subprocess
from tempfile import TemporaryDirectory

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32

from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
SCRATCH, WORK = 0x1F800000, 0x1F80007C
POSITION, MATRIX = 0x80200010, 0x80201010
VERTICES, NORMALS, ORDER = 0x80202010, 0x80203010, 0x80204010
TABLES, STREAMS = 0x80205010, 0x80206010
STACK, STOP = 0x803F0000, 0x803FF000
CASES, WORDS, TRACE_WORDS = 8192, 128, 1 + 6*12 + 1

PREFIX = r'''
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "rr/rro_model.h"
#include "game/camera/viewcam.h"
#include "game/gfx/gtepoly1.h"
static struct World {
 u8 before[16];RroModelScratch scratch;u8 middle[16];
 Vector positions[2];MatrixWords32 matrices[2];
 Vec3s8 vertices[2][4],normals[2][4];u32 order[2][256];
 RroBankCommand tables[2][4];u32 streams[8][128];
 s32 page;u8 after[16];
} world,expected;
#define SCRATCHPAD ((u8*)&world.scratch)
#define ModelTexturePage world.page
static char UnknownRroTypeFmt[]="unknown RRO type";
static unsigned scenario,selected,events,skip[2];
static s32 inputFlags;
static u32 *trace;
static s32 word(u32 x){s32 y;memcpy(&y,&x,4);return y;}
static u32 address(const void *pointer) {
 uintptr_t p=(uintptr_t)pointer,b;unsigned i;
 const struct {const void *p;u32 a,n;} regions[]={
  {&world.scratch.work7C,0x1F80007C,sizeof(world.scratch.work7C)},
  {world.positions,0x80200010,sizeof(world.positions)},
  {world.matrices,0x80201010,sizeof(world.matrices)},
  {world.vertices,0x80202010,sizeof(world.vertices)},
  {world.normals,0x80203010,sizeof(world.normals)},
  {world.order,0x80204010,sizeof(world.order)},
  {world.streams,0x80206010,sizeof(world.streams)}};
 if(pointer==world.tables[0])return 0x80205010;
 if(pointer==world.tables[1])return 0x80205020;
 for(i=0;i<sizeof(regions)/sizeof(regions[0]);i++) {
  b=(uintptr_t)regions[i].p;
  if(p>=b&&p-b<regions[i].n)return regions[i].a+(u32)(p-b);
 }
 assert(!"unmapped native pointer");return 0;
}
static void event(u32 kind,const u32 *commands,RroRenderWork *work,s32 count) {
 assert(events++<6&&!memcmp(&world,&expected,sizeof(world)));
 *trace++=kind;*trace++=commands?address(commands):0;
 *trace++=work?address(work):0;*trace++=(u32)count;
 if(work) {
  *trace++=(u32)work->flags08;*trace++=work->envClut0C;*trace++=work->envTpage10;
  *trace++=address(work->vertices00);*trace++=address(work->normals04);
  *trace++=address(work->ordering14);
 }else{for(unsigned i=0;i<6;i++)*trace++=0;}
 *trace++=(u32)world.page;*trace++=address(world.scratch.scene.commands50);
}
static s32 nextPage(void) {
 static const u32 values[]={0,1,15,16,31,32,0x7FFFFFFF,0xFFFFFFFF};
 return word(values[(scenario>>3)&7]);
}
void SetGteObjectTransform(ObjectTransform38 *output,const s16 *position,s32 *matrix) {
 assert(output==&world.scratch.work7C.transform);
 assert(position==(const s16*)&world.positions[selected]);
 assert(matrix==world.matrices[selected].w);
 assert(events++==0&&!memcmp(&world,&expected,sizeof(world)));
 *trace++=1;*trace++=address(output);*trace++=address(position);*trace++=address(matrix);
 for(unsigned i=0;i<6;i++)*trace++=0;
 *trace++=(u32)world.page;*trace++=address(world.scratch.scene.commands50);
 for(unsigned i=0;i<sizeof(*output);i++) {
  ((u8*)output)[i]=(u8)(scenario*17+i*13);
  ((u8*)&expected.scratch.work7C.transform)[i]=(u8)(scenario*17+i*13);
 }
 world.page=expected.page=nextPage();
 world.scratch.scene.commands50=expected.scratch.scene.commands50=world.tables[(scenario>>1)&1];
 world.scratch.scene.vertices54=expected.scratch.scene.vertices54=world.vertices[(scenario>>2)&1];
 world.scratch.scene.normals58=expected.scratch.scene.normals58=world.normals[(scenario>>4)&1];
 world.scratch.scene.view.projection.pointers.order=expected.scratch.scene.view.projection.pointers.order=world.order[(scenario>>5)&1];
 /* Independent packing specification; the production dispatcher writes these
  * fields after returning from this service. Preserve every other owner byte. */
 RroRenderWork *w=&expected.scratch.work7C.render;
 u32 page=(u32)nextPage();
 w->flags08=inputFlags;
 w->envClut0C=((((page&16)*16+127)*64)|((page&15)*4)|3)<<16;
 w->envTpage10=(0x20|(page&31))<<16;
 w->vertices00=world.scratch.scene.vertices54;w->normals04=world.scratch.scene.normals58;
 w->ordering14=world.order[(scenario>>5)&1]+128;
}
static const u32 *emit(unsigned kind,const u32 *commands,RroRenderWork *work,s32 count) {
 event(kind,commands,work,count);
 assert(work==&world.scratch.work7C.render);
 work->flags08=expected.scratch.work7C.render.flags08=word((u32)work->flags08^0x12345678U);
 work->ordering14=expected.scratch.work7C.render.ordering14=world.order[(scenario>>6)&1]+129;
 world.scratch.scene.commands50=expected.scratch.scene.commands50=world.tables[(scenario>>7)&1];
 world.page=expected.page=word((u32)world.page^0x80000055U);
 return commands+skip[kind-2];
}
const u32 *EmitRroLitGouraudQuads(const u32 *commands,RroRenderWork *work,s32 count) {
 return emit(2,commands,work,count);
}
const u32 *EmitRroEnvMapQuads(const u32 *commands,RroRenderWork *work,s32 count) {
 return emit(3,commands,work,count);
}
static void RroSetColor(u32 color) {
 assert(color==0x3C808080);event(4,0,0,word(color));
}
s32 DebugPrintStub(const char *format,...) {
 va_list ap;va_start(ap,format);s32 type=va_arg(ap,int),count=va_arg(ap,int);va_end(ap);
 assert(format==UnknownRroTypeFmt);event(5,0,0,count);
 trace[-11]=(u32)type;
 return word(0xA5000000U^scenario);
}
'''

TAIL = r'''
void run_case(unsigned number,s32 page,s32 object,s32 flags,const u32 *streams,u32 *out) {
 assert(sizeof(void*)==8&&(uintptr_t)&world>UINT32_MAX);
 assert(offsetof(RroRenderWork,normals04)==sizeof(void*));
 assert(offsetof(RroRenderWork,ordering14)==2*sizeof(void*)+16);
 assert(offsetof(RroRenderWork,vertices28)==3*sizeof(void*)+32);
 scenario=number;selected=number&1;events=0;trace=out+1;inputFlags=flags;
 skip[0]=(number>>8)&3;skip[1]=(number>>10)&3;
 memset(out,0,(1+6*12+1)*4);memset(&world,0xA5,sizeof(world));
 memcpy(world.streams,streams,sizeof(world.streams));world.page=page;
 for(unsigned i=0;i<2;i++)for(unsigned j=0;j<4;j++)world.tables[i][j].pointer=world.streams[i*4+j];
 world.scratch.scene.commands50=world.tables[0];
 world.scratch.scene.vertices54=world.vertices[0];world.scratch.scene.normals58=world.normals[0];
 world.scratch.scene.view.projection.pointers.order=world.order[0];
 expected=world;
 s32 result=DrawRroModel(&world.positions[selected],&world.matrices[selected],object,flags);
 assert(!memcmp(&world,&expected,sizeof(world)));
 out[0]=events;out[1+6*12]=(u32)result;
}
'''


def main():
    os.chdir(ROOT)
    owner = source_owners(ROOT/'src/main/PAL')['DrawRroModel']
    body = definition_source(owner.read_text(), 'DrawRroModel')
    # Adapt the existing target register binding and the genuine GTE write.
    body, pins = re.subn(r'\s+__asm__\("\$\d+"\)', '', body)
    assert pins == 1
    operation = 'RR_GTE_MTC2_RGB(0x3C808080);'
    assert body.count(operation) == 1
    body = body.replace(operation, 'RroSetColor(0x3C808080);')
    with TemporaryDirectory() as directory:
        source, library = Path(directory)/'native.c', Path(directory)/'native.dylib'
        source.write_text(PREFIX+body+TAIL)
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-Iinclude',
                        str(source), '-o', str(library)], check=True)
        native = C.CDLL(str(library))
        native.run_case.argtypes = [C.c_uint, C.c_int32, C.c_int32, C.c_int32, C.c_void_p, C.c_void_p]
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0, 0x400000); memory.mem_map(SCRATCH, 4096)
        write = lambda a, data: memory.mem_write(a & 0x1FFFFFFF, bytes(data))
        read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
        put = lambda a, value: write(a, struct.pack('<I', value & 0xFFFFFFFF))
        get = lambda a: int.from_bytes(read(a, 4), 'little')
        executable = (ROOT/'assets/PAL/main.exe').read_bytes()
        base, = struct.unpack_from('<I', executable, 0x18); write(base, executable[0x800:])
        start = address_of('DrawRroModel')
        if os.getenv('RR_COMPILED_ELF'):
            with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                elf = ELFFile(stream); symbol = elf.get_section_by_name('.symtab').get_symbol_by_name('DrawRroModel')[0]
                section = elf.get_section(symbol['st_shndx']); offset = symbol['st_value']-section['sh_addr']
                assert symbol['st_value'] == start
                write(start, section.data()[offset:offset+symbol['st_size']])
        services = {address_of(n): i for i, n in enumerate(
            ('SetGteObjectTransform', 'EmitRroLitGouraudQuads', 'EmitRroEnvMapQuads', 'DebugPrintStub'))}
        page_address = address_of('ModelTexturePage')
        format_address = address_of('UnknownRroTypeFmt')

        def store(cpu, access, address, size, value, user):
            address &= 0x1FFFFFFF
            assert any((a & 0x1FFFFFFF) <= address and address+size <= (a & 0x1FFFFFFF)+n
                       for a, n in ((WORK, 24), (STACK-64, 64))), (hex(address), size)

        cpu = Cpu(memory, 0, 100, 101, 102, store)
        profiles = (0, 1, 15, 16, 31, 32, 0x7FFFFFFF, 0xFFFFFFFF)
        counts = (0, 1, 2, 32767, -1, -2, -32768, -32767)
        layouts, returns = set(), set()
        for case in range(CASES):
            selected, object_index = case & 1, (case >> 3) & 3
            page = profiles[(case >> 6) & 7]; flags = (case*0x9E3779B9) & 0xFFFFFFFF
            skip = [(case >> 8) & 3, (case >> 10) & 3]
            streams = bytearray(b'\xA5'*(8*WORDS*4))
            for slot in range(8):
                at = slot*WORDS
                def command(index, value): struct.pack_into('<I', streams, (at+index)*4, value & 0xFFFFFFFF)
                a, b = (case+slot) % 4, (case//4+slot) % 4
                cursor = 0; command(cursor, a << 16); cursor += 1+a*4
                command(cursor, (b << 16)|1); cursor += 1+b*6
                command(cursor, ((counts[(case+slot) % 8] & 0xFFFF) << 16)|2); cursor += 1+skip[0]
                command(cursor, ((counts[(case//8+slot) % 8] & 0xFFFF) << 16)|3); cursor += 1+skip[1]
                command(cursor, 0 if not (case & 4) else (0x80000000|((case << 8)&0x7FFF0000)|0xFFFF))
            output = (C.c_uint32*TRACE_WORDS)(); input_buffer = C.create_string_buffer(bytes(streams))
            native.run_case(case, C.c_int32(page), object_index, C.c_int32(flags), input_buffer, output)
            expected_trace = list(output)
            write(SCRATCH, b'\xA5'*0x400); write(STREAMS, streams)
            for i in range(2):
                write(TABLES+i*16, struct.pack('<4I', *(STREAMS+(i*4+j)*WORDS*4 for j in range(4))))
            for offset, value in [(4, ORDER), (0x50, TABLES), (0x54, VERTICES), (0x58, NORMALS)]: put(SCRATCH+offset, value)
            put(page_address, page)
            cpu.reset(); cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
            cpu.r[4:8] = [POSITION+selected*16, MATRIX+selected*32, object_index, flags]
            cpu.r[29] = STACK; cpu.r[31] = STOP; cpu.pc = start
            trace = []

            def event(kind, commands=0, work=0, count=0):
                values = [get(WORK+offset) for offset in (8, 12, 16, 0, 4, 20)] if work else [0]*6
                trace.extend([kind, commands, work, count & 0xFFFFFFFF, *values,
                              get(page_address), get(SCRATCH+0x50)])

            for _ in range(2000):
                if cpu.pc == STOP: break
                instruction = get(cpu.pc)
                if instruction >> 26 == 0x12:
                    assert (instruction >> 21) & 31 == 4 and (instruction >> 11) & 31 == 6
                    color = cpu.r[(instruction >> 16) & 31]; assert color == 0x3C808080
                    event(4, count=color); assert cpu.delay is None; cpu.pc += 4; continue
                if cpu.pc not in services: cpu.step(); continue
                service = services[cpu.pc]; result = 0
                if service == 0:
                    assert cpu.r[4:7] == [WORK, POSITION+selected*16, MATRIX+selected*32]
                    trace.extend([1, *cpu.r[4:7], *([0]*6), get(page_address), get(SCRATCH+0x50)])
                    write(WORK, bytes((case*17+i*13)&255 for i in range(56)))
                    put(page_address, profiles[(case >> 3) & 7])
                    for offset, value in [(0x50, TABLES+((case >> 1)&1)*16),
                                          (0x54, VERTICES+((case >> 2)&1)*32),
                                          (0x58, NORMALS+((case >> 4)&1)*32),
                                          (4, ORDER+((case >> 5)&1)*1024)]: put(SCRATCH+offset, value)
                elif service in (1, 2):
                    assert cpu.r[5] == WORK
                    event(service+1, *cpu.r[4:7])
                    put(WORK+8, get(WORK+8)^0x12345678)
                    put(WORK+20, ORDER+((case >> 6)&1)*1024+129*4)
                    put(SCRATCH+0x50, TABLES+((case >> 7)&1)*16)
                    put(page_address, get(page_address)^0x80000055)
                    result = cpu.r[4]+skip[service-1]*4
                else:
                    assert cpu.r[4] == format_address
                    event(5, count=cpu.r[6]); trace[-11] = cpu.r[5]
                    result = 0xA5000000^case
                resume = cpu.r[31]
                for reg in (*range(2, 16), 24, 25): cpu.r[reg] = 0xDEAD0000+reg
                cpu.r[2] = result; cpu.pc = resume
            assert cpu.pc == STOP
            actual = [len(trace)//12, *trace]
            actual += [0]*(TRACE_WORDS-1-len(actual)); actual.append(cpu.r[2])
            assert actual == expected_trace, (case, 'exact service trace / native full owner')
            assert cpu.r[29] == STACK and cpu.r[31] == STOP
            for reg in (*range(16, 24), 28, 30): assert cpu.r[reg] == 0xBEEF0000+reg
            layouts.add((selected, object_index, (case >> 1)&1, (case >> 6)&7))
            returns.add(cpu.r[2])
        assert len(layouts) == 2*4*2*8 and 0 in returns and 1 in returns and len(returns) > 100
        print('PASS', CASES, 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
              'MIPS/native RRO dispatch cases; typed full pointers, live texture/root mutation, captured cursors, signed counts, all command kinds, exact service trace, complete native owner, ABI, guards, UBSan')


if __name__ == '__main__':
    main()
