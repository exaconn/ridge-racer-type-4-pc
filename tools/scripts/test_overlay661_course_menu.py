"""Original/candidate MIPS against full native owners, preserving PAL aliases."""
from pathlib import Path
import re,sys,subprocess,ctypes as C,struct,json,os,hashlib
from tempfile import TemporaryDirectory
from unicorn import Uc,UC_ARCH_MIPS,UC_MODE_MIPS32,UC_MODE_LITTLE_ENDIAN,UC_HOOK_CODE
from unicorn.mips_const import *
P=Path(__file__).resolve().parent
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/scripts/cc.sh').is_file())
os.chdir(ROOT)
scratch=TemporaryDirectory(prefix='rr661-native-');T=Path(scratch.name)
source=Path(sys.argv[1]) if len(sys.argv)>1 else ROOT/'src/overlays/PAL/661/func_80117430.c'
source=source.resolve()
full=source.read_text();start=full.index('void ovl661_func_80117430(void) {');body=full[start:];header=full[:start]
body=re.sub(r' __asm__\("\$\d+"\)','',body)
globals=sorted(set(re.findall(r'\b(?:ovl661_)?D_[0-9A-F]{8}\b',body))-{'D_800F3A60'})
functions=sorted(set(re.findall(r'\b(?:ovl661_)?func_[0-9A-F]{8}(?=\()',body))-{'ovl661_func_80117430'})
head='''#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "rr/layout.h"
#undef RR_ASSERT_SIZE
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_SIZE(t,n)
#define RR_ASSERT_OFFSET(t,f,n)
#include "rr/frame_storage_types.h"
'''+header
(T/'native_header.c').write_text(head)
pre=subprocess.check_output(['cc','-E','-Iinclude',str(T/'native_header.c')],text=True)
protos={}
for name in functions:
 m=re.search(r'([\w *]+)\b'+name+r'\(([^;{}]*)\);',pre);assert m,name
 types=[] if m[2].strip() in ['','void'] else [p.strip() for p in m[2].split(',')]
 protos[name]=(m[1].strip().split('\n')[-1],types)
code=head+'\n'
for name in globals:code+=f'__typeof__({name}) n_{name};\n#define {name} n_{name}\n'
code+='#define D_800F3A60 (n_D_800F3A38[0].a)\n'
code+='typedef union {RrFrameStorage frame;RrIconSpriteContext context;} FixtureFrame;static FixtureFrame frames[2];static RrUiRow24 ui[2][26];static u8 arenas[3][1024];\n'
code+='typedef uintptr_t (*Callback)(unsigned,uintptr_t*);static Callback cb;void set_callback(Callback c){cb=c;}\n'
for i,name in enumerate(functions):
 ret,types=protos[name];params=','.join(t+' a'+str(j) for j,t in enumerate(types)) or 'void';args=','.join('(uintptr_t)a'+str(j) for j in range(len(types))) or '0'
 code+=f'{ret} {name}({params}){{uintptr_t args[10]={{{args}}};uintptr_t result=cb({i},args);'+('return ('+ret+')result;' if ret!='void' else '')+'}\n'
code+=body+'\n'
code+='void *object(unsigned id){switch(id){\n'
for i,name in enumerate(globals):code+=f'case {i}:return &n_{name};\n'
G=len(globals)
code+=f'case {G}:return frames;case {G+1}:return ui;case {G+2}:return arenas;default:abort();}}}}\n'
code+='size_t object_size(unsigned id){switch(id){\n'
for i,name in enumerate(globals):code+=f'case {i}:return sizeof(n_{name});\n'
code+=f'case {G}:return sizeof(frames);case {G+1}:return sizeof(ui);case {G+2}:return sizeof(arenas);default:abort();}}}}\n'
code+='void pointers(unsigned bank){D_800ACC14=&frames[bank].context;D_800F4D80=ui[bank];D_800F4D70.first=arenas[bank];D_800F4D70.second=arenas[(bank+1)%3];D_800ACC18=arenas[(bank+2)%3];}\n'
(T/'native.c').write_text(code)
subprocess.run(['cc','-std=gnu11','-O2','-funsigned-char','-shared','-fPIC','-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-Iinclude',str(T/'native.c'),'-o',str(T/'native.dylib')],check=True)
lib=C.CDLL(str(T/'native.dylib'));lib.object.restype=C.c_void_p;lib.object_size.restype=C.c_size_t
N={name:(lib.object(i),lib.object_size(i)) for i,name in enumerate(globals)}
frame=lib.object(G);ui=lib.object(G+1);arenas=lib.object(G+2);FS=0x22778;US=26*36
assert lib.object_size(G)==FS*2 and lib.object_size(G+1)==US*2 and N['D_800F4D98'][1]==32 and N['D_800AC59C'][1]==4
assert min(frame,ui,arenas)>0xffffffff
u=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);u.mem_map(0,0x400000)
u.mem_write(0x10000,Path('assets/PAL/main.exe').read_bytes()[0x800:])
r4=Path('disc/PAL/R4.BIN').read_bytes();sector=struct.unpack_from('<I',r4,4+661*4)[0];u.mem_write(0x114878,r4[sector*2048:sector*2048+0x6000])
if os.getenv('RR_COMPILED_ELF'):
 from elftools.elf.elffile import ELFFile
 with open(os.environ['RR_COMPILED_ELF'],'rb') as f:u.mem_write(0x117430,ELFFile(f).get_section_by_name('.text').data())
