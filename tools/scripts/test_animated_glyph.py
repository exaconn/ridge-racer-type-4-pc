"""Original/compiled MIPS versus native animated sprite and DMA-owner oracle."""
from pathlib import Path
import ctypes as C,subprocess,struct,random,sys,os,re,json,hashlib
from unicorn import Uc,UC_ARCH_MIPS,UC_MODE_MIPS32,UC_MODE_LITTLE_ENDIAN,UC_HOOK_CODE
from unicorn.mips_const import *
root=next(p for p in Path(__file__).resolve().parents if (p/'tools/scripts/cc.sh').is_file());os.chdir(root)
P=Path(os.getenv('RR_TEST_DIR','build/tests/animated_glyph'));P.mkdir(parents=True,exist_ok=True)
source=Path(sys.argv[1]) if len(sys.argv)>1 else root/'build/legacy/func_8001F808.c'
n=int(sys.argv[2]) if len(sys.argv)>2 else 65536
s=r'''
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "rr/layout.h"
#undef RR_ASSERT_SIZE
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_SIZE(t,n)
#define RR_ASSERT_OFFSET(t,f,n)
#include "rr/animated_sprite_grid.h"
static RrGpuScratchPointers nativeScratch;
#define RR_GLYPH_ANIMATION_SCRATCH (&nativeScratch)
#include "rr/animated_glyph.h"
#include "rr/frame_storage_types.h"
static RrFrameStorage frames[2];
RrIconSpriteContext *D_800ACC14;
static u8 *allocation,*arena,*initialCursor;
static s16 animation[2];
static u32 seed,calls;
static u32 mix(u32 x){return x*1664525U+1013904223U;}
static u8 *stream(u32 x){return arena+(x%3)*4096;}
static void mutate(u32 event){
 u32 x=mix(seed+event*73U);
 D_800ACC14=(RrIconSpriteContext*)&frames[(x>>3)&1];
 nativeScratch.cursor=stream((x>>4)%3)+128+event*64;
 nativeScratch.order=(u32*)&frames[(x>>7)&1].bytes[0xB70];
 if(event==2 && (seed&1))animation[0]=(s16)(x>>8);
}
void func_80095B94(void *output){
 assert(calls++==0);assert(output==initialCursor);
 ((u8*)output)[3]=4;((u8*)output)[7]=0x64;mutate(1);
}
void func_80095A44(u32 *ordering,u32 *output){
 assert(calls==1||calls==3);assert(ordering==&D_800ACC14->head70);
 assert((u8*)output==initialCursor+(calls==1?0:20));
 u32 tag;memcpy(&tag,output,4);tag=(tag&0xFF000000U)|(*ordering&0xFFFFFFU);memcpy(output,&tag,4);
 *ordering=(*ordering&0xFF000000U)|((uintptr_t)output&0xFFFFFFU);
 mutate(++calls);
}
void func_80093C24(void *output,s32 a,s32 b,s32 page,RrGpuRectangleFields *window){
 assert(calls++==2);assert(output==initialCursor+20&&a==0&&b==1&&window==0);
 u32 words[3]={0x02000000U,0xE1000000U|((u32)page&0xFFFFU),0};memcpy(output,words,12);mutate(3);
}
'''
s+=re.sub(r'^#include[^\n]*\n','',source.read_text(),count=1,flags=re.M)
s+=r'''
void setup(u32 caseSeed,s16 timer,u32 head0,u32 head1){
 if(!allocation){assert(!posix_memalign((void**)&allocation,0x1000000,0x1000000));arena=allocation+0x200000;assert((uintptr_t)arena>UINT32_MAX);}
 seed=caseSeed;calls=0;memset(frames,0xA5,sizeof(frames));memset(arena,0x5A,12288);
 frames[0].ordering.ordering70[0]=head0;frames[1].ordering.ordering70[0]=head1;
 D_800ACC14=(RrIconSpriteContext*)&frames[seed&1];
 initialCursor=stream((seed>>1)%3)+64;nativeScratch.cursor=initialCursor;
 nativeScratch.order=(u32*)&frames[(seed>>3)&1].bytes[0x70];
 animation[0]=timer;animation[1]=(s16)0xA5B6;assert((uintptr_t)animation>UINT32_MAX);
}
void run(s32 x,s32 y,s32 glyph,s32 clut){func_8001F808(x,y,glyph,clut,animation);assert(calls==4&&animation[1]==(s16)0xA5B6);}
void *packet_bytes(void){return arena;}
void *frame_bytes(u32 bank){return &frames[bank&1];}
u32 cursor_offset(void){return nativeScratch.cursor-arena;}
u32 order_offset(void){return (u8*)nativeScratch.order-(u8*)frames;}
u32 root_index(void){return ((RrFrameStorage*)D_800ACC14)-frames;}
u32 animation_word(void){u32 word;memcpy(&word,animation,4);return word;}
'''
(P/'native.c').write_text(s)
subprocess.run(['cc','-std=gnu11','-O2','-funsigned-char','-Iinclude','-I'+str(P),'-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-dynamiclib',str(P/'native.c'),'-o',str(P/'native.dylib')],check=True)
lib=C.CDLL(str((P/'native.dylib').resolve()));lib.setup.argtypes=[C.c_uint32,C.c_int16,C.c_uint32,C.c_uint32];lib.run.argtypes=[C.c_int32]*4
for name in ['packet_bytes','frame_bytes']:getattr(lib,name).restype=C.c_void_p
lib.animation_word.restype=C.c_uint32
u=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);u.mem_map(0,0x400000);u.mem_map(0x1f800000,0x1000)
main=Path('assets/PAL/main.exe').read_bytes();u.mem_write(0x10000,main[0x800:])
if os.environ.get('RR_COMPILED_ELF'):
 from elftools.elf.elffile import ELFFile
 with open(os.environ['RR_COMPILED_ELF'],'rb') as f:
  e=ELFFile(f);sec=e.get_section_by_name('.text');u.mem_write(sec['sh_addr']&0x1fffffff,sec.data())
