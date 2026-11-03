"""Run the actual sprite/page emitters with native pointers and packet oracles."""
from pathlib import Path
from tempfile import TemporaryDirectory
import subprocess

sources = ['800940E0', '8009424C', '80093C24', '80095B94', '80095B24', '80095AF4', '8001FF84', '8001FCC8']
body = '\n'.join(Path(f'tools/scripts/fixtures/legacy/func_{a}.c').read_text().replace('register int v0 asm("$2");', 'int v0;') for a in sources)
source = r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "rr/layout.h"
#undef RR_ASSERT_SIZE
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_SIZE(t,n)
#define RR_ASSERT_OFFSET(t,f,n)
''' + body + r'''
static u32 ordering, *expected_packet;
static unsigned calls;
static unsigned char expected[20];
static size_t packet_size;
void func_80095A44(u32 *head,u32 *packet) {
 assert(head==&ordering && packet==expected_packet);
 assert((uintptr_t)head>UINT32_MAX && (uintptr_t)packet>UINT32_MAX);
 assert(memcmp(packet,expected,packet_size)==0);
 u32 tag=packet[0];packet[0]=(tag&0xFF000000)|(ordering&0xFFFFFF);
 ordering=(ordering&0xFF000000)|((uintptr_t)packet&0xFFFFFF);
 calls++;
}
int main(void) {
 assert(sizeof(void*)==8 && sizeof(struct P)==20 && sizeof(T)==12);
 for(unsigned n=0;n<65536;n++)for(unsigned k=0;k<4;k++) {
  struct P packets[2],want;struct R rect={(u8)n,(u8)(n>>8),(u8)(n*3),(u8)(n*7)};
  memset(packets,0xA6,sizeof(packets));want=packets[0];
  int flags=k==0?0:k==1?1:k==2?(int)n*2:-(int)n*2-1;
  int intensity=flags>>1;
  want.link=(want.link&0xFFFFFF)|0x04000000;
  want.code=0x64|(intensity==0?1:0)|(flags&1?2:0);
  if(intensity)want.r=want.g=want.b=(u8)intensity;
  want.x=(s16)(u16)n;want.y=(s16)(u16)(n^0x8000);want.u=rect.u;want.v=rect.v;want.w=rect.w;want.h=rect.h;want.clut=n;
  memcpy(expected,&want,20);packet_size=20;expected_packet=(u32*)packets;ordering=0xAB123456;
  assert(func_8001FF84(&ordering,packets,(int)n,(int)(n^0x8000),&rect,n,flags)==packets+1);
  want.link=(want.link&0xFF000000)|0x123456;
  assert(memcmp(packets,&want,20)==0);
  for(unsigned i=20;i<sizeof(packets);i++)assert(((u8*)packets)[i]==0xA6);
  assert(ordering==(0xAB000000|((uintptr_t)packets&0xFFFFFF)));
  T pages[2],page;memset(pages,0xB7,sizeof(pages));page=pages[0];
  page.a=(page.a&0xFFFFFF)|0x02000000;page.b=0xE1000200U|(n&0x9FF);page.c=0;
  memcpy(expected,&page,12);packet_size=12;expected_packet=(u32*)pages;ordering=0xCD654321;
  assert(func_8001FCC8(&ordering,pages,n)==pages+1);
  page.a=(page.a&0xFF000000)|0x654321;assert(memcmp(pages,&page,12)==0);
  for(unsigned i=12;i<sizeof(pages);i++)assert(((u8*)pages)[i]==0xB7);
  assert(ordering==(0xCD000000|((uintptr_t)pages&0xFFFFFF)));
 }
 assert(calls==524288);return 0;
}
'''
with TemporaryDirectory() as directory:
    c=Path(directory)/'test.c';exe=Path(directory)/'test';c.write_text(source)
    subprocess.run(['cc','-std=gnu11','-O2','-funsigned-char','-fsanitize=undefined','-fno-sanitize-recover=all','-Iinclude',str(c),'-o',str(exe)],check=True)
    subprocess.run([str(exe)],check=True)
print('262144 paired sprite/page cases passed with full native pointers')
