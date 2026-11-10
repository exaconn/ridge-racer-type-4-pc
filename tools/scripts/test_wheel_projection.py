"""Actual wheel projection with the shared rotation ABI and native car owners.

Runs real original/linked RotMatrix arithmetic. GTE projection is scripted;
only the defined low halves of VZ0/R33 are transported, as on hardware. The
unused vector/matrix padding is uninitialized in the original and is ignored.
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
from test_rotation_contract import SOURCE as ROTATION_SOURCE, TABLE

ROOT=Path(__file__).resolve().parents[2]
CARS,STACK,STOP=0x80200010,0x803F0000,0x803FF000
CASES,MAX_EVENTS,STATE=4096,32,1600
OUTPUT=4+MAX_EVENTS*12+STATE+256
PREFIX=r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "psyq/libgte/rotmat.h"
#include "vehicle_wire.h"
typedef struct {u8 before[16];RaceVehicle cars[2];u8 after[16];} World;
static World world,expected;
static unsigned scenario,selected,commands,events;static u32 data[32],control[32],*trace;
static s16 nextInput[3];
static u32 get(const void *p){u32 v;memcpy(&v,p,4);return v;}
static s32 word(u32 x){s32 v;memcpy(&v,&x,4);return v;}
static u32 low(const void *p){u16 v;memcpy(&v,p,2);return v;}
static void event(u32 kind,u32 at,u32 value){assert(events++<32);*trace++=kind;*trace++=at;*trace++=value;}
'''
HARDWARE=r'''
MatrixWords32 *RotMatrix(const Vec3sFields8 *angles,MatrixWords32 *matrix){
 assert(angles->x==0&&angles->z==0&&angles->y==(s16)world.cars[selected].rot54);
 event(0x700,0,(u16)angles->x|((u32)(u16)angles->y<<16));event(0x701,0,(u16)angles->z);
 return referenceRotation(angles,matrix);
}
static void setRotation(const void *p){
 for(unsigned i=0;i<5;i++){u32 v=i==4?low((const u8*)p+i*4):get((const u8*)p+i*4);control[i]=v;event(0x500+i,0,v);}
}
static void mutate(RaceVehicle *v){
 unsigned q=commands;
 for(unsigned i=0;i<5;i++){
  v->footprint110.pairs[i].x=(s16)(u16)(scenario*0x9E37U+q*131+i*37);
  v->footprint110.pairs[i].y=(s16)(u16)(scenario*0x1234U-q*137-i*41);
 }
 for(unsigned i=0;i<4;i++)v->wheelPositions88[i].y=word(scenario*0x9E3779B9U+q*113+i*0x31415927U);
 v->rot54=word(scenario*0x1234567U+q);
}
static void project(const void *p){
 assert(commands<4);u32 xy=get(p),z=low((const u8*)p+4);
 assert((s16)(u16)xy==nextInput[0]&&(s16)(u16)(xy>>16)==nextInput[1]&&(s16)(u16)z==nextInput[2]);
 data[0]=xy;data[1]=z;event(0x100,0,xy);event(0x101,0,z);event(0x600,0,0x4A486012U);
 data[9]=(u32)half((xy+control[0])^(scenario+commands));
 data[10]=(u32)half(z^control[2]^(commands*0x1717U));
 data[11]=(u32)half((xy>>16)+control[3]+scenario);
 mutate(&world.cars[selected]);mutate(&expected.cars[selected]);
 nextInput[0]=expected.cars[selected].footprint110.pairs[commands+1].x;
 nextInput[1]=(s16)expected.cars[selected].wheelPositions88[commands].y;
 nextInput[2]=expected.cars[selected].footprint110.pairs[commands+1].y;commands++;
}
static void storeIR(void *p){
 unsigned i=commands-1;assert(i<4&&p==&world.cars[selected].wheelPositions88[i]);
 for(unsigned j=0;j<3;j++){
  event(0x209+j,0x80200010U+selected*0x320+0x88+i*16+j*4,data[9+j]);
  memcpy((u8*)p+j*4,&data[9+j],4);
 }
 expected.cars[selected].wheelPositions88[i].x=word(data[9]);
 expected.cars[selected].wheelPositions88[i].y=word(data[10]);
 expected.cars[selected].wheelPositions88[i].z=word(data[11]);
}
#define RR_GTE_SETROT(p) setRotation(p)
#define RR_GTE_MVMVA_ROT_V0_NOCV(p) project(p)
#define RR_GTE_STORE_IR_MEM(p) storeIR(p)
'''
TAIL=r'''
void run_wheels(unsigned number,const u8 *input,u8 *output){
 assert(sizeof(void*)==8&&(uintptr_t)&world>UINT32_MAX);
 assert(sizeof(VehicleFootprint20)==20&&offsetof(RaceVehicle,footprint110)==0x110);
 scenario=number;selected=number&1;commands=events=0;memset(output,0,OUTPUT_SIZE);trace=(u32*)(output+4);
 rr_vehicle_wire_reset();memset(&world,0xA5,sizeof(world));
 for(unsigned i=0;i<2;i++)rr_vehicle_wire_load(&world.cars[i],input+i*0x320);
 expected=world;
 for(unsigned i=0;i<32;i++){data[i]=number*0x1234567U+i;control[i]=number*0x31415927U+i;}
 nextInput[0]=world.cars[selected].footprint110.pairs[0].x;
 nextInput[1]=(s16)world.cars[selected].wheelPositions88[0].y;
 nextInput[2]=world.cars[selected].footprint110.pairs[0].y;
 ComputeWheelOffsets(&world.cars[selected]);assert(commands==4&&events==31);
 assert(!memcmp(&world,&expected,sizeof(world)));
 memcpy(output,&events,4);
 for(unsigned i=0;i<2;i++)rr_vehicle_wire_store(output+4+32*12+i*0x320,&world.cars[i]);
 memcpy(output+4+32*12+1600,data,128);memcpy(output+4+32*12+1600+128,control,128);
}
'''


def main():
    body=definition_source(source_owners(ROOT/'src/main/PAL')['ComputeWheelOffsets'].read_text(),'ComputeWheelOffsets')
    assert '__asm__' not in body and 'volatile' not in body and 'footprint110.pairs[n]' in body
    reference=ROTATION_SOURCE[ROTATION_SOURCE.index('static u32 table'):ROTATION_SOURCE.index('void run_case')]
    reference=reference.replace('MatrixWords32 *RotMatrix(', 'static MatrixWords32 *referenceRotation(')
    entry=address_of('ComputeWheelOffsets');rotate=address_of('RotMatrix')
    exe=(ROOT/'assets/PAL/main.exe').read_bytes();base,=struct.unpack_from('<I',exe,0x18)
    table=exe[0x800+TABLE-base:0x800+TABLE-base+16384]
    memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000)
    write=lambda a,b:memory.mem_write(a&0x1FFFFFFF,bytes(b));read=lambda a,n:bytes(memory.mem_read(a&0x1FFFFFFF,n))
    write(base,exe[0x800:])
    def store_guard(cpu,access,address,size,value,user):
        a=address&0x1FFFFFFF
        assert (STACK-72)&0x1FFFFFFF<=a and a+size<=STACK&0x1FFFFFFF or any(
            ((CARS+b*0x320+0x88+i*16)&0x1FFFFFFF)<=a and a+size<=((CARS+b*0x320+0x94+i*16)&0x1FFFFFFF)
            for b in range(2) for i in range(4)),(hex(address),size)
    cpu=Cpu(memory,0,100,101,102,store_guard)
    with TemporaryDirectory() as directory:
        source,library=Path(directory)/'native.c',Path(directory)/'native.dylib'
        source.write_text(PREFIX+reference+HARDWARE+body+TAIL.replace('OUTPUT_SIZE',str(OUTPUT)))
        subprocess.run(['cc','-std=gnu11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                        '-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I'+str(ROOT/'include'),
                        '-I'+str(ROOT/'tools/scripts'),str(source),'-o',str(library)],check=True)
        native=C.CDLL(str(library));native.init_table.argtypes=[C.c_void_p];native.init_table(C.create_string_buffer(table))
        native.run_wheels.argtypes=[C.c_uint,C.c_void_p,C.c_void_p]
        for version in ('original','linked'):
            if version=='linked':
                with (ROOT/'build/PAL/main.elf').open('rb') as stream:
                    elf=ELFFile(stream)
                    for name in ('ComputeWheelOffsets','RotMatrix'):
                        symbol=elf.get_section_by_name('.symtab').get_symbol_by_name(name)[0];section=elf.get_section(symbol['st_shndx'])
                        off=symbol['st_value']-section['sh_addr'];write(symbol['st_value'],section.data()[off:off+symbol['st_size']])
            for case in range(CASES):
                state=bytes((case*19+i*29)&255 for i in range(STATE));output=C.create_string_buffer(OUTPUT)
                native.run_wheels(case,C.create_string_buffer(state),output);expected=output.raw
                write(CARS,state);write(STACK-256,b'\xA6'*512);cpu.reset();cpu.r=[0]+[0xBEEF0000+r for r in range(1,32)]
                selected=case&1;car=CARS+selected*0x320;cpu.r[4]=car;cpu.r[29]=STACK;cpu.r[31]=STOP;cpu.pc=entry
                data=[(case*0x1234567+i)&0xFFFFFFFF for i in range(32)];control=[(case*0x31415927+i)&0xFFFFFFFF for i in range(32)]
                events=[];commands=0
                for step in range(1000):
                    if cpu.pc==STOP:break
                    if cpu.pc==rotate:
                        events.extend((0x700,0,int.from_bytes(read(cpu.r[4],4),'little'),0x701,0,int.from_bytes(read(cpu.r[4]+4,2),'little')))
                    ins=int.from_bytes(read(cpu.pc,4),'little');op=ins>>26
                    if op not in (0x12,0x32,0x3A):cpu.step();continue
                    delayed=cpu.delay;cpu.delay=None;rs,rt,rd=(ins>>21)&31,(ins>>16)&31,(ins>>11)&31
                    if op in (0x32,0x3A):
                        a=(cpu.r[rs]+signed(ins,16))&0xFFFFFFFF
                        if op==0x32:
                            assert rt in (0,1);data[rt]=int.from_bytes(read(a,4 if rt==0 else 2),'little');events.extend((0x100+rt,0,data[rt]))
                        else:events.extend((0x200+rt,a,data[rt]));cpu.store(a,data[rt],4)
                    elif ins&0x02000000:
                        assert ins==0x4A486012 and commands<4;events.extend((0x600,0,ins));q=commands
                        data[9]=signed((data[0]+control[0])^(case+q),16)&0xFFFFFFFF
                        data[10]=signed(data[1]^control[2]^(q*0x1717),16)&0xFFFFFFFF
                        data[11]=signed((data[0]>>16)+control[3]+case,16)&0xFFFFFFFF
                        for i in range(5):write(car+0x110+i*4,struct.pack('<HH',(case*0x9E37+q*131+i*37)&65535,(case*0x1234-q*137-i*41)&65535))
                        for i in range(4):write(car+0x8C+i*16,struct.pack('<I',(case*0x9E3779B9+q*113+i*0x31415927)&0xFFFFFFFF))
                        write(car+0x54,struct.pack('<I',(case*0x1234567+q)&0xFFFFFFFF));commands+=1
                    else:
                        assert rs==6 and rd<5;value=cpu.r[rt]&(65535 if rd==4 else 0xFFFFFFFF)
                        control[rd]=value;events.extend((0x500+rd,0,value))
                    cpu.pc=delayed if delayed is not None else cpu.pc+4
                assert cpu.pc==STOP and commands==4 and len(events)==31*3
                actual=struct.pack('<I',31)+struct.pack('<'+'I'*len(events),*events)+bytes(12)+read(CARS,STATE)+struct.pack('<64I',*data,*control)
                assert actual==expected,(version,case,'actual body / full owners / defined GTE transport')
                for r in (*range(16,24),28,30):assert cpu.r[r]==0xBEEF0000+r
                assert cpu.r[29]==STACK and cpu.r[31]==STOP
                assert read(STACK-256,184)==b'\xA6'*184 and read(STACK,256)==b'\xA6'*256
            print('PASS',version,CASES,'actual wheel projection flows; original RotMatrix math, native car/footprint owners, live prefetch before stores, defined GTE transfers, complete state, ABI, guards and UBSan',flush=True)


if __name__=='__main__':main()
