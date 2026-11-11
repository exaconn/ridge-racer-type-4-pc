"""Results label renderer with real native owners and original/linked MIPS.

One layer controls the two drawing services to test caller liveness. The other
includes the actual sprite/page builders and SDK setters without rewriting any
production body. Original/linked MIPS and native C execute the real AddPrim.
A test symbol binding enables post-link observation without changing its body.
DMA links use the registered-address backend, never host low bits.
Independent models check complete frames, packet storage, tables and pointers.
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
FRAMES=(0x80180010,0x801C0010);FRAME_SIZE=0x22778;FRAME_OWNER=FRAME_SIZE+32
ARENA=0x80280010;ARENA_SIZE=4096;ARENA_OWNER=ARENA_SIZE+32
STACK,STOP=0x803F0000,0x803FF000
SIZE=4+96+16+2*FRAME_OWNER+ARENA_OWNER+12+12
FUNCTION='DrawResultsStandingsOverlay'
DEPENDENCIES=('get_mode','get_tw','SetDrawMode','SetSprt','SetShadeTex',
              'SetSemiTrans','AddSpritePrim','AddDrawModePrim')
PREFIX=r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "game/race/result4.h"
#include "game/ui/badgeprim.h"
#include "game/ui/badge1.h"
#include "rr/frame_storage_types.h"
#include "rr/gpu_scratch_types.h"
#include "rr/gpu_address.h"
#include "psyq/libgpu/gpudisp.h"
#include "psyq/libgpu/primattr.h"
#include "rr/animated_glyph.h"
#include "psyq/libgpu/settrans.h"
typedef struct {u8 before[16];UiUvRect value[24];u8 after[16];} LabelOwner;
typedef struct {u8 before[16];u8 value[16];u8 after[16];} IndexOwner;
typedef struct {u8 before[16];FrameStorage value;u8 after[16];} FrameOwner;
typedef struct {u8 before[16];u8 value[4096];u8 after[16];} PacketOwner;
static LabelOwner labels;static IndexOwner indices;static FrameOwner frames[2];
static PacketOwner packets;static GpuScratchPointers scratch;
#define ResultMenuRowTable labels.value
#define ResultMenuTableIndices indices.value
#define SCRATCHPAD ((uintptr_t)&scratch)
FrameStorage *CurrentDrawBuf;
s16 GameModeType; s8 MenuSubmodePendingId; u16 SelectedCourseChoice;
static u32 alias;
typedef uintptr_t (*Observer)(unsigned,const uintptr_t *);
static Observer observer;
void set_observer(Observer value){observer=value;}
static uintptr_t observe(unsigned kind,const uintptr_t *args){return observer(kind,args);}
static void *resolve(u32 address){
 u32 physical=address&0x1FFFFFFF;
 if(!address)return 0;
 for(unsigned b=0;b<2;b++){
  u32 base=0x180000+b*0x40000;
  if(physical>=base&&physical<=base+sizeof(FrameOwner))return (u8 *)&frames[b]+(physical-base);
 }
 if(physical>=0x280000&&physical<=0x280000+sizeof(PacketOwner))return (u8 *)&packets+(physical-0x280000);
 if(physical>=0x9F730&&physical<0x9F790)return (u8 *)labels.value+(physical-0x9F730);
 assert(!"unregistered native owner");return 0;
}
static u32 encode(const void *pointer){
 if(!pointer)return 0;uintptr_t p=(uintptr_t)pointer;
 for(unsigned b=0;b<2;b++){
  uintptr_t base=(uintptr_t)&frames[b];
  if(p>=base&&p-base<=sizeof(FrameOwner))return (0x80180000+b*0x40000+alias)+(p-base);
 }
 uintptr_t base=(uintptr_t)&packets;
 if(p>=base&&p-base<=sizeof(PacketOwner))return 0x80280000+alias+(p-base);
 base=(uintptr_t)labels.value;
 if(p>=base&&p-base<sizeof(labels.value))return 0x8009F730+(p-base);
 assert(!"pointer lost its complete owner");return 0;
}
uintptr_t native_pointer(u32 address){return (uintptr_t)resolve(address);}
u32 pal_pointer(uintptr_t pointer){return encode((const void *)pointer);}
void prepare_alias(u32 value){
 assert(value==0||value==0x20000000);alias=value;
 static GpuDmaRegion map[3];
 map[0]=(GpuDmaRegion){&frames[0].value,0x80180010+alias,sizeof(FrameStorage)};
 map[1]=(GpuDmaRegion){&frames[1].value,0x801C0010+alias,sizeof(FrameStorage)};
 map[2]=(GpuDmaRegion){packets.value,0x80280010+alias,sizeof(packets.value)};
 assert(GpuDmaSetRegions(map,3));
}
void initialize(void){
 _Static_assert(sizeof(void *)==8,"full native pointers");
 _Static_assert(sizeof(FrameOwner)==0x22778+32,"complete frame owner");
 _Static_assert(sizeof(UiUvRect)==4,"shared UV record");
 assert((uintptr_t)&packets>UINT32_MAX&&(uintptr_t)&frames[0]>UINT32_MAX);
 memset(&labels,0xA5,sizeof(labels));memset(&indices,0xA5,sizeof(indices));
}
static u32 get(const u8 **p){u32 value;memcpy(&value,*p,4);*p+=4;return value;}
static void put(u8 **p,u32 value){memcpy(*p,&value,4);*p+=4;}
void load_state(const u8 *p){
 assert(get(&p)==alias);
 memcpy(labels.value,p,96);p+=96;memcpy(indices.value,p,16);p+=16;
 memcpy(frames,p,sizeof(frames));p+=sizeof(frames);memcpy(&packets,p,sizeof(packets));p+=sizeof(packets);
 GameModeType=get(&p);MenuSubmodePendingId=get(&p);SelectedCourseChoice=get(&p);
 CurrentDrawBuf=resolve(get(&p));scratch.cursor=resolve(get(&p));scratch.order=resolve(get(&p));
}
void store_state(u8 *p){
 put(&p,alias);memcpy(p,labels.value,96);p+=96;memcpy(p,indices.value,16);p+=16;
 memcpy(p,frames,sizeof(frames));p+=sizeof(frames);memcpy(p,&packets,sizeof(packets));p+=sizeof(packets);
 put(&p,(s32)GameModeType);put(&p,(s32)MenuSubmodePendingId);put(&p,SelectedCourseChoice);
 put(&p,encode(CurrentDrawBuf));put(&p,encode(scratch.cursor));put(&p,encode(scratch.order));
}
void check_table_guards(void){for(unsigned i=0;i<16;i++)assert(labels.before[i]==0xA5&&labels.after[i]==0xA5&&indices.before[i]==0xA5&&indices.after[i]==0xA5);}
'''
TRACE=r'''
void *AddSpritePrim(u32 *head,void *cursor,s32 x,s32 y,UiUvRect *uv,u16 clut,s32 flags){
 uintptr_t args[]={(uintptr_t)head,(uintptr_t)cursor,(u32)x,(u32)y,(uintptr_t)uv,clut,(u32)flags};return (void *)observe(0,args);
}
void *AddDrawModePrim(u32 *head,GpuDrawModePacket *cursor,u16 page){
 uintptr_t args[]={(uintptr_t)head,(uintptr_t)cursor,page};return (void *)observe(1,args);
}
'''
DMA=r'''
/* Only post-link observation; actual_dma_link is the literal production body. */
void AddPrim(u32 *head,u32 *packet){
 actual_dma_link(head,packet);
 u32 address=GpuDmaEncodeAddress(packet);
 uintptr_t args[]={(uintptr_t)head,(uintptr_t)packet,address};observe(2,args);
}
'''

def signed(v,bits=32):
 v&=(1<<bits)-1
 return v-(1<<bits)if v&(1<<(bits-1))else v
def word(v):return struct.pack('<I',v&0xFFFFFFFF)

class State:
 def __init__(self,data):
  at=0
  def take(n):
   nonlocal at
   value=bytearray(data[at:at+n]);at+=n;return value
  self.alias=int.from_bytes(take(4),'little');self.labels=take(96);self.indices=take(16)
  self.frames=[take(FRAME_OWNER)for _ in range(2)];self.arena=take(ARENA_OWNER)
  self.scalars=list(struct.unpack('<3I',take(12)));self.roots=list(struct.unpack('<3I',take(12)));assert at==SIZE
 def snapshot(self):return b''.join([word(self.alias),self.labels,self.indices,*self.frames,self.arena,*map(word,self.scalars),*map(word,self.roots)])
 def memory(self,address,size):
  physical=address&0x1FFFFFFF
  for b,base in enumerate(FRAMES):
   offset=physical-((base-16)&0x1FFFFFFF)
   if 0<=offset and offset+size<=FRAME_OWNER:return self.frames[b],offset
  offset=physical-((ARENA-16)&0x1FFFFFFF)
  if 0<=offset and offset+size<=ARENA_OWNER:return self.arena,offset
  raise AssertionError(('outside wire owners',hex(address),size))
 def get_word(self,address):
  data,offset=self.memory(address,4);return struct.unpack_from('<I',data,offset)[0]
 def put_word(self,address,value):
  data,offset=self.memory(address,4);struct.pack_into('<I',data,offset,value&0xFFFFFFFF)
 def dma(self,head,packet,address):
  self.put_word(packet,(self.get_word(packet)&0xFF000000)|(self.get_word(head)&0xFFFFFF))
  self.put_word(head,(self.get_word(head)&0xFF000000)|(address&0xFFFFFF))

def reference(state,fade,service,pipeline):
 shade=signed((fade&0xFFFFFFFF)*2)if fade<65 else 128
 flags=((shade+1)*2)&0xFFFFFFFF;cursor=state.roots[1]
 def sprite(cursor,x,y,label,clut):
  head=state.roots[0]+112;uv=address_of('ResultMenuRowTable')+label*4
  if not pipeline:return service(0,[head,cursor,x,y,uv,clut,flags],state)
  data,o=state.memory(cursor,20);u,v,w,h=state.labels[label*4:label*4+4]
  data[o+3]=4;data[o+7]=0x64
  brightness=signed(flags)>>1
  if brightness:data[o+4:o+7]=bytes([brightness&255])*3
  else:data[o+7]|=1
  if flags&1:data[o+7]|=2
  struct.pack_into('<hhBBHHH',data,o+8,signed(x,16),signed(y,16),u,v,clut,w,h)
  state.dma(head,cursor,cursor);service(2,[head,cursor,cursor],state)
  return cursor+20
 label=0;x=80
 for row in range(4):
  if row<2:
   mode=signed(state.scalars[0],16)
   if mode!=1 and row==1:continue
   if mode==1:label=state.indices[signed(state.scalars[1],8)*2+row]
   else:label={0:3,2:0,3:0,4:1,6:2}[mode]
  elif row==2:label=12
  else:label+=1+(state.scalars[2]&7)
  cursor=sprite(cursor,x,72,label,0x263A)
  x+=2+state.labels[label*4+2]
 if signed(state.scalars[0],16)==1:label,count,x=21,2,230
 else:label,count,x=22,1,214
 clut=0x263A
 for _ in range(count):
  cursor=sprite(cursor,x,36,label,clut);label=23;x-=2+state.labels[23*4+2];clut=0x263B
 head=state.roots[0]+112
 if pipeline:
  data,o=state.memory(cursor,12);data[o+3]=2
  struct.pack_into('<II',data,o+4,0xE100022E,0)
  state.dma(head,cursor,cursor);service(2,[head,cursor,cursor],state);cursor+=12
 else:cursor=service(1,[head,cursor,46],state)
 state.roots[1]=cursor

class ResultsLabelPipelineTest(unittest.TestCase):
 @classmethod
 def setUpClass(cls):
  cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup);d=Path(cls.temp.name)
  owners=source_owners(ROOT/'src/main/PAL');main=definition_source(owners[FUNCTION].read_text(),FUNCTION)
  cls.libraries=[]
  for pipeline in(False,True):
   bodies=[definition_source(owners[n].read_text(),n)for n in DEPENDENCIES]if pipeline else []
   bodies.append(main)
   if pipeline:
    link=definition_source(owners['AddPrim'].read_text(),'AddPrim')
    source=PREFIX+'\n#define AddPrim actual_dma_link\n'+link+'\n#undef AddPrim\n'+DMA+'\n'.join(bodies)
    bodies.append(link)
   else:source=PREFIX+TRACE+'\n'.join(bodies)
   assert all(source.count(body)==1 and not re.search(r'\b(?:asm|__asm__)\s*\(',body)for body in bodies)
   path=d/('pipeline'if pipeline else'trace');path.with_suffix('.c').write_text(source)
   result=subprocess.run(['cc','-std=gnu11','-O2','-shared','-fPIC','-funsigned-char','-Wall','-Wextra','-Werror','-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(path.with_suffix('.c')),str(ROOT/'src/native/gpu_address.c'),'-o',str(path.with_suffix('.so'))],capture_output=True,text=True)
   if result.returncode:raise AssertionError(result.stderr)
   lib=C.CDLL(str(path.with_suffix('.so')));lib.initialize();lib.native_pointer.argtypes=[C.c_uint32];lib.native_pointer.restype=C.c_size_t
   lib.pal_pointer.argtypes=[C.c_size_t];lib.pal_pointer.restype=C.c_uint32;lib.load_state.argtypes=[C.c_char_p]
   cls.libraries.append(lib)
  cls.original=(ROOT/'assets/PAL/main.exe').read_bytes()[0x800:]
  with(ROOT/'build/PAL/main.elf').open('rb')as stream:
   elf=ELFFile(stream);cls.linked=[(s['sh_addr'],s.data())for s in elf.iter_sections()if s['sh_flags']&2 and s['sh_type']=='SHT_PROGBITS']
 def read(self,a,n):return bytes(self.u.mem_read(a&0x1FFFFFFF,n))
 def write(self,a,data):self.u.mem_write(a&0x1FFFFFFF,bytes(data))
 def load(self,state,native):
  if native:self.lib.load_state(state.snapshot());return
  self.write(address_of('ResultMenuRowTable'),state.labels);self.write(address_of('ResultMenuTableIndices'),state.indices)
  for a,data in zip(FRAMES,state.frames):self.write(a-16,data)
  self.write(ARENA-16,state.arena)
  for name,size,value in zip(('GameModeType','MenuSubmodePendingId','SelectedCourseChoice'),(2,1,2),state.scalars):self.write(address_of(name),word(value)[:size])
  self.write(address_of('CurrentDrawBuf'),word(state.roots[0]));self.write(0x1F800000,b''.join(map(word,state.roots[1:])))
 def snapshot(self,native):
  if native:
   out=C.create_string_buffer(SIZE);self.lib.store_state(out);return State(out.raw)
  return State(b''.join([word(self.alias),self.read(address_of('ResultMenuRowTable'),96),self.read(address_of('ResultMenuTableIndices'),16),*(self.read(a-16,FRAME_OWNER)for a in FRAMES),self.read(ARENA-16,ARENA_OWNER),word(signed(int.from_bytes(self.read(address_of('GameModeType'),2),'little'),16)),word(signed(self.read(address_of('MenuSubmodePendingId'),1)[0],8)),word(int.from_bytes(self.read(address_of('SelectedCourseChoice'),2),'little')),self.read(address_of('CurrentDrawBuf'),4),self.read(0x1F800000,8)]))
 def boundary(self,op,args,state):
  args=[v&0xFFFFFFFF for v in args];self.events.append((op,tuple(args),state.snapshot()));serial=len(self.events)
  if self.case&16:
   state.roots[0]=FRAMES[(self.case+serial)&1]+state.alias
   state.roots[1]=ARENA+state.alias+512+serial*4
   state.scalars[:]=[(0,1,2,3,4,6)[(self.case+serial)%6],(self.case+serial)%8,(self.case*13+serial)&65535]
   for i in range(24):state.labels[i*4+2]=(state.labels[i*4+2]+serial+1)&255
   for i in range(16):state.indices[i]=(state.indices[i]+serial+1)%24
  if op<2:return args[1]+(20 if op==0 else 12)+((self.case+serial)%3)*4
  return 0
 def service(self,op,args,native):
  if native:
   for i in((0,1,4)if op==0 else(0,1)):args[i]=self.lib.pal_pointer(args[i])
  state=self.snapshot(native);result=self.boundary(op,args,state);self.load(state,native)
  return self.lib.native_pointer(result)if native and op<2 else result
 def run_mips(self,fade,pipeline):
  cpu=self.cpu;cpu.reset();cpu.pc=address_of(FUNCTION);cpu.r[4]=fade&0xFFFFFFFF;cpu.r[29]=STACK;cpu.r[31]=STOP
  saved={r:0x9ABC0000+r for r in(*range(16,24),28,30)}
  for r,v in saved.items():cpu.r[r]=v
  pending=None
  for _ in range(30000):
   if pending and cpu.pc==pending[0]:
    _,head,packet=pending;pending=None;self.service(2,[head,packet,packet],False)
   if cpu.pc==STOP:
    self.assertIsNone(pending);self.assertEqual(cpu.r[29],STACK);self.assertTrue(all(cpu.r[r]==v for r,v in saved.items()));return
   if pipeline:
    if cpu.pc==address_of('AddPrim'):
     self.assertIsNone(pending);pending=(cpu.r[31],cpu.r[4],cpu.r[5])
   elif cpu.pc in self.hooks:
    op=self.hooks[cpu.pc];n=7 if op==0 else 3
    args=[cpu.r[4+i]if i<4 else int.from_bytes(self.read(cpu.r[29]+i*4,4),'little')for i in range(n)]
    result=self.service(op,args,False);resume=cpu.r[31]
    for r in(*range(2,16),24,25):cpu.r[r]=0xDEAD0000+r
    cpu.r[2]=result;cpu.pc=resume;continue
   cpu.step()
  self.fail('MIPS instruction limit')
 def test_native_and_original_linked_rendering(self):
  observer_type=C.CFUNCTYPE(C.c_size_t,C.c_uint,C.POINTER(C.c_size_t));errors=[]
  def observe(op,args):
   try:return self.service(op,list(args[:7 if op==0 else 3]),True)
   except BaseException as error:errors.append(error);return 0
  observer=observer_type(observe)
  fades=(-2147483648,-2147483647,-1073741824,-65537,-129,-1,0,1,31,63,64,65,127,2147483647)
  for linked in(False,True):
   self.u=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);self.u.mem_map(0,0x400000);self.u.mem_map(0x1F800000,0x1000);self.write(0x80010000,self.original)
   if linked:
    for a,data in self.linked:self.write(a,data)
   allowed=[(a-16,FRAME_OWNER)for a in FRAMES]+[(ARENA-16,ARENA_OWNER),(STACK-1024,1088),(0x1F800000,8)]
   def store(cpu,access,a,n,v,user):
    a&=0x1FFFFFFF;self.assertTrue(any((p&0x1FFFFFFF)<=a and a+n<=(p&0x1FFFFFFF)+z for p,z in allowed),(hex(a),n))
   self.cpu=Cpu(self.u,0,100,101,102,store);self.hooks={address_of('AddSpritePrim'):0,address_of('AddDrawModePrim'):1};rng=random.Random(0x1ABE154)
   for self.case in range(256):
    self.alias=0x20000000 if self.case&8 else 0
    state=State(rng.randbytes(SIZE));state.alias=self.alias
    state.indices[:]=bytes(rng.randrange(24)for _ in range(16));state.scalars[:]=[(0,1,2,3,4,6)[self.case%6],(self.case//6)%8,(self.case*71)&65535]
    state.roots[:]=[FRAMES[self.case&1]+self.alias,ARENA+self.alias+16+(self.case%32)*4,FRAMES[(self.case//2)&1]+self.alias+112]
    fade=fades[(self.case//3)%len(fades)]
    for pipeline in(False,True):
     with self.subTest(linked=linked,case=self.case,pipeline=pipeline):
      expected=State(state.snapshot());self.events=[];reference(expected,fade,self.boundary,pipeline);events=self.events[:]
      self.lib=self.libraries[int(pipeline)];self.lib.set_observer(observer);self.lib.prepare_alias(self.alias)
      for native in(False,True):
       self.load(state,native);self.events=[]
       if native:getattr(self.lib,FUNCTION)(fade)
       else:self.run_mips(fade,pipeline)
       if errors:raise errors[0]
       self.assertEqual(events,self.events);self.assertEqual(expected.snapshot(),self.snapshot(native).snapshot())
       if native:self.lib.check_table_guards()
