from pathlib import Path
from tempfile import TemporaryDirectory
import ctypes as C,subprocess,sys,os,re,struct,random,json,hashlib
from unicorn import Uc,UC_ARCH_MIPS,UC_MODE_MIPS32,UC_MODE_LITTLE_ENDIAN
from unicorn.mips_const import *
P=Path(__file__).resolve().parent;ROOT=next(x for x in P.parents if (x/'Makefile').exists());os.chdir(ROOT)
source=Path(sys.argv[1]if len(sys.argv)>1 else ROOT/'tools/scripts/fixtures/views/func_8005ED34.c').resolve();count=int(sys.argv[2])if len(sys.argv)>2 else 131072
body=source.read_text();body=body[body.index('void func_8005ED34'):];body=re.sub(r'\s+(?:asm|__asm__)\s*\("\$\d+"\)','',body)
class Key(C.Structure):_fields_=[('glyph',C.c_int16),('timing',C.c_uint16)]
class Channel(C.Structure):_fields_=[('glyph',C.c_int16),('offset',C.c_uint16)]
class Track(C.Structure):_fields_=[('state',C.c_int32),('disabled',C.c_int32),('index',C.c_int32),('hold',C.c_int32),('wait',C.c_int32),('keys',C.c_void_p),('channel',Channel*2),('reserved20',C.c_int32),('step',C.c_int32)]
class Root(C.Structure):_fields_=[('prefix00',C.c_ubyte*0x4c),('speed4C',C.c_uint32),('prefix50',C.c_ubyte*(0x134-0x50)),('scroll',Track),('motionTail',C.c_ubyte*56)]
class Clock(C.Structure):_fields_=[('active',C.c_int16),('reserved02',C.c_ubyte*14),('tempo',C.c_uint32)]
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest();pack=lambda v:struct.pack('<I',v&0xffffffff)
with TemporaryDirectory(prefix='rr5ed34-')as td:
 d=Path(td);head='#include <stdint.h>\n#include <stddef.h>\n#include "rr/cpu_md_leaf.h"\n#include "rr/layout.h"\n#undef RR_ASSERT_SIZE\n#undef RR_ASSERT_OFFSET\n#define RR_ASSERT_SIZE(t,n)\n#define RR_ASSERT_OFFSET(t,f,n)\n#include "rr/game_motion.h"\ntypedef RrMotionControllerPrefix RrScrollController;\ntypedef RrMotionClock20 RrScrollClock20;\nstruct {u8 before[16];RrScrollController value;u8 after[16];} root;\nstruct {u8 before[16];RrScrollClock20 value;u8 after[16];} clockOwner;\nstruct {u8 before[16];RrScrollKey4 value[8];u8 after[16];} keys;\nRrScrollController *D_800ACA28;\n'
 tail='\nvoid *owner(int n){return n==0?(void*)&root.value:n==1?(void*)&clockOwner.value:(void*)keys.value;}\nvoid run(void){D_800ACA28=&root.value;func_8005ED34(&clockOwner.value);}\nsize_t root_size(void){return sizeof root.value;}\n'
 (d/'test.c').write_text(head+body+tail);subprocess.run(['cc','-std=gnu11','-O2','-shared','-fPIC','-funsigned-char','-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-Iinclude','-I'+str(P),str(d/'test.c'),'-o',str(d/'test.dylib')],check=True)
 lib=C.CDLL(str(d/'test.dylib'));lib.owner.restype=C.c_void_p;lib.root_size.restype=C.c_size_t;N=[lib.owner(i)for i in range(3)];assert all(n>0xffffffff for n in N);assert lib.root_size()==C.sizeof(Root)
 u=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);u.mem_map(0,0x400000);exe=Path('assets/PAL/main.exe').read_bytes();u.mem_write(0x10000,exe[0x800:])
 elf=os.getenv('RR_COMPILED_ELF')
 if elf:
  from elftools.elf.elffile import ELFFile
  with open(elf,'rb')as f:
   e=ELFFile(f);sec=e.get_section_by_name('.text');u.mem_write(sec['sh_addr']&0x1fffffff,sec.data())
 write=lambda a,b:u.mem_write(a&0x1fffffff,b);read=lambda a,n:bytes(u.mem_read(a&0x1fffffff,n));A=[0x80180000,0x80190000,0x801A0000];sizes=[0x188,20,32];ctx=u.context_save();rng=random.Random(0x5ed34);edges=[0,1,2,3,7,127,128,255,256,65535,65536,0x7fffffff,0x80000000,0xffffffff]
 def project():
  r=Root.from_address(N[0]);t=r.scroll;assert t.keys==N[2]
  b=C.string_at(N[0],0x134)+b''.join(pack(getattr(t,n))for n in ['state','disabled','index','hold','wait'])+pack(A[2])+C.string_at(N[0]+Root.scroll.offset+Track.channel.offset,8)+pack(t.reserved20)+pack(t.step)+bytes(r.motionTail)[:44]
  assert len(b)==0x188;return b
 for case in range(count):
  u.context_restore(ctx)
  for n,size in zip(N,[C.sizeof(Root),20,32]):C.memmove(n-16,bytes([0xa5])*(size+32),size+32)
  r=Root.from_address(N[0]);t=r.scroll;c=Clock.from_address(N[1]);ks=(Key*8).from_address(N[2]);r.speed4C=edges[case%len(edges)]if case%3 else rng.getrandbits(32);c.tempo=edges[(case//len(edges))%len(edges)]if case%5 else rng.getrandbits(32);c.active=[-32768,-1,0,1,32767][case%5]
  t.state=[-2147483648,-1,0,1,2,3,2147483647][(case//5)%7];t.disabled=[0,0,0,1,-1][(case//35)%5];t.index=case%7;t.hold=C.c_int32(edges[(case//7)%len(edges)]).value;t.wait=C.c_int32(edges[(case//13)%len(edges)]).value;t.keys=N[2];t.step=C.c_int32(rng.getrandbits(32)).value
  for j in range(2):t.channel[j].glyph=[-32768,-1,0,1,32767][(case+j)%5];t.channel[j].offset=rng.getrandbits(16)
  for j in range(8):ks[j].glyph=[-32768,-1,0,1,32767][(case+j)%5];ks[j].timing=rng.getrandbits(16)
  initialRoot=C.string_at(N[0],C.sizeof(Root));write(A[0]-16,b'\xa5'*16+project()+b'\xa5'*16);write(A[1]-16,C.string_at(N[1]-16,52));write(A[2]-16,C.string_at(N[2]-16,64));write(0x800ACA28,pack(A[0]));write(0x803df000,b'\x5a'*0x2000)
  for reg in range(UC_MIPS_REG_1,UC_MIPS_REG_31+1):u.reg_write(reg,(0x5aa50000+case*31+reg)&0xffffffff)
  u.reg_write(UC_MIPS_REG_A0,A[1]);u.reg_write(UC_MIPS_REG_SP,0x803e0000);u.reg_write(UC_MIPS_REG_RA,0x803ff000)
  saved={q:u.reg_read(q)for q in [*range(UC_MIPS_REG_S0,UC_MIPS_REG_S7+1),UC_MIPS_REG_GP,UC_MIPS_REG_SP,UC_MIPS_REG_FP,UC_MIPS_REG_RA]}
  u.emu_start(0x8005ED34,0x803ff000,count=5000);assert u.reg_read(UC_MIPS_REG_PC)==0x803ff000,case;assert all(u.reg_read(q)==v for q,v in saved.items()),('callee saves',case)
  lib.run();assert read(A[0]-16,0x188+32)==b'\xa5'*16+project()+b'\xa5'*16,('root',case)
  for i in [1,2]:assert read(A[i]-16,sizes[i]+32)==C.string_at(N[i]-16,sizes[i]+32),('readonly',case,i)
  assert C.string_at(N[0]-16,16)==b'\xa5'*16 and C.string_at(N[0]+C.sizeof(Root),16)==b'\xa5'*16
  # Native layout padding is guard storage, not part of the target projection.
  for a,b in [(0x134,Root.scroll.offset),(Root.scroll.offset+20,Root.scroll.offset+Track.keys.offset),(Root.motionTail.offset+44,C.sizeof(Root))]:assert C.string_at(N[0]+a,b-a)==initialRoot[a:b]
  if case and case%1024==0:print(case,'cases',flush=True)
 result=dict(cases=count,engine=('compiled'if elf else'original')+' MIPS/native C',source_sha256=sha(source),header_sha256=sha(ROOT/'include/rr/game_motion.h'),md_header_sha256=sha(ROOT/'include/rr/cpu_md_leaf.h'),original_exe_sha256=sha(ROOT/'assets/PAL/main.exe'),test_sha256=sha(__file__),elf_sha256=sha(elf)if elf else None,full_host_pointers=True,guarded_complete_owners=True,callee_saves_stack_return=True,readonly_clock_and_keys=True,unsigned_product_and_division_domains=True)
 if os.getenv('RR_AUDIT_JSON'):Path(os.environ['RR_AUDIT_JSON']).write_text(json.dumps(result,indent=2)+'\n')
 print(count,result['engine'],'cases passed',flush=True)
