"""Native full pointers against original or candidate MIPS, with canonical wire serialization."""
from pathlib import Path
import sys,ctypes as C,struct,subprocess,os,json,hashlib
from unicorn import Uc,UC_ARCH_MIPS,UC_MODE_MIPS32,UC_MODE_LITTLE_ENDIAN
from unicorn.mips_const import *
root=next(x for x in Path(__file__).resolve().parents if (x/'tools/scripts/cc.sh').is_file());os.chdir(root)
fixture=root/'tools/scripts/test_game_motion_native.py'
p=Path(os.getenv('RR_TEST_DIR','build/goal_50/part34/mips_native'));p.mkdir(parents=True,exist_ok=True)
ns={'__file__':str(fixture)};script=fixture.read_text();exec(script[:script.index('with TemporaryDirectory()')],ns);src=ns['src']
setup=src[src.index('u32 seed=i*0x9e3779b9U;',src.index('int main')):src.index(' if(t->state==0',src.index('int main'))]
validation=src[src.index(' if(t->state==0',src.index('int main')):src.index('reference=actual;',src.index('int main'))].replace('continue;','return 0;')
src=src[:src.index('int main')]+'''static unsigned currentIndex;
int setup(unsigned i){currentIndex=i;'''+setup+validation+'''D_800ACA28=&actual.controller;return 1;}
void run(void){func_8005AC0C(&clock);}
void snapshot(u8 *out){RrMotionTrack44 *t=&actual.controller.track;memcpy(out,&actual.controller,0x15C);out+=0x15C;
#define PUT(f,n) memcpy(out,&t->f,n);out+=n;
 PUT(state,4);PUT(disabled,4);PUT(word08,4);u32 key=0x80200000+32*(currentIndex%4);memcpy(out,&key,4);out+=4;
 PUT(elapsed,2);PUT(duration,2);PUT(position,8);PUT(scale,4);PUT(word20,4);PUT(word24,4);PUT(velocity,4);
}
void *get_clock(void){return &clock;}void *get_key(void){return &keys[currentIndex%4];}
'''
(p/'mips_native.c').write_text(src)
subprocess.run(['cc','-std=gnu11','-O2','-shared','-fPIC','-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-Iinclude','-I'+str(p),str(p/'mips_native.c'),'-o',str(p/'mips_native.dylib')],check=True)
lib=C.CDLL(str(p/'mips_native.dylib'));lib.get_clock.restype=C.c_void_p;lib.get_key.restype=C.c_void_p
u=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);u.mem_map(0,0x400000)
u.mem_write(0x10000,Path('assets/PAL/main.exe').read_bytes()[0x800:])
if os.getenv('RR_COMPILED_ELF'):
 from elftools.elf.elffile import ELFFile
 with open(os.environ['RR_COMPILED_ELF'],'rb') as f:u.mem_write(0x5AC0C,ELFFile(f).get_section_by_name('.text').data())
buf=(C.c_ubyte*0x188)();count=0
serials=int(os.getenv("RR_CASES","32768"))
for serial in range(serials):
 i=serial if serial<1024 else (serial*0x9e3779b9)&0xFFFFF
 if not lib.setup(i):continue
 lib.snapshot(buf);before=bytes(buf);clock=C.string_at(lib.get_clock(),20);key=C.string_at(lib.get_key(),32)
 u.mem_write(0x17FFF0,b'\xC7'*16);u.mem_write(0x180188,b'\xD9'*16);u.mem_write(0x180000,before);u.mem_write(0x1A0000,clock);u.mem_write(0x200000+32*(i%4),key);u.mem_write(0xACA28,struct.pack('<I',0x80180000))
 for reg in range(UC_MIPS_REG_0,UC_MIPS_REG_31+1):u.reg_write(reg,0x5A5AA5A5)
 u.reg_write(UC_MIPS_REG_HI,0x13579BDF);u.reg_write(UC_MIPS_REG_LO,0x2468ACE0);u.reg_write(UC_MIPS_REG_A0,0x801A0000);u.reg_write(UC_MIPS_REG_RA,0x802F0000);u.reg_write(UC_MIPS_REG_SP,0x80300000)
 u.emu_start(0x8005AC0C,0x802F0000,count=1000)
 assert u.reg_read(UC_MIPS_REG_PC)==0x802F0000,(i,hex(u.reg_read(UC_MIPS_REG_PC)))
 lib.run();lib.snapshot(buf)
 got=bytes(u.mem_read(0x180000,0x188));want=bytes(buf)
 assert got==want,(i,got[0x15C:].hex(),want[0x15C:].hex())
 assert bytes(u.mem_read(0x1A0000,20))==clock and bytes(u.mem_read(0x200000+32*(i%4),32))==key
 assert bytes(u.mem_read(0x17FFF0,16))==b'\xC7'*16 and bytes(u.mem_read(0x180188,16))==b'\xD9'*16
 count+=1
print(count,'cases;', 'compiled MIPS' if os.getenv('RR_COMPILED_ELF') else 'original MIPS','versus full native pointer owners')

if os.getenv('RR_AUDIT_JSON'):
 source=Path(sys.argv[1]) if len(sys.argv)>1 else root/'build/legacy/func_8005AC0C.c'
 sha=lambda f:hashlib.sha256(Path(f).read_bytes()).hexdigest()
 report=dict(source=str(source),source_sha256=sha(source),test_sha256=sha(__file__),fixture_sha256=sha(fixture),header_sha256=sha(root/'include/rr/game_motion.h'),cases=count,serial_cases=serials,engine='compiled MIPS' if os.getenv('RR_COMPILED_ELF') else 'original MIPS',full_host_pointers=True,guarded_controller=True,unchanged_clock_and_key=True,poisoned_caller_saved_and_hi_lo=True,sanitizers=['undefined','bounds'])
 if os.getenv('RR_COMPILED_ELF'):report['elf_sha256']=sha(os.environ['RR_COMPILED_ELF'])
 Path(os.environ['RR_AUDIT_JSON']).write_text(json.dumps(report,indent=2)+'\n')
