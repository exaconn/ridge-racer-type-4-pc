"""Actual tiled GPU renderer versus ordered byte operations, including large and aliased inputs."""
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

ROOT=Path(__file__).resolve().parents[2]

class TiledRectangleTest(unittest.TestCase):
    def test_native_tiles(self):
        def header(name):
            return re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',
                          (ROOT/'include/rr'/name).read_text(),flags=re.M)
        body=(ROOT/'tools/scripts/fixtures/views/func_80019D5C.c').read_text()
        body=re.sub(r'^#include[^\n]*\n','',body,flags=re.M)
        body=re.sub(r' asm\("\$\d+"\)','',body)
        body=re.sub(r'__asm__\("".*?\);',';',body)
        assert body.count('*(u8 **)0x1F800000U')==1
        assert body.count('(RrGpuScratchPointers *)0x1F800000U')==1
        body=body.replace('*(u8 **)0x1F800000U','actual->scratch.cursor')
        body=body.replace('(RrGpuScratchPointers *)0x1F800000U','&actual->scratch')
        code=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
'''+''.join(header(n) for n in ['gpu_scratch_types.h','gpu.h','animated_sprite_grid.h','tiled_rectangle.h'])+r'''
enum {MAX_TILES=129*129,WORDS=1024+MAX_TILES*6+16};
typedef struct {
 u8 guard0[32];RrGpuScratchPointers scratch;RrGpuRectangleFields rectangle;
 u32 wire[WORDS];u8 guard1[32];
} World;
static World *actual,*expected;
'''+body+r'''
static s16 signed16(u16 value){s16 n;memcpy(&n,&value,2);return n;}
static u8 *mapped(const void *pointer,size_t size) {
 uintptr_t offset=(uintptr_t)pointer-(uintptr_t)actual;
 assert(offset<=sizeof(*actual)-size);return (u8 *)expected+offset;
}
static u32 read_value(const void *pointer,int size) {
 u8 *p=mapped(pointer,size);u32 n=0;
 for(int i=0;i<size;i++)n|=(u32)p[i]<<(8*i);return n;
}
static void write_value(void *pointer,u32 n,int size) {
 u8 *p=mapped(pointer,size);for(int i=0;i<size;i++)p[i]=n>>(8*i);
}
static s32 field(const void *rectangle,int offset){return signed16(read_value((const u8 *)rectangle+offset,2));}
static void link(void *packet,u32 *head,int length) {
 write_value(packet,read_value(head,4),4);write_value((u8 *)packet+3,length,1);
 write_value(head,(u32)(uintptr_t)packet,4);write_value((u8 *)head+3,0,1);
}
static void reference(RrGpuRectangleFields *rectangle,s32 intensity,u32 *head) {
 u8 *packet=expected->scratch.cursor;
 u32 shade=intensity<0?0:intensity>255?255:(u32)intensity;
 u32 color=0x66000000U|shade*0x010101U;
 write_value(packet+4,0xE6000000U,4);link(packet,head,1);
 int remaining_y=field(rectangle,6),y=field(rectangle,2);packet+=8;
 unsigned count=0;
 while(remaining_y>0) {
  int texture_y=y&255,rows=256-texture_y;if(rows>remaining_y)rows=remaining_y;
  int remaining_x=field(rectangle,4),x=field(rectangle,0);
  while(remaining_x>0) {
   int texture_x=x&63,columns=256-texture_x;if(columns>remaining_x)columns=remaining_x;
   u32 position=(((u32)y-1U)<<16)|((u32)x&65535U);
   u32 size=((u32)rows<<16)|(u32)columns;
   u32 uv=((u32)texture_y<<8)|(u32)texture_x;
   u32 mode=0xE1000300U|((u32)(y&256)>>4)|((u32)(x&1023)>>6);
   write_value(packet+12,position,4);write_value(packet+20,size,4);
   write_value(packet+8,color,4);write_value(packet+16,uv,4);write_value(packet+4,mode,4);
   link(packet,head,5);packet+=24;x+=columns;remaining_x-=columns;
   assert(++count<=MAX_TILES);
  }
  remaining_y-=rows;y+=rows;
 }
 write_value(packet+4,0xE6000001U,4);
 /* The pointer slot is read again after each insertion store. */
 write_value(packet,read_value(expected->scratch.order+703,4),4);
 write_value(packet+3,1,1);
 write_value(expected->scratch.order+703,(u32)(uintptr_t)packet,4);
 write_value((u8 *)(expected->scratch.order+703)+3,0,1);
 expected->scratch.cursor=packet+8;
}
static unsigned tile_count(s32 x,s32 y,s32 width,s32 height) {
 unsigned result=0;
 while(height>0) {
  int rows=256-(y&255);if(rows>height)rows=height;
  int w=width,at=x;
  while(w>0){int n=256-(at&63);if(n>w)n=w;result++;w-=n;at+=n;}
  height-=rows;y+=rows;
 }
 return result;
}
int main(void) {
 actual=malloc(sizeof(*actual));expected=malloc(sizeof(*expected));assert(actual&&expected);
 static const s16 coords[]={-32768,-1,0,63,64,255,256,32767};
 static const s16 widths[]={-32768,-1,0,1,63,64,193,255,256,257,32767};
 static const s16 heights[]={-32768,-1,0,1,2,64,193,255,256,257,32767};
 static const s32 intensities[]={INT32_MIN,-1,0,1,127,255,256,INT32_MAX};
 for(unsigned n=0;n<4160;n++) {
  memset(actual,(u8)(n*17),sizeof(*actual));
  u8 *begin=(u8 *)(actual->wire+1024);
  actual->scratch.cursor=begin;actual->scratch.order=actual->wire;
  actual->rectangle.x=coords[(n/121)%8];actual->rectangle.y=coords[(n/97)%8];
  actual->rectangle.w=widths[n%11];actual->rectangle.h=heights[(n/11)%11];
  if(n>=4096){actual->rectangle.w=actual->rectangle.h=32767;actual->rectangle.x=coords[(n-4096)%8];actual->rectangle.y=coords[(n-4096)/8];}
  u32 *head=actual->wire+900;unsigned kind=n%12;
  if(kind==1)head=(u32 *)begin;
  if(kind==2)head=(u32 *)(begin+4);
  if(kind==3)head=(u32 *)(begin+16);
  if(kind==4)head=(u32 *)(begin+28);
  if(kind==5)actual->scratch.order=(u32 *)begin-703;
  if(kind==6)actual->scratch.order=(u32 *)(begin+4)-703;
  if(kind==7)actual->scratch.order=head-703;
  if(kind==8)actual->scratch.order=(u32 *)(begin+16)-703;
  if(kind==9) {
   unsigned count=tile_count(actual->rectangle.x,actual->rectangle.y,actual->rectangle.w,actual->rectangle.h);
   actual->scratch.order=(u32 *)(begin+8+count*24)-703;
  }
  RrGpuRectangleFields *rectangle=&actual->rectangle;
  if(n<4096 && n%17==0) {
   /* Includes halfword-aligned descriptors and descriptors changed by output.
    * Height/Y are captured once; width/X are read again for each tile row. */
   static const int offsets[]={0,2,8,10,32,34};
   rectangle=(RrGpuRectangleFields *)(begin+offsets[(n/17)%6]);
   rectangle->x=-1;rectangle->y=250;rectangle->w=63;rectangle->h=300;
  }
  s32 intensity=intensities[(n/31)%8];
  memcpy(expected,actual,sizeof(*actual));reference(rectangle,intensity,head);
  func_80019D5C(rectangle,intensity,head);
  if(memcmp(actual,expected,sizeof(*actual))) {fprintf(stderr,"tile case %u failed\n",n);abort();}
 }
 free(actual);free(expected);puts("4160 native tiled-rectangle cases passed with UBSan");return 0;
}
'''
        with TemporaryDirectory() as directory:
            source=Path(directory)/'tiles.c';binary=Path(directory)/'tiles';source.write_text(code)
            subprocess.run(['cc','-std=gnu99','-O2','-fsanitize=undefined',
                            '-fno-sanitize-recover=all','-I',str(ROOT/'include'),
                            str(source),'-o',str(binary)],check=True)
            subprocess.run([str(binary)],check=True)

if __name__=='__main__': unittest.main()
