"""Execute the actual renderer initializer and verify all untouched storage."""
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

ROOT=Path(__file__).resolve().parents[2]


class RendererSetupTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temporary=TemporaryDirectory()
        cls.addClassCleanup(cls.temporary.cleanup)
        from c_functions import definition_source, source_owners
        owner=source_owners(ROOT/'src/main/PAL')['SetupRenderer']
        body=definition_source(owner.read_text(),'SetupRenderer')
        body=re.sub(r'\s+(?:__asm__|asm)\("\$\d+"\)','',body)
        assert not re.search(r'(?:__asm__|asm)\s*\(',body)
        code=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rr/renderer_setup.h"
typedef struct {
    u8 prefix[16];
    RendererScratch scratch;
    u8 middle[16];
    s32 flags;
    u8 gap[12];
    GpuColorCommand gouraud;
    u32 untouched1C;
    GpuColorCommand flat;
    u32 untouched24;
    u8 suffix[16];
} World;
static World actual,expected;
#define SCRATCHPAD ((u8 *)&actual.scratch)
#define MirrorRenderFlag actual.flags
#define LoadingScreenClearColor actual.gouraud
#define DefaultScreenClearColor actual.flat
''' + body + r'''
static void store(size_t offset,u32 value,int size) {
    for(int i=0;i<size;i++)((u8*)&expected)[offset+i]=value>>(8*i);
}
static void check(s32 shift,u32 flagbits,u32 seed) {
    for(size_t i=0;i<sizeof(actual);i++) {
        seed=seed*1664525U+1013904223U;((u8*)&actual)[i]=seed>>24;
    }
    memcpy(&actual.flags,&flagbits,4);
    memcpy(&expected,&actual,sizeof(actual));
    /* Independent memory specification, rather than the source field names. */
    size_t scratch=offsetof(World,scratch);
    /* PAL's four scene pointer slots follow the matrix prefix. Account for
     * native pointer alignment independently of the production field names. */
    size_t pointer=sizeof(void*),afterReserved=sizeof(ViewMatrixScratch48)+pointer+4;
    size_t depth=((afterReserved+pointer-1)/pointer)*pointer+3*pointer+4;
    size_t viewport=depth+12;
    store(scratch+depth,(u32)shift,4);
    store(scratch+depth+4,flagbits,4);
    store(offsetof(World,flat),0x2C808080U,4);
    store(offsetof(World,gouraud),0x3CFFFFFFU,4);
    store(scratch+viewport+4,320,2);
    store(scratch+viewport,0,2);
    store(scratch+viewport+2,0,2);
    store(scratch+viewport+6,256,2);
    SetupRenderer(shift);
    assert(!memcmp(&actual,&expected,sizeof(actual)));
    /* A second call must replace stale parameters and templates again. */
    SetupRenderer(shift);
    assert(!memcmp(&actual,&expected,sizeof(actual)));
}
int main(int argc,char **argv) {
    assert(argc==2);
    const s32 extremes[]={(-2147483647-1),-65536,-32768,-1,0,1,31,32,32767,65535,65536,2147483647};
    if(atoi(argv[1])==0) {
        for(u32 i=0;i<65536;i++) {
            s32 shift=(s32)i-32768;
            check(shift,i*2654435761U,i*1717U);
        }
    } else {
        for(u32 i=0;i<65536;i++) {
            u32 flags=i|(i<<16);
            for(unsigned j=0;j<sizeof(extremes)/sizeof(*extremes);j++)check(extremes[j],flags,i+j*712U);
        }
        for(u32 i=0;i<65536;i++) {
            u32 raw=i*2654435761U;s32 shift;memcpy(&shift,&raw,4);
            check(shift,raw^0x80000001U,i);
        }
    }
    return 0;
}
'''
        source=Path(cls.temporary.name)/'renderer_setup.c'
        source.write_text(code)
        cls.binary=source.with_suffix('')
        subprocess.run(['cc','-std=gnu99','-O2','-fno-strict-aliasing',
                        '-fsanitize=undefined','-fno-sanitize-recover=all',
                        '-I',str(ROOT/'include'),str(source),'-o',str(cls.binary)],check=True)

    def test_all_signed_halfword_shifts(self):
        subprocess.run([str(self.binary),'0'],check=True)

    def test_flags_and_full_word_shift_extremes(self):
        subprocess.run([str(self.binary),'1'],check=True)


if __name__=='__main__':
    unittest.main()
