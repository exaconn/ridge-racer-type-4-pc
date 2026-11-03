from overlay_units import read_c_source
"""Scene exits reload their frame after real reset/transition helpers."""
from pathlib import Path
import subprocess
from tempfile import TemporaryDirectory
import unittest

class MenuScreenExitTest(unittest.TestCase):
    def test_helpers_frames_and_halfword_cells(self):
        root=Path(__file__).resolve().parents[2]
        bodies="\n".join(read_c_source(root/p) for p in [
            'tools/scripts/fixtures/views/func_80036068.c','tools/scripts/fixtures/views/func_8003CAB4.c',
            'src/overlays/PAL/660/func_8011712C.c','src/overlays/PAL/661/func_80114AA0.c'])
        source=r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include "rr/layout.h"
#undef RR_ASSERT_SIZE
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_SIZE(t,n) _Static_assert(sizeof(t)==n,"size")
#define RR_ASSERT_OFFSET(t,f,n) _Static_assert(offsetof(t,f)==n,"offset")
#include "rr/menu_screen_exit.h"
_Static_assert(__builtin_types_compatible_p(__typeof__(D_800F4C6A),u16),"mode width");
u16 D_800F4C6A,D_800AD50A,D_8009F914[256];
s16 D_800F3A26,D_800F4D8A,D_800F4D44;
s32 D_800F2EC8;
RrGpuRectangle *D_800ACC14;
static RrFrameOrderingPrefix frames[4],saved[4];
static unsigned phase,case_id,which;
static u32 rng=0x1712C;
static u32 next(void){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;}
static void mutate(unsigned n){
 D_800F4C6A=(u16)(case_id+n);D_800F3A26=(s16)(case_id-n);
 D_800F4D8A=(s16)(case_id+n*3);D_800F2EC8=(s32)(case_id+n);
 D_800AD50A=(u16)(case_id^n);D_800F4D44=(s16)((case_id+n)&255);
 D_800ACC14=&frames[n].rectangle;
}
int func_8004FC18(s32 a,s32 b){assert(phase++==0);assert(a==0&&b==0);mutate(0);return -1;}
int func_80036030(void){assert(phase++==1);mutate(1);return 7;}
void func_8003CAF4(void){assert(phase++==2);assert(D_800F2EC8==0);mutate(2);}
void func_800207D4(u32 *ordering,s32 intensity,u32 mode){
 assert(phase++==3);assert(ordering==frames[2].ordering70);
 assert((uintptr_t)ordering>UINT32_MAX);
 assert(intensity==255&&mode==69);
 assert(D_800F4C6A==1&&D_800F3A26==(which?38:53)&&D_800F4D8A==0);
 assert(D_800AD50A==D_8009F914[(case_id+2)&255]);
 assert(D_800F2EC8==(s32)(case_id+2));
 assert(memcmp(frames,saved,sizeof(frames))==0);
 ordering[(case_id>>8)%704]^=0x76543210U;
 mutate(3);
}
'''+bodies+r'''
int main(void){unsigned i,j;
 for(i=0;i<256;i++)D_8009F914[i]=(u16)(i*257U);
 for(which=0;which<2;which++)for(case_id=0;case_id<65536;case_id++){
  for(j=0;j<sizeof(frames);j++)((u8*)frames)[j]=(u8)next();
  memcpy(saved,frames,sizeof(frames));phase=0;
  D_800F4C6A=(u16)case_id;D_800F3A26=(s16)case_id;D_800F4D8A=(s16)~case_id;
  D_800ACC14=&frames[3].rectangle;
  if(which)ovl661_func_80114AA0();else ovl660_func_8011712C();
  assert(phase==4);assert(D_800ACC14==&frames[3].rectangle);
  assert(D_800F4C6A==(u16)(case_id+3));assert(D_800F3A26==(s16)(case_id-3));
  assert(D_800F4D8A==(s16)(case_id+9));assert(D_800F2EC8==(s32)(case_id+3));
  assert(D_800AD50A==(u16)(case_id^3));
  saved[2].ordering70[(case_id>>8)%704]^=0x76543210U;
  assert(memcmp(frames,saved,sizeof(frames))==0);
 }
 return 0;
}
'''
        with TemporaryDirectory() as tmp:
            p=Path(tmp);(p/'test.c').write_text(source)
            subprocess.run(['cc','-std=gnu11','-O2','-fsanitize=undefined','-fno-sanitize-recover=all','-I',str(root/'include'),str(p/'test.c'),'-o',str(p/'test')],check=True)
            subprocess.run([str(p/'test')],check=True)

if __name__=='__main__':unittest.main()
