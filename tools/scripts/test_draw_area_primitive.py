"""Actual UI area caller and SDK helpers, complete frame owners and byte oracle."""
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

from c_functions import definition_source, source_owners

ROOT=Path(__file__).resolve().parents[2]
PREFIX=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "rr/frame_loop.h"
#include "rr/gpu_address.h"
#include "rr/animated_glyph.h"
#include "game/ui/badge1.h"
#include "psyq/libgpu/gpudisp.h"
typedef struct {
    u32 before[4];FrameStorage frames[2];GpuDrawModePacket packets[8];
    s16 width,height;s32 bank;u32 after[4];
} World;
static World actual,expected;static GpuDmaRegion regions[3];
#define DrawBuffers (actual.frames)
#define DrawBufIndex (actual.bank)
#define D_800ABAC4 (actual.width)
#define D_800ABAC6 (actual.height)
static u32 seed=0xA4EA319U;
static u32 next(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
static s32 signed_word(u32 bits){s32 n;memcpy(&n,&bits,4);return n;}
static s16 signed_half(u16 bits){s16 n;memcpy(&n,&bits,2);return n;}
static s32 narrow(u32 value){value&=65535;return value<32768?(s32)value:(s32)value-65536;}
static u32 command(u32 kind,s32 x,s32 y,s16 width,s16 height){
    s32 limits[2]={(s32)width-1,(s32)height-1},v[2]={x,y};
    for(unsigned i=0;i<2;i++){
        if(v[i]<0)v[i]=0;else if(v[i]>limits[i])v[i]=limits[i];
        v[i]=narrow((u32)v[i]);
    }
    return kind|((u32)v[0]&1023U)|(((u32)v[1]&1023U)<<10);
}
static void put(u8 *p,u32 value){memcpy(p,&value,4);}
'''

TAIL=r'''
int main(void){
    const s32 edges[]={INT32_MIN,INT32_MIN+255,-65537,-32769,-32768,-1,0,1,255,256,32767,32768,65535,65536,INT32_MAX-255,INT32_MAX};
    const s16 limits[]={INT16_MIN,-1,0,1,320,1024,INT16_MAX};
    unsigned branches[2]={0,0},banks[2]={0,0};
    for(unsigned scenario=0;scenario<4096;scenario++)for(unsigned same=0;same<2;same++){
        memset(&actual,0xA5,sizeof(actual));
        actual.bank=(scenario>>1)&1;actual.width=limits[scenario%7];actual.height=limits[(scenario/7)%7];
        actual.frames[0].environment.display5C.display.y=signed_half((u16)next());
        actual.frames[1].environment.display5C.display.y=same?actual.frames[0].environment.display5C.display.y:
            signed_half((u16)actual.frames[0].environment.display5C.display.y^0x8000U);
        u32 address=(scenario&1)?0xA0140000U:0x80140000U;
        regions[0]=(GpuDmaRegion){&actual.frames[0],address,sizeof(FrameStorage)};
        regions[1]=(GpuDmaRegion){&actual.frames[1],address+0x40000,sizeof(FrameStorage)};
        regions[2]=(GpuDmaRegion){actual.packets,address+0xC0000,sizeof(actual.packets)};
        assert(GpuDmaSetRegions(regions,3));
        unsigned index=(scenario/4)%7,head=(scenario/28)%704;
        u32 *ordering=&actual.frames[actual.bank].buffer.ordering70.words[head];
        actual.packets[index].tag.word=next();*ordering=next();
        s32 x=scenario&4?signed_word(next()):edges[scenario%16];
        s32 y=scenario&8?signed_word(next()):edges[(scenario/16)%16];
        s32 width=scenario&16?signed_word(next()):edges[(scenario/32)%16];
        s32 height=scenario&32?signed_word(next()):edges[(scenario/64)%16];
        GpuRectangleFields *window=&actual.frames[0].environment.draw00.window;
        window->x=signed_half((u16)next());window->y=signed_half((u16)next());
        window->w=signed_half((u16)next());window->h=signed_half((u16)next());
        memcpy(&expected,&actual,sizeof(actual));
        s32 rx=narrow((u32)x),ry=narrow((u32)y+(same?0U:(u32)actual.bank*256U));
        s32 rw=narrow((u32)width),rh=narrow((u32)height);
        u32 first=command(0xE3000000U,rx,ry,actual.width,actual.height);
        u32 second=command(0xE4000000U,narrow((u32)(rx+rw-1)),narrow((u32)(ry+rh-1)),actual.width,actual.height);
        u32 old=*ordering,packetAddress=regions[2].address+index*12U;
        u8 *p=(u8*)&expected.packets[index];
        put(p,0x02000000U|(old&0xFFFFFFU));put(p+4,first);put(p+8,second);
        expected.frames[actual.bank].buffer.ordering70.words[head]=(old&0xFF000000U)|(packetAddress&0xFFFFFFU);
        assert((uintptr_t)ordering>UINT32_MAX&&(uintptr_t)&actual.packets[index]>UINT32_MAX);
        void *result=AddDrawAreaPrim(ordering,&actual.packets[index],x,y,width,height);
        assert(result==&actual.packets[index+1]);
        unsigned texture=(index+1)%8,offset=(index+2)%8;
        u8 *textureBytes=(u8*)&expected.packets[texture],*offsetBytes=(u8*)&expected.packets[offset];
        u32 textureTag=expected.packets[texture].tag.word,offsetTag=expected.packets[offset].tag.word;
        u32 textureCommand=0;
        if(scenario&1){
            textureCommand=0xE2000000U|((((u32)(u16)window->y&255U)/8U)<<15)|
                ((((u32)(u16)window->x&255U)/8U)<<10)|
                ((((0U-(u32)(u16)window->h)&255U)/8U)<<5)|
                (((0U-(u32)(u16)window->w)&255U)/8U);
        }
        put(textureBytes,(textureTag&0xFFFFFFU)|0x02000000U);
        put(textureBytes+4,textureCommand);put(textureBytes+8,0);
        SetTexWindow(&actual.packets[texture],scenario&1?window:NULL);
        put(offsetBytes,(offsetTag&0xFFFFFFU)|0x02000000U);
        put(offsetBytes+4,0xE5000000U|((u32)(u16)window->x&2047U)|(((u32)(u16)window->y&2047U)<<11));
        put(offsetBytes+8,0);
        SetDrawOffset(&actual.packets[offset],window);
        assert(!memcmp(&actual,&expected,sizeof(actual)));branches[same]++;banks[actual.bank]++;
    }
    assert(branches[0]&&branches[1]&&banks[0]&&banks[1]);
    return 0;
}
'''


class DrawAreaPrimitiveTest(unittest.TestCase):
    def test_real_caller_full_word_coordinates_both_frame_layouts(self):
        owners=source_owners(ROOT/'src/main/PAL')
        bodies=[]
        for name in ('get_cs','get_ce','get_ofs','get_tw','SetDrawArea','SetTexWindow','SetDrawOffset','AddPrim','AddDrawAreaPrim'):
            body=definition_source(owners[name].read_text(),name)
            self.assertIsNone(re.search(r'\b(?:asm|__asm__)\s*\(',body))
            bodies.append(body)
        with TemporaryDirectory() as temporary:
            directory=Path(temporary);source=directory/'native.c';binary=directory/'native'
            source.write_text(PREFIX+'\n'.join(bodies)+TAIL)
            built=subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror',
                                  '-fsanitize=undefined,bounds','-fno-sanitize-recover=all',
                                  '-I',str(ROOT/'include'),str(source),str(ROOT/'src/native/gpu_address.c'),'-o',str(binary)],
                                 capture_output=True,text=True)
            self.assertEqual(built.returncode,0,built.stderr)
            subprocess.run([str(binary)],check=True)


if __name__=='__main__':
    unittest.main()
