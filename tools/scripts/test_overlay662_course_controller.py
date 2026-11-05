"""Full-owner original/compiled MIPS against native C, overlay662 controller154C8."""
from pathlib import Path
import ctypes as C, re,struct,sys,os,subprocess,random,json,hashlib
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.mips_const import *
ROOT=Path(__file__).resolve().parents[2]
source=Path(sys.argv[1]).resolve() if len(sys.argv)>1 else ROOT/'src/overlays/PAL/662/func_801154C8.c'
count=int(sys.argv[2]) if len(sys.argv)>2 else 4096
os.chdir(ROOT)
P=Path(os.getenv('RR_TEST_DIR',str(ROOT/'build/tests/overlay662_course_controller'))).resolve();P.mkdir(parents=True,exist_ok=True)
body=source.read_text().split('\n',1)[1];names=sorted(set(re.findall(r'\b(?:ovl662_)?(?:D_[0-9A-F]{8}|CarTable)\b',body)))
funcs=sorted(set(re.findall(r'\b(?:ovl662_)?func_[0-9A-F]{8}(?=\()',body))-{'ovl662_func_801154C8'})
head='''#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "rr/layout.h"
#undef RR_ASSERT_SIZE
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_SIZE(t,n)
#define RR_ASSERT_OFFSET(t,f,n)
#include "__CONTROLLER_HEADER__"
#include "rr/animated_sprite_grid.h"
'''
head=head.replace('__CONTROLLER_HEADER__',re.search(r'#include "([^"]+)"',source.read_text())[1])
(P/'native_header.c').write_text(head);pre=subprocess.check_output(['cc','-E','-Iinclude','-I'+str(P),'-I'+str(source.parent),str(P/'native_header.c')],text=True)
protos={}
for name in funcs:
 m=re.search(r'([\w *]+)\b'+name+r'\(([^;{}]*)\);',pre);assert m,name
 ret=m[1].strip().split('\n')[-1];types=[] if m[2].strip()=='void' else [x.strip() for x in m[2].split(',')];protos[name]=(ret,types)
code=head+'\n';guarded=[]
def guard(name,typ):
 global code
 i=len(guarded);guarded.append(name);code+=f'static struct {{u8 before[64]; {typ} value; u8 after[64];}} guard{i};\n#define {name} (guard{i}.value)\n'
for name in names:
 guard('n_'+name,f'__typeof__({name})');code+=f'#define {name} n_{name}\n'
code+='typedef RrUiRow24 Rows40[40];typedef u8 Mode56[56];typedef u8 Arena4096[4096];\ntypedef struct {RrGpuScratchPointers pointers;u8 tail[4096-8];} ScratchStorage;\n'
extra_types=[('frame0','RrFrameStorage'),('frame1','RrFrameStorage'),('rows0','Rows40'),('rows1','Rows40'),('badge0','RrBadgeRow1C'),('badge1','RrBadgeRow1C'),('mode0','Mode56'),('mode1','Mode56'),('arena0','Arena4096'),('arena1','Arena4096'),('arena2','Arena4096'),('scratch','ScratchStorage')]
for name,t in extra_types:guard(name,t)
code+='void init_guards(void){'
for i in range(len(guarded)):code+=f'memset(guard{i}.before,0xA6,64);memset(guard{i}.after,0x6A,64);'
code+='}\nunsigned check_guards(void){unsigned k;'
for i in range(len(guarded)):code+=f'for(k=0;k<64;k++)if(guard{i}.before[k]!=0xA6||guard{i}.after[k]!=0x6A)return {i+1};'
code+='return 0;}\ntypedef uintptr_t(*Callback)(unsigned,uintptr_t*);static Callback cb;void set_callback(Callback p){cb=p;}\n'
for i,name in enumerate(funcs):
 ret,types=protos[name];params=','.join(t+' a'+str(j) for j,t in enumerate(types)) or 'void'
 args=','.join(('((uintptr_t)a%d.r|((uintptr_t)a%d.g<<8)|((uintptr_t)a%d.b<<16))'%(j,j,j)) if t=='RrPreviewRgb3' else '(uintptr_t)a'+str(j) for j,t in enumerate(types)) or '0'
 code+=f'{ret} {name}({params}){{uintptr_t a[10]={{{args}}};uintptr_t r=cb({i},a);'+('return ('+ret+')r;' if ret!='void' else '')+'}\n'
