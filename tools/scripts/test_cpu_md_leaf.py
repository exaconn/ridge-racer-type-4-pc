"""Typed HI/LO leaf primitives against Python arithmetic and compiled MIPS/Unicorn."""
from pathlib import Path
import hashlib,json,os,random,struct,subprocess
from unicorn import Uc,UC_ARCH_MIPS,UC_MODE_MIPS32,UC_MODE_LITTLE_ENDIAN,UC_HOOK_MEM_WRITE
from unicorn.mips_const import *
from elftools.elf.elffile import ELFFile
ROOT=Path(__file__).resolve().parents[2]
os.chdir(ROOT)
stage=Path(os.environ.get('RR_MD_WORKDIR','build/cpu_md_leaf_test'));stage.mkdir(parents=True,exist_ok=True)
C=r'''
#include "rr/cpu_md_leaf.h"
void rr_md_probe(u32 *w){u32 a=w[0],b=w[1],acc=w[2],add=w[3],den=w[4],out,coefficient=0x51EB0000U;
 RR_MD_MULT_S(a,b);RR_MD_LO(w[5]);RR_MD_HI(w[6]);
 RR_MD_MULT_U(a,b);RR_MD_LO(w[7]);RR_MD_HI(w[8]);
 RR_MD_MULT_S_FINISH16(a,coefficient,0x851F);w[9]=coefficient;RR_MD_LO(w[10]);RR_MD_HI(w[11]);
 RR_MD_MULT_S(a,b);RR_MD_LO_ACCUM_SHL2(out,acc,add);w[12]=out;w[13]=acc;
 RR_MD_DIV_S(out,(s32)a,(s32)den);w[14]=out;RR_MD_LO(w[15]);RR_MD_HI(w[16]);
}
#if !defined(__mips__)
#include <stdio.h>
#include <string.h>
typedef struct {u32 pre[4],words[17],post[4];} World;
int main(void){World w;u32 before[5];assert(sizeof(World)==100 && sizeof(u32)==4);
 while(fread(&w,1,sizeof w,stdin)==sizeof w){memcpy(before,w.words,20);rr_md_probe(w.words);assert(!memcmp(before,w.words,20));if(fwrite(&w,1,sizeof w,stdout)!=sizeof w)return 2;}return 0;}
#endif
'''
source=stage/'probe.c';obj=stage/'probe.o';elf=stage/'probe.elf';ld=stage/'probe.ld';native_exe=stage/'native'
source.write_text(C)
subprocess.run(['tools/scripts/cc.sh',str(source),str(obj)],check=True,capture_output=True)
ld.write_text('SECTIONS {.text 0x80020000 : SUBALIGN(4) {*(.text)}}\n')
subprocess.run(['tools/toolchain/bin/mipsel-none-elf-ld','-EL','-T',str(ld),'-o',str(elf),str(obj)],check=True,capture_output=True)
subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror','-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-Iinclude',str(source),'-o',str(native_exe)],check=True)
rng=random.Random(0x48494C4F);limits=[0,1,0xFFFFFFFF,0x7FFFFFFF,0x80000000,2,0xFFFFFFFE,0x10000,0xFFFF,0x51EB851F,100,0x80000001]
mask=0xFFFFFFFF;sign=lambda x:x-(1<<32) if x&(1<<31) else x
cases=[];expected=[]
for i in range(8192):
 words=[rng.randrange(1<<32) for _ in range(25)];a=limits[i%12] if i<4096 else words[4];b=limits[i//12%12] if i<4096 else words[5]
 den=limits[i//144%12] if i<4096 else words[8]
 if not den or (a==0x80000000 and den==mask):den=1
 words[4:9]=[a,b,words[6],words[7],den];acc,add=words[6:8]
 cases.append(struct.pack('<25I',*words));v=words.copy();p=(sign(a)*sign(b))&((1<<64)-1);u=a*b;coef=0x51EB851F;f=(sign(a)*coef)&((1<<64)-1)
 q=abs(sign(a))//abs(sign(den));q=-q if (sign(a)<0)!=(sign(den)<0) else q;rem=sign(a)-q*sign(den)
 v[9:21]=[p&mask,p>>32,u&mask,u>>32,coef,f&mask,f>>32,p&mask,((acc+add)<<2)&mask,q&mask,q&mask,rem&mask]
 expected.append(struct.pack('<25I',*v))
native=subprocess.run([str(native_exe)],input=b''.join(cases),capture_output=True,check=True).stdout
assert native==b''.join(expected),'native primitives differ from independent Python arithmetic'
with elf.open('rb') as f:
 e=ELFFile(f);section=e.get_section_by_name('.text');code=section.data();start=section['sh_addr']
 assert not [s.name for s in e.get_section_by_name('.symtab').iter_symbols() if s.name and s['st_shndx']=='SHN_UNDEF']
uc=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);uc.mem_map(0,0x200000)
write=lambda a,b:uc.mem_write(a&0x1fffffff,bytes(b));read=lambda a,n:bytes(uc.mem_read(a&0x1fffffff,n))
world=0x800D0000;arg=world+16;sp=0x801F0000;stop=0x80010000;write(start,code)
def store(cpu,access,a,n,v,user):
 a&=0x1fffffff
 assert n==4 and ((arg&0x1fffffff)+20<=a and a+4<=(arg&0x1fffffff)+68 or (sp&0x1fffffff)-256<=a and a+4<=(sp&0x1fffffff)+16),hex(a)
uc.hook_add(UC_HOOK_MEM_WRITE,store)
initial_context=uc.context_save()
for i,(data,want) in enumerate(zip(cases,expected)):
 uc.context_restore(initial_context)
 write(world,data)
 for r in range(1,32):uc.reg_write(UC_MIPS_REG_0+r,0xBEEF0000+r)
 uc.reg_write(UC_MIPS_REG_A0,arg);uc.reg_write(UC_MIPS_REG_SP,sp);uc.reg_write(UC_MIPS_REG_RA,stop)
 uc.reg_write(UC_MIPS_REG_HI,0x12345678);uc.reg_write(UC_MIPS_REG_LO,0x87654321)
 uc.emu_start(start,stop,count=1000)
 assert uc.reg_read(UC_MIPS_REG_PC)==stop and uc.reg_read(UC_MIPS_REG_SP)==sp and uc.reg_read(UC_MIPS_REG_RA)==stop,i
 assert read(world,100)==want,(i,[(j,a,b) for j,(a,b) in enumerate(zip(struct.unpack("<25I",read(world,100)),struct.unpack("<25I",want))) if a!=b])
 for r in list(range(16,24))+[28,30]:assert uc.reg_read(UC_MIPS_REG_0+r)==0xBEEF0000+r,(i,r)
 assert uc.reg_read(UC_MIPS_REG_LO)==struct.unpack_from('<I',want,16+15*4)[0] and uc.reg_read(UC_MIPS_REG_HI)==struct.unpack_from('<I',want,16+16*4)[0],i
sha=lambda f:hashlib.sha256(Path(f).read_bytes()).hexdigest()
audit={'cases':8192,'reference':'independent Python signed/unsigned64 arithmetic, native C and compiled MIPS/Unicorn','header_sha256':sha('include/rr/cpu_md_leaf.h'),'test_sha256':sha(__file__),'fixture_sha256':sha(source),'elf_sha256':sha(elf),'full_guards_and_inputs':True,'hi_lo_state_outputs':True,'callee_saves_and_stack':True,'sanitizers':['undefined','bounds'],'valid_signed_division_only':True,'operations':['signed MULT','unsigned MULTU','HI/LO transfers','coefficient finish16 and signed MULT','LO plus wrapping accumulate/shift','checked signed DIV quotient and remainder']}
(stage/'verified.json').write_text(json.dumps(audit,indent=2)+'\n')
print('PASS 8192 HI/LO vectors: Python, native C and compiled MIPS/Unicorn; complete guarded state and ABI preserved')
