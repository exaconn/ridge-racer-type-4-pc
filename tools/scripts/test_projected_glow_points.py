"""Actual projected-point emitter vs original/linked MIPS and a wire oracle.

RTPS results are scripted: this checks C ownership, transport and fan/DMA
construction, not GTE numerical accuracy, flags, pipeline timing or the caller.
Only the one retained, pre-existing physical register name is removed on LP64.
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
NAME = 'EmitProjectedLitQuadsGT4'
SCRATCH, WORK, STREAM, GPU = 0x1F800000, 0x1F80007C, 0x80200010, 0x80180000
STACK, STOP = 0x803F0000, 0x803FF000
CAPACITY, MAX_EVENTS = 24, 1400
RING, MATRIX, STAGE, FIELDS, PIXELS = 192, 264, 304, 784, 812
STATE = PIXELS + 16384
OUTPUT = 8 + MAX_EVENTS*12 + STATE + 256
DEPTHS = (-2147483648,-1,0,1,447,448,449,895,896,1791,1792,65535,2147483647,128,256,32767)
COUNTS = (-32768,-1,0,1,2,3,4,24)
FACING = (-2147483648,-2147483647,-65,-64,-63,-1,0,1,63,64,65,2147483647)

PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "rr/rro_effects.h"
#include "rr/gpu_address.h"
typedef union {u32 words[4096];GpuPolyG3 triangles[585];} GpuArena;
static struct World {
 u8 before[16];RroEffectScratch scratch;u8 gap[16];
 RroGlowPointCommand input[24];Vec3s8 ring[9];GpuArena gpu;u8 after[16];
} world,expected;
#define SCRATCHPAD ((u8*)&world.scratch.scene)
#define SpotlightConeRingOuter world.ring
#define WORK world.scratch.work7C
#define SCENE world.scratch.scene
static unsigned scenario,commands,events,points;static u32 data[32],control[32],*trace;
static GpuDmaRegion region;
static const s32 depths[]={-2147483647-1,-1,0,1,447,448,449,895,896,1791,1792,65535,2147483647,128,256,32767};
static u32 get(const void *p){u32 v;memcpy(&v,p,4);return v;}
static void put(void *p,u32 v){memcpy(p,&v,4);}
static s32 word(u32 v){s32 x;memcpy(&x,&v,4);return x;}
static u32 address(const void *p){
 uintptr_t a=(uintptr_t)p,b=(uintptr_t)world.gpu.words;
 if(a>=b&&a-b<sizeof(world.gpu))return 0x80180000U+(u32)(a-b);
 if(!p)return 0;
 b=(uintptr_t)world.input;if(a>=b&&a-b<sizeof(world.input))return 0x80200010U+(u32)(a-b);
 b=(uintptr_t)world.ring;if(a>=b&&a-b<72)return FAN_ADDRESS+(u32)(a-b);
 b=(uintptr_t)&WORK.matrix44;if(a>=b&&a-b<40)return 0x1F8000C0U+(u32)(a-b);
 if(p==&WORK.depth70)return 0x1F8000ECU;
 for(unsigned i=0;i<24;i++){
  b=(uintptr_t)&WORK.projection188.glow.points[i];
  if(a>=b&&a-b<16)return 0x1F80021CU+i*20+(u32)(a-b);
 }
 assert(!"unknown or truncated native pointer");return 0;
}
static void *resolve(u32 a){
 if(!a)return 0;
 u32 d=a-0x80180000U;assert(d<sizeof(world.gpu));return (u8*)world.gpu.words+d;
}
static void loadState(struct World *w,const u8 *p){
 memcpy(w->input,p,192);memcpy(w->ring,p+192,72);memcpy(&w->scratch.work7C.matrix44,p+264,40);
 for(unsigned i=0;i<24;i++){
  RroGlowProjection *q=&w->scratch.work7C.projection188.glow.points[i];
  memcpy(&q->translation,p+304+i*20,16);q->ordering=resolve(get(p+320+i*20));
 }
 w->scratch.scene.view.projection.pointers.cursor=resolve(get(p+784));
 w->scratch.scene.depthShift60=word(get(p+788));w->scratch.work7C.depth70=word(get(p+792));
 w->scratch.work7C.facing98.x=word(get(p+796));w->scratch.work7C.facing98.y=word(get(p+800));
 w->scratch.work7C.ordering6C=resolve(get(p+804));w->scratch.work7C.colors15C.palette.rgb160[0]=word(get(p+808));
 memcpy(w->gpu.words,p+812,16384);
}
static void snapshot(u8 *p){
 memcpy(p,world.input,192);memcpy(p+192,world.ring,72);memcpy(p+264,&WORK.matrix44,40);
 for(unsigned i=0;i<24;i++){
  RroGlowProjection *q=&WORK.projection188.glow.points[i];memcpy(p+304+i*20,&q->translation,16);put(p+320+i*20,address(q->ordering));
 }
 put(p+784,address(SCENE.view.projection.pointers.cursor));put(p+788,(u32)SCENE.depthShift60);put(p+792,(u32)WORK.depth70);
 put(p+796,(u32)WORK.facing98.x);put(p+800,(u32)WORK.facing98.y);put(p+804,address(WORK.ordering6C));
 put(p+808,(u32)WORK.colors15C.palette.rgb160[0]);memcpy(p+812,world.gpu.words,16384);
}
static void event(u32 kind,u32 at,u32 value){assert(events++<1400);*trace++=kind;*trace++=at;*trace++=value;}
static void load(unsigned reg,const void *p){data[reg]=get(p);event(0x100+reg,address(p),data[reg]);}
static void store(unsigned reg,void *p){event(0x200+reg,address(p),data[reg]);put(p,data[reg]);}
static s32 readData(unsigned reg){event(0x300+reg,0,data[reg]);return word(data[reg]);}
static void writeControl(unsigned reg,u32 value){event(0x500+reg,0,value);control[reg]=value;}
static void mutate(void){
 unsigned q=commands,at=(q*3)%18;
 put((u8*)world.ring+at*4,get((u8*)world.ring+at*4)^(scenario*0x17171717U+q));
 SCENE.depthShift60=(s32)((scenario+q+11)&31)-2;
 SCENE.view.projection.pointers.cursor=(u8*)world.gpu.words+(scenario>=512?2052:8192+(q&1)*256);
 WORK.facing98.x=word(scenario*0x9E3779B9U+q);WORK.facing98.y=word(scenario*0x31415927U-q);
 WORK.ordering6C=world.gpu.words+512+(q&1)*256;
 WORK.colors15C.palette.rgb160[0]=word(scenario*0x1234567U+q*0x17171717U);
 if(q<points)world.input[q].fields.orderingBias07=(s8)(u8)(scenario+q*37);
}
static void rtps(const void *p){
 load(0,p);load(1,(const u8*)p+4);event(0x600,0,0x4A180001U);
 unsigned q=commands;
 data[14]=data[0]^control[5]^(scenario*0x1020304U+q*0x10001U);
 data[9]=(u32)(s32)(s16)(u16)(scenario*13+q*17);data[10]=(u32)(s32)(s16)(u16)(scenario*19+q*23);
 data[11]=(u32)(s32)(s16)(u16)(scenario*29+q*31);
 data[19]=(u32)((scenario==1||scenario>=256)?1:depths[(scenario+q*3)&15]);mutate();commands++;
}
static void setMatrix(const void *p,unsigned first,unsigned n){
 for(unsigned i=0;i<n;i++)writeControl(first+i,get((const u8*)p+i*4));
}
static void storeIR(void *p){for(unsigned i=0;i<3;i++)store(9+i,(u8*)p+i*4);}
#define RR_GTE_RTPS_LDV(p) rtps(p)
#define RR_GTE14_SWC2_SZ3(p) store(19,p)
#define RR_GTE_STORE_IR_MEM(p) storeIR(p)
#define RR_GTE14_SWC2_SXY2(p) store(14,p)
#define RR_GTE_SETROT(p) setMatrix(p,0,5)
#define RR_GTE_SETTR(p) setMatrix(p,5,3)
#define RR_GTE_MFC2(out,reg) ((out)=readData(reg))
#define RR_GTE_MFC2_SXY2_NOP(out) ((out)=readData(14))
'''
TAIL = r'''
void run_case(unsigned flow,s32 count,const u8 *input,const u8 *wanted,u8 *output){
 assert(sizeof(void*)==8&&(uintptr_t)&world>UINT32_MAX);
 assert(sizeof(RroGlowProjection)==24&&offsetof(RroGlowProjection,ordering)==16);
 if(!region.native){region=(GpuDmaRegion){&world.gpu,0x80180000U,sizeof(world.gpu)};assert(GpuDmaSetRegions(&region,1));}
 memset(&world,0xA5,sizeof(world));loadState(&world,input);expected=world;loadState(&expected,wanted);
 scenario=flow;commands=events=0;points=count>0?(unsigned)count:0;memset(output,0,OUTPUT_SIZE);trace=(u32*)(output+8);
 for(unsigned i=0;i<32;i++){data[i]=flow*0x1234567U+i*0x17171717U;control[i]=flow*0x31415927U+i;}
 const RroGlowPointCommand *end=EmitProjectedLitQuadsGT4(world.input,count,&WORK);
 assert(end==world.input+points);assert(!memcmp(&world,&expected,sizeof(world)));
 put(output,events);put(output+4,0x80200010U+(u32)(end-world.input)*8);snapshot(output+8+1400*12);
 memcpy(output+8+1400*12+STATE_SIZE,data,128);memcpy(output+8+1400*12+STATE_SIZE+128,control,128);
}
'''


def oracle(flow, count, incoming, fan):
    s = bytearray(incoming);trace = [];commands = 0;n = max(count,0)
    get = lambda at: struct.unpack_from('<I',s,at)[0]
    def put(at,v):struct.pack_into('<I',s,at,v&0xFFFFFFFF)
    def event(kind,at,value):trace.extend((kind,at,value&0xFFFFFFFF))
    def offset(a):
        if STREAM <= a < STREAM+192:return a-STREAM
        if fan <= a < fan+72:return RING+a-fan
        if GPU <= a < GPU+16384:return PIXELS+a-GPU
        if WORK+0x44 <= a < WORK+0x6C:return MATRIX+a-WORK-0x44
        if WORK+0x1A0 <= a < WORK+0x380:return STAGE+a-WORK-0x1A0
        assert a == WORK+0x70,hex(a)
        return FIELDS+8
    def load(reg,a):data[reg]=get(offset(a));event(0x100+reg,a,data[reg])
    def store(reg,a):event(0x200+reg,a,data[reg]);put(offset(a),data[reg])
    def read(reg):event(0x300+reg,0,data[reg]);return data[reg]
    def ctl(reg,v):event(0x500+reg,0,v);control[reg]=v
    def rtps(a):
        nonlocal commands
        load(0,a);load(1,a+4);event(0x600,0,0x4A180001);q=commands
        data[14]=(data[0]^control[5]^(flow*0x1020304+q*0x10001))&0xFFFFFFFF
        data[9]=signed(flow*13+q*17,16)&0xFFFFFFFF;data[10]=signed(flow*19+q*23,16)&0xFFFFFFFF
        data[11]=signed(flow*29+q*31,16)&0xFFFFFFFF;data[19]=(1 if flow==1 or flow>=256 else DEPTHS[(flow+q*3)&15])&0xFFFFFFFF
        at=RING+(q*3%18)*4;put(at,get(at)^(flow*0x17171717+q))
        put(FIELDS+4,((flow+q+11)&31)-2);put(FIELDS,GPU+(2052 if flow>=512 else 8192+(q&1)*256))
        put(FIELDS+12,flow*0x9E3779B9+q);put(FIELDS+16,flow*0x31415927-q)
        put(FIELDS+20,GPU+2048+(q&1)*1024);put(FIELDS+24,flow*0x1234567+q*0x17171717)
        if q<n:s[q*8+7]=(flow+q*37)&255
        commands+=1
    def trunc64(x):return -(abs(x)//64) if x<0 else x//64
    dx=trunc64(signed(-get(FIELDS+12)&0xFFFFFFFF,32));dy=trunc64(signed(-get(FIELDS+16)&0xFFFFFFFF,32))
    shift=signed(get(FIELDS+4),32)+2;data=[(flow*0x1234567+i*0x17171717)&0xFFFFFFFF for i in range(32)]
    control=[(flow*0x31415927+i)&0xFFFFFFFF for i in range(32)];paths=set()
    if not n:paths.add('empty')
    for i in range(n):
        rtps(STREAM+i*8);store(19,WORK+0x70);raw=signed(data[19],32);depth=raw>>shift;at=STAGE+i*20
        if raw<=0 or depth>=448:put(at+16,0);paths.add('nonpositive' if raw<=0 else 'far');continue
        for j in range(3):store(9+j,WORK+0x1A0+i*20+j*4)
        store(14,WORK+0x1AC+i*20);put(at,get(at)+dx);put(at+4,get(at+4)+dy)
        put(at+16,get(FIELDS+20)+(depth+signed(s[i*8+7],8))*4);paths.add('drawn')
    for i,v in [(5,4096),(3,4096),(1,4096),(4,0),(2,0)]:put(MATRIX+i*4,v)
    for i in range(5):ctl(i,get(MATRIX+4+i*4))
    packet=get(FIELDS);first=None;valid=0
    for i in range(n):
        at=STAGE+i*20;head=get(at+16)
        if not head:continue
        if first is None:first=(head,packet)
        valid+=1
        for j in range(3):ctl(5+j,get(at+j*4))
        center=get(at+12);rtps(fan);previous=read(14);color=get(FIELDS+24)
        for j in range(8):
            rtps(fan+(j+1)*8);p=PIXELS+packet-GPU;h=PIXELS+head-GPU
            put(p+8,center);put(p+16,previous);put(p+4,color);put(p+20,0);put(p+12,0)
            put(p,get(h));s[p+3]=6;put(h,packet);s[h+3]=0
            previous=read(14);put(p+24,previous);packet+=28
        p=PIXELS+packet-GPU;h=PIXELS+head-GPU
        put(p+4,0xE1000220);put(p,get(h));s[p+3]=1;put(h,packet);packet+=8;s[h+3]=0
    put(FIELDS,packet)
    assert commands==n+9*valid and len(trace)//3<=MAX_EVENTS
    out=struct.pack('<II',len(trace)//3,STREAM+n*8)+struct.pack('<'+'I'*len(trace),*trace)
    out+=bytes(MAX_EVENTS*12-len(trace)*4)+s+struct.pack('<64I',*data,*control)
    return bytes(s),out,paths,first


def main():
    owner=source_owners(ROOT/'src/main/PAL')[NAME];body=definition_source(owner.read_text(),NAME)
    pin='register s32 previous __asm__("$15");';assert body.count(pin)==1
    body=body.replace(pin,'s32 previous;');assert '__asm__' not in body and 'volatile' not in body
    fan=address_of('SpotlightConeRingOuter');entry=address_of(NAME)
    memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000);memory.mem_map(SCRATCH,4096)
    write=lambda a,b:memory.mem_write(a&0x1FFFFFFF,bytes(b));read=lambda a,n:bytes(memory.mem_read(a&0x1FFFFFFF,n))
    exe=(ROOT/'assets/PAL/main.exe').read_bytes();base,=struct.unpack_from('<I',exe,0x18);write(base,exe[0x800:])
    regions=[(STREAM,192),(fan,72),(WORK+0x44,40),(WORK+0x1A0,480),(SCRATCH,4),(SCRATCH+0x60,4),
             (WORK+0x70,4),(WORK+0x98,8),(WORK+0x6C,4),(WORK+0x160,4),(GPU,16384)]
    assert sum(n for a,n in regions)==STATE
    def snapshot():return b''.join(read(a,n) for a,n in regions)
    def restore(state):
        at=0
        for a,n in regions:write(a,state[at:at+n]);at+=n
    def store_guard(cpu,access,address,size,value,user):
        a=address&0x1FFFFFFF
        assert any((b&0x1FFFFFFF)<=a and a+size<=(b&0x1FFFFFFF)+n for b,n in
                   [(SCRATCH,4),(WORK+0x48,20),(WORK+0x70,4),(WORK+0x1A0,480),(GPU,16384),(STACK-8,8)]),(hex(address),size)
    cpu=Cpu(memory,0,100,101,102,store_guard);cases=[]
    for case in range(2394):
        forced=case>=2048;flow=1 if forced else case;count=1 if forced else COUNTS[case%8]
        if count==24 and case%511:count=4
        if case in (511,1023,1535,2047):count=24
        shift=0 if forced else (case>>3)&31
        s=bytearray((case*19+i*29)&255 for i in range(STATE))
        for i in range(24):struct.pack_into('<I',s,STAGE+i*20+16,GPU+1024+i*4)
        struct.pack_into('<7I',s,FIELDS,GPU+8192,(shift-2)&0xFFFFFFFF,0,
                         FACING[case%12]&0xFFFFFFFF,FACING[(case//12)%12]&0xFFFFFFFF,GPU+2048,case*0x17171717&0xFFFFFFFF)
        if 2048<=case<2304:flow=case-2048+256
        elif 2304<=case<2362:flow=case-2304+512
        elif case>=2362:flow=1;shift=case-2362;struct.pack_into('<I',s,FIELDS+4,(shift-2)&0xFFFFFFFF)
        cases.append((flow,count,s,shift))
    with TemporaryDirectory() as directory:
        source=Path(directory)/'native.c';library=Path(directory)/'native.dylib'
        source.write_text(PREFIX.replace('FAN_ADDRESS',hex(fan)+'U')+body+TAIL.replace('OUTPUT_SIZE',str(OUTPUT)).replace('STATE_SIZE',str(STATE)))
        subprocess.run(['cc','-std=c11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                        '-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I'+str(ROOT/'include'),str(source),
                        str(ROOT/'src/native/gpu_address.c'),'-o',str(library)],check=True)
        native=C.CDLL(str(library));native.run_case.argtypes=[C.c_uint,C.c_int32,C.c_void_p,C.c_void_p,C.c_void_p]
        for version in ('original','linked'):
            if version=='linked':
                with (ROOT/'build/PAL/main.elf').open('rb') as stream:
                    elf=ELFFile(stream);symbol=elf.get_section_by_name('.symtab').get_symbol_by_name(NAME)[0]
                    assert symbol['st_value']==entry and symbol['st_size']==0x268
                    section=elf.get_section(symbol['st_shndx']);off=entry-section['sh_addr'];write(entry,section.data()[off:off+symbol['st_size']])
            paths=set();aliases=set();biases=set();shifts=set();counts=set()
            for case,(flow,count,state,shift) in enumerate(cases):
                wanted,expected,seen,first=oracle(flow,count,state,fan);output=C.create_string_buffer(OUTPUT)
                native.run_case(flow,count,C.create_string_buffer(bytes(state)),C.create_string_buffer(wanted),output)
                if output.raw!=expected:
                    differences=[(i,output.raw[i:i+4].hex(),expected[i:i+4].hex()) for i in range(0,OUTPUT,4) if output.raw[i:i+4]!=expected[i:i+4]]
                    raise AssertionError((version,case,'native/wire',differences[:12]))
                restore(state);write(STACK-256,b'\xA6'*512);cpu.reset();cpu.r=[0]+[0xBEEF0000+r for r in range(1,32)]
                cpu.r[4:7]=[STREAM,count&0xFFFFFFFF,WORK];cpu.r[29]=STACK;cpu.r[31]=STOP;cpu.pc=entry
                data=[(flow*0x1234567+i*0x17171717)&0xFFFFFFFF for i in range(32)];control=[(flow*0x31415927+i)&0xFFFFFFFF for i in range(32)]
                events=[];commands=0
                for step in range(20000):
                    if cpu.pc==STOP:break
                    ins=int.from_bytes(read(cpu.pc,4),'little');op=ins>>26
                    if op not in (0x12,0x32,0x3A):cpu.step();continue
                    delayed=cpu.delay;cpu.delay=None;rs,rt,rd=(ins>>21)&31,(ins>>16)&31,(ins>>11)&31
                    if op in (0x32,0x3A):
                        a=(cpu.r[rs]+signed(ins,16))&0xFFFFFFFF
                        if op==0x32:data[rt]=int.from_bytes(read(a,4),'little');events.extend((0x100+rt,a,data[rt]))
                        else:events.extend((0x200+rt,a,data[rt]));cpu.store(a,data[rt],4)
                    elif ins&0x02000000:
                        assert ins==0x4A180001;events.extend((0x600,0,ins));q=commands
                        data[14]=(data[0]^control[5]^(flow*0x1020304+q*0x10001))&0xFFFFFFFF
                        data[9]=signed(flow*13+q*17,16)&0xFFFFFFFF;data[10]=signed(flow*19+q*23,16)&0xFFFFFFFF
                        data[11]=signed(flow*29+q*31,16)&0xFFFFFFFF;data[19]=(1 if flow==1 or flow>=256 else DEPTHS[(flow+q*3)&15])&0xFFFFFFFF
                        changed=bytearray(snapshot());at=RING+(q*3%18)*4
                        def put(at,v):struct.pack_into('<I',changed,at,v&0xFFFFFFFF)
                        put(at,int.from_bytes(changed[at:at+4],'little')^(flow*0x17171717+q))
                        put(FIELDS+4,((flow+q+11)&31)-2);put(FIELDS,GPU+(2052 if flow>=512 else 8192+(q&1)*256))
                        put(FIELDS+12,flow*0x9E3779B9+q);put(FIELDS+16,flow*0x31415927-q)
                        put(FIELDS+20,GPU+2048+(q&1)*1024);put(FIELDS+24,flow*0x1234567+q*0x17171717)
                        if q<max(count,0):changed[q*8+7]=(flow+q*37)&255
                        restore(changed);commands+=1
                    elif rs==0:events.extend((0x300+rd,0,data[rd]));cpu.r[rt]=data[rd]
                    else:
                        assert rs==6;events.extend((0x500+rd,0,cpu.r[rt]));control[rd]=cpu.r[rt]
                    cpu.pc=delayed if delayed is not None else cpu.pc+4
                assert cpu.pc==STOP,(version,case,hex(cpu.pc))
                actual=struct.pack('<II',len(events)//3,cpu.r[2])+struct.pack('<'+'I'*len(events),*events)
                actual+=bytes(MAX_EVENTS*12-len(events)*4)+snapshot()+struct.pack('<64I',*data,*control)
                assert actual==expected,(version,case,'MIPS/wire')
                for r in (*range(16,24),28,30):assert cpu.r[r]==0xBEEF0000+r
                assert cpu.r[29]==STACK and cpu.r[31]==STOP
                assert read(STACK-256,248)==b'\xA6'*248 and read(STACK,256)==b'\xA6'*256
                paths|=seen;shifts.add(shift);counts.add(count)
                if first is not None:
                    bias=signed(wanted[7],8);biases.add(bias)
                    delta=first[0]-first[1]
                    if 0<=delta<232:aliases.add(delta//4)
            assert paths=={'empty','nonpositive','far','drawn'},paths
            assert shifts==set(range(32)) and counts==set(COUNTS)
            assert aliases==set(range(58)),aliases
            assert biases==set(range(-128,128)),len(biases)
            print('PASS',version,len(cases),'projected glow flows; full-width staged OT pointers, 24-point capacity, live roots/bias/ring/color, captured offsets/shift, all256 biases/32 shifts/58 head overlaps, signed depths/counts, exact GTE transfers, complete owners, guards and UBSan',flush=True)


if __name__=='__main__':main()
