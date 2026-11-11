"""Current image-row caller through actual sprite/triangle/SDK/DMA helpers.

The byte oracle checks complete frame banks, rows, packet arena and scratch
owner on LP64, with registered PAL CPU aliases. All selected production bodies,
including AddPrim, are used literally without host-only register adaptation.
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
#include <stdint.h>
#include <string.h>
#include "rr/frame_loop.h"
#include "rr/gpu_scratch_types.h"
#include "rr/gpu_address.h"
#include "game/ui/uitext.h"
#include "game/gfx/sprefct.h"
#include "game/ui/badge1.h"
#include "game/ui/badgespr.h"
#include "psyq/libgpu/gpudisp.h"
#include "psyq/libgpu/addprim.h"
#include "psyq/libgpu/primattr.h"
#include "psyq/libgpu/primatt2.h"
#include "psyq/libgpu/primatt3.h"
#include "psyq/libgpu/settrans.h"
static struct World {
    u32 guard[4]; FrameStorage frames[2]; ImageRow20 rows[3];
    GpuScratchPointers scratch; u32 arena[2048]; u32 tail[4];
} actual, expected;
static unsigned bank;
#define CurrentDrawBuf (&actual.frames[bank])
#define ImageRows actual.rows
#define SCRATCHPAD ((uintptr_t)&actual.scratch)
'''
TAIL = r'''
static u32 seed=0x1A6E2039U;
static u32 next(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
static s32 signed_word(u32 bits){s32 n;memcpy(&n,&bits,4);return n;}
static s16 signed_half(u16 bits){s16 n;memcpy(&n,&bits,2);return n;}
static void half(u8 *p,u16 value){memcpy(p,&value,2);}
static void word(u8 *p,u32 value){memcpy(p,&value,4);}
static void link_packet(unsigned offset,u32 address){
    u32 *packet=(u32 *)((u8 *)expected.arena+offset);
    u32 *head=&expected.frames[bank].buffer.ordering70.words[694];
    *packet=(*packet&0xFF000000U)|(*head&0xFFFFFFU);
    *head=(*head&0xFF000000U)|((address+offset)&0xFFFFFFU);
}
static void mode_packet(unsigned offset,u32 page,u32 address){
    u8 *p=(u8 *)expected.arena+offset;
    p[3]=2;word(p+4,0xE1000200U|(page&0x9FFU));word(p+8,0);
    link_packet(offset,address);
}
static void color(u8 *p,unsigned shade){
    if((u16)shade!=128)p[4]=p[5]=p[6]=(u8)shade;
}
int main(void){
    const s32 shades[]={INT16_MIN,-129,-128,-1,0,1,127,128,129,255,256,INT16_MAX};
    const s32 styles[]={0,1,2,-7};
    GpuDmaRegion regions[3];
    for(unsigned scenario=0;scenario<2048;scenario++){
        u8 *bytes=(u8 *)&actual;
        for(unsigned i=0;i<sizeof(actual);i++)bytes[i]=(u8)next();
        bank=scenario&1;
        ImageRow20 *row=&actual.rows[1];
        row->geom.coords.x=signed_half((u16)next());
        row->geom.coords.y=signed_half((u16)next());
        row->geom.coords.u=(u16)next();row->geom.coords.v=(u16)next();
        row->geom.coords.cx=(u16)next();row->geom.coords.cy=(u16)next();
        actual.scratch.cursor=(u8 *)actual.arena+16;
        actual.scratch.order=actual.frames[bank].buffer.ordering70.words;
        assert(sizeof(void *)==8&&(uintptr_t)actual.arena>UINT32_MAX);
        assert((uintptr_t)actual.scratch.order>UINT32_MAX);
        u32 address=(scenario&128)?0xA01C0000U:0x801C0000U;
        regions[0]=(GpuDmaRegion){actual.frames,0x80100000U,sizeof(actual.frames)};
        regions[1]=(GpuDmaRegion){actual.arena,address,sizeof(actual.arena)};
        regions[2]=(GpuDmaRegion){actual.rows,0x801F0000U,sizeof(actual.rows)};
        assert(GpuDmaSetRegions(regions,3));
        expected=actual;
        s32 style=styles[(scenario/2)%4],shade=shades[(scenario/8)%12];
        s32 semi=signed_word(next());unsigned offset=16;
        unsigned emitted=style==0?72:style==1?144:0;
        for(unsigned item=0;item<emitted;item++){
            unsigned tile=item/(style==0?4:8);
            s32 tx=row->geom.coords.u+(tile%9)*24;
            s32 ty=row->geom.coords.v+(tile/9)*96;
            u16 page=((ty&256)>>4)|128|((tx&1023)>>6);
            u16 clut=((row->geom.coords.cy+tile/2)*64)|((row->geom.coords.cx/16)&63);
            if(style==0){
                unsigned quadrant=item%4;
                mode_packet(offset,5,address);offset+=12;
                u8 *p=(u8 *)expected.arena+offset;
                p[3]=4;p[7]=0x64|((u16)semi?2:0)|((u16)shade==128?1:0);
                color(p,shade);
                half(p+8,row->geom.coords.x+(tile%6)*48+(quadrant%2)*24);
                half(p+10,row->geom.coords.y+(tile/6)*96+(quadrant/2)*48);
                p[12]=(tx%64)*2+(quadrant%2)*24;
                p[13]=(tile/9)*96+(quadrant/2)*48;
                half(p+14,clut);half(p+16,24);half(p+18,48);
                link_packet(offset,address);offset+=20;
                mode_packet(offset,page,address);offset+=12;
            }else{
                unsigned strip=(item/2)%4,triangle=item%2;
                u8 *p=(u8 *)expected.arena+offset;
                p[3]=7;p[7]=0x24|((u16)shade==128?1:0);color(p,shade);
                for(unsigned point=0;point<3;point++){
                    unsigned vertex=point+triangle;
                    s32 y=row->geom.coords.y+(tile/6)*96+strip*24+(vertex/2)*24;
                    y+=vertex<2?12-(shade*12)/128:(shade*12)/128-12;
                    half(p+8+point*8,row->geom.coords.x+(tile%6)*48+(vertex%2)*48);
                    half(p+10+point*8,y);
                    p[12+point*8]=(tx%64)*2+(vertex%2)*48;
                    p[13+point*8]=(tile/9)*96+strip*24+(vertex/2)*24;
                }
                half(p+14,clut);half(p+22,page);
                link_packet(offset,address);offset+=32;
            }
        }
        expected.scratch.cursor=(u8 *)actual.arena+offset;
        DrawImageRow(1,style,shade,semi);
        assert(!memcmp(&actual,&expected,sizeof(actual)));
    }
    return 0;
}
'''


class ImageRowPipelineTest(unittest.TestCase):
    def test_both_emitters_banks_noop_styles_and_full_native_cursors(self):
        owners=source_owners(ROOT/'src/main/PAL')
        names=('GetClut','get_mode','get_tw','SetDrawMode','SetPolyFT3',
               'SetSprt','SetSemiTrans','SetShadeTex','AddPrim','AddDrawModePrim',
               'EmitPolyFT3','AddSpriteWithDrawMode','DrawImageRow')
        bodies=[]
        for name in names:
            body=definition_source(owners[name].read_text(),name)
            self.assertIsNone(re.search(r'\b(?:asm|__asm__)\s*\(',body))
            bodies.append(body)
        with TemporaryDirectory() as temporary:
            directory=Path(temporary);source=directory/'row.c';binary=directory/'row'
            source.write_text(PREFIX+'\n'.join(bodies)+TAIL)
            built=subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror',
                                  '-fsanitize=undefined,bounds','-fno-sanitize-recover=all',
                                  '-I',str(ROOT/'include'),str(source),
                                  str(ROOT/'src/native/gpu_address.c'),'-o',str(binary)],
                                 capture_output=True,text=True)
            self.assertEqual(built.returncode,0,built.stderr)
            subprocess.run([str(binary)],check=True)


if __name__=='__main__':
    unittest.main()
