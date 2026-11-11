"""Literal AddPrim C and original/linked MIPS on complete native tag owners.

The independent word oracle checks both link updates, length preservation,
self-alias and adjacent words, every slot and both CPU aliases. Native pointers
remain above 4 GiB; only the existing DMA backend serializes PSX addresses.
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
TAGS=(0x80190010,0x801A0010);STACK,STOP=0x803F0000,0x803FF000
PREFIX=r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "rr/gpu.h"
#include "rr/gpu_address.h"
#include "rr/animated_glyph.h"
typedef struct {u32 before[4],words[8],after[4];} TagOwner;
static TagOwner owners[2];
_Static_assert(sizeof(GpuDmaTag)==4,"complete wire tag");
_Static_assert(offsetof(GpuDmaTag,bytes.length)==3,"DMA length byte");
'''
TAIL=r'''
void run_case(u32 aliases,u32 hb,u32 hi,u32 pb,u32 pi,const u8 *input,u8 *output){
 assert(hb<2&&pb<2&&hi<8&&pi<8);memcpy(owners,input,sizeof(owners));
 assert((uintptr_t)owners>UINT32_MAX);
 static GpuDmaRegion map[2];
 for(unsigned i=0;i<2;i++)map[i]=(GpuDmaRegion){owners[i].words,
   0x80190010+i*0x10000+((aliases&(1U<<i))?0x20000000:0),sizeof(owners[i].words)};
 assert(GpuDmaSetRegions(map,2));
 assert(GpuDmaEncodeAddress(&owners[pb].words[pi])==map[pb].address+pi*4);
 AddPrim(&owners[hb].words[hi],&owners[pb].words[pi]);
 for(unsigned i=0;i<2;i++)for(unsigned j=0;j<4;j++)assert(owners[i].before[j]==0xA5A5A5A5&&owners[i].after[j]==0xA5A5A5A5);
 memcpy(output,owners,sizeof(owners));
}
'''

class DmaTagOwnerTest(unittest.TestCase):
 @classmethod
 def setUpClass(cls):
  cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup);d=Path(cls.temp.name)
  owner=source_owners(ROOT/'src/main/PAL')['AddPrim'];body=definition_source(owner.read_text(),'AddPrim')
  assert not re.search(r'\b(?:asm|__asm__)\s*\(',body)
  source=PREFIX+body+TAIL;assert source.count(body)==1;(d/'tag.c').write_text(source)
  result=subprocess.run(['cc','-std=c11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror','-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(d/'tag.c'),str(ROOT/'src/native/gpu_address.c'),'-o',str(d/'tag.so')],capture_output=True,text=True)
  if result.returncode:raise AssertionError(result.stderr)
  cls.lib=C.CDLL(str(d/'tag.so'));cls.lib.run_case.argtypes=[C.c_uint32]*5+[C.c_char_p,C.c_void_p];cls.lib.run_case.restype=None
  cls.original=(ROOT/'assets/PAL/main.exe').read_bytes()[0x800:]
  with(ROOT/'build/PAL/main.elf').open('rb')as stream:
   elf=ELFFile(stream);cls.linked=[(s['sh_addr'],s.data())for s in elf.iter_sections()if s['sh_flags']&2 and s['sh_type']=='SHT_PROGBITS']
 def write(self,a,data):self.u.mem_write(a&0x1FFFFFFF,bytes(data))
 def read(self,a,n):return bytes(self.u.mem_read(a&0x1FFFFFFF,n))
 def test_both_links_lengths_aliases_and_guarded_complete_owners(self):
  for linked in(False,True):
   self.u=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);self.u.mem_map(0,0x400000);self.write(0x80010000,self.original)
   if linked:
    for a,data in self.linked:self.write(a,data)
   def store(cpu,access,a,n,v,user):
    a&=0x1FFFFFFF;self.assertTrue(any((p&0x1FFFFFFF)<=a and a+n<=(p&0x1FFFFFFF)+32 for p in TAGS),(hex(a),n))
   cpu=Cpu(self.u,0,100,101,102,store);rng=random.Random(0x24ADD008)
   for aliases in range(4):
    addresses=[p+(0x20000000 if aliases&(1<<b)else 0)for b,p in enumerate(TAGS)]
    for head in range(16):
     for packet in range(16):
      hb,hi=divmod(head,8);pb,pi=divmod(packet,8)
      initial=bytearray(rng.randbytes(128))
      for b in range(2):initial[b*64:b*64+16]=initial[b*64+48:b*64+64]=bytes([0xA5])*16
      expected=bytearray(initial);ha=hb*64+16+hi*4;pa=pb*64+16+pi*4
      get=lambda at:struct.unpack_from('<I',expected,at)[0]
      struct.pack_into('<I',expected,pa,(get(pa)&0xFF000000)|(get(ha)&0xFFFFFF))
      struct.pack_into('<I',expected,ha,(get(ha)&0xFF000000)|((addresses[pb]+pi*4)&0xFFFFFF))
      with self.subTest(linked=linked,aliases=aliases,head=head,packet=packet):
       out=C.create_string_buffer(128);self.lib.run_case(aliases,hb,hi,pb,pi,bytes(initial),out);self.assertEqual(expected,out.raw)
       for b,p in enumerate(TAGS):self.write(p-16,initial[b*64:(b+1)*64])
       cpu.reset();cpu.pc=address_of('AddPrim');cpu.r[4]=addresses[hb]+hi*4;cpu.r[5]=addresses[pb]+pi*4;cpu.r[29]=STACK;cpu.r[31]=STOP
       saved={r:0x9ABC0000+r for r in(*range(16,24),28,30)}
       for r,v in saved.items():cpu.r[r]=v
       for _ in range(100):
        if cpu.pc==STOP:break
        cpu.step()
       self.assertEqual(cpu.pc,STOP);self.assertEqual(cpu.r[29],STACK);self.assertTrue(all(cpu.r[r]==v for r,v in saved.items()))
       actual=b''.join(self.read(p-16,64)for p in TAGS);self.assertEqual(expected,actual)
