"""Overlay659 frame controller: original/compiled MIPS vs full native owners.

Stateful mock services, full-width software pointers and precise argument lists.
Every service entry and the final guarded world are compared against MIPS.
"""
from overlay_units import read_c_source, symbol_addresses, resolve_c_source_path
SYMBOL_ADDRESSES = symbol_addresses()
from pathlib import Path
import re,sys,subprocess,ctypes as C,struct,json,os,hashlib
from unicorn import Uc,UC_ARCH_MIPS,UC_MODE_MIPS32,UC_MODE_LITTLE_ENDIAN,UC_HOOK_CODE
from unicorn.mips_const import *
ROOT=Path(__file__).resolve().parents[2];os.chdir(ROOT)
P=Path(os.getenv('RR_TEST_DIR','build/tests/overlay659_frame_controller'));P.mkdir(parents=True,exist_ok=True)
source=Path(sys.argv[1]) if len(sys.argv)>1 else Path('src/overlays/PAL/659/func_80114908.c')
body=read_c_source(source);body=body[body.index('void ovl659_update_race'):]
names=sorted(set(re.findall(r'\b(?:D_[0-9A-F]{8}|ovl659_[a-z][a-z0-9_]*)\b',body)))
names=[n for n in names if not re.search(r"\b"+n+r"\s*\(",body)]
assert 'D_800AD4EF' not in names
functions=sorted(set(re.findall(r'\b(?:ovl659_)?func_[0-9A-F]{8}(?=\()',body))-{'ovl659_update_race'})
head='''#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rr/layout.h"
#include "rr/frame_storage_types.h"
#undef RR_ASSERT_SIZE
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_SIZE(t,n) _Static_assert(sizeof(void*)!=4 || sizeof(t)==(n),"PAL size")
#define RR_ASSERT_OFFSET(t,f,n) _Static_assert(sizeof(void*)!=4 || offsetof(t,f)==(n),"PAL offset")
#include "rr/overlay659_frame_controller.h"
'''
(P/'header.c').write_text(head);pre=subprocess.check_output(['cc','-E','-Iinclude',str(P/'header.c')],text=True)
code=head+'\n';guarded={'D_800ABEF8','D_800ADAF0'}
for name in names:
 if name in guarded:code+=f'static struct {{u32 guard[4];__typeof__({name}) data;u32 tail[4];}} block_{name};\n#define {name} block_{name}.data\n'
 else:code+=f'__typeof__({name}) n_{name};\n#define {name} n_{name}\n'
code+='''
static struct {u32 guard[4];RrOvl659Scratch data;u32 tail[4];} scratchBlock;
#define scratch scratchBlock.data
#undef RR_659_SCRATCH
#define RR_659_SCRATCH (&scratch)
static struct {u32 guard[4];RrRaceVehicle data[8];u32 tail[4];} extraBlock;
static struct {u32 guard[4];u32 data[3][1024];u32 tail[4];} streamBlock;
_Static_assert(sizeof(RrRaceVehicle)==0x320,"complete vehicle");
_Static_assert(sizeof(RrFrameStorage)==0x22778,"complete frame arena");
_Static_assert(sizeof(RrMenuSettings2C)==44,"settings owner");
_Static_assert(offsetof(RrMenuSettings2C,phase17)==23,"true AD4EF alias");
typedef uintptr_t (*Callback)(unsigned,uintptr_t*);
static Callback cb;void set_callback(Callback c){cb=c;}
'''
protos={}
for i,name in enumerate(functions):
 m=re.search(r'([\w *]+)\b'+name+r'\(([^;{}]*)\);',pre);assert m,name
 ret=m[1].strip().split('\n')[-1].removeprefix('extern ');params=m[2]
 types=[] if params.strip() in ['', 'void'] else [t.strip() for t in params.split(',')];clean=[]
 for t in types:
  match=re.fullmatch(r'(.*?\*+)\s*(\w+)',t)
  if match:t=match[1]
  elif '*' not in t and len(t.split())>1:t=t.rsplit(' ',1)[0]
  clean.append(t)
 protos[name]=(ret,clean);args=','.join('(uintptr_t)a'+str(j) for j in range(len(clean))) or '0';decl=','.join(t+' a'+str(j) for j,t in enumerate(clean)) or 'void'
 code+=f'{ret} {name}({decl}){{uintptr_t a[10]={{{args}}};uintptr_t r=cb({i},a);'+('return ('+ret+')r;' if ret!='void' else '')+'}\n'
