from overlay_units import read_c_source
"""Overlay660 frame controller: original MIPS/native service-state oracle.

Usage: python test_overlay660_race_controller.py [source.c [case_count]]
Default: public source,32768 cases; generated files stay under build/tests.
RR_COMPILED_ELF optionally substitutes candidate MIPS for compiler audits.
Services are controlled mocks, not physical GPU/audio emulation.
"""
from pathlib import Path
import re,sys,subprocess,ctypes as C,struct,json,os
from unicorn import Uc,UC_ARCH_MIPS,UC_MODE_MIPS32,UC_MODE_LITTLE_ENDIAN,UC_HOOK_CODE
from unicorn.mips_const import *
root=Path(__file__).resolve().parents[2]
if not (root/'assets/PAL/main.exe').is_file():root=Path(__file__).resolve().parents[3]
os.chdir(root)
P=Path(os.environ.get('RR_TEST_DIR','build/tests/overlay660_race_controller'));P.mkdir(parents=True,exist_ok=True)
source=Path(sys.argv[1]) if len(sys.argv)>1 else Path('src/overlays/PAL/660/func_8011718C.c')
body=read_c_source(source)
if body.startswith('#include'):body=body.split('\n',1)[1]
globals=sorted(set(re.findall(r'\b(?:ovl660_)?D_[0-9A-F]{8}\b',body)))
aliases={'D_800AC0E2':'half1EA','D_800AC0E6':'half1EE'}
globals=[x for x in globals if x not in aliases]
functions=sorted(set(re.findall(r'\b(?:ovl660_)?func_[0-9A-F]{8}(?=\()',body))-{'ovl660_func_8011718C'})
head='''#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "rr/layout.h"
#undef RR_ASSERT_SIZE
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_SIZE(t,n)
#define RR_ASSERT_OFFSET(t,f,n)
#include "__RR_CONTROLLER_HEADER__"
'''
head=head.replace('__RR_CONTROLLER_HEADER__',os.environ.get('RR_CONTROLLER_HEADER','rr/overlay660_race_controller.h'))
(P/'native_header.c').write_text(head)
pre=subprocess.check_output(['cc','-E','-Iinclude','-I'+str(source.parent),'-I'+str(P),str(P/'native_header.c')],text=True)
protos={}
for name in functions:
 matches=list(re.finditer(r'([\w *]+)\b'+name+r'\(([^;{}]*)\);',pre))
 m=matches[-1] if matches else None
 assert m,name
 protos[name]=(m[1].strip().split('\n')[-1],m[2])
# Each object retains its public declared type. Frames and pointer owners remain full-sized.
code=head+'\n'
for name in globals:code+=f'__typeof__({name}) n_{name};\n#define {name} n_{name}\n'
for name,off in aliases.items():code+=f'#define {name} (n_D_800ABEF8[0].{off})\n'
code+='static Rr660FrameScratch scratch;\n#undef RR_660_FRAME_SCRATCH\n#define RR_660_FRAME_SCRATCH (&scratch)\nstatic RrRaceVehicle extra[8];static u32 streams[3][1024];\n'
code+='typedef uintptr_t (*Callback)(unsigned,uintptr_t*);static Callback cb;void set_callback(Callback c){cb=c;}\n'
for i,name in enumerate(functions):
 ret,params=protos[name];types=[] if params.strip() in ['', 'void'] else [p.strip() for p in params.split(',')]
 # Prototypes here contain only types, except established GPU APIs with parameter names.
 clean=[]
 for t in types:
  m=re.fullmatch(r'(.*?\*+)\s*(\w+)',t)
  if m:t=m[1]
  elif '*' not in t and len(t.split())>1 and t not in ['unsigned long','struct w3g2p *']:
   t=t.rsplit(' ',1)[0]
  clean.append(t)
 params=','.join(t+' a'+str(j) for j,t in enumerate(clean)) or 'void'
 args=','.join('(uintptr_t)a'+str(j) for j in range(len(clean))) or '0'
 code+=f'{ret} {name}({params}){{uintptr_t args[10]={{{args}}};uintptr_t result=cb({i},args);'+('return ('+ret+')result;' if ret!='void' else '')+'}\n'
 protos[name]=(ret,clean)
