"""Overlay659 preview controller: original/compiled MIPS vs guarded native C.

RGB compares the three defined channels; its ABI padding is not a channel.
Stateful mocks preserve documented selector and active-model mode domains.
"""
from overlay_units import read_c_source, symbol_addresses, resolve_c_source_path
SYMBOL_ADDRESSES = symbol_addresses()
from pathlib import Path
import re,sys,subprocess,ctypes as C,struct,json,os,hashlib
from unicorn import Uc,UC_ARCH_MIPS,UC_MODE_MIPS32,UC_MODE_LITTLE_ENDIAN
from unicorn.mips_const import *
ROOT=Path(__file__).resolve().parents[2];os.chdir(ROOT)
P=Path(os.getenv('RR_TEST_DIR','build/tests/overlay659_preview_controller'));P.mkdir(parents=True,exist_ok=True)
source=Path(sys.argv[1]) if len(sys.argv)>1 else Path('src/overlays/PAL/659/func_80115CE4.c')
body=read_c_source(source);body=body[body.index('void ovl659_update_preview'):];body=re.sub(r' __asm__\("\$\d+"\)','',body)
names=sorted(set(re.findall(r'\b(?:(?:D_[0-9A-F]{8}|CarTable)|ovl659_[a-z][a-z0-9_]*)\b',body)))
names=[n for n in names if not re.search(r"\b"+n+r"\s*\(",body)]
functions=sorted(set(re.findall(r'\b(?:ovl659_)?func_[0-9A-F]{8}(?=\()',body))-{'ovl659_update_preview'})
head='''#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rr/layout.h"
#undef RR_ASSERT_SIZE
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_SIZE(t,n) _Static_assert(sizeof(void*)!=4 || sizeof(t)==(n),"PAL size")
#define RR_ASSERT_OFFSET(t,f,n) _Static_assert(sizeof(void*)!=4 || offsetof(t,f)==(n),"PAL offset")
#include "rr/frame_storage_types.h"
#include "rr/overlay659_preview_controller.h"
'''
(P/'header.c').write_text(head);pre=subprocess.check_output(['cc','-E','-Iinclude',str(P/'header.c')],text=True)
code=head+'\n';guard_names=[]
def guard(name,typ):
 global code
 i=len(guard_names);guard_names.append(name);code+=f'static struct {{u8 before[64];{typ} value;u8 after[64];}} guard{i};\n#define {name} guard{i}.value\n'
for n in names:guard('n_'+n,f'__typeof__({n})');code+=f'#define {n} n_{n}\n'
code+='typedef RrUiRow24 Rows7[7];typedef u8 Mode56[56];\n'
extras=[('frame0','RrFrameStorage'),('frame1','RrFrameStorage'),('rows0','Rows7'),('rows1','Rows7'),('badge0','RrBadgeRow1C'),('badge1','RrBadgeRow1C'),('mode0','Mode56'),('mode1','Mode56')]
for n,t in extras:guard(n,t)
code+='void init_guards(void){'
for i in range(len(guard_names)):code+=f'memset(guard{i}.before,0xA6,64);memset(guard{i}.after,0x6A,64);'
code+='}\nunsigned check_guards(void){unsigned k;'
for i in range(len(guard_names)):code+=f'for(k=0;k<64;k++)if(guard{i}.before[k]!=0xA6||guard{i}.after[k]!=0x6A)return {i+1};'
code+='return 0;}\ntypedef uintptr_t (*Callback)(unsigned,uintptr_t*);static Callback cb;void set_callback(Callback c){cb=c;}\n'
code+='''_Static_assert(sizeof(RrPreviewRgb3)==4&&__alignof__(RrPreviewRgb3)==4,"RGB word ABI");
_Static_assert(sizeof(RrUiRow24)==36&&sizeof(RrBadgeRow1C)==28,"complete rows");
_Static_assert(sizeof(RrRaceVehicle)==800&&sizeof(RrMenuSettings2C)==44,"complete owners");
_Static_assert(sizeof(unsigned long)>=sizeof(void*),"full compiler pointer width");
'''
protos={}
for i,n in enumerate(functions):
 m=re.search(r'([\w *]+)\b'+n+r'\(([^;{}]*)\);',pre);assert m,n
 ret=m[1].strip().split('\n')[-1].removeprefix('extern ');params=m[2];types=[] if params.strip() in ['', 'void'] else [t.strip() for t in params.split(',')];clean=[]
 for t in types:
  match=re.fullmatch(r'(.*?\*+)\s*(\w+)',t)
  if match:t=match[1]
  elif '*' not in t and len(t.split())>1:t=t.rsplit(' ',1)[0]
  clean.append(t)
 protos[n]=(ret,clean);decl=','.join(t+' a'+str(j) for j,t in enumerate(clean)) or 'void'
 args=','.join(f'((uintptr_t)a{j}.r|((uintptr_t)a{j}.g<<8)|((uintptr_t)a{j}.b<<16))' if t=='RrPreviewRgb3' else '(uintptr_t)a'+str(j) for j,t in enumerate(clean)) or '0'
 code+=f'{ret} {n}({decl}){{uintptr_t a[10]={{{args}}};uintptr_t r=cb({i},a);'+('return ('+ret+')r;' if ret!='void' else '')+'}\n'
