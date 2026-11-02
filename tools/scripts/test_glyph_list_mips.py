"""GPU list renderer: actual MIPS and native full pointers, wire DMA tags normalized."""
from pathlib import Path
from tempfile import TemporaryDirectory
import os,sys,runpy,subprocess,ctypes as C,struct,json,hashlib
from unicorn import Uc,UC_ARCH_MIPS,UC_MODE_MIPS32,UC_MODE_LITTLE_ENDIAN
from unicorn.mips_const import *
P=Path(__file__).resolve().parent;ROOT=next(p for p in P.parents if (p/'tools/scripts/cc.sh').is_file());os.chdir(ROOT)
ns=runpy.run_path(str(P/'test_glyph_list_native.py'));fixture=ns['fixture'];source=ns['source'];FS=0x22778;SIZE=FS*2;BASE=0x800ADAF0
with TemporaryDirectory(prefix='rr59700-') as td:
 d=Path(td);(d/'native.c').write_text(fixture)
 subprocess.run(['cc','-std=gnu11','-O2','-shared','-fPIC','-DRR_NATIVE_SHARED','-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I'+str(ROOT/'include'),'-I'+str(P),str(d/'native.c'),'-o',str(d/'native.dylib')],check=True)
 lib=C.CDLL(str(d/'native.dylib'))
 for f in ['get_frame','get_script','get_order','get_primary','get_secondary']:getattr(lib,f).restype=C.c_void_p
 lib.get_color.restype=C.c_int32;lib.get_bank.restype=C.c_int32
 u=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);u.mem_map(0,0x400000);u.mem_map(0x1F800000,0x1000);u.mem_write(0x10000,(ROOT/'assets/PAL/main.exe').read_bytes()[0x800:])
 if os.getenv('RR_COMPILED_ELF'):
  from elftools.elf.elffile import ELFFile
  with open(os.environ['RR_COMPILED_ELF'],'rb') as f:u.mem_write(0x59700,ELFFile(f).get_section_by_name('.text').data())
 def read(a,n):return bytes(u.mem_read(a&0x1fffffff,n))
 def write(a,b):u.mem_write(a&0x1fffffff,b)
 def normalize(b,p,offsets):
  b=bytearray(b);low=p&0xFFFFFF
  for off in offsets:
   tag=struct.unpack_from('<I',b,off)[0];delta=((tag&0xFFFFFF)-low)&0xFFFFFF
   if delta<SIZE:struct.pack_into('<I',b,off,(tag&0xFF000000)|((BASE+delta)&0xFFFFFF))
  return bytes(b)
 coverage=set();banks=set();counts=set();mixed=False;xs=set();ys=set();gaps=set();colors=set()
 count=int(sys.argv[2]) if len(sys.argv)>2 else 4096
 edges=[0,1,32767,32768,65535,65534,65536,131071,256,257,511,512,1023,1024,2047,2048]
 for serial in range(count):
  n=edges[serial] if serial<len(edges) else (serial*0x9e3779b9)&0x1FFFF
  lib.setup(n);frame=lib.get_frame();cursor=lib.get_cursor();context=lib.get_context();ng=lib.get_count();bank=lib.get_bank();color=lib.get_color();assert frame>0xffffffff and lib.get_script()>0xffffffff
  before=C.string_at(frame,SIZE);script=C.string_at(lib.get_script(),80);primary=C.string_at(lib.get_primary(),200);secondary=C.string_at(lib.get_secondary(),160)
  write(BASE,before);write(0x801D0000,script);write(0x800A1090,primary);write(0x800A1158,secondary);write(0x800ACC14,struct.pack('<I',BASE+context));write(0x1F800000,struct.pack('<2I',BASE+cursor,0x801E0000))
  for reg in range(UC_MIPS_REG_1,UC_MIPS_REG_31+1):u.reg_write(reg,(0x5AA50000+n*31+reg)&0xffffffff)
  u.reg_write(UC_MIPS_REG_A0,0x801D0000);u.reg_write(UC_MIPS_REG_A1,color&0xffffffff);u.reg_write(UC_MIPS_REG_A2,bank&0xffffffff);u.reg_write(UC_MIPS_REG_SP,0x80300000);u.reg_write(UC_MIPS_REG_RA,0x802F0000)
  u.emu_start(0x80059700,0x802F0000,count=10000);assert u.reg_read(UC_MIPS_REG_PC)==0x802F0000
  lib.run();offsets=[0x70,FS+0x70]+[cursor+i*28+j for i in range(ng) for j in [0,20]]
  assert read(BASE,SIZE)==normalize(C.string_at(frame,SIZE),frame,offsets),('frame output/guards',n,ng,bank)
  assert read(0x1F800000,8)==struct.pack('<2I',BASE+cursor+ng*28,0x801E0000),('full cursor/order',n)
  assert read(0x800ACC14,4)==struct.pack('<I',BASE+context)
  assert read(0x801D0000,80)==script and read(0x800A1090,200)==primary and read(0x800A1158,160)==secondary
  assert C.string_at(lib.get_primary(),200)==primary and C.string_at(lib.get_secondary(),160)==secondary
  tokens=struct.unpack('<40h',script);banks.add(bank);counts.add(ng);xs.add(tokens[0]);ys.add(tokens[1]);colors.add(color)
  glyphs=tokens[2:2+2*ng:2];gaps.update(tokens[3:3+2*ng:2]);mixed|=any(g>=1024 for g in glyphs) and any(g<1024 for g in glyphs)
  for g in glyphs:coverage.add(('secondary' if bank or g>=1024 else 'primary',g-1024 if g>=1024 else g))
 assert coverage==set([('primary',i) for i in range(50)]+[('secondary',i) for i in range(40)]) if count>=4096 else True
 if count>=4096:
  assert banks=={0,1,-1,-2147483648,2147483647} and counts==set(range(10)) and mixed
  assert {-32768,32767,-1,0,1}<=xs and {-32768,32767,-1,0,1}<=ys and {-32768,32767,-1,0,1}<=gaps, ({-32768,32767,-1,0,1}-xs,{-32768,32767,-1,0,1}-ys,{-32768,32767,-1,0,1}-gaps)
  assert {-2147483648,2147483647,-1,0,1,127,128,255,256}<=colors
 result={'source':str(source),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'header_sha256':hashlib.sha256((ROOT/'include/rr/glyph_list.h').read_bytes()).hexdigest(),'fixture_sha256':hashlib.sha256(fixture.encode()).hexdigest(),'test_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'cases':count,'engine':'compiled MIPS' if os.getenv('RR_COMPILED_ELF') else 'original MIPS','glyph_descriptors_covered':len(coverage),'banks':sorted(banks),'glyph_counts':sorted(counts),'mixed_primary_extended':mixed,'checks':['all200+160 fontbytes and80 scriptbytes immutable','two entire0x22778 frame owners; every output field and untouched byte','wire24tag normalization only; CPU pointerowners remain64bit native','full scratchcursor/order and contextpointer preservation','native undefined/bounds sanitizer','X/Y/gap signed16 extrema and color/bank signed32 extrema']}
 if os.getenv('RR_COMPILED_ELF'):result['elf_sha256']=hashlib.sha256(Path(os.environ['RR_COMPILED_ELF']).read_bytes()).hexdigest()
 if os.getenv('RR_AUDIT_JSON'):Path(os.environ['RR_AUDIT_JSON']).write_text(json.dumps(result,indent=2)+'\n')
 print(count,'cases;',result['engine']+'/native full pointers;',len(coverage),'font descriptors; complete frames/cursor/guards passed')
