"""Real animated-object owners, initialization, state, matrices and C renderer.

PsyQ packet setters/linkers and DpqColor bodies run unchanged. Trigonometry,
other matrix services and the genuinely handwritten particle emitter are
instrumented at their public boundaries. DPCS uses a functional Q12 color model;
FLAG/timing and the handwritten emitter's algorithm are outside this fixture.
"""
import ctypes as C
from pathlib import Path
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from c_functions import address_of, definition_source, source_owners
from test_scene_camera_tracks import CameraCpu

ROOT=Path(__file__).resolve().parents[2]
ARENA,ORDER,CAMERA,DRAW,ARG=0x80200000,0x80240000,0x1F800000,0x80260000,0x80270000
STACK,STOP=0x803F0000,0x803FF000
NAMES=['InitAnimatedTrackObjects','SetAnimatedTrackObjectState','SetupAnimatedObjectMatrices',
       'DrawAnimatedTrackObject','SetLineG2','CatPrim','AddPrims','DpqColor']
SERVICES=['BuildRotMatrixY','BuildRotMatrixX','BuildRotMatrixZ','MulMatrix2','ApplyMatrixLV',
          'ApplyMatrixSV','SetTransMatrix','MulMatrix','SetRotMatrix','PsyqRand','PsyqSrand',
          'VSync','SquareRoot0','Atan2Table','func_8006DB0C','PsyqMemset']
