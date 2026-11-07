"""Actual HUD number and glyph body, captured metadata, native aliases and UBSan."""
from pathlib import Path
import re,subprocess,tempfile,unittest
root=Path(__file__).resolve().parents[2]
header=(root/'include/rr/hud_number.h').read_text();header=re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',header,flags=re.M)
body=(root/'tools/scripts/fixtures/views/func_800217D0.c').read_text();body=re.sub(r'^#include[^\n]*\n','',body,flags=re.M);body=re.sub(r' asm\("\$\d+"\)','',body);body=re.sub(r'__asm__\("".*?\);',';',body)
body,n=re.subn(r'\(RrGpuScratchPointers \*\)0x1F800000U','&scratch',body);assert n==2
helper=(root/'tools/scripts/fixtures/views/func_80021568.c').read_text().replace('EmitHudGlyph12','native_glyph').replace('(s32) arg1','(uintptr_t) arg1')
assert helper.count('(uintptr_t) arg1')==1
def h(name):return re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',(root/'include/rr'/name).read_text(),flags=re.M)
code=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "types.h"
'''+h('gpu_scratch_types.h')+h('gpu.h')+h('animated_sprite_grid.h')+header+helper+r'''
typedef struct {RrNumberTarget rows[4];u32 heads[4];u32 arena[96];u8 guard[32];} World;
static World actual,expected;
static RrGpuScratchPointers scratch;
#define HudNumberTargets actual.rows
s32 DrawBufIndex;
static s32 number,x,y;static int selected,bank,iteration,variant;
static u32 *captured_order;
static u8 *cursor,*initial_cursor,*returned;
static s32 digits[5];
static u8 *map(const void *p) {
 ptrdiff_t offset=(const u8 *)p-(const u8 *)&actual;
 assert(offset>=0 && offset<sizeof(actual));return (u8 *)&expected+offset;
}
static void mutate(void) {
 actual.rows[selected].x=expected.rows[selected].x=(s16)(x+iteration*17);
 actual.rows[selected].y=expected.rows[selected].y=(s16)(y-iteration*19);
 actual.rows[selected].heads[bank]=expected.rows[selected].heads[bank]=0;
 DrawBufIndex^=3;scratch.cursor=(u8 *)actual.arena+320;
}
void *EmitHudGlyph12(u32 *order,void *packet,s32 px,s32 py,s32 glyph,s32 clut) {
 u8 *p=packet;
 RrSpritePacket *q=(RrSpritePacket *)map(p);u32 *head=(u32 *)map(captured_order);
 assert(iteration<5 && order==captured_order && p==cursor);
 assert(px==x+(iteration-1)*8 && py==y && glyph==digits[iteration] && clut==0x7EFA);
 if(iteration)assert(scratch.cursor==(u8 *)actual.arena+320);
 q->len=4;q->code=0x65;q->w=12;q->h=16;q->x=(s16)px;q->y=(s16)py;
 q->v=240;q->u=(u8)(glyph*12);q->clut=(u16)clut;
 *(u32 *)q=(*(u32 *)q&0xFF000000U)|(*head&0xFFFFFFU);
 *head=(*head&0xFF000000U)|((uintptr_t)p&0xFFFFFFU);
 assert(native_glyph(order,p,px,py,glyph,clut)==p+20);
 assert(!memcmp(&actual,&expected,sizeof(actual)));
 cursor=p+20;iteration++;mutate();return cursor;
}
u8 *AddDrawModePrim(u32 *order,u8 *p,u16 page) {
 assert(iteration==5 && order==captured_order && p==cursor && page==5);
 assert(scratch.cursor==(u8 *)actual.arena+320);
 returned=p+12+variant*4;return returned;
}
'''+body+r'''
static u32 seed=0x78319ac5;
static u32 next(void) {seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
int main(void) {
 s32 values[]={INT32_MIN,INT32_MAX,-10001,-10000,-1000,-1,0,1,999,1000,9999,10000,10001};
 s16 positions[]={-32768,-32767,-1,0,1,32766,32767};
 unsigned n,j,k;u8 *bytes=(u8 *)&actual;
 assert(sizeof(RrNumberTarget)==sizeof(void *)*5);
 for(n=0;n<65536;n++) {
  for(j=0;j<sizeof(actual);j++)bytes[j]=next();
  selected=n%4;bank=(n/4)%4;variant=n&1;DrawBufIndex=bank;
  number=n<13?values[n]:(s32)next();x=n<14?positions[n%7]:(s16)next();y=n<14?positions[(n+3)%7]:(s16)next();iteration=0;
  actual.rows[selected].x=x;actual.rows[selected].y=y;
  for(j=0;j<4;j++)for(k=0;k<4;k++)actual.rows[j].heads[k]=actual.heads+k;
  initial_cursor=(u8 *)actual.arena;cursor=initial_cursor;scratch.cursor=initial_cursor;scratch.order=actual.heads;
  captured_order=n%3==0?(u32 *)cursor:(n%3==1?(u32 *)(cursor+4):actual.heads+bank);
  actual.rows[selected].heads[bank]=captured_order;
  memcpy(&expected,&actual,sizeof(actual));
  digits[0]=number>=10000;digits[1]=(number/1000)%10;digits[2]=(number/100)%10;digits[3]=(number/10)%10;digits[4]=number%10;
  DrawHudNumber5(number,selected);
  assert(iteration==5 && scratch.cursor==returned && scratch.order==actual.heads);
  assert(!memcmp(&actual,&expected,sizeof(actual)));
 }
 puts("65536 number cases passed with actual glyph helper and UBSan");return 0;
}
'''
class HudNumberTests(unittest.TestCase):
    def test_actual_number_and_glyph_helper_with_ubsan(self):
        with tempfile.TemporaryDirectory() as tmp:
            c=Path(tmp)/'number.c';exe=Path(tmp)/'number';c.write_text(code)
            subprocess.run(['cc','-std=gnu99','-O2','-fsanitize=undefined','-fno-sanitize-recover=all','-I',str(root/'include'),str(c),'-o',str(exe)],check=True,capture_output=True)
            subprocess.run([str(exe)],check=True,capture_output=True)
if __name__=='__main__':unittest.main()
