"""Actual sprite grid with a byte-level oracle and live native pointer aliases."""
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

ROOT=Path(__file__).resolve().parents[2]


class AnimatedSpriteGridTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temporary=TemporaryDirectory()
        cls.addClassCleanup(cls.temporary.cleanup)
        def header(name):
            text=(ROOT/'include/rr'/name).read_text()
            return re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',text,flags=re.M)
        body=(ROOT/'tools/scripts/fixtures/views/func_8001E1F0.c').read_text()
        body=re.sub(r'^#include[^\n]*\n','',body,flags=re.M)
        body=re.sub(r'\s+__asm__\("\$\d+"\)','',body)
        body=re.sub(r',?\s*"\$\d+"','',body)
        assert body.count('(RrGpuScratchPointers*)0x1F800000U')==2
        assert body.count('*(u8**)0x1F800000U')==1
        body=body.replace('(RrGpuScratchPointers*)0x1F800000U','&actual.scratch')
        body=body.replace('*(u8**)0x1F800000U','actual.scratch.cursor')
        code=r'''
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
''' + header('gpu_scratch_types.h') + header('gpu.h') + header('animated_sprite_grid.h') + r'''
typedef struct {
    u8 prefix[32];
    RrGpuScratchPointers scratch;
    u32 orderword;
    u8 pad[12];
    RrSpriteGridClock clock;
    u8 pad2[48];
    u8 packets[384];
    u8 suffix[64];
} World;
static World actual __attribute__((aligned(65536))),expected;
''' + body + r'''
static u8 *locate(const void *address,size_t size) {
    uintptr_t begin=(uintptr_t)&actual,position=(uintptr_t)address;
    assert(position>=begin&&position-begin<=sizeof(actual)-size);
    return (u8*)&expected+(position-begin);
}
static u32 read32(const void *address) {
    u32 value;memcpy(&value,locate(address,4),4);return value;
}
static void write32(void *address,u32 value){memcpy(locate(address,4),&value,4);}
static void byte(void *address,u8 value){*locate(address,1)=value;}
static u32 *order(void) {
    u32 *pointer;memcpy(&pointer,&expected.scratch.order,sizeof(pointer));
    assert(((uintptr_t)pointer&3)==0);return pointer;
}
static u8 *cursor(void) {
    u8 *pointer;memcpy(&pointer,&expected.scratch.cursor,sizeof(pointer));return pointer;
}
static void publish(u8 *pointer){memcpy(&expected.scratch.cursor,&pointer,sizeof(pointer));}
static void reference(u32 intensity,RrSpriteGridClock *state) {
    u16 tick;memcpy(&tick,locate(&state->tick0E,2),2);
    u8 *packet=cursor(); /* Captured before the clock halfword is published. */
    tick++;memcpy(locate(&state->tick0E,2),&tick,2);
    int phase=tick;
    u32 color=intensity|(intensity<<8)|(intensity<<16)|0x66000000U;
    for(int i=0;i<15;i++) {
        int animation=(phase/4)&15;
        if(animation==15)animation=0;
        write32(packet+8,((176+i/5*48)<<16)|(172+(i%5)*26));
        write32(packet+4,color);
        write32(packet+16,0x180010);
        write32(packet+12,0x2F29A400U+animation*16);
        phase+=5;
        write32(packet,read32(order()));
        byte(packet+3,4);
        write32(order(),(u32)(uintptr_t)packet);
        byte((u8*)order()+3,0);
        packet+=20;
    }
    publish(packet);
    write32(packet+4,0xE100022A);
    write32(cursor(),read32(order()));
    byte(cursor()+3,1);
    write32(order(),(u32)(uintptr_t)cursor());
    byte((u8*)order()+3,0);
    publish(cursor()+8);
}
static void one(u16 tick,u32 intensity,int alias) {
    for(unsigned i=0;i<sizeof(actual);i++)((u8*)&actual)[i]=(u8)(i*73U+(i>>3)+19U);
    actual.scratch.cursor=actual.packets+16;
    actual.scratch.order=&actual.orderword;
    actual.orderword=0xCC13579B;
    RrSpriteGridClock *state=&actual.clock;
    if(alias==1)state=(RrSpriteGridClock *)(actual.packets+16);
    if(alias==2)state=(RrSpriteGridClock *)((u8*)&actual.scratch.cursor-14);
    if(alias==3)state=(RrSpriteGridClock *)((u8*)&actual.orderword-12);
    if(alias==4)actual.scratch.order=(u32 *)&actual.scratch.order;
    if(alias==5)actual.scratch.order=(u32 *)(actual.packets+16);
    if(alias!=2)state->tick0E=tick;
    /* Pointer storage and packets share one64K block, so a self-pointing
     * order slot's low32 write produces the exact full native packet pointer. */
    assert(((uintptr_t)&actual&65535)==0);
    assert(((uintptr_t)actual.packets>>32)==((uintptr_t)&actual.scratch>>32));
    expected=actual;
    reference(intensity,state);
    func_8001E1F0(intensity,state);
    assert(memcmp(&actual,&expected,sizeof(actual))==0);
    assert(actual.scratch.cursor==actual.packets+16+15*20+8);
}
int main(int argc,char **argv) {
    assert(argc==2);
    if(!strcmp(argv[1],"ticks")) {
        for(unsigned tick=0;tick<65536;tick++)
            one(tick,tick*1664525U+1013904223U,0);
    } else if(!strcmp(argv[1],"colors")) {
        const u32 values[]={0,1,127,128,255,256,0x80000000U,0xFFFFFFFFU,0x12345678U};
        for(unsigned i=0;i<sizeof(values)/sizeof(*values);i++)
        for(unsigned tick=0;tick<256;tick++)one(tick,values[i],0);
    } else {
        const u16 ticks[]={0,1,59,60,61,63,64,65534,65535};
        for(int alias=1;alias<=5;alias++)
        for(unsigned t=0;t<sizeof(ticks)/sizeof(*ticks);t++)
        for(unsigned color=0;color<256;color++)
            one(ticks[t],color,alias);
    }
    return 0;
}
'''
        directory=Path(cls.temporary.name);source=directory/'grid.c'
        source.write_text(code);cls.executable=directory/'grid'
        result=subprocess.run(['cc','-std=c99','-O2','-Wall','-Wextra','-Werror','-Wno-error=deprecated-non-prototype',
                               '-fsanitize=undefined','-fno-strict-aliasing',
                               '-I',str(ROOT/'include'),str(source),'-o',str(cls.executable)],
                              capture_output=True,text=True)
        if result.returncode:raise AssertionError(result.stderr)

    def run_case(self,case):
        result=subprocess.run([str(self.executable),case],capture_output=True,text=True)
        self.assertEqual(result.returncode,0,result.stderr)
        self.assertEqual(result.stderr,'')

    def test_all_tick_values_frames_and_wire_packets(self):self.run_case('ticks')
    def test_raw_intensity_packing_and_order_header_clearing(self):self.run_case('colors')
    def test_clock_cursor_order_and_packet_aliases(self):self.run_case('alias')


if __name__=='__main__':unittest.main()
