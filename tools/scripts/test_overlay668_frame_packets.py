"""Current STR frame-packet owner with actual SDK constructors/builders/helpers.

An independent byte oracle covers all modes, callback-time roots/cursors and
packet/head overlap. Draw-mode insertion is an instrumented service. DMA links
use the production native mapper; only physical MIPS bindings are normalized.
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
#include "rr/frame_storage_types.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "rr/frame_storage_types.h"
#include "rr/gpu_scratch_types.h"
#include "rr/gpu_address.h"
#include "rr/draw_environment.h"
#include "psyq/libgpu/gpuenv.h"
#include "rr/sprite_emit.h"
typedef struct {u32 kind,args[6],before[16];} Event;
typedef struct {
 u32 before[4];FrameStorage frames[2];
 struct {u32 before[4];GpuScratchPointers value;u32 after[4];} scratch;
 s32 bank;s16 width,height;u32 root,calls;Event events[8];u32 after[4];
} World;
static World actual,expected,*world;
static GpuDmaRegion regions[2];
static unsigned scenario,profile,aliasIndex;static s32 selectedMode;
#define SCRATCHPAD ((u8*)&world->scratch.value)
#define CurrentDrawBuf (&world->frames[world->root])
#define DrawBufIndex world->bank
#define D_800ABAC4 world->width
#define D_800ABAC6 world->height
static u32 address(const World *world,const void *p){
 uintptr_t a=(uintptr_t)p;
 for(unsigned i=0;i<2;i++){uintptr_t b=(uintptr_t)&world->frames[i];if(a>=b&&a-b<sizeof(FrameStorage))return 0x80180000U+i*0x40000U+(u32)(a-b);}
 assert(!"unmapped/truncated frame pointer");return 0;
}
static void *resolve(World *world,u32 a){
 for(unsigned i=0;i<2;i++){u32 d=a-(0x80180000U+i*0x40000U);if(d<sizeof(FrameStorage))return world->frames[i].bytes+d;}
 assert(!"invalid wire address");return 0;
}
static void event(unsigned kind,const u32 *args,const void *packet){
 assert(world->calls<8);Event *e=&world->events[world->calls++];e->kind=kind;memcpy(e->args,args,24);
 memcpy(e->before,packet,64);
}
static u8 *choose_cursor(unsigned phase){
 if(scenario&128){unsigned root=profile&&selectedMode!=2?((scenario+3)&1):world->root;
  unsigned index=selectedMode==0?703:0;return world->frames[root].bytes+0x70+index*4-aliasIndex*4;}
 return world->frames[(scenario>>5)&1].buffer.packets1670+((scenario+phase)&7)*64;
}
static void mutate(unsigned phase){
 if(!profile)return;
 world->root=(scenario+phase)&1;world->bank=(s32)((u32)world->bank^scenario*0x31415927U^phase);
 world->scratch.value.cursor=choose_cursor(phase);
 world->scratch.value.order=world->frames[world->root].buffer.secondaryOrderingB70.words+((scenario+phase)%704);
 world->frames[world->root].buffer.ordering70.words[100+(scenario&63)]^=scenario*0x17171717U+phase;
}
s32 GetVideoMode(void){
 s32 result=(scenario>>3)&1;u32 a[6]={(u32)result};event(2,a,world->scratch.value.cursor);mutate(2);return result;
}
'''
SERVICES = r'''
DrawEnvironment *SetDefDrawEnv(DrawEnvironment *env,s32 x,s32 y,s32 width,s32 height){
 u32 a[6]={(u32)x,(u32)y,(u32)width,(u32)height};event(1,a,world->scratch.value.cursor);
 return real_SetDefDrawEnv(env,x,y,width,height);
}
void SetDrawEnv(u32 *packet,DrawEnvironment *env){
 u32 a[6]={address(world,packet)};memcpy(a+1,&env->clip,8);a[3]=(u16)env->offset_x|((u32)(u16)env->offset_y<<16);
 a[4]=env->page|((u32)env->dither<<16)|((u32)env->draw_display<<24);a[5]=env->background;
 event(3,a,packet);real_SetDrawEnv(packet,env);mutate(3);
}
static void draw_mode(u32 *ordering,void *cursor,u16 page){
 u32 *p=cursor;u32 a[6]={address(world,ordering),address(world,cursor),page,address(world,world->scratch.value.cursor)};
 assert(cursor==world->scratch.value.cursor);event(4,a,cursor);
 ((u8*)p)[3]=2;p[1]=0xE1000200U|(page&0x9FFU);p[2]=0;
 p[0]=(p[0]&0xFF000000U)|(*ordering&0xFFFFFFU);
 *ordering=(*ordering&0xFF000000U)|(address(world,p)&0xFFFFFFU);mutate(4);
}
void *AddDrawModePrim(u32 *ordering,GpuDrawModePacket *mode_packet,u16 page){ void *cursor = (void *)mode_packet;draw_mode(ordering,cursor,page);return (u8*)cursor+12;}
'''
TAIL = r'''
static u32 get(const void *p){u32 n;memcpy(&n,p,4);return n;}
static void put(void *p,u32 n){memcpy(p,&n,4);}
static void put16(u8 *p,u32 n){p[0]=(u8)n;p[1]=(u8)(n>>8);}
static s32 half(u32 n){return (n&32768)?(s32)(n&65535)-65536:(s32)(n&65535);}
static u32 coord(s32 n,s16 limit){return (u32)(n<0?0:(limit-1<n?(s32)(u16)limit-1:n))&1023U;}
static void reference(void){
 if(selectedMode<0||selectedMode>3)return;
 if(selectedMode==2){
  u8 *p=world->scratch.value.cursor;p[3]=3;p[7]=0x62;put16(p+10,40);put16(p+12,320);put16(p+14,160);p[4]=p[5]=24;p[6]=12;put16(p+8,0);
  u32 *head=world->frames[world->root].buffer.ordering70.words;
  put(p,(get(p)&0xFF000000U)|(*head&0xFFFFFFU));*head=(*head&0xFF000000U)|(address(world,p)&0xFFFFFFU);
  world->scratch.value.cursor=p+16;draw_mode(head,p+16,37);world->scratch.value.cursor=p+28;return;
 }
 u32 y=(u32)world->bank*256U;if(selectedMode==0)y|=40;if(selectedMode==3)y|=80;
 u32 h=selectedMode==1?256:160;u32 a[6]={0,y,320,h};event(1,a,world->scratch.value.cursor);
 s32 video=GetVideoMode();u8 env[28]={0};put16(env+2,y);put16(env+4,320);put16(env+6,h);put16(env+10,y);put16(env+20,10);env[22]=1;env[23]=h<(video?289U:257U);
 u8 *packet=world->scratch.value.cursor;u32 b[6]={address(world,packet),get(env),get(env+4),get(env+8),get(env+20),env[24]};event(3,b,packet);
 s32 yy=half(y);put(packet+4,0xE3000000U|coord(0,world->width)|(coord(yy,world->height)<<10));
 put(packet+8,0xE4000000U|coord(319,world->width)|(coord(half(y+h-1),world->height)<<10));
 put(packet+12,0xE5000000U|(((u32)yy&2047U)<<11));put(packet+16,0xE100060AU);put(packet+20,0xE2000000U);put(packet+24,0xE6000000U);packet[3]=6;
 mutate(3);u32 *head=world->frames[world->root].buffer.ordering70.words+(selectedMode==0?703:0);
 put(packet,(get(packet)&0xFF000000U)|(*head&0xFFFFFFU));*head=(*head&0xFF000000U)|(address(world,packet)&0xFFFFFFU);world->scratch.value.cursor=packet+64;
}
static u32 seed=0x115BD0;
static u32 next(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
int main(void){
 static const s32 modes[]={(-2147483647-1),-1,0,1,2,3,4,2147483647};
 static const s32 banks[]={(-2147483647-1),-65537,-256,-1,0,1,2,255,256,65535,2147483647};
 static const s16 limits[]={-32768,-1,0,1,256,320,512,1024,32767};
 unsigned seen[8]={0},aliases[4][16]={{0}};
 assert(sizeof(void*)==8&&(uintptr_t)&actual>UINT32_MAX);
 GpuDmaTag tag;tag.word=0xAB123456U;assert(tag.bits.address==0x123456&&tag.bits.length==0xAB);
 for(unsigned i=0;i<2;i++)regions[i]=(GpuDmaRegion){&actual.frames[i],0x80180000U+i*0x40000U,sizeof(FrameStorage)};
 assert(GpuDmaSetRegions(regions,2));
 for(scenario=0;scenario<4096;scenario++){
  /* Keep the complete owners initialized, including all packet lookahead. */
  for(unsigned i=0;i<sizeof(actual);i+=4)put((u8*)&actual+i,next());
  actual.root=(scenario>>4)&1;actual.bank=banks[(scenario>>2)%11];actual.width=limits[(scenario>>3)%9];actual.height=limits[(scenario>>5)%9];
  actual.calls=0;memset(actual.events,0,sizeof(actual.events));selectedMode=modes[scenario&7];profile=(scenario>>6)&1;aliasIndex=(scenario>>8)&15;
  world=&actual;actual.scratch.value.cursor=choose_cursor(0);actual.scratch.value.order=actual.frames[actual.root].buffer.ordering70.words;
  expected=actual;expected.scratch.value.cursor=resolve(&expected,address(&actual,actual.scratch.value.cursor));expected.scratch.value.order=resolve(&expected,address(&actual,actual.scratch.value.order));
  world=&expected;reference();u32 cursor=address(&expected,expected.scratch.value.cursor),ordering=address(&expected,expected.scratch.value.order);
  world=&actual;ovl668_emit_letterbox_pass(selectedMode);
  assert(address(&actual,actual.scratch.value.cursor)==cursor&&address(&actual,actual.scratch.value.order)==ordering);
  expected.scratch.value=actual.scratch.value;assert(!memcmp(&actual,&expected,sizeof(actual)));
  seen[scenario&7]++;if((scenario&128)&&selectedMode>=0&&selectedMode<=3)aliases[selectedMode][aliasIndex]++;
 }
 for(unsigned i=0;i<8;i++)assert(seen[i]);for(unsigned m=0;m<4;m++)for(unsigned i=0;i<16;i++)assert(aliases[m][i]);
 return 0;
}
'''


class Overlay668FramePacketsTest(unittest.TestCase):
    def test_real_draw_descriptor_helpers_full_owners_and_aliases(self):
        owners = source_owners(ROOT / 'src/main/PAL')
        helpers = []
        for name in ('get_cs', 'get_ce', 'get_ofs', 'get_mode', 'get_tw', 'SetDefDrawEnv', 'SetDrawEnv'):
            body = definition_source(owners[name].read_text(), name)
            body = re.sub(r'\s+(?:__asm__|asm)\("\$\d+"\)', '', body)
            body = body.replace('"$5"', '')
            if name in ('SetDefDrawEnv', 'SetDrawEnv'):
                body = body.replace(name, 'real_' + name)
            helpers.append(body)
        emitter = definition_source((ROOT / 'src/overlays/PAL/668/func_80115BD0.c').read_text(),
                                    'ovl668_emit_letterbox_pass')
        with TemporaryDirectory() as directory:
            source = Path(directory) / 'frame_packets.c'
            binary = source.with_suffix('')
            source.write_text(PREFIX + '\n'.join(helpers) + SERVICES + emitter + TAIL)
            result = subprocess.run(['cc', '-std=c11', '-O2', '-Wall', '-Wextra', '-Werror',
                                     '-Wno-unused-parameter', '-fsanitize=undefined,bounds',
                                     '-fno-sanitize-recover=all', '-I', str(ROOT / 'include'),
                                     str(source), str(ROOT / 'src/native/gpu_address.c'),
                                     '-o', str(binary)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            result = subprocess.run([str(binary)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)


if __name__ == '__main__':
    unittest.main()
