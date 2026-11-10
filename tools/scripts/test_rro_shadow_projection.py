"""Current shadow projection/culling vs original or compiled MIPS.

All native cases return before packet emission. The separate full-emission test
covers that path, including packet/head aliases. GTE results are scripted; this
checks vertex/command pointers, live loads, workspace writes and culling, not
GTE math.
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
from mips_preview_engine import signed
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
WORK, MESH, STREAM, VERTICES = 0x1F80007C, 0x80200010, 0x80210010, 0x80220010
STACK, STOP = 0x803F0000, 0x803FF000
CASES, STATE, MAX_EVENTS = 4096, 248, 64
OUTPUT = 4 + MAX_EVENTS*12 + STATE + 128
DEPTHS = (-2147483648, -1, 0, 1, 447, 448, 449, 895, 896, 1791, 1792, 32767, 65535, 2147483647, 128, 256)
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "rr/rro_effects.h"
#include "rr/gpu_address.h"
typedef union {
 u32 words[9];
 struct {u32 command;RroTexturedQuadPrefix quad;} decoded;
} ShadowStream;
static struct World {
 u8 before[16];RroEffectScratch scratch;RroEffectMesh mesh;
 ShadowStream streams[2];Vec3s8 vertices[2][8];u32 gpu[64];u8 after[16];
} world,expected;
#define SCRATCHPAD ((u8*)&world.scratch.scene)
static unsigned scenario,events,clips;static u32 data[32],*trace;
static const s32 depths[]={-2147483647-1,-1,0,1,447,448,449,895,896,1791,1792,32767,65535,2147483647,128,256};
static s32 word(u32 x){s32 y;memcpy(&y,&x,4);return y;}
static GpuDmaRegion gpuRegion;
static void installGpuMap(void) {
 gpuRegion=(GpuDmaRegion){world.gpu,0x80180000U,sizeof(world.gpu)};
 assert(GpuDmaSetRegions(&gpuRegion,1));
}
static u32 address(const void *pointer) {
 uintptr_t p=(uintptr_t)pointer,b;
 const struct {const void *p;u32 address,size;} regions[]={
  {world.streams,0x80210010,72},{world.vertices,0x80220010,128},
  {world.scratch.work7C.projection188.shadow.xy,0x1F80021C,16},
  {&world.scratch.work7C.depth70,0x1F8000EC,4},
  {&world.scratch.work7C.firstArea74,0x1F8000F0,4},
  {&world.scratch.work7C.secondArea78,0x1F8000F4,4}};
 for(unsigned i=0;i<sizeof(regions)/sizeof(regions[0]);i++){
  b=(uintptr_t)regions[i].p;if(p>=b&&p-b<regions[i].size)return regions[i].address+(u32)(p-b);
 }
 assert(!"unmapped or truncated native pointer");return 0;
}
static void event(u32 kind,u32 target,u32 value){assert(events++<64);*trace++=kind;*trace++=target;*trace++=value;}
static void load(unsigned reg,const void *p){u32 value;memcpy(&value,p,4);data[reg]=value;event(0x100+reg,address(p),value);}
static void store(unsigned reg,void *p){
 event(0x200+reg,address(p),data[reg]);memcpy(p,&data[reg],4);
 if(p==&world.scratch.work7C.depth70)expected.scratch.work7C.depth70=word(data[reg]);
 else if(p==&world.scratch.work7C.firstArea74)expected.scratch.work7C.firstArea74=word(data[reg]);
 else if(p==&world.scratch.work7C.secondArea78)expected.scratch.work7C.secondArea78=word(data[reg]);
 else {
  uintptr_t offset=(uintptr_t)p-(uintptr_t)world.scratch.work7C.projection188.shadow.xy;
  assert(offset<16);memcpy((u8*)expected.scratch.work7C.projection188.shadow.xy+offset,&data[reg],4);
 }
 if(reg==24&&clips==1){
  /* The fourth index is read after the first area store; other roots were
   * captured earlier, so replacing them must not redirect this projection. */
  unsigned stream=(scenario>>4)&1;
  world.streams[stream].decoded.quad.vertices.indices[3]=expected.streams[stream].decoded.quad.vertices.indices[3]=(scenario+5)&7;
  world.mesh.vertices=expected.mesh.vertices=world.vertices[((scenario>>5)&1)^1];
  world.mesh.commands=expected.mesh.commands=world.streams[stream^1].words;
 }
}
static void command(u32 code){
 event(0x600,0,code);
 switch(code){
 case 0x4A280030:
  data[12]=scenario*0x1234567U;data[13]=~(scenario*0x1234567U);data[14]=scenario*0x9E3779B9U;break;
 case 0x4A180001:data[14]=scenario*0x31415927U;break;
 case 0x4B68002E:data[7]=(u32)depths[scenario&15];break;
 case 0x4B400006:{
  s32 area=clips++?((scenario&4)?(s32)scenario*131:0):((scenario&3)?-(s32)(scenario*113):0);
  data[24]=(u32)area^(u32)world.scratch.work7C.windingBC;break;
 }
 default:assert(!"unexpected GTE command");
 }
}
#define RR_GTE_RTPT_LDV(a,b,c) do {load(0,a);load(1,(const u8*)(a)+4);load(2,b);load(3,(const u8*)(b)+4);load(4,c);load(5,(const u8*)(c)+4);command(0x4A280030);} while(0)
#define RR_GTE14_NOP2() ((void)0)
#define RR_GTE14_NCLIP() command(0x4B400006)
#define RR_GTE14_SWC2_SXYZ(p) do {store(12,p);store(13,(u8*)(p)+4);store(14,(u8*)(p)+8);} while(0)
#define RR_GTE14_SWC2_MAC0(p) store(24,p)
#define RR_GTE_RTPS_LDV(p) do {load(0,p);load(1,(const u8*)(p)+4);command(0x4A180001);} while(0)
#define RR_GTE14_AVSZ4() command(0x4B68002E)
#define RR_GTE14_SWC2_SXY2(p) store(14,p)
#define RR_GTE14_SWC2_OTZ(p) store(7,p)
'''
TAIL = r'''
void run_case(unsigned number,const u8 *input,u8 *output){
 installGpuMap();
 assert(sizeof(void*)==8&&(uintptr_t)&world>UINT32_MAX);
 scenario=number;events=clips=0;memset(&world,0xA5,sizeof(world));memset(data,0,sizeof(data));memset(output,0,OUTPUT_SIZE);trace=(u32*)(output+4);
 memcpy(world.streams,input,72);memcpy(world.vertices,input+72,128);
 u32 mesh[3];memcpy(mesh,input+200,12);
 world.mesh.vertices=world.vertices[(mesh[0]-0x80220010)/64];world.mesh.vertexCount=mesh[1];world.mesh.commands=world.streams[(mesh[2]-0x80210010)/36].words;
 memcpy(&world.scratch.work7C.depth70,input+212,12);memcpy(&world.scratch.work7C.windingBC,input+224,4);
 memcpy(&world.scratch.scene.depthShift60,input+228,4);memcpy(world.scratch.work7C.projection188.shadow.xy,input+232,16);
 world.scratch.scene.view.projection.pointers.cursor=(u8*)world.gpu;
 world.scratch.scene.view.projection.pointers.order=world.gpu;expected=world;
 EmitCarShadowQuads(&world.mesh,&world.scratch.work7C);
 assert(clips==2);
 s32 depth=depths[number&15];if(depth>0)depth>>=world.scratch.scene.depthShift60;
 expected.scratch.work7C.depth70=depth;assert(!memcmp(&world,&expected,sizeof(world)));
 memcpy(output,&events,4);u8 *p=output+4+64*12;
 memcpy(p,world.streams,72);p+=72;memcpy(p,world.vertices,128);p+=128;
 mesh[0]=address(world.mesh.vertices);mesh[1]=world.mesh.vertexCount;mesh[2]=address(world.mesh.commands);memcpy(p,mesh,12);p+=12;
 memcpy(p,&world.scratch.work7C.depth70,12);p+=12;memcpy(p,&world.scratch.work7C.windingBC,4);p+=4;
 memcpy(p,&world.scratch.scene.depthShift60,4);p+=4;memcpy(p,world.scratch.work7C.projection188.shadow.xy,16);p+=16;
 memcpy(p,data,128);
}
'''


def main():
    os.chdir(ROOT)
    owner = source_owners(ROOT/'src/main/PAL')['EmitCarShadowQuads']
    body = definition_source(owner.read_text(), 'EmitCarShadowQuads')
    body = re.sub(r'__asm__\("\$\d+"\)', '', body)
    with TemporaryDirectory() as directory:
        source, library = Path(directory)/'native.c', Path(directory)/'native.dylib'
        source.write_text(PREFIX+body+TAIL.replace('OUTPUT_SIZE', str(OUTPUT)))
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-Iinclude',
                        str(source), str(ROOT/'src/native/gpu_address.c'), '-o', str(library)], check=True)
        native = C.CDLL(str(library)); native.run_case.argtypes = [C.c_uint, C.c_void_p, C.c_void_p]
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0, 0x400000); memory.mem_map(0x1F800000, 4096)
        write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b))
        read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
        put = lambda a, v: write(a, struct.pack('<I', v & 0xFFFFFFFF))
        get = lambda a: int.from_bytes(read(a, 4), 'little')
        executable = (ROOT/'assets/PAL/main.exe').read_bytes(); base, = struct.unpack_from('<I', executable, 0x18)
        write(base, executable[0x800:]); start = address_of('EmitCarShadowQuads')
        if os.getenv('RR_COMPILED_ELF'):
            with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                elf = ELFFile(stream); symbol = elf.get_section_by_name('.symtab').get_symbol_by_name('EmitCarShadowQuads')[0]
                section = elf.get_section(symbol['st_shndx']); offset = symbol['st_value']-section['sh_addr']
                assert symbol['st_value'] == start; write(start, section.data()[offset:offset+symbol['st_size']])

        def check_store(cpu, access, address, size, value, user):
            assert any(a <= address and address+size <= a+n for a, n in
                       [(WORK+0x70, 12), (WORK+0x1A0, 16)]), (hex(address), size)

        cpu = Cpu(memory, 0, 100, 101, 102, check_store); paths = set()
        for case in range(CASES):
            state = bytearray((case*19+i*29)&255 for i in range(STATE))
            for stream in range(2):
                struct.pack_into('<I4H', state, stream*36, 0x10003, *[(case+stream+i)&7 for i in range(4)])
            struct.pack_into('<3I', state, 200, VERTICES+((case>>5)&1)*64, 8, STREAM+((case>>4)&1)*36)
            winding = 0x80000000 if case&64 else 0
            struct.pack_into('<II', state, 224, winding, (case>>7)&7)
            incoming = C.create_string_buffer(bytes(state)); output = (C.c_uint32*((OUTPUT+3)//4))()
            native.run_case(case, incoming, output); expected = bytes(output)[:OUTPUT]
            write(STREAM, state[:72]); write(VERTICES, state[72:200]); write(MESH, state[200:212])
            write(WORK+0x70, state[212:224]); write(WORK+0xBC, state[224:228]); write(0x1F800060, state[228:232]); write(WORK+0x1A0, state[232:248])
            cpu.reset(); cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
            cpu.r[4:6] = [MESH, WORK]; cpu.r[29] = STACK; cpu.r[31] = STOP; cpu.pc = start
            data, events, clips = [0]*32, [], 0
            for _ in range(1000):
                if cpu.pc == STOP: break
                instruction = get(cpu.pc); opcode = instruction >> 26
                if opcode not in (0x12, 0x32, 0x3A): cpu.step(); continue
                previous = cpu.delay; cpu.delay = None
                rs, rt = (instruction >> 21)&31, (instruction >> 16)&31
                if opcode in (0x32, 0x3A):
                    a = (cpu.r[rs]+signed(instruction, 16))&0xFFFFFFFF
                    if opcode == 0x32:
                        value = get(a); data[rt] = value; events.extend([0x100+rt, a, value])
                    else:
                        events.extend([0x200+rt, a, data[rt]]); cpu.store(a, data[rt], 4)
                        if rt == 24 and clips == 1:
                            write(STREAM+((case>>4)&1)*36+10, struct.pack('<H', (case+5)&7))
                            put(MESH, VERTICES+(((case>>5)&1)^1)*64); put(MESH+8, STREAM+(((case>>4)&1)^1)*36)
                else:
                    assert instruction & 0x02000000
                    events.extend([0x600, 0, instruction])
                    if instruction == 0x4A280030:
                        data[12]=(case*0x1234567)&0xFFFFFFFF; data[13]=~data[12]&0xFFFFFFFF; data[14]=(case*0x9E3779B9)&0xFFFFFFFF
                    elif instruction == 0x4A180001: data[14]=(case*0x31415927)&0xFFFFFFFF
                    elif instruction == 0x4B68002E: data[7]=DEPTHS[case&15]&0xFFFFFFFF
                    else:
                        assert instruction == 0x4B400006
                        area = (case*131 if case&4 else 0) if clips else (-(case*113) if case&3 else 0)
                        data[24]=(area&0xFFFFFFFF)^winding; clips += 1
                cpu.pc = previous if previous is not None else cpu.pc+4
            assert cpu.pc == STOP and clips == 2
            snapshot = read(STREAM, 72)+read(VERTICES, 128)+read(MESH, 12)+read(WORK+0x70, 12)+read(WORK+0xBC, 4)+read(0x1F800060, 4)+read(WORK+0x1A0, 16)
            actual = struct.pack('<I', len(events)//3)+struct.pack('<'+'I'*len(events), *events)+bytes(MAX_EVENTS*12-len(events)*4)+snapshot+struct.pack('<32I', *data)
            assert actual == expected, (case, 'projection/culling trace and complete owners')
            for reg in (*range(16, 24), 28, 30): assert cpu.r[reg] == 0xBEEF0000+reg
            assert cpu.r[29] == STACK and cpu.r[31] == STOP
            depth = DEPTHS[case&15]; shift = (case>>7)&7
            paths.add('nonpositive' if depth <= 0 else 'too_far' if depth >> shift >= 448 else 'backface')
            if 0 < depth and depth >> shift < 448:
                if not case&3: paths.add('zero_first_area')
                if not case&4: paths.add('zero_second_area')
        assert paths == {'nonpositive', 'too_far', 'backface', 'zero_first_area', 'zero_second_area'}
        print('PASS', CASES, 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
              'MIPS/native shadow projection/culling cases; full native command/vertex pointers, live fourth index, captured roots, exact GTE transfers, complete owners, ABI, UBSan; packet emission excluded')


if __name__ == '__main__':
    main()
