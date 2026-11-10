"""Garage digital motion and repeat masks, with live overlapping storage."""
from pathlib import Path
import subprocess
from tempfile import TemporaryDirectory
import unittest

class GarageRepeatTest(unittest.TestCase):
    def test_motion_and_live_repeat_owners(self):
        root=Path(__file__).resolve().parents[2]
        bodies='\n'.join((root/f'src/overlays/PAL/662/func_{a}.c').read_text() for a in ['8011D664','8011D730','8011D8E4'])
        source=r'''
#include <assert.h>
#include "game/system/pad.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "rr/layout.h"
#undef RR_ASSERT_SIZE
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_SIZE(t,n) _Static_assert(sizeof(void*)!=4 || sizeof(t)==n,"target size")
#define RR_ASSERT_OFFSET(t,f,n) _Static_assert(sizeof(void*)!=4 || offsetof(t,f)==n,"target offset")
#include "rr/overlay662_input.h"
typedef union {ControllerState5C rows[3];u32 words[69];} ControllerOwner;
static struct World {u32 guard[4];ControllerOwner controller;u32 counters[8];u32 mask;GaragePoint out;u32 tail[4];} w,e;
#define PlayerControllers w.controller.rows
#define ovl662_stick_repeat_up w.counters[0]
#define ovl662_stick_repeat_down w.counters[1]
#define ovl662_stick_repeat_left w.counters[2]
#define ovl662_stick_repeat_right w.counters[3]
#define ovl662_dpad_repeat_up w.counters[4]
#define ovl662_dpad_repeat_down w.counters[5]
#define ovl662_dpad_repeat_left w.counters[6]
#define ovl662_dpad_repeat_right w.counters[7]
static unsigned calls;
void *PsyqMemset(void *p,s32 value,u32 n){assert(value==0 && n==4);calls++;return memset(p,value,n);}
''' + bodies + r'''
static u32 seed=0x1d664;
static u32 next(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
static void randomize(void){unsigned i;for(i=0;i<sizeof(w);i++)((u8*)&w)[i]=(u8)next();}
static u32 *slot(struct World *v,unsigned index){
 if(index==0)return &v->mask;
 if(index<=8)return &v->counters[index-1];
 if(index==9)return &v->controller.words[0];
 if(index==10)return &v->controller.words[5];
 return &v->controller.words[16];
}
static void reference_repeat(unsigned analog,unsigned where){
 unsigned i,base=analog?0:4;u32 mask=0;unsigned axis=e.controller.rows[0].axes14.bytes[3];
 const u32 bits[4]={0x1000,0x4000,0x8000,0x2000};
 if(analog){if(e.controller.rows[0].input00.fields.type!=0x73 || e.controller.rows[0].kind1C==0 || e.controller.rows[0].flag40!=0)return;}
 else if(e.controller.rows[0].input00.fields.type==0x12)return;
 for(i=0;i<4;i++){
  int active;
  if(analog){if(i==2)axis=e.controller.rows[0].axes14.bytes[2];active=(i&1)?axis>224:axis<32;}
  else active=(e.controller.rows[0].input00.fields.buttons&bits[i])!=0;
  if(active){u32 value=e.counters[base+i]+1U;
   if(value>=47 && value<0x80000000U)value=40;
   e.counters[base+i]=value;
   if(value==1 || value==40)mask|=1U<<i;
   *slot(&e,where)=mask;
  }else e.counters[base+i]=0;
 }
}
static void repeat_case(unsigned analog,unsigned where){
 memcpy(&e,&w,sizeof(w));reference_repeat(analog,where);calls=0;
 if(analog)ovl662_poll_stick_direction_repeat(slot(&w,where));else ovl662_poll_dpad_direction_repeat(slot(&w,where));
 assert(calls==0 && memcmp(&w,&e,sizeof(w))==0);
}
int main(void){unsigned type,b,i,j,k;const u32 edges[]={0,1,2,38,39,40,41,45,46,47,48,0x7ffffffe,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};
 /* Every button word for every controller type. */
 for(type=0;type<256;type++)for(b=0;b<65536;b++){
  int x=0,y=0,step=(b&8)?4:1;
  memset(&w,(u8)(b+type),sizeof(w));w.controller.rows[0].input00.fields.type=type;w.controller.rows[0].input00.fields.buttons=b;
  memcpy(&e,&w,sizeof(w));calls=0;
  if(type!=18){if(b&0x1000)y=-2*step;if(b&0x4000)y=2*step;if(b&0x8000)x=-step;if(b&0x2000)x=step;
   if(x || y){e.out.x=x;e.out.y=y;}}
  ovl662_read_dpad_cursor_delta(&w.out);
  assert(calls==1 && memcmp(&w,&e,sizeof(w))==0);
 }
 /* Exhaustive axis pairs crossed with all counter boundaries and output owners. */
 for(k=0;k<16;k++)for(i=0;i<256;i++)for(j=0;j<256;j++){
  randomize();w.controller.rows[0].input00.fields.type=0x73;w.controller.rows[0].kind1C=(i&1)?-1:1;w.controller.rows[0].flag40=0;
  w.controller.rows[0].axes14.bytes[3]=i;w.controller.rows[0].axes14.bytes[2]=j;
  for(b=0;b<8;b++)w.counters[b]=edges[(k+b)&15];
  repeat_case(1,(i+j+k)%12);
 }
 /* Every digital button word, every boundary and overlapping output owner. */
 for(k=0;k<16;k++)for(b=0;b<65536;b++){
  randomize();w.controller.rows[0].input00.fields.type=(b&1)?0x73:0x41;w.controller.rows[0].input00.fields.buttons=b;
  for(i=0;i<8;i++)w.counters[i]=edges[(k+i)&15];
  repeat_case(0,(b+k)%12);
 }
 /* Gating: exact types, signed kind and every flag byte; full-word counters. */
 for(k=0;k<131072;k++){
  randomize();w.controller.rows[0].input00.fields.type=(k&1)?0x73:(u8)k;w.controller.rows[0].flag40=(u8)(k>>1);
  if((k&7)==0)w.controller.rows[0].kind1C=0;
  for(i=0;i<8;i++)w.counters[i]=next();
  repeat_case(k&1,k%12);
 }
 assert((uintptr_t)&w>UINT32_MAX);
 return 0;
}
'''
        with TemporaryDirectory() as temp:
            p=Path(temp);(p/'test.c').write_text(source)
            subprocess.run(['cc','-std=gnu11','-O2','-fsanitize=undefined','-fno-sanitize-recover=all','-I',str(root/'include'),str(p/'test.c'),'-o',str(p/'test')],check=True)
            subprocess.run([str(p/'test')],check=True)

if __name__=='__main__':unittest.main()