code+=body+'\n';objects=[('n_'+n,n) for n in names]+[(n,n) for n,t in extras]
code+='void *object(unsigned i){switch(i){'
for i,(c,n) in enumerate(objects):code+=f'case {i}:return &{c};'
code+='default:abort();}}\nsize_t object_size(unsigned i){switch(i){'
for i,(c,n) in enumerate(objects):code+=f'case {i}:return sizeof({c});'
code+='default:abort();}}\n'
native=P/'native.c';native.write_text(code)
subprocess.run(['cc','-std=gnu11','-O2','-shared','-fPIC','-Iinclude','-fsanitize=undefined,bounds','-fno-sanitize-recover=all',str(native),'-o',str(P/'native.dylib')],check=True)
lib=C.CDLL(str(P/'native.dylib'));lib.object.restype=C.c_void_p;lib.object_size.restype=C.c_size_t
N={n:(lib.object(i),lib.object_size(i)) for i,(c,n) in enumerate(objects)};assert all(p>0xffffffff for p,s in N.values())
assert N['D_800ABEF8'][1]==1600 and N['D_800F3A38'][1]==184 and N['D_800AD4D8'][1]==44 and N['D_800F4D98'][1]==32
addr=lambda n:SYMBOL_ADDRESSES[n] if n in SYMBOL_ADDRESSES else __import__('c_functions').address_of(n)
pointers={'D_800ACC14','D_800F4D80','D_800F3AFC','D_800F2AB0'}
regions=[(n,p,addr(n),size) for n,(p,size) in N.items() if n in names and n not in pointers]
extra_addr={'frame0':0x80180000,'frame1':0x801A2778,'rows0':0x801D0000,'rows1':0x801D0200,'badge0':0x801D1000,'badge1':0x801D101C,'mode0':0x801D2000,'mode1':0x801D2038}
for n,a in extra_addr.items():p,size=N[n];regions.append((n,p,a,size))
intervals=sorted((a,a+size,n) for n,p,a,size in regions)
assert all(x[1]<=y[0] for x,y in zip(intervals,intervals[1:])),intervals
u=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);u.mem_map(0,0x400000);u.mem_write(0x10000,Path('assets/PAL/main.exe').read_bytes()[0x800:])
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
def pal(value):
 for n,p,a,s in regions:
  if p<=value<p+s:return a+value-p
 raise AssertionError(('unowned native pointer',hex(value)))
def host(value):
 for n,p,a,s in regions:
  if a<=value<a+s:return p+value-a
 raise AssertionError(('unowned PAL pointer',hex(value)))
def put(a,data,native):
 if native:C.memmove(host(a),data,len(data))
 else:write(a,data)
def field(a,value,size,native):put(a,(value&((1<<(size*8))-1)).to_bytes(size,'little'),native)
def root(n,a,native):
 if native:C.c_void_p.from_address(N[n][0]).value=host(a)
 else:write(addr(n),struct.pack('<I',a))
def snapshot(native):
 if native:
  bad=lib.check_guards();assert not bad,('native guard',case,guard_names[bad-1])
 b=b''.join(C.string_at(p,s) if native else read(a,s) for n,p,a,s in regions)
 roots=[pal(C.c_void_p.from_address(N[n][0]).value) if native else struct.unpack('<I',read(addr(n),4))[0] for n in sorted(pointers)]
 return b+struct.pack('<4I',*roots)
