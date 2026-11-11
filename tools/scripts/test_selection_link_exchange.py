"""Real link exchange with full native selection buffers and controlled BIOS I/O.

Independent models check both peer roles, live handles, short transfers,
overlapping buffers, CPU mirrors and complete owner/guard/ABI preservation.
The production function is literal; only its external file services are modeled.
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
STACK,STOP=0x803F0000,0x803FF000
PREFIX=r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "game/data/selection.h"
#include "game/race/raceai2.h"
#include "rr/bios_events.h"
static struct {u32 before[4];CurrentSelection rows;u16 role;s32 read,write;u32 after[4];} world;
typedef struct {s32 kind,descriptor,offset,count,result;u8 before[32];} Event;
_Static_assert(sizeof(Event)==52,"explicit event wire layout");
static Event events[2];static unsigned calls,seed;static s32 returns[2];
static void *pointers[2];
#define LinkHostFlag world.role
#define LinkReadHandle world.read
#define LinkWriteHandle world.write
static s32 service(s32 kind,s32 descriptor,const void *p,s32 count){
 unsigned ordinal=calls++;assert(ordinal<2&&p==pointers[ordinal]);
 assert((uintptr_t)p>UINT32_MAX&&count>=0&&count<=8);
 size_t offset=(const u8*)p-world.rows.bytes;assert(offset+(size_t)count<=32);
 Event *e=&events[ordinal];e->kind=kind;e->descriptor=descriptor;e->offset=(s32)offset;e->count=count;e->result=returns[ordinal];
 memcpy(e->before,world.rows.bytes,32);
 if(kind==0){s32 written=e->result<0?0:e->result>count?count:e->result;
  for(s32 i=0;i<written;i++)world.rows.bytes[offset+i]=(u8)(seed+ordinal*17U+i*13U);
 }
 world.role^=0x8000U;world.read+=11;world.write-=7;return e->result;
}
s32 FileRead(s32 descriptor,void *p,s32 count){return service(0,descriptor,p,count);}
s32 FileWrite(s32 descriptor,const void *p,s32 count){return service(1,descriptor,p,count);}
'''
TAIL=r'''
void link_case(const u8 *input,u32 number,u32 role,s32 first,s32 second,s32 count,s32 dual,
               const s32 *results,u8 *output,s32 *metadata,u8 *trace){
 assert(first>=0&&second>=0&&count>=0&&first+count<=32&&second+count<=32);
 memset(&world,0xA5,sizeof(world));memcpy(world.rows.bytes,input,32);world.role=(u16)role;world.read=33;world.write=47;
 calls=0;seed=number;returns[0]=results[0];returns[1]=results[1];memset(events,0,sizeof(events));
 pointers[0]=world.rows.bytes+second;pointers[1]=world.rows.bytes+first;
 metadata[0]=LinkExchange(pointers[1],pointers[0],count,dual);metadata[1]=calls;metadata[2]=world.role;metadata[3]=world.read;metadata[4]=world.write;
 memcpy(output,world.rows.bytes,32);memcpy(trace,events,sizeof(events));
 for(unsigned i=0;i<4;i++)assert(world.before[i]==0xA5A5A5A5U&&world.after[i]==0xA5A5A5A5U);
}
'''

def signed(n):return n-0x100000000 if n&0x80000000 else n

def oracle(initial,number,role,first,second,count,dual,results):
 data=bytearray(initial);read,write=33,47;events=[]
 for ordinal in range(2):
  if ordinal and (results[0]!=count or not dual):break
  kind=(1 if role else 0)if ordinal==0 else(0 if role else 1)
  offset=second if ordinal==0 else first;result=results[ordinal]
  events.append(struct.pack('<5i',kind,write if kind else read,offset,count,result)+bytes(data))
  if not kind:
   for i in range(max(0,min(count,result))):data[offset+i]=(number+ordinal*17+i*13)&255
  read+=11;write-=7
 status=0 if all(results[i]==count for i in range(len(events)))else-1
 return bytes(data),(status,len(events),role^(0x8000 if len(events)&1 else 0),read,write),b''.join(events)+bytes((2-len(events))*52)


class SelectionLinkExchangeTest(unittest.TestCase):
 @classmethod
 def setUpClass(cls):
  cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup);directory=Path(cls.temp.name)
  owners=source_owners(ROOT/'src/main/PAL');body=definition_source(owners['LinkExchange'].read_text(),'LinkExchange');source=PREFIX+body+TAIL
  assert source.count(body)==1 and not re.search(r'\b(?:asm|__asm__)\s*\(',body)
  c=directory/'link.c';c.write_text(source);library=c.with_suffix('.so')
  built=subprocess.run(['cc','-std=c11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror','-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(c),'-o',str(library)],capture_output=True,text=True)
  if built.returncode:raise AssertionError(built.stderr)
  cls.lib=C.CDLL(str(library));cls.lib.link_case.restype=None;cls.lib.link_case.argtypes=[C.c_char_p,C.c_uint32,C.c_uint32,*([C.c_int32]*4),C.POINTER(C.c_int32),C.c_void_p,C.POINTER(C.c_int32),C.c_void_p]
  cls.original=(ROOT/'assets/PAL/main.exe').read_bytes()[0x800:];elf=ELFFile(io.BytesIO((ROOT/'build/PAL/main.elf').read_bytes()))
  cls.linked=[(s['sh_addr'],s.data())for s in elf.iter_sections()if s['sh_flags']&2 and s['sh_type']=='SHT_PROGBITS']
 def write(self,a,data):self.u.mem_write(a&0x1FFFFFFF,bytes(data))
 def read(self,a,n):return bytes(self.u.mem_read(a&0x1FFFFFFF,n))
 def setup_cpu(self,linked):
  self.u=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);self.u.mem_map(0,0x400000);self.write(0x80010000,self.original)
  if linked:
   for a,data in self.linked:self.write(a,data)
  def store(cpu,access,a,n,v,user):
   a&=0x1FFFFFFF;self.assertTrue((STACK-1024)&0x1FFFFFFF<=a and a+n<=(STACK+64)&0x1FFFFFFF,(hex(a),n))
  self.cpu=Cpu(self.u,0,100,101,102,store)
 def execute(self,initial,number,role,first,second,count,dual,results):
  base=address_of('SelectionRows');cpu=self.cpu;cpu.reset();cpu.pc=address_of('LinkExchange');cpu.r[29]=STACK;cpu.r[31]=STOP
  saved={r:0xA18C0000+r for r in(*range(16,24),28,30)}
  for r,v in saved.items():cpu.r[r]=v
  pointers=[base+second+(0x20000000 if number&8 else 0),base+first+(0x20000000 if number&4 else 0)]
  for i,v in enumerate([pointers[1],pointers[0],count,dual]):cpu.r[4+i]=v&0xFFFFFFFF
  self.write(base-16,bytes([0xA5])*16+initial+bytes([0xA5])*16)
  self.write(address_of('LinkHostFlag'),struct.pack('<H',role));self.write(address_of('LinkReadHandle'),struct.pack('<i',33));self.write(address_of('LinkWriteHandle'),struct.pack('<i',47));events=[]
  hooks={address_of('FileRead'):0,address_of('FileWrite'):1}
  for _ in range(2000):
   if cpu.pc==STOP:
    self.assertEqual(cpu.r[29],STACK);self.assertTrue(all(cpu.r[r]==v for r,v in saved.items()))
    self.assertEqual(self.read(base-16,16),bytes([0xA5])*16);self.assertEqual(self.read(base+32,16),bytes([0xA5])*16)
    meta=(signed(cpu.r[2]),len(events),struct.unpack('<H',self.read(address_of('LinkHostFlag'),2))[0],struct.unpack('<i',self.read(address_of('LinkReadHandle'),4))[0],struct.unpack('<i',self.read(address_of('LinkWriteHandle'),4))[0])
    return self.read(base,32),meta,b''.join(events)+bytes((2-len(events))*52)
   if cpu.pc in hooks:
    self.assertIsNone(cpu.delay);ordinal=len(events);self.assertLess(ordinal,2);kind=hooks[cpu.pc];self.assertEqual(cpu.r[5],pointers[ordinal]);self.assertEqual(cpu.r[6],count)
    offset=(cpu.r[5]&0x1FFFFFFF)-(base&0x1FFFFFFF);result=results[ordinal]
    events.append(struct.pack('<5i',kind,signed(cpu.r[4]),offset,count,result)+self.read(base,32))
    if not kind:
     size=max(0,min(count,result));self.write(base+offset,bytes((number+ordinal*17+i*13)&255 for i in range(size)))
    flag=struct.unpack('<H',self.read(address_of('LinkHostFlag'),2))[0]^0x8000;self.write(address_of('LinkHostFlag'),struct.pack('<H',flag))
    for name,delta in [('LinkReadHandle',11),('LinkWriteHandle',-7)]:
     v=struct.unpack('<i',self.read(address_of(name),4))[0]+delta;self.write(address_of(name),struct.pack('<i',v))
    returned=cpu.r[31]
    for r in(2,3,*range(4,16),24,25):cpu.r[r]=0xD15C0000+r
    cpu.r[2]=result&0xFFFFFFFF;cpu.pc=returned;continue
   cpu.step()
  self.fail('MIPS instruction limit')
 def test_peer_roles_aliases_live_handles_and_short_transfers(self):
  for linked in(False,True):
   self.setup_cpu(linked);rng=random.Random(0x11A1E7)
   for case in range(512):
    role=(0,1,65535,32768,32767)[case%5];dual=(0,1,-1,-2147483648,2147483647)[(case//5)%5];count=(0,1,2,4,8)[(case//25)%5]
    first=(case*5)%(33-count);second=(case*13)%(33-count);options=(count,0,-1,count-1,count+1);results=(options[(case//7)%5],options[(case//11)%5]);initial=rng.randbytes(32)
    expected=oracle(initial,case,role,first,second,count,dual,results)
    output=C.create_string_buffer(32);meta=(C.c_int32*5)();trace=C.create_string_buffer(104)
    self.lib.link_case(initial,case,role,first,second,count,dual,(C.c_int32*2)(*results),output,meta,trace)
    self.assertEqual((output.raw,tuple(meta),trace.raw),expected)
    self.assertEqual(self.execute(initial,case,role,first,second,count,dual,results),expected)