code+=body+'\n'
code+='void *object(unsigned id){switch(id){\n'
for i,name in enumerate(globals):code+=f'case {i}:return &n_{name};\n'
code+=f'case {len(globals)}:return &scratch;case {len(globals)+1}:return extra;case {len(globals)+2}:return streams;default:abort();}}}}\n'
code+='size_t object_size(unsigned id){switch(id){\n'
for i,name in enumerate(globals):code+=f'case {i}:return sizeof(n_{name});\n'
code+=f'case {len(globals)}:return sizeof(scratch);case {len(globals)+1}:return sizeof(extra);case {len(globals)+2}:return sizeof(streams);default:abort();}}}}\n'
code+='void pointers(unsigned root,unsigned cursor){D_800ACC14=(void*)&D_800ADAF0[root];scratch.pointers.cursor=(void*)streams[cursor];scratch.pointers.order=streams[(cursor+2)%3];}\n'
(P/(source.stem+'_native.c')).write_text(code)
subprocess.run(['cc','-std=gnu11','-O2','-funsigned-char','-shared','-fPIC','-Iinclude','-I'+str(source.parent),'-I'+str(P),'-fsanitize=undefined,bounds','-fno-sanitize-recover=all',str(P/(source.stem+'_native.c')),'-o',str(P/(source.stem+'_native.dylib'))],check=True)
lib=C.CDLL(str(P/(source.stem+'_native.dylib')));lib.object.restype=C.c_void_p;lib.object_size.restype=C.c_size_t
print('Native compiled',len(globals),'owners',len(functions),'services',flush=True)
G=len(globals);N={name:(lib.object(i),lib.object_size(i)) for i,name in enumerate(globals)}
scratch=lib.object(G);streams=lib.object(G+2);frame=N['D_800ADAF0'][0];frameSize=0x22778
assert scratch>0xffffffff and N['D_800ABEF8'][1]==0x640 and N['D_800ADAF0'][1]==0x44ef0 and lib.object_size(G)==0x88
u=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);u.mem_map(0,0x400000);u.mem_map(0x1F800000,0x1000)
u.mem_write(0x10000,Path('assets/PAL/main.exe').read_bytes()[0x800:])
r4=Path('disc/PAL/R4.BIN').read_bytes();sector=struct.unpack_from('<I',r4,4+660*4)[0];u.mem_write(0x114878,r4[sector*2048:sector*2048+0x6000])
if os.getenv('RR_COMPILED_ELF'):
 from elftools.elf.elffile import ELFFile
 with open(os.environ['RR_COMPILED_ELF'],'rb') as file:u.mem_write(0x11718C,ELFFile(file).get_section_by_name('.text').data())
def read(a,n):return bytes(u.mem_read(a&0x1fffffff,n))
def write(a,b):u.mem_write(a&0x1fffffff,b)
addr=lambda name:int(name[-8:],16)
regions=[(name,p,addr(name),size) for name,(p,size) in N.items() if name not in ['D_800ACC14','D_800ADAF0']]
regions += [('scratchTail',scratch+16,0x1F800008,0x74)]
mappings=[(p,a,size) for name,p,a,size in regions]+[(frame,0x800ADAF0,frameSize*2),(streams,0x80190000,12288)]
def palpointer(p):
 for base,a,size in mappings:
  if base<=p<base+size:return a+p-base
 raise AssertionError(('unowned native pointer',hex(p)))
def nativepointer(a):
 for p,base,size in mappings:
  if base<=a<base+size:return p+a-base
 raise AssertionError(('unowned PAL pointer',hex(a)))
def snapshot(native):
 parts=[C.string_at(p,size) if native else read(a,size) for name,p,a,size in regions]
 for i in range(2):parts.append(C.string_at(frame+i*frameSize+0x18,1) if native else read(0x800ADAF0+i*frameSize+0x18,1))
 if native:
  root=palpointer(C.c_void_p.from_address(N['D_800ACC14'][0]).value)
  cursor=palpointer(C.c_void_p.from_address(scratch).value);order=palpointer(C.c_void_p.from_address(scratch+8).value)
 else:root=struct.unpack('<I',read(0x800ACC14,4))[0];cursor,order=struct.unpack('<2I',read(0x1F800000,8))
 return b''.join(parts)+struct.pack('<3I',root,cursor,order)
def setscalar(name,value,native):
 p,size=N[name];b=(value&((1<<(size*8))-1)).to_bytes(size,'little')
 if native:C.memmove(p,b,size)
 else:write(addr(name),b)
def sethalf(offset,value,native):
 b=struct.pack('<H',value&65535)
 if native:C.memmove(N['D_800ABEF8'][0]+offset,b,2)
 else:write(0x800ABEF8+offset,b)
