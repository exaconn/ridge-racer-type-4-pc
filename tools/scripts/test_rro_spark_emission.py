"""Actual spark C vs original/compiled MIPS and a structured behavior oracle.

Uses full native owners/pointers and scripted random/GTE services. The numerical
GTE model is deliberately synthetic; transfer order, state, motion and DMA wire
behavior are checked. No original fixture or case set is replaced.
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
WORK, SPARKS = 0x1F80007C, 0x80250010
STACK, STOP = 0x803F0000, 0x803FF000
CASES, PREFIX_SIZE, GPU_SIZE, MAX_EVENTS = 4096, 560, 16384, 256
STATE = PREFIX_SIZE+GPU_SIZE
SERVICE = 8+PREFIX_SIZE+256
OUTPUT = 8+MAX_EVENTS*12+4*SERVICE+STATE+256
DEPTHS = (0, 1, 4, 447, 448, 1791, 1792, 65535)
TIMERS = (0, 1, 169, 170, 171, 4096, -2147483648, 2147483647)
SPEEDS = (0, 1, -1, 8191, -8192, 2147483647, -2147483648, 0x40000000)
HALVES = (-32768, -32767, -1, 0, 1, 32767, 1234, -4321)
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "rr/rro_effects.h"
#include "rr/gpu_address.h"
typedef union {u32 words[2048];RroSparkPacket packets[409];} GpuArena;
typedef struct {
 u8 before[16];RroEffectScratch scratch;RroSpark sparks[2][16];
 _Alignas(16384) GpuArena gpu[2];u8 after[16];
} World;
static World world,expected;
#define SCRATCHPAD ((u8*)&world.scratch.scene)
static unsigned scenario,events,services,projections;static u32 data[32],control[32],*trace;static u8 *serviceTrace;
static const s32 timers[]={0,1,169,170,171,4096,-2147483647-1,2147483647};
static const u32 depths[]={0,1,4,447,448,1791,1792,65535};
static s32 word(u32 x){s32 y;memcpy(&y,&x,4);return y;}
static void put(u8 **p,u32 value){memcpy(*p,&value,4);*p+=4;}
static u32 get(const u8 **p){u32 value;memcpy(&value,*p,4);*p+=4;return value;}
u32 gpu_address(void){
 uintptr_t p=(uintptr_t)world.gpu;assert(p>UINT32_MAX&&(p&16383)==0&&(p&0xFFFFFFU)+sizeof(world.gpu)<=0x1000000U);
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
 b=(uintptr_t)world.scratch.work7C.verticesE0;if(p>=b&&p-b<8)return 0x1F80015CU+(u32)(p-b);
 if(pointer==&world.scratch.work7C.depth70)return 0x1F8000EC;
 assert(!"unmapped or truncated native pointer");return 0;
}
static void *resolveGpu(u32 value){u32 offset=value-gpu_address();assert(offset<sizeof(world.gpu));return (u8*)world.gpu+offset;}
static void loadFields(World *target,const u8 *p){
 memcpy(target->sparks,p,512);p+=512;memcpy(target->scratch.work7C.verticesE0,p,8);p+=8;
 memcpy(&target->scratch.work7C.depth70,p,4);p+=4;memcpy(target->scratch.work7C.colors15C.palette.rgb160,p,24);p+=24;
 target->scratch.scene.view.projection.pointers.cursor=resolveGpu(get(&p));
 target->scratch.scene.view.projection.pointers.order=resolveGpu(get(&p));
 target->scratch.scene.depthShift60=word(get(&p));memcpy(target->gpu,p,16384);
}
static void snapshot(u8 *p,int gpu){
 memcpy(p,world.sparks,512);p+=512;memcpy(p,world.scratch.work7C.verticesE0,8);p+=8;
 memcpy(p,&world.scratch.work7C.depth70,4);p+=4;memcpy(p,world.scratch.work7C.colors15C.palette.rgb160,24);p+=24;
 put(&p,address(world.scratch.scene.view.projection.pointers.cursor));put(&p,address(world.scratch.scene.view.projection.pointers.order));put(&p,(u32)world.scratch.scene.depthShift60);
 if(gpu)memcpy(p,world.gpu,16384);
}
static void event(u32 kind,u32 target,u32 value){assert(events++<256);*trace++=kind;*trace++=target;*trace++=value;}
static void writeData(unsigned reg,u32 value){event(0x400+reg,0,value);data[reg]=value;}
static void writeControl(unsigned reg,u32 value){event(0x500+reg,0,value);control[reg]=value;}
static void farColor(const void *pointer){const u8 *p=pointer;u32 r=p[0],g=p[1],b=p[2];writeControl(21,r<<4);writeControl(22,g<<4);writeControl(23,b<<4);}
static void load(unsigned reg,const void *p){u32 value;memcpy(&value,p,4);event(0x100+reg,address(p),value);data[reg]=value;}
static void store(unsigned reg,void *p){event(0x200+reg,address(p),data[reg]);memcpy(p,&data[reg],4);}
static void command(u32 code){
 event(0x600,0,code);
 if(code==0x4A780010){
  u32 mix=data[8]*0x01010101U+control[21]*3U+control[22]*5U+control[23]*7U;
  data[22]=(data[6]&0xFF000000U)|((data[6]^mix)&0xFFFFFFU);
 }else{
  assert(code==0x4A280030);unsigned i=projections++;
  data[12]=data[0]^(scenario*0x17171717U+i);
  data[13]=data[2]^(data[3]*0x9E3779B9U+scenario*0x31415927U+i);
  data[14]=data[4]^(data[5]*0x31415927U+scenario*0x17171717U+i);
  data[19]=depths[(scenario+i)&7];data[17]=data[19]?data[19]-1:0;data[18]=data[19];
 }
}
static u32 randomValue(unsigned index){
 if(!index)return scenario&3;
 if(index==1&&!(scenario&16))return 0x100;
 if(index==2&&!(scenario&32))return 0x100;
 return scenario*0x9E3779B9U+index*0x1234567U;
}
s32 PsyqRand(void){
 unsigned index=services++;assert(index<4);u32 value=randomValue(index);
 put(&serviceTrace,index);put(&serviceTrace,value);snapshot(serviceTrace,0);serviceTrace+=560;
 memcpy(serviceTrace,data,128);serviceTrace+=128;memcpy(serviceTrace,control,128);serviceTrace+=128;
 RroSpark *target=&world.sparks[scenario&1][15];
 target->timer=(scenario&256)?0:(scenario&128)?512+(scenario&255):timers[(scenario+index)&7];
 unsigned w=index%3;target->motion.words[w]=word((u32)target->motion.words[w]^(scenario*0x17171717U+index));
 world.scratch.work7C.colors15C.palette.rgb160[(scenario+index)%6]=word(scenario*0x31415927U+index);
 world.scratch.work7C.verticesE0[0].fields.pad06=(s16)(scenario*131+index);
 world.scratch.work7C.depth70=word(scenario*0x1234567U+index+0x80000000U);
 unsigned bank=(scenario+index)&1;
 world.scratch.scene.view.projection.pointers.order=&world.gpu[bank].words[256+index*8];
 world.scratch.scene.view.projection.pointers.cursor=(u8*)&world.gpu[bank^1].words[1400+index*20];
 world.scratch.scene.depthShift60=(scenario+index+7)%30;
 return word(value);
}
#define PTR(p) ((void*)(unsigned long)(p))
#define RR_GTE_SETFC_BYTES3(p) farColor(PTR(p))
#define RR_GTE_MTC2_RGB(v) writeData(6,(u32)(v))
#define RR_GTE_MTC2_IR0(v) writeData(8,(u32)(v))
#define RR_GTE14_NOP2() ((void)0)
#define RR_GTE_OP(code) command(code)
#define RR_GTE14_SWC2_RGB2(p) store(22,PTR(p))
#define RR_GTE14_LWC2_V1(p) do{load(2,PTR(p));load(3,(u8*)PTR(p)+4);}while(0)
#define RR_GTE_RTPT_LDV2(p) do{load(4,PTR(p));load(5,(u8*)PTR(p)+4);command(0x4A280030);}while(0)
#define RR_GTE14_SWC2_SZ3(p) store(19,PTR(p))
#define RR_GTE14_SWC2_SXY1(p) store(13,PTR(p))
#define RR_GTE14_SWC2_SXY2(p) store(14,PTR(p))
'''
TAIL = r'''
void run_case(unsigned number,s32 speed,const u8 *input,const u8 *reference,u8 *output){
 installGpuMap();
 assert(sizeof(void*)==8&&sizeof(long)==8&&(uintptr_t)&world>UINT32_MAX);
 scenario=number;events=services=projections=0;memset(&world,0xA5,sizeof(world));memset(output,0,OUTPUT_SIZE);
 for(unsigned r=0;r<32;r++){data[r]=number*0x1234567U+r*0x17171717U;control[r]=~(number*0x31415927U+r*0x1234567U);}
 loadFields(&world,input);expected=world;trace=(u32*)(output+8);serviceTrace=output+8+256*12;
 DrawSparkLines(world.sparks[number&1],&world.scratch.work7C,speed);
 loadFields(&expected,reference);
 if(memcmp(&world,&expected,sizeof(world))){
  const u8 *a=(const u8*)&world,*b=(const u8*)&expected;for(unsigned i=0;i<sizeof(world);i++)if(a[i]!=b[i]){fprintf(stderr,"case %u owner offset %u: %u != %u\n",number,i,a[i],b[i]);break;}
  assert(!"complete native owner mismatch");
 }
 memcpy(output,&events,4);memcpy(output+4,&services,4);u8 *p=output+8+256*12+4*(8+560+256);
 snapshot(p,1);p+=16944;memcpy(p,data,128);memcpy(p+128,control,128);
}
'''


def oracle(number, speed, incoming, gpu):
    state=bytearray(incoming);events=[];services=[];data=[(number*0x1234567+r*0x17171717)&0xFFFFFFFF for r in range(32)]
    control=[~(number*0x31415927+r*0x1234567)&0xFFFFFFFF for r in range(32)]
    read32=lambda a:struct.unpack_from('<I',state,a)[0]
    def put32(a,v):struct.pack_into('<I',state,a,v&0xFFFFFFFF)
    def put16(a,v):struct.pack_into('<H',state,a,v&65535)
    def event(kind,a,v):events.extend([kind,a,v&0xFFFFFFFF])
    def random():
        index=len(services)
        value=number&3 if not index else 256 if index==1 and not number&16 or index==2 and not number&32 else (number*0x9E3779B9+index*0x1234567)&0xFFFFFFFF
        services.append(struct.pack('<II',index,value)+state[:PREFIX_SIZE]+struct.pack('<64I',*data,*control))
        target=(number&1)*256+15*16
        timer=0 if number&256 else 512+(number&255) if number&128 else TIMERS[(number+index)&7]
        put32(target,timer);offset=target+4+(index%3)*4;put32(offset,read32(offset)^((number*0x17171717+index)&0xFFFFFFFF))
        put32(524+((number+index)%6)*4,number*0x31415927+index);put16(518,number*131+index)
        put32(520,number*0x1234567+index+0x80000000)
        bank=(number+index)&1;put32(552,gpu+bank*8192+(256+index*8)*4);put32(548,gpu+(bank^1)*8192+(1400+index*20)*4);put32(556,(number+index+7)%30)
        return value
    def store(reg,a,offset):event(0x200+reg,a,data[reg]);put32(offset,data[reg])
    def load(reg,a,offset):data[reg]=read32(offset);event(0x100+reg,a,data[reg])
    far=state[544:547]
    for j,value in enumerate(far):control[21+j]=value<<4;event(0x515+j,0,value<<4)
    data[6]=0x42000000;event(0x406,0,data[6])
    cursor=read32(548);ordering=read32(552)+432;shift=read32(556)+2;gate=random()
    launch=signed((-speed-160)&65535,16);active=emitted=born=0;first_bucket=None
    for index in range(16):
        at=(number&1)*256+index*16;life=signed((read32(at)-170)&0xFFFFFFFF,32)
        if life<=0:
            if gate:continue
            gate=1;life=4096;born+=1;x=(random()&511)-256;y=(random()&511)-256
            if not x:x=256
            if not y:y=256
            put16(at+4,x);put16(at+6,y+320);z=(random()&127)-3120;put16(at+8,z)
            vx=(abs(x)//2)*(-1 if x<0 else 1);vy=(abs(y)//2)*(-1 if y<0 else 1)
            put16(at+12,vx);put16(at+14,vy);put16(at+10,launch)
        put32(at,life);data[8]=life;event(0x408,0,life);event(0x600,0,0x4A780010)
        mix=data[8]*0x01010101+control[21]*3+control[22]*5+control[23]*7
        data[22]=(data[6]&0xFF000000)|((data[6]^mix)&0xFFFFFF)
        store(22,cursor+8,PREFIX_SIZE+cursor-gpu+8)
        x,y,z,vz,vx,vy=struct.unpack_from('<6h',state,at+4)
        for j,v in enumerate([x>>4,y>>4,z>>4]):put16(512+j*2,v)
        load(2,WORK+0xE0,512);load(3,WORK+0xE4,516)
        for j,v in enumerate([(x+(vx>>3))>>4,(y+(vy>>3))>>4,(z+(vz>>3))>>4]):put16(512+j*2,v)
        load(4,WORK+0xE0,512);load(5,WORK+0xE4,516);event(0x600,0,0x4A280030)
        data[12]=data[0]^((number*0x17171717+active)&0xFFFFFFFF)
        data[13]=data[2]^((data[3]*0x9E3779B9+number*0x31415927+active)&0xFFFFFFFF)
        data[14]=data[4]^((data[5]*0x31415927+number*0x17171717+active)&0xFFFFFFFF)
        data[19]=DEPTHS[(number+active)&7];data[17]=max(0,data[19]-1);data[18]=data[19];active+=1
        for j,v in enumerate([x+(vx>>4),y+(vy>>4),z+(vz>>4)]):put16(at+4+j*2,v)
        store(19,WORK+0x70,520)
        if data[19] and data[19]>>shift<448:
            bucket=data[19]>>shift
            if first_bucket is None:first_bucket=bucket
            store(13,cursor+12,PREFIX_SIZE+cursor-gpu+12);store(14,cursor+16,PREFIX_SIZE+cursor-gpu+16)
            head=PREFIX_SIZE+ordering-gpu+bucket*4;packet=PREFIX_SIZE+cursor-gpu
            put32(packet,read32(head));state[packet+3]=4;put32(head,cursor);state[head+3]=0
            put32(packet+4,0xE1000220);cursor+=20;emitted+=1
    put32(548,cursor)
    trace=struct.pack('<II',len(events)//3,len(services))+struct.pack('<'+'I'*len(events),*events)+bytes(MAX_EVENTS*12-len(events)*4)
    trace+=b''.join(services)+bytes((4-len(services))*SERVICE)+state+struct.pack('<64I',*data,*control)
    return bytes(state),trace,dict(first_bucket=first_bucket,active=active,emitted=emitted,born=born,services=len(services))


def main():
    os.chdir(ROOT);owner=source_owners(ROOT/'src/main/PAL')['DrawSparkLines']
    body=re.sub(r'__asm__\("\$\d+"\)','',definition_source(owner.read_text(),'DrawSparkLines'))
    with TemporaryDirectory() as directory:
        source,library=Path(directory)/'native.c',Path(directory)/'native.dylib';source.write_text(PREFIX+body+TAIL.replace('OUTPUT_SIZE',str(OUTPUT)))
        subprocess.run(['cc','-std=c11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror','-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-Iinclude',str(source),str(ROOT/'src/native/gpu_address.c'),'-o',str(library)],check=True)
        native=C.CDLL(str(library));native.run_case.argtypes=[C.c_uint,C.c_int32,C.c_void_p,C.c_void_p,C.c_void_p];native.gpu_address.restype=C.c_uint32;gpu=native.gpu_address()
        memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000);memory.mem_map(0x1000000,0x1000000);memory.mem_map(0x1F800000,4096)
        write=lambda a,b:memory.mem_write(a&0x1FFFFFFF,bytes(b));read=lambda a,n:bytes(memory.mem_read(a&0x1FFFFFFF,n))
        put=lambda a,v:write(a,struct.pack('<I',v&0xFFFFFFFF));get=lambda a:int.from_bytes(read(a,4),'little')
        executable=(ROOT/'assets/PAL/main.exe').read_bytes();base,=struct.unpack_from('<I',executable,0x18);write(base,executable[0x800:]);start=address_of('DrawSparkLines');rand=address_of('PsyqRand')
        if os.getenv('RR_COMPILED_ELF'):
            with open(os.environ['RR_COMPILED_ELF'],'rb') as stream:
                elf=ELFFile(stream);symbol=elf.get_section_by_name('.symtab').get_symbol_by_name('DrawSparkLines')[0];section=elf.get_section(symbol['st_shndx']);offset=symbol['st_value']-section['sh_addr'];assert symbol['st_value']==start;write(start,section.data()[offset:offset+symbol['st_size']])
        def check_store(cpu,access,address,size,value,user):
            address&=0x1FFFFFFF
            assert any((a&0x1FFFFFFF)<=address and address+size<=(a&0x1FFFFFFF)+n for a,n in [(SPARKS,512),(WORK+0xE0,8),(WORK+0x70,4),(0x1F800000,4),(gpu,GPU_SIZE),(STACK-256,256)]),(hex(address),size)
        cpu=Cpu(memory,0,100,101,102,check_store);seen_shifts,seen_speeds,aliases,paths=set(),set(),set(),set()
        for case in range(CASES):
            state=bytearray((case*19+i*29)&255 for i in range(STATE));bank=case&1
            for b in range(2):
                for i in range(16):
                    timer=TIMERS[(case//4+i)&7]
                    if b==bank:
                        if case&256:timer=0
                        elif case&128:timer=512+i*170+(case&31)
                    at=b*256+i*16;struct.pack_into('<i6h',state,at,timer,*[HALVES[(case+i+j*3)&7] for j in range(6)])
            if not case&384:struct.pack_into('<i',state,bank*256+((case>>4)&7)*16,0)
            shift=(case>>2)%30;order_bank=(case>>6)&1;ordering=gpu+order_bank*8192+128*4;packet=gpu+(order_bank^1)*8192+900*4
            struct.pack_into('<3I',state,548,packet,ordering,shift)
            struct.pack_into('<4096I',state,PREFIX_SIZE,*[(case*0x31415927+i*0x17171717)&0xFFFFFFFF for i in range(4096)])
            speed=SPEEDS[(case>>5)&7]
            if case&1024:
                _,_,meta=oracle(case,speed,state,gpu)
                if meta['first_bucket'] is not None:
                    packet=ordering+432+meta['first_bucket']*4-((case>>4)%5)*4;struct.pack_into('<I',state,548,packet)
            reference,expected,meta=oracle(case,speed,state,gpu)
            incoming=C.create_string_buffer(bytes(state));wanted=C.create_string_buffer(reference);output=(C.c_uint32*((OUTPUT+3)//4))()
            native.run_case(case,speed,incoming,wanted,output);assert bytes(output)[:OUTPUT]==expected,(case,'native trace and structured oracle')
            write(SPARKS,state[:512]);write(WORK+0xE0,state[512:520]);write(WORK+0x70,state[520:524]);write(WORK+0x160,state[524:548]);write(0x1F800000,state[548:556]);write(0x1F800060,state[556:560]);write(gpu,state[PREFIX_SIZE:])
            cpu.reset();cpu.r=[0]+[0xBEEF0000+r for r in range(1,32)];cpu.r[4:7]=[SPARKS+bank*256,WORK,speed&0xFFFFFFFF];cpu.r[29]=STACK;cpu.r[31]=STOP;cpu.pc=start
            data=[(case*0x1234567+r*0x17171717)&0xFFFFFFFF for r in range(32)];control=[~(case*0x31415927+r*0x1234567)&0xFFFFFFFF for r in range(32)];events=[];services=[];projections=0
            def snapshot(gpu_bytes=False):
                return read(SPARKS,512)+read(WORK+0xE0,8)+read(WORK+0x70,4)+read(WORK+0x160,24)+read(0x1F800000,8)+read(0x1F800060,4)+(read(gpu,GPU_SIZE) if gpu_bytes else b'')
            for _ in range(30000):
                if cpu.pc==STOP:break
                if cpu.pc==rand:
                    index=len(services);value=case&3 if not index else 256 if index==1 and not case&16 or index==2 and not case&32 else (case*0x9E3779B9+index*0x1234567)&0xFFFFFFFF
                    services.append(struct.pack('<II',index,value)+snapshot()+struct.pack('<64I',*data,*control))
                    at=SPARKS+bank*256+15*16;timer=0 if case&256 else 512+(case&255) if case&128 else TIMERS[(case+index)&7];put(at,timer);a=at+4+(index%3)*4;put(a,get(a)^((case*0x17171717+index)&0xFFFFFFFF))
                    put(WORK+0x160+((case+index)%6)*4,case*0x31415927+index);write(WORK+0xE6,struct.pack('<H',(case*131+index)&65535));put(WORK+0x70,case*0x1234567+index+0x80000000)
                    b=(case+index)&1;put(0x1F800004,gpu+b*8192+(256+index*8)*4);put(0x1F800000,gpu+(b^1)*8192+(1400+index*20)*4);put(0x1F800060,(case+index+7)%30)
                    resume=cpu.r[31]
                    for r in (*range(2,16),24,25):cpu.r[r]=0xDEAD0000+r
                    cpu.r[2]=value;cpu.pc=resume;continue
                instruction=get(cpu.pc);opcode=instruction>>26
                if opcode not in (0x12,0x32,0x3A):cpu.step();continue
                previous=cpu.delay;cpu.delay=None;rs,rt,rd=(instruction>>21)&31,(instruction>>16)&31,(instruction>>11)&31
                if opcode in (0x32,0x3A):
                    a=(cpu.r[rs]+signed(instruction,16))&0xFFFFFFFF
                    if opcode==0x32:data[rt]=get(a);events.extend([0x100+rt,a,data[rt]])
                    else:events.extend([0x200+rt,a,data[rt]]);cpu.store(a,data[rt],4)
                elif instruction&0x02000000:
                    events.extend([0x600,0,instruction])
                    if instruction==0x4A780010:
                        mix=data[8]*0x01010101+control[21]*3+control[22]*5+control[23]*7;data[22]=(data[6]&0xFF000000)|((data[6]^mix)&0xFFFFFF)
                    else:
                        assert instruction==0x4A280030;i=projections;projections+=1
                        data[12]=data[0]^((case*0x17171717+i)&0xFFFFFFFF);data[13]=data[2]^((data[3]*0x9E3779B9+case*0x31415927+i)&0xFFFFFFFF);data[14]=data[4]^((data[5]*0x31415927+case*0x17171717+i)&0xFFFFFFFF)
                        data[19]=DEPTHS[(case+i)&7];data[17]=max(0,data[19]-1);data[18]=data[19]
                elif rs==4:data[rd]=cpu.r[rt];events.extend([0x400+rd,0,data[rd]])
                else:assert rs==6;control[rd]=cpu.r[rt];events.extend([0x500+rd,0,control[rd]])
                cpu.pc=previous if previous is not None else cpu.pc+4
            assert cpu.pc==STOP and projections==meta['active']
            actual=struct.pack('<II',len(events)//3,len(services))+struct.pack('<'+'I'*len(events),*events)+bytes(MAX_EVENTS*12-len(events)*4)+b''.join(services)+bytes((4-len(services))*SERVICE)+snapshot(True)+struct.pack('<64I',*data,*control)
            assert actual==expected,(case,'MIPS trace, random protocol, complete owners and structured oracle')
            for r in (*range(16,24),28,30):assert cpu.r[r]==0xBEEF0000+r
            assert cpu.r[29]==STACK and cpu.r[31]==STOP
            seen_shifts.add(shift);seen_speeds.add(speed);paths.add((meta['born']>0,meta['active']>0,meta['emitted']>0))
            if meta['emitted']:aliases.add((case>>4)%5 if case&1024 else 'separate')
        assert seen_shifts==set(range(30)) and seen_speeds==set(SPEEDS)
        assert aliases==set(range(5))|{'separate'} and any(not active for _,active,_ in paths) and any(born for born,_,_ in paths)
        print('PASS',CASES,'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
              'MIPS/native full spark cases; real stack/record/packet types, full pointers, wrapped motion/timers/speed, spawn gate, random callbacks, captured GPU roots, partial culled-packet colors, all head aliases, complete owners, ABI, UBSan; scripted GTE math')


if __name__=='__main__':
    main()
