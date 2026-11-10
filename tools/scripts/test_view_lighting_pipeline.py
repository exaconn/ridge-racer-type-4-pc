"""Actual lighting initialization, matrix construction and GTE submission.

The trigonometric rotation service is instrumented and may mutate shared inputs.
GTE transport and fixed-point IR results are modeled; FLAG and timing are outside
this fixture. Matrix/input padding ignored by the corresponding GTE registers is
not read natively. Register narrowing follows the primary emulator implementation:
https://github.com/stenzek/duckstation/blob/master/src/core/gte.cpp (WriteRegister).
"""
import ctypes as C
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from c_functions import address_of, definition_source, source_owners
from test_gte_matrix_products import adapted_body
from test_scene_camera_tracks import CameraCpu

ROOT = Path(__file__).resolve().parents[2]
NAMES = ['InitViewLighting','BuildLightMatrix','SetupViewLighting','SetColorMatrix',
         'SetRotMatrix','SetLightMatrix','ApplyRotMatrix','SetBackColor','SetFarColor',
         'SetFogNear','SetDQA','SetDQB']
STACK, STOP = 0x803F0000, 0x803FF000
STATE, INPUT, OUTPUT = 284, 540, 1816
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "game/camera/viewini.h"
#include "game/camera/viewcam.h"
#include "rr/matrix_workspace.h"
#include "psyq/libgte/gtemat3.h"
#include "psyq/libgte/gtemat7.h"
#include "psyq/libgte/rotmatyxz.h"
#include "psyq/libgte/gtetrig.h"
typedef struct {
 u8 before[16];MatrixWords32 defaults,color;Vec3sFields8 defaultAngles[3],angles[3];
 u16 strength[3];s16 course;s32 courseColors[27],back[3],far[3];
 PreviewMatrixWorkspace light;u8 after[16];
} World;
static World live;
static u32 control[32],data[32],events[64][5];
static unsigned count,scenario,rotations,kind;
#define D_800A3B00 live.defaults
#define ColorMatrix live.color
#define D_800A3B20 live.defaultAngles[0]
#define D_800A3B28 live.defaultAngles[1]
#define D_800A3B30 live.defaultAngles[2]
#define LightSourceAngles live.angles
#define LightSourceIntensities live.strength
#define CourseIndex live.course
#define CourseFarColorTable live.courseColors
#define BackColorR live.back[0]
#define BackColorG live.back[1]
#define BackColorB live.back[2]
#define FarColorR live.far[0]
#define FarColorG live.far[1]
#define FarColorB live.far[2]
#define LightMatrix live.light
static s32 word(u32 v){s32 n;memcpy(&n,&v,4);return n;}
static s16 half(u16 v){s16 n;memcpy(&n,&v,2);return n;}
static u16 get16(const void *p){u16 n;memcpy(&n,p,2);return n;}
static u32 next(u32 n){n^=n<<13;n^=n>>17;n^=n<<5;return n;}
static void event(u32 k,u32 a,u32 b,u32 c,u32 d){assert(count<64);u32 *e=events[count++];e[0]=k;e[1]=a;e[2]=b;e[3]=c;e[4]=d;}
static u32 narrow(unsigned space,unsigned reg,u32 value){
 if((!space&&(reg==4||reg==12||reg==20||reg==26||reg==27||reg==29||reg==30))||
    (space&&(reg==1||reg==3||reg==5||(reg>=8&&reg<=11))))return (u32)(s32)half((u16)value);
 if(space&&reg>=16&&reg<=19)return value&65535U;
 return value;
}
static void write_reg(unsigned space,unsigned reg,u32 value){value=narrow(space,reg,value);(space?data:control)[reg]=value;event(space,reg,value,0,0);}
static void transfer_matrix(unsigned first,const void *pointer){
 const u8 *bytes=pointer;assert((uintptr_t)pointer>UINT32_MAX);
 for(unsigned i=0;i<4;i++)write_reg(0,first+i,(u32)get16(bytes+i*4)|((u32)get16(bytes+i*4+2)<<16));
 write_reg(0,first+4,(u32)(s32)half(get16(bytes+16)));
}
static void vector(const Vec3s8 *input){
 const u8 *p=(const u8*)input;
 write_reg(1,0,(u32)get16(p)|((u32)get16(p+2)<<16));write_reg(1,1,(u32)(s32)half(get16(p+4)));
}
static void multiply(void){
 s16 v[3]={half((u16)data[0]),half((u16)(data[0]>>16)),half((u16)data[1])};
 event(2,0x4A486012U,0,0,0);
 for(unsigned row=0;row<3;row++){
  int64_t sum=0;for(unsigned col=0;col<3;col++){unsigned n=row*3+col;s16 c=half((u16)(control[n/2]>>((n&1)*16)));sum+=(int64_t)c*v[col];}
  int64_t scaled=sum>=0?sum/4096:-((-sum+4095)/4096);data[25+row]=(u32)scaled;
  if(scaled>32767)scaled=32767;if(scaled<-32768)scaled=-32768;data[9+row]=(u32)scaled;
 }
}
static void store_word(unsigned reg,unsigned offset,void *output){u32 value=data[reg];memcpy((u8*)output+offset,&value,4);event(3,reg,value,0,0);}
#define RR_GTE_SETROT_5W(p) transfer_matrix(0,p)
#define RR_GTE_SETLIGHT_5W(p) transfer_matrix(8,p)
#define RR_GTE_SETCOL_5W(p) transfer_matrix(16,p)
#define RR_GTE_LDV0_2W(p) vector(p)
#define RR_GTE_MVMVA_ROT_V0_1NOP() multiply()
#define RR_GTE_SWC2_TRIPLE_AT(r0,o0,r1,o1,r2,o2,p) do {store_word(r0,o0,p);store_word(r1,o1,p);store_word(r2,o2,p);} while(0)
#define RR_GTE_CTC2(v,r) write_reg(0,r,(u32)(v))
static void mutate(unsigned call){
 if(!(scenario&1U))return;
 for(unsigned i=0;i<3;i++){
  live.angles[i].x=half((u16)((u16)live.angles[i].x+scenario+call+i));
  live.angles[i].y=half((u16)((u16)live.angles[i].y^(scenario*73U+call+i*13U)));
  live.angles[i].z=half((u16)((u16)live.angles[i].z-scenario-call*7U-i));
  live.strength[i]^=(u16)(scenario*17U+call*31U+i*257U);
  live.back[i]=word(scenario*0x1020304U+call*0x17171717U+i*0x11111111U);
  live.far[i]=word(~(scenario*0x3141592U+call*0x1234567U+i*0x22222222U));
 }
 unsigned c=call%8,l=(call+5)%8;live.color.w[c]=word((u32)live.color.w[c]^(scenario*0x1234567U+call));
 live.light.words[l]^=scenario*0x17171717U+call;
}
MatrixWords32 *RotMatrixYXZ(const Vec3sFields8 *angles,MatrixWords32 *output){
 unsigned call=rotations++;assert(call<3&&angles==&live.angles[call]);
 event(4,(u32)(s32)angles->x,(u32)(s32)angles->y,(u32)(s32)angles->z,call);
 u32 seed=((u32)(u16)angles->x|((u32)(u16)angles->y<<16))^((u32)(u16)angles->z*0x9E3779B9U+scenario*0x17171717U+call*0x12345U);
 for(unsigned i=0;i<8;i++){seed=next(seed);output->w[i]=word(seed);}
 mutate(call);return output;
}
'''
TAIL = r'''
void run_case(unsigned operation,unsigned number,const u8 *input,u8 *output){
 _Static_assert(sizeof(World)==316&&offsetof(World,light)==268,"independent wire offsets");
 assert(sizeof(void*)==8&&(uintptr_t)&live>UINT32_MAX);
 memset(&live,0xA5,sizeof(live));memcpy(&live.defaults,input,284);
 memcpy(control,input+284,128);memcpy(data,input+412,128);
 for(unsigned i=0;i<32;i++){control[i]=narrow(0,i,control[i]);data[i]=narrow(1,i,data[i]);}
 count=rotations=0;memset(events,0,sizeof(events));kind=operation;scenario=number;
 switch(kind){
 case 0:InitViewLighting();break;
 case 1:InitViewLighting();SetupViewLighting();break;
 case 2:SetupViewLighting();break;
 case 3:BuildLightMatrix();break;
 case 4:SetColorMatrix(&ColorMatrix);break;
 case 5:SetBackColor(BackColorR,BackColorG,BackColorB);break;
 case 6:SetFarColor(FarColorR,FarColorG,FarColorB);break;
 default:assert(0);
 }
 assert(rotations==((kind==1||kind==2||kind==3)?3U:0U));
 memcpy(output,&count,4);memcpy(output+4,events,sizeof(events));
 memcpy(output+1284,&live.defaults,284);memcpy(output+1568,control,124);memcpy(output+1692,data,124);
 for(unsigned i=0;i<16;i++)assert(live.before[i]==0xA5&&live.after[i]==0xA5);
}
void exhaust_padding(void){
 static const u16 last[]={0,1,32767,32768,65535,0x1234,0xAA55,0x55AA};
 for(unsigned profile=0;profile<8;profile++)for(unsigned pad=0;pad<65536;pad++){
  memset(&live,0xA5,sizeof(live));
  for(unsigned i=0;i<9;i++)ColorMatrix.halves[i]=(u16)(profile*12345U+i*0x1717U);
  ColorMatrix.halves[8]=last[profile];ColorMatrix.halves[9]=(u16)pad;
  World entry=live;memset(control,0x5A,sizeof(control));memset(events,0,sizeof(events));count=0;
  SetColorMatrix(&ColorMatrix);assert(count==5&&control[20]==(u32)(s32)half(last[profile]));
  for(unsigned i=0;i<4;i++)assert(control[16+i]==((u32)ColorMatrix.halves[i*2]|((u32)ColorMatrix.halves[i*2+1]<<16)));
  for(unsigned i=0;i<32;i++)if(i<16||i>20)assert(control[i]==0x5A5A5A5AU);
  assert(!memcmp(&live,&entry,sizeof(live)));
 }
}
'''


def s32(x):return C.c_int32(x).value

def s16(x):return C.c_int16(x).value

def step_seed(x):
    x^=(x<<13)&0xFFFFFFFF;x^=x>>17;x^=(x<<5)&0xFFFFFFFF
    return x&0xFFFFFFFF


class Hardware:
    def __init__(self, control, data):
        self.control=[self.narrow(0,i,v) for i,v in enumerate(control)]
        self.data=[self.narrow(1,i,v) for i,v in enumerate(data)];self.events=[]
    @staticmethod
    def narrow(space,reg,value):
        if not space and reg in (4,12,20,26,27,29,30) or space and reg in (1,3,5,8,9,10,11):return s16(value)&0xFFFFFFFF
        if space and reg in (16,17,18,19):return value&65535
        return value&0xFFFFFFFF
    def event(self,k,*args):self.events.append((k,*args,*([0]*(4-len(args)))))
    def write(self,space,reg,value):
        value=self.narrow(space,reg,value);(self.data if space else self.control)[reg]=value;self.event(space,reg,value)
    def matrix(self,first,buffer):
        halves=struct.unpack_from('<9H',buffer)
        for i in range(4):self.write(0,first+i,halves[i*2]|halves[i*2+1]<<16)
        self.write(0,first+4,halves[8])
    def multiply(self):
        self.event(2,0x4A486012)
        vector=[s16(self.data[0]),s16(self.data[0]>>16),s16(self.data[1])]
        for row in range(3):
            coefficients=[s16(self.control[(row*3+c)//2]>>(((row*3+c)&1)*16)) for c in range(3)]
            result=sum(a*b for a,b in zip(coefficients,vector))//4096
            self.data[25+row]=result&0xFFFFFFFF;self.data[9+row]=max(-32768,min(32767,result))&0xFFFFFFFF
    def output(self,state):
        assert len(self.events)<=64
        return (struct.pack('<I',len(self.events))+b''.join(struct.pack('<5I',*e) for e in self.events)+
                bytes((64-len(self.events))*20)+bytes(state)+struct.pack('<31I',*self.control[:31])+struct.pack('<31I',*self.data[:31]))


def mutation(state,scenario,call):
    if not scenario&1:return
    for i in range(3):
        at=88+i*8;x,y,z=struct.unpack_from('<3H',state,at)
        struct.pack_into('<3H',state,at,(x+scenario+call+i)&65535,(y^(scenario*73+call+i*13))&65535,(z-scenario-call*7-i)&65535)
        strength=struct.unpack_from('<H',state,112+i*2)[0];struct.pack_into('<H',state,112+i*2,strength^((scenario*17+call*31+i*257)&65535))
        struct.pack_into('<I',state,228+i*4,(scenario*0x1020304+call*0x17171717+i*0x11111111)&0xFFFFFFFF)
        struct.pack_into('<I',state,240+i*4,~(scenario*0x3141592+call*0x1234567+i*0x22222222)&0xFFFFFFFF)
    for base,index,value in [(32,call%8,scenario*0x1234567+call),(252,(call+5)%8,scenario*0x17171717+call)]:
        at=base+index*4;value^=struct.unpack_from('<I',state,at)[0];struct.pack_into('<I',state,at,value&0xFFFFFFFF)


def rotation(state,hw,scenario,call):
    x,y,z=struct.unpack_from('<3h',state,88+call*8);hw.event(4,x&0xFFFFFFFF,y&0xFFFFFFFF,z&0xFFFFFFFF,call)
    seed=((x&65535)|((y&65535)<<16))^(((z&65535)*0x9E3779B9+scenario*0x17171717+call*0x12345)&0xFFFFFFFF)
    words=[]
    for _ in range(8):seed=step_seed(seed);words.append(seed)
    mutation(state,scenario,call)
    return struct.pack('<8I',*words)


def reference(kind,scenario,blob):
    state=bytearray(blob[:STATE]);hw=Hardware(struct.unpack_from('<32I',blob,284),struct.unpack_from('<32I',blob,412))
    if kind in (0,1):
        state[32:64]=state[:32];state[88:112]=state[64:88]
        struct.pack_into('<3H',state,112,4096,4096,4096);struct.pack_into('<3I',state,228,19,29,28)
        course=struct.unpack_from('<h',state,118)[0];state[240:252]=state[120+course*12:132+course*12]
    if kind in (1,2,4):hw.matrix(16,state[32:64])
    if kind in (1,2,3):
        for call in range(3):
            strength=struct.unpack_from('<H',state,112+call*2)[0];z=(-strength)&65535
            matrix=rotation(state,hw,scenario,call);hw.matrix(0,matrix)
            hw.write(1,0,0);hw.write(1,1,z);hw.multiply()
            for component in range(3):
                value=hw.data[9+component];hw.event(3,9+component,value)
                struct.pack_into('<H',state,252+call*6+component*2,value&65535)
    if kind in (1,2):hw.matrix(8,state[252:284])
    if kind in (1,2,5):
        for i in range(3):hw.write(0,13+i,struct.unpack_from('<I',state,228+i*4)[0]<<4)
    if kind in (1,2,6):
        for i in range(3):hw.write(0,21+i,struct.unpack_from('<I',state,240+i*4)[0]<<4)
    if kind in (1,2):hw.write(0,27,-((20000*320)//290));hw.write(0,28,0x01400000)
    return hw.output(state)


class GteCpu(CameraCpu):
    def step(self):
        word=self.load(self.pc,4);op=word>>26
        if op not in (18,50,58):return super().step()
        rs=(word>>21)&31;rt=(word>>16)&31;rd=(word>>11)&31
        if op==18:
            if word==0x4A486012:self.hw.multiply()
            elif rs==6:self.hw.write(0,rd,self.r[rt])
            elif rs==4:self.hw.write(1,rd,self.r[rt])
            elif rs==0:self.r[rt]=self.hw.data[rd]
            elif rs==2:self.r[rt]=self.hw.control[rd]
            else:raise AssertionError(hex(word))
        else:
            address=(self.r[rs]+s16(word))&0xFFFFFFFF
            if op==50:self.hw.write(1,rt,self.load(address,4))
            else:self.hw.event(3,rt,self.hw.data[rt]);self.store(address,self.hw.data[rt],4)
        destination=self.delay;self.delay=None;self.pc=(self.pc+4 if destination is None else destination)&0xFFFFFFFF


class ViewLightingPipelineTest(unittest.TestCase):
    def test_real_callers_transfers_wrapping_and_defined_fields(self):
        owners=source_owners(ROOT/'src/main/PAL');bodies='\n'.join(adapted_body(owners[n].read_text(),n) for n in NAMES)
        regions=[('D_800A3B00',32),('ColorMatrix',32),('D_800A3B20',8),('D_800A3B28',8),('D_800A3B30',8),
                 ('LightSourceAngles',24),('LightSourceIntensities',6),('CourseIndex',2),('CourseFarColorTable',108),
                 ('BackColorR',4),('BackColorG',4),('BackColorB',4),('FarColorR',4),('FarColorG',4),('FarColorB',4),('LightMatrix',32)]
        addresses={n:address_of(n) for n,_ in regions};starts={n:address_of(n) for n in NAMES};rotate=address_of('RotMatrixYXZ')
        self.assertEqual(sum(size for _,size in regions),STATE)
        original=(ROOT/'assets/PAL/main.exe').read_bytes();base=struct.unpack_from('<I',original,24)[0]
        variants={'original':{}}
        with (ROOT/'build/PAL/main.elf').open('rb') as stream:
            elf=ELFFile(stream);table=elf.get_section_by_name('.symtab');code={}
            for n in NAMES:
                symbol=table.get_symbol_by_name(n)[0];section=elf.get_section(symbol['st_shndx']);offset=symbol['st_value']-section['sh_addr']
                self.assertEqual(symbol['st_value'],starts[n]);code[n]=section.data()[offset:offset+symbol['st_size']]
            variants['compiled']=code
        engines=[]
        with TemporaryDirectory() as directory:
            source,library=Path(directory)/'native.c',Path(directory)/'native.dylib';source.write_text(PREFIX+bodies+TAIL)
            subprocess.run(['cc','-std=c11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                            '-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(source),'-o',str(library)],check=True)
            native=C.CDLL(str(library));native.run_case.argtypes=[C.c_uint,C.c_uint,C.c_void_p,C.c_void_p]
            for variant,code in variants.items():
                memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000)
                write=lambda a,b,m=memory:m.mem_write(a&0x1FFFFFFF,bytes(b))
                read=lambda a,n,m=memory:bytes(m.mem_read(a&0x1FFFFFFF,n))
                write(base,original[0x800:])
                for name,data in code.items():write(starts[name],data)
                def store(cpu,access,address,size,value,user):
                    a=address&0x1FFFFFFF;lo=(STACK-256)&0x1FFFFFFF
                    allowed=[(lo,256),*[(addresses[n]&0x1FFFFFFF,length) for n,length in regions]]
                    self.assertTrue(any(lo<=a and a+size<=lo+length for lo,length in allowed),(hex(address),size))
                engines.append((variant,GteCpu(memory,0,100,101,102,store),write,read))
            randomizer=random.Random(0x6E63C);seen=set();target_count=0;target_seen=set()
            edges=[0,1,0x7FFFFFFF,0x80000000,0xFFFFFFFF,0x00008000,0xFFFF7FFF,0x12345678,0x87654321]
            strengths=[0,1,32767,32768,65535,4096]
            for number in range(8192):
                kind=number%7;blob=bytearray(randomizer.randbytes(INPUT));struct.pack_into('<h',blob,118,number%9)
                if number<4096:
                    for i in range(3):struct.pack_into('<H',blob,112+i*2,strengths[(number//7+i)%len(strengths)])
                    for field in [*range(0,64,4),*range(120,284,4)]:struct.pack_into('<I',blob,field,edges[(number//7+field//4)%len(edges)])
                expected=reference(kind,number,blob);output=(C.c_ubyte*OUTPUT)()
                native.run_case(kind,number,C.create_string_buffer(bytes(blob)),output)
                self.assertEqual(bytes(output),expected,('native',number,kind));seen.add((kind,number%9,number&1))
                if number<70 or number%31==0:
                    for variant,cpu,write,read in engines:
                        def load_state(state):
                            at=0
                            for n,length in regions:write(addresses[n],state[at:at+length]);at+=length
                        def state():return b''.join(read(addresses[n],length) for n,length in regions)
                        load_state(blob[:STATE]);cpu.hw=Hardware(struct.unpack_from('<32I',blob,284),struct.unpack_from('<32I',blob,412));calls=0
                        entries={0:['InitViewLighting'],1:['InitViewLighting','SetupViewLighting'],2:['SetupViewLighting'],
                                 3:['BuildLightMatrix'],4:['SetColorMatrix'],5:['SetBackColor'],6:['SetFarColor']}[kind]
                        for entry in entries:
                            cpu.reset();cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)];cpu.r[29]=STACK;cpu.r[31]=STOP;cpu.pc=starts[entry]
                            if entry=='SetColorMatrix':cpu.r[4]=addresses['ColorMatrix']
                            if entry in ('SetBackColor','SetFarColor'):
                                cpu.r[4:7]=list(struct.unpack_from('<3I',state(),228 if entry=='SetBackColor' else 240))
                            write(STACK-512,b'\xA5'*1024)
                            for _ in range(8000):
                                if cpu.pc==STOP:break
                                if cpu.pc==rotate:
                                    self.assertIsNone(cpu.delay);self.assertEqual(cpu.r[4],addresses['LightSourceAngles']+calls*8)
                                    self.assertTrue(STACK-256<=cpu.r[5] and cpu.r[5]+32<=STACK)
                                    current=bytearray(state());matrix=rotation(current,cpu.hw,number,calls);load_state(current)
                                    pointer=cpu.r[5];write(pointer,matrix);calls+=1;resume=cpu.r[31]
                                    for r in (1,2,3,*range(4,16),24,25):cpu.r[r]=0xDEAD0000+r
                                    cpu.r[2]=pointer;cpu.pc=resume;cpu.hi=0x12345678;cpu.lo=0x87654321
                                else:cpu.step()
                            self.assertEqual(cpu.pc,STOP,(variant,number,entry));self.assertEqual(cpu.r[29],STACK);self.assertEqual(cpu.r[31],STOP)
                            for r in (*range(16,24),28,30):self.assertEqual(cpu.r[r],0xBEEF0000+r)
                            self.assertEqual(read(STACK-512,256),b'\xA5'*256);self.assertEqual(read(STACK,512),b'\xA5'*512)
                        self.assertEqual(cpu.hw.output(state()),expected,(variant,number,kind));self.assertEqual(calls,3 if kind in (1,2,3) else 0)
                        target_count+=1;target_seen.add((kind,number%9,number&1))
            self.assertEqual(seen,{(k,c,m) for k in range(7) for c in range(9) for m in range(2)});self.assertEqual(target_seen,seen)
            native.exhaust_padding()
            print(f'8192 native lighting flows, {target_count} original/compiled MIPS flows, 524288 padding/coefficient transfers; current initializer/build/submission and SDK bodies, live input changes, whole software owners, defined input fields, full signed RGB wrapping, guards and UBSan')
