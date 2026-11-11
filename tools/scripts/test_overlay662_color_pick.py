"""Pixel color picker with real hit testing and actual sound-wrapper closure."""
from overlay_units import read_c_source
from pathlib import Path
from tempfile import TemporaryDirectory
import re
import subprocess
import unittest

class Overlay662ColorPickTest(unittest.TestCase):
    def test_all_pixels_color_pairs_zoom_and_gates(self):
        root=Path(__file__).resolve().parents[2]
        paths=['src/overlays/PAL/garage/func_8011E6C8.c','tools/scripts/fixtures/views/func_8004F218.c','tools/scripts/fixtures/views/func_8004F24C.c','src/overlays/PAL/garage/func_80125370.c']
        bodies='\n'.join(re.sub(r'^extern[^\n]*;\n','',read_c_source(root/p),flags=re.M) for p in paths)
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
#include "rr/overlay662_color_pick.h"
#include "rr/audio_pan.h"
GarageSelectionPrefix ovl662_editor_workspace;GarageWorkingStorage ovl662_editor_cursor_state;
static GarageSelectionPrefix expected;static GarageWorkingStorage expected_work;
static struct Image {u32 before;GarageImage512 words;u32 after;} images[4],image_model[4];
static unsigned audio_calls,expected_mode,expected_before,after_color;
void TriggerSfx(s32 program,s32 level,s32 pan,s32 pitch,s32 voice){
 assert((unsigned)program==expected_mode&&level==127&&pan==64&&pitch==0x1E00&&voice==-1);
 assert(ovl662_editor_workspace.config22==expected_before);audio_calls++;
 ovl662_editor_workspace.config22=(u8)after_color;ovl662_editor_workspace.config1C=0;ovl662_editor_workspace.active21=255;
 ovl662_editor_workspace.reset08[0]=INT16_MIN;ovl662_editor_workspace.image38[0]=0;
 ovl662_editor_cursor_state.words[1]^=0x12345678U;
}
'''+bodies+r'''
static unsigned origin(unsigned pixel,unsigned crop){return pixel<crop/2?0:pixel-crop/2>64-crop?64-crop:pixel-crop/2;}
static void setpixel(struct Image *p,unsigned x,unsigned y,unsigned color){
 u8 *bytes=(u8*)&p->words;unsigned at=y*32+x/2;
 bytes[at]=x%2?(u8)((color*16)|11):(u8)(0xA0|color);
}
int main(void){unsigned pixel,pair,active,mode,j;uint64_t cases=0;
 static const unsigned zooms[]={2,4,8,16};
 static const s16 outside[8][2]={{81,71},{210,71},{82,70},{82,327},{INT16_MIN,71},{INT16_MAX,71},{82,INT16_MIN},{82,INT16_MAX}};
 for(j=0;j<4;j++){images[j].before=0xFACE1234U+j;images[j].after=0xDEAD5678U+j;memset(images[j].words,(int)(0x21+j*17),2048);}
 memcpy(image_model,images,sizeof(images));
 for(mode=0;mode<10;mode++){
  GarageSelectionPrefix before;GarageWorkingStorage work;
  memset(&ovl662_editor_workspace,0,sizeof(ovl662_editor_workspace));memset(&ovl662_editor_cursor_state,0,sizeof(ovl662_editor_cursor_state));
  ovl662_editor_workspace.active21=255;ovl662_editor_workspace.config1C=0;
  ovl662_editor_cursor_state.words[1]=mode<8?2:mode==8?0:1;
  ovl662_editor_cursor_state.halves[10]=mode<8?outside[mode][0]:82;
  ovl662_editor_cursor_state.halves[11]=mode<8?outside[mode][1]:71;
  memcpy(&before,&ovl662_editor_workspace,sizeof(before));memcpy(&work,&ovl662_editor_cursor_state,sizeof(work));
  ovl662_apply_color_picker();assert(audio_calls==0);
  assert(memcmp(&before,&ovl662_editor_workspace,sizeof(before))==0&&memcmp(&work,&ovl662_editor_cursor_state,sizeof(work))==0);cases++;
 }
 for(pixel=0;pixel<4096;pixel++)for(pair=0;pair<256;pair++)for(active=0;active<2;active++){
  unsigned x=pixel%64,y=pixel/64,zoom=zooms[(pair>>4)&3],crop=128/zoom;
  unsigned ox=origin(x,crop),oy=origin(y,crop),first=pair%16,second=pair/16,selected=first?first:second;
  unsigned b0=active,b1=2+active,at=y*32+x/2;
  unsigned old=(pixel+pair+active)&255,before_calls=audio_calls;
  memset(&ovl662_editor_workspace,0,sizeof(ovl662_editor_workspace));memset(&ovl662_editor_cursor_state,0,sizeof(ovl662_editor_cursor_state));
  for(j=0;j<2;j++){ovl662_editor_workspace.image38[j]=images[j].words;ovl662_editor_workspace.image40[j]=images[2+j].words;}
  if(pair%17==0){b1=b0;ovl662_editor_workspace.image40[active]=images[b0].words;}
  setpixel(&images[b0],x,y,first);setpixel(&image_model[b0],x,y,first);
  setpixel(&images[b1],x,y,second);setpixel(&image_model[b1],x,y,second);
  ovl662_editor_workspace.active21=(u8)active;ovl662_editor_workspace.config1C=(u8)zoom;ovl662_editor_workspace.config22=(u8)old;
  ovl662_editor_workspace.reset08[0]=(s16)ox;ovl662_editor_workspace.reset08[1]=(s16)oy;
  ovl662_editor_cursor_state.words[1]=2;
  ovl662_editor_cursor_state.halves[10]=(s16)(82+(x-ox)*zoom+(pair+active)%zoom);
  ovl662_editor_cursor_state.halves[11]=(s16)(71+(y-oy)*zoom*2+(pair+active)%(zoom*2));
  assert(ovl662_editor_cursor_state.halves[10]<210&&ovl662_editor_cursor_state.halves[11]<327);
  expected_mode=selected?1:4;expected_before=selected?selected:old;after_color=(pair^pixel^0x9D)&255;
  memcpy(&expected,&ovl662_editor_workspace,sizeof(expected));memcpy(&expected_work,&ovl662_editor_cursor_state,sizeof(expected_work));
  expected.config22=(u8)after_color;expected.config1C=0;expected.active21=255;expected.reset08[0]=INT16_MIN;expected.image38[0]=0;
  expected_work.words[1]^=0x12345678U;
  ovl662_apply_color_picker();assert(audio_calls==before_calls+1);
  assert(memcmp(&expected,&ovl662_editor_workspace,sizeof(expected))==0);
  assert(memcmp(&expected_work,&ovl662_editor_cursor_state,sizeof(expected_work))==0);
  assert(((u8*)&images[b0].words)[at]==((u8*)&image_model[b0].words)[at]);
  assert(((u8*)&images[b1].words)[at]==((u8*)&image_model[b1].words)[at]);
  assert((uintptr_t)images[b0].words>UINT32_MAX);cases++;
 }
 assert(memcmp(images,image_model,sizeof(images))==0);assert(audio_calls==2097152U);assert(cases==2097162ULL);return 0;
}
'''
        with TemporaryDirectory() as tmp:
            p=Path(tmp);(p/'test.c').write_text(source)
            subprocess.run(['cc','-std=gnu11','-O2','-fsanitize=undefined','-fno-sanitize-recover=all','-I',str(root/'include'),str(p/'test.c'),'-o',str(p/'test')],check=True)
            subprocess.run([str(p/'test')],check=True)

if __name__=='__main__':unittest.main()
