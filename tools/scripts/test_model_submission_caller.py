"""Actual intro caller, typed scratch owner and full-width submission interface.

Matrix builders, bank selection, object transform and submission are service
boundaries. GTE matrix transports are executed and checked; this fixture does
not test the algorithms inside those services or the handwritten dispatcher.
"""
import ctypes as C
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN

from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
CASES, STATE, MAX_EVENTS = 2048, 1644, 24
EVENT = STATE+20
OUTPUT = 8+MAX_EVENTS*EVENT+STATE
SCRATCH, STATES, STACK, STOP = 0x1F800000, (0x80200010, 0x80201010), 0x803F0000, 0x803FF000
POOL = (0x80210010, 0x80211010, 0x80212010, 0x80213010, 0x80214010, 0x80215010, 0x80216010, 0x80217010)
TABLES = (0x80218010, 0x80219010)
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "rr/rro_model.h"
#include "game/gfx/model.h"
#include "game/gfx/skyrot1.h"
#include "game/camera/viewcam.h"
#include "psyq/libgte/gtemat2.h"
static struct {
 u8 before[16];RroModelScratch scratch;u8 middle[16];
 IntroSceneState states[2];s16 positions[8][8];
 u32 gpu[2][64],streams[2][16];Vec3s8 vertices[2][4],normals[2][4];
 RroBankCommand tables[2][2];u8 after[16];
} world;
#define SCRATCHPAD ((u8*)&world.scratch)
#define RotatingSkyTransforms ((char*)world.positions)
IntroSceneState *IntroSceneStatePtr;
s32 ModelBankFormat;
static u32 control[32];
static unsigned scenario,events,multiplies;
static MatrixWords32 *temporary;
static u8 *trace;
enum {STATE=1644,EVENT=1664,MAX_EVENTS=24};
static u32 get(const void *p){u32 v;memcpy(&v,p,4);return v;}
static void put(void *p,u32 v){memcpy(p,&v,4);}
static u32 address(const void *p) {
 if(!p)return 0;
 if(p==&world.scratch.scene)return 0x1F800000;
 if(p==&world.scratch.work7C)return 0x1F80007C;
 if(p==&world.scratch.work7C.setup.rotation)return 0x1F800084;
 if(p==&world.scratch.work7C.setup.light)return 0x1F8000A4;
 for(unsigned i=0;i<2;i++) {
  if(p==&world.states[i])return 0x80200010U+i*0x1000U;
  if(p==world.tables[i])return 0x80218010U+i*0x1000U;
 }
 const struct {const void *p;u32 a;size_t n;} regions[]={
  {world.positions,positionAddress,128},
  {world.gpu[0],0x80210010,256},{world.gpu[1],0x80211010,256},
  {world.streams[0],0x80212010,64},{world.streams[1],0x80213010,64},
  {world.vertices[0],0x80214010,32},{world.vertices[1],0x80215010,32},
  {world.normals[0],0x80216010,32},{world.normals[1],0x80217010,32}};
 uintptr_t q=(uintptr_t)p;
 for(unsigned i=0;i<sizeof(regions)/sizeof(regions[0]);i++) {
  uintptr_t b=(uintptr_t)regions[i].p;
  if(q>=b&&q-b<regions[i].n)return regions[i].a+(u32)(q-b);
 }
 assert(!"unknown native pointer");return 0;
}
static void *resolve(u32 a) {
 for(unsigned i=0;i<2;i++) {
  if(a==0x80200010U+i*0x1000U)return &world.states[i];
  if(a==0x80218010U+i*0x1000U)return world.tables[i];
 }
 const struct {void *p;u32 a,n;} regions[]={
  {world.positions,positionAddress,128},
  {world.gpu[0],0x80210010,256},{world.gpu[1],0x80211010,256},
  {world.streams[0],0x80212010,64},{world.streams[1],0x80213010,64},
  {world.vertices[0],0x80214010,32},{world.vertices[1],0x80215010,32},
  {world.normals[0],0x80216010,32},{world.normals[1],0x80217010,32}};
 if(!a)return NULL;
 for(unsigned i=0;i<sizeof(regions)/sizeof(regions[0]);i++)
  if(a>=regions[i].a&&a-regions[i].a<regions[i].n)return (u8*)regions[i].p+a-regions[i].a;
 assert(!"unknown fixture address");return NULL;
}
static void snapshot(u8 *out) {
 RroSceneScratchPrefix *s=&world.scratch.scene;
 put(out,address(s->view.projection.pointers.cursor));put(out+4,address(s->view.projection.pointers.order));
 memcpy(out+8,&s->view.projection.pose08,32);memcpy(out+40,&s->view.matrix28,32);
 put(out+72,address(s->modelEntries48));put(out+76,s->reserved4C);
 put(out+80,address(s->commands50));put(out+84,address(s->vertices54));put(out+88,address(s->normals58));
 put(out+92,s->reserved5C);put(out+96,(u32)s->depthShift60);put(out+100,(u32)s->renderFlags64);
 put(out+104,s->reserved68);memcpy(out+108,&s->viewport6C,8);
 put(out+116,address(s->draw74.draw.primitive));put(out+120,(u32)s->draw74.draw.counter);
 memcpy(out+124,&world.scratch.work7C,96);
 for(unsigned i=0;i<2;i++) {
  u8 *p=out+220+i*188;memcpy(p,&world.states[i],64);put(p+64,address(world.states[i].p40));
  memcpy(p+68,&world.states[i].s44,120);
 }
 memcpy(out+596,world.positions,128);memcpy(out+724,world.gpu,512);
 memcpy(out+1236,world.streams,128);memcpy(out+1364,world.vertices,64);memcpy(out+1428,world.normals,64);
 for(unsigned i=0;i<2;i++)for(unsigned j=0;j<2;j++)put(out+1492+i*8+j*4,address(world.tables[i][j].pointer));
 put(out+1508,address(IntroSceneStatePtr));put(out+1512,(u32)ModelBankFormat);memcpy(out+1516,control,128);
}
static unsigned record(u32 kind,u32 a,u32 b,u32 c,u32 d) {
 u32 words[5]={kind,a,b,c,d};unsigned serial=events++;
 assert(serial<MAX_EVENTS);memcpy(trace+serial*EVENT,words,20);snapshot(trace+serial*EVENT+20);return serial;
}
static u32 next(u32 x){return x*1664525U+1013904223U;}
static u32 hash(const void *p,unsigned n){const u8 *b=p;u32 x=0;for(unsigned i=0;i<n;i++)x=x*33U+b[i];return x;}
static void matrix_transport(unsigned first,const void *p) {
 for(unsigned i=0;i<5;i++) {
  u32 v=get((const u8*)p+i*4);
  if(i==4)v=(u32)(s32)(s16)v;
  control[first+i]=v;record(100+first+i,v,0,0,0);
 }
}
#define RR_GTE_SETROT(p) matrix_transport(0,p)
#define RR_GTE_SETLLM(p) matrix_transport(8,p)
void SelectModelBank(s32 slot) {
 record(1,(u32)slot,0,0,0);unsigned bank=scenario&1;
 RroSceneScratchPrefix *s=&world.scratch.scene;
 s->commands50=world.tables[bank];s->modelEntries48=world.streams[bank];
 s->vertices54=world.vertices[bank];s->normals58=world.normals[bank];
 IntroSceneStatePtr=&world.states[bank];
}
MatrixWords32 *RotMatrix(const Vec3sFields8 *angles,MatrixWords32 *matrix) {
 record(2,address(angles),address(matrix),0,0);
 u32 seed=hash(angles,6)^scenario;
 for(unsigned i=0;i<8;i++){seed=next(seed);put((u8*)matrix+i*4,seed);}
 if(scenario&2)IntroSceneStatePtr=&world.states[1-(scenario&1)];
 return 0;
}
MatrixWords32 *MulRotMatrix0(const MatrixWords32 *input,MatrixWords32 *output) {
 unsigned serial=multiplies++;assert(serial<2&&input==&world.scratch.work7C.setup.rotation);
 if(serial==0)assert(output==&world.scratch.work7C.setup.light);else temporary=output;
 record(3,address(input),serial,0,0);
 u32 seed=hash(input,32)^scenario;
 for(unsigned i=0;i<5;i++)seed^=control[i];
 for(unsigned i=0;i<8;i++){seed=next(seed);output->w[i]=(s32)seed;}
 return output;
}
void SetGteObjectTransform(ObjectTransform38 *output,const s16 *position,s32 *matrix) {
 assert(output==&world.scratch.work7C.transform&&matrix==temporary->w);
 record(4,address(output),address(position),hash(matrix,32),0);
 u32 seed=hash(position,6)^hash(matrix,32);
 for(unsigned i=0;i<14;i++){seed=next(seed);put((u8*)output+i*4,seed);}
 static const s32 formats[]={-2147483647,-1,0,1,2,2147483647};ModelBankFormat=formats[scenario%6];
}
void SubmitModel(RroSceneScratchPrefix *scene,s32 index,s32 flags,s32 bias) {
 assert(scene==&world.scratch.scene&&(uintptr_t)scene>UINT32_MAX);
 assert((uintptr_t)scene->commands50>UINT32_MAX&&scene->draw74.draw.counter==0);
 record(5,address(scene),(u32)index,(u32)flags,(u32)bias);
}
'''
TAIL = r'''
void run_case(unsigned number,s32 index,s32 animate,const u8 *input,u8 *output) {
 scenario=number;events=multiplies=0;temporary=NULL;trace=output+8;memset(&world,0xA5,sizeof(world));
 RroSceneScratchPrefix *s=&world.scratch.scene;
 s->view.projection.pointers.cursor=resolve(get(input));s->view.projection.pointers.order=resolve(get(input+4));
 memcpy(&s->view.projection.pose08,input+8,32);memcpy(&s->view.matrix28,input+40,32);
 s->modelEntries48=resolve(get(input+72));s->reserved4C=get(input+76);
 s->commands50=resolve(get(input+80));s->vertices54=resolve(get(input+84));s->normals58=resolve(get(input+88));
 s->reserved5C=get(input+92);s->depthShift60=(s32)get(input+96);s->renderFlags64=(s32)get(input+100);
 s->reserved68=get(input+104);memcpy(&s->viewport6C,input+108,8);
 s->draw74.draw.primitive=resolve(get(input+116));s->draw74.draw.counter=(s32)get(input+120);
 memcpy(&world.scratch.work7C,input+124,96);
 for(unsigned i=0;i<2;i++) {
  const u8 *p=input+220+i*188;memcpy(&world.states[i],p,64);world.states[i].p40=resolve(get(p+64));
  memcpy(&world.states[i].s44,p+68,120);
 }
 memcpy(world.positions,input+596,128);memcpy(world.gpu,input+724,512);memcpy(world.streams,input+1236,128);
 memcpy(world.vertices,input+1364,64);memcpy(world.normals,input+1428,64);
 for(unsigned i=0;i<2;i++)for(unsigned j=0;j<2;j++)world.tables[i][j].pointer=resolve(get(input+1492+i*8+j*4));
 IntroSceneStatePtr=resolve(get(input+1508));ModelBankFormat=(s32)get(input+1512);memcpy(control,input+1516,128);
 assert(sizeof(void*)==8&&(uintptr_t)IntroSceneStatePtr>UINT32_MAX);
 assert(offsetof(RroModelScratch,work7C)!=0x7C&&offsetof(RroSceneScratchPrefix,draw74.draw.counter)!=0x78);
 assert(offsetof(RroSceneScratchPrefix,view.matrix28)!=0x28);
 memset(output,0,8+MAX_EVENTS*EVENT+STATE);s32 yaw=DrawRotatingTitleModel(index,animate);
 put(output,(u32)yaw);put(output+4,events);snapshot(output+8+MAX_EVENTS*EVENT);
 for(unsigned i=0;i<16;i++)assert(world.before[i]==0xA5&&world.middle[i]==0xA5&&world.after[i]==0xA5);
 for(unsigned i=96;i<sizeof(RroScratchWork);i++)assert(((u8*)&world.scratch.work7C)[i]==0xA5);
}
'''


def next_word(x):
    return (x*1664525+1013904223) & 0xFFFFFFFF


def hash_bytes(data):
    x = 0
    for b in data:x = (x*33+b) & 0xFFFFFFFF
    return x


class TransportCpu(Cpu):
    def step(self):
        word = self.load(self.pc, 4)
        if word >> 26 == 18:
            assert (word >> 21) & 31 == 6
            reg, value = (word >> 11) & 31, self.r[(word >> 16) & 31]
            if reg in (4, 12):value = C.c_int16(value).value & 0xFFFFFFFF
            self.control[reg] = value;self.record_hardware(100+reg, value)
            target = self.delay;self.delay = None;self.pc = target if target is not None else self.pc+4
            return
        super().step()


def main():
    owners = source_owners(ROOT/'src/main/PAL');body = definition_source(owners['DrawRotatingTitleModel'].read_text(), 'DrawRotatingTitleModel')
    position = address_of('RotatingSkyTransforms');entry = address_of('DrawRotatingTitleModel');pointer = address_of('IntroSceneStatePtr');format_at = address_of('ModelBankFormat')
    prefix = PREFIX.replace('static u32 control[32];','static const u32 positionAddress=%dU;\nstatic u32 control[32];' % position)
    executable = (ROOT/'assets/PAL/main.exe').read_bytes();base = struct.unpack_from('<I', executable, 24)[0]
    with TemporaryDirectory() as temp:
        source, library = Path(temp)/'native.c', Path(temp)/'native.dylib';source.write_text(prefix+body+TAIL)
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-I'+str(ROOT/'include'),
                        str(source), '-o', str(library)], check=True)
        native = C.CDLL(str(library));native.run_case.argtypes = [C.c_uint, C.c_int32, C.c_int32, C.c_void_p, C.c_void_p]
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN);memory.mem_map(0, 0x400000);memory.mem_map(SCRATCH, 0x1000)
        write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b))
        read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
        put = lambda a, v: write(a, struct.pack('<I', v & 0xFFFFFFFF))
        write(base, executable[0x800:])
        services = {address_of(n): i+1 for i, n in enumerate(('SelectModelBank', 'RotMatrix', 'MulRotMatrix0', 'SetGteObjectTransform', 'SubmitModel'))}
        rng = random.Random(0x1BD405F0D0);cases = []
        for case in range(CASES):
            data = bytearray(rng.randbytes(STATE))
            for offset, address in [(0, POOL[0]), (4, POOL[1]), (72, POOL[2]), (80, TABLES[0]),
                                    (84, POOL[4]), (88, POOL[6]), (116, POOL[2]), (284, position), (472, position+16), (1508, STATES[0])]:
                struct.pack_into('<I', data, offset, address)
            for i in range(4):struct.pack_into('<I', data, 1492+i*4, POOL[2+i%2])
            for i in range(32):
                if i in (4, 12):struct.pack_into('<i', data, 1516+i*4, C.c_int16(struct.unpack_from('<I', data, 1516+i*4)[0]).value)
            for i in range(2):
                for j in range(8):struct.pack_into('<h', data, 220+i*188+0xAA+j*2, (-32768,-1024,-1,0,1,32,33,1023,1024,4095,32767)[(case+i+j)%11])
            cases.append((data, case%8, (0,-2147483648,-1,1,2147483647)[(case//8)%5]))
        segments = [(SCRATCH,220), *((a,188) for a in STATES), (position,128),
                    *((a,n) for a,n in zip(POOL,(256,256,64,64,32,32,32,32))), *((a,8) for a in TABLES), (pointer,4),(format_at,4)]
        assert sum(n for a,n in segments)+128 == STATE
        for version in ('original','linked'):
            code = executable[0x800+entry-base:0x800+entry-base+0x1B8]
            if version == 'linked':
                with (ROOT/'build/PAL/main.elf').open('rb') as f:
                    elf=ELFFile(f);symbol=elf.get_section_by_name('.symtab').get_symbol_by_name('DrawRotatingTitleModel')[0]
                    assert symbol['st_value']==entry and symbol['st_size']==0x1B8
                    section=elf.get_section(symbol['st_shndx']);offset=entry-section['sh_addr'];code=section.data()[offset:offset+0x1B8]
            write(entry,code);covered=set()
            for case,(data,index,animate) in enumerate(cases):
                output=(C.c_ubyte*OUTPUT)();native.run_case(case,index,animate,C.create_string_buffer(bytes(data)),output);expected=bytes(output)
                yaw,events=struct.unpack_from('<II',expected);at=0
                for address,size in segments:write(address,data[at:at+size]);at+=size
                control=list(struct.unpack_from('<32I',data,1516));write(STACK-256,b'\xA6'*512);event=0;multiplies=0;temporary=None
                def snapshot():return b''.join(read(a,n) for a,n in segments)+struct.pack('<32I',*control)
                def record(kind,*args):
                    nonlocal event
                    assert event<events;covered.add(kind);words=(kind,*args,*([0]*(4-len(args))))
                    assert struct.pack('<5I',*words)+snapshot()==expected[8+event*EVENT:8+(event+1)*EVENT],(version,case,event,kind)
                    event+=1
                def store(cpu,access,address,size,value,user):
                    assert STACK-0x48<=address and address+size<=STACK or address==SCRATCH+0x78 and size==4 or address in (SCRATCH+0x7C,SCRATCH+0x7E,SCRATCH+0x80) and size==2 or any(address==a+0xAA+index*2 and size==2 for a in STATES),(hex(address),size)
                cpu=TransportCpu(memory,0,100,101,102,store);cpu.control=control;cpu.record_hardware=lambda k,v:record(k,v)
                cpu.r=[0]+[0xBEEF0000+r for r in range(1,32)];cpu.r[4:6]=[index,animate&0xFFFFFFFF];cpu.r[29]=STACK;cpu.r[31]=STOP;cpu.pc=entry
                for instruction in range(1500):
                    if cpu.pc==STOP:break
                    if cpu.pc not in services:cpu.step();continue
                    assert cpu.delay is None;kind=services[cpu.pc];a,b,c,d=cpu.r[4:8]
                    if kind==1:
                        record(kind,a);bank=case&1
                        for offset,value in [(72,POOL[2+bank]),(80,TABLES[bank]),(84,POOL[4+bank]),(88,POOL[6+bank])]:put(SCRATCH+offset,value)
                        put(pointer,STATES[bank]);result=0
                    elif kind==2:
                        record(kind,a,b);seed=hash_bytes(read(a,6))^case
                        for i in range(8):seed=next_word(seed);put(b+i*4,seed)
                        if case&2:put(pointer,STATES[1-(case&1)])
                        result=0
                    elif kind==3:
                        assert a==SCRATCH+0x84 and multiplies<2
                        if multiplies==0:assert b==SCRATCH+0xA4
                        else:temporary=b
                        record(kind,a,multiplies);multiplies+=1;seed=hash_bytes(read(a,32))^case
                        for value in control[:5]:seed^=value
                        for i in range(8):seed=next_word(seed);put(b+i*4,seed)
                        result=b
                    elif kind==4:
                        assert a==SCRATCH+0x7C and c==temporary
                        record(kind,a,b,hash_bytes(read(c,32)));seed=hash_bytes(read(b,6))^hash_bytes(read(c,32))
                        for i in range(14):seed=next_word(seed);put(a+i*4,seed)
                        put(format_at,(-2147483647,-1,0,1,2,2147483647)[case%6]);result=0
                    else:record(kind,a,b,c,d);result=0
                    resume=cpu.r[31]
                    for register in (2,3,*range(4,16),24,25):cpu.r[register]=0xDEAD0000+register
                    cpu.r[2]=result&0xFFFFFFFF;cpu.pc=resume;cpu.hi=0x12345678;cpu.lo=0x87654321
                assert cpu.pc==STOP and event==events and cpu.r[2]==yaw
                assert snapshot()==expected[8+MAX_EVENTS*EVENT:],(version,case,'final')
                assert cpu.r[29]==STACK and cpu.r[31]==STOP
                for r in (*range(16,24),28,30):assert cpu.r[r]==0xBEEF0000+r
                assert read(STACK-256,256-0x48)==b'\xA6'*(256-0x48) and read(STACK,256)==b'\xA6'*256
            assert covered=={1,2,3,4,5,*range(100,105),*range(108,113)},covered
            print('PASS',version,CASES,'actual intro caller flows; full-width scene submission, typed scratch and GTE transports')


if __name__=='__main__':main()