def read(a,n):return bytes(u.mem_read(a&0x1fffffff,n))
def write(a,b):u.mem_write(a&0x1fffffff,b)
def addr(name):return int(name[-8:],16)
pointerNames={'D_800ACC14','D_800ACC18','D_800F4D80','D_800F4D70'}
regions=[(name,p,addr(name),size) for name,(p,size) in N.items() if name not in pointerNames]
regions+=[('ui',ui,0x801A0000,US*2),('arenas',arenas,0x801B0000,3072)]
mappings=[(p,a,size) for name,p,a,size in regions]+[(frame,0x800ADAF0,FS*2)]
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
 for j in range(2):parts.append(C.string_at(frame+j*FS,0x74) if native else read(0x800ADAF0+j*FS,0x74))
 if native:
  ptrs=[palpointer(C.c_void_p.from_address(N[n][0]).value) for n in ['D_800ACC14','D_800ACC18','D_800F4D80']]
  ptrs += [palpointer(C.c_void_p.from_address(N['D_800F4D70'][0]+j*8).value) for j in range(2)]
 else:
  ptrs=[struct.unpack('<I',read(addr(n),4))[0] for n in ['D_800ACC14','D_800ACC18','D_800F4D80']]
  ptrs+=list(struct.unpack('<2I',read(0x800F4D70,8)))
 return b''.join(parts)+struct.pack('<5I',*ptrs)
def setscalar(name,v,native):
 if name=='D_800F3A60':p=N['D_800F3A38'][0]+0x28;a=0x800F3A60;size=4
 else:p,size=N[name];a=addr(name)
 b=(v&((1<<(size*8))-1)).to_bytes(size,'little')
 if native:C.memmove(p,b,size)
 else:write(a,b)
def setmem(a,b,native):
 if native:C.memmove(nativepointer(a),b,len(b))
 else:write(a,b)
