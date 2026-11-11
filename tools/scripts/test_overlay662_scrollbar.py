"""Scrollbar geometry: all signed16 coordinates and every defined byte zoom."""
from overlay_units import read_c_source
from pathlib import Path
from tempfile import TemporaryDirectory
import re
import subprocess
import unittest

class Overlay662ScrollbarTest(unittest.TestCase):
    def test_all_coordinates_and_defined_divisors(self):
        root=Path(__file__).resolve().parents[2]
        body=read_c_source(root/'src/overlays/PAL/garage/func_8011C174.c')
        body=re.sub(r' __asm__\("\$[0-9]+"\)','',body)
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
#include "rr/overlay662_scrollbar.h"
GarageSelectionPrefix ovl662_editor_workspace;
static GarageSelectionPrefix saved;
static u32 pixels[8];
'''+body+r'''
static s16 half(u32 bits){u16 h=(u16)bits;s16 value;memcpy(&value,&h,2);return value;}
static s16 narrow(int64_t value){u16 bits=(u16)((uint64_t)value&65535U);s16 result;memcpy(&result,&bits,2);return result;}
int main(void){unsigned zoom,coord,axis;uint64_t cases=0;
 static const s32 modes[]={1,-1,65536,INT32_MIN,INT32_MAX};
 memset(&ovl662_editor_workspace,0,sizeof(ovl662_editor_workspace));
 ovl662_editor_workspace.image38[0]=pixels;ovl662_editor_workspace.image40[1]=pixels+3;ovl662_editor_workspace.palette48[0]=pixels+5;
 ovl662_editor_workspace.fade00=INT32_MIN;ovl662_editor_workspace.word04=INT32_MAX;
 for(zoom=2;zoom<256;zoom++){
  if(zoom==3)continue;
  ovl662_editor_workspace.config1C=(u8)zoom;
  for(coord=0;coord<65536;coord++){
   ovl662_editor_workspace.reset08[0]=half(coord);
   ovl662_editor_workspace.reset08[1]=half(coord*73U+17);
   memcpy(&saved,&ovl662_editor_workspace,sizeof(saved));
   for(axis=0;axis<2;axis++){
    struct {u16 before;GpuRectangleFields rect;u16 after;} output;
    GpuRectangleFields expected;
    int64_t extent=axis?224:112,denominator=zoom/2;
    int64_t length=extent/denominator;
    int64_t position=axis?87:90;
    int64_t source=axis?saved.reset08[1]:saved.reset08[0];
    if(zoom!=2)position+=source*(extent-length)/(64-64/denominator);
    expected.x=axis?216:narrow(position);expected.y=axis?narrow(position):337;
    expected.w=axis?4:(s16)length;expected.h=axis?(s16)length:8;
    memset(&output,0,sizeof(output));output.before=0xFACE;output.after=0xBEAD;
    ovl662_compute_scrollbar_thumb(axis?modes[(coord+zoom)%5]:0,&output.rect);
    assert(memcmp(&output.rect,&expected,sizeof(expected))==0);
    assert(output.before==0xFACE&&output.after==0xBEAD);cases++;
   }
   assert(memcmp(&saved,&ovl662_editor_workspace,sizeof(saved))==0);
  }
 }
 assert(cases==33161216ULL);return 0;
}
'''
        with TemporaryDirectory() as tmp:
            p=Path(tmp);(p/'test.c').write_text(source)
            subprocess.run(['cc','-std=gnu11','-O2','-fsanitize=undefined','-fno-sanitize-recover=all','-I',str(root/'include'),str(p/'test.c'),'-o',str(p/'test')],check=True)
            subprocess.run([str(p/'test')],check=True)

if __name__=='__main__':unittest.main()
