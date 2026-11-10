"""Current scene/rank submission, real SDK packet types and native DMA mapping.

The byte oracle covers complete owners and packet/head overlaps. Mode-building
and linking are instrumented services; mask construction and the mode-emitter
protocol execute their actual current C bodies. No GPU hardware is executed.
"""
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

from c_functions import definition_source, source_owners

ROOT = Path(__file__).resolve().parents[2]
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "rr/frame_storage_types.h"
#include "rr/gpu_scratch_types.h"
#include "rr/gpu_address.h"
#include "rr/sprite_emit.h"
#include "rr/draw_environment.h"
#include "psyq/libgpu/gpudisp.h"
typedef struct {
 u32 before[4];FrameStorage frames[2];u32 gpu[2048];
 GpuRectangleFields geometry[4],atlas[2][4];u8 color[4];u32 root,after[4];
} Data;
typedef struct {u32 kind,first,second,third,cursor,ordering;Data data;} Event;
typedef struct {Data data;GpuScratchPointers scratch;unsigned calls,serial;Event events[16];u32 guards[4];} World;
static World actual,expected,*w;
static unsigned scenario,profile,mapProfile,modelRun;
static GpuDmaRegion regions[3];
static unsigned seen[4],aliases[4],maps[2],mutations[2],shaderFlags[3][5];
static unsigned differentAddresses;
#define CurrentDrawBuf (&w->data.frames[w->data.root])
#define SCRATCHPAD ((u8*)&w->scratch)
#define CarCameraSpringStiffness (w->data.geometry)
#define CarCameraPitchOffsets (w->data.atlas)
static u32 get32(const void *p){u32 n;memcpy(&n,p,4);return n;}
static u16 get16(const void *p){u16 n;memcpy(&n,p,2);return n;}
static void put32(void *p,u32 n){memcpy(p,&n,4);}
static void put16(void *p,u32 n){u16 v=(u16)n;memcpy(p,&v,2);}
static u32 frame_address(unsigned bank){return (mapProfile?0x80140000U:0x80040000U)+bank*0x40000U;}
static u32 gpu_address(void){return mapProfile?0x801C0000U:0x80100000U;}
static u32 wire(const void *p){
 uintptr_t n=(uintptr_t)p,b;
 for(unsigned bank=0;bank<2;bank++){
  b=(uintptr_t)&w->data.frames[bank];if(n>=b&&n-b<sizeof(FrameStorage))return frame_address(bank)+(u32)(n-b);
 }
 b=(uintptr_t)w->data.gpu;if(n>=b&&n-b<sizeof(w->data.gpu))return gpu_address()+(u32)(n-b);
 assert(!"pointer outside complete native owners");return 0;
}
static void event(unsigned kind,u32 first,u32 second,u32 third){
 assert(w->calls<16);Event *e=&w->events[w->calls++];e->kind=kind;
 e->first=first;e->second=second;e->third=third;e->cursor=wire(w->scratch.cursor);
 e->ordering=wire(w->scratch.order);memcpy(&e->data,&w->data,sizeof(Data));
}
static void mutate(unsigned phase){
 unsigned serial=w->serial++;
 if(!profile)return;
 u32 value=scenario*0x31415927U+serial*0x17171717U+phase;
 w->data.root^=1;w->scratch.cursor=(u8*)w->data.gpu+7000;
 w->scratch.order=&w->data.frames[w->data.root^1].buffer.ordering70.words[17];
 for(unsigned bank=0;bank<2;bank++){
  w->data.frames[bank].buffer.ordering70.words[0]^=value;
  w->data.frames[bank].buffer.ordering70.words[10]^=value>>3;
  w->data.frames[bank].buffer.ordering70.words[703]^=value<<7;
 }
 for(unsigned i=0;i<4;i++){
  u8 *r=(u8*)&w->data.geometry[i];
  for(unsigned j=0;j<4;j++)put16(r+j*2,value+i*17U+j*313U);
 }
 for(unsigned bank=0;bank<2;bank++)for(unsigned i=0;i<4;i++){
  u8 *r=(u8*)&w->data.atlas[bank][i];
  for(unsigned j=0;j<4;j++)put16(r+j*2,(value>>(j+1))+bank*1717U+i*91U+j);
 }
 for(unsigned i=0;i<3;i++)w->data.color[i]=(u8)(value>>(i*8));
}
static void native_stp(GpuDrawModePacket *,int);
static void *native_mode(u32 *,GpuDrawModePacket *,u16);
void SetDrawStp(GpuDrawModePacket *packet,int enabled){
 event(1,wire(packet),(u32)enabled,0);
 if(modelRun){u8 *p=(u8*)packet;p[3]=2;put32(p+4,0xE6000000U|(enabled!=0));put32(p+8,0);}
 else native_stp(packet,enabled);
 mutate(1);
}
void SetDrawMode(void *packet,s32 display,s32 dither,s32 page,GpuRectangleFields *window){
 assert(display==0&&dither==1&&!window);event(2,wire(packet),(u32)page,0);
 u8 *p=packet;p[3]=2;put32(p+4,0xE1000200U|((u32)page&0x9FFU));put32(p+8,0);
 mutate(2);
}
void AddPrim(u32 *ordering,u32 *packet){
 event(3,wire(ordering),wire(packet),0);
 put32(packet,(get32(packet)&0xFF000000U)|(get32(ordering)&0xFFFFFFU));
 u32 link=modelRun?wire(packet):GpuDmaEncodeAddress(packet);
 put32(ordering,(get32(ordering)&0xFF000000U)|(link&0xFFFFFFU));mutate(3);
}
void *AddDrawModePrim(u32 *ordering,GpuDrawModePacket *packet,u16 page){
 event(4,wire(ordering),wire(packet),page);
 if(!modelRun)return native_mode(ordering,packet,page);
 SetDrawMode(packet,0,1,page,0);AddPrim(ordering,(u32*)packet);return (u8*)packet+12;
}
'''
TAIL = r'''
static void link(u8 *packet,u8 *ordering){
 put32(packet,(get32(packet)&0xFF000000U)|(get32(ordering)&0xFFFFFFU));
 put32(ordering,(get32(ordering)&0xFF000000U)|(wire(packet)&0xFFFFFFU));
}
static void reference_scene(unsigned bank,s32 shade,u8 *rgb,s32 semi,s32 blend){
 for(unsigned i=0;i<4;i++){
  u8 *p=w->scratch.cursor;const u8 *geometry=(const u8*)&w->data.geometry[i];
  const u8 *atlas=(const u8*)&w->data.atlas[bank][i];
  p[3]=4;p[7]=(u8)(0x64|(blend?2:0)|(shade?1:0));
  for(unsigned c=0;c<3;c++)p[4+c]=rgb[c];
  put16(p+8,get16(geometry));put16(p+10,get16(geometry+2));
  put16(p+16,get16(geometry+4));put16(p+18,get16(geometry+6));
  p[12]=(u8)get16(atlas);p[13]=(u8)get16(atlas+2);
  link(p,w->data.frames[w->data.root].bytes+0x70);p+=20;
  p[3]=1;u32 x=get16(atlas+4),y=get16(atlas+6);
  put32(p+4,0xE1000300U|(((u32)semi&3U)<<5)|((y&256U)>>4)|((x&1023U)>>6)|((y&512U)<<2));
  link(p,w->data.frames[w->data.root].bytes+0x70);p+=8;w->scratch.cursor=p;
  SetDrawStp((GpuDrawModePacket*)p,0);link(p,w->data.frames[w->data.root].bytes+0x70);
  w->scratch.cursor=p+12;
 }
}
static void reference_queue(void){
 u8 *p=w->scratch.cursor;SetDrawStp((GpuDrawModePacket*)p,1);
 link(p,w->data.frames[w->data.root].bytes+0xB6C);w->scratch.cursor=p+12;
}
static void reference_rank(void){
 u8 *p=AddDrawModePrim((u32*)(w->data.frames[w->data.root].bytes+0x98),(GpuDrawModePacket*)w->scratch.cursor,5);
 unsigned bank=0;
 for(unsigned i=0;i<242;i++){
  p[3]=4;p[7]=0x65;p[12]=244;p[13]=104;put16(p+14,0x2678);
  put16(p+8,(i%22)*13+17);put16(p+10,(i/22)*28+96);put16(p+16,12);put16(p+18,24);
  bank=w->data.root;link(p,w->data.frames[bank].bytes+0x98);p+=20;
 }
 w->scratch.cursor=AddDrawModePrim((u32*)(w->data.frames[bank].bytes+0x98),(GpuDrawModePacket*)p,14);
}
static u32 seed=0x96A24;
static u32 next(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
static void compare(void){
 assert(!memcmp(&actual.data,&expected.data,sizeof(Data)));
 assert(actual.calls==expected.calls&&actual.serial==expected.serial);
 assert(!memcmp(actual.events,expected.events,sizeof(actual.events)));
 assert(!memcmp(actual.guards,expected.guards,sizeof(actual.guards)));
 w=&actual;u32 cursor=wire(w->scratch.cursor),order=wire(w->scratch.order);
 w=&expected;assert(cursor==wire(w->scratch.cursor)&&order==wire(w->scratch.order));
}
static u8 *start(unsigned alias){
 if(!alias)return (u8*)w->data.gpu+128;
 static const unsigned positions[]={0,0x70,0x98,0xB6C};
 return w->data.frames[(scenario>>5)&1].bytes+positions[alias];
}
int main(void){
 assert(sizeof(void*)==8&&(uintptr_t)&actual>UINT32_MAX);
 assert(sizeof(FrameStorage)==0x22778&&sizeof(FrameOrderingTable)==2816);
 assert(offsetof(FrameBuffer,ordering70)==0x70&&offsetof(FrameBuffer,secondaryOrderingB70)==0xB70);
 assert(sizeof(SpritePacket)==20&&sizeof(GpuCommandPacket)==8&&sizeof(GpuDrawModePacket)==12);
 GpuDmaTag tag;tag.word=0xA5C3D2E1U;assert(tag.bits.address==0xC3D2E1&&tag.bits.length==0xA5);
 tag.bits.address=0x123456;assert(tag.word==0xA5123456U);
 const s32 flags[]={(-2147483647-1),-1,0,1,2147483647};
 for(scenario=0;scenario<4096;scenario++){
  memset(&actual,0,sizeof(actual));
  for(unsigned i=0;i<sizeof(Data);i+=4){u32 value=next();memcpy((u8*)&actual.data+i,&value,4);}
  actual.data.root=(scenario>>5)&1;actual.calls=actual.serial=0;
  profile=(scenario>>6)&1;mapProfile=(scenario>>2)&1;unsigned alias=(scenario>>3)%4;
  for(unsigned i=0;i<4;i++)actual.guards[i]=next();
  w=&actual;actual.scratch.cursor=start(alias);actual.scratch.order=&actual.data.frames[1].buffer.ordering70.words[17];
  memcpy(&expected,&actual,sizeof(actual));w=&expected;expected.scratch.cursor=start(alias);expected.scratch.order=&expected.data.frames[1].buffer.ordering70.words[17];
  w=&actual;for(unsigned bank=0;bank<2;bank++)regions[bank]=(GpuDmaRegion){&actual.data.frames[bank],frame_address(bank),sizeof(FrameStorage)};
  regions[2]=(GpuDmaRegion){actual.data.gpu,gpu_address(),sizeof(actual.data.gpu)};assert(GpuDmaSetRegions(regions,3));
  differentAddresses+=((uintptr_t)actual.scratch.cursor&0xFFFFFFU)!=(wire(actual.scratch.cursor)&0xFFFFFFU);
  unsigned operation=scenario%4,bank=(scenario/500)&1;
  s32 shade=flags[(scenario/4)%5],semi=flags[(scenario/20)%5],blend=flags[(scenario/100)%5];
  for(unsigned pass=0;pass<2;pass++){
   modelRun=pass==0;w=modelRun?&expected:&actual;
   u8 *rgb=(scenario&128)?w->scratch.cursor+3:w->data.color;
   if(operation==0){if(modelRun)reference_scene(bank,shade,rgb,semi,blend);else EmitSceneOverlaySprites(bank,shade,rgb,semi,blend);}
   else if(operation==1){if(modelRun)reference_queue();else QueueDrawStp();}
   else if(operation==2){if(modelRun)reference_rank();else DrawResultsRankIcons();}
   else SetDrawStp((GpuDrawModePacket*)w->scratch.cursor,flags[(scenario/4)%5]);
  }
  compare();seen[operation]++;aliases[alias]++;maps[mapProfile]++;mutations[profile]++;
  if(operation==0){shaderFlags[0][(scenario/4)%5]++;shaderFlags[1][(scenario/20)%5]++;shaderFlags[2][(scenario/100)%5]++;}
 }
 for(unsigned i=0;i<4;i++)assert(seen[i]&&aliases[i]);assert(maps[0]&&maps[1]&&mutations[0]&&mutations[1]&&differentAddresses);
 for(unsigned row=0;row<3;row++)for(unsigned i=0;i<5;i++)assert(shaderFlags[row][i]);
 return 0;
}
'''


class ScenePacketSubmissionTest(unittest.TestCase):
    def test_current_emitters_and_typed_sdk_interfaces(self):
        owners = source_owners(ROOT / 'src/main/PAL')
        names = ('SetDrawStp', 'AddDrawModePrim', 'EmitSceneOverlaySprites',
                 'QueueDrawStp', 'DrawResultsRankIcons')
        bodies = []
        for name in names:
            body = definition_source(owners[name].read_text(), name)
            if name in ('SetDrawStp', 'AddDrawModePrim'):
                body = re.sub(r'\b' + name + r'\b',
                              'native_stp' if name == 'SetDrawStp' else 'native_mode', body)
            bodies.append(body)
        source_text = re.sub(r'\s+(?:__asm__|asm)\("\$\d+"\)', '', '\n'.join(bodies))
        with TemporaryDirectory() as directory:
            source = Path(directory) / 'scene.c'
            binary = source.with_suffix('')
            source.write_text(PREFIX + source_text + TAIL)
            result = subprocess.run(['cc', '-std=c11', '-O2', '-Wall', '-Wextra', '-Werror',
                                     '-fno-strict-aliasing', '-fsanitize=undefined,bounds',
                                     '-fno-sanitize-recover=all', '-I', str(ROOT / 'include'),
                                     str(source), str(ROOT / 'src/native/gpu_address.c'),
                                     '-o', str(binary)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            result = subprocess.run([str(binary)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)


if __name__ == '__main__':
    unittest.main()
