"""Literal tile builders and SDK setters with real native DMA addresses.

Independent wire models check complete guarded packet arenas, all ordering-head
aliases, full-word arguments, signed grid division/wrapping, both CPU mirrors
and returned full pointers. All native and original/linked MIPS helpers execute
their actual bodies; no service or production body is replaced.
"""
import ctypes as C
from pathlib import Path
import random
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu

ROOT=Path(__file__).resolve().parents[2]
BUFFERS=(0x80180010,0x80190010);HEADS=0x801A0010;HEADER=0x801B0010
OWNER=288;SIZE=2*OWNER+48
STACK,STOP=0x803F0000,0x803FF000
TILES=('AddTilePrim','AddTileSemiTransPrim','AddTileGrid2x2')
SETTERS=('SetPolyF3','SetPolyFT3','SetSprt','SetTile','SetLineF2','SetLineG2','SetSemiTrans','SetShadeTex')
PREFIX=r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "rr/gpu_address.h"
#include "rr/gpu_line.h"
#include "rr/animated_glyph.h"
#include "game/gfx/sprefct.h"
#include "game/ui/badge1.h"
#include "psyq/libgpu/primatt2.h"
#include "psyq/libgpu/primatt3.h"
#include "psyq/libgpu/primatt5.h"
#include "psyq/libgpu/primatt6.h"
#include "psyq/libgpu/primatt7.h"
#include "psyq/libgpu/primattr.h"
#include "psyq/libgpu/settrans.h"
typedef union {GpuTileCommand tiles[16];u32 words[64];u8 bytes[256];} TileArena;
typedef struct {u32 before[4];TileArena value;u32 after[4];} TileOwner;
typedef struct {u32 before[4],words[4],after[4];} HeadOwner;
typedef struct {TileOwner buffers[2];HeadOwner heads;} TileWorld;
typedef union {GpuPrimitiveHeader header;GpuPolyFT3Packet triangle;
 GpuSpriteCommand sprite;GpuTileCommand tile;GpuLineF2Packet line;u32 words[8];} Primitive;
