"""Actual two-bank initialization plus its ready draw/display helpers, UBSan."""
from pathlib import Path
import re,subprocess,tempfile,unittest
root=Path(__file__).resolve().parents[2]
from c_functions import definition_source,source_owners
owners=source_owners(root/'src/main/PAL')
def current_body(name):
 return definition_source(owners[name].read_text(),name)
src=current_body('SetupDrawDispEnvs')
src=re.sub(r'\s+(?:__asm__|asm)\("\$\d+"\)','',src)
# Physical MIPS clobbers have no host register counterpart; retain each empty
# asm's actual operand dependencies while removing only those register names.
src=src.replace('"$2", "$30"','').replace('"$2"','')
assert not re.search(r'"\$\d+"',src)
draw=current_body('SetDefDrawEnv').replace('SetDefDrawEnv','native_draw')
disp=current_body('SetDefDispEnv').replace('SetDefDispEnv','native_display')
hook=current_body('InitFrameEnvStub').replace('InitFrameEnvStub','native_bank_hook')
fixture=r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "types.h"
#include <stddef.h>
#include "rr/layout.h"
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_OFFSET(type,field,offset) _Static_assert(offsetof(type,field)==offset,"native layout")
#include "rr/frame_loop.h"

static FrameStorage actual[2],expected[2];
#define D_800ADAF0 actual
RuntimeSettings Options;
static int step,video;
static u32 input_flags;
static int GetVideoMode(void) {return video;}
''' + draw+disp+hook+r'''
void SetGeomOffset(s32 x,s32 y) {assert(step++==0 && x==160 && y==(s32)((((input_flags>>1)&1)+1)*128));}
void SetGeomScreen(s32 z) {assert(step++==1 && z==290);}
DrawEnvironment *SetDefDrawEnv(DrawEnvironment *e,s32 x,s32 y,s32 w,s32 h) {
 int index=step-2;assert(index>=0 && index<2);step++;
 assert(e==&actual[index].buffer.environment00.draw00);assert(x==0 && y==(index?(s32)((1-((input_flags>>1)&1))*256):0));
 assert(w==((input_flags&4)?480:320) && h==(s32)((((input_flags>>1)&1)+1)*256));
 return native_draw(e,x,y,w,h);
}
DisplayEnvironment *SetDefDispEnv(DisplayEnvironment *e,s32 x,s32 y,s32 w,s32 h) {
 int index=step-4;assert(index>=0 && index<2);step++;
 assert(e==&actual[index].environment.display5C);assert(x==0 && y==(index?0:(s32)((1-((input_flags>>1)&1))*256)));
 assert(w==((input_flags&4)?480:320) && h==(s32)((((input_flags>>1)&1)+1)*256));
 return native_display(e,x,y,w,h);
}
void InitFrameEnvStub(s32 bank) {assert(step++==6+bank && bank>=0 && bank<2);native_bank_hook(bank);}
''' + src + r'''
static void reference(s32 r,s32 g,s16 b,u32 flags) {
 int i;int interlace=(flags>>1)&1,height=(interlace+1)*256,width=(flags&4)?480:320;
 for(i=0;i<2;i++) {
  FrameEnvironment *e=&expected[i].environment;
  e->draw00.clip.x=0;e->draw00.clip.y=i?(1-interlace)*256:0;e->draw00.clip.w=width;e->draw00.clip.h=height;
  e->draw00.offset_x=e->draw00.clip.x;e->draw00.offset_y=e->draw00.clip.y;
  e->draw00.page=10;e->draw00.dither=1;e->draw00.draw_display=video?(height<289):(height<257);
  e->draw00.background=flags&1;e->draw00.r=(u8)r;e->draw00.g=(u8)g;e->draw00.b=(u8)b;
  e->draw00.window.x=e->draw00.window.y=e->draw00.window.w=e->draw00.window.h=0;
  e->display5C.display.x=0;e->display5C.display.y=i?0:(1-interlace)*256;e->display5C.display.w=320;e->display5C.display.h=height;
  e->display5C.screen.x=Options.halves[26];e->display5C.screen.y=Options.halves[28];e->display5C.screen.w=0;e->display5C.screen.h=256;
  e->display5C.flags10.bytes.interlace=interlace;e->display5C.flags10.bytes.rgb24=(flags&4)?1:0;e->display5C.flags10.bytes.mode=e->display5C.flags10.bytes.reserved=0;
 }
}
static void run(s32 r,s32 g,s16 b,u32 flags,int mode,u16 sx,u16 sy) {
 memset(actual,0xA7,sizeof(actual));memcpy(expected,actual,sizeof(actual));
 input_flags=flags;step=0;video=mode;Options.halves[26]=sx;Options.halves[28]=sy;
 reference(r,g,b,flags);SetupDrawDispEnvs(r,g,b,flags);
 assert(step==8 && !memcmp(actual,expected,sizeof(actual)));
}
int main(void) {
 u32 flags[]={0,1,2,3,4,5,6,7,0xFFFFFFFFU,0x80000000U,0x7FFFFFFFU,0xFFFFFFF8U};
 s32 colors[]={INT32_MIN,-1,0,1,127,128,255,256,INT32_MAX};
 int i,j,k;u32 seed=0x12345678;
 assert(sizeof(FrameEnvironment)==0x70 && sizeof(FrameStorage)==0x22778);
 for(i=0;i<12;i++)for(j=0;j<9;j++)for(k=0;k<2;k++)run(colors[j],colors[8-j],(s16)colors[j],flags[i],k,0x8000,0xFFFF);
 for(i=0;i<2000;i++){seed=seed*1664525U+1013904223U;run((s32)seed,(s32)(seed^0x55555555U),(s16)(seed>>16),seed,(seed>>3)&1,(u16)seed,(u16)(seed>>16));}
 return 0;
}
'''
class FrameEnvironmentTests(unittest.TestCase):
    def test_actual_initialization_with_real_helpers(self):
        with tempfile.TemporaryDirectory() as t:
            c=Path(t)/'env.c';exe=Path(t)/'env';c.write_text(fixture)
            result=subprocess.run(['cc','-std=gnu99','-O2','-Wall','-Wextra','-Werror','-Wno-unused-parameter','-fsanitize=undefined','-fno-sanitize-recover=all','-I',str(root/'include'),str(c),'-o',str(exe)],capture_output=True,text=True)
            self.assertEqual(result.returncode,0,result.stderr)
            subprocess.run([str(exe)],check=True,capture_output=True)
if __name__=='__main__':unittest.main()
