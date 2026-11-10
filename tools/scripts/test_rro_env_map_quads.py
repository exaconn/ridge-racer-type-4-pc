"""Actual envmap emitter, native DMA backend, original MIPS and a wire oracle.

Scripted GTE arithmetic isolates CPU transfers, captured/live inputs, tagged DMA
addresses, direct/masked packet layouts, lookahead writes and overlapping heads.
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
from test_rro_raw_textured_quads import COUNTS, DEPTHS, AREAS, projected

ROOT = Path(__file__).resolve().parents[2]
STREAM, VERTICES, NORMALS, WORK = 0x80200010, 0x80210010, 0x80220010, 0x1F80007C
GPU, PREVIOUS, FRAME0, FRAME1 = 0x80180000, 0x801A0000, 0x80230010, 0x80231010
STACK, STOP = 0x803F0000, 0x803FF000
CASES, PREFIX_SIZE, GPU_SIZE, MAX_EVENTS = 8192 + 256, 1416, 16384, 768
STATE = PREFIX_SIZE + GPU_SIZE
OUTPUT = 12 + MAX_EVENTS * 12 + STATE + 128
MASKS = (0, 1, 2, 3, 0x80000000, 0xFFFFFFFF, 0x12345678)
CONTROLS = (0x78, 0x7C, 0, 0xFFFC, 0x79, 0x7A, 0x7B, 0xFFFF)
IR = (-2147483648, -32768, -16385, -12289, -1, 0, 1, 16383,
      16384, 32767, 2147483647, -64, 63, 64, -128, 127)
ENV_HEAD_OFFSETS = (0, 56, 72, 84, 96, 108, 112)
PREFIX = r'''
#include <assert.h>
#include "rr/frame_storage_types.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "rr/car_render_types.h"
#include "rr/rro_model.h"
#include "rr/fade_sprite.h"
#include "rr/gpu_address.h"
typedef union {u32 words[56];RroEnvMapQuad quads[7];} QuadStream;
typedef union {u32 words[4096];GpuEnvMapEmission packets[136];} GpuArena;
static struct World {
 u8 before[16];RroModelScratch scratch;QuadStream stream;
 Vec3s8 vertices[2][16],normals[2][16];s32 mask;
 FrameStorage frames[2],*currentFrame;
 u32 previous[64];GpuArena gpu;u8 after[16];
} world,expected;
static GpuDmaRegion regions[2];
#define SCRATCHPAD ((u8*)&world.scratch)
#define WORK world.scratch.work7C.render
#define SCENE world.scratch.scene
#define MaskBitSetting world.mask
#define CurrentDrawBuf world.currentFrame
static unsigned scenario,biasSeed,events,hooks,q,clips,geometryBank,normalBank;
static unsigned lightOrdinal,normalSlot,envOrdinal,rgbGroups;
static u8 *activePacket;
static u32 data[32],*trace;
static const s32 depths[]={-2147483647-1,-1,0,1,447,448,1791,1792};
static const s32 areas[]={0,1,-1,-2147483647-1,2147483647};
static const u32 masks[]={0,1,2,3,0x80000000U,0xFFFFFFFFU,0x12345678U};
static const u16 controls[]={0x78,0x7C,0,0xFFFC,0x79,0x7A,0x7B,0xFFFF};
static const s32 ir[]={-2147483647-1,-32768,-16385,-12289,-1,0,1,16383,16384,32767,2147483647,-64,63,64,-128,127};
static const unsigned headOffsets[]={0,56,72,84,96,108,112};
static const s16 xy[][2]={{0,0},{-1,20},{320,20},{10,-1},{10,512},{319,511},{-32768,-32768},{32767,32767},{-1,-1},{320,512}};
static u32 address(const void *pointer){
 uintptr_t p=(uintptr_t)pointer,b=(uintptr_t)&world.gpu;
 if(p>=b&&p-b<sizeof(world.gpu))return 0x80180000U+(u32)(p-b);
 b=(uintptr_t)world.vertices;if(p>=b&&p-b<sizeof(world.vertices))return 0x80210010U+(u32)(p-b);
 b=(uintptr_t)world.normals;if(p>=b&&p-b<sizeof(world.normals))return 0x80220010U+(u32)(p-b);
 b=(uintptr_t)&world.stream;if(p>=b&&p-b<sizeof(world.stream))return 0x80200010U+(u32)(p-b);
 b=(uintptr_t)&WORK.depth18;if(p>=b&&p-b<72)return 0x1F800094U+(u32)(p-b);
 for(unsigned i=0;i<2;i++)if(pointer==&world.frames[i])return i?0x80231010U:0x80230010U;
 assert(!"unmapped or truncated native pointer");return 0;
}
static void *resolve(u32 a){
 u32 offset=a-0x80180000U;if(offset<sizeof(world.gpu))return (u8*)&world.gpu+offset;
 offset=a-0x80210010U;if(offset<sizeof(world.vertices))return (u8*)world.vertices+offset;
 offset=a-0x80220010U;if(offset<sizeof(world.normals))return (u8*)world.normals+offset;
 if(a==0x80230010U)return &world.frames[0];if(a==0x80231010U)return &world.frames[1];
 assert(!"invalid wire pointer");return 0;
}
static u32 get(const u8 *p){u32 v;memcpy(&v,p,4);return v;}
static void put(u8 *p,u32 v){memcpy(p,&v,4);}
static void loadState(struct World *w,const u8 *p){
 memcpy(&w->stream,p,224);memcpy(w->vertices,p+224,256);memcpy(w->normals,p+480,256);p+=736;
 RroRenderWork *work=&w->scratch.work7C.render;
 work->vertices00=resolve(get(p));work->normals04=resolve(get(p+4));
 memcpy(&work->flags08,p+8,12);work->ordering14=resolve(get(p+20));
 memcpy(&work->depth18,p+24,16);memcpy(work->vertices28,p+40,32);memcpy(&work->normal48,p+72,8);memcpy(work->projected50,p+80,16);
 w->scratch.scene.view.projection.pointers.cursor=resolve(get(p+96));w->scratch.scene.depthShift60=(s32)get(p+100);w->mask=(s32)get(p+104);
 for(unsigned bank=0;bank<2;bank++)memcpy(&w->frames[bank],p+108+bank*156,156);w->currentFrame=resolve(get(p+420));memcpy(w->previous,p+424,256);memcpy(&w->gpu,p+680,16384);
}
static void snapshot(u8 *p){
 memcpy(p,&world.stream,224);memcpy(p+224,world.vertices,256);memcpy(p+480,world.normals,256);p+=736;
 put(p,address(WORK.vertices00));put(p+4,address(WORK.normals04));memcpy(p+8,&WORK.flags08,12);put(p+20,address(WORK.ordering14));
 memcpy(p+24,&WORK.depth18,16);memcpy(p+40,WORK.vertices28,32);memcpy(p+72,&WORK.normal48,8);memcpy(p+80,WORK.projected50,16);
 put(p+96,address(SCENE.view.projection.pointers.cursor));put(p+100,(u32)SCENE.depthShift60);put(p+104,(u32)world.mask);
 for(unsigned bank=0;bank<2;bank++)memcpy(p+108+bank*156,&world.frames[bank],156);put(p+420,address(world.currentFrame));memcpy(p+424,world.previous,256);memcpy(p+680,&world.gpu,16384);
}
static void mutate(unsigned phase){
 hooks++;u32 mask=scenario*0x31415927U+q*0x17171717U+phase;
 RroEnvMapQuad *current=&world.stream.quads[q];
 for(unsigned i=0;i<4;i++)current->normals.indices[i]=(current->normals.indices[i]+phase+q)&15;
 current->uv10=(s32)((u32)current->uv10^mask);current->uv14=(s32)((u32)current->uv14^mask);current->uv18=(s32)((u32)current->uv18^mask);
 current->orderingBias1D=(s8)(u8)(biasSeed+q*37+phase);WORK.flags08=(s32)((u32)WORK.flags08^mask);
 world.mask=(s32)masks[(scenario+q+phase)%7];
 if(phase==1){
  for(unsigned i=0;i<4;i++)current[1].vertices.indices[i]=(current[1].vertices.indices[i]+3+q)&15;
  for(unsigned i=0;i<16;i++)for(unsigned j=0;j<2;j++)world.vertices[geometryBank][i].words[j]^=mask+i*0x1234567U+j;
  WORK.vertices00=world.vertices[geometryBank^1];
  SCENE.view.projection.pointers.cursor=(u8*)&world.gpu.words[((scenario+q)&1)*2048+1400+q*20];SCENE.depthShift60=(scenario+q+11)&31;
  world.currentFrame=&world.frames[(scenario+q)&1];world.frames[0].buffer.environment00.draw00.clip.h=256;world.frames[1].buffer.environment00.draw00.clip.h=480;
 }
 if(phase<=2)WORK.normals04=world.normals[(scenario+q+phase)&1];
 if(phase==3){
  for(unsigned i=0;i<16;i++)for(unsigned j=0;j<2;j++)world.normals[normalBank][i].words[j]^=mask+i*0x1234567U+j;
  WORK.normals04=world.normals[normalBank^1];
 }
 if(phase==8){
  current->control1E=controls[((scenario>>4)+q)&7];WORK.depth18=(scenario+q)%448;
  u8 *head;
  if(scenario&2048){unsigned offset=(current->control1E&3)?headOffsets[((scenario>>3)+q)%7]:(((scenario>>3)+q)%14)*4;head=activePacket+offset;}
  else {unsigned bank=(unsigned)((activePacket-(u8*)&world.gpu)/8192);head=(u8*)&world.gpu.words[(bank^1)*2048+768+q*16];}
  WORK.ordering14=(u32*)(head-4*(WORK.depth18+current->orderingBias1D));
 }
 if(phase==9){
  for(unsigned i=0;i<16;i++)for(unsigned j=0;j<4;j++)((u8*)world.previous)[i*16+4+j]=((scenario+q+i+j)&1)?0xE6:0x17;
  WORK.envClut0C^=mask;WORK.envTpage10^=mask;
 }
 if(phase==10)WORK.envTpage10^=mask;
}
static void event(u32 kind,u32 target,u32 value){assert(events++<768);*trace++=kind;*trace++=target;*trace++=value;}
static void load(unsigned r,const void *p){u32 v;memcpy(&v,p,4);event(0x100+r,address(p),v);data[r]=v;}
static void store(unsigned r,void *p){u32 a=address(p);event(0x200+r,a,data[r]);memcpy(p,&data[r],4);if(a==0x1F800098U)mutate(1);if(a==0x1F80009CU)mutate(2);}
static u32 readRegister(unsigned r){event(0x300+r,0,data[r]);return data[r];}
static u32 projected(unsigned vertex){unsigned mode=(scenario+q)%12;if(mode==10)mode=vertex&1?9:8;if(mode==11)mode=vertex&1?7:0;return (u16)xy[mode][0]|((u32)(u16)xy[mode][1]<<16);}
static void command(u32 code){
 event(0x600,0,code);
 if(code==0x4A280030){q=q==~0U?0:q+1;clips=lightOrdinal=envOrdinal=rgbGroups=0;for(unsigned i=0;i<3;i++)data[12+i]=projected(i);}
 else if(code==0x4A180001)data[14]=projected(3);
 else if(code==0x4B400006)data[24]=(u32)areas[(scenario+q*3+clips++*2)%5];
 else if(code==0x4B68002E)data[7]=(u32)depths[(scenario+q*5)&7];
 else if(code==0x4A4A6412){normalSlot=0;if(lightOrdinal==0)mutate(3);}
 else if(code==0x4A4AE412){normalSlot=1;mutate(5);}
 else if(code==0x4A4B6412){normalSlot=2;mutate(7);}
 else if(code==0x4B38041C){
  u32 bits=data[normalSlot*2]^(data[normalSlot*2+1]*0x9E3779B9U)^data[6]^(scenario*0x1234567U+q*0x17171717U+lightOrdinal*0x31415927U);
  data[20]=data[21];data[21]=data[22];data[22]=(data[6]&0xFF000000U)|(bits&0xFFFFFFU);
  if(lightOrdinal==0)mutate(4);if(lightOrdinal==1)mutate(6);lightOrdinal++;
 }else if(code==0x4A486012||code==0x4A48E012||code==0x4A496012){
  const u32 codes[]={0x4A486012,0x4A48E012,0x4A496012,0x4A486012};assert(envOrdinal<4&&code==codes[envOrdinal]);
  data[9]=(u32)ir[(scenario+q*3+envOrdinal*5)&15];data[10]=(u32)ir[(scenario*3+q+envOrdinal*7)&15];
  if(envOrdinal==0)mutate(9);if(envOrdinal==2)mutate(10);envOrdinal++;
 }else assert(code==0x4AA80428);
}
static void normalTriplet(const void *a,const void *b,const void *c){
 u32 first=address(a);assert(first>=0x80220010U&&first<0x80220110U);normalBank=(first-0x80220010U)/128;
 load(0,a);load(1,(const u8*)a+4);load(2,b);load(3,(const u8*)b+4);load(4,c);load(5,(const u8*)c+4);
}
static void rgbTriplet(void *p){
 if(rgbGroups++==0)activePacket=(u8*)p-4;
 store(20,(u8*)p+4);store(21,(u8*)p+16);store(22,(u8*)p+28);
 if(rgbGroups==1)mutate(8);
}
#define RR_GTE14_LWC2_V012(p) do{for(unsigned i=0;i<6;i++)load(i,(const u8*)(p)+i*4);}while(0)
#define RR_GTE14_LWC2_V0(p) do{load(0,p);load(1,(const u8*)(p)+4);}while(0)
#define RR_GTE14_LWC2_V0_V1_V2(a,b,c) normalTriplet((const void*)(a),(const void*)(b),(const void*)(c))
#define RR_GTE14_NOP2() ((void)0)
#define RR_NOP_DEP() ((void)0)
#define RR_GTE14_RTPT() command(0x4A280030)
#define RR_GTE14_RTPS() command(0x4A180001)
#define RR_GTE14_NCLIP() command(0x4B400006)
#define RR_GTE14_AVSZ4() command(0x4B68002E)
#define RR_GTE14_SWC2_MAC0(p) store(24,(void*)(p))
#define RR_GTE14_SWC2_OTZ(p) store(7,p)
#define RR_GTE14_SWC2_SXYZ(p) do{store(12,p);store(13,(u8*)(p)+4);store(14,(u8*)(p)+8);}while(0)
#define RR_GTE_MFC2_SXY2_NOP(out) ((out)=(s32)readRegister(14))
#define RR_GTE_MFC2_IR1_IR2(x,y) do{(x)=(s32)readRegister(9);(y)=(s32)readRegister(10);}while(0)
#define RR_GTE14_LIGHT0() do{command(0x4A4A6412);command(0x4AA80428);command(0x4AA80428);command(0x4B38041C);}while(0)
#define RR_GTE_OP(code) command(code)
#define RR_GTE_SQR12() command(0x4AA80428)
#define RR_GTE_SWC2_RGB012_ENV(p) rgbTriplet(p)
#define RR_GTE14_SWC2_RGB2(p) store(22,p)
'''
TAIL = r'''
void run_case(unsigned number,unsigned bias,s32 count,const u8 *input,const u8 *wanted,u8 *output){
 assert(sizeof(long)==8&&sizeof(void*)==8&&(uintptr_t)&world>UINT32_MAX);
 assert((u8*)&WORK.projected50-(u8*)&WORK.depth18==56);
 scenario=number;biasSeed=bias;geometryBank=(scenario>>1)&1;events=hooks=0;q=~0U;
 memset(&world,0xA5,sizeof(world));loadState(&world,input);expected=world;loadState(&expected,wanted);
 regions[0]=(GpuDmaRegion){&world.gpu,0x80180000U,sizeof(world.gpu)};
 regions[1]=(GpuDmaRegion){world.previous,0x801A0000U,sizeof(world.previous)};assert(GpuDmaSetRegions(regions,2));
 for(unsigned i=0;i<32;i++)data[i]=number*0x1234567U+i*0x17171717U;
 data[6]=0x3C000000U|((number*0x1234567U)&0xFFFFFFU);
 memset(output,0,OUTPUT_SIZE);trace=(u32*)(output+12);
 const u32 *result=EmitRroEnvMapQuads(world.stream.words,&WORK,count);
 assert(!memcmp(&world,&expected,sizeof(world)));
 put(output,address(result));put(output+4,events);put(output+8,hooks);
 snapshot(output+12+768*12);memcpy(output+12+768*12+STATE_SIZE,data,128);
}
'''


def mutate(case, q, phase, state, bias_seed, normal_bank=0, packet=GPU):
    get = lambda at: struct.unpack_from('<I', state, at)[0]
    def put(at, value): struct.pack_into('<I', state, at, value & 0xFFFFFFFF)
    mask = (case * 0x31415927 + q * 0x17171717 + phase) & 0xFFFFFFFF
    at = q * 32
    for i in range(4):
        index, = struct.unpack_from('<H', state, at + 8 + i * 2)
        struct.pack_into('<H', state, at + 8 + i * 2, (index + phase + q) & 15)
    for offset in (16, 20, 24): put(at + offset, get(at + offset) ^ mask)
    state[at + 29] = (bias_seed + q * 37 + phase) & 255
    put(744, get(744) ^ mask); put(840, MASKS[(case + q + phase) % 7])
    if phase == 1:
        for i in range(4):
            index, = struct.unpack_from('<H', state, at + 32 + i * 2)
            struct.pack_into('<H', state, at + 32 + i * 2, (index + 3 + q) & 15)
        bank = (case >> 1) & 1
        for i in range(16):
            for j in range(2):
                offset = 224 + bank * 128 + i * 8 + j * 4
                put(offset, get(offset) ^ ((mask + i * 0x1234567 + j) & 0xFFFFFFFF))
        put(736, VERTICES + (bank ^ 1) * 128)
        put(832, GPU + ((case + q) & 1) * 8192 + (1400 + q * 20) * 4); put(836, (case + q + 11) & 31)
        put(1156, FRAME1 if (case + q) & 1 else FRAME0)
        struct.pack_into('<h', state, 844 + 6, 256); struct.pack_into('<h', state, 1000 + 6, 480)
    if phase <= 2: put(740, NORMALS + ((case + q + phase) & 1) * 128)
    if phase == 3:
        for i in range(16):
            for j in range(2):
                offset = 480 + normal_bank * 128 + i * 8 + j * 4
                put(offset, get(offset) ^ ((mask + i * 0x1234567 + j) & 0xFFFFFFFF))
        put(740, NORMALS + (normal_bank ^ 1) * 128)
    if phase == 8:
        control = CONTROLS[((case >> 4) + q) & 7]; struct.pack_into('<H', state, at + 30, control)
        depth = (case + q) % 448; put(760, depth)
        if case & 2048:
            offset = ENV_HEAD_OFFSETS[((case >> 3) + q) % 7] if control & 3 else (((case >> 3) + q) % 14) * 4
            head = packet + offset
        else: head = GPU + (((packet - GPU) // 8192) ^ 1) * 8192 + (768 + q * 16) * 4
        put(756, head - 4 * (depth + signed(state[at + 29], 8)))
    if phase == 9:
        for i in range(16):
            for j in range(4): state[1160 + i * 16 + 4 + j] = 0xE6 if (case + q + i + j) & 1 else 0x17
        put(748, get(748) ^ mask); put(752, get(752) ^ mask)
    if phase == 10: put(752, get(752) ^ mask)


def oracle(case, count, incoming):
    bias_seed = case >> 2 if case < 8192 else case - 8192
    case = case if case < 8192 else 60
    state = bytearray(incoming); events = []; hooks = 0
    data = [(case * 0x1234567 + i * 0x17171717) & 0xFFFFFFFF for i in range(32)]
    data[6] = 0x3C000000 | ((case * 0x1234567) & 0xFFFFFF)
    get = lambda at: struct.unpack_from('<I', state, at)[0]
    def put(at, value): struct.pack_into('<I', state, at, value & 0xFFFFFFFF)
    def event(kind, target, value): events.extend((kind, target, value & 0xFFFFFFFF))
    def load(r, a, at): data[r] = get(at); event(0x100 + r, a, data[r])
    def store(r, a, at): event(0x200 + r, a, data[r]); put(at, data[r])
    def command(code): event(0x600, 0, code)
    def read_register(r): event(0x300 + r, 0, data[r]); return data[r]
    vertices = 224 + get(736) - VERTICES
    packet, shift, restore = get(832), get(836), get(840) | 0xE6000000
    frame_at = 844 if get(1156) == FRAME0 else 1000
    vshift = 6 if struct.unpack_from('<h', state, frame_at + 6)[0] == 256 else 7
    for i, index in enumerate(struct.unpack_from('<4H', state, 0)):
        state[776 + i * 8:784 + i * 8] = state[vertices + index * 8:vertices + index * 8 + 8]
    paths, direct_aliases, env_aliases, biases, normal_banks, env_irs, controls = (set() for _ in range(7))
    if count <= 0: paths.add('empty')
    for q in range(max(count, 0)):
        def hook(phase, normal_bank=0):
            nonlocal hooks
            mutate(case, q, phase, state, bias_seed, normal_bank, packet); hooks += 1
        for i in range(6): load(i, WORK + 40 + i * 4, 776 + i * 4)
        command(0x4A280030); corners, xy = projected(case, q); data[12:15] = xy[:3]
        indices = struct.unpack_from('<4H', state, (q + 1) * 32)
        def vertex(i):
            at = vertices + indices[i] * 8
            return bytes(state[at:at + 8])
        state[776:784] = vertex(0)
        command(0x4B400006); data[24] = AREAS[(case + q * 3) % 5] & 0xFFFFFFFF
        second = vertex(1)
        store(24, WORK + 28, 764); hook(1)
        for i in range(3): store(12 + i, WORK + 80 + i * 4, 816 + i * 4)
        load(0, WORK + 64, 800); load(1, WORK + 68, 804)
        state[784:792] = second
        command(0x4A180001); data[14] = xy[3]
        state[792:800] = vertex(2)
        command(0x4B68002E); data[7] = DEPTHS[(case + q * 5) & 7] & 0xFFFFFFFF
        fourth = vertex(3)
        store(7, WORK + 24, 760); state[800:808] = fourth
        depth = signed(get(760), 32)
        if depth <= 0: paths.add('nonpositive'); continue
        depth >>= shift; put(760, depth)
        if depth >= 448: paths.add('far'); continue
        command(0x4B400006); data[24] = AREAS[(case + q * 3 + 2) % 5] & 0xFFFFFFFF
        target = PREFIX_SIZE + packet - GPU
        for i in range(3):
            for offset in (12, 68): put(target + offset + i * 12, get(816 + i * 4))
        store(24, WORK + 32, 768); hook(2)
        if signed(get(764), 32) <= 0 and signed(get(768), 32) >= 0: paths.add('winding'); continue
        screen = read_register(14); put(828, screen); put(target + 48, screen); put(target + 104, screen)
        if not any(x >= 0 for x, _ in corners) or not any(x < 320 for x, _ in corners): paths.add('clip_x'); continue
        if not any(y >= 0 for _, y in corners) or not any(y < 512 for _, y in corners): paths.add('clip_y'); continue
        normal_root = get(740); normal_bank = (normal_root - NORMALS) // 128; normal_banks.add(normal_bank)
        normals = struct.unpack_from('<4H', state, q * 32 + 8)
        for i, index in enumerate(normals[:3]):
            load(i * 2, normal_root + index * 8, 480 + normal_root - NORMALS + index * 8)
            load(i * 2 + 1, normal_root + index * 8 + 4, 484 + normal_root - NORMALS + index * 8)
        ordinal = 0
        def start_light(slot):
            command((0x4A4A6412, 0x4A4AE412, 0x4A4B6412)[slot])
            if ordinal == 0: hook(3, normal_bank)
            elif slot == 1: hook(5)
            elif slot == 2: hook(7)
        def finish_light(slot):
            nonlocal ordinal
            for code in (0x4AA80428, 0x4AA80428, 0x4B38041C): command(code)
            bits = data[slot * 2] ^ ((data[slot * 2 + 1] * 0x9E3779B9) & 0xFFFFFFFF) ^ data[6] ^ ((case * 0x1234567 + q * 0x17171717 + ordinal * 0x31415927) & 0xFFFFFFFF)
            data[20:23] = [data[21], data[22], (data[6] & 0xFF000000) | (bits & 0xFFFFFF)]
            if ordinal in (0, 1): hook(4 + ordinal * 2)
            ordinal += 1
        start_light(0)
        at = 480 + normal_root - NORMALS + normals[3] * 8
        state[808:816] = state[at:at + 8]
        finish_light(0)
        uv0, uv1 = get(q * 32 + 16), get(q * 32 + 20)
        start_light(1); put(target + 16, uv0 + get(744)); put(target + 28, uv1); finish_light(1)
        uv2 = get(q * 32 + 24); uv3 = signed(uv2, 32) >> 16
        start_light(2); put(target + 40, uv2); put(target + 52, uv3); finish_light(2)
        for r, offset in ((20, 8), (21, 20), (22, 32)): store(r, packet + offset, target + offset)
        hook(8)
        depth = get(760); bias = signed(state[q * 32 + 29], 8); control, = struct.unpack_from('<H', state, q * 32 + 30)
        head = get(756) + (depth + bias) * 4; head_at = PREFIX_SIZE + head - GPU
        controls.add(control); biases.add(bias)
        if control & 3:
            if packet <= head < packet + 120: env_aliases.add(head - packet)
            put(target + 4, 0xE6000000); put(target + 60, 0xE6000000 | (control & 3))
            for r, offset in ((20, 64), (21, 76), (22, 88)): store(r, packet + offset, target + offset)
            state[target + 67] = 0x3E
            def env(index):
                command((0x4A486012, 0x4A48E012, 0x4A496012, 0x4A486012)[index])
                u = IR[(case + q * 3 + index * 5) & 15]; v = IR[(case * 3 + q + index * 7) & 15]
                data[9], data[10] = u & 0xFFFFFFFF, v & 0xFFFFFFFF; env_irs.update((u, v))
                if index == 0: hook(9)
                if index == 2: hook(10)
            def uv():
                u, v = signed(read_register(9), 32), signed(read_register(10), 32)
                return (((v >> vshift) + 192) << 8) | ((u >> 6) + 192)
            env(0)
            previous = get(head_at); physical = previous & 0x1FFFFFFF
            if (PREVIOUS & 0x1FFFFFFF) <= physical < (PREVIOUS & 0x1FFFFFFF) + 252: payload = 1160 + physical - (PREVIOUS & 0x1FFFFFFF) + 4
            else:
                assert (GPU & 0x1FFFFFFF) <= physical < (GPU & 0x1FFFFFFF) + GPU_SIZE - 4, hex(previous)
                payload = PREFIX_SIZE + physical - (GPU & 0x1FFFFFFF) + 4
            if state[payload] != 0xE6:
                put(target + 116, restore); put(target + 112, get(head_at)); state[target + 115] = 1
                put(head_at, packet + 112); state[head_at + 3] = 0; paths.add('restore')
            else: paths.add('reuse_mask')
            put(target + 56, get(head_at)); state[target + 59] = 13
            put(head_at, packet + 56); state[head_at + 3] = 0
            put(target, get(head_at)); state[target + 3] = 13
            put(head_at, packet); state[head_at + 3] = 0
            put(target + 72, uv() | get(748))
            load(0, WORK + 72, 808); load(1, WORK + 76, 812); start_light(0); finish_light(0)
            store(22, packet + 44, target + 44); store(22, packet + 100, target + 100)
            env(1); u1, v1 = signed(read_register(9), 32), signed(read_register(10), 32)
            env(2); put(target + 84, ((((v1 >> vshift) + 192) << 8) | ((u1 >> 6) + 192)) | get(752))
            uv2 = uv(); env(3); put(target + 96, uv2); put(target + 108, uv()); packet += 120; paths.add('env')
        else:
            if packet <= head < packet + 56: direct_aliases.add((head - packet) // 4)
            load(0, WORK + 72, 808); load(1, WORK + 76, 812); start_light(0); finish_light(0)
            state[target + 11] = (control >> 1) & 255
            put(target + 4, get(head_at)); state[target + 7] = 12
            put(head_at, packet + 4); state[head_at + 3] = 0
            store(22, packet + 44, target + 44); packet += 56; paths.add('direct')
    put(832, packet)
    result = STREAM + max(count, 0) * 32
    output = struct.pack('<3I', result, len(events) // 3, hooks) + struct.pack('<' + 'I' * len(events), *events) + bytes(MAX_EVENTS * 12 - len(events) * 4) + state + struct.pack('<32I', *data)
    return bytes(state), output, dict(paths=paths, direct_aliases=direct_aliases, env_aliases=env_aliases, biases=biases, hooks=hooks, normal_banks=normal_banks, env_irs=env_irs, controls=controls, vshift=vshift)


def main():
    os.chdir(ROOT); name = 'EmitRroEnvMapQuads'
    owner = source_owners(ROOT / 'src/main/PAL')[name]
    body = re.sub(r'(?:__asm__|asm)\("\$\d+"\)', '', definition_source(owner.read_text(), name))
    with TemporaryDirectory() as directory:
        source, library = Path(directory) / 'native.c', Path(directory) / 'native.dylib'
        source.write_text(PREFIX + body + TAIL.replace('OUTPUT_SIZE', str(OUTPUT)).replace('STATE_SIZE', str(STATE)))
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-Iinclude', str(source),
                        str(ROOT / 'src/native/gpu_address.c'), '-o', str(library)], check=True)
        native = C.CDLL(str(library)); native.run_case.argtypes = [C.c_uint, C.c_uint, C.c_int32, C.c_void_p, C.c_void_p, C.c_void_p]
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0, 0x400000); memory.mem_map(0x1F800000, 4096)
        write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b)); read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
        get = lambda a: int.from_bytes(read(a, 4), 'little')
        executable = (ROOT / 'assets/PAL/main.exe').read_bytes(); base, = struct.unpack_from('<I', executable, 0x18); write(base, executable[0x800:])
        if os.getenv('RR_COMPILED_ELF'):
            with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                elf = ELFFile(stream); symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(name)[0]
                section = elf.get_section(symbol['st_shndx']); offset = symbol['st_value'] - section['sh_addr']
                assert symbol['st_value'] == address_of(name); write(symbol['st_value'], section.data()[offset:offset + symbol['st_size']])
        def check_store(cpu, access, address, size, value, user):
            address &= 0x1FFFFFFF
            assert any((a & 0x1FFFFFFF) <= address and address + size <= (a & 0x1FFFFFFF) + n for a, n in [(WORK, 96), (0x1F800000, 4), (GPU, GPU_SIZE), (STACK - 256, 256)]), (hex(address), size)
        cpu = Cpu(memory, 0, 100, 101, 102, check_store)
        mask_address = address_of('MaskBitSetting'); frame_address = address_of('CurrentDrawBuf')
        def snapshot():
            return read(STREAM, 224) + read(VERTICES, 256) + read(NORMALS, 256) + read(WORK, 96) + read(0x1F800000, 4) + read(0x1F800060, 4) + read(mask_address, 4) + read(FRAME0, 156) + read(FRAME1, 156) + read(frame_address, 4) + read(PREVIOUS, 256) + read(GPU, GPU_SIZE)
        def install(state):
            for a, start, end in ((STREAM, 0, 224), (VERTICES, 224, 480), (NORMALS, 480, 736), (WORK, 736, 832), (0x1F800000, 832, 836), (0x1F800060, 836, 840), (mask_address, 840, 844), (FRAME0, 844, 1000), (FRAME1, 1000, 1156), (frame_address, 1156, 1160), (PREVIOUS, 1160, 1416), (GPU, 1416, STATE)): write(a, state[start:end])
        def hook(case, q, phase, bias_seed, normal_bank, packet):
            hooked = bytearray(snapshot()); mutate(case, q, phase, hooked, bias_seed, normal_bank, packet); install(hooked)
        coverage = {key: set() for key in ('paths', 'direct_aliases', 'env_aliases', 'biases', 'normal_banks', 'env_irs', 'controls')}
        shifts, counts, vshifts, masks = (set() for _ in range(4))
        for case in range(CASES):
            flow = case if case < 8192 else 60; bias_seed = case >> 2 if case < 8192 else case - 8192
            state = bytearray((case * 19 + i * 29) & 255 for i in range(STATE)); bank = (flow >> 1) & 1
            for q in range(7):
                struct.pack_into('<4H', state, q * 32, *[(case + q + i) & 15 for i in range(4)])
                struct.pack_into('<4H', state, q * 32 + 8, *[(case * 3 + q + i * 5) & 15 for i in range(4)])
            count = COUNTS[flow % len(COUNTS)]; shift = (flow >> 5) & 31
            struct.pack_into('<2I', state, 736, VERTICES + bank * 128, NORMALS + (bank ^ 1) * 128)
            struct.pack_into('<I', state, 756, GPU + ((case >> 2) & 1) * 8192 + 512 * 4)
            struct.pack_into('<2I', state, 832, GPU + ((case >> 2) & 1) * 8192 + 900 * 4, shift)
            struct.pack_into('<I', state, 840, MASKS[(case >> 4) % 7])
            struct.pack_into('<h', state, 850, (256, 240, 480, -1)[(case >> 2) & 3]); struct.pack_into('<h', state, 1006, (480, 256)[case & 1])
            struct.pack_into('<I', state, 1156, FRAME1 if case & 1 else FRAME0)
            for i in range(4096): struct.pack_into('<I', state, PREFIX_SIZE + i * 4, PREVIOUS + (i % 16) * 16 + ((case + i) & 3))
            reference, expected, meta = oracle(case, count, state)
            incoming, wanted = C.create_string_buffer(bytes(state)), C.create_string_buffer(reference); output = C.create_string_buffer(OUTPUT)
            native.run_case(flow, bias_seed, count, incoming, wanted, output)
            assert output.raw == expected, (case, 'native/wire oracle; full owners and production DMA mapping')
            install(state)
            cpu.reset(); cpu.r = [0] + [0xBEEF0000 + r for r in range(1, 32)]; cpu.r[4:7] = [STREAM, WORK, count & 0xFFFFFFFF]; cpu.r[29] = STACK; cpu.r[31] = STOP; cpu.pc = address_of(name)
            data = [(flow * 0x1234567 + i * 0x17171717) & 0xFFFFFFFF for i in range(32)]; data[6] = 0x3C000000 | ((flow * 0x1234567) & 0xFFFFFF)
            events = []; q = -1; clips = hooks = ordinal = normal_slot = normal_bank = env_ordinal = rgb_groups = 0; packet = GPU; previous_store = None
            def mutate_now(phase):
                nonlocal hooks
                hook(flow, q, phase, bias_seed, normal_bank, packet); hooks += 1
            for _ in range(15000):
                if cpu.pc == STOP: break
                instruction = get(cpu.pc); opcode = instruction >> 26
                if opcode not in (0x12, 0x32, 0x3A): cpu.step(); continue
                previous = cpu.delay; cpu.delay = None; rs, rt = (instruction >> 21) & 31, (instruction >> 16) & 31
                if opcode in (0x32, 0x3A):
                    a = (cpu.r[rs] + signed(instruction, 16)) & 0xFFFFFFFF
                    if opcode == 0x32:
                        data[rt] = get(a); events.extend([0x100 + rt, a, data[rt]])
                        if rt == 0 and NORMALS <= a < NORMALS + 256: normal_bank = (a - NORMALS) // 128
                    else:
                        events.extend([0x200 + rt, a, data[rt]]); cpu.store(a, data[rt], 4)
                        if a in (WORK + 28, WORK + 32): mutate_now(1 if a == WORK + 28 else 2)
                        if GPU <= a < GPU + GPU_SIZE:
                            if rt == 20 and rgb_groups == 0: packet = a - 8
                            if rt == 22 and previous_store == 21:
                                rgb_groups += 1
                                if rgb_groups == 1: mutate_now(8)
                            previous_store = rt
                elif not instruction & 0x02000000:
                    assert rs == 0
                    r = (instruction >> 11) & 31; events.extend([0x300 + r, 0, data[r]]); cpu.r[rt] = data[r]
                else:
                    events.extend([0x600, 0, instruction])
                    if instruction == 0x4A280030:
                        q += 1; clips = ordinal = env_ordinal = rgb_groups = 0; previous_store = None; _, xy = projected(flow, q); data[12:15] = xy[:3]
                    elif instruction == 0x4A180001: data[14] = xy[3]
                    elif instruction == 0x4B400006: data[24] = AREAS[(flow + q * 3 + clips * 2) % 5] & 0xFFFFFFFF; clips += 1
                    elif instruction == 0x4B68002E: data[7] = DEPTHS[(flow + q * 5) & 7] & 0xFFFFFFFF
                    elif instruction == 0x4A4A6412:
                        normal_slot = 0
                        if ordinal == 0: mutate_now(3)
                    elif instruction == 0x4A4AE412: normal_slot = 1; mutate_now(5)
                    elif instruction == 0x4A4B6412: normal_slot = 2; mutate_now(7)
                    elif instruction == 0x4B38041C:
                        bits = data[normal_slot * 2] ^ ((data[normal_slot * 2 + 1] * 0x9E3779B9) & 0xFFFFFFFF) ^ data[6] ^ ((flow * 0x1234567 + q * 0x17171717 + ordinal * 0x31415927) & 0xFFFFFFFF)
                        data[20:23] = [data[21], data[22], (data[6] & 0xFF000000) | (bits & 0xFFFFFF)]
                        if ordinal in (0, 1): mutate_now(4 + ordinal * 2)
                        ordinal += 1
                    elif instruction in (0x4A486012, 0x4A48E012, 0x4A496012):
                        assert instruction == (0x4A486012, 0x4A48E012, 0x4A496012, 0x4A486012)[env_ordinal]
                        data[9] = IR[(flow + q * 3 + env_ordinal * 5) & 15] & 0xFFFFFFFF; data[10] = IR[(flow * 3 + q + env_ordinal * 7) & 15] & 0xFFFFFFFF
                        if env_ordinal == 0: mutate_now(9)
                        if env_ordinal == 2: mutate_now(10)
                        env_ordinal += 1
                    else: assert instruction == 0x4AA80428
                cpu.pc = previous if previous is not None else cpu.pc + 4
            assert cpu.pc == STOP and q + 1 == max(count, 0) and hooks == meta['hooks'], (case, hooks, meta['hooks'])
            actual = struct.pack('<3I', cpu.r[2], len(events) // 3, hooks) + struct.pack('<' + 'I' * len(events), *events) + bytes(MAX_EVENTS * 12 - len(events) * 4) + snapshot() + struct.pack('<32I', *data)
            assert actual == expected, (case, 'MIPS/wire oracle; exact transfers, live fields, packet/head aliasing, ABI')
            for r in (*range(16, 24), 28, 30): assert cpu.r[r] == 0xBEEF0000 + r
            assert cpu.r[29] == STACK and cpu.r[31] == STOP
            for key in coverage: coverage[key] |= meta[key]
            shifts.add(shift); counts.add(count); vshifts.add(meta['vshift']); masks.add(struct.unpack_from('<I', state, 840)[0])
        assert coverage['paths'] == {'empty', 'nonpositive', 'far', 'winding', 'clip_x', 'clip_y', 'direct', 'env', 'restore', 'reuse_mask'}, coverage['paths']
        assert coverage['direct_aliases'] == set(range(14)), coverage['direct_aliases']
        assert coverage['env_aliases'] == set(ENV_HEAD_OFFSETS), coverage['env_aliases']
        assert shifts == set(range(32)) and counts == set(COUNTS) and vshifts == {6, 7}
        assert coverage['biases'] == set(range(-128, 128)), len(coverage['biases'])
        assert masks == set(MASKS) and coverage['normal_banks'] == {0, 1}
        assert coverage['env_irs'] == set(IR) and coverage['controls'] == set(CONTROLS)
        print('PASS', CASES, 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original', name,
              'MIPS/native cases; complete owners/full pointers, real DMA translation/tagged links, direct/masked packets and restore/reuse, '
              'lookahead XY and all valid packet/head aliases, signed UVs/bias/counts, captured/current geometry/normals/frame/mask, '
              'live UV/flags/ordering/depth/control/CLUT/page, RGB FIFO, exact GTE transfers and ABI, UBSan; scripted GTE math', flush=True)


if __name__ == '__main__':
    main()
