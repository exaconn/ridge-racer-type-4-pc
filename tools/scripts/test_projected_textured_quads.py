"""Actual projected FT4 emitter, native owners and original/linked MIPS.

NCLIP results are scripted. This checks C staging, live fields, wrapping depth
sums, DMA links and aliases; it does not implement numerical GTE clipping.
"""
import ctypes as C
from pathlib import Path
import struct
import subprocess
from tempfile import TemporaryDirectory

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN

from c_functions import address_of, definition_source, source_owners
from mips_preview_engine import signed
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
NAME = 'EmitProjectedQuadsFT4'
SCRATCH, WORK, STREAM = 0x1F800000, 0x1F80007C, 0x80200010
PROJECTED, GPU, STACK, STOP = WORK+0x188, 0x80180000, 0x803F0000, 0x803FF000
STATE, MAX_EVENTS, CASES = 8888, 64, 8192+256+20
OUTPUT = 8+MAX_EVENTS*12+STATE+128
COUNTS = (-32768, -1, 0, 1, 2, 3, 4, 5, 6)
AREAS = (0, 1, -1, -2147483648, 2147483647)
DEPTHS = (-2147483648, 2147483647, -1, 0, 1, 447, 448, 1792)
UVS = (0, 1, 0x7FFFFFFF, 0x80000000, 0x8000FFFF, 0xFFFF0000, 0xFFFFFFFF, 0x12345678)
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "rr/rro_effects.h"
#include "rr/gpu_address.h"
typedef union {u32 words[42];RroTexturedQuadPrefix quads[6];} Stream;
typedef union {u32 words[1024];GpuPolyFT4 packets[102];} GpuArena;
static struct World {
 u8 before[16];RroEffectScratch scratch;Stream stream;
 _Alignas(8192) GpuArena gpu[2];u8 after[16];
} world,expected;
#define SCRATCHPAD ((u8*)&world.scratch)
#define WORK world.scratch.work7C
#define SCENE world.scratch.scene
static unsigned scenario,biasSeed,events,q;static u32 data[32],*trace;
static GpuDmaRegion region;
static u32 get(const void *p){u32 v;memcpy(&v,p,4);return v;}
static void put(void *p,u32 v){memcpy(p,&v,4);}
static s32 word(u32 v){s32 x;memcpy(&x,&v,4);return x;}
static void *resolve(u32 a){u32 d=a-0x80180000U;assert(d<sizeof(world.gpu));return (u8*)world.gpu+d;}
static u32 address(const void *p){
 uintptr_t a=(uintptr_t)p,b=(uintptr_t)world.gpu;
 if(a>=b&&a-b<sizeof(world.gpu))return 0x80180000U+(u32)(a-b);
 b=(uintptr_t)&world.stream;if(a>=b&&a-b<=sizeof(world.stream))return 0x80200010U+(u32)(a-b);
 assert(!"unknown or truncated native pointer");return 0;
}
static void loadState(struct World *w,const u8 *p){
 memcpy(&w->stream,p,168);memcpy(&w->scratch.work7C.projection188,p+168,504);
 w->scratch.work7C.firstArea74=word(get(p+672));w->scratch.work7C.windingBC=word(get(p+676));
 w->scratch.work7C.colors15C.palette.rgb160[0]=word(get(p+680));
 w->scratch.work7C.ordering6C=resolve(get(p+684));
 w->scratch.scene.view.projection.pointers.cursor=resolve(get(p+688));
 w->scratch.scene.depthShift60=word(get(p+692));memcpy(w->gpu,p+696,8192);
}
static void snapshot(u8 *p){
 memcpy(p,&world.stream,168);memcpy(p+168,&WORK.projection188,504);
 put(p+672,(u32)WORK.firstArea74);put(p+676,(u32)WORK.windingBC);
 put(p+680,(u32)WORK.colors15C.palette.rgb160[0]);put(p+684,address(WORK.ordering6C));
 put(p+688,address(SCENE.view.projection.pointers.cursor));put(p+692,(u32)SCENE.depthShift60);
 memcpy(p+696,world.gpu,8192);
}
static void event(u32 kind,u32 address,u32 value){assert(events++<64);*trace++=kind;*trace++=address;*trace++=value;}
static void mutate(void){
 u32 mask=scenario*0x31415927U+q*0x17171717U+37;
 RroTexturedQuadPrefix *quad=&world.stream.quads[q];
 quad->uv0C=word((u32)quad->uv0C^mask);quad->uv10=word((u32)quad->uv10^mask);quad->uv14=word((u32)quad->uv14^mask);
 quad->orderingBias19=(s8)(u8)(biasSeed+q*37);
 WORK.ordering6C=&world.gpu[(scenario+q)&1].words[256+q*8];
 WORK.colors15C.palette.rgb160[0]=word((u32)WORK.colors15C.palette.rgb160[0]^mask);
 WORK.windingBC=word((u32)WORK.windingBC^mask);
 SCENE.view.projection.pointers.cursor=(u8*)&world.gpu[(scenario+q+1)&1].words[900];
 SCENE.depthShift60=(s32)((scenario+q+11)&31)-4;
 for(unsigned i=0;i<63;i++){
  WORK.projection188.pipeline[i].sxy=word((u32)WORK.projection188.pipeline[i].sxy^(mask+i));
  WORK.projection188.pipeline[i].sz=word((u32)WORK.projection188.pipeline[i].sz+mask+i);
 }
}
static void nclip(s32 a,s32 b,s32 c){
 static const s32 areas[]={0,1,-1,-2147483647-1,2147483647};q++;
 data[12]=(u32)a;event(0x10C,0,(u32)a);data[14]=(u32)c;event(0x10E,0,(u32)c);
 data[13]=(u32)b;event(0x10D,0,(u32)b);event(0x600,0,0x4B400006);
 data[24]=(u32)areas[(scenario+q*3)%5];
}
static void storeArea(void *p){assert(p==&WORK.firstArea74);event(0x218,0x1F8000F0,data[24]);put(p,data[24]);mutate();}
#define RR_GTE_NCLIP_SXY3(a,b,c) nclip(a,b,c)
#define RR_GTE14_SWC2_MAC0(p) storeArea(p)
'''
TAIL = r'''
void run_case(unsigned number,unsigned bias,s32 count,const u8 *input,const u8 *wanted,u8 *output){
 assert(sizeof(void*)==8&&sizeof(unsigned long)==sizeof(void*)&&(uintptr_t)&world>UINT32_MAX);
 if(!region.native){region=(GpuDmaRegion){world.gpu,0x80180000U,sizeof(world.gpu)};assert(GpuDmaSetRegions(&region,1));}
 memset(&world,0xA5,sizeof(world));loadState(&world,input);expected=world;loadState(&expected,wanted);
 scenario=number;biasSeed=bias;events=0;q=~0U;memset(output,0,OUTPUT_SIZE);trace=(u32*)(output+8);
 for(unsigned i=0;i<32;i++)data[i]=number*0x1234567U+i*0x17171717U;
 const RroTexturedQuadPrefix *result=EmitProjectedQuadsFT4(world.stream.quads,count,&WORK);
 assert(!memcmp(&world,&expected,sizeof(world)));
 put(output,address(result));put(output+4,events);snapshot(output+8+64*12);memcpy(output+8+64*12+STATE_SIZE,data,128);
}
'''


def mutate(flow, q, bias, state):
    get = lambda at: struct.unpack_from('<I', state, at)[0]
    def put(at, value):struct.pack_into('<I', state, at, value & 0xFFFFFFFF)
    mask = (flow*0x31415927+q*0x17171717+37) & 0xFFFFFFFF
    for at in (q*28+12, q*28+16, q*28+20):put(at, get(at)^mask)
    state[q*28+25] = (bias+q*37) & 255
    put(684, GPU+((flow+q)&1)*4096+(256+q*8)*4)
    put(680, get(680)^mask);put(676, get(676)^mask)
    put(688, GPU+((flow+q+1)&1)*4096+900*4);put(692, ((flow+q+11)&31)-4)
    for i in range(63):
        at = 168+i*8;put(at, get(at)^(mask+i));put(at+4, get(at+4)+mask+i)


def oracle(flow, bias, count, incoming):
    state = bytearray(incoming);events = [];paths = set();first_head = None
    get = lambda at: struct.unpack_from('<I', state, at)[0]
    def put(at, value):struct.pack_into('<I', state, at, value & 0xFFFFFFFF)
    data = [(flow*0x1234567+i*0x17171717) & 0xFFFFFFFF for i in range(32)]
    packet, shift, winding = get(688), signed(get(692), 32)+4, get(676)
    if count <= 0:paths.add('empty')
    for q in range(max(count, 0)):
        i0, i1, i2, i3 = struct.unpack_from('<4H', state, q*28)
        indices = (i0, signed(i1, 16), i2, signed(i3, 16))
        xy = [get(168+(i+3)*8) for i in indices]
        depth = signed(sum(get(172+(i+3)*8) for i in indices) & 0xFFFFFFFF, 32)
        for r, v in ((12,xy[0]), (14,xy[2]), (13,xy[1])):
            data[r] = v;events.extend((0x100+r,0,v))
        events.extend((0x600,0,0x4B400006));data[24] = AREAS[(flow+q*3)%5] & 0xFFFFFFFF
        at = 696+packet-GPU
        for i, v in enumerate(xy):put(at+8+i*8,v)
        events.extend((0x218,WORK+0x74,data[24]));put(672,data[24]);mutate(flow,q,bias,state)
        if signed(get(672)^winding,32) <= 0:paths.add('area');continue
        if depth <= 0:paths.add('nonpositive');continue
        depth >>= shift
        if depth >= 448:paths.add('far');continue
        paths.add('emitted');head = get(684)+(depth+signed(state[q*28+25],8))*4
        if first_head is None:first_head = head
        h = 696+head-GPU
        put(at,get(h));state[at+3] = 9;put(h,packet);state[h+3] = 0
        for i in range(3):put(at+12+i*8,get(q*28+12+i*4))
        put(at+36,signed(get(q*28+20),32)>>16);put(at+4,get(680));packet += 40
    put(688,packet)
    result = STREAM+max(count,0)*28
    out = struct.pack('<2I',result,len(events)//3)+struct.pack('<'+'I'*len(events),*events)
    out += bytes(MAX_EVENTS*12-len(events)*4)+state+struct.pack('<32I',*data)
    return bytes(state),out,paths,first_head


def main():
    owners = source_owners(ROOT/'src/main/PAL');body = definition_source(owners[NAME].read_text(),NAME)
    original = (ROOT/'assets/PAL/main.exe').read_bytes();base = struct.unpack_from('<I',original,24)[0]
    entry = address_of(NAME)
    with TemporaryDirectory() as directory:
        source,library = Path(directory)/'native.c',Path(directory)/'native.dylib'
        source.write_text(PREFIX+body+TAIL.replace('OUTPUT_SIZE',str(OUTPUT)).replace('STATE_SIZE',str(STATE)))
        subprocess.run(['cc','-std=c11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                        '-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I'+str(ROOT/'include'),
                        str(source),str(ROOT/'src/native/gpu_address.c'),'-o',str(library)],check=True)
        native = C.CDLL(str(library));native.run_case.argtypes = [C.c_uint,C.c_uint,C.c_int32,C.c_void_p,C.c_void_p,C.c_void_p]
        memory = Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0,0x400000);memory.mem_map(SCRATCH,4096)
        write = lambda a,b: memory.mem_write(a & 0x1FFFFFFF,bytes(b))
        read = lambda a,n: bytes(memory.mem_read(a & 0x1FFFFFFF,n))
        put = lambda a,v: write(a,struct.pack('<I',v & 0xFFFFFFFF))
        write(base,original[0x800:])
        regions = [(STREAM,168),(PROJECTED,504),(WORK+0x74,4),(WORK+0xBC,4),
                   (WORK+0x160,4),(WORK+0x6C,4),(SCRATCH,4),(SCRATCH+0x60,4),(GPU,8192)]
        assert sum(n for a,n in regions) == STATE
        def snapshot():return b''.join(read(a,n) for a,n in regions)
        def restore(state):
            at = 0
            for a,n in regions:write(a,state[at:at+n]);at += n
        def store(cpu,access,address,size,value,user):
            a = address & 0x1FFFFFFF
            assert any((start & 0x1FFFFFFF) <= a and a+size <= (start & 0x1FFFFFFF)+n
                       for start,n in [(WORK+0x74,4),(SCRATCH,4),(GPU,8192),(STACK-8,8)]),(hex(address),size)
        cpu = Cpu(memory,0,100,101,102,store)
        cases = []
        for case in range(CASES):
            forced = case >= 8192;flow = 1 if forced else case;bias = case-8192 if forced else case>>2
            count = 1 if forced else COUNTS[case%len(COUNTS)];shift = 0 if forced else (case>>5)&31
            state = bytearray((case*19+i*29)&255 for i in range(STATE))
            for q in range(6):
                indices = [(case+q*7+i*13)%60 for i in range(4)]
                if not forced:
                    indices[1] = (case+q*11)%63-3;indices[3] = (case+q*17)%63-3
                struct.pack_into('<4H',state,q*28,*[i & 65535 for i in indices])
                struct.pack_into('<3I',state,q*28+12,*[UVS[((case>>3)+q+i)&7] for i in range(3)])
            for i in range(63):
                struct.pack_into('<Ii',state,168+i*8,(case*0x1234567+i*0x17171717)&0xFFFFFFFF,
                                 1 if forced else DEPTHS[((case>>4)+i)&7])
            struct.pack_into('<6I',state,672,0,0 if forced else AREAS[(case>>8)%5]&0xFFFFFFFF,
                             case*0x12345 & 0xFFFFFFFF,GPU+256*4,GPU+900*4,(shift-4)&0xFFFFFFFF)
            struct.pack_into('<2048I',state,696,*[(case*0x31415927+i*0x17171717)&0xFFFFFFFF for i in range(2048)])
            _,_,_,first = oracle(flow,bias,count,state)
            if (case & 4096 or forced) and first is not None:
                struct.pack_into('<I',state,688,first-(case%10)*4)
            cases.append((flow,bias,count,state,shift))
        for version in ('original','linked'):
            if version == 'linked':
                with (ROOT/'build/PAL/main.elf').open('rb') as stream:
                    elf = ELFFile(stream);symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(NAME)[0]
                    assert symbol['st_value'] == entry and symbol['st_size'] == 0x19C
                    section = elf.get_section(symbol['st_shndx']);off = entry-section['sh_addr']
                    write(entry,section.data()[off:off+symbol['st_size']])
            paths,aliases,biases,shifts,counts = (set() for _ in range(5))
            for case,(flow,bias,count,state,shift) in enumerate(cases):
                wanted,expected,seen,first = oracle(flow,bias,count,state)
                output = C.create_string_buffer(OUTPUT)
                native.run_case(flow,bias,count,C.create_string_buffer(bytes(state)),C.create_string_buffer(wanted),output)
                assert output.raw == expected,(version,case,'native/wire')
                restore(state);write(STACK-256,b'\xA6'*512)
                cpu.reset();cpu.r = [0]+[0xBEEF0000+r for r in range(1,32)]
                cpu.r[4:7] = [STREAM,count & 0xFFFFFFFF,WORK];cpu.r[29] = STACK;cpu.r[31] = STOP;cpu.pc = entry
                data = [(flow*0x1234567+i*0x17171717)&0xFFFFFFFF for i in range(32)];events = [];q = -1
                for step in range(2000):
                    if cpu.pc == STOP:break
                    instruction = int.from_bytes(read(cpu.pc,4),'little');opcode = instruction>>26
                    if opcode not in (0x12,0x3A):cpu.step();continue
                    delayed = cpu.delay;cpu.delay = None;rs,rt = (instruction>>21)&31,(instruction>>16)&31
                    if opcode == 0x3A:
                        a = (cpu.r[rs]+signed(instruction,16)) & 0xFFFFFFFF
                        assert a == WORK+0x74 and rt == 24
                        events.extend((0x218,a,data[24]));cpu.store(a,data[24],4)
                        changed = bytearray(snapshot());mutate(flow,q,bias,changed);restore(changed)
                    elif rs == 4:
                        rd = (instruction>>11)&31
                        assert rd in (12,14,13)
                        if rd == 12:q += 1
                        data[rd] = cpu.r[rt];events.extend((0x100+rd,0,data[rd]))
                    else:
                        assert instruction == 0x4B400006
                        events.extend((0x600,0,instruction));data[24] = AREAS[(flow+q*3)%5] & 0xFFFFFFFF
                    cpu.pc = delayed if delayed is not None else cpu.pc+4
                assert cpu.pc == STOP and q+1 == max(count,0),(version,case,hex(cpu.pc))
                actual = struct.pack('<2I',cpu.r[2],len(events)//3)+struct.pack('<'+'I'*len(events),*events)
                actual += bytes(MAX_EVENTS*12-len(events)*4)+snapshot()+struct.pack('<32I',*data)
                assert actual == expected,(version,case,'MIPS/wire')
                assert cpu.r[29] == STACK and cpu.r[31] == STOP
                for r in (*range(16,24),28,30):assert cpu.r[r] == 0xBEEF0000+r
                assert read(STACK-256,248) == b'\xA6'*248 and read(STACK,256) == b'\xA6'*256
                paths |= seen;shifts.add(shift);counts.add(count)
                if first is not None:
                    delta = first-struct.unpack_from('<I',state,688)[0]
                    if 0 <= delta < 40:aliases.add(delta//4)
                    if case >= 8192:biases.add(signed(bias & 255,8))
            assert paths == {'empty','area','nonpositive','far','emitted'},paths
            assert aliases == set(range(10)) and shifts == set(range(32)) and counts == set(COUNTS),(aliases,shifts,counts)
            assert biases == set(range(-128,128)),len(biases)
            print('PASS',version,CASES,'actual projected FT4 flows; high native owners, negative/zero counts, signed indices, wrapping depth, live fields, all biases/shifts, packet/head aliases and guards',flush=True)


if __name__ == '__main__':main()
