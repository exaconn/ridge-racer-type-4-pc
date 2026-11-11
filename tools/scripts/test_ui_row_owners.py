"""Actual constructors/updates and rumble setter, without body rewriting.

Trig, badge drawing and line emission are controlled service boundaries. Their
algorithms are outside this fixture. Independent state models check native C
and original/linked MIPS, including live root/table changes by those services.
"""
import ctypes as C
from pathlib import Path
import random
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
BADGES=(0x80240000,0x80241000);RECTS=(0x80242000,0x80243000)
FRAMES=(0x80180000,0x801C0000);FRAME_SIZE=0x22778
NAMES=('PadSetRumbleParams','InitRect16Row','InitBadgeRow','UpdateBadgeRowAnim','UpdateRect16Row')
SERVICES=('rsin','rcos','DrawBadgeRow','AddPartialLine')
PREFIX=r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "game/system/pad.h"
#include "game/system/pad3.h"
#include "game/ui/uidraw.h"
#include "game/ui/uibar.h"
#include "game/ui/hudmap.h"
#include "rr/ui_panel.h"
#include "rr/frame_storage_types.h"
#include "rr/trig.h"
typedef struct {u8 before[16];BadgeRow1C rows[8];u8 after[16];} BadgeOwner;
typedef struct {u8 before[16];MenuRectRow10 rows[8];u8 after[16];} RectOwner;
typedef struct {u8 before[16];ControllerState5C rows[2];u8 after[16];} PadOwner;
typedef struct {u8 before[16];FrameStorage value;u8 after[16];} FrameOwner;
BadgeOwner badges[2];RectOwner rectangles[2];PadOwner controllers;FrameOwner frames[2];
#define PlayerControllers controllers.rows
BadgeRow1C *BadgeRows;MenuRectRow10 *Rect16Rows;FrameStorage *CurrentDrawBuf;
s32 BadgeRowCount,Rect16RowCount;
MenuRectEdgeCorners MenuRectLineCorners[6];u8 MenuRectLineIntensitySlots[6];
typedef s32 (*Observer)(u32,const uintptr_t *);static Observer observer;
void set_observer(Observer value){observer=value;}
s32 rsin(s32 angle){uintptr_t a[]={(u32)angle};return observer(0,a);}
s32 rcos(s32 angle){uintptr_t a[]={(u32)angle};return observer(1,a);}
void DrawBadgeRow(s32 index){uintptr_t a[]={(u32)index};observer(2,a);}
void AddPartialLine(u32 *order,int x0,int y0,int x1,int y1,int level,
                    int width,int red,int green,int blue,int flags){
 uintptr_t a[]={(uintptr_t)order,(u32)x0,(u32)y0,(u32)x1,(u32)y1,
               (u32)level,(u32)width,(u32)red,(u32)green,(u32)blue,(u32)flags};observer(3,a);
}
void initialize_guards(void){memset(frames,0xB6,sizeof(frames));memset(&controllers,0xA5,sizeof(controllers));}
void check_guards(void){
 for(unsigned i=0;i<sizeof(frames);i++)assert(((u8 *)frames)[i]==0xB6);
 for(unsigned i=0;i<16;i++)assert(controllers.before[i]==0xA5&&controllers.after[i]==0xA5);
}
_Static_assert(sizeof(void *)==8,"full native pointers");
_Static_assert(sizeof(BadgeOwner)==256&&sizeof(RectOwner)==160&&sizeof(PadOwner)==216,"complete owners");
_Static_assert(sizeof(FrameStorage)==0x22778,"full frame storage");
_Static_assert(offsetof(BadgeRow1C,animationTick12)==18,"signed counter");
'''
def s16(n):
 n&=65535;return n-65536 if n&32768 else n
def s32(n):
 n&=0xFFFFFFFF;return n-0x100000000 if n&0x80000000 else n
def div(n,d):return -(abs(n)//d)if n<0 else n//d
class State:
 def __init__(self,data,roots,counts,tables):
  self.data=[bytearray(x)for x in data];self.roots=list(roots);self.counts=list(counts);self.tables=[bytearray(x)for x in tables]
 def snapshot(self):return(tuple(bytes(x)for x in self.data),tuple(self.roots),tuple(self.counts),tuple(bytes(x)for x in self.tables))
 def half(self,kind,index,offset,value=None):
  bank=self.roots[kind];assert bank in(0,1)and -2<=index<=5
  data=self.data[bank+kind*2];at=16+(index+2)*(28 if kind==0 else 16)+offset
  if value is None:return struct.unpack_from('<h',data,at)[0]
  struct.pack_into('<H',data,at,value&65535)
 def effect(self,operation,serial,case,index,angle=0):
  if case&8:
   self.roots[:]=[(case+serial)&1,(case+serial+1)&1,(case//2+serial)&1]
   self.counts[:]=[s32(0x80000001+case+serial),3-serial%6]
   if -2<=index<=5:
    for kind,offset in((0,12),(0,14),(0,18),(1,4),(1,14)):
     self.half(kind,index,offset,self.half(kind,index,offset)^(0x8101+serial))
   if -2<=index<=5:self.half(0,index,18,(32767,-32768,-1,0)[(case+serial)%4])
   for i in range(12):self.tables[0][i]=(self.tables[0][i]+1+serial)%4
   for i in range(6):self.tables[1][i]=(self.tables[1][i]+1+serial)%4
  return((angle*37+case*13+operation*911)&8191)-4096 if operation<2 else s32(0x81230000+serial*17+case)

def model(state,name,args,case):
 events=[];index=args[0]
 def service(operation,values):
  events.append((operation,tuple(v&0xFFFFFFFF for v in values),state.snapshot()))
  return state.effect(operation,len(events)-1,case,index,values[0]&0xFFFFFFFF)
 get=lambda off:state.half(0,index,off)
 put=lambda off,value:state.half(0,index,off,value)
 if name=='PadSetRumbleParams':
  struct.pack_into('<H',state.data[4],index*92+82,args[1]&65535);state.data[4][index*92+73]=args[2]&255
 elif name=='InitRect16Row':
  if index<state.counts[1]:
   for off,value in zip((0,2,4,6,8,10,12),(args[6],0,*args[1:6])):state.half(1,index,off,value)
 elif name=='InitBadgeRow':
  if index<state.counts[0]:
   _,x,y,width,height,phase,status=args;put(0,status);put(2,0)
   delta=0 if s16(status)<0 else 22
   if s16(status)==0:height=128
   for off,value in((6,y-delta),(10,y-delta),(4,x),(8,x),(12,width),(14,height+delta),(16,phase)):put(off,value)
 elif name=='UpdateBadgeRowAnim':
  if index<state.counts[0]:
   phase,tick=get(16),get(18)
   if phase==0:
    put(26,2);put(16,10);put(24,0);put(18,0);put(4,get(8));put(6,get(10)+get(14)-2);put(20,get(8));put(22,get(10)+get(14)-2)
   elif phase in(1,2,4,5,7,8):
    if phase==8:put(24,get(12)+2)
    scale=service(1 if phase in(4,5)else 0,[div(tick*1024,15)])
    if phase in(1,5,7):put(24,div((get(12)+2)*scale,4096))
    else:put(26,div(get(14)*scale,4096))
    if phase in(2,4,8):
     put(22,get(10)+(div(get(14),2)-div(get(26),2)if phase==8 else get(14)-get(26)))
     if phase!=8:put(6,get(22))
    if phase in(7,8):
     put(20,get(8))
     if phase==7:put(26,get(14));put(22,get(10))
    if tick<15:put(18,get(18)+1)
    else:put(16,{1:2,2:3,4:5,5:11,7:3,8:3}[phase]);put(18,0)
   elif phase==3:
    for target,source in((4,8),(6,10),(20,8),(22,10)):put(target,get(source))
    put(24,get(12)+2);put(26,get(14))
   elif phase==10 and get(2):put(16,1)
   service(2,[index])
 elif name=='UpdateRect16Row':
  if index>=state.counts[1]:return 0,events
  g=lambda off:state.half(1,index,off)
  p=lambda off,value:state.half(1,index,off,value)
  phase,tick=g(12),g(14);x,y,width,height=(g(n)for n in(4,6,8,10))
  xs=(x,x+width-1,x,x+width-1);ys=(y,y,y+height-1,y+height-1);levels=[0]*4
  if phase==0:p(12,1);p(14,0)
  elif phase==5:levels=[256]*4
  elif phase in(1,2,3,4):
   levels[:phase-1]=[256]*(phase-1);levels[phase-1]=div(tick*256,6)
   if tick<6:p(14,g(14)+1)
   else:p(14,0);p(12,g(12)+1)
  elif phase==8 and g(2):p(12,1)
  for line in range(6):
   a,b=state.tables[0][line*2:line*2+2]
   service(3,[FRAMES[state.roots[2]]+128,xs[a],ys[a],xs[b],ys[b],levels[state.tables[1][line]],1,8,8,8,-1])
  return int(phase==5),events
 return None,events

class UiRowOwnersTest(unittest.TestCase):
 @classmethod
 def setUpClass(cls):
  cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup);d=Path(cls.temp.name)
  owners=source_owners(ROOT/'src/main/PAL')
  source=PREFIX+'\n'.join(definition_source(owners[n].read_text(),n)for n in NAMES)
  (d/'rows.c').write_text(source)
  result=subprocess.run(['cc','-std=gnu11','-O2','-funsigned-char','-shared','-fPIC','-Wall','-Wextra','-Werror',
   '-Wno-implicit-fallthrough','-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(d/'rows.c'),'-o',str(d/'rows.so')],capture_output=True,text=True)
  if result.returncode:raise AssertionError(result.stderr)
  cls.lib=C.CDLL(str(d/'rows.so'))
  cls.buffers=[(C.c_ubyte*n).in_dll(cls.lib,name)for name,n in(('badges',512),('rectangles',320),('controllers',216))]
  cls.frames=(C.c_ubyte*((FRAME_SIZE+32)*2)).in_dll(cls.lib,'frames')
  cls.native_data=[(C.addressof(cls.buffers[0])+i*256,256)for i in range(2)]+[(C.addressof(cls.buffers[1])+i*160,160)for i in range(2)]+[(C.addressof(cls.buffers[2])+16,184)]
  cls.data_addresses=(*BADGES,*RECTS,address_of('PlayerControllers'))
  cls.owners=[(base,pal,size)for(base,size),pal in zip(cls.native_data,cls.data_addresses)]
  cls.owners +=[(C.addressof(cls.frames)+i*(FRAME_SIZE+32),FRAMES[i],FRAME_SIZE+32)for i in range(2)]
  assert all(base>0xFFFFFFFF for base,_,_ in cls.owners)
  cls.tables=[(C.c_ubyte*n).in_dll(cls.lib,name)for name,n in(('MenuRectLineCorners',12),('MenuRectLineIntensitySlots',6))]
  cls.root_names=('BadgeRows','Rect16Rows','CurrentDrawBuf');cls.count_names=('BadgeRowCount','Rect16RowCount')
  cls.roots=[C.c_void_p.in_dll(cls.lib,n)for n in cls.root_names];cls.counts=[C.c_int32.in_dll(cls.lib,n)for n in cls.count_names]
  cls.values=((BADGES[0]+72,BADGES[1]+72,0),(RECTS[0]+48,RECTS[1]+48,0),(FRAMES[0]+16,FRAMES[1]+16))
  cls.functions=[]
  for name,types in zip(NAMES,([C.c_int32]*3,[C.c_int32]+[C.c_int16]*6,[C.c_int32,C.c_int32,C.c_int16,C.c_int32,C.c_uint16,C.c_uint16,C.c_uint16],[C.c_int32],[C.c_int32])):
   fn=getattr(cls.lib,name);fn.argtypes=types;fn.restype=C.c_int32 if name=='UpdateRect16Row'else None;cls.functions.append(fn)
  cls.original=(ROOT/'assets/PAL/main.exe').read_bytes()[0x800:]
  with(ROOT/'build/PAL/main.elf').open('rb')as stream:
   elf=ELFFile(stream);cls.linked=[(s['sh_addr'],s.data())for s in elf.iter_sections()if s['sh_flags']&2 and s['sh_type']=='SHT_PROGBITS']
 def native_pointer(self,address):
  if not address:return None
  for base,pal,size in self.owners:
   if pal<=address<=pal+size:return base+address-pal
  raise AssertionError(('unknown owner',hex(address)))
 def pal_pointer(self,address):
  if not address:return 0
  for base,pal,size in self.owners:
   if base<=address<=base+size:return pal+address-base
  raise AssertionError(('unknown pointer',hex(address)))
 def read(self,address,size):return bytes(self.u.mem_read(address&0x1FFFFFFF,size))
 def write(self,address,data):self.u.mem_write(address&0x1FFFFFFF,bytes(data))
 def put(self,address,value):self.write(address,struct.pack('<I',value&0xFFFFFFFF))
 def load(self,state,native):
  for(base,size),pal,data in zip(self.native_data,self.data_addresses,state.data):
   if native:C.memmove(base,bytes(data),size)
   else:self.write(pal,data)
  for obj,name,data in zip(self.tables,('MenuRectLineCorners','MenuRectLineIntensitySlots'),state.tables):
   if native:C.memmove(C.addressof(obj),bytes(data),len(data))
   else:self.write(address_of(name),data)
  for cell,name,values,root in zip(self.roots,self.root_names,self.values,state.roots):
   if native:cell.value=self.native_pointer(values[root])
   else:self.put(address_of(name),values[root])
  for cell,name,value in zip(self.counts,self.count_names,state.counts):
   if native:cell.value=value
   else:self.put(address_of(name),value)
 def snapshot(self,native):
  if native:
   data=[C.string_at(base,size)for base,size in self.native_data]
   roots=[values.index(self.pal_pointer(cell.value))for values,cell in zip(self.values,self.roots)]
   counts=[cell.value for cell in self.counts];tables=[bytes(obj)for obj in self.tables]
  else:
   data=[self.read(pal,size)for(_,size),pal in zip(self.native_data,self.data_addresses)]
   roots=[values.index(int.from_bytes(self.read(address_of(n),4),'little'))for values,n in zip(self.values,self.root_names)]
   counts=[struct.unpack('<i',self.read(address_of(n),4))[0]for n in self.count_names]
   tables=[self.read(address_of(n),size)for n,size in(('MenuRectLineCorners',12),('MenuRectLineIntensitySlots',6))]
  return State(data,roots,counts,tables)
 def service(self,operation,args,native):
  values=[int(v)&0xFFFFFFFF for v in args]
  if operation==3 and native:values[0]=self.pal_pointer(args[0])
  state=self.snapshot(native);self.events.append((operation,tuple(values),state.snapshot()))
  result=state.effect(operation,len(self.events)-1,self.case,self.index,values[0]);self.load(state,native);return result
 def run_mips(self,name,args):
  cpu=self.cpu;cpu.reset();cpu.pc=address_of(name);cpu.r[29]=STACK;cpu.r[31]=STOP
  saved={r:0x9ABC0000+r for r in(*range(16,24),28,30)}
  for r,v in saved.items():cpu.r[r]=v
  for i,v in enumerate(args):
   if i<4:cpu.r[i+4]=v&0xFFFFFFFF
   else:self.put(STACK+i*4,v)
  for _ in range(10000):
   if cpu.pc==STOP:
    self.assertEqual(cpu.r[29],STACK);self.assertTrue(all(cpu.r[r]==v for r,v in saved.items()));return s32(cpu.r[2])
   if cpu.pc in self.hooks:
    operation=self.hooks[cpu.pc];count=(1,1,1,11)[operation]
    values=[cpu.r[i+4]if i<4 else int.from_bytes(self.read(cpu.r[29]+i*4,4),'little')for i in range(count)]
    result=self.service(operation,values,False);resume=cpu.r[31]
    for r in(*range(2,16),24,25):cpu.r[r]=0xDEAD0000+r
    cpu.r[2]=result&0xFFFFFFFF;cpu.pc=resume
   else:cpu.step()
  self.fail('MIPS instruction limit')
 def test_actual_owners_and_service_reloads(self):
  observer_type=C.CFUNCTYPE(C.c_int32,C.c_uint32,C.POINTER(C.c_size_t));errors=[]
  def observe(operation,args):
   try:return self.service(operation,list(args[:(1,1,1,11)[operation]]),True)
   except BaseException as error:errors.append(error);return 0
  observer=observer_type(observe);self.lib.set_observer(observer);self.lib.initialize_guards()
  edges=(-2147483648,2147483647,-65537,-32768,-1,0,1,32767,65535,65536,0x12345678)
  phases=(-32768,-1,*range(13),32767);ticks=(-32768,-1,0,1,5,6,14,15,16,32767)
  for linked in(False,True):
   self.u=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);self.u.mem_map(0,0x400000);self.write(0x80010000,self.original)
   if linked:
    for address,data in self.linked:self.write(address,data)
   for address in FRAMES:self.write(address,bytes([0xB6])*(FRAME_SIZE+32))
   allowed=[(a,size)for _,a,size in self.owners[:5]]+[(STACK-512,512)]
   def check_store(cpu,access,address,size,value,user):
    address&=0x1FFFFFFF
    self.assertTrue(any((a&0x1FFFFFFF)<=address and address+size<=(a&0x1FFFFFFF)+n for a,n in allowed),(hex(address),size))
   self.cpu=Cpu(self.u,0,100,101,102,check_store);self.hooks={address_of(n):i for i,n in enumerate(SERVICES)};rng=random.Random(0xBAD6E)
   for self.case in range(512):
    data=[rng.randbytes(n)for n in(256,256,160,160,184)];roots=[self.case&1,(self.case//2)&1,(self.case//4)&1]
    self.index=(-2,-1,0,1,3,4,5,2147483647)[self.case%8];counts=[6 if self.case&128 else 4]*2
    if self.case%11==0:counts=[-1,-1]
    if self.index>=counts[0]:roots[:2]=[2,2]
    state=State(data,roots,counts,[bytes(rng.randrange(4)for _ in range(n))for n in(12,6)])
    if self.index<counts[0]:
     for bank in(0,1):
      old=state.roots[:];state.roots[:2]=[bank,bank]
      for kind in(0,1):
       state.half(kind,self.index,2,(0,1,-1,32767)[(self.case//128)%4])
       state.half(kind,self.index,16 if kind==0 else 12,phases[(self.case//8)%len(phases)])
       state.half(kind,self.index,18 if kind==0 else 14,ticks[(self.case//16)%len(ticks)])
      state.roots[:]=old
    for operation,name in enumerate(NAMES):
     with self.subTest(linked=linked,case=self.case,function=name):
      args=[self.index]+[edges[(self.case+i*3)%len(edges)]for i in range(6)]
      if operation==0:args=[self.case&1,*args[1:3]]
      elif operation==1:args=[self.index]+[s16(x)for x in args[1:]]
      elif operation==2:args[2]=s16(args[2]);args[4:]=[v&65535 for v in args[4:]]
      else:args=[self.index]
      expected=State(*state.snapshot());result,events=model(expected,name,args,self.case)
      for native in(False,True):
       self.load(state,native);self.events=[]
       actual=self.functions[operation](*args)if native else self.run_mips(name,args)
       if errors:raise errors[0]
       self.assertEqual(events,self.events);self.assertEqual(expected.snapshot(),self.snapshot(native).snapshot())
       if result is not None:self.assertEqual(result,actual)
       if native:self.lib.check_guards()
    for address in FRAMES:self.assertEqual(bytes([0xB6])*(FRAME_SIZE+32),self.read(address,FRAME_SIZE+32))
