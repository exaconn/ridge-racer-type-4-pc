"""Literal selection codec and consumer on the complete shared owner.

Native C, original MIPS and linked MIPS run the same producer/consumer chain.
Independent models check all eight records, signed glyph boundaries, both CPU
aliases, preserved option bytes, surrounding guards and register/stack ABI.
No services or production bodies are replaced.
"""
import ctypes as C
import io
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
NAMES=('UnpackChoiceGlyph','PackChoiceGlyph','GetPlayerCarVariantIndex')
STACK,STOP=0x803F0000,0x803FF000
PREFIX=r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "game/data/selection.h"
#include "game/data/trkld5.h"
#include "game/menu/mnchoice.h"
static struct {u8 before[16];CurrentSelection rows;u8 after[16];} world;
_Static_assert(sizeof(CurrentSelection)==32,"complete shared selection owner");
_Static_assert(_Alignof(CurrentSelection)==1,"byte-aligned records");
_Static_assert(sizeof(world)==64,"owner and both guards");
_Static_assert(offsetof(CurrentSelection,entries[7].option)==31,"last record byte");
_Static_assert(offsetof(CurrentSelection,selection.rows[0])==4,"menu list begins at record one");
#define SelectionRows world.rows
'''
TAIL=r'''
void owner_case(const u8 *input,s32 slot,s32 glyph,u8 *output,s32 *values){
 assert(slot>=0&&slot<8);memset(&world,0xA5,sizeof(world));memcpy(world.rows.bytes,input,32);
 assert(sizeof(void*)==8&&(uintptr_t)&world.rows>UINT32_MAX);
 UnpackChoiceGlyph((s8*)&world.rows.records[slot],glyph);
 values[0]=PackChoiceGlyph(&world.rows.records[slot]);
 values[1]=GetPlayerCarVariantIndex(slot);
 memcpy(output,world.rows.bytes,32);
 for(unsigned i=0;i<16;i++)assert(world.before[i]==0xA5&&world.after[i]==0xA5);
}
'''

def quotient(n,d):return -(abs(n)//d)if n<0 else n//d

def reference(initial,slot,glyph):
 result=bytearray(initial);team=quotient(glyph,160);maker=quotient(glyph,40)-team*4
 parameter=glyph-quotient(glyph,40)*40
 result[slot*4:slot*4+3]=bytes((team&255,maker&255,parameter&255))
 team,maker,parameter=result[slot*4:slot*4+3]
 packed=team*160+maker*40+parameter
 variant=(8 if team==2 and maker==0 else 0)if parameter==39 else maker+(4 if parameter==19 else 0)
 return bytes(result),(packed,variant)


class SelectionOwnerPipelineTest(unittest.TestCase):
 @classmethod
 def setUpClass(cls):
  cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup);directory=Path(cls.temp.name)
  owners=source_owners(ROOT/'src/main/PAL');bodies=[definition_source(owners[n].read_text(),n)for n in NAMES]
  source=PREFIX+'\n'.join(bodies)+TAIL
  assert all(source.count(body)==1 and not re.search(r'\b(?:asm|__asm__)\s*\(',body)for body in bodies)
  c=directory/'selection.c';c.write_text(source);library=c.with_suffix('.so')
  built=subprocess.run(['cc','-std=c11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror','-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(c),'-o',str(library)],capture_output=True,text=True)
  if built.returncode:raise AssertionError(built.stderr)
  cls.lib=C.CDLL(str(library));cls.lib.owner_case.restype=None;cls.lib.owner_case.argtypes=[C.c_char_p,C.c_int32,C.c_int32,C.c_void_p,C.POINTER(C.c_int32)]
  cls.original=(ROOT/'assets/PAL/main.exe').read_bytes()[0x800:]
  elf=ELFFile(io.BytesIO((ROOT/'build/PAL/main.elf').read_bytes()))
  cls.linked=[(s['sh_addr'],s.data())for s in elf.iter_sections()if s['sh_flags']&2 and s['sh_type']=='SHT_PROGBITS']
 def write(self,address,data):self.u.mem_write(address&0x1FFFFFFF,bytes(data))
 def read(self,address,size):return bytes(self.u.mem_read(address&0x1FFFFFFF,size))
 def setup_cpu(self,linked):
  self.u=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);self.u.mem_map(0,0x400000);self.write(0x80010000,self.original)
  if linked:
   for a,data in self.linked:self.write(a,data)
  base=address_of('SelectionRows');ranges=[(base,32),(STACK-1024,1088)]
  def store(cpu,access,address,size,value,user):
   address&=0x1FFFFFFF
   self.assertTrue(any((a&0x1FFFFFFF)<=address and address+size<=(a&0x1FFFFFFF)+n for a,n in ranges),(hex(address),size))
  self.cpu=Cpu(self.u,0,100,101,102,store)
 def invoke(self,name,args):
  cpu=self.cpu;cpu.reset();cpu.pc=address_of(name);cpu.r[29]=STACK;cpu.r[31]=STOP
  saved={r:0xA8710000+r for r in(*range(16,24),28,30)}
  for r,v in saved.items():cpu.r[r]=v
  for i,v in enumerate(args):cpu.r[4+i]=v&0xFFFFFFFF
  for _ in range(1000):
   if cpu.pc==STOP:
    self.assertEqual(cpu.r[29],STACK);self.assertTrue(all(cpu.r[r]==v for r,v in saved.items()));return cpu.r[2]
   cpu.step()
  self.fail('MIPS instruction limit')
 def test_all_records_codec_options_and_consumer_native_original_linked(self):
  edges=(-2147483648,-2147483647,-65537,-32768,-161,-160,-40,-1,0,19,39,40,159,160,319,320,359,639,640,32767,65536,2147483647)
  for linked in(False,True):
   self.setup_cpu(linked);rng=random.Random(0x5E1EC710);base=address_of('SelectionRows')
   for case in range(1024):
    slot=case%8;glyph=edges[(case//8)%len(edges)]if case<8*len(edges)else rng.randrange(-2147483648,2147483648)
    initial=rng.randbytes(32);expected,values=reference(initial,slot,glyph)
    native=C.create_string_buffer(32);result=(C.c_int32*2)();self.lib.owner_case(initial,slot,glyph,native,result)
    self.assertEqual(native.raw,expected);self.assertEqual(tuple(result),values)
    self.write(base-16,bytes([0xA5])*16+initial+bytes([0xA5])*16)
    record=base+slot*4+(0x20000000 if case&8 else 0)
    self.invoke('UnpackChoiceGlyph',[record,glyph]);packed=self.invoke('PackChoiceGlyph',[record]);variant=self.invoke('GetPlayerCarVariantIndex',[slot])
    self.assertEqual((packed,variant),values);self.assertEqual(self.read(base-16,64),bytes([0xA5])*16+expected+bytes([0xA5])*16)