def setinput(value,native):
 b=struct.pack('<H',value&65535)
 if native:C.memmove(N['D_800F3A38'][0]+8,b,2)
 else:write(0x800F3A40,b)
events=[];observed=[];case=0;callbackError=[]
def service(event,args,native):
 name=functions[event];normalized=[palpointer(v) if native and '*' in t else v&0xffffffff for v,t in zip(args,protos[name][1])]
 (observed if native else events).append((event,normalized,snapshot(native)))
 if case&64:
  changes={}
  if name=='func_8006EF8C':
   # Values consumed after the first render preparation: fresh timer, phase, pause and input.
   changes={'D_800AC5D0':[-32768,-1,0,1,32767][case%5],'D_800FF6B0':[0,1,2,3,4,5,6,-1][(case//5)%8], 'D_800F4C68':[-128,-1,0,1,127][(case//7)%5]}
   setinput([0,0x800,0xffff,0x8800][case%4],native)
  if name in ['func_80034844','func_800348E8','func_8004FA50']:
   changes['D_800F2DE4']=[0x7fffffff,0x80000000,0xffffffff,0,74][case%5]
   changes['D_800F2FDC']=[-128,-1,0,1,127][case%5]
  if name in ['func_8002E0D8','func_8006ECE8']:
   changes['D_800F2A64']=[0,1,0xffffffff,0x80000000,0x7fffffff][case%5]
  if name=='func_8002E0D8':
   expected=[-32768,-1,0,1,32767][(case//7)%5]
   changes['D_800AD522']=expected
   sethalf(0x1ea,expected+(case&1),native)
  if name in ['func_8003C254','ovl660_func_80117084','func_8004FED4']:
   changes['D_800FF6B0']=[0,1,2,3,4,5,6,-1,0x7fffffff,0x80000000][case%10]
  if name=='func_8004FA04':changes['D_800F2FDC']=[-128,-1,0,1,127][case%5]
  if name in ['func_8005007C','func_80036030']:changes['D_800FF6B0']=[0,1,4,5,6,-1,0x80000000,0x7fffffff][case%8]
  if name=='func_8003C96C':
   changes={'D_800AC5A4':[0,1,0x80000000,0x7fffffff][case%4],'D_800ABEB0':[0xffffffff,0x7fffffff,0x80000000,0][case%4]}
  if name in ['func_80022CD4','func_80052A30','func_8004FED4','func_800396E4','func_8003C254','ovl660_func_80117084']:
   # Both aliases are in the same actual vehicle owner.
   sethalf(0x1ee,[0,-1,1,32767,-32768][case%5],native)
   sethalf(0x1ea,case%2,native)
   if name=='func_800396E4':
    sethalf(0x1d8,[-32768,-1,0,1,32767][case%5],native)
    sethalf(0x27a,[-32768,-1,0,1,32767][(case//5)%5],native)
    sethalf(0x2aa,[-32768,-1,0,1,32767][(case//25)%5],native)
  for key,value in changes.items():
   if key in N:setscalar(key,value,native)
 # Fresh frame roots after services; every later head must be reread.
 if case&128 and name in ['func_800207D4','func_80022098','func_80033F3C','func_80034844','func_8003C96C']:
  root=(case+event)%2
  if native:C.c_void_p.from_address(N['D_800ACC14'][0]).value=frame+root*frameSize
  else:write(0x800ACC14,struct.pack('<I',0x800ADAF0+root*frameSize))
 if name=='func_8001FC28':
  result=0x80190000+((case+len(observed if native else events))%3)*4096+((event*16)&1023)
  scratchresult=0x80190000+((case+1)%3)*4096
  if native:C.c_void_p.from_address(scratch).value=nativepointer(scratchresult)
  else:write(0x1F800000,struct.pack('<I',scratchresult))
  return nativepointer(result) if native else result
 return 0
CB=C.CFUNCTYPE(C.c_size_t,C.c_uint,C.POINTER(C.c_size_t))
def callback(event,args):
 try:return service(event,list(args[:len(protos[functions[event]][1])]),True)
 except BaseException as e:callbackError.append(e);return 0
cb=CB(callback);lib.set_callback(cb)
regs=[UC_MIPS_REG_A0,UC_MIPS_REG_A1,UC_MIPS_REG_A2,UC_MIPS_REG_A3]
def hook(uc,a,size,data):
 event=serviceMap[a];count=len(protos[functions[event]][1]);args=[uc.reg_read(r)&0xffffffff for r in regs[:min(count,4)]]
 if count>4:args+=list(struct.unpack('<'+'I'*(count-4),read(uc.reg_read(UC_MIPS_REG_SP)+16,4*(count-4))))
 value=service(event,args,False);uc.reg_write(UC_MIPS_REG_V0,value);uc.reg_write(UC_MIPS_REG_PC,uc.reg_read(UC_MIPS_REG_RA))
serviceMap={int(name[-8:],16):i for i,name in enumerate(functions)}
for a in serviceMap:u.hook_add(UC_HOOK_CODE,hook,begin=a,end=a)
def initialize(n,native):
 for name,p,a,size in regions:
  data=bytes(((n*17+j*29+(a>>8))&255) for j in range(size))
  if native:C.memmove(p,data,size)
  else:write(a,data)
 data=b'\xa5'*(frameSize*2)
 if native:C.memmove(frame,data,len(data));lib.pointers(n%2,n%3)
 else:
  write(0x800ADAF0,data);write(0x800ACC14,struct.pack('<I',0x800ADAF0+(n%2)*frameSize));write(0x1F800000,struct.pack('<2I',0x80190000+(n%3)*4096,0x80190000+((n+2)%3)*4096))
 values={'D_800F2DE4':[0xffffffff,0,1,23,24,25,49,50,51,73,74,75,174,175,176,0x7fffffff,0x80000000][n%17],
 'D_800FF6B0':[0,1,2,3,4,5,6,7,-1,0x7fffffff,0x80000000][(n//17)%11],
 'D_800AC5D0':[-32768,-1,0,1,14,15,24,25,75,32767][(n//187)%10],
 'D_800F4C64':[-1,0,1,0x80000000][n%4],'D_800F4C68':[-128,-1,0,1,127][(n//3)%5],
 'D_800F2FDC':[-128,-1,0,1,2,127][(n//11)%6],
 'D_800AC5A4':[0,1,2,0x80000000,0x7fffffff][n%5],
 'D_800ABEB0':[0,1,0xffffffff,0x7fffffff,0x80000000][(n//5)%5], 'D_800F2A64':[0,1,0xffffffff,0x7fffffff,0x80000000][(n//7)%5], 'D_800AD522':[-32768,-1,0,1,32767][n%5]}
 for name,v in values.items():
  if name in N:setscalar(name,v,native)
 sethalf(0x1ee,[0,-1,1,32767,-32768][n%5],native);sethalf(0x1ea,(n//7)%2,native)
 # Independent full input-owner buttons; high bits also exercise unsigned halfword reads.
 setinput([0,0x800,0xffff,0x7ff,0x8800][(n//13)%5],native)
count=int(sys.argv[2]) if len(sys.argv)>2 else 8192
coverage=set()
for case in range(count):
 events=[];observed=[];initialize(case,False)
 u.reg_write(UC_MIPS_REG_SP,0x803F0000);u.reg_write(UC_MIPS_REG_RA,0x803FF800)
 try:u.emu_start(0x8011718C,0x803FF800,count=30000)
 except Exception:print('MIPS failure',case,hex(u.reg_read(UC_MIPS_REG_PC)));raise
 expected=snapshot(False);expectedFrames=read(0x800ADAF0,frameSize*2)
 initialize(case,True);lib.ovl660_func_8011718C()
 if callbackError:raise callbackError[0]
 assert len(events)==len(observed),(case,len(events),len(observed))
 for j,(e,o) in enumerate(zip(events,observed)):
  assert e[:2]==o[:2],(case,j,functions[e[0]],e[1],o[1])
  if e[2]!=o[2]:raise AssertionError((case,j,functions[e[0]],next((i,x,y) for i,(x,y) in enumerate(zip(e[2],o[2])) if x!=y)))
  coverage.add(functions[e[0]])
 assert expected==snapshot(True),('final',case)
 assert expectedFrames==C.string_at(frame,frameSize*2),('full frame guards',case)
 if case and case%512==0:print(case,'cases',flush=True)
print(count,'cases passed;',('compiled MIPS' if os.getenv('RR_COMPILED_ELF') else 'original MIPS')+'/native full pointers; state before all services; complete frame owners;',len(coverage),'services',flush=True)
(P/'native_coverage.json').write_text(json.dumps(sorted(coverage),indent=2)+'\n')