events=[];observed=[];coverage={};case=0
# The real selector's upper bound is exclusive. Preserve full width and PAL alias.
def service(event,args,native):
 normalized=[palpointer(v) if native and '*' in t else v&0xffffffff for v,t in zip(args,protos[functions[event]][1])]
 records=observed if native else events;records.append((event,normalized,snapshot(native)))
 name=functions[event];serial=len(records);coverage[name]=coverage.get(name,0)+1
 if name=='func_8002B330':
  selector,width,target,upper,minimum=normalized;assert width in [2,4] and upper>minimum,(case,normalized)
  value=minimum+(case//11+serial)%(upper-minimum)
  if target==0x800AC64A:value=min(value,3)
  if target==0x800ABEC8:value%=4
  setmem(target,int(value).to_bytes(width,'little'),native)
 if name=='func_800207D4':
  pointer=normalized[0];old=struct.unpack('<I',read(pointer,4))[0] if not native else C.c_uint32.from_address(nativepointer(pointer)).value
  setmem(pointer,struct.pack('<I',old^0x12345678),native)
 # Mutate fresh global loads while preserving every later array bound.
 if case&64:
  phase=(case//9)%9 if name=='func_8003BAB0' else (C.c_uint16.from_address(N['D_800F4D8A'][0]).value if native else struct.unpack('<H',read(0x800F4D8A,2))[0])
  changes={'D_800AC648':(case+serial)%10,'D_800AC64A':(case//3+serial)%(2 if phase==5 else 4),'D_800ABEC8':(case+serial)%4,'D_800AC5E0':4,'ovl661_D_80118414':10}
  if name!='func_8002B330':
   changes.update({'D_800F3A60':(case//7+serial)&1,'D_800AC59C':(case+serial)%6,'D_800AC5D0':[-32768,-1,0,1,64,32767][(case+serial)%6]})
  if name=='func_8003BAB0':changes['D_800F4D8A']=(case//9)%9
  for n,value in changes.items():setscalar(n,value,native)
  if name in ['func_8003A198','func_8003AE4C','func_80053CB0']:
   first=0x801B0000+(case+serial)%3*1024+16
   second=0x801B0000+(case+serial+1)%3*1024+32
   published=0x801B0000+(case+serial+2)%3*1024+64
   if native:
    C.c_void_p.from_address(N['D_800F4D70'][0]).value=nativepointer(first)
    C.c_void_p.from_address(N['D_800F4D70'][0]+8).value=nativepointer(second)
    C.c_void_p.from_address(N['D_800ACC18'][0]).value=nativepointer(published)
   else:
    write(0x800F4D70,struct.pack('<2I',first,second));write(0x800ACC18,struct.pack('<I',published))
  if name in ['func_8003A198','func_8004F24C']:
   bank=(case+serial)&1
   if native:
    C.c_void_p.from_address(N['D_800F4D80'][0]).value=ui+bank*US
    C.c_void_p.from_address(N['D_800ACC14'][0]).value=frame+bank*FS
   else:
    write(0x800F4D80,struct.pack('<I',0x801A0000+bank*US));write(0x800ACC14,struct.pack('<I',0x800ADAF0+bank*FS))
 if name=='func_8003BAB0':return (case//9)%3
 if name=='func_8003AE4C':return (case//11)&1
 if name=='func_80032E9C':return (case*31+serial)&0xffffffff
 return 0
CB=C.CFUNCTYPE(C.c_size_t,C.c_uint,C.POINTER(C.c_size_t));errors=[]
def callback(event,args):
 try:return service(event,list(args[:len(protos[functions[event]][1])]),True)
 except BaseException as e:errors.append(e);return 0
cb=CB(callback);lib.set_callback(cb)
regs=[UC_MIPS_REG_A0,UC_MIPS_REG_A1,UC_MIPS_REG_A2,UC_MIPS_REG_A3]
serviceMap={int(n[-8:],16):i for i,n in enumerate(functions)}
def hook(uc,a,size,data):
 event=serviceMap[a];argc=len(protos[functions[event]][1]);args=[uc.reg_read(r)&0xffffffff for r in regs[:min(argc,4)]]
 if argc>4:args+=list(struct.unpack('<'+'I'*(argc-4),read(uc.reg_read(UC_MIPS_REG_SP)+16,(argc-4)*4)))
 value=service(event,args,False);ret=uc.reg_read(UC_MIPS_REG_RA)
 for reg in [UC_MIPS_REG_V1,UC_MIPS_REG_A0,UC_MIPS_REG_A1,UC_MIPS_REG_A2,UC_MIPS_REG_A3,UC_MIPS_REG_T0,UC_MIPS_REG_T1,UC_MIPS_REG_T2,UC_MIPS_REG_T3,UC_MIPS_REG_T4,UC_MIPS_REG_T5,UC_MIPS_REG_T6,UC_MIPS_REG_T7,UC_MIPS_REG_T8,UC_MIPS_REG_T9,UC_MIPS_REG_HI,UC_MIPS_REG_LO]:uc.reg_write(reg,(0xDEAD0000+event*0x100+len(events))&0xffffffff)
 uc.reg_write(UC_MIPS_REG_V0,value);uc.reg_write(UC_MIPS_REG_PC,ret)
for a in serviceMap:u.hook_add(UC_HOOK_CODE,hook,begin=a,end=a)
def initialize(n,native):
 for name,p,a,size in regions:
  b=bytes(((n*17+j*29+(a>>8))&255) for j in range(size))
  if native:C.memmove(p,b,size)
  else:write(a,b)
 framebytes=b'\xa5'*(FS*2)
 if native:C.memmove(frame,framebytes,len(framebytes));lib.pointers(n&1)
 else:
  write(0x800ADAF0,framebytes);write(0x800ACC14,struct.pack('<I',0x800ADAF0+(n&1)*FS));write(0x800F4D80,struct.pack('<I',0x801A0000+(n&1)*US));write(0x800ACC18,struct.pack('<I',0x801B0000+((n&1)+2)%3*1024));write(0x800F4D70,struct.pack('<2I',0x801B0000+(n&1)*1024,0x801B0000+((n&1)+1)%3*1024))
 values={'D_800F4D8A':n%9,'D_800F2DE4':[0,1,0x7fffffff,0xffffffff][(n//9)%4],'D_800ABEB0':[0xffffffff,0x7fffffff,0x80000000,0][(n//36)%4],'D_800AC648':(n//4)%10,'D_800AC64A':(n//9)%(2 if n%9==5 else 4),'D_800AC5E0':4,'ovl661_D_80118414':10,'D_800ABEC8':(n//13)%4,'D_800AC59C':0xA5A50000|([0,1,2,3,4,5,0x8000,0xffff][(n//7)%8]),'D_800AC5D0':[-32768,-1,0,1,24,64,32767][(n//11)%7],'D_800F3A60':(n//7)&1}
 for name,v in values.items():setscalar(name,v,native)
 for bank in range(2):
  base=0x801A0000+bank*US
  for off in [0x29C,0x350,0x374,0x398]:setmem(base+off,bytes([[0,1,2,3,5,7][(n//17+bank+(off>>5))%6]]),native)
  setmem(base+0x31A,bytes([(n//19+bank)%11]),native)
 # Cancel independent of accept (the two controls share one complete owner).
 setmem(0x800F3A64,struct.pack('<I',(n//5)&1),native)
count=int(sys.argv[2]) if len(sys.argv)>2 else 32768
for case in range(count):
 events=[];observed=[];initialize(case,False)
 for reg in range(UC_MIPS_REG_1,UC_MIPS_REG_31+1):u.reg_write(reg,(0x5A5AA5A5+case*31+reg)&0xffffffff)
 u.reg_write(UC_MIPS_REG_SP,0x803F0000);u.reg_write(UC_MIPS_REG_RA,0x803FF800)
 try:u.emu_start(0x80117430,0x803FF800,count=10000)
 except Exception:print('MIPS failure',case,hex(u.reg_read(UC_MIPS_REG_PC)));raise
 expected=snapshot(False);fullframes=read(0x800ADAF0,FS*2);initialize(case,True);lib.ovl661_func_80117430()
 if errors:raise errors[0]
 assert len(events)==len(observed),(case,len(events),len(observed))
 for j,(a,b) in enumerate(zip(events,observed)):
  assert a[:2]==b[:2],(case,j,functions[a[0]],a[1],b[1])
  assert a[2]==b[2],(case,j,functions[a[0]],next((i,x,y) for i,(x,y) in enumerate(zip(a[2],b[2])) if x!=y))
 assert expected==snapshot(True),('final',case)
 assert fullframes==C.string_at(frame,FS*2),('frame owner guard',case)
 if case and case%4096==0:print(case,'cases',flush=True)
print(count,'cases passed;',('compiled MIPS' if os.getenv('RR_COMPILED_ELF') else 'original MIPS')+'/native full pointers; pre-service states, 26 UI rows, full selection32, two full frames;',len(coverage),'services')
audit={'source':str(source.relative_to(ROOT)),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'header_sha256':hashlib.sha256((ROOT/'include/rr/overlay661_course_menu.h').read_bytes()).hexdigest(),'test_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'cases':count,'engine':'compiled MIPS' if os.getenv('RR_COMPILED_ELF') else 'original MIPS','native_sanitizers':['undefined','bounds'],'coverage':coverage,'phase5_precondition':'When row24 state2, selected0..1 before every indexed access, including after services','alias':'D_F3A60 == D_F3A38[0].a','owners':['UI2x26rows','selection32','selection wordunion4','two full0x22778 frame owners','arena3x1024 full native pointers'],'checks':['all event arguments and complete scalar/UI/arena states','contextprefix before every event; entire frame guards after each case','caller-saved MIPS registers and HI/LO poisoned after all services','callbacks replace roots and fresh bounded selections; selector full width; union signed-low-half and high-half preservation']}
if os.getenv('RR_COMPILED_ELF'):audit['elf_sha256']=hashlib.sha256(Path(os.environ['RR_COMPILED_ELF']).read_bytes()).hexdigest()
if os.getenv('RR_AUDIT_JSON'):Path(os.environ['RR_AUDIT_JSON']).write_text(json.dumps(audit,indent=2)+'\n')