code+=body+'\nvoid *object(unsigned i){switch(i){\n'
for i,name in enumerate(names):code+=f'case {i}:return &{name};\n'
G=len(names);code+=f'case {G}:return &scratch;case {G+1}:return extraBlock.data;case {G+2}:return streamBlock.data;default:abort();}}}}\n'
code+='size_t object_size(unsigned i){switch(i){\n'
for i,name in enumerate(names):code+=f'case {i}:return sizeof({name});\n'
code+=f'case {G}:return sizeof(scratch);case {G+1}:return sizeof(extraBlock.data);case {G+2}:return sizeof(streamBlock.data);default:abort();}}}}\n'
code+='''
size_t scratch_offset(void){return offsetof(RrOvl659Scratch,word78);}
void pointers(unsigned f,unsigned c){D_800ACC14=(void*)&D_800ADAF0[f];scratch.pointers.cursor=(void*)streamBlock.data[c];scratch.pointers.order=streamBlock.data[(c+2)%3];}
void npc(unsigned slot,unsigned record){D_800FFC20[slot]=record<2?&D_800ABEF8[record]:&extraBlock.data[record-2];}
'''
native=P/'native.c';native.write_text(code)
subprocess.run(['cc','-std=gnu11','-O2','-shared','-fPIC','-Iinclude','-fsanitize=undefined,bounds','-fno-sanitize-recover=all',str(native),'-o',str(P/'native.dylib')],check=True)
lib=C.CDLL(str(P/'native.dylib'));lib.object.restype=C.c_void_p;lib.object_size.restype=C.c_size_t;lib.scratch_offset.restype=C.c_size_t
N={n:(lib.object(i),lib.object_size(i)) for i,n in enumerate(names)}
scratch=lib.object(G);scratch_size=lib.object_size(G);scratch_field=lib.scratch_offset();extra=lib.object(G+1);streams=lib.object(G+2)
frame=N['D_800ADAF0'][0];frame_size=N['D_800ADAF0'][1];assert frame_size==0x44EF0 and N['D_800ABEF8'][1]==0x640 and N['D_800FFC20'][1]==64
assert min(scratch,extra,streams,frame)>0xffffffff and scratch_field==0x80 and scratch_size==0x88
u=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);u.mem_map(0,0x400000);u.mem_map(0x1F7FF000,0x2000)
u.mem_write(0x10000,Path('assets/PAL/main.exe').read_bytes()[0x800:])
r4=Path('disc/PAL/R4.BIN').read_bytes();sector=struct.unpack_from('<I',r4,4+659*4)[0];u.mem_write(0x114878,r4[sector*2048:sector*2048+0x6000])
if os.getenv('RR_COMPILED_ELF'):
 from elftools.elf.elffile import ELFFile
 with open(os.environ['RR_COMPILED_ELF'],'rb') as f:
  elf=ELFFile(f)
  for n in ['.text','.rodata']:
   sec=elf.get_section_by_name(n)
   if sec:u.mem_write(sec['sh_addr']&0x1fffffff,sec.data())
read=lambda a,n:bytes(u.mem_read(a&0x1fffffff,n))
def write(a,b):u.mem_write(a&0x1fffffff,b)
addr=lambda n:SYMBOL_ADDRESSES[n] if n in SYMBOL_ADDRESSES else __import__('c_functions').address_of(n)
regions=[]
for n,(p,size) in N.items():
 if n in ['D_800ACC14','D_800FFC20']:continue
 if n in guarded:regions.append((n,p-16,addr(n)-16,size+32))
 else:regions.append((n,p,addr(n),size))
regions += [('extra',extra-16,0x8017FFF0,8*0x320+32),('streams',streams-16,0x8018FFF0,12288+32)]
mappings=[(p,a,s) for n,p,a,s in regions]
def palpointer(value):
 for p,a,s in mappings:
  if p<=value<p+s:return a+value-p
 raise AssertionError(('unowned native pointer',hex(value)))
