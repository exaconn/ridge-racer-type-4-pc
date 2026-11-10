"""Current track-object C vs original/compiled MIPS and a wire oracle.

Full native owners/pointers, all packet/head overlaps and pipeline prefetches.
GTE arithmetic is scripted; numerical accuracy, flags and timing are out of scope.
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
STREAM, VERTICES, MESH, WORK = 0x80210010, 0x80220010, 0x80230010, 0x1F80007C
STACK, STOP = 0x803F0000, 0x803FF000
CASES, PREFIX_SIZE, GPU_SIZE, MAX_EVENTS = 8192, 1468, 16384, 256
STATE = PREFIX_SIZE+GPU_SIZE
OUTPUT = 4+MAX_EVENTS*12+STATE+256
DEPTHS = (-2147483648, -1, 0, 1, 4, 1791, 1792, 2147483647)
UVS = (0, 1, 0x7FFFFFFF, 0x80000000, 0x8000FFFF, 0xFFFF0000, 0xFFFFFFFF, 0x12345678)
# Boundary/crossing rectangles, including quads which surround the viewport.
XY = ((0, 0), (-1, 20), (320, 20), (10, -1), (10, 256),
      (319, 255), (-32768, -32768), (32767, 32767), (-1, -1), (320, 256))
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "rr/track_object_render.h"
#include "rr/gpu.h"
typedef union {u32 words[57];struct {RroBodyCommandHeader header;TrackObjectQuad quads[7];} decoded;} Stream;
typedef union {u32 words[2048];GpuPolyFT4 packets[204];} GpuArena;
typedef struct {
 u8 before[16];TrackObjectScratch scratch;RroEffectMesh meshes[4];Stream streams[4];Vec3s8 vertices[2][16];
 _Alignas(16384) GpuArena gpu[2];u8 after[16];
} World;
static World world,expected;
#define SCRATCHPAD ((u8*)&world.scratch.scene)
static s32 ScreenFrame,FlashingLightColorTable[32];
static unsigned scenario,events,quad;static u32 data[32],control[32],*trace;
static const s32 depths[]={-2147483647-1,-1,0,1,4,1791,1792,2147483647};
static const s16 xy[][2]={{0,0},{-1,20},{320,20},{10,-1},{10,256},{319,255},{-32768,-32768},{32767,32767},{-1,-1},{320,256}};
u32 gpu_address(void){uintptr_t p=(uintptr_t)world.gpu;assert(p>UINT32_MAX&&(p&16383)==0&&(p&0xFFFFFFU)+sizeof(world.gpu)<=0x1000000U);return 0x81000000U|(u32)(p&0xFFFFFFU);}
static u32 address(const void *pointer){
 uintptr_t p=(uintptr_t)pointer,b=(uintptr_t)world.gpu;
 if(p>=b&&p-b<sizeof(world.gpu))return gpu_address()+(u32)(p-b);
 b=(uintptr_t)world.vertices;if(p>=b&&p-b<sizeof(world.vertices))return 0x80220010U+(u32)(p-b);
 b=(uintptr_t)&world.scratch.work7C;if(p>=b&&p-b<sizeof(world.scratch.work7C))return 0x1F80007CU+(u32)(p-b);
 b=(uintptr_t)world.meshes;if(p>=b&&p-b<sizeof(world.meshes))return 0x80230010U+(u32)((p-b)/sizeof(RroEffectMesh))*12U;
 assert(!"unmapped or truncated native pointer");return 0;
}
static void *gpuPointer(u32 p){u32 offset=p-gpu_address();assert(offset<sizeof(world.gpu));return (u8*)world.gpu+offset;}
static u32 get(const u8 **p){u32 v;memcpy(&v,*p,4);*p+=4;return v;}
static void put(u8 **p,u32 v){memcpy(*p,&v,4);*p+=4;}
static void loadState(World *w,const u8 *p,int initial){
 memcpy(w->streams,p,912);p+=912;memcpy(w->vertices,p,256);p+=256;
 for(unsigned i=0;i<4;i++){
  u32 v=get(&p),n=get(&p),s=get(&p);assert(v>=0x80220010U&&v<0x80220110U&&s>=0x80210010U&&s<0x802103A0U);
  w->meshes[i].vertices=(Vec3s8*)((u8*)world.vertices+v-0x80220010U);w->meshes[i].vertexCount=n;
  w->meshes[i].commands=(u32*)((u8*)world.streams+s-0x80210010U);
 }
 memcpy(&w->scratch.work7C,p,104);p+=104;
 w->scratch.scene.view.projection.pointers.cursor=gpuPointer(get(&p));w->scratch.scene.view.projection.pointers.order=gpuPointer(get(&p));
 u32 mesh=get(&p);assert(mesh>=0x80230010U&&mesh<0x80230040U&&(mesh-0x80230010U)%12==0);
 w->scratch.scene.modelEntries48=&world.meshes[(mesh-0x80230010U)/12];
 w->scratch.scene.depthShift60=(s32)get(&p);
 if(initial){ScreenFrame=(s32)get(&p);memcpy(FlashingLightColorTable,p,128);}else p+=4;
 p+=128;memcpy(w->gpu,p,16384);
}
static void snapshot(u8 *p){
 memcpy(p,world.streams,912);p+=912;memcpy(p,world.vertices,256);p+=256;
 for(unsigned i=0;i<4;i++){
  put(&p,address(world.meshes[i].vertices));put(&p,world.meshes[i].vertexCount);
  put(&p,0x80210010U+(u32)((const u8*)world.meshes[i].commands-(const u8*)world.streams));
 }
 memcpy(p,&world.scratch.work7C,104);p+=104;
 put(&p,address(world.scratch.scene.view.projection.pointers.cursor));put(&p,address(world.scratch.scene.view.projection.pointers.order));
 put(&p,address(world.scratch.scene.modelEntries48));put(&p,(u32)world.scratch.scene.depthShift60);put(&p,(u32)ScreenFrame);
 memcpy(p,FlashingLightColorTable,128);p+=128;memcpy(p,world.gpu,16384);
}
static void event(u32 kind,u32 target,u32 value){assert(events++<256);*trace++=kind;*trace++=target;*trace++=value;}
static void load(unsigned r,const void *p){u32 v;memcpy(&v,p,4);event(0x100+r,address(p),v);data[r]=v;}
static void store(unsigned r,void *p){event(0x200+r,address(p),data[r]);memcpy(p,&data[r],4);}
static u32 readData(unsigned r){event(0x300+r,0,data[r]);return data[r];}
static void writeControl(unsigned r,u32 v){event(0x500+r,0,v);control[r]=v;}
static u32 projected(unsigned vertex){unsigned mode=(scenario+quad)%10;if(mode>=8)mode=vertex&1?5:mode;return (u16)xy[mode][0]|((u32)(u16)xy[mode][1]<<16);}
static void command(u32 code){
 event(0x600,0,code);
 if(code==0x4A4A2012){
  quad=quad==~0U?0:quad+1;data[9]=data[0]^control[5];data[10]=data[1]^control[6];data[11]=(u32)depths[(scenario+quad*3)&7];
 }else if(code==0x4A280030){for(unsigned i=0;i<3;i++)data[12+i]=projected(i);}
 else {assert(code==0x4A180001);data[14]=projected(3);}
}
#define RR_GTE_MVMVA_LLM_BK_LDV0(p) do {load(0,p);load(1,(const u8*)(p)+4);command(0x4A4A2012);} while(0)
#define RR_GTE_READ_IR3(out) ((out)=(s32)readData(11))
#define RR_NOP_DEP_ON(x) ((void)(x))
#define RR_GTE_TR_FROM_IR() do {u32 a=readData(9),b=readData(10),c=readData(11);writeControl(5,a);writeControl(6,b);writeControl(7,c);} while(0)
#define RR_GTE_RTPT_LDV(a,b,c) do {load(0,(const void*)(a));load(1,(const u8*)(a)+4);load(2,(const void*)(b));load(3,(const u8*)(b)+4);load(4,(const void*)(c));load(5,(const u8*)(c)+4);command(0x4A280030);} while(0)
#define RR_GTE_RTPS_LDV(p) do {load(0,(const void*)(p));load(1,(const u8*)(p)+4);command(0x4A180001);} while(0)
#define RR_GTE_SWC2_SXYZ(p) do {store(12,(void*)(p));store(13,(u8*)(p)+4);store(14,(u8*)(p)+8);} while(0)
#define RR_GTE_SWC2_SXY2(p) store(14,(void*)(p))
'''
TAIL = r'''
void run_case(unsigned number,int flashing,const u8 *input,const u8 *wanted,u8 *output){
 assert(sizeof(long)==8&&sizeof(void*)==8&&(uintptr_t)&world>UINT32_MAX);
 scenario=number;events=0;quad=~0U;memset(&world,0xA5,sizeof(world));loadState(&world,input,1);expected=world;loadState(&expected,wanted,0);
 for(unsigned i=0;i<32;i++){data[i]=number*0x1234567U+i*0x17171717U;control[i]=~(number*0x31415927U+i*0x1234567U);}
 memset(output,0,OUTPUT_SIZE);trace=(u32*)(output+4);
 DrawTrackObjectModel((number>>3)&3,flashing);assert(!memcmp(&world,&expected,sizeof(world)));
 memcpy(output,&events,4);u8 *p=output+4+256*12;snapshot(p);p+=STATE_SIZE;memcpy(p,data,128);memcpy(p+128,control,128);
}
'''


def oracle(case, flashing, incoming, gpu):
    state = bytearray(incoming)
    events = []
    data = [(case*0x1234567+r*0x17171717)&0xFFFFFFFF for r in range(32)]
    control = [~(case*0x31415927+r*0x1234567)&0xFFFFFFFF for r in range(32)]
    get = lambda at: struct.unpack_from('<I', state, at)[0]
    def put(at, v): struct.pack_into('<I', state, at, v&0xFFFFFFFF)
    def event(kind, address, value): events.extend((kind, address, value&0xFFFFFFFF))
    def load(r, address, at): data[r] = get(at); event(0x100+r, address, data[r])
    def store(r, address, at): event(0x200+r, address, data[r]); put(at, data[r])
    def command(code): event(0x600, 0, code)
    def rd(r): event(0x300+r, 0, data[r]); return data[r]
    mesh = get(1328)-MESH+(case>>3&3)*12+1168
    vertices, _, stream = struct.unpack_from('<3I', state, mesh)
    start = stream-STREAM
    count = struct.unpack_from('<H', state, start+2)[0]
    packet, ordering = get(1320), get(1324)
    shift = get(1332)+2
    put(1216+84, flashing)
    first_head = None
    paths, emitted_biases, emitted_uvs = set(), set(), set()
    def stage(at):
        put(1216, get(at+28))
        state[1220:1222] = state[at+26:at+28]
    stage(start+4)
    for i in range(count):
        at = start+4+i*32
        load(0, WORK, 1216); load(1, WORK+4, 1220); command(0x4A4A2012)
        data[9] = data[0]^control[5]; data[10] = data[1]^control[6]; data[11] = DEPTHS[(case+i*3)&7]&0xFFFFFFFF
        stage(at+32)
        depth = signed(rd(11), 32)
        if depth <= 0: paths.add('nonpositive'); continue
        depth >>= shift
        if depth >= 448: paths.add('far'); continue
        put(1296, depth)
        ir = [rd(r) for r in (9, 10, 11)]
        for r, value in zip((5, 6, 7), ir): control[r] = value; event(0x500+r, 0, value)
        indices = struct.unpack_from('<4H', state, at)
        for r, index in enumerate(indices[:3]):
            load(r*2, vertices+index*8, 912+vertices-VERTICES+index*8)
            load(r*2+1, vertices+index*8+4, 916+vertices-VERTICES+index*8)
        command(0x4A280030)
        mode = (case+i)%10
        xy = [XY[(5 if v&1 else mode) if mode>=8 else mode] for v in range(4)]
        packed = [(x&65535)|((y&65535)<<16) for x, y in xy]
        data[12:15] = packed[:3]
        target = PREFIX_SIZE+packet-gpu
        for k in range(3): put(target+12+k*8, get(at+12+k*4))
        put(target+36, signed(get(at+20), 32)>>16)
        for k in range(3): store(12+k, WORK+88+k*4, 1304+k*4)
        load(0, vertices+indices[3]*8, 912+vertices-VERTICES+indices[3]*8)
        load(1, vertices+indices[3]*8+4, 916+vertices-VERTICES+indices[3]*8)
        command(0x4A180001); data[14] = packed[3]
        phase = signed(state[at+24], 8)
        color_index = (get(1336)+phase)&31 if flashing else 0
        put(target+4, get(1340+color_index*4))
        for k in range(3): put(target+8+k*8, get(1304+k*4))
        store(14, WORK+100, 1316); put(target+32, get(1316))
        if not any(x >= 0 for x, _ in xy) or not any(x < 320 for x, _ in xy): paths.add('clip_x'); continue
        if not any(y >= 0 for _, y in xy) or not any(y < 256 for _, y in xy): paths.add('clip_y'); continue
        bias = signed(state[at+25], 8)
        head = ordering+512+4*(depth+bias)
        if first_head is None: first_head = head
        head_at = PREFIX_SIZE+head-gpu
        put(target, get(head_at)); state[target+3] = 9
        put(head_at, packet); packet += 40; state[head_at+3] = 0
        paths.add('emitted'); emitted_biases.add(bias)
        emitted_uvs.update(get(at+12+k*4) for k in range(3))
    put(1320, packet)
    output = struct.pack('<I', len(events)//3)+struct.pack('<'+'I'*len(events), *events)+bytes(MAX_EVENTS*12-len(events)*4)+state+struct.pack('<64I', *data, *control)
    return bytes(state), output, (first_head, paths, count, emitted_biases, emitted_uvs)


def main():
    os.chdir(ROOT)
    owner = source_owners(ROOT/'src/main/PAL')['DrawTrackObjectModel']
    body = re.sub(r'__asm__\("\$\d+"\)', '', definition_source(owner.read_text(), 'DrawTrackObjectModel'))
    with TemporaryDirectory() as directory:
        source, library = Path(directory)/'native.c', Path(directory)/'native.dylib'
        source.write_text(PREFIX+body+TAIL.replace('OUTPUT_SIZE', str(OUTPUT)).replace('STATE_SIZE', str(STATE)))
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-Iinclude', str(source), '-o', str(library)], check=True)
        native = C.CDLL(str(library)); native.run_case.argtypes = [C.c_uint, C.c_int, C.c_void_p, C.c_void_p, C.c_void_p]
        native.gpu_address.restype = C.c_uint32; gpu = native.gpu_address()
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0, 0x400000); memory.mem_map(0x1000000, 0x1000000); memory.mem_map(0x1F800000, 4096)
        write = lambda a, b: memory.mem_write(a&0x1FFFFFFF, bytes(b))
        read = lambda a, n: bytes(memory.mem_read(a&0x1FFFFFFF, n))
        put = lambda a, v: write(a, struct.pack('<I', v&0xFFFFFFFF))
        get = lambda a: int.from_bytes(read(a, 4), 'little')
        executable = (ROOT/'assets/PAL/main.exe').read_bytes(); base, = struct.unpack_from('<I', executable, 0x18)
        write(base, executable[0x800:]); start = address_of('DrawTrackObjectModel')
        if os.getenv('RR_COMPILED_ELF'):
            with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                elf = ELFFile(stream); symbol = elf.get_section_by_name('.symtab').get_symbol_by_name('DrawTrackObjectModel')[0]
                section = elf.get_section(symbol['st_shndx']); offset = symbol['st_value']-section['sh_addr']
                assert symbol['st_value'] == start; write(start, section.data()[offset:offset+symbol['st_size']])
        def check_store(cpu, access, address, size, value, user):
            address &= 0x1FFFFFFF
            assert any((a&0x1FFFFFFF) <= address and address+size <= (a&0x1FFFFFFF)+n for a, n in
                       [(WORK, 6), (WORK+80, 24), (0x1F800000, 4), (gpu, GPU_SIZE), (STACK-256, 256)]), (hex(address), size)
        cpu = Cpu(memory, 0, 100, 101, 102, check_store)
        paths, aliases, biases, shifts, counts, uv_words = set(), set(), set(), set(), set(), set()
        frame_address, colors_address = address_of('ScreenFrame'), address_of('FlashingLightColorTable')
        for case in range(CASES):
            state = bytearray((case*19+i*29)&255 for i in range(STATE))
            for stream in range(4):
                count = (case+stream)%7
                struct.pack_into('<I', state, stream*228, 3|(count<<16))
                for i in range(7):
                    at = stream*228+4+i*32
                    struct.pack_into('<4H', state, at, *[(case+stream+i+k)&15 for k in range(4)])
                    struct.pack_into('<3I', state, at+12, *[UVS[((case>>4)+i+k)&7] for k in range(3)])
                    state[at+24] = (case*3+i*5)&255; state[at+25] = ((case>>3)+i*37)&255
                struct.pack_into('<3I', state, 1168+stream*12, VERTICES+(stream&1)*128, 16, STREAM+stream*228)
            order_bank = (case>>6)&1; ordering = gpu+order_bank*8192+128*4; packet = gpu+(order_bank^1)*8192+900*4
            shift = (case>>5)%30; flashing = (0, 1, -1, -2147483648)[case&3]
            frame = (0, 1, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF)[(case>>2)%5]
            struct.pack_into('<5I', state, 1320, packet, ordering, MESH, shift, frame)
            struct.pack_into('<4096I', state, PREFIX_SIZE, *[(case*0x31415927+i*0x17171717)&0xFFFFFFFF for i in range(4096)])
            if case&4096:
                _, _, (head, _, _, _, _) = oracle(case, flashing, state, gpu)
                if head is not None: packet = head-((case>>4)%10)*4; struct.pack_into('<I', state, 1320, packet)
            reference, expected, (head, case_paths, count, emitted_biases, emitted_uvs) = oracle(case, flashing, state, gpu)
            incoming, wanted = C.create_string_buffer(bytes(state)), C.create_string_buffer(reference)
            output = (C.c_uint32*((OUTPUT+3)//4))(); native.run_case(case, flashing, incoming, wanted, output)
            assert bytes(output)[:OUTPUT] == expected, (case, 'native/oracle transfer trace and complete owners')
            write(STREAM, state[:912]); write(VERTICES, state[912:1168]); write(MESH, state[1168:1216]); write(WORK, state[1216:1320])
            write(0x1F800000, state[1320:1328]); write(0x1F800048, state[1328:1332]); write(0x1F800060, state[1332:1336])
            write(frame_address, state[1336:1340]); write(colors_address, state[1340:1468]); write(gpu, state[PREFIX_SIZE:])
            cpu.reset(); cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]; cpu.r[4:6] = [(case>>3)&3, flashing&0xFFFFFFFF]; cpu.r[29] = STACK; cpu.r[31] = STOP; cpu.pc = start
            data = [(case*0x1234567+r*0x17171717)&0xFFFFFFFF for r in range(32)]
            control = [~(case*0x31415927+r*0x1234567)&0xFFFFFFFF for r in range(32)]; events = []; quad = -1
            for _ in range(10000):
                if cpu.pc == STOP: break
                instruction = get(cpu.pc); opcode = instruction>>26
                if opcode not in (0x12, 0x32, 0x3A): cpu.step(); continue
                previous = cpu.delay; cpu.delay = None; rs, rt, rd = (instruction>>21)&31, (instruction>>16)&31, (instruction>>11)&31
                if opcode in (0x32, 0x3A):
                    a = (cpu.r[rs]+signed(instruction, 16))&0xFFFFFFFF
                    if opcode == 0x32: data[rt] = get(a); events.extend([0x100+rt, a, data[rt]])
                    else: events.extend([0x200+rt, a, data[rt]]); cpu.store(a, data[rt], 4)
                elif instruction&0x02000000:
                    events.extend([0x600, 0, instruction])
                    if instruction == 0x4A4A2012:
                        quad += 1; data[9] = data[0]^control[5]; data[10] = data[1]^control[6]; data[11] = DEPTHS[(case+quad*3)&7]&0xFFFFFFFF
                    else:
                        mode = (case+quad)%10
                        xy = [XY[(5 if v&1 else mode) if mode>=8 else mode] for v in range(4)]
                        packed = [(x&65535)|((y&65535)<<16) for x, y in xy]
                        if instruction == 0x4A280030: data[12:15] = packed[:3]
                        else: assert instruction == 0x4A180001; data[14] = packed[3]
                elif rs == 0: cpu.r[rt] = data[rd]; events.extend([0x300+rd, 0, data[rd]])
                else: assert rs == 6; control[rd] = cpu.r[rt]; events.extend([0x500+rd, 0, control[rd]])
                cpu.pc = previous if previous is not None else cpu.pc+4
            assert cpu.pc == STOP and quad+1 == count
            snapshot = read(STREAM, 912)+read(VERTICES, 256)+read(MESH, 48)+read(WORK, 104)+read(0x1F800000, 8)+read(0x1F800048, 4)+read(0x1F800060, 4)+read(frame_address, 4)+read(colors_address, 128)+read(gpu, GPU_SIZE)
            actual = struct.pack('<I', len(events)//3)+struct.pack('<'+'I'*len(events), *events)+bytes(MAX_EVENTS*12-len(events)*4)+snapshot+struct.pack('<64I', *data, *control)
            assert actual == expected, (case, 'MIPS/oracle wire packets, aliases, prefetch, complete owners and GTE transfers')
            for r in (*range(16, 24), 28, 30): assert cpu.r[r] == 0xBEEF0000+r
            assert cpu.r[29] == STACK and cpu.r[31] == STOP
            paths |= case_paths; counts.add(count); shifts.add(shift)
            if head is not None:
                aliases.add((case>>4)%10 if case&4096 else 'separate')
                biases |= emitted_biases; uv_words |= emitted_uvs
        assert paths == {'emitted', 'nonpositive', 'far', 'clip_x', 'clip_y'}, paths
        assert aliases == set(range(10))|{'separate'} and shifts == set(range(30)) and counts == set(range(7))
        assert biases == set(range(-128, 128)) and uv_words == set(UVS)
        print('PASS', CASES, 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
              'MIPS/native track-object cases; full owners/pointers, 0..6 quads, final prefetch, depth/clipping, signed phases/biases, wrapped frames, all packet/head aliases, partial culled packets, ABI, UBSan; scripted GTE math')


if __name__ == '__main__':
    main()
