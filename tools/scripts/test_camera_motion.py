"""Camera motion controller: native C arithmetic against original and linked MIPS."""
from pathlib import Path
from tempfile import TemporaryDirectory
import subprocess,struct,random,sys,json,hashlib,os
from unicorn import Uc,UC_ARCH_MIPS,UC_MODE_MIPS32,UC_MODE_LITTLE_ENDIAN
from elftools.elf.elffile import ELFFile
from unicorn.mips_const import *
sys.path.insert(0,'tools/scripts');from mips_preview_engine import Mips,signed
ROOT=Path(__file__).resolve().parents[2];os.chdir(ROOT)
p=ROOT/'include/rr';src=ROOT/'build/legacy/func_8005B9F4.c';rng=random.Random(0x5B9F4);cases=[];profiles={}
for i in range(8192):
 b=bytearray(rng.randbytes(1484));base=16;motion=base+0x74
 put=lambda off,fmt,v:struct.pack_into('<'+fmt,b,off,v)
 mode=[0,1,2,-1][i%4];phase=[0,1,2,-1][i//4%4];active=[0,1,-1][i//16%3]
 scale=[0,1,-1,100,720,2147483647,-2147483648][i//48%7] if i<4096 else signed(rng.randrange(1<<32))
 timing=[0,1,149,150,151,200,2147483647,-2147483648][i//336%8] if i<4096 else signed(rng.randrange(1<<32))
 denominator=signed((timing*2-300)*scale)
 if mode in [0,1] and phase==0 and active and denominator==0:scale=1;timing=151
 put(base+0x4c,'i',scale);put(motion,'i',phase);put(motion+4,'i',mode);put(motion+8,'i',i//64%3);put(motion+0xc,'I',0x800e0000)
 put(motion+0x50,'H',[0,1,32767,32768,65535][i//128%5]);put(motion+0x52,'h',[0,1,-1,32767,-32768][i//640%5])
 put(1416,'h',active);put(1416+0x10,'i',timing)
 cases.append(bytes(b));profile=str((mode,phase,active!=0));profiles[profile]=profiles.get(profile,0)+1
c_head=r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "rr/layout.h"
#undef RR_ASSERT_SIZE
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_SIZE(t,n) _Static_assert(sizeof(void*)!=4 || sizeof(t)==n,"PAL size")
#define RR_ASSERT_OFFSET(t,f,n) _Static_assert(sizeof(void*)!=4 || offsetof(t,f)==n,"PAL offset")
#include "rr/camera_motion.h"
RrCameraRootPrefix D_800AC650;
typedef struct {u8 pre[16];RrCameraObjectPrefix object;u8 remaining[0x484];u8 post[16];} World;
static World world;
static RrCameraKey16 keys[3];
'''
c_tail=r'''
int main(void){u8 data[1484],before[sizeof(World)],allowed[sizeof(World)];RrCameraMotionInput14 input;unsigned i,j;
 _Static_assert(sizeof(keys)==48 && sizeof(input)==20,"native key/input sizes");
 _Static_assert(sizeof(void*)==8 && sizeof(unsigned long)==sizeof(void*),"full native pointers");
 memset(allowed,0,sizeof allowed);
 #define ALLOW(field) memset(allowed+offsetof(World,object.motion74.field),1,sizeof(world.object.motion74.field))
 ALLOW(phase00);ALLOW(key08);ALLOW(start10);ALLOW(target20);ALLOW(delta30);ALLOW(current40);ALLOW(counter50);ALLOW(horizon52);ALLOW(angles64);
 while(fread(data,1,sizeof data,stdin)==sizeof data){
  memset(&world,0xA5,sizeof world);memcpy(world.pre,data,16);memcpy(&world.object,data+16,0x74);
  memcpy(&world.object.motion74,data+16+0x74,12);world.object.motion74.keys0C=keys;
  memcpy(world.object.motion74.start10,data+16+0x84,0x60);
  memcpy(world.remaining,data+16+0xE4,0x484);memcpy(world.post,data+1400,16);
  memcpy(&input,data+1416,20);memcpy(keys,data+1436,48);D_800AC650.camera3D8=&world.object;
  assert((uintptr_t)&world.object>UINT32_MAX && (uintptr_t)keys>UINT32_MAX && (uintptr_t)&input>UINT32_MAX);
  memcpy(before,&world,sizeof world);func_8005B9F4(&input);
  assert(D_800AC650.camera3D8==&world.object && world.object.motion74.keys0C==keys);
  assert(!memcmp(&input,data+1416,20) && !memcmp(keys,data+1436,48));
  for(i=0;i<sizeof world;i++)if(!allowed[i])assert(((u8*)&world)[i]==before[i]);
  memcpy(data,world.pre,16);memcpy(data+16,&world.object,0x74);
  memcpy(data+16+0x74,&world.object.motion74,12);memcpy(data+16+0x84,world.object.motion74.start10,0x60);
  memcpy(data+16+0xE4,world.remaining,0x484);memcpy(data+1400,world.post,16);
  j=fwrite(data,1,1416,stdout);if(j!=1416)return 2;
 }return 0;
}
'''
with TemporaryDirectory() as tmp:
 c=Path(tmp)/'test.c';exe=Path(tmp)/'test';c.write_text(c_head+'\n'+__import__('re').sub(r' __asm__\("\$\d+"\)','',src.read_text())+c_tail)
 subprocess.run(['cc','-std=gnu11','-O2','-Wall','-Wextra','-Werror','-Wno-implicit-fallthrough','-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-Iinclude','-I'+str(p),str(c),'-o',str(exe)],check=True)
 r=subprocess.run([str(exe)],input=b''.join(cases),capture_output=True)
 assert r.returncode==0,r.stderr.decode();native=r.stdout
assert len(native)==8192*1416
class Cpu(Mips):
 def step(self):
  w=self.load(self.pc,4)
  if w>>26==0 and w&63==26:
   a=signed(self.r[(w>>21)&31]);b=signed(self.r[(w>>16)&31]);assert b!=0 and not(a==-2147483648 and b==-1)
   q=abs(a)//abs(b);q=-q if (a<0)!=(b<0) else q;self.lo=q&0xffffffff;self.hi=(a-q*b)&0xffffffff
   destination=self.delay;self.delay=None;self.pc=(self.pc+4 if destination is None else destination)&0xffffffff;return
  if w>>26!=1:return super().step()
  assert self.delay is None;rs=(w>>21)&31;rt=(w>>16)&31;assert rt in [0,1]
  take=signed(self.r[rs])<0 if rt==0 else signed(self.r[rs])>=0
  self.delay=(self.pc+4+(signed(w,16)<<2))&0xffffffff if take else self.pc+8;self.pc+=4
def verify(code,label):
 uc=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);uc.mem_map(0,0x200000)
 write=lambda a,b:uc.mem_write(a&0x1fffffff,bytes(b));read=lambda a,n:bytes(uc.mem_read(a&0x1fffffff,n))
 start=0x8005b9f4;obj=0x800d0010;input=0x800f8000;sp=0x801f0000;stop=0x80010000;write(start,code)
 allowed={0:4,8:4,0x10:4,0x14:4,0x18:4,0x20:4,0x24:4,0x28:4,0x30:4,0x34:4,0x38:4,0x40:4,0x44:4,0x48:4,0x50:2,0x52:2,0x64:2,0x66:2,0x68:2}
 def store(cpu,access,a,n,v,user):assert allowed.get((a&0x1fffffff)-(obj&0x1fffffff)-0x74)==n,(hex(a),n)
 cpu=Cpu(uc,0,100,101,102,store)
 # Validate the local signed DIV extension against a straight Unicorn block;
 # isolated instructions avoid the previously documented branch translation issue.
 divuc=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);divuc.mem_map(0,4096)
 block=struct.pack('<III',(4<<21)|(5<<16)|26,(2<<11)|18,(3<<11)|16)
 divuc.mem_write(0,block);write(0x80000800,block)
 checks=random.Random(0xD1483D8)
 for j in range(1024):
  a=checks.randrange(0x100000000);b=checks.randrange(1,0x100000000)
  divuc.reg_write(UC_MIPS_REG_A0,a);divuc.reg_write(UC_MIPS_REG_A1,b);divuc.emu_start(0,12,count=3)
  cpu.reset();cpu.r[4]=a;cpu.r[5]=b;cpu.pc=0x80000800
  for n in range(3):cpu.step()
  assert cpu.r[2]==divuc.reg_read(UC_MIPS_REG_V0) and cpu.r[3]==divuc.reg_read(UC_MIPS_REG_V1)
 for i,b in enumerate(cases):
  write(obj-16,b[:1416]);write(input,b[1416:1436]);write(0x800e0000,b[1436:]);write(0x800aca28,struct.pack('<I',obj))
  cpu.reset();cpu.r=[0]+[0xbeef0000+r for r in range(1,32)];cpu.r[4]=input;cpu.r[29]=sp;cpu.r[31]=stop;cpu.pc=start;cpu.hi=0x12345678;cpu.lo=0x87654321
  for step in range(1000):
   if cpu.pc==stop:break
   cpu.step()
  assert cpu.pc==stop and cpu.r[29]==sp and cpu.r[31]==stop,(i,hex(cpu.pc))
  actual=read(obj-16,1416);expected=native[i*1416:(i+1)*1416]
  assert actual==expected,(label,i,[(hex(j-16),a,b) for j,(a,b) in enumerate(zip(actual,expected)) if a!=b])
  assert read(input,20)==b[1416:1436] and read(0x800e0000,48)==b[1436:]
  for r in list(range(16,24))+[28,30]:assert cpu.r[r]==0xbeef0000+r
 print('PASS',label,8192,'whole guarded owner, readonly input/key tables, exact stores, native pointer width, UBSan/bounds')
 return {'cases':8192,'code_sha256':hashlib.sha256(code).hexdigest()}
a=verify(Path('assets/PAL/main.exe').read_bytes()[0x4c1f4:0x4c824],'original MIPS/native')
elf=os.environ.get('RR_COMPILED_ELF');b=None
if elf:
 with open(elf,'rb') as f:
  e=ELFFile(f);section=e.get_section_by_name('.text');code=section.data();assert section['sh_addr']==0x8005B9F4
 b=verify(code,'compiled MIPS/native')
sha=lambda f:hashlib.sha256(Path(f).read_bytes()).hexdigest()
audit={'original':a,'compiled':b,'source_sha256':sha(src),'header_sha256':sha(p/'camera_motion.h'),'md_header_sha256':sha(p/'cpu_md_leaf.h'),'test_sha256':sha(__file__),'engine_sha256':sha('tools/scripts/mips_preview_engine.py'),'div_extension_unicorn_blocks':1024,'full_owner_guards':True,'full_host_pointer_addresses':True,'exact_store_bounds':True,'sanitizers':['undefined','bounds'],'profiles':profiles,'contract':'Disjoint mutable camera, readonly accessible key table and input. Initial key0..2; nonzero signed division and no INT_MIN/-1 trap. Integer architectural emulator; no physical timing model.'}
if elf:audit['elf_sha256']=sha(elf)
if os.environ.get('RR_AUDIT_JSON'):Path(os.environ['RR_AUDIT_JSON']).write_text(json.dumps(audit,indent=2)+'\n')
