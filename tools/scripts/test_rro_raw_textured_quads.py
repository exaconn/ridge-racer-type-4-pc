"""Current raw-textured FT4 emitter vs original/compiled MIPS and a wire oracle.

Actual owners use full native pointers. GTE projection/depth/area are scripted;
store hooks distinguish prefetched vertex words from live commands and roots.
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
STREAM, VERTICES, WORK = 0x80200010, 0x80210010, 0x1F80007C
STACK, STOP = 0x803F0000, 0x803FF000
CASES, PREFIX_SIZE, GPU_SIZE, MAX_EVENTS = 8192 + 256, 528, 16384, 384
STATE = PREFIX_SIZE + GPU_SIZE
OUTPUT = 12 + MAX_EVENTS * 12 + STATE + 128
COUNTS = (-32768, -1, 0, 1, 2, 3, 4, 5, 6)
DEPTHS = (-2147483648, -1, 0, 1, 447, 448, 1791, 1792)
AREAS = (0, 1, -1, -2147483648, 2147483647)
UVS = (0, 1, 0x7FFFFFFF, 0x80000000, 0x8000FFFF, 0xFFFF0000, 0xFFFFFFFF, 0x12345678)
XY = ((0, 0), (-1, 20), (320, 20), (10, -1), (10, 512),
      (319, 511), (-32768, -32768), (32767, 32767), (-1, -1), (320, 512))
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "rr/car_render_types.h"
#include "rr/rro_model.h"
#include "rr/gpu_address.h"
typedef union {u32 words[42];RroRawTexturedQuad quads[7];} QuadStream;
typedef union {u32 words[2048];GpuPolyFT4 packets[204];} GpuArena;
static struct World {
 u8 before[16];RroModelScratch scratch;QuadStream stream;Vec3s8 vertices[2][16];
 _Alignas(16384) GpuArena gpu[2];u8 after[16];
} world,expected;
#define SCRATCHPAD ((u8*)&world.scratch)
#define WORK world.scratch.work7C.render
#define SCENE world.scratch.scene
static unsigned scenario,biasSeed,events,hooks,q,clips,geometryBank;static u32 data[32],*trace;
static const s32 depths[]={-2147483647-1,-1,0,1,447,448,1791,1792};
static const s32 areas[]={0,1,-1,-2147483647-1,2147483647};
static const s16 xy[][2]={{0,0},{-1,20},{320,20},{10,-1},{10,512},{319,511},{-32768,-32768},{32767,32767},{-1,-1},{320,512}};
static GpuDmaRegion gpuRegion;
u32 gpu_address(void){
 assert((uintptr_t)world.gpu>UINT32_MAX);
 if(!gpuRegion.native){
  gpuRegion=(GpuDmaRegion){world.gpu,0x80180000U,sizeof(world.gpu)};
  assert(GpuDmaSetRegions(&gpuRegion,1));
 }
 return gpuRegion.address;
}
static u32 address(const void *pointer){
 uintptr_t p=(uintptr_t)pointer,b=(uintptr_t)world.gpu;
 if(p>=b&&p-b<sizeof(world.gpu))return gpu_address()+(u32)(p-b);
 b=(uintptr_t)world.vertices;if(p>=b&&p-b<sizeof(world.vertices))return 0x80210010U+(u32)(p-b);
 b=(uintptr_t)&world.stream;if(p>=b&&p-b<sizeof(world.stream))return 0x80200010U+(u32)(p-b);
 b=(uintptr_t)&WORK.depth18;if(p>=b&&p-b<72)return 0x1F800094U+(u32)(p-b);
 assert(!"unmapped or truncated native pointer");return 0;
}
static void *resolve(u32 a){
 u32 offset=a-gpu_address();if(offset<sizeof(world.gpu))return (u8*)world.gpu+offset;
 offset=a-0x80210010U;if(offset<sizeof(world.vertices))return (u8*)world.vertices+offset;
 assert(!"invalid wire pointer");return 0;
}
static u32 get(const u8 *p){u32 v;memcpy(&v,p,4);return v;}
static void put(u8 *p,u32 v){memcpy(p,&v,4);}
static void loadState(struct World *w,const u8 *p){
 memcpy(&w->stream,p,168);memcpy(w->vertices,p+168,256);p+=424;
 RroRenderWork *work=&w->scratch.work7C.render;
 work->vertices00=resolve(get(p));work->normals04=resolve(get(p+4));
 memcpy(&work->flags08,p+8,12);work->ordering14=resolve(get(p+20));
 memcpy(&work->depth18,p+24,16);memcpy(work->vertices28,p+40,32);memcpy(&work->normal48,p+72,8);memcpy(work->projected50,p+80,16);
 w->scratch.scene.view.projection.pointers.cursor=resolve(get(p+96));w->scratch.scene.depthShift60=(s32)get(p+100);memcpy(w->gpu,p+104,16384);
}
static void snapshot(u8 *p){
 memcpy(p,&world.stream,168);memcpy(p+168,world.vertices,256);p+=424;
 put(p,address(WORK.vertices00));put(p+4,address(WORK.normals04));memcpy(p+8,&WORK.flags08,12);put(p+20,address(WORK.ordering14));
 memcpy(p+24,&WORK.depth18,16);memcpy(p+40,WORK.vertices28,32);memcpy(p+72,&WORK.normal48,8);memcpy(p+80,WORK.projected50,16);
 put(p+96,address(SCENE.view.projection.pointers.cursor));put(p+100,(u32)SCENE.depthShift60);memcpy(p+104,world.gpu,16384);
}
static void mutate(unsigned phase){
 hooks++;u32 mask=scenario*0x31415927U+q*0x17171717U+phase;
 RroRawTexturedQuad *current=&world.stream.quads[q];
 current->uv08=(s32)((u32)current->uv08^mask);current->uv0C=(s32)((u32)current->uv0C^mask);current->uv10=(s32)((u32)current->uv10^mask);
 current->orderingBias15=(s8)(u8)(biasSeed+q*37+phase);
 WORK.ordering14=&world.gpu[(scenario+q+phase)&1].words[512+phase*64+q*16];
 if(phase==1){
  for(unsigned i=0;i<4;i++)current[1].vertices.indices[i]=(current[1].vertices.indices[i]+3+q)&15;
  for(unsigned i=0;i<16;i++)for(unsigned j=0;j<2;j++)world.vertices[geometryBank][i].words[j]^=mask+i*0x1234567U+j;
  WORK.vertices00=world.vertices[geometryBank^1];WORK.flags08=(s32)((u32)WORK.flags08^mask);
  SCENE.view.projection.pointers.cursor=(u8*)&world.gpu[(scenario+q)&1].words[1400+q*20];SCENE.depthShift60=(scenario+q+11)&31;
 }
}
static void event(u32 kind,u32 target,u32 value){assert(events++<384);*trace++=kind;*trace++=target;*trace++=value;}
static void load(unsigned r,const void *p){u32 v;memcpy(&v,p,4);event(0x100+r,address(p),v);data[r]=v;}
static void store(unsigned r,void *p){u32 a=address(p);event(0x200+r,a,data[r]);memcpy(p,&data[r],4);if(a==0x1F800098U)mutate(1);if(a==0x1F80009CU)mutate(2);}
static u32 projected(unsigned vertex){
 unsigned mode=(scenario+q)%12;
 if(mode==10)mode=vertex&1?9:8;
 if(mode==11)mode=vertex&1?7:0;
 return (u16)xy[mode][0]|((u32)(u16)xy[mode][1]<<16);
}
static void command(u32 code){
 event(0x600,0,code);
 if(code==0x4A280030){q=q==~0U?0:q+1;clips=0;for(unsigned i=0;i<3;i++)data[12+i]=projected(i);}
 else if(code==0x4A180001)data[14]=projected(3);
 else if(code==0x4B400006)data[24]=(u32)areas[(scenario+q*3+clips++*2)%5];
 else{assert(code==0x4B68002E);data[7]=(u32)depths[(scenario+q*5)&7];}
}
#define RR_GTE14_LWC2_V012(p) do{for(unsigned i=0;i<6;i++)load(i,(const u8*)(p)+i*4);}while(0)
#define RR_GTE14_LWC2_V0(p) do{load(0,p);load(1,(const u8*)(p)+4);}while(0)
#define RR_GTE14_NOP2() ((void)0)
#define RR_GTE14_RTPT() command(0x4A280030)
#define RR_GTE14_RTPS() command(0x4A180001)
#define RR_GTE14_NCLIP() command(0x4B400006)
#define RR_GTE14_AVSZ4() command(0x4B68002E)
#define RR_GTE14_SWC2_MAC0(p) store(24,(void*)(p))
#define RR_GTE14_SWC2_OTZ(p) store(7,p)
#define RR_GTE14_SWC2_SXYZ(p) do{store(12,p);store(13,(u8*)(p)+4);store(14,(u8*)(p)+8);}while(0)
#define RR_GTE14_SWC2_SXY2(p) store(14,p)
'''
TAIL = r'''
void run_case(unsigned number,unsigned bias,s32 count,const u8 *input,const u8 *wanted,u8 *output){
 assert(sizeof(long)==8&&sizeof(void*)==8&&(uintptr_t)&world>UINT32_MAX);
 assert((u8*)&WORK.projected50-(u8*)&WORK.depth18==56);
 scenario=number;biasSeed=bias;geometryBank=(scenario>>1)&1;events=hooks=0;q=~0U;
 memset(&world,0xA5,sizeof(world));loadState(&world,input);expected=world;loadState(&expected,wanted);
 for(unsigned i=0;i<32;i++)data[i]=number*0x1234567U+i*0x17171717U;
 memset(output,0,OUTPUT_SIZE);trace=(u32*)(output+12);
 const u32 *result=EmitRroRawTexturedQuads(world.stream.words,&WORK,count);
 assert(!memcmp(&world,&expected,sizeof(world)));
 put(output,address(result));put(output+4,events);put(output+8,hooks);
 snapshot(output+12+384*12);memcpy(output+12+384*12+STATE_SIZE,data,128);
}
'''


def mutate(case, q, phase, state, gpu, bias_seed):
    get = lambda at: struct.unpack_from('<I', state, at)[0]
    def put(at, value): struct.pack_into('<I', state, at, value & 0xFFFFFFFF)
    mask = (case * 0x31415927 + q * 0x17171717 + phase) & 0xFFFFFFFF
    at = q * 24
    for i in range(3): put(at + 8 + i * 4, get(at + 8 + i * 4) ^ mask)
    state[at + 21] = (bias_seed + q * 37 + phase) & 255
    put(444, gpu + ((case + q + phase) & 1) * 8192 + (512 + phase * 64 + q * 16) * 4)
    if phase == 1:
        for i in range(4):
            index, = struct.unpack_from('<H', state, at + 24 + i * 2)
            struct.pack_into('<H', state, at + 24 + i * 2, (index + 3 + q) & 15)
        bank = (case >> 1) & 1
        for i in range(16):
            for j in range(2):
                at = 168 + bank * 128 + i * 8 + j * 4
                put(at, get(at) ^ ((mask + i * 0x1234567 + j) & 0xFFFFFFFF))
        put(424, VERTICES + (bank ^ 1) * 128)
        put(432, get(432) ^ mask)
        put(520, gpu + ((case + q) & 1) * 8192 + (1400 + q * 20) * 4)
        put(524, (case + q + 11) & 31)


def projected(case, q):
    mode = (case + q) % 12
    corners = [XY[(9 if i & 1 else 8) if mode == 10 else ((7 if i & 1 else 0) if mode == 11 else mode)] for i in range(4)]
    return corners, [(x & 65535) | ((y & 65535) << 16) for x, y in corners]


def oracle(case, count, incoming, gpu):
    bias_seed = case >> 2 if case < 8192 else case - 8192
    case = case if case < 8192 else 60
    state = bytearray(incoming); events = []; hooks = 0
    data = [(case * 0x1234567 + i * 0x17171717) & 0xFFFFFFFF for i in range(32)]
    get = lambda at: struct.unpack_from('<I', state, at)[0]
    def put(at, value): struct.pack_into('<I', state, at, value & 0xFFFFFFFF)
    def event(kind, target, value): events.extend((kind, target, value & 0xFFFFFFFF))
    def load(r, at): data[r] = get(at); event(0x100 + r, WORK + at - 424, data[r])
    def store(r, at): event(0x200 + r, WORK + at - 424, data[r]); put(at, data[r])
    def command(code): event(0x600, 0, code)
    vertices = 168 + get(424) - VERTICES
    packet, shift = get(520), get(524)
    for i, index in enumerate(struct.unpack_from('<4H', state, 0)):
        state[464 + i * 8:472 + i * 8] = state[vertices + index * 8:vertices + index * 8 + 8]
    first_head = None; paths, aliases, biases = set(), set(), set()
    if count <= 0: paths.add('empty')
    for q in range(max(count, 0)):
        for i in range(6): load(i, 464 + i * 4)
        command(0x4A280030)
        corners, xy = projected(case, q)
        data[12:15] = xy[:3]
        indices = struct.unpack_from('<4H', state, (q + 1) * 24)
        def vertex(i):
            at = vertices + indices[i] * 8
            return bytes(state[at:at + 8])
        state[464:472] = vertex(0)
        command(0x4B400006); data[24] = AREAS[(case + q * 3) % 5] & 0xFFFFFFFF
        second = vertex(1)
        store(24, 452); mutate(case, q, 1, state, gpu, bias_seed); hooks += 1
        for i in range(3): store(12 + i, 504 + i * 4)
        load(0, 488); load(1, 492)
        state[472:480] = second
        command(0x4A180001); data[14] = xy[3]
        state[480:488] = vertex(2)
        command(0x4B68002E); data[7] = DEPTHS[(case + q * 5) & 7] & 0xFFFFFFFF
        fourth = vertex(3)
        store(7, 448); state[488:496] = fourth
        depth = signed(get(448), 32)
        if depth <= 0: paths.add('nonpositive'); continue
        depth >>= shift; put(448, depth)
        if depth >= 448: paths.add('far'); continue
        command(0x4B400006); data[24] = AREAS[(case + q * 3 + 2) % 5] & 0xFFFFFFFF
        target = PREFIX_SIZE + packet - gpu
        for i in range(3): put(target + 8 + i * 8, get(504 + i * 4))
        store(24, 456); mutate(case, q, 2, state, gpu, bias_seed); hooks += 1
        if signed(get(452), 32) <= 0 and signed(get(456), 32) >= 0: paths.add('winding'); continue
        store(14, 516); put(target + 32, get(516))
        if not any(x >= 0 for x, _ in corners) or not any(x < 320 for x, _ in corners): paths.add('clip_x'); continue
        if not any(y >= 0 for _, y in corners) or not any(y < 512 for _, y in corners): paths.add('clip_y'); continue
        put(target + 4, 0x2F000000)
        values = [get(q * 24 + 8 + i * 4) for i in range(3)]
        for i, value in enumerate(values): put(target + 12 + i * 8, value)
        put(target + 36, signed(values[2], 32) >> 16)
        bias = signed(state[q * 24 + 21], 8)
        head = get(444) + (depth + bias) * 4
        if first_head is None: first_head = head
        if packet <= head < packet + 40: aliases.add((head - packet) // 4)
        head_at = PREFIX_SIZE + head - gpu
        put(target, get(head_at)); state[target + 3] = 9
        put(head_at, packet); packet += 40; state[head_at + 3] = 0
        paths.add('emitted'); biases.add(bias)
    put(520, packet)
    result = STREAM + max(count, 0) * 24
    output = struct.pack('<3I', result, len(events) // 3, hooks) + struct.pack('<' + 'I' * len(events), *events) + bytes(MAX_EVENTS * 12 - len(events) * 4) + state + struct.pack('<32I', *data)
    return bytes(state), output, dict(head=first_head, paths=paths, aliases=aliases, biases=biases, hooks=hooks)


def main():
    os.chdir(ROOT); name = 'EmitRroRawTexturedQuads'
    owner = source_owners(ROOT / 'src/main/PAL')[name]
    body = re.sub(r'(?:__asm__|asm)\("\$\d+"\)', '', definition_source(owner.read_text(), name))
    with TemporaryDirectory() as directory:
        source, library = Path(directory) / 'native.c', Path(directory) / 'native.dylib'
        source.write_text(PREFIX + body + TAIL.replace('OUTPUT_SIZE', str(OUTPUT)).replace('STATE_SIZE', str(STATE)))
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-Iinclude', str(source), str(ROOT / 'src/native/gpu_address.c'), '-o', str(library)], check=True)
        native = C.CDLL(str(library)); native.run_case.argtypes = [C.c_uint, C.c_uint, C.c_int32, C.c_void_p, C.c_void_p, C.c_void_p]
        native.gpu_address.restype = C.c_uint32; gpu = native.gpu_address()
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0, 0x400000); memory.mem_map(0x1000000, 0x1000000); memory.mem_map(0x1F800000, 4096)
        write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b)); read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
        put = lambda a, v: write(a, struct.pack('<I', v & 0xFFFFFFFF)); get = lambda a: int.from_bytes(read(a, 4), 'little')
        executable = (ROOT / 'assets/PAL/main.exe').read_bytes(); base, = struct.unpack_from('<I', executable, 0x18); write(base, executable[0x800:])
        if os.getenv('RR_COMPILED_ELF'):
            with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                elf = ELFFile(stream); symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(name)[0]
                section = elf.get_section(symbol['st_shndx']); offset = symbol['st_value'] - section['sh_addr']
                assert symbol['st_value'] == address_of(name); write(symbol['st_value'], section.data()[offset:offset + symbol['st_size']])
        def check_store(cpu, access, address, size, value, user):
            address &= 0x1FFFFFFF
            assert any((a & 0x1FFFFFFF) <= address and address + size <= (a & 0x1FFFFFFF) + n for a, n in [(WORK, 96), (0x1F800000, 4), (gpu, GPU_SIZE), (STACK - 256, 256)]), (hex(address), size)
        cpu = Cpu(memory, 0, 100, 101, 102, check_store)
        def snapshot():
            return read(STREAM, 168) + read(VERTICES, 256) + read(WORK, 96) + read(0x1F800000, 4) + read(0x1F800060, 4) + read(gpu, GPU_SIZE)
        paths, aliases, biases, shifts, counts = (set() for _ in range(5))
        for case in range(CASES):
            # Retain the original matrix, then independently sweep every bias
            # through an emitting flow (60) so clipping cannot hide a value.
            flow = case if case < 8192 else 60
            bias_seed = case >> 2 if case < 8192 else case - 8192
            state = bytearray((case * 19 + i * 29) & 255 for i in range(STATE)); bank = (flow >> 1) & 1
            for q in range(7):
                struct.pack_into('<4H', state, q * 24, *[(case + q + i) & 15 for i in range(4)])
                struct.pack_into('<3I', state, q * 24 + 8, *[UVS[((case >> 4) + q + i) & 7] for i in range(3)])
            count = COUNTS[flow % len(COUNTS)]; shift = (flow >> 5) & 31
            struct.pack_into('<2I', state, 424, VERTICES + bank * 128, VERTICES + (bank ^ 1) * 128)
            struct.pack_into('<I', state, 444, gpu + ((case >> 2) & 1) * 8192 + 512 * 4)
            struct.pack_into('<2I', state, 520, gpu + ((case >> 2) & 1) * 8192 + 900 * 4, shift)
            struct.pack_into('<4096I', state, PREFIX_SIZE, *[(case * 0x31415927 + i * 0x17171717) & 0xFFFFFFFF for i in range(4096)])
            if case & 2048:
                _, _, meta = oracle(case, count, state, gpu)
                if meta['head'] is not None: struct.pack_into('<I', state, 520, meta['head'] - ((case >> 3) % 10) * 4)
            reference, expected, meta = oracle(case, count, state, gpu)
            incoming, wanted = C.create_string_buffer(bytes(state)), C.create_string_buffer(reference); output = C.create_string_buffer(OUTPUT)
            native.run_case(flow, bias_seed, count, incoming, wanted, output)
            assert output.raw == expected, (case, 'native/wire oracle; full owners, prefetch and live fields')
            write(STREAM, state[:168]); write(VERTICES, state[168:424]); write(WORK, state[424:520]); write(0x1F800000, state[520:524]); write(0x1F800060, state[524:528]); write(gpu, state[PREFIX_SIZE:])
            cpu.reset(); cpu.r = [0] + [0xBEEF0000 + r for r in range(1, 32)]; cpu.r[4:7] = [STREAM, WORK, count & 0xFFFFFFFF]; cpu.r[29] = STACK; cpu.r[31] = STOP; cpu.pc = address_of(name)
            data = [(flow * 0x1234567 + i * 0x17171717) & 0xFFFFFFFF for i in range(32)]; events = []; q = -1; clips = hooks = 0
            for _ in range(10000):
                if cpu.pc == STOP: break
                instruction = get(cpu.pc); opcode = instruction >> 26
                if opcode not in (0x12, 0x32, 0x3A): cpu.step(); continue
                previous = cpu.delay; cpu.delay = None; rs, rt = (instruction >> 21) & 31, (instruction >> 16) & 31
                if opcode in (0x32, 0x3A):
                    a = (cpu.r[rs] + signed(instruction, 16)) & 0xFFFFFFFF
                    if opcode == 0x32: data[rt] = get(a); events.extend([0x100 + rt, a, data[rt]])
                    else:
                        events.extend([0x200 + rt, a, data[rt]]); cpu.store(a, data[rt], 4)
                        if a in (WORK + 28, WORK + 32):
                            hooked = bytearray(snapshot()); mutate(flow, q, 1 if a == WORK + 28 else 2, hooked, gpu, bias_seed); hooks += 1
                            write(STREAM, hooked[:168]); write(VERTICES, hooked[168:424]); write(WORK, hooked[424:520]); write(0x1F800000, hooked[520:524]); write(0x1F800060, hooked[524:528])
                else:
                    assert instruction & 0x02000000; events.extend([0x600, 0, instruction])
                    if instruction == 0x4A280030:
                        q += 1; clips = 0; _, xy = projected(flow, q); data[12:15] = xy[:3]
                    elif instruction == 0x4A180001: data[14] = xy[3]
                    elif instruction == 0x4B400006: data[24] = AREAS[(flow + q * 3 + clips * 2) % 5] & 0xFFFFFFFF; clips += 1
                    else: assert instruction == 0x4B68002E; data[7] = DEPTHS[(flow + q * 5) & 7] & 0xFFFFFFFF
                cpu.pc = previous if previous is not None else cpu.pc + 4
            assert cpu.pc == STOP and q + 1 == max(count, 0) and hooks == meta['hooks']
            actual = struct.pack('<3I', cpu.r[2], len(events) // 3, hooks) + struct.pack('<' + 'I' * len(events), *events) + bytes(MAX_EVENTS * 12 - len(events) * 4) + snapshot() + struct.pack('<32I', *data)
            assert actual == expected, (case, 'MIPS/wire oracle; exact transfers, prefetch, packet/head overlap, ABI')
            for r in (*range(16, 24), 28, 30): assert cpu.r[r] == 0xBEEF0000 + r
            assert cpu.r[29] == STACK and cpu.r[31] == STOP
            paths |= meta['paths']; aliases |= meta['aliases']; biases |= meta['biases']; shifts.add(shift); counts.add(count)
        assert paths == {'empty', 'nonpositive', 'far', 'winding', 'clip_x', 'clip_y', 'emitted'}, paths
        assert aliases == set(range(10)) and shifts == set(range(32)) and counts == set(COUNTS), (aliases, shifts, counts)
        assert biases == set(range(-128, 128)), len(biases)
        print('PASS', CASES, 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original', name,
              'MIPS/native cases; real owners/full pointers, captured roots, partial prefetch, live UV/bias/ordering, '
              'all packet/head aliases, signed counts, clipping/depth/winding, exact GTE transfers, ABI, UBSan; scripted GTE math', flush=True)


if __name__ == '__main__':
    main()
