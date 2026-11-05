"""Actual four-tile builder with real packet/link helpers and alias-aware oracle."""
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

ROOT=Path(__file__).resolve().parents[2]


class ScreenTilesTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temporary=TemporaryDirectory()
        cls.addClassCleanup(cls.temporary.cleanup)
        def header(name):
            text=(ROOT/'include/rr'/name).read_text()
            return re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',text,flags=re.M)
        def source(name):
            text=(ROOT/'tools/scripts/fixtures/views'/name).read_text()
            text=re.sub(r'^#include[^\n]*\n','',text,flags=re.M)
            return re.sub(r'\s+(?:__asm__|asm)\("\$\d+"\)','',text)
        body=source('func_800207D4.c')
        assert body.count('(u8**)0x1F800000U')==3
        body=body.replace('(u8**)0x1F800000U','&cursor')
        helpers=source('func_80095BB4.c').replace('func_80095BB4','actual_tile')
        helpers+=source('func_80095AF4.c').replace('func_80095AF4','actual_semitransparency')
        helpers+=source('func_80095A44.c').replace('AddPrim','actual_link')
        code=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#define RR_FIELD_OFFSET(t,f) offsetof(t,f)
#define M2C_FIELD(p,t,o) (*(t)((u8 *)(p)+(o)))
''' + header('gpu.h') + header('screen_tiles.h') + r'''
static u8 pool[192] __attribute__((aligned(64)));
static u8 *cursor;
RrGpuRectangle *D_800ACC14;
''' + helpers + body + r'''
static RrGpuRectangle contexts[4];
static u32 ordering;
static int frame,event_count,scenario,final_calls;
static u16 wanted_mode;
static void trace(int id) {
    assert(event_count==frame*3+id-1);
    event_count++;
}
void func_80095BB4(void *packet) {
    assert(packet==pool+16+frame*16);
    trace(1);actual_tile(packet);
}
void func_80095AF4(void *packet,s32 enabled) {
    assert(packet==pool+16+frame*16&&enabled==1);
    trace(2);actual_semitransparency(packet,enabled);
    /* Explicit boundary hooks exercise live caller state. The actual helper
     * body above still supplies every packet operation. */
    if(scenario==2||scenario==3)D_800ACC14=&contexts[frame];
}
void AddPrim(u32 *table,u32 *packet) {
    assert(table==&ordering&&(u8*)packet==pool+16+frame*16);
    trace(3);actual_link(table,packet);
    frame++;
    if(scenario==1||scenario==3) {
        D_800ACC14=&contexts[frame%4];
        cursor=pool+160;
    }
}
u8 *func_8001FCC8(u32 *table,u8 *packet,u16 mode) {
    assert(table==&ordering&&packet==pool+80&&cursor==packet);
    assert(mode==wanted_mode&&frame==4&&event_count==12&&final_calls==0);
    final_calls++;
    cursor=pool+176; /* The caller must replace this with the return value. */
    return packet+12;
}
static void put16(u8 *where,int value) {u16 bits=value;memcpy(where,&bits,2);}
static int get_height(RrGpuRectangle *context,u8 *memory) {
    uintptr_t address=(uintptr_t)context,begin=(uintptr_t)pool;
    s16 value;
    if(address>=begin&&address+8<=begin+sizeof(pool))
        memcpy(&value,memory+(address-begin)+6,2);
    else memcpy(&value,&context->rect.h,2);
    return value;
}
static u32 reference(u8 *memory,s32 intensity,u32 initial,RrGpuRectangle **last_context) {
    int color=intensity<0?0:intensity>255?255:intensity;
    RrGpuRectangle *context=D_800ACC14;
    for(int i=0;i<4;i++) {
        u8 *packet=memory+16+i*16;
        packet[3]=3;packet[7]=0x62;
        if(scenario==2||scenario==3)context=&contexts[i];
        /* This order is observed in MIPS, including both height reads. */
        put16(packet+8,(i&1)*160);
        int y=(get_height(context,memory)/2)*(i/2);
        put16(packet+12,160);put16(packet+10,y);
        int height=get_height(context,memory);
        packet[4]=packet[5]=packet[6]=color;
        put16(packet+14,height/2);
        u32 tag;memcpy(&tag,packet,4);
        tag=(tag&0xFF000000U)|(initial&0xFFFFFFU);
        memcpy(packet,&tag,4);
        initial=(initial&0xFF000000U)|((uintptr_t)(pool+16+i*16)&0xFFFFFFU);
        if(scenario==1||scenario==3)context=&contexts[(i+1)%4];
    }
    *last_context=context;
    return initial;
}
static void one(s32 intensity,s16 height,u32 mode,int alias,int hooks) {
    for(unsigned i=0;i<sizeof(pool);i++)pool[i]=(u8)(i*73U+(i>>3)+19U);
    memset(contexts,0xA5,sizeof(contexts));
    for(int i=0;i<4;i++)contexts[i].rect.h=(s16)((u16)height+i*37U);
    D_800ACC14=alias>=0?(RrGpuRectangle *)(pool+16+alias-6):&contexts[0];
    cursor=pool+16;ordering=0xA5123456U;
    frame=event_count=final_calls=0;scenario=hooks;wanted_mode=mode;
    RrGpuRectangle saved[4];memcpy(saved,contexts,sizeof(saved));
    u8 wanted[sizeof(pool)];memcpy(wanted,pool,sizeof(pool));
    RrGpuRectangle *wanted_context;
    u32 wanted_ordering=reference(wanted,intensity,ordering,&wanted_context);
    func_800207D4(&ordering,intensity,mode);
    assert(frame==4&&event_count==12&&final_calls==1&&cursor==pool+92);
    assert(ordering==wanted_ordering&&D_800ACC14==wanted_context);
    assert(memcmp(pool,wanted,sizeof(pool))==0);
    assert(memcmp(saved,contexts,sizeof(saved))==0);
}
int main(int argc,char **argv) {
    assert(argc==2);
    if(!strcmp(argv[1],"values")) {
        const s16 heights[]={-32768,-513,-1,0,1,2,239,240,241,32767};
        const u32 modes[]={0,1,0xFFFF,0xABCD8000U,0xFFFFFFFFU};
        for(int color=-2;color<=257;color++)
        for(unsigned h=0;h<sizeof(heights)/sizeof(*heights);h++)
        for(unsigned m=0;m<sizeof(modes)/sizeof(*modes);m++)
            one(color,heights[h],modes[m],-1,0);
        one(INT_MIN,-32768,0x12345678,-1,0);one(INT_MAX,32767,0xABCDFFFF,-1,0);
        for(unsigned h=0;h<65536;h++)one((int)(h%300)-20,(s16)h,0xA5A50000U|h,-1,0);
    } else if(!strcmp(argv[1],"alias")) {
        /* All context bases are 4-byte aligned, matching the canonical union.
         * Height aliases packet tag, blue/code, y and height respectively. */
        const int positions[]={2,6,10,14};
        for(unsigned p=0;p<sizeof(positions)/sizeof(*positions);p++)
        for(int color=0;color<256;color++)
            one(color,240,0x12348000,positions[p],0);
    } else {
        for(int hooks=1;hooks<=3;hooks++)
        for(int color=-1;color<=256;color++)
            one(color,(s16)(color*73),0xFFFF0000U|(u16)color,-1,hooks);
    }
    return 0;
}
'''
        directory=Path(cls.temporary.name);native=directory/'tiles.c'
        native.write_text(code);cls.executable=directory/'tiles'
        result=subprocess.run(['cc','-std=c99','-O2','-Wall','-Wextra','-Werror','-Wno-error=deprecated-non-prototype',
                               '-Wno-pointer-to-int-cast','-fsanitize=undefined',
                               '-fno-strict-aliasing','-I',str(ROOT/'include'),
                               str(native),'-o',str(cls.executable)],capture_output=True,text=True)
        if result.returncode:raise AssertionError(result.stderr)

    def run_case(self,case):
        result=subprocess.run([str(self.executable),case],capture_output=True,text=True)
        self.assertEqual(result.returncode,0,result.stderr)
        self.assertEqual(result.stderr,'')

    def test_clamping_all_heights_modes_and_exact_packets(self):self.run_case('values')
    def test_context_height_aliases_packet_fields(self):self.run_case('alias')
    def test_live_context_hooks_and_cursor_publication(self):self.run_case('hooks')


if __name__=='__main__':unittest.main()
