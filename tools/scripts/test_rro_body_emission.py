"""Actual body emitter vs original/compiled MIPS and independent wire protocol.

GTE projection/area results are scripted. The actual C runs with complete public
types and high native pointers; this validates staging, live roots, projection
storage and packet/DMA behavior, not numerical GTE accuracy or timing.
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
CASES, INPUT, STATE, MAX_EVENTS = 4096, 1912, 18304, 384
OUTPUT = 4+MAX_EVENTS*12+STATE+128
DEPTHS, AREAS = (0, 1, 16, 64, 1791, 1792, 1793, 65535), (0, 1, -1, -2147483648, 2147483647)
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "rr/rro_effects.h"
#include "rr/gpu_address.h"
typedef union {
 u32 words[41];
 struct {RroBodyCommandHeader header;RroBodyQuad quads[8];} decoded;
} BodyStream;
typedef union {u32 words[2048];RroBodyPacket packets[204];} GpuArena;
static struct World {
 u8 before[16];RroEffectScratch scratch;RroEffectMesh mesh;
 BodyStream streams[2];Vec3s8 vertices[2][63];
 _Alignas(16384) GpuArena gpu[2];u8 after[16];
} world,expected;
#define SCRATCHPAD ((u8*)&world.scratch.scene)
static unsigned scenario,events,groups,clips,geometryBank,drawStream,drawCount,projectedCount,expectedGroups;
static s32 shift,bias;static u8 *referenceCursor;static u32 *referenceOrder;
static u32 data[32],*trace;
static const s32 depths[]={0,1,16,64,1791,1792,1793,65535};
static const s32 areas[]={0,1,-1,-2147483647-1,2147483647};
static s32 word(u32 x){s32 y;memcpy(&y,&x,4);return y;}
static void put(void *p,u32 value){memcpy(p,&value,4);}
u32 gpu_address(void){
 uintptr_t p=(uintptr_t)world.gpu;
 assert(p>UINT32_MAX&&(p&16383)==0&&(p&0xFFFFFFU)+sizeof(world.gpu)<=0x1000000U);
 return 0x80180000U;
}
static GpuDmaRegion gpuRegion;
static void installGpuMap(void) {
 gpuRegion=(GpuDmaRegion){world.gpu,0x80180000U,sizeof(world.gpu)};
 assert(GpuDmaSetRegions(&gpuRegion,1));
}
static u32 address(const void *pointer){
 uintptr_t p=(uintptr_t)pointer,b=(uintptr_t)world.gpu;
 if(p>=b&&p-b<sizeof(world.gpu))return gpu_address()+(u32)(p-b);
 const struct {const void *p;u32 address,size;} regions[]={
  {world.streams,0x80210010,328},{world.vertices,0x80220010,1008},
  {world.scratch.work7C.verticesE0,0x1F80015C,24},
  {&world.scratch.work7C.projection188,0x1F800204,504},
  {&world.scratch.work7C.firstArea74,0x1F8000F0,4}};
 for(unsigned i=0;i<sizeof(regions)/sizeof(regions[0]);i++){
  b=(uintptr_t)regions[i].p;if(p>=b&&p-b<regions[i].size)return regions[i].address+(u32)(p-b);
 }
 assert(!"truncated or unmapped native pointer");return 0;
}
static void *expectedGpu(const void *p){
 uintptr_t offset=(uintptr_t)p-(uintptr_t)world.gpu;assert(offset<sizeof(world.gpu));return (u8*)expected.gpu+offset;
}
static void event(u32 kind,u32 target,u32 value){assert(events++<384);*trace++=kind;*trace++=target;*trace++=value;}
static void load(unsigned reg,const void *p){u32 value;memcpy(&value,p,4);data[reg]=value;event(0x100+reg,address(p),value);}
static void store(unsigned reg,void *p){
 event(0x200+reg,address(p),data[reg]);memcpy(p,&data[reg],4);
 if(reg==24){assert(p==&world.scratch.work7C.firstArea74);return;}
 uintptr_t offset=(uintptr_t)p-(uintptr_t)&world.scratch.work7C.projection188;
 assert(offset>=24&&offset+4<=504);memcpy((u8*)&expected.scratch.work7C.projection188+offset,&data[reg],4);
}
static s32 depthAt(unsigned index){
 s32 value=depths[scenario&7];if(value&&!(scenario&1024))value+=(index*13)&31;
 return value>65535?65535:value;
}
static void referenceFace(const RroBodyQuad *captured,s32 area,unsigned face){
 RroEffectWork *work=&expected.scratch.work7C;
 RroBodyQuad *quad=&expected.streams[drawStream].decoded.quads[face];
 u32 total=(u32)work->projection188.mesh.vertices[captured->vertices[0]].sz;
 total+=(u32)work->projection188.mesh.vertices[captured->vertices[1]].sz;
 total+=(u32)work->projection188.mesh.vertices[captured->vertices[2]].sz;
 total+=(u32)work->projection188.mesh.vertices[quad->vertices[3]].sz;
 if(word(total)<=0)return;
 u32 bucket=total>>shift;if(bucket>=448)return;
 work->firstArea74=area;if(!area)return;
 u32 *head=(u32*)expectedGpu(referenceOrder)+bias+bucket;
 u8 *packet=expectedGpu(referenceCursor);u32 previous;memcpy(&previous,head,4);
 /* Independent GPU wire oracle, including head/packet overlap. */
 put(packet,previous);packet[3]=9;put(head,address(referenceCursor));((u8*)head)[3]=0;
 for(unsigned i=0;i<4;i++){
  put(packet+8+i*8,(u32)work->colors15C.palette.rgb160[quad->colors[i]]);
  put(packet+12+i*8,(u32)work->projection188.mesh.vertices[quad->vertices[i]].sxy);
 }
 put(packet+4,0xE1000220U);referenceCursor+=40;
}
static void command(u32 code){
 event(0x600,0,code);
 if(code==0x4A280030){
  for(unsigned j=0;j<3;j++){
   unsigned index=groups*3+j;
   data[12+j]=data[j*2]^(data[j*2+1]*0x9E3779B9U+scenario*0x31415927U+index*0x17171717U);
   data[17+j]=(u32)depthAt(index);
  }
  groups++;
  if(groups==1){
   world.mesh.vertices=expected.mesh.vertices=world.vertices[geometryBank^1];
   world.mesh.vertexCount=expected.mesh.vertexCount=(scenario*17)%61;
   world.mesh.commands=expected.mesh.commands=world.streams[drawStream].words;
  }
  u32 value=world.vertices[geometryBank][groups*3].words[0]^(scenario*0x1234567U+groups);
  world.vertices[geometryBank][groups*3].words[0]=expected.vertices[geometryBank][groups*3].words[0]=value;
 }else{
  assert(code==0x4B400006&&clips<drawCount);
  unsigned face=clips++;s32 area=areas[(scenario+face)%5];data[24]=(u32)area;
  RroBodyQuad captured=expected.streams[drawStream].decoded.quads[face];
  unsigned vertex=(captured.vertices[3]+1)%projectedCount;
  world.streams[drawStream].decoded.quads[face].vertices[3]=expected.streams[drawStream].decoded.quads[face].vertices[3]=vertex;
  if(scenario&256){
   vertex=(captured.vertices[0]+2)%projectedCount;
   world.streams[drawStream].decoded.quads[face].vertices[0]=expected.streams[drawStream].decoded.quads[face].vertices[0]=vertex;
  }
  unsigned color=(scenario+face)%6;
  world.scratch.work7C.colors15C.palette.rgb160[color]=expected.scratch.work7C.colors15C.palette.rgb160[color]=word(scenario*0x9E3779B9U+face*0x1234567U);
  if(face==0&&(scenario&128)){
   world.scratch.scene.view.projection.pointers.cursor=expected.scratch.scene.view.projection.pointers.cursor=(u8*)&world.gpu[0].words[1500];
   world.scratch.scene.view.projection.pointers.order=expected.scratch.scene.view.projection.pointers.order=&world.gpu[1].words[256];
   world.scratch.scene.depthShift60=expected.scratch.scene.depthShift60=(scenario+3)%28;
   world.scratch.work7C.facing98.z=expected.scratch.work7C.facing98.z=-7;
   world.streams[drawStream].decoded.header.fields.count=expected.streams[drawStream].decoded.header.fields.count=8-drawCount;
  }
  referenceFace(&captured,area,face);
 }
}
static void writeData(unsigned reg,u32 value){event(0x400+reg,0,value);data[reg]=value;}
#define RR_GTE14_LWC2_V012(p) do{for(unsigned r=0;r<6;r++)load(r,(const u8*)(p)+r*4);}while(0)
#define RR_GTE14_NOP2() ((void)0)
#define RR_GTE14_RTPT() command(0x4A280030)
#define GTE_STORE_SXY_SZ(p) do{for(unsigned j=0;j<3;j++){store(12+j,(u8*)(p)+j*8);store(17+j,(u8*)(p)+j*8+4);}}while(0)
#define RR_GTE_NCLIP_SXY3(a,b,c) do{writeData(12,(u32)(a));writeData(14,(u32)(c));writeData(13,(u32)(b));command(0x4B400006);}while(0)
#define RR_GTE14_SWC2_MAC0(p) store(24,p)
'''
TAIL = r'''
void run_case(unsigned number,const u8 *input,u8 *output){
 installGpuMap();
 assert(sizeof(void*)==8&&sizeof(long)==8&&(uintptr_t)&world>UINT32_MAX);
 scenario=number;events=groups=clips=0;memset(&world,0xA5,sizeof(world));memset(data,0,sizeof(data));memset(output,0,OUTPUT_SIZE);trace=(u32*)(output+4);
 memcpy(world.streams,input,328);memcpy(world.vertices,input+328,1008);
 u32 mesh[3];memcpy(mesh,input+1336,12);geometryBank=(mesh[0]-0x80220010)/504;
 unsigned inputStream=(mesh[2]-0x80210010)/164;drawStream=inputStream^1;
 world.mesh.vertices=world.vertices[geometryBank];world.mesh.vertexCount=mesh[1];world.mesh.commands=world.streams[inputStream].words;
 expectedGroups=(mesh[1]+2)/3;if(!expectedGroups)expectedGroups=1;projectedCount=expectedGroups*3;assert(projectedCount<=60);
 drawCount=world.streams[drawStream].decoded.header.fields.count;assert(drawCount<=8);
 memcpy(world.scratch.work7C.verticesE0,input+1348,24);memcpy(&world.scratch.work7C.projection188,input+1372,504);
 memcpy(world.scratch.work7C.colors15C.palette.rgb160,input+1876,24);memcpy(&world.scratch.work7C.firstArea74,input+1900,4);
 memcpy(&world.scratch.work7C.facing98.z,input+1904,4);memcpy(&world.scratch.scene.depthShift60,input+1908,4);
 shift=world.scratch.scene.depthShift60+4;assert(shift>=4&&shift<=31);
 bias=world.scratch.work7C.facing98.z>0?113:108;
 for(unsigned i=0;i<4096;i++)put((u8*)world.gpu+i*4,number*0x31415927U+i*0x17171717U);
 unsigned orderBank=(number>>6)&1;unsigned bucket=(u32)depths[number&7]>>(shift-2);if(bucket>=448)bucket=0;
 unsigned packetWord=(number&1024)?128+bias+bucket-((number>>4)%10):900;
 unsigned packetBank=(number&1024)?orderBank:orderBank^1;
 world.scratch.scene.view.projection.pointers.cursor=(u8*)&world.gpu[packetBank].words[packetWord];
 world.scratch.scene.view.projection.pointers.order=&world.gpu[orderBank].words[128];expected=world;
 referenceCursor=world.scratch.scene.view.projection.pointers.cursor;referenceOrder=world.scratch.scene.view.projection.pointers.order;
 EmitCarBodyQuads(&world.mesh,&world.scratch.work7C);
 assert(groups==expectedGroups&&clips==drawCount);
 memcpy(expected.scratch.work7C.verticesE0,expected.vertices[geometryBank]+groups*3,24);
 expected.scratch.scene.view.projection.pointers.cursor=referenceCursor;
 if(memcmp(&world,&expected,sizeof(world))){
  const u8 *a=(const u8*)&world,*b=(const u8*)&expected;for(unsigned i=0;i<sizeof(world);i++)if(a[i]!=b[i]){fprintf(stderr,"case %u owner offset %u: %u != %u\n",number,i,a[i],b[i]);break;}
  assert(!"complete native owner mismatch");
 }
 memcpy(output,&events,4);u8 *p=output+4+384*12;
 memcpy(p,world.streams,328);p+=328;memcpy(p,world.vertices,1008);p+=1008;
 mesh[0]=address(world.mesh.vertices);mesh[1]=world.mesh.vertexCount;mesh[2]=address(world.mesh.commands);memcpy(p,mesh,12);p+=12;
 memcpy(p,world.scratch.work7C.verticesE0,24);p+=24;memcpy(p,&world.scratch.work7C.projection188,504);p+=504;
 memcpy(p,world.scratch.work7C.colors15C.palette.rgb160,24);p+=24;memcpy(p,&world.scratch.work7C.firstArea74,4);p+=4;
 memcpy(p,&world.scratch.work7C.facing98.z,4);p+=4;memcpy(p,&world.scratch.scene.depthShift60,4);p+=4;
 put(p,address(world.scratch.scene.view.projection.pointers.cursor));p+=4;put(p,address(world.scratch.scene.view.projection.pointers.order));p+=4;
 memcpy(p,world.gpu,sizeof(world.gpu));p+=sizeof(world.gpu);memcpy(p,data,128);
}
'''


def main():
    os.chdir(ROOT)
    owner = source_owners(ROOT/'src/main/PAL')['EmitCarBodyQuads']
    body = re.sub(r'__asm__\("\$\d+"\)', '', definition_source(owner.read_text(), 'EmitCarBodyQuads'))
    with TemporaryDirectory() as directory:
        source, library = Path(directory)/'native.c', Path(directory)/'native.dylib'
        source.write_text(PREFIX+body+TAIL.replace('OUTPUT_SIZE', str(OUTPUT)))
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-Iinclude', str(source), str(ROOT/'src/native/gpu_address.c'), '-o', str(library)], check=True)
        native = C.CDLL(str(library)); native.run_case.argtypes = [C.c_uint, C.c_void_p, C.c_void_p]
        native.gpu_address.restype = C.c_uint32; gpu = native.gpu_address()
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0, 0x400000); memory.mem_map(0x1000000, 0x1000000); memory.mem_map(0x1F800000, 4096)
        write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b))
        read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
        put = lambda a, v: write(a, struct.pack('<I', v & 0xFFFFFFFF))
        get = lambda a: int.from_bytes(read(a, 4), 'little')
        executable = (ROOT/'assets/PAL/main.exe').read_bytes(); base, = struct.unpack_from('<I', executable, 0x18)
        write(base, executable[0x800:]); start = address_of('EmitCarBodyQuads')
        if os.getenv('RR_COMPILED_ELF'):
            with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                elf = ELFFile(stream); symbol = elf.get_section_by_name('.symtab').get_symbol_by_name('EmitCarBodyQuads')[0]
                section = elf.get_section(symbol['st_shndx']); offset = symbol['st_value']-section['sh_addr']
                assert symbol['st_value'] == start; write(start, section.data()[offset:offset+symbol['st_size']])

        def check_store(cpu, access, address, size, value, user):
            address &= 0x1FFFFFFF
            assert any((a&0x1FFFFFFF) <= address and address+size <= (a&0x1FFFFFFF)+n
                       for a, n in [(WORK+0xE0, 24), (WORK+0x1A0, 480), (WORK+0x74, 4),
                                    (0x1F800000, 4), (gpu, 16384), (STACK-8, 8)]), (hex(address), size)

        cpu = Cpu(memory, 0, 100, 101, 102, check_store)
        seen_counts, seen_quads, aliases, shifts, paths = set(), set(), set(), set(), set()
        for case in range(CASES):
            state = bytearray((case*19+i*29)&255 for i in range(INPUT)); count=case%61
            expected_groups=max(1, (count+2)//3); projected_count=expected_groups*3
            for stream in range(2):
                n=(case//7+stream*3)%9; struct.pack_into('<I', state, stream*164, n<<16|3)
                for q in range(8):
                    struct.pack_into('<8H', state, stream*164+4+q*20,
                                     *[(case+stream+q*3+j)%projected_count for j in range(4)],
                                     *[(case+q+j)%6 for j in range(4)])
            geometry_bank=(case>>4)&1; input_stream=(case>>5)&1; draw_stream=input_stream^1
            draw_count=struct.unpack_from('<H', state, draw_stream*164+2)[0]
            struct.pack_into('<3I', state, 1336, VERTICES+geometry_bank*504, count, STREAM+input_stream*164)
            facing=(-1, 0, 1)[(case>>2)%3]; scene_shift=(case>>3)%28
            struct.pack_into('<iI', state, 1904, facing, scene_shift)
            incoming=C.create_string_buffer(bytes(state)); output=(C.c_uint32*((OUTPUT+3)//4))()
            native.run_case(case, incoming, output); expected=bytes(output)[:OUTPUT]
            write(STREAM, state[:328]); write(VERTICES, state[328:1336]); write(MESH, state[1336:1348])
            write(WORK+0xE0, state[1348:1372]); write(WORK+0x188, state[1372:1876]); write(WORK+0x160, state[1876:1900])
            write(WORK+0x74, state[1900:1904]); write(WORK+0xA0, state[1904:1908]); write(0x1F800060, state[1908:1912])
            order_bank=(case>>6)&1; bias=113 if facing>0 else 108; shift=scene_shift+4
            bucket=DEPTHS[case&7]>>(shift-2); bucket=bucket if bucket<448 else 0
            packet_word=128+bias+bucket-((case>>4)%10) if case&1024 else 900
            packet_bank=order_bank if case&1024 else order_bank^1
            packet=gpu+packet_bank*8192+packet_word*4; ordering=gpu+order_bank*8192+128*4
            put(0x1F800000,packet); put(0x1F800004,ordering)
            write(gpu, struct.pack('<4096I', *[(case*0x31415927+i*0x17171717)&0xFFFFFFFF for i in range(4096)]))
            cpu.reset(); cpu.r=[0]+[0xBEEF0000+r for r in range(1,32)]
            cpu.r[4:6]=[MESH,WORK];cpu.r[29]=STACK;cpu.r[31]=STOP;cpu.pc=start
            data,events,groups,clips=[0]*32,[],0,0
            for _ in range(10000):
                if cpu.pc==STOP:break
                instruction=get(cpu.pc);opcode=instruction>>26
                if opcode not in (0x12,0x32,0x3A):cpu.step();continue
                previous=cpu.delay;cpu.delay=None;rs,rt,rd=(instruction>>21)&31,(instruction>>16)&31,(instruction>>11)&31
                if opcode in (0x32,0x3A):
                    a=(cpu.r[rs]+signed(instruction,16))&0xFFFFFFFF
                    if opcode==0x32:value=get(a);data[rt]=value;events.extend([0x100+rt,a,value])
                    else:events.extend([0x200+rt,a,data[rt]]);cpu.store(a,data[rt],4)
                elif not instruction&0x02000000:
                    assert rs==4;data[rd]=cpu.r[rt];events.extend([0x400+rd,0,cpu.r[rt]])
                else:
                    events.extend([0x600,0,instruction])
                    if instruction==0x4A280030:
                        for j in range(3):
                            index=groups*3+j
                            data[12+j]=data[j*2]^((data[j*2+1]*0x9E3779B9+case*0x31415927+index*0x17171717)&0xFFFFFFFF)
                            depth=DEPTHS[case&7];data[17+j]=min(65535,depth+((index*13)&31) if depth and not case&1024 else depth)
                        groups+=1
                        if groups==1:put(MESH,VERTICES+(geometry_bank^1)*504);put(MESH+4,(case*17)%61);put(MESH+8,STREAM+draw_stream*164)
                        a=VERTICES+geometry_bank*504+groups*24;put(a,get(a)^((case*0x1234567+groups)&0xFFFFFFFF))
                    else:
                        assert instruction==0x4B400006 and clips<draw_count
                        face=clips;clips+=1;data[24]=AREAS[(case+face)%5]&0xFFFFFFFF
                        q=STREAM+draw_stream*164+4+face*20
                        index=int.from_bytes(read(q+6,2),'little');write(q+6,struct.pack('<H',(index+1)%projected_count))
                        if case&256:
                            index=int.from_bytes(read(q,2),'little');write(q,struct.pack('<H',(index+2)%projected_count))
                        put(WORK+0x160+((case+face)%6)*4,case*0x9E3779B9+face*0x1234567)
                        if face==0 and case&128:
                            put(0x1F800000,gpu+1500*4);put(0x1F800004,gpu+8192+256*4);put(0x1F800060,(case+3)%28);put(WORK+0xA0,-7)
                            write(STREAM+draw_stream*164+2,struct.pack('<H',8-draw_count))
                cpu.pc=previous if previous is not None else cpu.pc+4
            assert cpu.pc==STOP and groups==expected_groups and clips==draw_count
            snapshot=read(STREAM,328)+read(VERTICES,1008)+read(MESH,12)+read(WORK+0xE0,24)+read(WORK+0x188,504)+read(WORK+0x160,24)+read(WORK+0x74,4)+read(WORK+0xA0,4)+read(0x1F800060,4)+read(0x1F800000,8)+read(gpu,16384)
            actual=struct.pack('<I',len(events)//3)+struct.pack('<'+'I'*len(events),*events)+bytes(MAX_EVENTS*12-len(events)*4)+snapshot+struct.pack('<32I',*data)
            assert actual==expected,(case,'prefetch / projection / full body packet protocol and owners')
            for reg in (*range(16,24),28,30):assert cpu.r[reg]==0xBEEF0000+reg
            assert cpu.r[29]==STACK and cpu.r[31]==STOP
            seen_counts.add(count);seen_quads.add(draw_count);shifts.add(scene_shift)
            emitted=(get(0x1F800000)-packet)//40;assert emitted>=0 and get(0x1F800000)==packet+emitted*40
            if emitted:
                paths.add('emitted');aliases.add((case>>4)%10 if case&1024 else 'separate')
            else:paths.add('empty' if not draw_count else 'rejected')
        assert seen_counts==set(range(61)) and seen_quads==set(range(9)) and shifts==set(range(28))
        assert paths=={'empty','emitted','rejected'} and aliases==set(range(10))|{'separate'}
        print('PASS',CASES,'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
              'MIPS/native full body cases; real owners, full pointers, counts 0..60, final prefetch, live/captured roots, palette and indices, all packet/head aliases, DMA words, complete GPU arenas, exact GTE transfers, ABI, UBSan; scripted GTE math')


if __name__=='__main__':
    main()