def w(a,data):u.mem_write(a&0x1fffffff,data)
def r(a,n):return bytes(u.mem_read(a&0x1fffffff,n))
def lw(a):return struct.unpack('<I',r(a,4))[0]
def sw(a,x):w(a,struct.pack('<I',x&0xffffffff))
def reg(i):return u.reg_read(i)&0xffffffff
def mix(x):return (x*1664525+1013904223)&0xffffffff
def stream(x):return 0x80200000+(x%3)*4096
def mutate(event):
 x=mix(seed+event*73);sw(0x800acc14,0x80130000+((x>>3)&1)*0x30000);sw(0x1f800000,stream((x>>4)%3)+128+event*64);sw(0x1f800004,0x80130b70+((x>>7)&1)*0x30000)
 if event==2 and seed&1:w(0x80210000,struct.pack('<H',(x>>8)&65535))
service_counts={'sprite_init':0,'link':0,'draw_mode':0}
def hook(uc,addr,size,data):
 global calls
 ret=reg(UC_MIPS_REG_RA)
 a0,a1,a2,a3=map(reg,[UC_MIPS_REG_A0,UC_MIPS_REG_A1,UC_MIPS_REG_A2,UC_MIPS_REG_A3])
 if addr==0x80095b94:
  service_counts['sprite_init']+=1
  assert calls==0 and a0==initial;calls+=1;w(a0+3,b'\x04');w(a0+7,b'\x64');mutate(1)
 elif addr==0x80095a44:
  service_counts['link']+=1
  assert calls in [1,3] and a0==lw(0x800acc14)+0x70 and a1==initial+(0 if calls==1 else 20)
  sw(a1,(lw(a1)&0xff000000)|(lw(a0)&0xffffff));sw(a0,(lw(a0)&0xff000000)|(a1&0xffffff));calls+=1;mutate(calls)
 elif addr==0x80093c24:
  service_counts['draw_mode']+=1
  assert calls==2 and a0==initial+20 and a1==0 and a2==1 and lw(reg(UC_MIPS_REG_SP)+16)==0;calls+=1
  w(a0,struct.pack('<III',0x02000000,0xe1000000|(a3&65535),0));mutate(3)
 else:return
 for register in [UC_MIPS_REG_AT,UC_MIPS_REG_V0,UC_MIPS_REG_V1,UC_MIPS_REG_A0,UC_MIPS_REG_A1,UC_MIPS_REG_A2,UC_MIPS_REG_A3,UC_MIPS_REG_T0,UC_MIPS_REG_T1,UC_MIPS_REG_T2,UC_MIPS_REG_T3,UC_MIPS_REG_T4,UC_MIPS_REG_T5,UC_MIPS_REG_T6,UC_MIPS_REG_T7,UC_MIPS_REG_T8,UC_MIPS_REG_T9,UC_MIPS_REG_HI,UC_MIPS_REG_LO]:
  u.reg_write(register,mix(seed+calls*97+register))
 u.reg_write(UC_MIPS_REG_PC,ret)
