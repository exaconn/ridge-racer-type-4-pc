"""Actual trail-panel emitter, native owners and original/linked MIPS traces.

NCLIP is scripted. An independent wire oracle checks three G4 packets and E1,
captured roots/colors, live indices/points, clipping and packet/head overlaps.
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
NAME = 'EmitProjectedLitTrisGT3'
SCRATCH, WORK, STREAM, POINTS = 0x1F800000, 0x1F80007C, 0x80200010, 0x80210010
GPU, STACK, STOP = 0x80180000, 0x803F0000, 0x803FF000
STATE, MAX_EVENTS, CASES = 8408, 32, 4096+256+58
OUTPUT = 4+MAX_EVENTS*12+STATE+128
COUNTS = (-32768, -1, 0, 1, 2, 3, 4)
AREAS = (0, 1, -1, -2147483648, 2147483647)
DEPTHS = (-2147483648, -1, 0, 1, 447, 448, 449, 895, 896, 1791, 1792, 65535, 2147483647)
XY = ((10,20),(-1,20),(320,20),(10,-1),(10,256),(319,255),
      (-32768,-32768),(32767,32767),(0,0),(-1,-1),(320,256))
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "rr/rro_effects.h"
#include "rr/gpu_address.h"
typedef union {u32 words[12];RroTrailPanelCommand commands[4];} Stream;
typedef union {u32 words[1024];struct {GpuPolyG4 panels[3];GpuCommandPacket mode;} groups[35];} GpuArena;
static struct World {
 u8 before[16];RroEffectScratch scratch;Stream stream;RroProjectedVertex points[2][8];
 _Alignas(8192) GpuArena gpu[2];u8 after[16];
} world,expected;
#define SCRATCHPAD ((u8*)&world.scratch)
#define WORK world.scratch.work7C
#define SCENE world.scratch.scene
static unsigned scenario,biasSeed,q,events;static u32 data[32],*trace;
static GpuDmaRegion region;
static u32 get(const void *p){u32 v;memcpy(&v,p,4);return v;}
static void put(void *p,u32 v){memcpy(p,&v,4);}
static s32 word(u32 v){s32 x;memcpy(&x,&v,4);return x;}
static void *resolve(u32 a){
 u32 d=a-0x80180000U;if(d<sizeof(world.gpu))return (u8*)world.gpu+d;
 d=a-0x80210010U;assert(d<sizeof(world.points));return (u8*)world.points+d;
}
static u32 address(const void *p){
 uintptr_t a=(uintptr_t)p,b=(uintptr_t)world.gpu;
 if(a>=b&&a-b<sizeof(world.gpu))return 0x80180000U+(u32)(a-b);
 b=(uintptr_t)world.points;if(a>=b&&a-b<sizeof(world.points))return 0x80210010U+(u32)(a-b);
 assert(!"unknown or truncated native pointer");return 0;
}
static void loadState(struct World *w,const u8 *p){
 memcpy(&w->stream,p,48);memcpy(w->points,p+48,128);
 memcpy(&w->scratch.work7C.depth70,p+176,8);memcpy(w->scratch.work7C.colors15C.palette.rgb160,p+184,8);
 w->scratch.work7C.windingBC=word(get(p+192));w->scratch.work7C.ordering6C=resolve(get(p+196));
 w->scratch.work7C.projectedD0=resolve(get(p+200));w->scratch.work7C.projectedD4=resolve(get(p+204));
 w->scratch.scene.view.projection.pointers.cursor=resolve(get(p+208));w->scratch.scene.depthShift60=word(get(p+212));
 memcpy(w->gpu,p+216,8192);
}
static void snapshot(u8 *p){
 memcpy(p,&world.stream,48);memcpy(p+48,world.points,128);memcpy(p+176,&WORK.depth70,8);
 memcpy(p+184,WORK.colors15C.palette.rgb160,8);put(p+192,(u32)WORK.windingBC);put(p+196,address(WORK.ordering6C));
 put(p+200,address(WORK.projectedD0));put(p+204,address(WORK.projectedD4));
 put(p+208,address(SCENE.view.projection.pointers.cursor));put(p+212,(u32)SCENE.depthShift60);memcpy(p+216,world.gpu,8192);
}
static void event(u32 kind,u32 at,u32 value){assert(events++<32);*trace++=kind;*trace++=at;*trace++=value;}
static u32 coordinate(unsigned i){
 static const s16 xy[][2]={{10,20},{-1,20},{320,20},{10,-1},{10,256},{319,255},
                         {-32768,-32768},{32767,32767},{0,0},{-1,-1},{320,256}};
 unsigned mode=(scenario+q*3+5)%12;
 if(scenario==1||scenario==6)mode=0;
 if(mode==11)mode=(i&1)?10:9;
 return (u16)xy[mode][0]|((u32)(u16)xy[mode][1]<<16);
}
static void mutate(void){
 u32 mask=scenario*0x31415927U+q*0x17171717U+37;RroTrailPanelCommand *c=&world.stream.commands[q];
 for(unsigned i=0;i<3;i++)c->outer[i]=(c->outer[i]+q+3)&7;
 c->center=(c->center+q+5)&7;c->orderingBias09=(s8)(u8)(biasSeed+q*37);
 WORK.ordering6C=&world.gpu[(scenario+q)&1].words[256+q*8];WORK.depth70=1+(scenario+q*13)%447;
 WORK.projectedD0=world.points[(scenario+q)&1];WORK.projectedD4=world.points[(scenario+q+1)&1];
 WORK.colors15C.palette.rgb160[0]=word((u32)WORK.colors15C.palette.rgb160[0]^mask);
 WORK.colors15C.palette.rgb160[1]=word((u32)WORK.colors15C.palette.rgb160[1]^(mask+1));
 WORK.windingBC=word((u32)WORK.windingBC^mask);
 SCENE.view.projection.pointers.cursor=(u8*)&world.gpu[(scenario+q+1)&1].words[900];
 SCENE.depthShift60=(s32)((scenario+q+11)&31)-3;
 for(unsigned i=0;i<16;i++){
  world.points[i/8][i%8].sxy=word(coordinate(i));
  world.points[i/8][i%8].sz=word((u32)world.points[i/8][i%8].sz+mask+i);
 }
}
static void loadXY(s32 a,s32 b,s32 c){
 q++;data[12]=(u32)a;event(0x10C,0,(u32)a);data[14]=(u32)c;event(0x10E,0,(u32)c);
 data[13]=(u32)b;event(0x10D,0,(u32)b);
}
static void nclip(void){
 static const s32 areas[]={0,1,-1,-2147483647-1,2147483647};
 event(0x600,(u32)WORK.depth70,0x4B400006);data[24]=(u32)areas[(scenario+q*3)%5];
}
static void storeArea(void *p){assert(p==&WORK.firstArea74);event(0x218,0x1F8000F0,data[24]);put(p,data[24]);mutate();}
#define RR_GTE_LDSXY3(a,b,c) loadXY(a,b,c)
#define RR_GTE14_NCLIP() nclip()
#define RR_GTE14_SWC2_MAC0(p) storeArea(p)
'''
TAIL = r'''
void run_case(unsigned flow,unsigned bias,s32 count,const u8 *input,const u8 *wanted,u8 *output){
 assert(sizeof(void*)==8&&(uintptr_t)&world>UINT32_MAX);
 if(!region.native){region=(GpuDmaRegion){world.gpu,0x80180000U,sizeof(world.gpu)};assert(GpuDmaSetRegions(&region,1));}
 memset(&world,0xA5,sizeof(world));loadState(&world,input);expected=world;loadState(&expected,wanted);
 assert((uintptr_t)WORK.projectedD0>UINT32_MAX&&(uintptr_t)WORK.projectedD4>UINT32_MAX);
 scenario=flow;biasSeed=bias;q=~0U;events=0;memset(output,0,OUTPUT_SIZE);trace=(u32*)(output+4);
 for(unsigned i=0;i<32;i++)data[i]=flow*0x1234567U+i*0x17171717U;
 EmitProjectedLitTrisGT3(world.stream.commands,count,&WORK);
 assert(!memcmp(&world,&expected,sizeof(world)));
 put(output,events);snapshot(output+4+32*12);memcpy(output+4+32*12+STATE_SIZE,data,128);
}
'''


def coordinate(flow, i, mode):
    if flow in (1,6):mode = 0
    if mode == 11:mode = 10 if i & 1 else 9
    x,y = XY[mode]
    return (x & 65535)|((y & 65535)<<16)


def mutate(flow, q, bias, state):
    get = lambda at: struct.unpack_from('<I',state,at)[0]
    def put(at,v):struct.pack_into('<I',state,at,v & 0xFFFFFFFF)
    mask = (flow*0x31415927+q*0x17171717+37) & 0xFFFFFFFF
    outer = struct.unpack_from('<3H',state,q*12)
    struct.pack_into('<3H',state,q*12,*[(i+q+3)&7 for i in outer])
    center = struct.unpack_from('<H',state,q*12+6)[0];struct.pack_into('<H',state,q*12+6,(center+q+5)&7)
    state[q*12+9] = (bias+q*37) & 255
    put(196,GPU+((flow+q)&1)*4096+(256+q*8)*4);put(176,1+(flow+q*13)%447)
    put(200,POINTS+((flow+q)&1)*64);put(204,POINTS+((flow+q+1)&1)*64)
    put(184,get(184)^mask);put(188,get(188)^(mask+1));put(192,get(192)^mask)
    put(208,GPU+((flow+q+1)&1)*4096+900*4);put(212,((flow+q+11)&31)-3)
    for i in range(16):
        put(48+i*8,coordinate(flow,i,(flow+q*3+5)%12));put(52+i*8,get(52+i*8)+mask+i)


def oracle(flow, bias, count, incoming):
    state = bytearray(incoming);events = [];paths = set();first_head = None
    get = lambda at: struct.unpack_from('<I',state,at)[0]
    def put(at,v):struct.pack_into('<I',state,at,v & 0xFFFFFFFF)
    packet,shift,winding = get(208),signed(get(212),32)+3,get(192)
    base,alt = 48+get(200)-POINTS,48+get(204)-POINTS
    primary,secondary = get(184)|0x3A000000,get(188)
    data = [(flow*0x1234567+i*0x17171717)&0xFFFFFFFF for i in range(32)]
    if count <= 0:paths.add('empty')
    for q in range(max(count,0)):
        outer = struct.unpack_from('<3H',state,q*12);center = struct.unpack_from('<H',state,q*12+6)[0]
        xy = [get(base+i*8) for i in outer]
        for r,v in ((12,xy[0]),(14,xy[2]),(13,xy[1])):data[r] = v;events.extend((0x100+r,0,v))
        depth = signed(get(base+center*8+4),32)>>shift;put(176,depth)
        if not 1 <= depth <= 447:paths.add('depth');continue
        events.extend((0x600,depth,0x4B400006));data[24] = AREAS[(flow+q*3)%5]&0xFFFFFFFF
        at = 216+packet-GPU
        for i,v in enumerate(xy):put(at+16+i*36,v)
        xs = [signed(get(pool+i*8),16) for pool in (base,alt) for i in outer]
        events.extend((0x218,WORK+0x74,data[24]));put(180,data[24]);mutate(flow,q,bias,state)
        if signed(get(180)^winding,32) <= 0:paths.add('area');continue
        if max(xs)<0 or min(xs)>=320:paths.add('x');continue
        outer = struct.unpack_from('<3H',state,q*12)
        ys = [signed(get(pool+i*8)>>16,16) for pool in (base,alt) for i in outer]
        if max(ys)<0 or min(ys)>=256:paths.add('y');continue
        paths.add('emitted');center = struct.unpack_from('<H',state,q*12+6)[0]
        for i,index in enumerate(outer):put(at+32+i*36,get(alt+index*8))
        for i in (2,1,0):put(at+i*36+8,get(base+center*8))
        other = get(alt+center*8)
        for offset,value in [(4,primary),(20,secondary),(28,0),(12,0),(24,other)]:
            for i in (2,1,0):put(at+i*36+offset,value)
        head = get(196)+(signed(get(176),32)+signed(state[q*12+9],8))*4
        if first_head is None:first_head = head
        h = 216+head-GPU
        for i in range(3):
            put(at+i*36,get(h));state[at+i*36+3] = 8;put(h,packet+i*36);state[h+3] = 0
        put(at+112,0xE1000220);put(at+108,get(h));state[at+111] = 1
        put(h,packet+108);state[h+3] = 0;packet += 116
    put(208,packet)
    out = struct.pack('<I',len(events)//3)+struct.pack('<'+'I'*len(events),*events)
    out += bytes(MAX_EVENTS*12-len(events)*4)+state+struct.pack('<32I',*data)
    return bytes(state),out,paths,first_head


def main():
    owner = source_owners(ROOT/'src/main/PAL')[NAME];body = definition_source(owner.read_text(),NAME)
    original = (ROOT/'assets/PAL/main.exe').read_bytes();base = struct.unpack_from('<I',original,24)[0];entry = address_of(NAME)
    with TemporaryDirectory() as directory:
        source,library = Path(directory)/'native.c',Path(directory)/'native.dylib'
        source.write_text(PREFIX+body+TAIL.replace('OUTPUT_SIZE',str(OUTPUT)).replace('STATE_SIZE',str(STATE)))
        subprocess.run(['cc','-std=c11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                        '-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I'+str(ROOT/'include'),
                        str(source),str(ROOT/'src/native/gpu_address.c'),'-o',str(library)],check=True)
        native = C.CDLL(str(library));native.run_case.argtypes = [C.c_uint,C.c_uint,C.c_int32,C.c_void_p,C.c_void_p,C.c_void_p]
        memory = Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000);memory.mem_map(SCRATCH,4096)
        write = lambda a,b: memory.mem_write(a & 0x1FFFFFFF,bytes(b));read = lambda a,n: bytes(memory.mem_read(a & 0x1FFFFFFF,n))
        write(base,original[0x800:])
        regions = [(STREAM,48),(POINTS,128),(WORK+0x70,8),(WORK+0x160,8),(WORK+0xBC,4),
                   (WORK+0x6C,4),(WORK+0xD0,8),(SCRATCH,4),(SCRATCH+0x60,4),(GPU,8192)]
        assert sum(n for a,n in regions) == STATE
        def snapshot():return b''.join(read(a,n) for a,n in regions)
        def restore(state):
            at = 0
            for a,n in regions:write(a,state[at:at+n]);at += n
        def store(cpu,access,address,size,value,user):
            a = address & 0x1FFFFFFF
            assert any((start & 0x1FFFFFFF)<=a and a+size<=(start & 0x1FFFFFFF)+n
                       for start,n in [(WORK+0x70,8),(SCRATCH,4),(GPU,8192),(STACK-24,24)]),(hex(address),size)
        cpu = Cpu(memory,0,100,101,102,store);cases = []
        for case in range(CASES):
            forced = case>=4096;flow = (1 if case<4352 or case&1 else 6) if forced else case
            bias = case-4096 if forced else case>>2;count = 1 if forced else COUNTS[case%len(COUNTS)]
            shift = 0 if forced else (case>>1)&31
            state = bytearray((case*19+i*29)&255 for i in range(STATE))
            for q in range(4):struct.pack_into('<4H',state,q*12,*[(case+q+i*3)&7 for i in range(4)])
            for i in range(16):
                struct.pack_into('<Ii',state,48+i*8,coordinate(flow,i,(case>>4)%12),1 if forced else DEPTHS[(case>>6)%len(DEPTHS)])
            struct.pack_into('<10I',state,176,0,0,case*0x17171717&0xFFFFFFFF,~case&0xFFFFFFFF,
                             0 if forced else AREAS[(case>>7)%5]&0xFFFFFFFF,GPU+256*4,
                             POINTS+((case>>2)&1)*64,POINTS+((case>>3)&1)*64,GPU+850*4,(shift-3)&0xFFFFFFFF)
            struct.pack_into('<2048I',state,216,*[(case*0x31415927+i*0x17171717)&0xFFFFFFFF for i in range(2048)])
            _,_,_,first = oracle(flow,bias,count,state)
            if (case&2048 or forced) and first is not None:
                gap = ((case-4352)//2)%29 if case>=4352 else case%29
                struct.pack_into('<I',state,208,first-gap*4)
            cases.append((flow,bias,count,state,shift))
        for version in ('original','linked'):
            if version == 'linked':
                with (ROOT/'build/PAL/main.elf').open('rb') as stream:
                    elf = ELFFile(stream);symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(NAME)[0]
                    assert symbol['st_value'] == entry and symbol['st_size'] == 0x3E8
                    section = elf.get_section(symbol['st_shndx']);off = entry-section['sh_addr'];write(entry,section.data()[off:off+symbol['st_size']])
            paths,aliases,biases,shifts,counts,pointer_aliases = (set() for _ in range(6))
            for case,(flow,bias,count,state,shift) in enumerate(cases):
                wanted,expected,seen,first = oracle(flow,bias,count,state);output = C.create_string_buffer(OUTPUT)
                native.run_case(flow,bias,count,C.create_string_buffer(bytes(state)),C.create_string_buffer(wanted),output)
                assert output.raw == expected,(version,case,'native/wire')
                restore(state);write(STACK-256,b'\xA6'*512);cpu.reset();cpu.r = [0]+[0xBEEF0000+r for r in range(1,32)]
                cpu.r[4:7] = [STREAM,count&0xFFFFFFFF,WORK];cpu.r[29] = STACK;cpu.r[31] = STOP;cpu.pc = entry
                data = [(flow*0x1234567+i*0x17171717)&0xFFFFFFFF for i in range(32)];events = [];q = -1
                for step in range(3000):
                    if cpu.pc == STOP:break
                    instruction = int.from_bytes(read(cpu.pc,4),'little');opcode = instruction>>26
                    if opcode not in (0x12,0x3A):cpu.step();continue
                    delayed = cpu.delay;cpu.delay = None;rs,rt = (instruction>>21)&31,(instruction>>16)&31
                    if opcode == 0x3A:
                        a = (cpu.r[rs]+signed(instruction,16))&0xFFFFFFFF;assert a == WORK+0x74 and rt == 24
                        events.extend((0x218,a,data[24]));cpu.store(a,data[24],4)
                        changed = bytearray(snapshot());mutate(flow,q,bias,changed);restore(changed)
                    elif rs == 4:
                        rd = (instruction>>11)&31;assert rd in (12,14,13)
                        if rd == 12:q += 1
                        data[rd] = cpu.r[rt];events.extend((0x100+rd,0,data[rd]))
                    else:
                        assert instruction == 0x4B400006
                        events.extend((0x600,int.from_bytes(read(WORK+0x70,4),'little'),instruction));data[24] = AREAS[(flow+q*3)%5]&0xFFFFFFFF
                    cpu.pc = delayed if delayed is not None else cpu.pc+4
                assert cpu.pc == STOP and q+1 == max(count,0),(version,case,hex(cpu.pc))
                actual = struct.pack('<I',len(events)//3)+struct.pack('<'+'I'*len(events),*events)
                actual += bytes(MAX_EVENTS*12-len(events)*4)+snapshot()+struct.pack('<32I',*data)
                assert actual == expected,(version,case,'MIPS/wire')
                assert cpu.r[29] == STACK and cpu.r[31] == STOP
                for r in (*range(16,24),28,30):assert cpu.r[r] == 0xBEEF0000+r
                assert read(STACK-256,232) == b'\xA6'*232 and read(STACK,256) == b'\xA6'*256
                paths |= seen;shifts.add(shift);counts.add(count)
                pointer_aliases.add(struct.unpack_from('<I',state,200)[0]==struct.unpack_from('<I',state,204)[0])
                if first is not None:
                    delta = first-struct.unpack_from('<I',state,208)[0]
                    if 0<=delta<116:aliases.add(delta//4)
                    if case>=4096:biases.add(signed(bias&255,8))
            assert paths == {'empty','depth','area','x','y','emitted'},paths
            assert aliases == set(range(29)) and biases == set(range(-128,128)),(aliases,len(biases))
            assert shifts == set(range(32)) and counts == set(COUNTS) and pointer_aliases == {False,True}
            print('PASS',version,CASES,'actual three-G4/E1 trail flows; native owners, captured roots/colors, live indices/coordinates, clipping, all biases/shifts and29 packet/head aliases',flush=True)


if __name__ == '__main__':main()