PREFIX=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "game/gfx/poly1.h"
#include "game/camera/viewini.h"
#include "rr/camera_matrices.h"
#include "rr/gpu_address.h"
#include "rr/timing.h"
#include "psyq/libgpu/addprim3.h"
#include "psyq/libgpu/addprim4.h"
#include "psyq/libgpu/primatt7.h"
#include "psyq/libgte/gtecolo3.h"
#include "psyq/libgte/gtemat3.h"
#include "psyq/libgte/gtemat4.h"
#include "psyq/libgte/gtemat5.h"
#include "psyq/libgte/gtemat7.h"
static u8 *allocation,*saved;static AnimatedTrackArena *arena;
static unsigned colorLength,arenaLength,number,eventCount;static u32 randomState,farColor[3],events[512][5];
static struct {u8 before[16];u32 words[1024];u8 after[16];} ordering;
static AnimatedTrackScratch camera,draw;
#define SCRATCHPAD ((u8*)&camera)
AnimatedTrackParticle (*D_8010E738)[255];
const AnimatedTrackColorKey *D_8010E740[9],*D_8010E768[9];
AnimatedTrackObject *D_8010E78C;
MatrixWords32 ColorMatrix,HalfTransViewMatrix;
Vec3sFields8 D_800F38F8[3];
s32 DrawBufIndex;
s16 D_80107180[3];
static u32 next(u32 x){x^=x<<13;x^=x>>17;x^=x<<5;return x;}
static s32 signed32(u32 x){s32 v;memcpy(&v,&x,4);return v;}
static s16 signed16(u16 x){s16 v;memcpy(&v,&x,2);return v;}
static u32 get32(const void *p){u32 x;memcpy(&x,p,4);return x;}
static void put32(void *p,u32 x){memcpy(p,&x,4);}
static void event(u32 kind,u32 a,u32 b,u32 c,u32 d){assert(eventCount<512);u32 *e=events[eventCount++];e[0]=kind;e[1]=a;e[2]=b;e[3]=c;e[4]=d;}
static u32 hash(const void *p,unsigned bytes){const u8 *b=p;u32 x=0x811C9DC5U;for(unsigned i=0;i<bytes;i++)x=(x^b[i])*16777619U;return x;}
static u32 pointer(const void *p){
 if(!p)return 0;uintptr_t a=(uintptr_t)p,lo=(uintptr_t)arena->objects;
 if(a>=lo&&a<lo+sizeof(arena->objects)){assert((a-lo)%sizeof(AnimatedTrackObject)==0);return 0x8020B34CU+(u32)((a-lo)/sizeof(AnimatedTrackObject))*248U;}
 u32 encoded;assert(GpuDmaAddressOf(p,&encoded));return encoded;
}
static void rotation(unsigned kind,void *output,s32 angle){
 event(kind,(u32)angle,0,0,0);MatrixWords32 *m=output;u32 x=(u32)angle^(kind*0x17171717U);
 for(unsigned i=0;i<9;i++){x=next(x+i+1U);m->halves[i]=(u16)x;}
}
void BuildRotMatrixY(void *p,s32 a){rotation(1,p,a);}
void BuildRotMatrixX(void *p,s32 a){rotation(2,p,a);}
void BuildRotMatrixZ(void *p,s32 a){rotation(3,p,a);}
static MatrixWords32 *product(unsigned kind,const MatrixWords32 *a,MatrixWords32 *b){
 u32 x=hash(a,18),y=hash(b,18);event(kind,x,y,a==b,0);
 for(unsigned i=0;i<9;i++){x=next(x+y+i);b->halves[i]=(u16)x;}return b;
}
MatrixWords32 *MulMatrix2(const MatrixWords32 *a,MatrixWords32 *b){return product(4,a,b);}
MatrixWords32 *MulMatrix(MatrixWords32 *a,const MatrixWords32 *b){return product(8,b,a);}
Vec3i *ApplyMatrixLV(const MatrixWords32 *m,const Vec3i *in,Vec3i *out){
 u32 h=hash(m,18);event(5,h,(u32)in->x,(u32)in->y,(u32)in->z);
 out->x=signed32((u32)in->x^h);out->y=signed32((u32)in->y^next(h));out->z=signed32((u32)in->z^next(next(h)));
 /* A callee may change the live translation before Setup reloads it. */
 if(number&1U)for(unsigned i=5;i<8;i++)HalfTransViewMatrix.w[i]=signed32((u32)HalfTransViewMatrix.w[i]+number*0x1234567U+i);
 return out;
}
s16 *ApplyMatrixSV(const MatrixWords32 *m,const Vec3s8 *in,s16 *out){
 u32 h=hash(m,18);event(6,h,(u32)(s32)in->fields.x,(u32)(s32)in->fields.y,(u32)(s32)in->fields.z);
 u32 values[3]={h,next(h),next(next(h))};for(unsigned i=0;i<3;i++){u16 bits=(u16)values[i];memcpy((u8*)out+i*2,&bits,2);}return out;
}
void SetTransMatrix(int *p){event(7,(u32)p[5],(u32)p[6],(u32)p[7],0);}
void SetRotMatrix(int *p){event(9,hash(p,18),0,0,0);}
int PsyqRand(void){randomState=next(randomState);event(10,randomState,0,0,0);return (int)(randomState&32767U);}
int PsyqSrand(u32 seed){randomState=seed;event(11,seed,0,0,0);return 0;}
s32 VSync(s32 mode){u32 v=number*71U+13U;event(12,(u32)mode,v,0,0);return signed32(v);}
int SquareRoot0(int value){u32 v=next((u32)value)&32767U;event(13,(u32)value,v,0,0);return (int)v;}
int Atan2Table(int x,int y){u32 v=((u32)x*17U^(u32)y*31U)&4095U;event(14,(u32)x,(u32)y,v,0);return (int)v;}
void *PsyqMemset(void *p,s32 byte,u32 length){return memset(p,byte,length);}
AnimatedLinePacket *func_8006DB0C(AnimatedTrackScratch *s,AnimatedLinePacket *p,AnimatedTrackObject *o,s32 color){
 unsigned count=number%7U;event(15,pointer(p),pointer(o),(u32)color,count);
 assert(s==&camera||s==&draw);assert(count<=255);
 for(unsigned i=0;i<count;i++){p[i].color0.word^=number*0x17171717U+i;p[i].xy1=signed32(number*17U+i);}
 s->work7C[13]^=number+0x1234U;
 if(number&1U){DrawBufIndex^=1;o->positionC8.x+=17;o->positionC8.z-=23;}
 return p+count;
}
static void save_far(int *r,int *g,int *b){*r=signed32(farColor[0]);*g=signed32(farColor[1]);*b=signed32(farColor[2]);}
static void set_far(s32 r,s32 g,s32 b){farColor[0]=(u32)r;farColor[1]=(u32)g;farColor[2]=(u32)b;}
static u32 data6,ir0,rgb2;
static void dpcs(void){
 u32 result=data6&0xFF000000U;int factor=signed16((u16)ir0);
 for(unsigned i=0;i<3;i++){
  int input=(data6>>(8*i))&255U;
  int64_t sum=(int64_t)input*65536+(int64_t)factor*((int64_t)signed32(farColor[i])-input*16);
  int64_t mac=sum>=0?sum/4096:-((-sum+4095)/4096);if(mac>32767)mac=32767;if(mac<-32768)mac=-32768;
  int64_t channel=mac>=0?mac/16:-((-mac+15)/16);if(channel<0)channel=0;if(channel>255)channel=255;result|=(u32)channel<<(8*i);
 }
 rgb2=result;
}
#define RR_GTE_SAVE_FC(r,g,b) save_far(r,g,b)
#define RR_GTE_SETFC(r,g,b) set_far(r,g,b)
#define RR_GTE_LWC2_AT(reg,off,p) do {assert((reg)==6&&(off)==0);data6=get32(p);} while(0)
#define RR_GTE_MTC2(v,reg) do {assert((reg)==8);ir0=(u32)(v);} while(0)
#define RR_GTE_CMD_1NOP(command) do {assert((command)==0x4A780010);dpcs();} while(0)
#define RR_GTE_SWC2_AT(reg,off,p) do {assert((reg)==22&&(off)==0);put32(p,rgb2);} while(0)
'''
TAIL=r'''
static void guards(void){
 for(unsigned i=0;i<16;i++)assert(allocation[i]==0xA5&&allocation[16+arenaLength+i]==0xA5&&ordering.before[i]==0xA5&&ordering.after[i]==0xA5);
 for(unsigned i=sizeof(arena->particles);i<offsetof(AnimatedTrackArena,objects);i++)assert(((u8*)arena)[i]==0);
 for(unsigned n=0;n<5;n++)for(unsigned i=offsetof(AnimatedTrackObject,style3C)+4;i<offsetof(AnimatedTrackObject,keys40);i++)assert(((u8*)&arena->objects[n])[i]==0);
 assert((uintptr_t)arena>UINT32_MAX&&D_8010E78C==arena->objects&&D_8010E738==arena->particles);
}
static void scene_wire(u8 *out,const AnimatedTrackScratch *s){
 memset(out,0,228);put32(out,pointer(s->scene.view.projection.pointers.cursor));put32(out+4,pointer(s->scene.view.projection.pointers.order));
 memcpy(out+8,&s->scene.view.projection.pose08,32);memcpy(out+0x28,&s->scene.view.matrix28,32);
 assert(!s->scene.modelEntries48&&!s->scene.commands50&&!s->scene.vertices54&&!s->scene.normals58);
 put32(out+0x4C,s->scene.reserved4C);put32(out+0x5C,s->scene.reserved5C);put32(out+0x60,(u32)s->scene.depthShift60);put32(out+0x64,(u32)s->scene.renderFlags64);put32(out+0x68,s->scene.reserved68);
 memcpy(out+0x6C,&s->scene.viewport6C,8);memcpy(out+0x74,&s->scene.draw74.words,8);memcpy(out+0x7C,s->work7C,104);
}
unsigned output_size(void){return 0x17F5CU+colorLength+4096+456+196;}
void capture(u8 *out){
 guards();memcpy(out,arena->particles,45900);
 for(unsigned i=0;i<5;i++){
  const AnimatedTrackObject *o=&arena->objects[i];u8 *p=out+0xB34C+i*248;
  put32(p,pointer(o->particles00));memcpy(p+4,&o->activeCount04,60);put32(p+0x40,pointer(o->keys40));put32(p+0x44,pointer(o->keys44));
  memcpy(p+0x48,&o->palette48,128);memcpy(p+0xC8,&o->positionC8,32);
  for(unsigned j=0;j<2;j++){put32(p+0xE8+j*4,pointer(o->headsE8[j]));put32(p+0xF0+j*4,pointer(o->tailsF0[j]));}
 }
 memcpy(out+0xB824,arena->packets,51000);memcpy(out+0x17F5C,arena->colorOffsets,colorLength);out+=0x17F5C+colorLength;
 memcpy(out,ordering.words,4096);out+=4096;scene_wire(out,&camera);scene_wire(out+228,&draw);out+=456;
 memcpy(out,&ColorMatrix,32);memcpy(out+32,&HalfTransViewMatrix,32);memcpy(out+64,D_800F38F8,24);memcpy(out+88,D_80107180,6);memset(out+94,0,2);
 put32(out+96,(u32)DrawBufIndex);memcpy(out+100,farColor,12);put32(out+112,randomState);
 put32(out+116,pointer(D_8010E738));put32(out+120,pointer(D_8010E78C));
 for(unsigned i=0;i<9;i++){put32(out+124+i*4,pointer(D_8010E740[i]));put32(out+160+i*4,pointer(D_8010E768[i]));}
}
void prepare(const u8 *payload,unsigned length){
 assert(sizeof(void*)==8);colorLength=length;arenaLength=sizeof(AnimatedTrackArena)+length-72;
 allocation=malloc(arenaLength+32);saved=malloc(arenaLength);assert(allocation&&saved);memset(allocation,0xA5,arenaLength+32);arena=(AnimatedTrackArena*)(allocation+16);memset(arena,0,arenaLength);
 memcpy(arena->colorOffsets,payload,length);memset(&ordering,0xA5,sizeof(ordering));memset(ordering.words,0,sizeof(ordering.words));memset(&camera,0,sizeof(camera));memset(&draw,0,sizeof(draw));
 camera.scene.view.projection.pointers.cursor=(u8*)arena->packets;camera.scene.view.projection.pointers.order=ordering.words;draw=camera;
 static GpuDmaRegion regions[3];regions[0]=(GpuDmaRegion){arena->particles,0x80200000U,45900};regions[1]=(GpuDmaRegion){arena->packets,0x8020B824U,51000+length};regions[2]=(GpuDmaRegion){ordering.words,0x80240000U,4096};assert(GpuDmaSetRegions(regions,3));
 InitAnimatedTrackObjects(arena);memcpy(saved,arena,arenaLength);guards();
}
void configure(unsigned operation,unsigned n,u8 *before){
 number=n;memcpy(arena,saved,arenaLength);eventCount=0;memset(events,0,sizeof(events));
 for(unsigned i=0;i<1024;i++)ordering.words[i]=n*0x17171717U+i*0x12345U;
 memset(&camera,0,sizeof(camera));memset(&draw,0,sizeof(draw));camera.scene.view.projection.pointers.cursor=(u8*)arena->packets;camera.scene.view.projection.pointers.order=ordering.words;
 camera.scene.view.projection.pose08.position00.xyz.x=(s32)(n*77U%1000000U)-500000;camera.scene.view.projection.pose08.position00.xyz.y=(s32)(n*13U%1000000U)-500000;camera.scene.view.projection.pose08.position00.xyz.z=(s32)(n*91U%1000000U)-500000;draw=camera;
 u32 seed=n+0x1234U;for(unsigned i=0;i<8;i++){seed=next(seed);ColorMatrix.w[i]=signed32(seed);seed=next(seed);HalfTransViewMatrix.w[i]=signed32(seed);}
 for(unsigned i=0;i<3;i++){D_800F38F8[i]=(Vec3sFields8){(s16)n,(s16)(n+1),(s16)(n+2),(s16)0x7788};D_80107180[i]=(s16)(n%1024U+i*100U);farColor[i]=n*0x1234567U+i;}
 DrawBufIndex=(n/5U)%2U;randomState=n+0x3456U;
 AnimatedTrackObject *o=&arena->objects[n%5U];o->particles00=arena->particles[n%5U];o->style3C=(s32)(n/10U%9U);o->keys40=D_8010E740[o->style3C];o->keys44=D_8010E768[o->style3C];
 o->capacity08=255;o->initialized10=255;o->frame14=32+(s32)(n%150U);o->startFrame28=32;o->endFrame2C=181;o->advance30=1;
 o->positionC8=(Vec3i){(s32)(n*7U%10000U)-5000,(s32)(n*11U%10000U)-5000,(s32)(n*17U%10000U)-5000};o->anglesD8=(Vec3i){(s32)(n*97U),(s32)(n*137U),(s32)(n*197U)};
 if(operation==3){
  unsigned branch=n%6U;
  if(branch==0)o->initialized10=(n&1U)?254:0;
  if(branch==1)o->frame14=(s32)(n%32U);
  if(branch==2)o->frame14=182;
  if(branch==3)o->frame14=32;
  if(branch==4)o->frame14=47;
  if(n&8U){const AnimatedTrackColorKey *k=o->keys40;while(k->duration!=-1)k++;o->keyIndex34=(s32)(k-o->keys40);}
  s32 duration=o->keys40[o->keyIndex34].duration;o->colorTick18=duration>0?(s32)(n%(u32)duration):0;
  o->tailsF0[DrawBufIndex]=(n&4U)?o->headsE8[DrawBufIndex]+n%32U:0;
 }
 if(n>=720U){
  for(unsigned i=0;i<sizeof(arena->particles);i++)((u8*)arena->particles)[i]=(u8)(n*17U+i*29U);
  for(unsigned i=0;i<16;i++){o->palette48.words[i]=0x52000000U|((n*0x171717U+i)&0xFFFFFFU);o->palette88.words[i]=0x52000000U|((n*0x123456U+i)&0xFFFFFFU);}
  if(n&32U)o->keys44=D_8010E740[o->style3C];
  if(n&64U){const AnimatedTrackColorKey *k=o->keys44;while(k->duration!=-1)k++;o->keyIndex38=(s32)(k-o->keys44);}
  static const u32 ticks[]={0,1,2,0x80000000U,0xFFFFFFFFU,0x7FFFFFFFU};
  o->colorTick18=signed32(ticks[n%6U]);o->colorTick1C=signed32(ticks[n/6U%6U]);
 }
 capture(before);
}
void invoke(unsigned operation,unsigned n,u8 *out,u32 *trace){
 AnimatedTrackObject *o=&arena->objects[n%5U];
 if(operation==1){s32 position[3]={signed32(n*0x17171717U),signed32(n*0x1234567U),signed32(n*0x3141592U)};SetAnimatedTrackObjectState((s32)(n%5U),(s32)(n%8U),(n&8U)?0:position,(s32)(n%10U)-1);}
 else if(operation==2)SetupAnimatedObjectMatrices(o);
 else if(operation==3)DrawAnimatedTrackObject((n&16U)?&camera:&draw,(s32)(n%5U),(s32)(n/10U%2U));
 else assert(0);
 capture(out);trace[0]=eventCount;memcpy(trace+1,events,eventCount*20);
}
void primitive(unsigned operation,unsigned a,unsigned b,unsigned c,u8 *out){
 assert(a<8&&b<8&&c<8);
 if(operation)AddPrims(&ordering.words[a],&ordering.words[b],&ordering.words[c]);
 else CatPrim(&ordering.words[a],&ordering.words[b]);
 capture(out);
}
'''

def word(x):return x&0xFFFFFFFF

def signed(x):return C.c_int32(x).value

def half(x):return C.c_int16(x).value

def next_word(x):
    x^=word(x<<13);x^=x>>17;x^=word(x<<5)
    return word(x)


def digest(blob):
    x=0x811C9DC5
    for b in blob:x=word((x^b)*16777619)
    return x


class ParticleCpu(CameraCpu):
    def step(self):
        instruction=self.load(self.pc,4);op=instruction>>26
        if op not in (18,50,58):return super().step()
        rt=(instruction>>16)&31;rd=(instruction>>11)&31;rs=(instruction>>21)&31
        if op==18:
            if instruction==0x4A780010:
                result=self.rgb&0xFF000000;factor=half(self.ir0)
                for i in range(3):
                    value=(self.rgb>>(i*8))&255;mac=(value*65536+factor*(signed(self.far[i])-value*16))//4096
                    channel=max(-32768,min(32767,mac))//16
                    result|=max(0,min(255,channel))<<(i*8)
                self.rgb2=result
            elif rs==2:
                assert 21<=rd<=23;self.r[rt]=self.far[rd-21]
            elif rs==6:
                assert 21<=rd<=23;self.far[rd-21]=self.r[rt]
            elif rs==4:
                assert rd==8;self.ir0=self.r[rt]
            else:raise AssertionError(hex(instruction))
        else:
            address=word(self.r[rs]+half(instruction))
            if op==50:
                assert rt==6;self.rgb=self.load(address,4)
            else:
                assert rt==22;self.store(address,self.rgb2,4)
        destination=self.delay;self.delay=None;self.pc=word(self.pc+4 if destination is None else destination)


class AnimatedTrackOwnerTest(unittest.TestCase):
    def test_actual_owners_and_c_pipeline_against_original_and_linked_mips(self):
        owners=source_owners(ROOT/'src/main/PAL')
        bodies='\n'.join(re.sub(r'\s+(?:__asm__|asm)\("\$\d+"\)','',definition_source(owners[n].read_text(),n)) for n in NAMES)
        self.assertNotIn('__asm__',bodies)
        disc=(ROOT/'disc/PAL/R4.BIN').read_bytes();sectors=struct.unpack_from('<951I',disc,4)
        scene=disc[sectors[15]*2048:sectors[16]*2048];start=struct.unpack_from('<I',scene,44)[0]+0x17F5C
        offsets=struct.unpack_from('<18I',scene,start);end=start+72
        for offset in offsets:
            cursor=start+offset
            for _ in range(128):
                duration=struct.unpack_from('<i',scene,cursor+4)[0];cursor+=8
                self.assertTrue(duration==-1 or duration>0)
                if duration==-1:break
            else:self.fail('unterminated original color key table')
            end=max(end,cursor)
        colors=scene[start:end];arena_size=0x17F5C+len(colors);global_at=arena_size+4096+456
        fields=[('ColorMatrix',0,32),('HalfTransViewMatrix',32,32),('D_800F38F8',64,24),('D_80107180',88,6),('DrawBufIndex',96,4),
                ('D_8010E738',116,4),('D_8010E78C',120,4),('D_8010E740',124,36),('D_8010E768',160,36)]
        addresses={n:address_of(n) for n,_,_ in fields};starts={n:address_of(n) for n in NAMES};services={address_of(n):n for n in SERVICES}
        exe=(ROOT/'assets/PAL/main.exe').read_bytes();base=struct.unpack_from('<I',exe,24)[0]
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
                            '-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I',str(ROOT/'include'),
                            str(source),str(ROOT/'src/native/gpu_address.c'),'-o',str(library)],check=True)
            native=C.CDLL(str(library));native.prepare.argtypes=[C.c_void_p,C.c_uint];native.capture.argtypes=[C.c_void_p]
            native.configure.argtypes=[C.c_uint,C.c_uint,C.c_void_p];native.invoke.argtypes=[C.c_uint,C.c_uint,C.c_void_p,C.c_void_p]
            native.primitive.argtypes=[C.c_uint,C.c_uint,C.c_uint,C.c_uint,C.c_void_p]
            native.prepare(C.create_string_buffer(colors),len(colors));size=native.output_size()
            self.assertEqual(size,global_at+196);out=(C.c_ubyte*size)();before=(C.c_ubyte*size)();trace=(C.c_uint32*(1+512*5))()
            for variant,code in variants.items():
                mem=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);mem.mem_map(0,0x400000);mem.mem_map(0x1F800000,0x1000)
                write=lambda a,b,m=mem:m.mem_write(a&0x1FFFFFFF,bytes(b));read=lambda a,n,m=mem:bytes(m.mem_read(a&0x1FFFFFFF,n))
                write(base,exe[0x800:])
                for name,data in code.items():write(starts[name],data)
                def store(cpu,access,address,length,value,user):
                    a=address&0x1FFFFFFF
                    allowed=[(ARENA&0x1FFFFFFF,arena_size),(ORDER&0x1FFFFFFF,4096),(CAMERA,228),(DRAW&0x1FFFFFFF,228),(STACK-1024&0x1FFFFFFF,1024),
                             *[(addresses[n]&0x1FFFFFFF,width) for n,_,width in fields]]
                    self.assertTrue(any(lo<=a and a+length<=lo+width for lo,width in allowed),(hex(address),length))
                engines.append((variant,ParticleCpu(mem,0,100,101,102,store),write,read))
            def install(cpu,write,blob):
                write(ARENA,blob[:arena_size]);write(ORDER,blob[arena_size:arena_size+4096]);write(CAMERA,blob[arena_size+4096:arena_size+4096+228]);write(DRAW,blob[arena_size+4096+228:global_at])
                for n,offset,width in fields:write(addresses[n],blob[global_at+offset:global_at+offset+width])
                cpu.far=list(struct.unpack_from('<3I',blob,global_at+100));cpu.random=struct.unpack_from('<I',blob,global_at+112)[0];cpu.events=[]
            def capture(cpu,read):
                globals_=bytearray(196)
                for n,offset,width in fields:globals_[offset:offset+width]=read(addresses[n],width)
                struct.pack_into('<3I',globals_,100,*cpu.far);struct.pack_into('<I',globals_,112,cpu.random)
                return read(ARENA,arena_size)+read(ORDER,4096)+read(CAMERA,228)+read(DRAW,228)+bytes(globals_)
            def execute(cpu,write,read,name,args,number):
                cpu.reset();cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)];cpu.r[4:4+len(args)]=args;cpu.r[29]=STACK;cpu.r[31]=STOP;cpu.pc=starts[name]
                write(STACK-1536,b'\xA5'*2048)
                def event(kind,*args):cpu.events.append((kind,*args,*([0]*(4-len(args)))))
                for _ in range(180000):
                    if cpu.pc==STOP:break
                    if cpu.pc not in services:cpu.step();continue
                    self.assertIsNone(cpu.delay);kind=services[cpu.pc];a,b,c,d=cpu.r[4:8];resume=cpu.r[31];result=0
                    if kind.startswith('BuildRotMatrix'):
                        k={'BuildRotMatrixY':1,'BuildRotMatrixX':2,'BuildRotMatrixZ':3}[kind];event(k,b);x=b^word(k*0x17171717)
                        values=[]
                        for i in range(9):x=next_word(word(x+i+1));values.append(x&65535)
                        write(a,struct.pack('<9H',*values))
                    elif kind in ('MulMatrix2','MulMatrix'):
                        left,right,dest=(a,b,b) if kind=='MulMatrix2' else (b,a,a);x=digest(read(left,18));y=digest(read(right,18));event(4 if kind=='MulMatrix2' else 8,x,y,int(left==right))
                        values=[]
                        for i in range(9):x=next_word(word(x+y+i));values.append(x&65535)
                        write(dest,struct.pack('<9H',*values));result=dest
                    elif kind in ('ApplyMatrixLV','ApplyMatrixSV'):
                        h=digest(read(a,18));values=struct.unpack('<3I' if kind=='ApplyMatrixLV' else '<3h',read(b,12 if kind=='ApplyMatrixLV' else 6));event(5 if kind=='ApplyMatrixLV' else 6,h,*[word(v) for v in values])
                        hashes=[h,next_word(h),next_word(next_word(h))]
                        if kind=='ApplyMatrixLV':
                            write(c,struct.pack('<3I',*[v^q for v,q in zip(values,hashes)]))
                            if number&1:
                                at=addresses['HalfTransViewMatrix']+20;old=struct.unpack('<3I',read(at,12));write(at,struct.pack('<3I',*[word(v+number*0x1234567+i) for i,v in enumerate(old,5)]))
                        else:write(c,struct.pack('<3H',*[v&65535 for v in hashes]))
                        result=c
                    elif kind=='SetTransMatrix':event(7,*struct.unpack('<3I',read(a+20,12)))
                    elif kind=='SetRotMatrix':event(9,digest(read(a,18)))
                    elif kind=='PsyqRand':cpu.random=next_word(cpu.random);event(10,cpu.random);result=cpu.random&32767
                    elif kind=='PsyqSrand':cpu.random=a;event(11,a)
                    elif kind=='VSync':result=word(number*71+13);event(12,a,result)
                    elif kind=='SquareRoot0':result=next_word(a)&32767;event(13,a,result)
                    elif kind=='Atan2Table':result=(word(a*17)^word(b*31))&4095;event(14,a,b,result)
                    elif kind=='PsyqMemset':write(a,bytes([b&255])*c);result=a
                    elif kind=='func_8006DB0C':
                        count=number%7;event(15,b,c,d,count)
                        self.assertIn(a,(CAMERA,DRAW));self.assertEqual(c,ARENA+0xB34C+(number%5)*248)
                        for i in range(count):
                            at=b+i*20;value=struct.unpack('<I',read(at+4,4))[0]^word(number*0x17171717+i);write(at+4,struct.pack('<I',value));write(at+16,struct.pack('<I',word(number*17+i)))
                        at=a+0x7C+13*4;write(at,struct.pack('<I',struct.unpack('<I',read(at,4))[0]^(number+0x1234)))
                        if number&1:
                            at=addresses['DrawBufIndex'];write(at,struct.pack('<I',struct.unpack('<I',read(at,4))[0]^1))
                            x,y,z=struct.unpack('<3i',read(c+0xC8,12));write(c+0xC8,struct.pack('<3i',x+17,y,z-23))
                        result=b+count*20
                    else:raise AssertionError(kind)
                    for r in (1,2,3,*range(4,16),24,25):cpu.r[r]=0xDEAD0000+r
                    cpu.r[2]=result;cpu.hi=0x12345678;cpu.lo=0x87654321;cpu.pc=resume
                self.assertEqual(cpu.pc,STOP,(name,number));self.assertEqual(cpu.r[29],STACK);self.assertEqual(cpu.r[31],STOP)
                for r in (*range(16,24),28,30):self.assertEqual(cpu.r[r],0xBEEF0000+r)
                self.assertEqual(read(STACK-1536,512),b'\xA5'*512);self.assertEqual(read(STACK,512),b'\xA5'*512)
            native.capture(out);initialized=bytes(out);initial=bytearray(size);initial[0x17F5C:arena_size]=colors
            for at in (arena_size+4096,arena_size+4096+228):struct.pack_into('<2I',initial,at,ARENA+0xB824,ORDER)
            for variant,cpu,write,read in engines:
                install(cpu,write,initial);execute(cpu,write,read,'InitAnimatedTrackObjects',[ARENA],0);self.assertEqual(capture(cpu,read),initialized,('initializer',variant))
            seen=set();target_flows=2
            for operation in (1,2,3):
                for number in range(1440):
                    native.configure(operation,number,before);native.invoke(operation,number,out,trace)
                    expected=bytes(out);native_events=[tuple(trace[1+i*5:6+i*5]) for i in range(trace[0])]
                    for variant,cpu,write,read in engines:
                        install(cpu,write,bytes(before));slot=ARENA+0xB34C+number%5*248
                        if operation==1:
                            write(ARG,struct.pack('<3I',word(number*0x17171717),word(number*0x1234567),word(number*0x3141592)))
                            name,args='SetAnimatedTrackObjectState',[number%5,number%8,0 if number&8 else ARG,word(number%10-1)]
                        elif operation==2:name,args='SetupAnimatedObjectMatrices',[slot]
                        else:name,args='DrawAnimatedTrackObject',[CAMERA if number&16 else DRAW,number%5,number//10%2]
                        execute(cpu,write,read,name,args,number)
                        self.assertEqual(cpu.events,native_events,(variant,operation,number,'services'))
                        self.assertEqual(capture(cpu,read),expected,(variant,operation,number,'complete owners'))
                        target_flows+=1
                    seen.add((operation,number%5,number//10%9,number//5%2,number//10%2))
            self.assertEqual(len(seen),3*5*9*2*2)
            aliases=0
            for operation in (0,1):
                for a in range(8):
                    for b in range(8):
                        for c in range(8) if operation else (0,):
                            number=2000+aliases;native.configure(2,number,before);native.primitive(operation,a,b,c,out);expected=bytes(out)
                            for variant,cpu,write,read in engines:
                                install(cpu,write,bytes(before))
                                name,args=('AddPrims',[ORDER+a*4,ORDER+b*4,ORDER+c*4]) if operation else ('CatPrim',[ORDER+a*4,ORDER+b*4])
                                execute(cpu,write,read,name,args,number);self.assertEqual(cpu.events,[])
                                self.assertEqual(capture(cpu,read),expected,(variant,'primitive aliases',operation,a,b,c));target_flows+=1
                            aliases+=1
            self.assertEqual(aliases,576)
            print(f'4320 native pipeline cases +576 primitive alias cases + actual arena initialization; {target_flows} original/linked MIPS flows; all5 objects,9 original color-key pairs,2 buffers,2 update modes, real packet linking, full native pointers, complete owner/ABI guards and UBSan')
