"""Actual textured-triangle helper and command setters with native pointers."""
from pathlib import Path
from tempfile import TemporaryDirectory
import subprocess,unittest
class TexturedTriangleTest(unittest.TestCase):
 def test_packet_and_pointer_abi(self):
  root=Path(__file__).resolve().parents[2]
  bodies='\n'.join((root/f'src/main/PAL/func_{a}.c').read_text() for a in ['80095B74','80095AF4','80095B24','8002039C'])
  code=r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "rr/textured_triangle.h"
static struct World {u32 guard[4];union {u32 words[24];struct S32 packets[3];} arena;s32 x[3],y[3],u[3],v[3];u32 head,tail[4];} world,expected;
static u32 *expected_head,*expected_packet;static unsigned calls;
void func_80095A44(u32 *head,u32 *packet){assert(calls++==0&&head==expected_head&&packet==expected_packet);assert((uintptr_t)head>UINT32_MAX&&(uintptr_t)packet>UINT32_MAX);assert(memcmp(&world,&expected,sizeof(world))==0);*packet=(*packet&0xFF000000U)|(*head&0xFFFFFFU);*head=(*head&0xFF000000U)|((uintptr_t)packet&0xFFFFFFU);}
'''+bodies+r'''
static u32 seed=0x2039c;static u32 next(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
static s32 bits(u32 b){s32 r;memcpy(&r,&b,4);return r;}
static void put16(u8 *p,u16 v){memcpy(p,&v,2);}
int main(void){assert(sizeof(void*)==8);
 for(unsigned n=0;n<262144;n++){
  memset(&world,0xA6,sizeof(world));for(unsigned i=0;i<24;i++)world.arena.words[i]=next();world.head=next();
  for(unsigned i=0;i<3;i++){world.x[i]=bits(next());world.y[i]=bits(next());world.u[i]=bits(next());world.v[i]=bits(next());}
  u16 tpage=(u16)n,clut=(u16)(n*73U),mode=(u16)(n/65536==0?0:n/65536==1?1:n),shade=(u16)(n/65536<2?n:n%3==0?128:next());
  unsigned slot=(n>>2)%2;u8 *packet=(u8*)&world.arena.packets[slot];expected=world;u8 *out=(u8*)&expected.arena.packets[slot];
  out[3]=7;out[7]=(u8)(0x24|(mode?2:0)|(shade==128?1:0));
  for(unsigned i=0;i<3;i++){put16(out+8+8*i,(u16)world.x[i]);put16(out+10+8*i,(u16)world.y[i]);out[12+8*i]=(u8)world.u[i];out[13+8*i]=(u8)world.v[i];}
  if(shade!=128)out[4]=out[5]=out[6]=(u8)shade;put16(out+22,tpage);put16(out+14,clut);
  /* Ordering may alias a real packet word,including tag and the last word. */
  unsigned kind=(n>>4)%3;expected_head=kind==0?&world.head:kind==1?world.arena.words+slot*8:world.arena.words+slot*8+7;
  expected_packet=world.arena.words+slot*8;calls=0;
  struct S32 *result=func_8002039C(expected_head,(struct S32*)packet,world.x,world.y,world.u,world.v,tpage,clut,mode,shade);
  u32 *eh=kind==0?&expected.head:kind==1?expected.arena.words+slot*8:expected.arena.words+slot*8+7;u32 *ep=expected.arena.words+slot*8;
  *ep=(*ep&0xFF000000U)|(*eh&0xFFFFFFU);*eh=(*eh&0xFF000000U)|((uintptr_t)expected_packet&0xFFFFFFU);
  assert(calls==1&&result==&world.arena.packets[slot+1]&&memcmp(&world,&expected,sizeof(world))==0);
 }
 puts("262144 cases;actual command setters,full native pointer ABI and ordering aliases");return 0;
}
'''
  with TemporaryDirectory() as d:
   c=Path(d)/'test.c';exe=Path(d)/'test';c.write_text(code)
   subprocess.run(['cc','-std=gnu11','-O2','-Wall','-Wextra','-Werror','-fsanitize=undefined','-fno-sanitize-recover=all','-I'+str(root/'include'),str(c),'-o',str(exe)],check=True)
   subprocess.run([str(exe)],check=True)
if __name__=='__main__':unittest.main()
