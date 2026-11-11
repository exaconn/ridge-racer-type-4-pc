"""Draw-area packet with real resident constructor and coordinate helpers."""
from overlay_units import read_c_source
from pathlib import Path
from tempfile import TemporaryDirectory
import subprocess
import unittest
from c_functions import definition_source, source_owners

class Overlay662DrawAreaTest(unittest.TestCase):
    def test_geometry_shift_links_and_real_constructor(self):
        self.run_geometry(False)

    def test_full_word_dimensions_use_the_canonical_entry(self):
        self.run_geometry(True)

    def run_geometry(self, raw_dimensions):
        root=Path(__file__).resolve().parents[2]
        body=read_c_source(root/'src/overlays/PAL/garage/func_8011ABB4.c')
        owners=source_owners(root/'src/main/PAL')
        resident='\n'.join(definition_source(owners[name].read_text(),name) for name in ('get_cs','get_ce'))
        constructor=definition_source(owners['SetDrawArea'].read_text(),'SetDrawArea')
        source=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "rr/layout.h"
#undef RR_ASSERT_SIZE
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_SIZE(t,n) _Static_assert(sizeof(void*)!=4 || sizeof(t)==n,"PAL size")
#define RR_ASSERT_OFFSET(t,f,n) _Static_assert(sizeof(void*)!=4 || offsetof(t,f)==n,"PAL offset")
#include "rr/overlay662_draw_area.h"
#include "rr/overlay662_garage.h"
#include "rr/frame_loop.h"
#include "psyq/libgpu/gpudisp.h"
#include "rr/gpu_address.h"
_Static_assert(sizeof(FrameStorage)==0x22778,"real frame stride");
_Static_assert(offsetof(FrameEnvironment,display5C.display.y)==0x5E,"comparison halfword");
FrameStorage DrawBuffers[2];static FrameStorage expected_frames[2];
GarageSelectionPrefix ovl662_editor_workspace;static GarageSelectionPrefix saved;
s32 DrawBufIndex;s16 D_800ABAC4,D_800ABAC6;
static struct Packets {u32 before[3],words[9],head,after[3];} actual,expected;
static GpuDmaRegion region;
'''+resident+constructor+body+r'''
static u32 rng=0x1ABB4;
static u32 next(void){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;}
static s32 word(u32 n){s32 v;memcpy(&v,&n,4);return v;}
static s16 half(u32 bits){u16 h=(u16)bits;s16 value;memcpy(&value,&h,2);return value;}
static s16 narrow(int64_t n){return half((u32)((uint64_t)n&65535U));}
static u32 command(s16 x,s16 y,u32 code){
 int64_t a=x,b=y;if(a<0)a=0;else if(a>=D_800ABAC4)a=D_800ABAC4-1;
 if(b<0)b=0;else if(b>=D_800ABAC6)b=D_800ABAC6-1;
 return code|((u32)a&1023U)|(((u32)b&1023U)*1024U);
}
static u32 *head(struct Packets *p,unsigned shape){
 static const unsigned index[]={3,4,5,0,6,2};return shape==0?&p->head:&p->words[index[shape-1]];
}
int main(void){unsigned i,shape,j;u32 x,y,w,h;uint64_t cases=0;
 static const s32 clocks[]={INT32_MIN,INT32_MAX,-1,0,1,0x00800000,-0x00800001,0x12345678};
 memset(&ovl662_editor_workspace,0,sizeof(ovl662_editor_workspace));
 region=(GpuDmaRegion){&actual,0x80180000U,sizeof(actual)};assert(GpuDmaSetRegions(&region,1));
 for(i=0;i<65536;i++){
  s16 rx,ry;int64_t yy;
  ovl662_editor_workspace.halfC8=half(i);ovl662_editor_workspace.halfCA=half(i*73U+17);
  memcpy(&saved,&ovl662_editor_workspace,sizeof(saved));
  DrawBuffers[0].environment.display5C.display.y=half(i);
  DrawBuffers[1].environment.display5C.display.y=half(i+(i&1));
  expected_frames[0].environment.display5C.display.y=DrawBuffers[0].environment.display5C.display.y;
  expected_frames[1].environment.display5C.display.y=DrawBuffers[1].environment.display5C.display.y;
  DrawBufIndex=i%9==8?word(next()):clocks[i%8];
  D_800ABAC4=i%3==0?1:i%3==1?320:1024;D_800ABAC6=i%3==0?512:i%3==1?256:1;
  x=next();y=next();w=(u16)i;h=(u16)(i*73U+17);
  rx=narrow((int64_t)saved.halfC8+x);ry=narrow((int64_t)saved.halfCA+y);
  yy=ry;if(i&1)yy+=(int64_t)DrawBufIndex*256;ry=narrow(yy);
  for(shape=0;shape<7;shape++){
   u32 *ordering,*eorder,*pkt=actual.words+3;u8 *result;
   for(j=0;j<3;j++){actual.before[j]=next();actual.after[j]=next();}
   for(j=0;j<9;j++)actual.words[j]=next();actual.head=next();
   memcpy(&expected,&actual,sizeof(actual));ordering=head(&actual,shape);eorder=head(&expected,shape);
   expected.words[3]=(expected.words[3]&0x00FFFFFFU)|0x02000000U;
   expected.words[4]=command(rx,ry,0xE3000000U);
   expected.words[5]=command(narrow((int64_t)(u16)rx+w-1),narrow((int64_t)(u16)ry+h-1),0xE4000000U);
   expected.words[3]=(expected.words[3]&0xFF000000U)|(*eorder&0xFFFFFFU);
   *eorder=(*eorder&0xFF000000U)|((region.address+(u32)((u8*)pkt-(u8*)&actual))&0xFFFFFFU);
   result=ovl662_emit_draw_area(ordering,pkt,x,y,(u16)w,(u16)h);
   assert(result==(u8*)(actual.words+6));assert((uintptr_t)result>UINT32_MAX);
   assert(memcmp(&actual,&expected,sizeof(actual))==0);
   assert(memcmp(&saved,&ovl662_editor_workspace,sizeof(saved))==0);cases++;
  }
 }
 assert(memcmp(DrawBuffers,expected_frames,sizeof(expected_frames))==0);
 assert(cases==458752ULL);return 0;
}
'''
        if raw_dimensions:
            source=source.replace('w=(u16)i;h=(u16)(i*73U+17);',
                                  'w=(u16)i|0xACF00000U;h=(u16)(i*73U+17)|0x13570000U;')
            source=source.replace('x,y,(u16)w,(u16)h);','x,y,w,h);')
        with TemporaryDirectory() as tmp:
            p=Path(tmp);(p/'test.c').write_text(source)
            subprocess.run(['cc','-std=gnu11','-O2','-fsanitize=undefined','-fno-sanitize-recover=all','-I',str(root/'include'),str(p/'test.c'),str(root/'src/native/gpu_address.c'),'-o',str(p/'test')],check=True)
            subprocess.run([str(p/'test')],check=True)

if __name__=='__main__':unittest.main()