def nativepointer(value):
 for p,a,s in mappings:
  if a<=value<a+s:return p+value-a
 raise AssertionError(('unowned PAL pointer',hex(value)))
def scalar(n,value,native):
 p,size=N[n];data=(value&((1<<(size*8))-1)).to_bytes(size,'little')
 if native:C.memmove(p,data,size)
 else:write(addr(n),data)
def field(n,offset,data,native):
 if native:C.memmove(N[n][0]+offset,data,len(data))
 else:write(addr(n)+offset,data)
def half(off,value,native):field('D_800ABEF8',off,struct.pack('<H',value&65535),native)
def roots(f,c,native):
 if native:lib.pointers(f,c)
 else:write(0x800ACC14,struct.pack('<I',0x800ADAF0+f*0x22778));write(0x1F800000,struct.pack('<2I',0x80190000+c*4096,0x80190000+((c+2)%3)*4096))
def npc(slot,record,native):
 if native:lib.npc(slot,record)
 else:write(0x800FFC20+slot*4,struct.pack('<I',0x800ABEF8+record*0x320 if record<2 else 0x80180000+(record-2)*0x320))
def snapshot(native):
 parts=[C.string_at(p,s) if native else read(a,s) for n,p,a,s in regions]
 if native:
  ptrs=[palpointer(C.c_void_p.from_address(N['D_800ACC14'][0]).value)]+[palpointer(C.c_void_p.from_address(N['D_800FFC20'][0]+j*8).value) for j in range(8)]
  cur=palpointer(C.c_void_p.from_address(scratch).value);order=palpointer(C.c_void_p.from_address(scratch+8).value)
  tail=C.string_at(scratch+16,0x70)+C.string_at(scratch+scratch_field,4)
  assert C.string_at(scratch+scratch_field+4,4)==b'\xA5'*4,'native tail padding'
  guards=C.string_at(scratch-16,16)+C.string_at(scratch+scratch_size,16)
 else:
  ptrs=[struct.unpack('<I',read(0x800ACC14,4))[0]]+list(struct.unpack('<8I',read(0x800FFC20,32)))
  cur,order=struct.unpack('<2I',read(0x1F800000,8));tail=read(0x1F800008,0x74);guards=read(0x1F7FFFF0,16)+read(0x1F80007C,16)
 return b''.join(parts)+struct.pack('<11I',*(ptrs+[cur,order]))+tail+guards