code+=body+'\n'
objects=[('n_'+name,name) for name in names]+[(name,name) for name,t in extra_types]
code+='void *object(unsigned i){switch(i){'
for i,(cname,name) in enumerate(objects):code+=f'case {i}:return &{cname};'
code+='default:abort();}}\nsize_t size(unsigned i){switch(i){'
for i,(cname,name) in enumerate(objects):code+=f'case {i}:return sizeof({cname});'
code+='default:abort();}}\n'
(P/'native.c').write_text(code)
subprocess.run(['cc','-std=gnu11','-O2','-funsigned-char','-shared','-fPIC','-Iinclude','-I'+str(P),'-I'+str(source.parent),'-fsanitize=undefined,bounds','-fno-sanitize-recover=all',str(P/'native.c'),'-o',str(P/'native.dylib')],check=True)
lib=C.CDLL(str(P/'native.dylib'));lib.object.restype=C.c_void_p;lib.size.restype=C.c_size_t
N={name:(lib.object(i),lib.size(i)) for i,(cname,name) in enumerate(objects)}
addr=lambda name:__import__('c_functions').address_of(name)
pointers={'D_800ACC14','D_800ACC18','D_800F4D80','D_800F3AFC','D_800F2AB0','D_800F4D70'}
regions=[(name,p,addr(name),size) for name,(p,size) in N.items() if name in names and name not in pointers]
extra_addr={'frame0':0x80180000,'frame1':0x801a2778,'rows0':0x801d0000,'rows1':0x801d05a0,'badge0':0x801d1000,'badge1':0x801d101c,'mode0':0x801d2000,'mode1':0x801d2038,'arena0':0x80200000,'arena1':0x80201000,'arena2':0x80202000}
for name,a in extra_addr.items():p,n=N[name];regions.append((name,p,a,n))
regions.append(('scratch_tail',N['scratch'][0]+2*C.sizeof(C.c_void_p),0x1f800008,4088))
assert N['D_800ABEF8'][1]==1600 and N['D_800F4D98'][1]==32 and N['D_800F3A38'][1]==184
assert all(p>0xffffffff for p,size in N.values())
raw=Path('assets/PAL/main.exe').read_bytes();r4=Path('disc/PAL/R4.BIN').read_bytes();sector=struct.unpack_from('<I',r4,4+662*4)[0]
u=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);u.mem_map(0,0x400000);u.mem_map(0x1f800000,0x1000);u.mem_write(0x10000,raw[0x800:]);u.mem_write(0x114878,r4[sector*2048:sector*2048+0x20000])
if os.getenv('RR_COMPILED_ELF'):
 with open(os.environ['RR_COMPILED_ELF'],'rb') as f:
  e=ELFFile(f)
  for n in ['.text','.rodata']:
   s=e.get_section_by_name(n)
   if s:u.mem_write(s['sh_addr']&0x1fffffff,s.data())
def read(a,n):return bytes(u.mem_read(a&0x1fffffff,n))
def write(a,b):u.mem_write(a&0x1fffffff,b)
def pal(p):
 if not p:return 0
 for _,base,a,n in regions:
  if base<=p<base+n:return a+p-base
 raise AssertionError(('unowned nativepointer',hex(p)))
def host(a):
 if not a:return 0
 for _,p,base,n in regions:
  if base<=a<base+n:return p+a-base
 raise AssertionError(('unowned PALpointer',hex(a)))
def put(a,b,native):
 if native:C.memmove(host(a),b,len(b))
 else:write(a,b)