typedef struct {u32 before[4];Primitive value;u32 after[4];} PrimitiveOwner;
static TileWorld world;static PrimitiveOwner primitive;
_Static_assert(sizeof(TileWorld)==624,"complete packet and head owners");
_Static_assert(sizeof(PrimitiveOwner)==64,"complete primitive storage and guards");
_Static_assert(sizeof(GpuPrimitiveHeader)==8,"shared packet header");
_Static_assert(offsetof(GpuPrimitiveHeader,color.bytes.code)==7,"command byte");
'''
TAIL=r'''
void tile_case(u32 operation,u32 aliases,u32 hb,u32 hi,u32 pb,u32 pi,
               const u8 *input,const s32 *args,u8 *output,u32 *returned){
 assert(operation<3&&hb<3&&pb<2&&hi<(hb==2?4:64)&&pi+(operation==2?4:1)<=16);
 memcpy(&world,input,sizeof(world));
 static GpuDmaRegion map[3];
 for(unsigned i=0;i<2;i++)map[i]=(GpuDmaRegion){world.buffers[i].value.bytes,
  0x80180010+i*0x10000+((aliases&(1U<<i))?0x20000000:0),256};
 map[2]=(GpuDmaRegion){world.heads.words,0x801A0010,16};assert(GpuDmaSetRegions(map,3));
 u32 *head=hb==2?&world.heads.words[hi]:&world.buffers[hb].value.words[hi];
 GpuTileCommand *start=&world.buffers[pb].value.tiles[pi];
 assert((uintptr_t)head>UINT32_MAX&&(uintptr_t)start>UINT32_MAX);
 void *result;
 if(operation==0)result=AddTilePrim(head,start,args[0],args[1],args[2],args[3],args[4],args[5],args[6]);
 else if(operation==1)result=AddTileSemiTransPrim(head,start,args[0],args[1],args[2],args[3],args[4],args[5],args[6]);
 else result=AddTileGrid2x2(head,start,args[0],args[1],args[2],args[3],args[4],args[5],args[6]);
 assert(result==&world.buffers[pb].value.tiles[pi+(operation==2?4:1)]);
 /* Return values may be one past the arena. This is pointer transport, not a
  * DMA read of an inaccessible byte; the complete pointer is checked above. */
 *returned=map[pb].address+(u32)((u8 *)result-world.buffers[pb].value.bytes);
 memcpy(output,&world,sizeof(world));
}
void primitive_case(u32 operation,s32 flag,const u8 *input,u8 *output){
 assert(operation<8);memcpy(&primitive,input,sizeof(primitive));
 assert((uintptr_t)&primitive>UINT32_MAX);
 switch(operation){
 case 0:SetPolyF3(&primitive.value);break;
 case 1:SetPolyFT3(&primitive.value);break;
 case 2:SetSprt(&primitive.value);break;
 case 3:SetTile(&primitive.value);break;
 case 4:SetLineF2(&primitive.value);break;
 case 5:SetLineG2(&primitive.value);break;
 case 6:SetSemiTrans(&primitive.value,flag);break;
 case 7:SetShadeTex(&primitive.value,flag);break;
 }
 memcpy(output,&primitive,sizeof(primitive));
}
'''

def signed(v):
 v&=0xFFFFFFFF
 return v-0x100000000 if v&0x80000000 else v
def half(n):return -(abs(n)//2)if n<0 else n//2
def word(v):return struct.pack('<I',v&0xFFFFFFFF)

def reference(initial,operation,head_at,packet_at,packet_address,args):
 out=bytearray(initial);x,y,width,height,red,green,blue=args
 count=4 if operation==2 else 1
 for i in range(count):
  if operation==2:
   w,h=half(width),half(height);tx=signed(x+w*(i&1));ty=signed(y+h)if i>=2 else y
  else:w,h,tx,ty=width,height,x,y
  at=packet_at+i*16;out[at+3]=3
  struct.pack_into('<HHHH',out,at+8,tx&65535,ty&65535,w&65535,h&65535)
  out[at+4:at+8]=bytes((red&255,green&255,blue&255,0x62 if operation==1 else 0x60))
  get=lambda offset:struct.unpack_from('<I',out,offset)[0]
  struct.pack_into('<I',out,at,(get(at)&0xFF000000)|(get(head_at)&0xFFFFFF))
  struct.pack_into('<I',out,head_at,(get(head_at)&0xFF000000)|((packet_address+i*16)&0xFFFFFF))
 return out,packet_address+count*16

class TilePacketOwnerTest(unittest.TestCase):
 @classmethod
 def setUpClass(cls):
  cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup);d=Path(cls.temp.name)
  owners=source_owners(ROOT/'src/main/PAL');names=(*SETTERS,'AddPrim',*TILES)
  bodies=[definition_source(owners[n].read_text(),n)for n in names];source=PREFIX+'\n'.join(bodies)+TAIL
  assert all(source.count(body)==1 and not re.search(r'\b(?:asm|__asm__)\s*\(',body)for body in bodies)
  (d/'tiles.c').write_text(source)
  result=subprocess.run(['cc','-std=c11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror','-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(d/'tiles.c'),str(ROOT/'src/native/gpu_address.c'),'-o',str(d/'tiles.so')],capture_output=True,text=True)
  if result.returncode:raise AssertionError(result.stderr)
  cls.lib=C.CDLL(str(d/'tiles.so'));cls.lib.tile_case.argtypes=[C.c_uint32]*6+[C.c_char_p,C.POINTER(C.c_int32),C.c_void_p,C.POINTER(C.c_uint32)]
  cls.lib.primitive_case.argtypes=[C.c_uint32,C.c_int32,C.c_char_p,C.c_void_p]
  cls.original=(ROOT/'assets/PAL/main.exe').read_bytes()[0x800:]
  with(ROOT/'build/PAL/main.elf').open('rb')as stream:
   elf=ELFFile(stream);cls.linked=[(s['sh_addr'],s.data())for s in elf.iter_sections()if s['sh_flags']&2 and s['sh_type']=='SHT_PROGBITS']
 def write(self,a,data):self.u.mem_write(a&0x1FFFFFFF,bytes(data))
 def read(self,a,n):return bytes(self.u.mem_read(a&0x1FFFFFFF,n))
 def setup_cpu(self,linked):
  self.u=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);self.u.mem_map(0,0x400000);self.write(0x80010000,self.original)
  if linked:
   for a,data in self.linked:self.write(a,data)
  ranges=[(p,256)for p in BUFFERS]+[(HEADS,16),(HEADER,32),(STACK-1024,1088)]
  def store(cpu,access,a,n,v,user):
   a&=0x1FFFFFFF;self.assertTrue(any((p&0x1FFFFFFF)<=a and a+n<=(p&0x1FFFFFFF)+z for p,z in ranges),(hex(a),n))
  self.cpu=Cpu(self.u,0,100,101,102,store)
 def execute(self,name,args):
  cpu=self.cpu;cpu.reset();cpu.pc=address_of(name);cpu.r[29]=STACK;cpu.r[31]=STOP
  saved={r:0x9ABC0000+r for r in(*range(16,24),28,30)}
  for r,v in saved.items():cpu.r[r]=v
  for i,v in enumerate(args):
   if i<4:cpu.r[4+i]=v&0xFFFFFFFF
   else:self.write(STACK+i*4,word(v))
  for _ in range(2000):
   if cpu.pc==STOP:
    self.assertEqual(cpu.r[29],STACK);self.assertTrue(all(cpu.r[r]==v for r,v in saved.items()));return cpu.r[2]
   cpu.step()
  self.fail('MIPS instruction limit')
 def test_real_tile_streams_full_words_aliases_and_returned_pointers(self):
  edges=(-2147483648,-2147483647,-65537,-32768,-1,0,1,32767,65536,2147483647)
  for linked in(False,True):
   self.setup_cpu(linked);rng=random.Random(0x71AE0003)
   for case in range(768):
    operation=case%3;aliases=(case//3)%4;pb=(case//12)&1;pi=(0,4,12,15)[(case//24)%4]
    if operation==2 and pi==15:pi=12
    packet_at=pb*OWNER+16+pi*16;addresses=[a+(0x20000000 if aliases&(1<<b)else 0)for b,a in enumerate(BUFFERS)]
    if case&128:
     hb=pb;hi=pi*4+(case//8)%(16 if operation==2 else 4)
    else:hb=2;hi=(case//8)%4
    head_at=2*OWNER+16+hi*4 if hb==2 else hb*OWNER+16+hi*4
    head=HEADS+hi*4 if hb==2 else addresses[hb]+hi*4;packet=addresses[pb]+pi*16
    args=[edges[(case//4+i*3)%len(edges)]for i in range(7)]
    initial=bytearray(rng.randbytes(SIZE))
    for at,span in((0,OWNER),(OWNER,OWNER),(2*OWNER,48)):
     initial[at:at+16]=initial[at+span-16:at+span]=bytes([0xA5])*16
    expected,returned=reference(initial,operation,head_at,packet_at,packet,args)
    with self.subTest(linked=linked,case=case,operation=TILES[operation],head_alias=hb<2):
     output=C.create_string_buffer(SIZE);result=C.c_uint32();self.lib.tile_case(operation,aliases,hb,hi,pb,pi,bytes(initial),(C.c_int32*7)(*args),output,C.byref(result));self.assertEqual(expected,output.raw);self.assertEqual(returned,result.value)
     for b,p in enumerate(BUFFERS):self.write(p-16,initial[b*OWNER:(b+1)*OWNER])
     self.write(HEADS-16,initial[2*OWNER:]);actual_return=self.execute(TILES[operation],[head,packet,*args])
     actual=b''.join([*(self.read(p-16,OWNER)for p in BUFFERS),self.read(HEADS-16,48)])
     self.assertEqual(expected,actual);self.assertEqual(returned,actual_return)
 def test_shared_header_setters_preserve_complete_packet_storage(self):
  commands=((4,0x20),(7,0x24),(4,0x64),(3,0x60),(3,0x40),(4,0x50))
  flags=(-2147483648,-1,0,1,2,2147483647)
  for linked in(False,True):
   self.setup_cpu(linked);rng=random.Random(0x8EAD0040)
   for code in range(256):
    for operation,name in enumerate(SETTERS):
     flag=flags[(code+operation)%len(flags)];initial=bytearray(rng.randbytes(64));initial[:16]=initial[-16:]=bytes([0xA5])*16;initial[23]=code
     expected=bytearray(initial)
     if operation<6:expected[19],expected[23]=commands[operation]
     else:
      bit=2 if operation==6 else 1;expected[23]=(code|bit)if flag else(code&~bit)
     with self.subTest(linked=linked,code=code,setter=name,flag=flag):
      output=C.create_string_buffer(64);self.lib.primitive_case(operation,flag,bytes(initial),output);self.assertEqual(expected,output.raw)
      self.write(HEADER-16,initial);self.execute(name,[HEADER,flag]);self.assertEqual(expected,self.read(HEADER-16,64))
