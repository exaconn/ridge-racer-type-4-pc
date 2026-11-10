"""Actual lit caller and triangle emitter vs original/compiled MIPS + wire oracle.

RotMatrixZ and GTE arithmetic are scripted. Both rendering bodies run unchanged
apart from CPU register annotations and named hardware adapters on the host.
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
MESH, GEO, PLACEMENTS, VERTICES, WORK = 0x80200010, 0x80210010, 0x80211010, 0x80220010, 0x1F80007C
STACK, STOP = 0x803F0000, 0x803FF000
CASES, PREFIX_SIZE, GPU_SIZE, MAX_EVENTS = 8192, 1212, 16384, 512
STATE = PREFIX_SIZE+GPU_SIZE
SERVICE = PREFIX_SIZE+256
OUTPUT = 8+MAX_EVENTS*12+SERVICE+STATE+256
DEPTHS = (-2147483648, -1, 0, 1, 148, 149, 297, 298)
XY = ((0, 0), (-1, 20), (320, 20), (10, -1), (10, 256),
      (319, 255), (-32768, -32768), (32767, 32767), (-1, -1), (320, 256))
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "game/gfx/tilerect.h"
#include "game/gfx/gtemodel.h"
#include "psyq/libgte/rotmatz.h"
#include "rr/gpu.h"
typedef union {u32 words[31];struct {u32 header;TrackObjectTriangle triangles[6];} decoded;} Geometry;
typedef union {u32 words[13];struct {u32 header;TrackObjectPlacement placements[6];} decoded;} Placements;
typedef union {u32 words[2048];GpuPolyG3 triangles[292];GpuCommandPacket commands[1024];} GpuArena;
typedef struct {
 u8 before[16];TrackObjectLitScratch scratch;RroEffectMesh meshes[2][4];Geometry geometry[4];Placements placements[4];Vec3s8 vertices[2][16];
 MatrixWords32 camera;_Alignas(16384) GpuArena gpu[2];u8 after[16];
} World;
static World world,expected;
#define SCRATCHPAD ((u8*)&world.scratch.scene)
#define CameraViewMatrix world.camera
#define D_800FF6A4 world.camera.fields.translation14
static unsigned scenario,events,services,instance,triangle;static u32 data[32],control[32],*trace;static u8 *serviceTrace;
static const s32 depths[]={-2147483647-1,-1,0,1,148,149,297,298};
static const s16 xy[][2]={{0,0},{-1,20},{320,20},{10,-1},{10,256},{319,255},{-32768,-32768},{32767,32767},{-1,-1},{320,256}};
u32 gpu_address(void){uintptr_t p=(uintptr_t)world.gpu;assert(p>UINT32_MAX&&(p&16383)==0&&(p&0xFFFFFFU)+sizeof(world.gpu)<=0x1000000U);return 0x81000000U|(u32)(p&0xFFFFFFU);}
static u32 address(const void *pointer){
 uintptr_t p=(uintptr_t)pointer,b=(uintptr_t)world.gpu;
 if(p>=b&&p-b<sizeof(world.gpu))return gpu_address()+(u32)(p-b);
 b=(uintptr_t)world.vertices;if(p>=b&&p-b<sizeof(world.vertices))return 0x80220010U+(u32)(p-b);
 b=(uintptr_t)world.geometry;if(p>=b&&p-b<sizeof(world.geometry))return 0x80210010U+(u32)(p-b);
 b=(uintptr_t)world.placements;if(p>=b&&p-b<sizeof(world.placements))return 0x80211010U+(u32)(p-b);
 b=(uintptr_t)world.meshes;if(p>=b&&p-b<sizeof(world.meshes)){assert((p-b)%sizeof(RroEffectMesh)==0);return 0x80200010U+(u32)((p-b)/sizeof(RroEffectMesh))*12U;}
 b=(uintptr_t)&world.scratch.work7C.rotation00;if(p>=b&&p-b<32)return 0x1F80007CU+(u32)(p-b);
 b=(uintptr_t)&world.scratch.work7C.input24;if(p>=b&&p-b<8)return 0x1F8000A0U+(u32)(p-b);
 if(pointer==&world.scratch.work7C.depth54)return 0x1F8000D0U;
 assert(!"unmapped or truncated native pointer");return 0;
}
static void *resolve(u32 a){
 u32 offset=a-gpu_address();if(offset<sizeof(world.gpu))return (u8*)world.gpu+offset;
 offset=a-0x80220010U;if(offset<sizeof(world.vertices))return (u8*)world.vertices+offset;
 offset=a-0x80210010U;if(offset<sizeof(world.geometry))return (u8*)world.geometry+offset;
 offset=a-0x80211010U;if(offset<sizeof(world.placements))return (u8*)world.placements+offset;
 offset=a-0x80200010U;if(offset<96&&offset%12==0)return (u8*)world.meshes+(offset/12)*sizeof(RroEffectMesh);
 assert(!"invalid wire pointer");return 0;
}
static u32 get(const u8 **p){u32 v;memcpy(&v,*p,4);*p+=4;return v;}
static void put(u8 **p,u32 v){memcpy(*p,&v,4);*p+=4;}
static void loadState(World *w,const u8 *p){
 for(unsigned b=0;b<2;b++)for(unsigned i=0;i<4;i++){w->meshes[b][i].vertices=resolve(get(&p));w->meshes[b][i].vertexCount=get(&p);w->meshes[b][i].commands=resolve(get(&p));}
 memcpy(w->geometry,p,496);p+=496;memcpy(w->placements,p,208);p+=208;memcpy(w->vertices,p,256);p+=256;
 TrackObjectLitWork *work=&w->scratch.work7C;
 memcpy(&work->rotation00,p,32);p+=32;work->reserved20=get(&p);memcpy(&work->input24,p,8);p+=8;
 work->vertices2C=resolve(get(&p));work->commands30=resolve(get(&p));memcpy(work->reserved34,p,32);p+=32;work->depth54=(s32)get(&p);
 w->scratch.scene.view.projection.pointers.cursor=resolve(get(&p));w->scratch.scene.view.projection.pointers.order=resolve(get(&p));w->scratch.scene.modelEntries48=resolve(get(&p));
 w->scratch.scene.depthShift60=(s32)get(&p);w->scratch.scene.renderFlags64=(s32)get(&p);w->scratch.scene.view.projection.pose08.angles10.xyz.z=(s32)get(&p);
 memcpy(&w->camera,p,32);p+=32;memcpy(w->camera.fields.translation14,p,12);p+=12;memcpy(w->gpu,p,16384);
}
static void snapshot(u8 *p,int gpu){
 for(unsigned b=0;b<2;b++)for(unsigned i=0;i<4;i++){put(&p,address(world.meshes[b][i].vertices));put(&p,world.meshes[b][i].vertexCount);put(&p,address(world.meshes[b][i].commands));}
 memcpy(p,world.geometry,496);p+=496;memcpy(p,world.placements,208);p+=208;memcpy(p,world.vertices,256);p+=256;
 TrackObjectLitWork *work=&world.scratch.work7C;
 memcpy(p,&work->rotation00,32);p+=32;put(&p,work->reserved20);memcpy(p,&work->input24,8);p+=8;
 put(&p,address(work->vertices2C));put(&p,address(work->commands30));memcpy(p,work->reserved34,32);p+=32;put(&p,(u32)work->depth54);
 put(&p,address(world.scratch.scene.view.projection.pointers.cursor));put(&p,address(world.scratch.scene.view.projection.pointers.order));put(&p,address(world.scratch.scene.modelEntries48));
 put(&p,(u32)world.scratch.scene.depthShift60);put(&p,(u32)world.scratch.scene.renderFlags64);put(&p,(u32)world.scratch.scene.view.projection.pose08.angles10.xyz.z);
 memcpy(p,&world.camera,32);p+=32;memcpy(p,world.camera.fields.translation14,12);p+=12;if(gpu)memcpy(p,world.gpu,16384);
}
static void event(u32 kind,u32 target,u32 value){assert(events++<512);*trace++=kind;*trace++=target;*trace++=value;}
MatrixWords32 *RotMatrixZ(s32 angle,MatrixWords32 *matrix){
 assert(matrix==&world.scratch.work7C.rotation00&&services++==0);event(0x700,address(matrix),(u32)angle);
 snapshot(serviceTrace,0);memcpy(serviceTrace+1212,data,128);memcpy(serviceTrace+1340,control,128);
 for(unsigned i=0;i<5;i++)matrix->w[i]=(s32)((u32)matrix->w[i]^(scenario*0x17171717U+i*0x1234567U+(u32)angle));
 unsigned bank=(scenario>>2)&1;world.scratch.scene.modelEntries48=world.meshes[bank^1];
 uintptr_t cursor=(u8*)world.scratch.scene.view.projection.pointers.cursor-(u8*)world.gpu;
 uintptr_t ordering=(u8*)world.scratch.scene.view.projection.pointers.order-(u8*)world.gpu;
 world.scratch.scene.view.projection.pointers.cursor=(u8*)world.gpu+(cursor^8192U);
 world.scratch.scene.view.projection.pointers.order=(u32*)((u8*)world.gpu+(ordering^8192U));
 world.scratch.scene.depthShift60=(world.scratch.scene.depthShift60+7)&31;
 world.scratch.scene.renderFlags64=!world.scratch.scene.renderFlags64;
 world.scratch.scene.view.projection.pose08.angles10.xyz.z=~angle;
 world.scratch.work7C.input24.fields.pad06=(s16)(scenario*131U);
 for(unsigned i=0;i<8;i++)world.camera.w[i]=(s32)(scenario*0x31415927U+i*0x17171717U);
 for(unsigned i=0;i<3;i++)world.camera.fields.translation14[i]=(s32)(scenario*0x1234567U+i*0x31415927U);
 u32 *header=(u32*)world.scratch.work7C.commands30;*header=(*header&65535U)|(((scenario>>4)%6)<<16);
 return matrix;
}
static void load(unsigned r,const void *p){u32 v;memcpy(&v,p,4);event(0x100+r,address(p),v);data[r]=v;}
static void store(unsigned r,void *p){event(0x200+r,address(p),data[r]);memcpy(p,&data[r],4);}
static u32 readData(unsigned r){event(0x300+r,0,data[r]);return data[r];}
static void writeControl(unsigned r,u32 v){event(0x500+r,0,v);control[r]=v;}
static void matrixControl(const void *p,unsigned start,unsigned count){u32 v;for(unsigned i=0;i<count;i++){memcpy(&v,(const u8*)p+i*4,4);writeControl(start+i,v);}}
static void command(u32 code){
 event(0x600,0,code);
 if(code==0x4A4A2012){instance=instance==~0U?0:instance+1;triangle=0;data[9]=data[0]^control[5];data[10]=data[1]^control[6];data[11]=(u32)depths[(scenario+instance*3)&7];}
 else {
  assert(code==0x4A280030);unsigned mode=(scenario+instance*5+triangle++)%10;
  for(unsigned i=0;i<3;i++){unsigned k=mode>=8?(i&1?5:mode):mode;data[12+i]=(u16)xy[k][0]|((u32)(u16)xy[k][1]<<16);}
 }
}
#define RR_GTE_SETROT(p) matrixControl(p,0,5)
#define RR_GTE_SETLLM(p) matrixControl(p,8,5)
#define RR_GTE_SETBK_P(p) matrixControl(p,13,3)
#define RR_GTE_MVMVA_LLM_BK_LDV0(p) do {load(0,p);load(1,(const u8*)(p)+4);command(0x4A4A2012);} while(0)
#define RR_GTE_SWC2_IR3(p) store(11,p)
#define RR_GTE_TR_FROM_IR() do {u32 a=readData(9),b=readData(10),c=readData(11);writeControl(5,a);writeControl(6,b);writeControl(7,c);} while(0)
#define RR_GTE_RTPT_LDV(a,b,c) do {load(0,a);load(1,(const u8*)(a)+4);load(2,b);load(3,(const u8*)(b)+4);load(4,c);load(5,(const u8*)(c)+4);command(0x4A280030);} while(0)
#define RR_GTE_STORE_READ_SXY3(p,x,y,z) do {store(12,(u8*)(p)+8);store(13,(u8*)(p)+16);store(14,(u8*)(p)+24);(x)=(s32)readData(12);(y)=(s32)readData(13);(z)=(s32)readData(14);} while(0)
'''
TAIL = r'''
void run_case(unsigned number,const u8 *input,const u8 *wanted,u8 *output){
 assert(sizeof(long)==8&&sizeof(void*)==8&&(uintptr_t)&world>UINT32_MAX);
 scenario=number;events=services=0;instance=~0U;triangle=0;memset(&world,0xA5,sizeof(world));loadState(&world,input);expected=world;loadState(&expected,wanted);
 for(unsigned i=0;i<32;i++){data[i]=number*0x1234567U+i*0x17171717U;control[i]=~(number*0x31415927U+i*0x1234567U);}
 memset(output,0,OUTPUT_SIZE);trace=(u32*)(output+8);serviceTrace=output+8+512*12;
 DrawTrackObjectLitModel(number&3,(number>>3)&3);assert(services==1&&!memcmp(&world,&expected,sizeof(world)));
 memcpy(output,&events,4);memcpy(output+4,&services,4);u8 *p=serviceTrace+SERVICE_SIZE;snapshot(p,1);p+=STATE_SIZE;memcpy(p,data,128);memcpy(p+128,control,128);
}
'''


def oracle(case, incoming, gpu):
    state = bytearray(incoming)
    events = []
    data = [(case*0x1234567+r*0x17171717)&0xFFFFFFFF for r in range(32)]
    control = [~(case*0x31415927+r*0x1234567)&0xFFFFFFFF for r in range(32)]
    get = lambda at: struct.unpack_from('<I', state, at)[0]
    def put(at, v): struct.pack_into('<I', state, at, v&0xFFFFFFFF)
    def event(kind, address, value): events.extend((kind, address, value&0xFFFFFFFF))
    def load(r, address, at): data[r] = get(at); event(0x100+r, address, data[r])
    def store(r, address, at): event(0x200+r, address, data[r]); put(at, data[r])
    def rd(r): event(0x300+r, 0, data[r]); return data[r]
    def ctrl(r, v): control[r] = v&0xFFFFFFFF; event(0x500+r, 0, v)
    mesh = get(1152)-MESH+((case>>3)&3)*12
    vertices, _, geometry = struct.unpack_from('<3I', state, mesh)
    put(1100, vertices); put(1104, geometry)
    for i, value in ((0, 4096), (4, 4096), (2, 4096), (3, 0), (1, 0)): put(1056+i*4, value)
    angle = get(1164); event(0x700, WORK, angle)
    service = bytes(state[:PREFIX_SIZE])+struct.pack('<64I', *data, *control)
    for i in range(5): put(1056+i*4, get(1056+i*4)^(case*0x17171717+i*0x1234567+angle)&0xFFFFFFFF)
    put(1152, MESH+(((case>>2)&1)^1)*48)
    put(1144, gpu+((get(1144)-gpu)^8192)); put(1148, gpu+((get(1148)-gpu)^8192))
    put(1156, (get(1156)+7)&31); put(1160, int(get(1160)==0)); put(1164, ~angle)
    struct.pack_into('<H', state, 1098, (case*131)&65535)
    for i in range(8): put(1168+i*4, case*0x31415927+i*0x17171717)
    for i in range(3):
        value = case*0x1234567+i*0x31415927
        put(1200+i*4, value); put(1188+i*4, value)
    geo_at = 96+geometry-GEO; triangle_count = (case>>4)%6
    put(geo_at, (get(geo_at)&65535)|(triangle_count<<16))
    for start, at, count in ((0, 1056, 5), (8, 1168, 5), (13, 1200, 3)):
        for i in range(count): ctrl(start+i, get(at+i*4))
    stream = get(get(1152)-MESH+(case&3)*12+8)
    placement_at = 592+stream-PLACEMENTS
    count = struct.unpack_from('<H', state, placement_at+2)[0]
    packet, ordering, shift = get(1144), get(1148), get(1156)
    limit = 149 if get(1160) else 298
    paths, biases, tri_aliases, mode_aliases, counts = set(), set(), set(), set(), set()
    first_head = None
    def stage(at):
        put(1092, get(at)); state[1096:1098] = state[at+4:at+6]
    def link(head, length):
        nonlocal packet
        at, head_at = PREFIX_SIZE+packet-gpu, PREFIX_SIZE+head-gpu
        if packet <= head < packet+(28 if length==6 else 8):
            (tri_aliases if length==6 else mode_aliases).add((head-packet)//4)
        put(at, get(head_at)); state[at+3] = length; put(head_at, packet)
        packet += 28 if length==6 else 8; state[head_at+3] = 0
    stage(placement_at+4)
    for instance in range(count):
        at = placement_at+4+instance*8
        load(0, WORK+36, 1092); load(1, WORK+40, 1096); event(0x600, 0, 0x4A4A2012)
        data[9] = data[0]^control[5]; data[10] = data[1]^control[6]; data[11] = DEPTHS[(case+instance*3)&7]&0xFFFFFFFF
        stage(at+8); store(11, WORK+84, 1140)
        depth = signed(data[11], 32)
        if depth <= 0: paths.add('nonpositive'); continue
        depth >>= shift
        if depth >= limit: paths.add('far'); continue
        bias = signed(state[at+7], 8); biases.add(bias)
        ir = [rd(r) for r in (9, 10, 11)]
        for r, value in zip((5, 6, 7), ir): ctrl(r, value)
        head = ordering+512+4*(depth+bias)
        if first_head is None: first_head = head
        counts.add(triangle_count)
        for triangle in range(max(1, triangle_count)):
            record = geo_at+4+triangle*20
            indices = struct.unpack_from('<3H', state, record)
            for i, index in enumerate(indices):
                load(i*2, vertices+index*8, 800+vertices-VERTICES+index*8)
                load(i*2+1, vertices+index*8+4, 804+vertices-VERTICES+index*8)
            event(0x600, 0, 0x4A280030)
            mode = (case+instance*5+triangle)%10
            xy = [XY[(5 if i&1 else mode) if mode>=8 else mode] for i in range(3)]
            data[12:15] = [(x&65535)|((y&65535)<<16) for x, y in xy]
            target = PREFIX_SIZE+packet-gpu
            for i in range(3): put(target+4+i*8, get(record+8+i*4))
            for i in range(3): store(12+i, packet+8+i*8, target+8+i*8)
            for i in range(3): rd(12+i)
            if not any(y >= 0 for _, y in xy) or not any(y < 256 for _, y in xy): paths.add('clip_y'); continue
            if not any(x >= 0 for x, _ in xy) or not any(x < 320 for x, _ in xy): paths.add('clip_x'); continue
            link(head, 6); paths.add('triangle')
        put(PREFIX_SIZE+packet-gpu+4, 0xE1000220); link(head, 1); paths.add('draw_mode')
    put(1144, packet)
    output = struct.pack('<II', len(events)//3, 1)+struct.pack('<'+'I'*len(events), *events)+bytes(MAX_EVENTS*12-len(events)*4)+service+state+struct.pack('<64I', *data, *control)
    meta = dict(head=first_head, paths=paths, biases=biases, tri_aliases=tri_aliases, mode_aliases=mode_aliases, triangle_counts=counts, placement_count=count, shift=shift)
    return bytes(state), output, meta


def main():
    os.chdir(ROOT)
    owners = source_owners(ROOT/'src/main/PAL')
    bodies = [re.sub(r'__asm__\("\$\d+"\)', '', definition_source(owners[n].read_text(), n)) for n in ('EmitModelTrisGT3', 'DrawTrackObjectLitModel')]
    with TemporaryDirectory() as directory:
        source, library = Path(directory)/'native.c', Path(directory)/'native.dylib'
        source.write_text(PREFIX+'\n'.join(bodies)+TAIL.replace('OUTPUT_SIZE', str(OUTPUT)).replace('SERVICE_SIZE', str(SERVICE)).replace('STATE_SIZE', str(STATE)))
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-Iinclude', str(source), '-o', str(library)], check=True)
        native = C.CDLL(str(library)); native.run_case.argtypes = [C.c_uint, C.c_void_p, C.c_void_p, C.c_void_p]
        native.gpu_address.restype = C.c_uint32; gpu = native.gpu_address()
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0, 0x400000); memory.mem_map(0x1000000, 0x1000000); memory.mem_map(0x1F800000, 4096)
        write = lambda a, b: memory.mem_write(a&0x1FFFFFFF, bytes(b))
        read = lambda a, n: bytes(memory.mem_read(a&0x1FFFFFFF, n))
        put = lambda a, v: write(a, struct.pack('<I', v&0xFFFFFFFF))
        get = lambda a: int.from_bytes(read(a, 4), 'little')
        executable = (ROOT/'assets/PAL/main.exe').read_bytes(); base, = struct.unpack_from('<I', executable, 0x18)
        write(base, executable[0x800:]); start = address_of('DrawTrackObjectLitModel'); rotate = address_of('RotMatrixZ')
        compiled = [os.getenv('RR_COMPILED_GTEMODEL_ELF'), os.getenv('RR_COMPILED_TILERECT_ELF')]
        assert all(compiled) or not any(compiled)
        for path, name in zip(compiled, ('DrawTrackObjectLitModel', 'EmitModelTrisGT3')):
            if path:
                with open(path, 'rb') as stream:
                    elf = ELFFile(stream); symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(name)[0]
                    section = elf.get_section(symbol['st_shndx']); offset = symbol['st_value']-section['sh_addr']
                    assert symbol['st_value'] == address_of(name); write(symbol['st_value'], section.data()[offset:offset+symbol['st_size']])
        def check_store(cpu, access, address, size, value, user):
            address &= 0x1FFFFFFF
            assert any((a&0x1FFFFFFF) <= address and address+size <= (a&0x1FFFFFFF)+n for a, n in
                       [(WORK, 88), (0x1F800000, 4), (gpu, GPU_SIZE), (STACK-512, 512)]), (hex(address), size)
        cpu = Cpu(memory, 0, 100, 101, 102, check_store)
        camera_address, background_address = address_of('CameraViewMatrix'), address_of('D_800FF6A4')
        paths, biases, tri_aliases, mode_aliases, shifts, placement_counts, triangle_counts = (set() for _ in range(7))
        for case in range(CASES):
            state = bytearray((case*19+i*29)&255 for i in range(STATE)); bank = (case>>2)&1
            for b in range(2):
                for i in range(4): struct.pack_into('<3I', state, (b*4+i)*12, VERTICES+((b+i)&1)*128, 16, (GEO+i*124) if b==bank else (PLACEMENTS+i*52))
            for stream in range(4):
                struct.pack_into('<I', state, 96+stream*124, 2|(((case+stream)%6)<<16))
                for i in range(6): struct.pack_into('<3H', state, 100+stream*124+i*20, *[(case+stream+i+k)&15 for k in range(3)])
                struct.pack_into('<I', state, 592+stream*52, 7|(((case+stream)%5)<<16))
                for i in range(6): state[596+stream*52+i*8+7] = ((case>>3)+i*37)&255
            state[1200:1212] = state[1188:1200]
            struct.pack_into('<II', state, 1100, VERTICES, GEO)
            order_bank = (case>>6)&1; packet = gpu+(order_bank^1)*8192+900*4; ordering = gpu+order_bank*8192+128*4
            angle = (0, 1, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF)[(case>>2)%5]
            struct.pack_into('<6I', state, 1144, packet, ordering, MESH+bank*48, (case>>5)&31, case&1, angle)
            struct.pack_into('<4096I', state, PREFIX_SIZE, *[(case*0x31415927+i*0x17171717)&0xFFFFFFFF for i in range(4096)])
            if case&4096:
                _, _, meta = oracle(case, state, gpu)
                if meta['head'] is not None:
                    wanted_packet = meta['head']-((case>>4)%7)*4
                    struct.pack_into('<I', state, 1144, gpu+((wanted_packet-gpu)^8192))
            reference, expected, meta = oracle(case, state, gpu)
            incoming, wanted = C.create_string_buffer(bytes(state)), C.create_string_buffer(reference)
            output = (C.c_uint32*((OUTPUT+3)//4))(); native.run_case(case, incoming, wanted, output)
            assert bytes(output)[:OUTPUT] == expected, (case, 'native/oracle services, GTE transfers and complete owners')
            write(MESH, state[:96]); write(GEO, state[96:592]); write(PLACEMENTS, state[592:800]); write(VERTICES, state[800:1056]); write(WORK, state[1056:1144])
            write(0x1F800000, state[1144:1152]); write(0x1F800048, state[1152:1156]); write(0x1F800060, state[1156:1164]); write(0x1F800020, state[1164:1168]); write(camera_address, state[1168:1200]); write(background_address, state[1200:1212]); write(gpu, state[PREFIX_SIZE:])
            def snapshot(gpu_bytes=False):
                return read(MESH, 96)+read(GEO, 496)+read(PLACEMENTS, 208)+read(VERTICES, 256)+read(WORK, 88)+read(0x1F800000, 8)+read(0x1F800048, 4)+read(0x1F800060, 8)+read(0x1F800020, 4)+read(camera_address, 32)+read(background_address, 12)+(read(gpu, GPU_SIZE) if gpu_bytes else b'')
            cpu.reset(); cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]; cpu.r[4:6] = [case&3, (case>>3)&3]; cpu.r[29] = STACK; cpu.r[31] = STOP; cpu.pc = start
            data = [(case*0x1234567+r*0x17171717)&0xFFFFFFFF for r in range(32)]
            control = [~(case*0x31415927+r*0x1234567)&0xFFFFFFFF for r in range(32)]; events = []; services = []; instance, triangle = -1, 0
            for _ in range(20000):
                if cpu.pc == STOP: break
                if cpu.pc == rotate:
                    assert cpu.r[5] == WORK and not services
                    angle = cpu.r[4]; events.extend([0x700, WORK, angle]); services.append(snapshot()+struct.pack('<64I', *data, *control))
                    for i in range(5): put(WORK+i*4, get(WORK+i*4)^((case*0x17171717+i*0x1234567+angle)&0xFFFFFFFF))
                    put(0x1F800048, MESH+(bank^1)*48); put(0x1F800000, gpu+((get(0x1F800000)-gpu)^8192)); put(0x1F800004, gpu+((get(0x1F800004)-gpu)^8192))
                    put(0x1F800060, (get(0x1F800060)+7)&31); put(0x1F800064, int(get(0x1F800064)==0)); put(0x1F800020, ~angle); write(WORK+42, struct.pack('<H', (case*131)&65535))
                    for i in range(8): put(camera_address+i*4, case*0x31415927+i*0x17171717)
                    for i in range(3): put(background_address+i*4, case*0x1234567+i*0x31415927)
                    header = get(WORK+48); put(header, (get(header)&65535)|(((case>>4)%6)<<16))
                    resume = cpu.r[31]
                    for r in (*range(2, 16), 24, 25): cpu.r[r] = 0xDEAD0000+r
                    cpu.r[2] = WORK; cpu.pc = resume; continue
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
                        instance += 1; triangle = 0; data[9] = data[0]^control[5]; data[10] = data[1]^control[6]; data[11] = DEPTHS[(case+instance*3)&7]&0xFFFFFFFF
                    else:
                        assert instruction == 0x4A280030
                        mode = (case+instance*5+triangle)%10; triangle += 1
                        xy = [XY[(5 if i&1 else mode) if mode>=8 else mode] for i in range(3)]
                        data[12:15] = [(x&65535)|((y&65535)<<16) for x, y in xy]
                elif rs == 0: cpu.r[rt] = data[rd]; events.extend([0x300+rd, 0, data[rd]])
                else: assert rs == 6; control[rd] = cpu.r[rt]; events.extend([0x500+rd, 0, control[rd]])
                cpu.pc = previous if previous is not None else cpu.pc+4
            assert cpu.pc == STOP and len(services) == 1 and instance+1 == meta['placement_count']
            actual = struct.pack('<II', len(events)//3, 1)+struct.pack('<'+'I'*len(events), *events)+bytes(MAX_EVENTS*12-len(events)*4)+services[0]+snapshot(True)+struct.pack('<64I', *data, *control)
            assert actual == expected, (case, 'MIPS/oracle service entry, cache lifetimes, wire packets, aliases, prefetch and complete owners')
            for r in (*range(16, 24), 28, 30): assert cpu.r[r] == 0xBEEF0000+r
            assert cpu.r[29] == STACK and cpu.r[31] == STOP
            paths |= meta['paths']; biases |= meta['biases']; tri_aliases |= meta['tri_aliases']; mode_aliases |= meta['mode_aliases']; shifts.add(meta['shift']); placement_counts.add(meta['placement_count']); triangle_counts |= meta['triangle_counts']
        assert paths == {'nonpositive', 'far', 'clip_x', 'clip_y', 'triangle', 'draw_mode'}, paths
        assert tri_aliases == set(range(7)) and mode_aliases == {0, 1}, (tri_aliases, mode_aliases)
        assert biases == set(range(-128, 128)) and shifts == set(range(32)) and placement_counts == set(range(5)) and triangle_counts == set(range(6))
        print('PASS', CASES, 'compiled' if any(compiled) else 'original',
              'MIPS/native lit caller+emitter cases; full owners/pointers, mutable rotation service, captured geometry/live placements, pipeline prefetch, zero-count do-loop, all triangle/draw-mode head aliases, partial culled packets, ABI, UBSan; scripted matrix/GTE math')


if __name__ == '__main__':
    main()