u.hook_add(UC_HOOK_CODE,hook);rng=random.Random(0x1f808)
values=[0,1,-1,0x7fffffff,-0x80000000,32767,-32768,320,319,-32,0x10000,-0x10000]
critical=[-32768,-32767,-32761,-32760,-9,-8,-7,-1,0,1,7,8,15,16,31,32,39,40,41,47,48,49,55,56,119,120,121,126,127,128,129,135,136,32759,32760,32766,32767]
critical_set=set(critical);timers=critical+[x for x in range(-32768,32768) if x not in critical_set]
assert len(timers)==len(set(timers))==65536
seen_timers=set()
for case in range(n):
 seed=rng.getrandbits(32);timer=timers[case&65535];seen_timers.add(timer)
 x=values[case%len(values)] if case%3 else rng.randrange(-0x80000000,0x80000000)
 y=values[(case//len(values))%len(values)] if case%5 else rng.randrange(-0x80000000,0x80000000)
 glyph=values[(case//144)%len(values)] if case%7 else rng.randrange(-0x80000000,0x80000000)
 clut=rng.randrange(-0x80000000,0x80000000);lib.setup(seed,timer,rng.getrandbits(32),rng.getrandbits(32))
 for j,a in enumerate([0x80130000,0x80160000]):w(a,C.string_at(lib.frame_bytes(j),0x22778))
 w(0x80200000,C.string_at(lib.packet_bytes(),12288));sw(0x80210000,lib.animation_word());sw(0x800acc14,0x80130000+(seed&1)*0x30000)
 initial=stream((seed>>1)%3)+64;sw(0x1f800000,initial);sw(0x1f800004,0x80130070+((seed>>3)&1)*0x30000)
 sp=0x803f0000;u.reg_write(UC_MIPS_REG_SP,sp);u.reg_write(UC_MIPS_REG_RA,0x80010000);sw(sp+16,0x80210000)
 for register,value in zip([UC_MIPS_REG_A0,UC_MIPS_REG_A1,UC_MIPS_REG_A2,UC_MIPS_REG_A3],[x,y,glyph,clut]):u.reg_write(register,value&0xffffffff)
 calls=0;u.emu_start(0x8001f808,0x80010000,count=5000);assert calls==4;lib.run(x,y,glyph,clut)
 assert r(0x80200000,12288)==C.string_at(lib.packet_bytes(),12288),('packet',case,timer,glyph)
 for j,a in enumerate([0x80130000,0x80160000]):assert r(a,0x22778)==C.string_at(lib.frame_bytes(j),0x22778),('frame',case,j)
 assert lw(0x1f800000)-0x80200000==lib.cursor_offset() and lw(0x80210000)==lib.animation_word(),('state',case)
 off=lib.order_offset();assert lw(0x1f800004)==0x80130000+(off//0x22778)*0x30000+off%0x22778
 assert lw(0x800acc14)==0x80130000+lib.root_index()*0x30000
print(f'{n} {"compiled-MIPS" if os.environ.get("RR_COMPILED_ELF") else "original-MIPS"}/native cases passed; 3 service categories, full frame and packet owners, signed timer16, full32 coordinates/glyph/clut, native wide cursor capture and fresh roots')

assert service_counts=={'sprite_init':n,'link':n*2,'draw_mode':n}
if n>=len(critical):assert critical_set<=seen_timers
if n>=65536:assert len(seen_timers)==65536
hf=lambda path:hashlib.sha256(Path(path).read_bytes()).hexdigest()
headers=set()
def collect(path):
 path=Path(path)
 if path in headers:return
 headers.add(path)
 for include in re.findall(r'^#include\s+"([^"\n]+)"',path.read_text(),re.M):
  child=Path('include')/include
  if not child.is_file():child=path.parent/include
  if child.is_file():collect(child)
for include in ['rr/animated_glyph.h','rr/frame_storage_types.h','rr/layout.h']:collect(Path('include')/include)
audit={'engine':'compiled MIPS' if os.environ.get('RR_COMPILED_ELF') else 'original MIPS','cases':n,'timer_states':len(seen_timers),'service_counts':service_counts,'caller_save_hi_lo_poison':True,'source_sha256':hf(source),'test_sha256':hf(__file__),'header_sha256':{str(path):hf(path) for path in sorted(headers)},'compiled_elf_sha256':hf(os.environ['RR_COMPILED_ELF']) if os.environ.get('RR_COMPILED_ELF') else None}
(P/'audit.json').write_text(json.dumps(audit,indent=2)+'\n')