case=0;events=[];seen=0;errors=[];coverage=set();mutations=set();root_changes=set();states=set();rgb_modes=set();rgb_values=set()
phases=[0,1,2,3,4,5,6,7,9,10,8,65535]
row_states=[0,2,3,5,7]
mode_states=[-128,-1,0,1,2,3,5,127]
preview_steps={'func_80033A74','func_8006EF8C','func_80037198','func_8002E5A0'}
def service(event,args,native):
 global seen
 name=functions[event];norm=[pal(v) if native and '*' in t else v&0xffffff if t=='RrPreviewRgb3' else v&0xffffffff for v,t in zip(args,protos[name][1])];before=snapshot(native)
 if native:
  assert seen<len(events),(case,name,'unexpected service');e=events[seen];assert e[:2]==(event,norm),(case,seen,name,norm,e[:2]);assert e[2]==before,(case,seen,name,'complete world before service');serial=seen;seen+=1
 else:serial=len(events);events.append((event,norm,before));coverage.add(name)
 seed=case*17+serial*29+event*7
 if name=='func_8002CC88':rgb_modes.add(norm[3]);rgb_values.add(norm[1])
 if case&32:
  for n,base,stride in [('D_800ACC14',0x80180000,0x22778),('D_800F4D80',0x801D0000,512),('D_800F3AFC',0x801D1000,28),('D_800F2AB0',0x801D2000,56)]:root(n,base+(seed&1)*stride,native)
  for a,v,size in [(0x800AC64A,seed%3,2),(0x800F3A60,[-1,0,1][(seed//3)%3],4),(0x800AC5D0,[-32768,-1,0,1,63,64,65,32767][seed%8],2),(0x800F4D8A,phases[seed%len(phases)],2),(0x800F2DE4,seed*0x9e3779b9,4),(0x800ABEB0,~(seed*0x9e3779b9),4),(0x800AD4EF,seed%8,1),(0x800AD4F7,(seed//5)%2,1),(0x800F3A60,[0,1,-1][(seed//7)%3],4)]:field(a,v,size,native)
  for bank in range(2):
   for row in range(7):field(0x801D0000+bank*512+row*36+20,row_states[(seed+row)%len(row_states)],1,native)
   # These helpers execute only inside a model branch entered in mode1/2.
   mode=1+(seed&1) if name in preview_steps else mode_states[(seed//2)%len(mode_states)]
   field(0x801D2001+bank*56,mode,1,native);field(0x801D2002+bank*56,seed&255,1,native)
  field(0x800F3A60,[0,1,-1][(seed//11)%3],4,native)
  field(0x800F3A38+0x28,[0,1,0x80000000][(seed//3)%3],4,native);field(0x800F3A38+0x2C,[0,1][(seed//7)%2],4,native)
  field(0x800ABEF8+0x54,seed*0x1234567,4,native)
  mutations.add(name);root_changes.add(name)
 if name=='func_8002B330':
  width=norm[1];limit=norm[3];assert (width,limit) in [(1,2),(2,3)],norm
  field(norm[2],(case//12+serial)%limit,width,native)
 if name=='func_8003FBA8':return [0,1,0xffffffff,0x80000000,0x7fffffff][case%5]
 return 0
CB=C.CFUNCTYPE(C.c_size_t,C.c_uint,C.POINTER(C.c_size_t))
def callback(event,args):
 try:return service(event,list(args[:len(protos[functions[event]][1])]),True)
 except BaseException as e:errors.append(e);return 0
cb=CB(callback);lib.set_callback(cb)
argregs=[UC_MIPS_REG_A0,UC_MIPS_REG_A1,UC_MIPS_REG_A2,UC_MIPS_REG_A3]
volatile_regs=[UC_MIPS_REG_V1]+argregs+[globals()[f'UC_MIPS_REG_T{i}'] for i in range(10)]
service_map={__import__('c_functions').address_of(n):i for i,n in enumerate(functions)}
def service_return(item):
 event,args,ra=item;result=service(event,args,False)
 for r in volatile_regs:u.reg_write(r,0xA5A55A5A)
 u.reg_write(UC_MIPS_REG_HI,0x11223344);u.reg_write(UC_MIPS_REG_LO,0x55667788);u.reg_write(UC_MIPS_REG_V0,result)
 return ra
allowed=[(a,s) for n,p,a,s in regions]+[(addr(n),4) for n in pointers]+[(0x803E0000,0x10100)]
def memwrite(uc,access,a,size,value,data):
 a&=0x1fffffff
 assert any((base&0x1fffffff)<=a and a+size<=(base&0x1fffffff)+s for base,s in allowed),('MIPS store outside owners',case,hex(a),size)
framefill=b'\xA5'*0x22778

def initialize(n,native):
 if native:lib.init_guards()
 for name,p,a,size in regions:
  data=framefill if name.startswith('frame') else bytes(((n*17+j*29+(a>>8))&255) for j in range(size))
  if native:C.memmove(p,data,size)
  else:write(a,data)
 for name,base,stride in [('D_800ACC14',0x80180000,0x22778),('D_800F4D80',0x801D0000,512),('D_800F3AFC',0x801D1000,28),('D_800F2AB0',0x801D2000,56)]:root(name,base+(n&1)*stride,native)
 for a,v,size in [(0x800F4D8A,phases[n%len(phases)],2),(0x800F2DE4,[0,0xffffffff,0x7fffffff,0x80000000][n%4],4),(0x800ABEB0,[0xffffffff,0,0x80000000,0x7fffffff][(n//4)%4],4),(0x800AC64A,(n//12)%3,2),(0x800AD4EF,(n//3)%8,1),(0x800AD4F7,(n//7)%2,1),(0x800AC5D0,[-32768,-1,0,1,2,63,64,65,32767][(n//12)%9],2),(0x800F3A60,[0,1,-1][(n//4)%3],4),(0x800F3A38+0x28,[0,1,0x80000000][(n//12)%3],4),(0x800F3A38+0x2C,(n//36)%2,4)]:field(a,v,size,native)
 for bank in range(2):
  for row in range(7):field(0x801D0000+bank*512+row*36+20,row_states[(n//12+row+bank)%len(row_states)],1,native)
  field(0x801D2001+bank*56,mode_states[(n//12+bank)%len(mode_states)],1,native);field(0x801D2002+bank*56,n&255,1,native)
engine_path=ROOT/'tools/scripts/mips_preview_engine.py'
ns={};exec(compile(read_c_source(engine_path),str(engine_path),'exec'),ns)
u=ns['Mips'](u,UC_MIPS_REG_0,UC_MIPS_REG_PC,UC_MIPS_REG_HI,UC_MIPS_REG_LO,memwrite)
count=int(sys.argv[2]) if len(sys.argv)>2 else 8192
for case in range(count):
 events=[];seen=0;u.reset();initialize(case,False);states.add(phases[case%len(phases)])
 write(0x803EFF00,bytes([(case*17)&255])*256)
 for r in range(UC_MIPS_REG_0,UC_MIPS_REG_31+1):u.reg_write(r,0x5A5AA5A5)
 u.reg_write(UC_MIPS_REG_SP,0x803F0000);u.reg_write(UC_MIPS_REG_RA,0x803FF800);u.reg_write(UC_MIPS_REG_HI,0x12345678);u.reg_write(UC_MIPS_REG_LO,0x87654321)
 u.pc=0x80115CE4
 for budget in range(30000):
  if u.pc==0x803FF800:break
  if u.pc in service_map:
   event=service_map[u.pc];argc=len(protos[functions[event]][1]);args=[u.reg_read(r) for r in argregs[:min(argc,4)]]
   if argc>4:args+=list(struct.unpack('<'+'I'*(argc-4),read(u.reg_read(UC_MIPS_REG_SP)+16,4*(argc-4))))
   assert u.delay is None
   u.pc=service_return((event,args,u.reg_read(UC_MIPS_REG_RA)))
  else:u.step()
 assert u.pc==0x803FF800,(case,hex(u.pc))
 expected=snapshot(False);initialize(case,True);lib.ovl659_update_preview()
 if errors:raise errors[0]
 assert seen==len(events),(case,'missing services');assert expected==snapshot(True),(case,'final full owners')
 if case and case%512==0:print(case,'cases',flush=True)
assert coverage==set(functions),(coverage,set(functions))
assert rgb_modes=={1,0x102},rgb_modes
print('PASS',count,('compiled MIPS interpreter' if os.getenv('RR_COMPILED_ELF') else 'original MIPS interpreter'),'services',len(coverage),'mutating',len(mutations),'RGB',len(rgb_values),'states',sorted(states),flush=True)
if os.getenv('RR_AUDIT_JSON'):
 sha=lambda f:hashlib.sha256(resolve_c_source_path(f).read_bytes()).hexdigest()
 deps=subprocess.check_output(['cc','-MM','-Iinclude',str(native)],text=True).replace('\\\n',' ').split(':',1)[1].split()
 audit=dict(source=str(resolve_c_source_path(source)),source_sha256=sha(source),test_sha256=sha(__file__),dependencies={f:sha(f) for f in deps+[str(engine_path)]},cases=count,engine='compiled MIPS interpreter' if os.getenv('RR_COMPILED_ELF') else 'original MIPS interpreter',services=sorted(coverage),mutating_services=sorted(mutations),root_replacing_services=sorted(root_changes),phases=sorted(states),rgb_modes=sorted(rgb_modes),rgb_channel_patterns=len(rgb_values),color_argument='three channels in aligned4-byte struct; padding masked',full_owner_guards=True,full_host_pointers=True,caller_save_hi_lo_poison=True,mips_store_bounds=True,sanitizers=['undefined','bounds'])
 if os.getenv('RR_COMPILED_ELF'):audit['elf_sha256']=sha(os.environ['RR_COMPILED_ELF'])
 Path(os.environ['RR_AUDIT_JSON']).write_text(json.dumps(audit,indent=2)+'\n')