def val(a,n,native):return int.from_bytes(C.string_at(host(a),n) if native else read(a,n),'little')
def field(a,v,n,native):put(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'),native)
def root(name,a,native,index=0):
 if native:C.c_void_p.from_address(N[name][0]+index*8).value=host(a)
 else:write(addr(name)+index*4,struct.pack('<I',a))
def scratch_set(a,native,index=0):
 if native:C.c_void_p.from_address(N['scratch'][0]+index*C.sizeof(C.c_void_p)).value=host(a)
 else:write(0x1f800000+index*4,struct.pack('<I',a))
def scratch_get(native,index=0):return pal(C.c_void_p.from_address(N['scratch'][0]+index*C.sizeof(C.c_void_p)).value or 0) if native else int.from_bytes(read(0x1f800000+index*4,4),'little')
def snapshot(native):
 if native:
  bad=lib.check_guards();assert not bad,('native redzone',case,guarded[bad-1])
 b=b''.join(C.string_at(p,n) if native else read(a,n) for _,p,a,n in regions)
 roots=[]
 for name in sorted(pointers):
  for i in range(2 if name=='D_800F4D70' else 1):roots.append(pal(C.c_void_p.from_address(N[name][0]+i*8).value or 0) if native else int.from_bytes(read(addr(name)+i*4,4),'little'))
 roots += [scratch_get(native,i) for i in range(2)]
 return b+struct.pack('<9I',*roots)
case=0;events=[];observed=[];errors=[];coverage=set();coverage_states=set();markers=set()
def service(i,args,native):
 name=funcs[i];types=protos[name][1];args=list(args);norm=[]
 for j,t in enumerate(types):
  v=args[j];norm.append(pal(v) if native and '*' in t else (v&0xffffff if t=='RrPreviewRgb3' else v&0xffffffff))
 records=observed if native else events;records.append((i,norm,snapshot(native)));step=len(records);seed=case*17+step*29+i*7
 if not native:
  coverage.add(name)
  if name=='func_8002E394':coverage_states.add(val(0x800F4D8A,2,False))
 # Mutate full owners and roots across callbacks while preserving array domains.
 if case&32:
  field(0x800ABEF8+(seed%1600),seed,1,native);field(0x800F3A38+(seed%184),seed,1,native)
  if step%3==0:
   root('D_800F4D80',0x801d0000+(seed%2)*1440,native);root('D_800F3AFC',0x801d1000+(seed%2)*28,native)
   root('D_800F2AB0',0x801d2000+(seed%2)*56,native);root('D_800ACC14',0x80180000+(seed%2)*0x22778,native)
   root('D_800F4D70',0x80200000+(seed%2)*4096,native,0)
  field(0x800F4BEC,[-2147483648,-1,0,1,4,8,39,40][seed%8],4,native)
 if case&32 and step%3==0:
  scratch_set(0x80200000+(seed%3)*4096+64+(step%32)*32,native)
  scratch_set(0x80180000+(seed%2)*0x22778+(0x70 if seed&1 else 0xb70),native,1)
  field(0x1f800008+(seed%4088),seed,1,native)
 if name=='func_800207D4':
  cursor=scratch_get(native);head=norm[0];old=val(head,4,native)
  put(cursor,struct.pack('<4I',0x03000000|(old&0xffffff),0xA0000000|seed,norm[1],norm[2]),native)
  field(head,(old&0xff000000)|(cursor&0xffffff),4,native);scratch_set(cursor+16,native)
 if name=='func_8002B330':
  width=norm[1];a=norm[2];limit=C.c_int32(norm[3]).value;low=C.c_int32(norm[4]).value
  assert width==2 and 0<limit<=9,(norm,case,[(funcs[e[0]],e[1]) for e in records])
  field(a,low+(seed%max(1,limit-low)),width,native)
 if name=='func_80037040':
  # Real helper supplies three selection bytes inside the full32-byteowner.
  for k,v in enumerate([norm[1]&255,(norm[1]>>8)&255,[0,19,39,255][case%4]]):field(norm[0]+k,v,1,native)
 if name=='func_8003708C':return [0,1,0x10001,0x80000000][(case+step//2)%4]
 if name=='func_8003AE4C':return [1,1,1,0,0x1000001,-1][case%6]
 if name=='func_8003FBA8':return [0,1,-1,0x80000000,0x7fffffff][case%5]
 return 0
CB=C.CFUNCTYPE(C.c_size_t,C.c_uint,C.POINTER(C.c_size_t))
@CB
def callback(i,args):
 try:return service(i,[args[j] for j in range(len(protos[funcs[i]][1]))],True)&0xffffffffffffffff
 except BaseException as e:errors.append(e);return 0
lib.set_callback(callback)
serviceAddrs={addr(n)&0x1fffffff:i for i,n in enumerate(funcs)}
resume=None
def hook(uc,a,size,data):
 global resume
 a&=0x1fffffff
 if a==0x3ff000:uc.emu_stop();return
 if a not in serviceAddrs:return
 i=serviceAddrs[a];sp=uc.reg_read(UC_MIPS_REG_SP);args=[]
 for j in range(len(protos[funcs[i]][1])):args.append(uc.reg_read(UC_MIPS_REG_A0+j) if j<4 else int.from_bytes(read(sp+16+(j-4)*4,4),'little'))
 resume=(i,args,uc.reg_read(UC_MIPS_REG_RA));uc.emu_stop()
def service_return(item):
 i,args,ra=item;result=service(i,args,False)
 for r in [UC_MIPS_REG_AT,UC_MIPS_REG_V0,UC_MIPS_REG_V1]+list(range(UC_MIPS_REG_A0,UC_MIPS_REG_T7+1))+[UC_MIPS_REG_T8,UC_MIPS_REG_T9,UC_MIPS_REG_HI,UC_MIPS_REG_LO]:u.reg_write(r,0xBAD00000+r+len(events))
 u.reg_write(UC_MIPS_REG_V0,result&0xffffffff)
 return ra
for a in serviceAddrs:write(a,struct.pack('<3I',0,0x03e00008,0))
for a in serviceAddrs:u.hook_add(UC_HOOK_CODE,hook,begin=a|0x80000000,end=a|0x80000000)
u.hook_add(UC_HOOK_CODE,hook,begin=0x803ff000,end=0x803ff000)
allowed=[(a,n) for _,p,a,n in regions]+[(addr(name),8 if name=='D_800F4D70' else 4) for name in pointers]+[(0x803dff00,0x100),(0x1f800000,8)]
def memwrite(uc,access,a,n,value,data):
 a=a&0x1fffffff
 if a==0x1272ba:markers.add('case3_publish')
 if a==0xf3a26:markers.add('case5_scene' if value==0x1b else 'case8_scene' if value==0x27 else 'other_scene')
 assert any((base&0x1fffffff)<=a and a+n<=(base&0x1fffffff)+size for base,size in allowed),('write outside fullowners/stack',case,hex(a),n,hex(uc.reg_read(UC_MIPS_REG_PC)))
u.hook_add(UC_HOOK_MEM_WRITE,memwrite)
rng=random.Random(662154)
for case in range(count):
 initial={name:rng.randbytes(n) for name,p,a,n in regions}
 for native in [False,True]:
  if native:lib.init_guards()
  for name,p,a,n in regions:put(a,initial[name],native)
  root('D_800ACC14',0x80180000+(case%2)*0x22778,native);root('D_800ACC18',0x80200000,native)
  root('D_800F4D80',0x801d0000+(case%2)*1440,native);root('D_800F3AFC',0x801d1000+(case%2)*28,native);root('D_800F2AB0',0x801d2000+(case%2)*56,native)
  scratch_set(0x80202000+64,native);scratch_set(0x80180000+0x70,native,1)
  root('D_800F4D70',0x80200000,native,0);root('D_800F4D70',0x80201000,native,1)
  for a,v,n in [(0x800AC5D4,[0,1,0x80000000,0xffffffff][case%4],4),(0x800AC64A,(case//6)%4,2),(0x801272E4,(case//9)%9,2),(0x801272E0,9,2),(0x801272D4,case%3,2),(0x800F4D8A,[0,0,0,3,4,5,8,1,9,65535][(case//6)%10],2),(0x800F4BEC,[-2147483648,-1,0,1,4,8,39,40][case%8],4),(0x800AC5D0,[0,1,2,64,32768,65535][case%6],2)]:field(a,v,n,native)
  for bank in range(2):
   phase=[1,2,3,5,6,7,-1,0][(case//7)%8];field(0x801d2000+bank*56+1,phase,1,native);field(0x801d2000+bank*56+2,case%256,1,native)
   field(0x801d1000+bank*28+0x10,[3,3,11,0][(case//11)%4],2,native)
   for row in range(40):field(0x801d0000+bank*1440+row*36+20,[2,2,0,3][(case+row)%4],1,native)
  field(0x800F3A38+0x28,[0,1,0x1000000][(case//7)%3],4,native);field(0x800F3A38+0x2c,[0,1][(case//11)%2],4,native)
  if not native:
   events=[];u.reg_write(UC_MIPS_REG_SP,0x803e0000);u.reg_write(UC_MIPS_REG_RA,0x803ff000)
   pc=0x801154c8
   for budget in range(512):
    resume=None
    try:u.emu_start(pc,0,count=50000)
    except UcError:
     print("CPU",hex(pc),hex(u.reg_read(UC_MIPS_REG_PC)),flush=True);raise
    if resume is None:break
    pc=service_return(resume)
   assert u.reg_read(UC_MIPS_REG_PC)&0x1fffffff==0x3ff000,(case,hex(u.reg_read(UC_MIPS_REG_PC)))
   final=snapshot(False)
  else:
   observed=[];lib.ovl662_func_801154C8();assert not errors,errors
   assert len(events)==len(observed),(case,len(events),len(observed))
   for k,(a,b) in enumerate(zip(events,observed)):
    
    if a!=b:
     off=next((j for j,(x,y) in enumerate(zip(a[2],b[2])) if x!=y),None);cur=0
     for rn,rp,ra,rs in regions:
      if off is not None and cur<=off<cur+rs:print('OWNER',rn,hex(ra+off-cur),a[2][off:off+16].hex(),b[2][off:off+16].hex(),flush=True)
      cur+=rs
     print('ROOTS',sorted(pointers),a[2][-28:].hex(),b[2][-28:].hex());raise AssertionError((case,k,funcs[a[0]],a[:2],b[:2],off))
   assert final==snapshot(True),('final',case)
 if case%512==0:print('PASS',case,flush=True)
assert coverage==set(funcs),('uncovered helpers',sorted(set(funcs)-coverage))
assert {0,3,4,5,8}<=coverage_states,('uncovered switch states',sorted(coverage_states))
assert {'case3_publish','case5_scene','case8_scene'}<=markers,('uncovered writes',sorted(markers))
print('PASS',count,'states',sorted(coverage_states),'markers',sorted(markers),'services',len(coverage),sorted(coverage),'native redzones/fullscratch/MIPSwriteguards',flush=True)

if os.getenv('RR_AUDIT_JSON'):
 deps=subprocess.check_output(['cc','-MM','-Iinclude','-I'+str(source.parent),str(source)],text=True).replace('\\\n',' ').split(':',1)[1].split()
 files={str(Path(x).resolve().relative_to(ROOT)) if Path(x).resolve().is_relative_to(ROOT) else str(Path(x).resolve()):hashlib.sha256(Path(x).read_bytes()).hexdigest() for x in deps}
 report={'cases':count,'source':str(source.relative_to(ROOT)),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'test_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'dependencies':files,'services':sorted(coverage),'switch_states':sorted(coverage_states),'markers':sorted(markers),'original_mips':not bool(os.getenv('RR_COMPILED_ELF')),'native_redzones':True,'scratch_bytes':4096,'mips_write_guards':True,'sanitizers':['undefined','bounds']}
 if os.getenv('RR_COMPILED_ELF'):report['compiled_elf_sha256']=hashlib.sha256(Path(os.environ['RR_COMPILED_ELF']).read_bytes()).hexdigest()
 Path(os.environ['RR_AUDIT_JSON']).write_text(json.dumps(report,indent=2)+'\n')
