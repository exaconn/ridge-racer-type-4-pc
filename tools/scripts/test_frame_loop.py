"""Outer frame loop: actual body, full pointers, helper mutations and u32 wrap."""
from pathlib import Path
import re, subprocess, tempfile, unittest
ROOT=Path(__file__).resolve().parents[2]
from c_functions import definition_source, source_owners
owner=source_owners(ROOT/'src/main/PAL')['MainLoop']
source=definition_source(owner.read_text(),'MainLoop')
source=re.sub(r'\s+(?:__asm__|asm)\("\$\d+"\)','',source)
assert not re.search(r'(?:__asm__|asm)\s*\(',source)
for before,after in [
    ('(u8 *)0x800F0000U','((u8 *)&native_bank)'),
    ('(GpuScratchPointers *)SCRATCHPAD','(&native_scratch)'),
]:
    assert source.count(before)==1,before
    source=source.replace(before,after)
fixture=r'''
#include <assert.h>
#include "rr/frame_storage_types.h"
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <string.h>
#include "rr/layout.h"
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_OFFSET(type,field,offset) _Static_assert(offsetof(type,field)==offset,"native layout")
#include "rr/frame_loop.h"
#include "psyq/libgpu/gpudisp.h"
#include "rr/gpu_scratch_types.h"
FrameStorage DrawBuffers[2];
FrameStep *ModeFuncTable[65536];
FrameStorage *CurrentDrawBuf;
u32 FrameCounter;
s32 VSyncThreshold,DrawSubOtFlag,ModeFrame;
u16 ModeGroup,ModeIndex;
s16 SubmodeState;
static struct {u8 prefix[0x4BF0];s32 bank;} native_bank;
static struct {u32 before[4];GpuScratchPointers value;u32 after[4];} scratch_owner;
#define native_scratch scratch_owner.value
static FrameStorage alternate[8],saved_alternate[8],saved_frames[2];
static FrameStep row[65536];
static jmp_buf done;
static int stage,frame_no,frames_to_run,ticks,optional_mode;
static u32 next_counter,expected_counter;
static u16 selector_a,selector_b;
static s32 threshold;
static void expect(int n) {assert(stage==n);stage++;}
static void step(void) {
 expect(10);assert(ModeGroup==selector_a && ModeIndex==selector_b);
 CurrentDrawBuf=&alternate[2];VSyncThreshold=-999;
}
void MainLoopStartupHook(void) {expect(0);}
s32 SetVideoMode(s32 n) {expect(1);assert(n==1);return INT_MIN;}
void ResetCallback(void) {expect(2);}
void SetMem(s32 n) {expect(3);assert(n==2);}
void SystemInit(void) {expect(4);FrameCounter=42;ModeFrame=77;ModeGroup=0;ModeIndex=0;SubmodeState=-2;}
void SetDispMask(s32 n) {expect(5);assert(n==0 && VSyncThreshold==128);FrameCounter=0xFFFFFFFFU;}
u32 *ClearOTagR(u32 *p,s32 n) {
 assert(n==704);
 if(stage==6) {
  u32 bank=expected_counter&1;u8 *base=DrawBuffers[bank].bytes;
  assert(FrameCounter==expected_counter && CurrentDrawBuf==&DrawBuffers[bank] && native_bank.bank==(s32)bank);
  assert(p==(u32 *)(base+0x70));
  assert(native_scratch.cursor==base+0x1670 && native_scratch.order==(u32 *)(base+0x70));
  if(!frame_no)assert(ModeFrame==0 && ModeGroup==1 && ModeIndex==1 && SubmodeState==0);
  expect(6);CurrentDrawBuf=&alternate[0];
 }else {expect(7);assert(p==(u32 *)(alternate[0].bytes+0xB70));CurrentDrawBuf=&alternate[1];}
 return (u32 *)alternate[7].bytes;
}
s32 CdFileStep(void) {expect(8);ModeGroup=65534;ModeIndex=65534;return -1;}
void SoundUpdate(void) {expect(9);ModeGroup=selector_a;ModeIndex=selector_b;}
void DrawSync(s32 n) {expect(11);assert(n==0);VSyncThreshold=threshold;}
s32 VSync(s32 mode) {
 if(mode==1) {
  assert(stage==12);ticks++;VSyncThreshold=threshold==INT_MAX?INT_MIN:INT_MAX;
  CurrentDrawBuf=&alternate[3];
  if(threshold!=INT_MIN && ticks<3)return threshold-1;
  stage++;return threshold;
 }
 expect(13);assert(mode==0);assert(ticks==(threshold==INT_MIN?1:3));
 CurrentDrawBuf=&alternate[4];return INT_MIN;
}
DisplayEnvironment *PutDispEnv(DisplayEnvironment *p) {expect(14);assert(p==&alternate[4].buffer.environment00.display5C);CurrentDrawBuf=&alternate[5];return p;}
DrawEnvironment *PutDrawEnv(DrawEnvironment *p) {expect(15);assert(p==&alternate[5].buffer.environment00.draw00);CurrentDrawBuf=&alternate[6];return p;}
void DrawOTag(void *p) {
 if(stage==16) {
  expect(16);assert(p==alternate[6].bytes+0xB6C);CurrentDrawBuf=&alternate[7];
  /* Flag is observed after the first draw, including negative nonzero values. */
  DrawSubOtFlag=optional_mode;
 }else {expect(17);assert(optional_mode && p==alternate[7].bytes+0x166C);}
}
void PadUpdate(void) {
 expect(optional_mode?18:17);
 assert(FrameCounter==expected_counter);
 frame_no++;
 if(frame_no==frames_to_run)longjmp(done,1);
 /* Increment must use this fresh word and wrap at 32 bits. */
 FrameCounter=next_counter;expected_counter=next_counter+1U;
 stage=6;ticks=0;DrawSubOtFlag=optional_mode?0:1;
}
''' + source + r'''
static void run(u32 counter,s32 wait,s32 optional,u16 a,u16 b) {
 memset(DrawBuffers,0xA5,sizeof(DrawBuffers));memcpy(saved_frames,DrawBuffers,sizeof(saved_frames));
 memset(alternate,0x6A,sizeof(alternate));memcpy(saved_alternate,alternate,sizeof(saved_alternate));
 memset(&scratch_owner,0xA7,sizeof(scratch_owner));memset(&native_bank,0xB6,sizeof(native_bank));
 stage=frame_no=ticks=0;frames_to_run=3;expected_counter=0;next_counter=counter;
 threshold=wait;optional_mode=optional;selector_a=a;selector_b=b;
 ModeFuncTable[a]=row;row[b]=step;
 DrawSubOtFlag=optional?0:1;
 if(!setjmp(done))MainLoop();
 assert(frame_no==3 && ticks==(wait==INT_MIN?1:3));
 assert(!memcmp(DrawBuffers,saved_frames,sizeof(saved_frames)));
 assert(!memcmp(alternate,saved_alternate,sizeof(saved_alternate)));
 for(unsigned i=0;i<4;i++)assert(scratch_owner.before[i]==0xA7A7A7A7U&&scratch_owner.after[i]==0xA7A7A7A7U);
 for(unsigned i=0;i<sizeof(native_bank.prefix);i++)assert(native_bank.prefix[i]==0xB6);
 assert(native_scratch.cursor==DrawBuffers[expected_counter&1].buffer.packets1670);
 assert(native_scratch.order==DrawBuffers[expected_counter&1].buffer.ordering70.words);
 row[b]=0;ModeFuncTable[a]=0;
}
int main(int argc,char **argv) {
 u32 counters[]={0,1,0x7FFFFFFFU,0x80000000U,0xFFFFFFFEU,0xFFFFFFFFU};
 s32 waits[]={INT_MIN,-1,0,1,128,INT_MAX};
 s32 options[]={0,1,-1,INT_MIN,INT_MAX};
 u16 selectors[]={0,1,32767,32768,65535};
 int i,j,k;
 (void)argv;assert(argc==1);
 assert(sizeof(void*)==8&&(uintptr_t)DrawBuffers>UINT32_MAX&&(uintptr_t)alternate>UINT32_MAX);
 assert(sizeof(FrameStorage)==0x22778 && offsetof(FrameStorage,bytes)==0);
 assert(offsetof(FrameBuffer,packets1670)==0x1670);
 assert(offsetof(FrameBuffer,ordering70)==0x70&&offsetof(FrameBuffer,secondaryOrderingB70)==0xB70);
 assert(offsetof(typeof(native_bank),bank)==0x4BF0);
 for(i=0;i<6;i++)for(j=0;j<6;j++)for(k=0;k<5;k++)
  run(counters[i],waits[j],options[k],selectors[k],selectors[4-k]);
 return 0;
}
'''
class FrameLoopTests(unittest.TestCase):
    def test_actual_loop_with_live_helper_state(self):
        with tempfile.TemporaryDirectory() as tmp:
            c=Path(tmp)/'frame.c'; exe=Path(tmp)/'frame'
            c.write_text(fixture)
            subprocess.run(['cc','-std=gnu99','-O2','-Wall','-Wextra','-Werror','-Wno-error=deprecated-non-prototype',
                            '-fsanitize=undefined','-fno-sanitize-recover=all',
                            '-I',str(ROOT/'include'),str(c),'-o',str(exe)],check=True,capture_output=True)
            subprocess.run([str(exe)],check=True,capture_output=True)
if __name__=='__main__':unittest.main()
