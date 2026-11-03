"""Actual FT4 emitter with independent memory oracle and native pointer aliases."""
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

ROOT=Path(__file__).resolve().parents[2]


class TexturedPanelTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temporary=TemporaryDirectory()
        cls.addClassCleanup(cls.temporary.cleanup)
        def header(name):
            text=(ROOT/'include/rr'/name).read_text()
            return re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',text,flags=re.M)
        body=(ROOT/'build/legacy/func_8005D40C.c').read_text()
        body=re.sub(r'^#include[^\n]*\n','',body,flags=re.M)
        body=re.sub(r'\s+__asm__\("\$\d+"\)','',body)
        assert body.count('(RrGpuScratchPointers*)0x1F800000U')==1
        body=body.replace('(RrGpuScratchPointers*)0x1F800000U','&actual.scratch')
        code=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
''' + header('gpu_scratch_types.h')+header('gpu.h') + header('animated_sprite_grid.h') + header('textured_panel.h') + r'''
typedef struct {
    u8 prefix[32];
    RrGpuScratchPointers scratch;
    u32 ordering[4];
    u8 gap[32];
    u8 packets[128];
    u8 suffix[32];
} World;
static World actual __attribute__((aligned(65536))),expected;
''' + body + r'''
static u8 *locate(const void *address,size_t size) {
    uintptr_t offset=(uintptr_t)address-(uintptr_t)&actual;
    assert(offset<=sizeof(actual)-size);return (u8*)&expected+offset;
}
static u32 read32(const void *address) {
    u32 value;memcpy(&value,locate(address,4),4);return value;
}
static void store(void *address,u32 value,int size) {
    u8 *output=locate(address,size);
    for(int i=0;i<size;i++)output[i]=value>>(8*i);
}
static u32 *order(void) {
    u32 *pointer;memcpy(&pointer,&expected.scratch.order,sizeof(pointer));return pointer;
}
static void reference(s32 index) {
    u8 *p;memcpy(&p,&expected.scratch.cursor,sizeof(p));
    int64_t half=(int64_t)index/2;
    int remainder=(int)((int64_t)index-half*2);
    /* Modulo1024 is computed in wide arithmetic, independently of the body's
     * unsigned32 shift/add encoding. Negative odd indices retain remainder -1. */
    int64_t page=(half*64+320)%1024;if(page<0)page+=1024;
    int top=remainder*128;
    store(p+3,9,1);store(p+7,0x2D,1);
    store(p+10,112,2);store(p+18,112,2);
    store(p+26,368,2);store(p+34,368,2);
    store(p+16,288,2);store(p+32,288,2);
    store(p+13,top,1);store(p+21,top,1);
    store(p+22,(u32)(page/64)|16,2);
    store(p+29,top+127,1);store(p+37,top+127,1);
    store(p+8,32,2);store(p+24,32,2);
    store(p+12,0,1);store(p+20,255,1);
    store(p+28,0,1);store(p+36,255,1);store(p+14,0x1BD6,2);
    store(p,(read32(p)&0xFF000000U)|(read32((u8*)order()+4)&0xFFFFFFU),4);
    /* Capture after tag publication. A tag alias may have changed scratch. */
    u32 *head=(u32*)((u8*)order()+4);
    store(head,(read32(head)&0xFF000000U)|((u32)(uintptr_t)p&0xFFFFFFU),4);
    p+=40;memcpy(&expected.scratch.cursor,&p,sizeof(p));
}
static void initialize(u32 seed) {
    for(size_t i=0;i<sizeof(actual);i++) {
        seed=seed*1664525U+1013904223U;((u8*)&actual)[i]=seed>>24;
    }
    actual.scratch.cursor=actual.packets+16;
    actual.scratch.order=actual.ordering;
}
static void check(s32 index) {
    memcpy(&expected,&actual,sizeof(actual));reference(index);
    func_8005D40C(index);assert(!memcmp(&actual,&expected,sizeof(actual)));
}
int main(int argc,char **argv) {
    assert(argc==2);
    const s32 extremes[]={(-2147483647-1),-2147483647,-65537,-65536,-32769,-32768,-3,-2,-1,0,1,2,3,32767,32768,65535,65536,2147483646,2147483647};
    if(atoi(argv[1])==0) {
        for(u32 i=0;i<65536;i++) {
            initialize(i*871U);check((s32)i-32768);
        }
        for(u32 i=0;i<65536;i++) {
            u32 raw=i*2654435761U;s32 index;memcpy(&index,&raw,4);
            initialize(raw);check(index);
        }
        for(unsigned i=0;i<sizeof(extremes)/sizeof(*extremes);i++) {
            initialize(i);check(extremes[i]);
            check(extremes[i]); /* Second emission consumes the new cursor/head. */
        }
    } else {
        for(unsigned i=0;i<sizeof(extremes)/sizeof(*extremes);i++) {
            for(int offset=0;offset<40;offset+=4) {
                initialize(i*873U+offset);
                actual.scratch.order=(u32*)((u8*)&actual+offsetof(World,packets)+16+offset-4);
                check(extremes[i]);
            }
            initialize(i);
            actual.scratch.order=(u32*)((u8*)&actual+offsetof(World,scratch)+offsetof(RrGpuScratchPointers,order)-4);
            check(extremes[i]);
            initialize(i);
            actual.scratch.order=(u32*)((u8*)&actual+offsetof(World,scratch)+offsetof(RrGpuScratchPointers,cursor)-4);
            check(extremes[i]);
        }
    }
    return 0;
}
'''
        source=Path(cls.temporary.name)/'textured_panel.c'
        source.write_text(code)
        cls.binary=source.with_suffix('')
        subprocess.run(['cc','-std=gnu99','-O2','-fno-strict-aliasing',
                        '-fsanitize=undefined','-fno-sanitize-recover=all',
                        '-I',str(ROOT/'include'),str(source),'-o',str(cls.binary)],check=True)

    def test_signed_indices_and_repeated_emission(self):
        subprocess.run([str(self.binary),'0'],check=True)

    def test_ordering_head_aliases(self):
        subprocess.run([str(self.binary),'1'],check=True)


if __name__=='__main__':
    unittest.main()