case=0;events=[];seen=0;errors=[];coverage=set();mutations=set();modes=set();root_changes=set()
words=[0,1,0xffffffff,0x7fffffff,0x80000000]
halves=[-32768,-1,0,1,32767]
heights=[0x80000000,-1,0,1,15,16,17,128,0x7fffffff]
def service(event,args,native):
 global seen
 name=functions[event];norm=[palpointer(v) if native and '*' in t else v&0xffffffff for v,t in zip(args,protos[name][1])];before=snapshot(native)
 if native:
  assert seen<len(events),(case,name,'unexpected service');e=events[seen];assert e[:2]==(event,norm),(case,seen,name,norm,e[:2]);assert e[2]==before,(case,seen,name,'complete world before service');serial=seen;seen+=1
 else:serial=len(events);events.append((event,norm,before));coverage.add(name)
 if case&64:
  changes={'D_800F2A64':(case*0x9e3779b9+event*97+serial*31)&0xffffffff}
  if name=='func_8006EF8C':
   changes.update(D_800AC5D0=halves[case%5],D_800FF6B0=[0,1,2,3,4,5,6,7,-1][(case//5)%9],D_800F4C68=[-128,-1,0,1,127][(case//7)%5])
   field('D_800F3A38',8,struct.pack('<H',[0,0x800,0xffff,0x8800][case%4]),native)
  if name in ['func_80034844','func_800348E8','func_8004FA50']:
   changes.update(D_800F2DE4=words[case%5],D_800F2FDC=[-128,-1,0,1,127][case%5])
  if name=='func_8004FA04':changes['D_800F2FDC']=[-128,-1,0,1,127][case%5]
  if name in ['func_8003C254','func_8005007C','func_80036030']:
   changes['D_800FF6B0']=[0,1,2,3,4,5,6,7,-1,0x7fffffff,0x80000000][(case//3+serial)%11]
  if name=='func_8003C96C':changes.update(D_800AC5A4=words[case%5],D_800ABEB0=words[(case//5)%5])
  if name in ['func_80022CD4','func_80052A30','func_8004FED4','func_800396E4','func_800207D4']:
   half(0x1EE,halves[case%5],native);half(0x1EA,(case//7)%2,native);half(0x1D8,halves[(case//5)%5],native);half(0x27A,halves[(case//25)%5],native)
  for n,v in changes.items():scalar(n,v,native)
  field('D_800AD4D8',0x17,bytes([(case+serial)%8]),native)
  npc(0,(case+serial)%10,native)
  mutations.add(name)
 if case&128:
  roots((case+serial)%2,(case+serial)%3,native);root_changes.add(name)
 if name=='func_8001FC28':
  # Mutate the captured incoming packet, then publish an unrelated cursor.
  # The return is a third pointer; callers must use the returned full pointer.
  data=struct.pack('<2I',(case*17+serial)&0xffffffff,event)
  if native:C.memmove(args[1],data,8)
  else:write(args[1],data)
  result=0x80190000+((case+serial)%3)*4096+((event*16)&1023)
  alternate=0x80190000+((case+serial+1)%3)*4096
  if native:C.c_void_p.from_address(scratch).value=nativepointer(alternate)
  else:write(0x1F800000,struct.pack('<I',alternate))
  if case&64:scalar('D_80107190',heights[(case+serial)%len(heights)],native)
  return nativepointer(result) if native else result
 return 0
CB=C.CFUNCTYPE(C.c_size_t,C.c_uint,C.POINTER(C.c_size_t))
def callback(event,args):
 try:return service(event,list(args[:len(protos[functions[event]][1])]),True)
 except BaseException as e:errors.append(e);return 0
cb=CB(callback);lib.set_callback(cb)
argregs=[UC_MIPS_REG_A0,UC_MIPS_REG_A1,UC_MIPS_REG_A2,UC_MIPS_REG_A3]
volatile_regs=[UC_MIPS_REG_V1]+argregs+[globals()[f'UC_MIPS_REG_T{i}'] for i in range(10)]
service_map={__import__('c_functions').address_of(n):i for i,n in enumerate(functions)}
def hook(uc,address,size,data):
 event=service_map[address];argc=len(protos[functions[event]][1]);args=[uc.reg_read(r)&0xffffffff for r in argregs[:min(argc,4)]]
 if argc>4:args+=list(struct.unpack('<'+'I'*(argc-4),read(uc.reg_read(UC_MIPS_REG_SP)+16,4*(argc-4))))
 result=service(event,args,False);ra=uc.reg_read(UC_MIPS_REG_RA)
 for r in volatile_regs:uc.reg_write(r,0xA5A55A5A)
 uc.reg_write(UC_MIPS_REG_HI,0x11223344);uc.reg_write(UC_MIPS_REG_LO,0x55667788);uc.reg_write(UC_MIPS_REG_V0,result);uc.reg_write(UC_MIPS_REG_PC,ra)
for a in service_map:u.hook_add(UC_HOOK_CODE,hook,begin=a,end=a)
framefill=b'\xA5'*(frame_size+32);streamfill=b'\x96'*(12288+32)
def initialize(n,native):
 for name,p,a,size in regions:
  data=framefill if name=='D_800ADAF0' else streamfill if name=='streams' else bytes(((n*17+j*29+(a>>8))&255) for j in range(size))
  if native:C.memmove(p,data,size)
  else:write(a,data)
 if native:C.memset(scratch-16,0xA5,scratch_size+32)
 else:write(0x1F7FFFF0,b'\xA5'*(0x7C+32))
 roots(n%2,n%3,native)
 for j in range(8):npc(j,(n+j)%10,native)
 values={'D_800F2DE4':[0xffffffff,0,1,23,24,25,49,50,51,73,74,75,174,175,176,0x7fffffff,0x80000000][n%17],
 'D_800FF6B0':[0,1,2,3,4,5,6,7,-1,0x7fffffff,0x80000000][(n//17)%11],
 'D_800AC5D0':[-32768,-1,0,1,14,15,24,25,37,75,32767][(n//187)%11],
 'D_800F4C68':[-128,-1,0,1,127][(n//3)%5],'D_800F2FDC':[-128,-1,0,1,2,127][(n//11)%6],
 'D_800AC5A4':words[n%5],'D_800ABEB0':words[(n//5)%5], 'D_800AC6EC':[0,1,-1][(n//11)%3],
 'D_800F2EB8':words[(n//7)%5], 'D_80107190':heights[(n//11)%len(heights)],'D_800AD522':halves[(n//13)%5]}
 for name,v in values.items():scalar(name,v,native)
 field('D_800AD4D8',0x17,bytes([n%8]),native)
 field('ovl659_finish_rank_limits',0,bytes([0,1,127,255][(n+j)%4] for j in range(4)),native)
 half(0x1EE,halves[n%5],native);half(0x1EA,(n//7)%2,native);half(0x1D8,halves[(n//5)%5],native);half(0x27A,halves[(n//25)%5],native)
 for j in range(8):
  data=struct.pack('<H',halves[(n+j)%5]&65535)
  if native:C.memmove(extra+j*0x320+0x1EE,data,2)
  else:write(0x80180000+j*0x320+0x1EE,data)
 field('D_800F3A38',8,struct.pack('<H',[0,0x800,0xffff,0x7ff,0x8800][(n//13)%5]),native)
count=int(sys.argv[2]) if len(sys.argv)>2 else 8192
for case in range(count):
 events=[];seen=0;initialize(case,False);modes.add([0,1,2,3,4,5,6,7,-1,0x7fffffff,0x80000000][(case//17)%11])
 for r in range(UC_MIPS_REG_0,UC_MIPS_REG_31+1):u.reg_write(r,0x5A5AA5A5)
 u.reg_write(UC_MIPS_REG_SP,0x803F0000);u.reg_write(UC_MIPS_REG_RA,0x803FF800);u.reg_write(UC_MIPS_REG_HI,0x12345678);u.reg_write(UC_MIPS_REG_LO,0x87654321)
 u.emu_start(0x80114908,0x803FF800,count=30000);assert u.reg_read(UC_MIPS_REG_PC)==0x803FF800
 expected=snapshot(False);initialize(case,True);lib.ovl659_update_race()
 if errors:raise errors[0]
 assert seen==len(events),(case,'missing services');assert expected==snapshot(True),(case,'final full owners')
 if case and case%512==0:print(case,'cases',flush=True)
assert coverage==set(functions),(coverage,set(functions))
assert mutations==set(functions) and root_changes==set(functions),(mutations,root_changes)
print('PASS',count,('compiled MIPS' if os.getenv('RR_COMPILED_ELF') else 'original MIPS'),'services',len(coverage),'mutating',len(mutations),'root_changes',len(root_changes),flush=True)
if os.getenv('RR_AUDIT_JSON'):
 sha=lambda f:hashlib.sha256(resolve_c_source_path(f).read_bytes()).hexdigest()
 deps=subprocess.check_output(['cc','-MM','-Iinclude',str(native)],text=True).replace('\\\n',' ').split(':',1)[1].split()
 audit=dict(source=str(resolve_c_source_path(source)),source_sha256=sha(source),test_sha256=sha(__file__),dependencies={f:sha(f) for f in deps},cases=count,engine='compiled MIPS' if os.getenv('RR_COMPILED_ELF') else 'original MIPS',services=sorted(coverage),mutating_services=sorted(mutations),root_replacing_services=sorted(root_changes),modes=sorted(modes),full_owner_guards=True,full_host_pointers=True,caller_save_hi_lo_poison=True,physical_settings_alias='D_800AD4EF = D_800AD4D8.phase17',scratch_wire_bytes=0x7C,scratch_native_bytes=scratch_size,sanitizers=['undefined','bounds'])
 if os.getenv('RR_COMPILED_ELF'):audit['elf_sha256']=sha(os.environ['RR_COMPILED_ELF'])
 Path(os.environ['RR_AUDIT_JSON']).write_text(json.dumps(audit,indent=2)+'\n')
