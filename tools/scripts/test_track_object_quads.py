"""Current billboard/flash C vs original/compiled MIPS and a wire oracle.

Complete native owners and pointers; GTE arithmetic is scripted. Store hooks
stress cached roots, prefetched translations and live primitive/palette fields.
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
MESH, BILL, FLASH, VERTICES, WORK = 0x80200010, 0x80210010, 0x80211010, 0x80220010, 0x1F80007C
STACK, STOP = 0x803F0000, 0x803FF000
CASES, PREFIX_SIZE, GPU_SIZE, MAX_EVENTS = 8192, 2628, 16384, 256
STATE = PREFIX_SIZE+GPU_SIZE
OUTPUT = 8+MAX_EVENTS*12+STATE+256
DEPTHS = (-2147483648, -1, 0, 1, 447, 448, 1791, 1792)
AREAS = (0, 1, -1, -2147483648, 2147483647)
FRAMES = (0, 1, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF)
UVS = (0, 1, 0x7FFFFFFF, 0x80000000, 0x8000FFFF, 0xFFFF0000, 0xFFFFFFFF, 0x12345678)
XY = ((0, 0), (-1, 20), (320, 20), (10, -1), (10, 256),
      (319, 255), (-32768, -32768), (32767, 32767), (-1, -1), (320, 256))
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "game/gfx/tilerect.h"
#include "rr/gpu.h"
#include "rr/model_table.h"
typedef union {u32 words[57];struct {RroBodyCommandHeader header;TrackObjectQuad quads[7];} decoded;} BillStream;
typedef union {u32 words[43];struct {RroBodyCommandHeader header;RroTexturedQuadPrefix quads[6];} decoded;} FlashStream;
typedef union {TrackObjectBillboardScratch billboard;TrackObjectFlashScratch flash;} Scratch;
typedef union {u32 words[2048];GpuPolyFT4 packets[204];} GpuArena;
typedef struct {
 u8 before[16];Scratch scratch;BillStream bills[4];FlashStream flashes[4];Vec3s8 vertices[2][16];
 MatrixWords32 camera;s32 frame,palettes[2][64];_Alignas(16384) GpuArena gpu[2];u8 after[16];
} World;
static World world,expected;
/* These are actual flexible-array owners, not an unrelated flat mesh array.
 * Comparing the whole allocations also checks the count, native padding and guards. */
static RroModelTable *tables[2],*expectedTables[2];
static u8 *tableMemory[2],*expectedTableMemory[2];
static const size_t tableBytes=16+sizeof(RroModelTable)+4*sizeof(RroEffectMesh)+16;
static void resetTables(void){
 for(unsigned b=0;b<2;b++){
  if(!tableMemory[b]){
   tableMemory[b]=malloc(tableBytes);expectedTableMemory[b]=malloc(tableBytes);
   assert(tableMemory[b]&&expectedTableMemory[b]);
   tables[b]=(RroModelTable*)(tableMemory[b]+16);expectedTables[b]=(RroModelTable*)(expectedTableMemory[b]+16);
   assert((uintptr_t)tables[b]>UINT32_MAX&&(uintptr_t)tables[b]%_Alignof(RroModelTable)==0);
  }
  memset(tableMemory[b],0xA5,tableBytes);memset(expectedTableMemory[b],0xA5,tableBytes);
 }
}
#define SCRATCHPAD ((u8*)&world.scratch)
#define CameraViewMatrix world.camera
#define ScreenFrame world.frame
#define TileFadeColorTable world.palettes[0]
#define D_8009E36C world.palettes[1]
#define SCENE world.scratch.billboard.scene
static unsigned kind,scenario,events,hooks,quad,streamIndex;static u32 data[32],control[32],*trace;
static const s32 depths[]={-2147483647-1,-1,0,1,447,448,1791,1792};
static const s32 areas[]={0,1,-1,-2147483647-1,2147483647};
static const u32 frames[]={0,1,0x7FFFFFFFU,0x80000000U,0xFFFFFFFFU};
static const s16 xy[][2]={{0,0},{-1,20},{320,20},{10,-1},{10,256},{319,255},{-32768,-32768},{32767,32767},{-1,-1},{320,256}};
static u8 *workBytes(Scratch *s){assert(offsetof(TrackObjectBillboardScratch,work7C)==offsetof(TrackObjectFlashScratch,work7C));return (u8*)s+offsetof(TrackObjectBillboardScratch,work7C);}
u32 gpu_address(void){uintptr_t p=(uintptr_t)world.gpu;assert(p>UINT32_MAX&&(p&16383)==0&&(p&0xFFFFFFU)+sizeof(world.gpu)<=0x1000000U);return 0x81000000U|(u32)(p&0xFFFFFFU);}
static u32 address(const void *pointer){
 uintptr_t p=(uintptr_t)pointer,b=(uintptr_t)world.gpu;
 if(p>=b&&p-b<sizeof(world.gpu))return gpu_address()+(u32)(p-b);
 b=(uintptr_t)world.vertices;if(p>=b&&p-b<sizeof(world.vertices))return 0x80220010U+(u32)(p-b);
 b=(uintptr_t)world.bills;if(p>=b&&p-b<sizeof(world.bills))return 0x80210010U+(u32)(p-b);
 b=(uintptr_t)world.flashes;if(p>=b&&p-b<sizeof(world.flashes))return 0x80211010U+(u32)(p-b);
 for(unsigned bank=0;bank<2;bank++){b=(uintptr_t)tables[bank]->entries;if(p>=b&&p-b<4*sizeof(RroEffectMesh)){assert((p-b)%sizeof(RroEffectMesh)==0);return 0x80200010U+bank*52U+(u32)((p-b)/sizeof(RroEffectMesh))*12U;}}
 b=(uintptr_t)workBytes(&world.scratch);if(p>=b&&p-b<68)return 0x1F80007CU+(u32)(p-b);
 assert(!"unmapped or truncated native pointer");return 0;
}
static void *resolve(u32 a){
 u32 offset=a-gpu_address();if(offset<sizeof(world.gpu))return (u8*)world.gpu+offset;
 offset=a-0x80220010U;if(offset<sizeof(world.vertices))return (u8*)world.vertices+offset;
 offset=a-0x80210010U;if(offset<sizeof(world.bills))return (u8*)world.bills+offset;
 offset=a-0x80211010U;if(offset<sizeof(world.flashes))return (u8*)world.flashes+offset;
 for(unsigned bank=0;bank<2;bank++){offset=a-(0x80200010U+bank*52U);if(offset<48&&offset%12==0)return &tables[bank]->entries[offset/12];}
 assert(!"invalid wire pointer");return 0;
}
static u32 get(const u8 **p){u32 v;memcpy(&v,*p,4);*p+=4;return v;}
static void put(u8 **p,u32 v){memcpy(*p,&v,4);*p+=4;}
static void loadState(World *w,RroModelTable **owners,const u8 *p){
 for(unsigned b=0;b<2;b++)for(unsigned i=0;i<4;i++){owners[b]->entries[i].vertices=resolve(get(&p));owners[b]->entries[i].vertexCount=get(&p);owners[b]->entries[i].commands=resolve(get(&p));}
 memcpy(w->bills,p,912);p+=912;memcpy(w->flashes,p,688);p+=688;memcpy(w->vertices,p,256);p+=256;memcpy(workBytes(&w->scratch),p,68);p+=68;
 RroSceneScratchPrefix *s=&w->scratch.billboard.scene;
 s->view.projection.pointers.cursor=resolve(get(&p));s->view.projection.pointers.order=resolve(get(&p));s->modelEntries48=resolve(get(&p));s->depthShift60=(s32)get(&p);s->renderFlags64=(s32)get(&p);
 memcpy(&s->view.matrix28,p,32);p+=32;memcpy(&w->camera,p,32);p+=32;w->frame=(s32)get(&p);memcpy(w->palettes,p,512);p+=512;for(unsigned b=0;b<2;b++)owners[b]->count=(s32)get(&p);memcpy(w->gpu,p,16384);
}
static void snapshot(u8 *p){
 for(unsigned b=0;b<2;b++)for(unsigned i=0;i<4;i++){put(&p,address(tables[b]->entries[i].vertices));put(&p,tables[b]->entries[i].vertexCount);put(&p,address(tables[b]->entries[i].commands));}
 memcpy(p,world.bills,912);p+=912;memcpy(p,world.flashes,688);p+=688;memcpy(p,world.vertices,256);p+=256;memcpy(p,workBytes(&world.scratch),68);p+=68;
 put(&p,address(SCENE.view.projection.pointers.cursor));put(&p,address(SCENE.view.projection.pointers.order));put(&p,address(SCENE.modelEntries48));put(&p,(u32)SCENE.depthShift60);put(&p,(u32)SCENE.renderFlags64);
 memcpy(p,&SCENE.view.matrix28,32);p+=32;memcpy(p,&world.camera,32);p+=32;put(&p,(u32)world.frame);memcpy(p,world.palettes,512);p+=512;for(unsigned b=0;b<2;b++)put(&p,(u32)tables[b]->count);memcpy(p,world.gpu,16384);
}
static void mutate(void){
 hooks++;unsigned bank=(scenario>>3)&1,model=(scenario>>1)&3;u32 mask=scenario*0x31415927U+quad;
 SCENE.modelEntries48=tables[bank^1]->entries;tables[bank]->entries[model].vertices=world.vertices[(scenario+quad)&1];
 tables[bank]->entries[model].commands=kind?world.flashes[(streamIndex+1)&3].words:world.bills[(streamIndex+1)&3].words;
 unsigned b=(scenario+quad)&1;SCENE.view.projection.pointers.cursor=(u8*)&world.gpu[b].words[1400+quad*20];SCENE.view.projection.pointers.order=&world.gpu[b^1].words[512+quad*4];
 SCENE.depthShift60=(scenario+quad+11)%30;SCENE.renderFlags64=(s32)(scenario*0x1234567U+quad);
 SCENE.view.matrix28.w[quad%8]=(s32)((u32)SCENE.view.matrix28.w[quad%8]^mask);world.camera.w[(quad+3)%8]=(s32)((u32)world.camera.w[(quad+3)%8]^mask);
 world.frame=(s32)frames[(scenario+quad)%5];s8 phase=(s8)(u8)((scenario&4096)?(scenario>>2)+quad*7:scenario*5+quad*7);s8 bias=(s8)(u8)((scenario>>2)+quad*37);
 if(kind){
  RroTexturedQuadPrefix *p=&world.flashes[streamIndex].decoded.quads[quad];
  for(unsigned i=0;i<4;i++)p->vertices.indices[i]=(p->vertices.indices[i]+3+quad)&15;
  p->uv0C=(s32)((u32)p->uv0C^mask);p->uv10=(s32)((u32)p->uv10^mask);p->uv14=(s32)((u32)p->uv14^mask);p->flashPhase18=phase;p->orderingBias19=bias;
 }else{
  TrackObjectQuad *p=&world.bills[streamIndex].decoded.quads[quad];
  for(unsigned i=0;i<4;i++)p->vertices.indices[i]=(p->vertices.indices[i]+3+quad)&15;
  p->uv0C=(s32)((u32)p->uv0C^mask);p->uv10=(s32)((u32)p->uv10^mask);p->uv14=(s32)((u32)p->uv14^mask);p->flashPhase18=phase;p->orderingBias19=bias;
  p[1].translationXY1C=(s32)((u32)p[1].translationXY1C^mask);p[1].translationZ1A=(u16)(p[1].translationZ1A+scenario+quad+1);
 }
 unsigned index=kind?((u32)world.frame+phase)&63:((u32)world.frame-(phase-64))&63;
 world.palettes[kind][index]=(s32)((scenario*0x1234567U+quad*0x17171717U)^0x2E000000U);
}
static void event(u32 kind_,u32 target,u32 value){assert(events++<256);*trace++=kind_;*trace++=target;*trace++=value;}
static void load(unsigned r,const void *p){u32 v;memcpy(&v,p,4);event(0x100+r,address(p),v);data[r]=v;}
static void store(unsigned r,void *p){u32 a=address(p);event(0x200+r,a,data[r]);memcpy(p,&data[r],4);if((!kind&&r==14&&a==0x1F8000B8U)||(kind&&r==24&&a==0x1F80009CU))mutate();}
static u32 readData(unsigned r){event(0x300+r,0,data[r]);return data[r];}
static void writeControl(unsigned r,u32 v){event(0x500+r,0,v);control[r]=v;}
static void matrixControl(const void *p,unsigned start,unsigned count){u32 v;for(unsigned i=0;i<count;i++){memcpy(&v,(const u8*)p+i*4,4);writeControl(start+i,v);}}
static u32 projected(unsigned vertex){unsigned mode=(scenario+quad)%10;if(mode>=8)mode=vertex&1?5:mode;return (u16)xy[mode][0]|((u32)(u16)xy[mode][1]<<16);}
static void command(u32 code){
 event(0x600,0,code);
 if(code==0x4A4A2012){assert(!kind);quad=quad==~0U?0:quad+1;data[9]=data[0]^control[8]^control[13];data[10]=data[1]^control[9]^control[14];data[11]=(u32)depths[(scenario+quad*3)&7];}
 else if(code==0x4A280030){if(kind)quad=quad==~0U?0:quad+1;for(unsigned i=0;i<3;i++)data[12+i]=projected(i);}
 else if(code==0x4A180001)data[14]=projected(3);
 else if(code==0x4B400006)data[24]=(u32)areas[(scenario+quad*3)%5];
 else {assert(code==0x4B68002E);data[7]=(u32)depths[(scenario+quad*5)&7];}
}
#define RR_GTE_SETROT(p) matrixControl(p,0,5)
#define RR_GTE_SETLLM(p) matrixControl(p,8,5)
#define RR_GTE_SETBK_P(p) matrixControl(p,13,3)
#define RR_GTE_SETTR(p) matrixControl(p,5,3)
#define RR_GTE_MVMVA_LLM_BK_LDV0(p) do {load(0,p);load(1,(const u8*)(p)+4);command(0x4A4A2012);} while(0)
#define RR_GTE_SWC2_IR3(p) store(11,p)
#define RR_GTE_TR_FROM_IR() do {u32 a=readData(9),b=readData(10),c=readData(11);writeControl(5,a);writeControl(6,b);writeControl(7,c);} while(0)
#define RR_GTE_RTPT_LDV(a,b,c) do {load(0,a);load(1,(const u8*)(a)+4);load(2,b);load(3,(const u8*)(b)+4);load(4,c);load(5,(const u8*)(c)+4);command(0x4A280030);} while(0)
#define RR_GTE_RTPS_LDV(p) do {load(0,p);load(1,(const u8*)(p)+4);command(0x4A180001);} while(0)
#define RR_GTE_SWC2_SXYZ(p) do {store(12,p);store(13,(u8*)(p)+4);store(14,(u8*)(p)+8);} while(0)
#define RR_GTE_SWC2_SXY2(p) store(14,p)
#define RR_GTE14_NCLIP() command(0x4B400006)
#define RR_GTE14_SWC2_MAC0(p) store(24,p)
#define RR_GTE14_NOP2() ((void)0)
#define RR_GTE14_AVSZ4() command(0x4B68002E)
#define RR_GTE14_SWC2_OTZ(p) store(7,p)
'''
TAIL = r'''
void run_case(unsigned mode,unsigned number,const u8 *input,const u8 *wanted,u8 *output){
 assert(sizeof(long)==8&&sizeof(void*)==8&&(uintptr_t)&world>UINT32_MAX);
 kind=mode;scenario=number;events=hooks=0;quad=~0U;memset(&world,0xA5,sizeof(world));resetTables();loadState(&world,tables,input);expected=world;loadState(&expected,expectedTables,wanted);
 const RroEffectMesh *mesh=(const RroEffectMesh*)SCENE.modelEntries48+((number>>1)&3);
 u32 stream=address(mesh->commands);streamIndex=kind?(stream-0x80211010U)/172:(stream-0x80210010U)/228;assert(streamIndex<4);
 for(unsigned i=0;i<32;i++){data[i]=number*0x1234567U+i*0x17171717U;control[i]=~(number*0x31415927U+i*0x1234567U);}
 memset(output,0,OUTPUT_SIZE);trace=(u32*)(output+8);
 if(kind)DrawTrackObjectFlashQuads((number>>1)&3);else DrawTrackObjectBillboard((number>>1)&3);
 assert(!memcmp(&world,&expected,sizeof(world)));for(unsigned b=0;b<2;b++)assert(!memcmp(tableMemory[b],expectedTableMemory[b],tableBytes));memcpy(output,&events,4);memcpy(output+4,&hooks,4);u8 *p=output+8+256*12;snapshot(p);p+=STATE_SIZE;memcpy(p,data,128);memcpy(p+128,control,128);
}
'''


def mutate(kind, case, q, state, gpu, mesh, stream):
    get = lambda at: struct.unpack_from('<I', state, at)[0]
    def put(at, v): struct.pack_into('<I', state, at, v&0xFFFFFFFF)
    mask = (case*0x31415927+q)&0xFFFFFFFF
    bank = (case>>3)&1; b = (case+q)&1
    put(2028, MESH+(bank^1)*52); put(mesh, VERTICES+b*128)
    put(mesh+8, (FLASH+((stream+1)&3)*172) if kind else (BILL+((stream+1)&3)*228))
    put(2020, gpu+b*8192+(1400+q*20)*4); put(2024, gpu+(b^1)*8192+(512+q*4)*4)
    put(2032, (case+q+11)%30); put(2036, case*0x1234567+q)
    at = 2040+(q%8)*4; put(at, get(at)^mask)
    at = 2072+((q+3)%8)*4; put(at, get(at)^mask)
    frame = FRAMES[(case+q)%5]; put(2104, frame)
    at = (1008+stream*172+4+q*28) if kind else (96+stream*228+4+q*32)
    for i in range(4):
        index, = struct.unpack_from('<H', state, at+i*2); struct.pack_into('<H', state, at+i*2, (index+3+q)&15)
    for i in range(3): put(at+12+i*4, get(at+12+i*4)^mask)
    state[at+24] = (((case>>2)+q*7) if case&4096 else (case*5+q*7))&255; state[at+25] = ((case>>2)+q*37)&255
    if not kind:
        put(at+32+28, get(at+32+28)^mask)
        z, = struct.unpack_from('<H', state, at+32+26); struct.pack_into('<H', state, at+32+26, (z+case+q+1)&65535)
    phase = signed(state[at+24], 8)
    index = ((frame+phase) if kind else (frame-(phase-64)))&63
    put(2108+kind*256+index*4, (case*0x1234567+q*0x17171717)^0x2E000000)


def oracle(kind, case, incoming, gpu):
    state = bytearray(incoming); events = []; hooks = 0
    data = [(case*0x1234567+r*0x17171717)&0xFFFFFFFF for r in range(32)]
    control = [~(case*0x31415927+r*0x1234567)&0xFFFFFFFF for r in range(32)]
    get = lambda at: struct.unpack_from('<I', state, at)[0]
    def put(at, v): struct.pack_into('<I', state, at, v&0xFFFFFFFF)
    def event(k, a, v): events.extend((k, a, v&0xFFFFFFFF))
    def ctrl(r, v): control[r] = v&0xFFFFFFFF; event(0x500+r, 0, v)
    def load(r, a, at): data[r] = get(at); event(0x100+r, a, data[r])
    def store(r, a, at): event(0x200+r, a, data[r]); put(at, data[r])
    def command(code): event(0x600, 0, code)
    def rd(r): event(0x300+r, 0, data[r]); return data[r]
    if not kind:
        for i, value in ((4, 16384), (2, 16384), (0, 16384), (3, 0), (1, 0)): put(1952+i*4, value)
    for start, at, count in (((0, 2040, 5), (5, 2060, 3)) if kind else ((0, 1952, 5), (8, 2040, 5), (13, 2092, 3))):
        for i in range(count): ctrl(start+i, get(at+i*4))
    mesh = ((get(2028)-MESH)//52)*48+((case>>1)&3)*12
    vertices, _, stream_pointer = struct.unpack_from('<3I', state, mesh)
    stream = (stream_pointer-(FLASH if kind else BILL))//(172 if kind else 228)
    start = (1008+stream*172) if kind else (96+stream*228)
    count = struct.unpack_from('<H', state, start+2)[0]
    packet, ordering, shift = get(2020), get(2024), get(2032)+2
    winding = 0x80000000 if get(2036) else 0
    first_head = None; paths, biases, aliases, phases, uv_words = set(), set(), set(), set(), set()
    def stage(at):
        put(1952+44, get(at+28)); state[1952+48:1952+50] = state[at+26:at+28]
    if not kind: stage(start+4)
    for q in range(count):
        at = start+4+q*(28 if kind else 32)
        if not kind:
            load(0, WORK+44, 1996); load(1, WORK+48, 2000); command(0x4A4A2012)
            data[9] = data[0]^control[8]^control[13]; data[10] = data[1]^control[9]^control[14]; data[11] = DEPTHS[(case+q*3)&7]&0xFFFFFFFF
            stage(at+32); store(11, WORK+40, 1992)
            depth = signed(data[11], 32)
            if depth < 0: paths.add('negative'); continue
            depth >>= shift
            if depth >= 448: paths.add('far'); continue
            ir = [rd(r) for r in (9, 10, 11)]
            for r, value in zip((5, 6, 7), ir): ctrl(r, value)
        indices = struct.unpack_from('<4H', state, at)
        for r, index in enumerate(indices[:3]):
            load(r*2, vertices+index*8, 1696+vertices-VERTICES+index*8)
            load(r*2+1, vertices+index*8+4, 1700+vertices-VERTICES+index*8)
        command(0x4A280030)
        mode = (case+q)%10; xy = [XY[(5 if v&1 else mode) if mode>=8 else mode] for v in range(4)]
        packed = [(x&65535)|((y&65535)<<16) for x, y in xy]; data[12:15] = packed[:3]
        target = PREFIX_SIZE+packet-gpu
        values = [get(at+12+i*4) for i in range(3)]
        for i, value in enumerate(values): put(target+12+i*8, value)
        put(target+36, signed(values[2], 32)>>16)
        projected = 1952+(36 if kind else 52)
        if kind: command(0x4B400006); data[24] = AREAS[(case+q*3)%5]&0xFFFFFFFF
        for i in range(3): store(12+i, WORK+projected-1952+i*4, projected+i*4)
        if kind: store(24, WORK+32, 1984)
        mutate(kind, case, q, state, gpu, mesh, stream); hooks += 1
        if kind and signed(get(1984)^winding, 32) < 0: paths.add('backface'); continue
        fourth = struct.unpack_from('<H', state, at+6)[0] if kind else indices[3]
        load(0, vertices+fourth*8, 1696+vertices-VERTICES+fourth*8)
        load(1, vertices+fourth*8+4, 1700+vertices-VERTICES+fourth*8); command(0x4A180001); data[14] = packed[3]
        phase = signed(state[at+24], 8); frame = get(2104)
        color_index = ((frame+phase) if kind else (frame-(phase-64)))&63
        for i in range(3): put(target+8+i*8, get(projected+i*4))
        put(target+4, get(2108+kind*256+color_index*4))
        if kind: command(0x4B68002E); data[7] = DEPTHS[(case+q*5)&7]&0xFFFFFFFF
        store(14, WORK+projected-1952+12, projected+12)
        if kind: store(7, WORK+28, 1980)
        put(target+32, get(projected+12))
        if kind:
            depth = signed(get(1980), 32)
            if depth <= 0: paths.add('nonpositive'); continue
            depth >>= shift; put(1980, depth)
            if depth >= 448: paths.add('far'); continue
        if not any(x >= 0 for x, _ in xy) or not any(x < 320 for x, _ in xy): paths.add('clip_x'); continue
        if not any(y >= 0 for _, y in xy) or not any(y < 256 for _, y in xy): paths.add('clip_y'); continue
        bias = signed(state[at+25], 8); head = ordering+512+(depth+bias)*4
        if first_head is None: first_head = head
        if packet <= head < packet+40: aliases.add((head-packet)//4)
        head_at = PREFIX_SIZE+head-gpu; put(target, get(head_at)); state[target+3] = 9; put(head_at, packet); packet += 40; state[head_at+3] = 0
        paths.add('emitted'); biases.add(bias); phases.add(phase); uv_words.update(values)
    put(2020, packet)
    output = struct.pack('<II', len(events)//3, hooks)+struct.pack('<'+'I'*len(events), *events)+bytes(MAX_EVENTS*12-len(events)*4)+state+struct.pack('<64I', *data, *control)
    return bytes(state), output, dict(head=first_head, paths=paths, aliases=aliases, biases=biases, phases=phases, uv_words=uv_words, count=count, hooks=hooks)


def main():
    os.chdir(ROOT); names = ('DrawTrackObjectBillboard', 'DrawTrackObjectFlashQuads'); owners = source_owners(ROOT/'src/main/PAL')
    bodies = [re.sub(r'(?:__asm__|asm)\("\$\d+"\)', '', definition_source(owners[n].read_text(), n)) for n in names]
    with TemporaryDirectory() as directory:
        source, library = Path(directory)/'native.c', Path(directory)/'native.dylib'
        source.write_text(PREFIX+'\n'.join(bodies)+TAIL.replace('OUTPUT_SIZE', str(OUTPUT)).replace('STATE_SIZE', str(STATE)))
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-Iinclude', str(source), '-o', str(library)], check=True)
        native = C.CDLL(str(library)); native.run_case.argtypes = [C.c_uint, C.c_uint, C.c_void_p, C.c_void_p, C.c_void_p]
        native.gpu_address.restype = C.c_uint32; gpu = native.gpu_address()
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN,); memory.mem_map(0, 0x400000); memory.mem_map(0x1000000, 0x1000000); memory.mem_map(0x1F800000, 4096)
        write = lambda a, b: memory.mem_write(a&0x1FFFFFFF, bytes(b)); read = lambda a, n: bytes(memory.mem_read(a&0x1FFFFFFF, n))
        put = lambda a, v: write(a, struct.pack('<I', v&0xFFFFFFFF)); get = lambda a: int.from_bytes(read(a, 4), 'little')
        def write_tables(state):
            for bank in range(2):
                write(MESH+bank*52-4, state[2620+bank*4:2624+bank*4])
                write(MESH+bank*52, state[bank*48:(bank+1)*48])
        executable = (ROOT/'assets/PAL/main.exe').read_bytes(); base, = struct.unpack_from('<I', executable, 0x18); write(base, executable[0x800:])
        if os.getenv('RR_COMPILED_ELF'):
            with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                elf = ELFFile(stream)
                for name in names:
                    symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(name)[0]; section = elf.get_section(symbol['st_shndx']); offset = symbol['st_value']-section['sh_addr']
                    assert symbol['st_value'] == address_of(name); write(symbol['st_value'], section.data()[offset:offset+symbol['st_size']])
        def check_store(cpu, access, address, size, value, user):
            address &= 0x1FFFFFFF
            assert any((a&0x1FFFFFFF) <= address and address+size <= (a&0x1FFFFFFF)+n for a, n in [(WORK, 68), (0x1F800000, 4), (gpu, GPU_SIZE), (STACK-256, 256)]), (hex(address), size)
        cpu = Cpu(memory, 0, 100, 101, 102, check_store)
        camera = address_of('CameraViewMatrix'); frame_address = address_of('ScreenFrame'); palettes = [address_of('TileFadeColorTable'), address_of('D_8009E36C')]
        for kind, name in enumerate(names):
            paths, biases, phases, aliases, shifts, counts, uv_words = (set() for _ in range(7))
            for case in range(CASES):
                state = bytearray((case*19+i*29)&255 for i in range(STATE)); bank = (case>>3)&1
                for b in range(2):
                    for i in range(4): struct.pack_into('<3I', state, (b*4+i)*12, VERTICES+((b+i)&1)*128, 16, (FLASH+i*172) if kind else (BILL+i*228))
                for stream in range(4):
                    for form, start, stride, records in ((0, 96+stream*228, 32, 7), (1, 1008+stream*172, 28, 6)):
                        struct.pack_into('<I', state, start, 3|(((case+stream)%7)<<16))
                        for i in range(records):
                            at = start+4+i*stride; struct.pack_into('<4H', state, at, *[(case+stream+i+k)&15 for k in range(4)])
                            struct.pack_into('<3I', state, at+12, *[UVS[((case>>4)+i+k)&7] for k in range(3)])
                order_bank = (case>>6)&1; packet = gpu+(order_bank^1)*8192+900*4; ordering = gpu+order_bank*8192+128*4; shift = (case>>5)%30
                struct.pack_into('<5I', state, 2020, packet, ordering, MESH+bank*52, shift, (case>>4)&1); struct.pack_into('<I', state, 2104, FRAMES[(case>>2)%5])
                struct.pack_into('<2I', state, 2620, 4, 4)
                struct.pack_into('<4096I', state, PREFIX_SIZE, *[(case*0x31415927+i*0x17171717)&0xFFFFFFFF for i in range(4096)])
                if case&2048:
                    _, _, meta = oracle(kind, case, state, gpu)
                    if meta['head'] is not None: struct.pack_into('<I', state, 2020, meta['head']-((case>>3)%10)*4)
                reference, expected, meta = oracle(kind, case, state, gpu)
                incoming, wanted = C.create_string_buffer(bytes(state)), C.create_string_buffer(reference); output = (C.c_uint32*((OUTPUT+3)//4))(); native.run_case(kind, case, incoming, wanted, output)
                assert bytes(output)[:OUTPUT] == expected, (name, case, 'native/oracle transfer trace, cached/live reads and complete owners')
                write_tables(state); write(BILL, state[96:1008]); write(FLASH, state[1008:1696]); write(VERTICES, state[1696:1952]); write(WORK, state[1952:2020]); write(0x1F800000, state[2020:2028]); write(0x1F800048, state[2028:2032]); write(0x1F800060, state[2032:2040]); write(0x1F800028, state[2040:2072]); write(camera, state[2072:2104]); write(frame_address, state[2104:2108]); write(palettes[0], state[2108:2364]); write(palettes[1], state[2364:2620]); write(gpu, state[PREFIX_SIZE:])
                def snapshot():
                    return read(MESH, 48)+read(MESH+52, 48)+read(BILL, 912)+read(FLASH, 688)+read(VERTICES, 256)+read(WORK, 68)+read(0x1F800000, 8)+read(0x1F800048, 4)+read(0x1F800060, 8)+read(0x1F800028, 32)+read(camera, 32)+read(frame_address, 4)+read(palettes[0], 256)+read(palettes[1], 256)+read(MESH-4, 4)+read(MESH+48, 4)+read(gpu, GPU_SIZE)
                cpu.reset(); cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]; cpu.r[4] = (case>>1)&3; cpu.r[29] = STACK; cpu.r[31] = STOP; cpu.pc = address_of(name)
                data = [(case*0x1234567+r*0x17171717)&0xFFFFFFFF for r in range(32)]; control = [~(case*0x31415927+r*0x1234567)&0xFFFFFFFF for r in range(32)]; events = []; q = -1; hooks = 0
                mesh = MESH+bank*52+((case>>1)&3)*12; selected_stream = (get(mesh+8)-(FLASH if kind else BILL))//(172 if kind else 228)
                for _ in range(10000):
                    if cpu.pc == STOP: break
                    instruction = get(cpu.pc); opcode = instruction>>26
                    if opcode not in (0x12, 0x32, 0x3A): cpu.step(); continue
                    previous = cpu.delay; cpu.delay = None; rs, rt, rd = (instruction>>21)&31, (instruction>>16)&31, (instruction>>11)&31
                    if opcode in (0x32, 0x3A):
                        a = (cpu.r[rs]+signed(instruction, 16))&0xFFFFFFFF
                        if opcode == 0x32: data[rt] = get(a); events.extend([0x100+rt, a, data[rt]])
                        else:
                            events.extend([0x200+rt, a, data[rt]]); cpu.store(a, data[rt], 4)
                            if (not kind and rt==14 and a==WORK+60) or (kind and rt==24 and a==WORK+32):
                                hooked = bytearray(snapshot()); mutate(kind, case, q, hooked, gpu, bank*48+((case>>1)&3)*12, selected_stream); hooks += 1
                                write_tables(hooked); write(BILL, hooked[96:1008]); write(FLASH, hooked[1008:1696]); write(0x1F800000, hooked[2020:2028]); write(0x1F800048, hooked[2028:2032]); write(0x1F800060, hooked[2032:2040]); write(0x1F800028, hooked[2040:2072]); write(camera, hooked[2072:2104]); write(frame_address, hooked[2104:2108]); write(palettes[0], hooked[2108:2364]); write(palettes[1], hooked[2364:2620])
                    elif instruction&0x02000000:
                        events.extend([0x600, 0, instruction])
                        if instruction==0x4A4A2012:
                            assert not kind; q += 1; data[9] = data[0]^control[8]^control[13]; data[10] = data[1]^control[9]^control[14]; data[11] = DEPTHS[(case+q*3)&7]&0xFFFFFFFF
                        elif instruction==0x4A280030:
                            if kind: q += 1
                            mode = (case+q)%10; xy = [XY[(5 if v&1 else mode) if mode>=8 else mode] for v in range(4)]; data[12:15] = [(x&65535)|((y&65535)<<16) for x, y in xy[:3]]
                        elif instruction==0x4A180001: x,y=xy[3];data[14]=(x&65535)|((y&65535)<<16)
                        elif instruction==0x4B400006: data[24] = AREAS[(case+q*3)%5]&0xFFFFFFFF
                        else: assert instruction==0x4B68002E;data[7]=DEPTHS[(case+q*5)&7]&0xFFFFFFFF
                    elif rs==0: cpu.r[rt]=data[rd];events.extend([0x300+rd,0,data[rd]])
                    else: assert rs==6;control[rd]=cpu.r[rt];events.extend([0x500+rd,0,control[rd]])
                    cpu.pc=previous if previous is not None else cpu.pc+4
                assert cpu.pc==STOP and q+1==meta['count'] and hooks==meta['hooks']
                actual=struct.pack('<II',len(events)//3,hooks)+struct.pack('<'+'I'*len(events),*events)+bytes(MAX_EVENTS*12-len(events)*4)+snapshot()+struct.pack('<64I',*data,*control)
                assert actual==expected,(name,case,'MIPS/oracle transfers, prefetch, captured roots, live fields, partial GPU packets and complete owners')
                for r in (*range(16,24),28,30): assert cpu.r[r]==0xBEEF0000+r
                assert cpu.r[29]==STACK and cpu.r[31]==STOP
                paths|=meta['paths'];biases|=meta['biases'];phases|=meta['phases'];aliases|=meta['aliases'];uv_words|=meta['uv_words'];shifts.add(shift);counts.add(meta['count'])
            required={'emitted','far','clip_x','clip_y'}|({'backface','nonpositive'} if kind else {'negative'})
            assert paths==required,(name,paths)
            assert aliases==set(range(10)) and shifts==set(range(30)) and counts==set(range(7)),(name,aliases)
            assert biases==set(range(-128,128)) and phases==set(range(-128,128)) and uv_words==set(UVS),(name,len(biases),len(phases),uv_words)
            print('PASS',CASES,'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',name,'MIPS/native cases; full owners/pointers, all packet/head aliases, captured roots, live indices/phases/palettes, prefetch, partial culled packets, wrapping frames/biases/winding, ABI, UBSan; scripted GTE math',flush=True)


if __name__=='__main__':
    main()
