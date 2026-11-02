"""Differential integer/unaligned/delay-slot checks against Unicorn execution."""
from pathlib import Path
import sys,struct,random,json,hashlib
from unicorn import Uc,UC_ARCH_MIPS,UC_MODE_MIPS32,UC_MODE_LITTLE_ENDIAN
from unicorn.mips_const import *
sys.path.insert(0,str(Path(__file__).resolve().parent))
from mips_preview_engine import Mips
rng=random.Random(65938);checks=0
base=0x80010000;data=0x80100000
R=lambda f,sh=0:((0 if f in [0,2,3,0x10,0x12] else 4)<<21)|((0 if f in [0x10,0x12] else 5)<<16)|((0 if f in [0x18,0x19] else 6)<<11)|(sh<<6)|f
I=lambda op,imm=0:(op<<26)|((0 if op==15 else 4)<<21)|((0 if op in [6,7] else 5)<<16)|(imm&65535)
ops=[R(f,sh) for f in [0,2,3] for sh in [0,1,16,31]]+[R(f) for f in [4,6,7,0x10,0x12,0x18,0x19,0x21,0x23,0x24,0x25,0x26,0x27,0x2A,0x2B]]
ops += [I(op,imm) for op in [9,10,11,12,13,14,15] for imm in [0,1,0x7FFF,0x8000,0xFFFF]]
ops += [I(op,k) for op,size in [(0x20,1),(0x21,2),(0x23,4),(0x24,1),(0x25,2),(0x28,1),(0x29,2),(0x2B,4)] for k in range(0,8,size)]
ops += [I(op,k) for op in [0x22,0x26,0x2A,0x2E] for k in range(4)]
ops += [I(op,2) for op in [4,5,6,7]]
for word in ops:
 for trial in range(8):
  uc=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);uc.mem_map(0,0x200000)
  backing=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);backing.mem_map(0,0x200000)
  vm=Mips(backing,UC_MIPS_REG_0,UC_MIPS_REG_PC,UC_MIPS_REG_HI,UC_MIPS_REG_LO,lambda *a:None)
  # NOPs expose completed architectural loads; the compiled target schedules
  # load delays, except the standard LWL/LWR merge pair.
  code=struct.pack('<6I',word,0,0,0,0,0);memory=rng.randbytes(32)
  for x in [uc,vm]:x.mem_write(base&0x1fffffff,code);x.mem_write(data&0x1fffffff,memory)
  values=[0]+[rng.getrandbits(32) for i in range(31)]
  values[4]=data if word>>26 in [0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x28,0x29,0x2A,0x2B,0x2E] else [0,1,0xFFFFFFFF,0x80000000,0x7FFFFFFF,15,31,0x87654321][trial]
  values[5]=[0,1,0xFFFFFFFF,0x80000000,0x7FFFFFFF,0x12345678,0x89ABCDEF,0x13572468][trial]
  for i,v in enumerate(values):uc.reg_write(UC_MIPS_REG_0+i,v);vm.reg_write(UC_MIPS_REG_0+i,v)
  for r,v in [(UC_MIPS_REG_HI,0x13579BDF),(UC_MIPS_REG_LO,0x2468ACE0)]:uc.reg_write(r,v);vm.reg_write(r,v)
  end=base+16;uc.emu_start(base,end,count=16);vm.pc=base
  for k in range(16):
   if vm.pc==end:break
   vm.step()
  assert vm.pc==end,(hex(word),trial,hex(vm.pc))
  assert [uc.reg_read(UC_MIPS_REG_0+i) for i in range(32)]==vm.r,(hex(word),trial,'GPR')
  assert (uc.reg_read(UC_MIPS_REG_HI),uc.reg_read(UC_MIPS_REG_LO))==(vm.hi,vm.lo),(hex(word),trial,'HI/LO')
  assert bytes(uc.mem_read(data&0x1fffffff,32))==bytes(vm.mem_read(data,32)),(hex(word),trial,'memory')
  checks+=1
# A local J with a store in its delay slot landing directly on a JAL.
uc=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);uc.mem_map(0,0x200000)
backing=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);backing.mem_map(0,0x200000)
vm=Mips(backing,UC_MIPS_REG_0,UC_MIPS_REG_PC,UC_MIPS_REG_HI,UC_MIPS_REG_LO,lambda *a:None)
code=[0x08000000|(((base+16)>>2)&0x3FFFFFF),I(0x29),0,0,0x0C000000|(((base+32)>>2)&0x3FFFFFF),0x24080017,0,0,0,0x03E00008,0]
for x in [uc,vm]:
 x.mem_write(base&0x1fffffff,struct.pack('<%dI'%len(code),*code));x.reg_write(UC_MIPS_REG_A0,data);x.reg_write(UC_MIPS_REG_A1,0x1234)
uc.emu_start(base,base+24,count=32);vm.pc=base
for i in range(32):
 if vm.pc==base+24:break
 vm.step()
assert vm.pc==base+24 and vm.reg_read(UC_MIPS_REG_RA)==uc.reg_read(UC_MIPS_REG_RA)==base+24
assert vm.reg_read(UC_MIPS_REG_T0)==uc.reg_read(UC_MIPS_REG_T0)==23
assert vm.load(data,2)==int.from_bytes(uc.mem_read(data&0x1fffffff,2),'little')==0x1234
checks+=1
# Standard little-endian unaligned load/store pairs at every byte alignment.
for offset in range(4):
 uc=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);uc.mem_map(0,0x200000)
 backing=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);backing.mem_map(0,0x200000)
 vm=Mips(backing,UC_MIPS_REG_0,UC_MIPS_REG_PC,UC_MIPS_REG_HI,UC_MIPS_REG_LO,lambda *a:None)
 block=[(0x22<<26)|(4<<21)|(6<<16)|3,(0x26<<26)|(4<<21)|(6<<16),0,(0x2A<<26)|(5<<21)|(6<<16)|3,(0x2E<<26)|(5<<21)|(6<<16),0]
 memory=bytes(range(32))
 for x in [uc,vm]:
  x.mem_write(base&0x1fffffff,struct.pack('<6I',*block));x.mem_write(data&0x1fffffff,memory)
  x.reg_write(UC_MIPS_REG_A0,data+offset);x.reg_write(UC_MIPS_REG_A1,data+16+offset);x.reg_write(UC_MIPS_REG_A2,0xA5A55A5A)
 uc.emu_start(base,base+24,count=16);vm.pc=base
 for step in range(6):vm.step()
 assert vm.pc==base+24
 assert vm.reg_read(UC_MIPS_REG_A2)==uc.reg_read(UC_MIPS_REG_A2)==int.from_bytes(memory[offset:offset+4],'little')
 assert bytes(vm.mem_read(data,32))==bytes(uc.mem_read(data&0x1fffffff,32))
 checks+=1
print('PASS',checks,'MIPS instruction blocks: integer, shifts, HI/LO, signed loads, unaligned merges, stores and delay slots')
if len(sys.argv)>1:
 engine=Path(__file__).with_name('mips_preview_engine.py');sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
 Path(sys.argv[1]).write_text(json.dumps(dict(checks=checks,engine_sha256=sha(engine),test_sha256=sha(__file__),reference='Unicorn architectural execution'),indent=2)+'\n')
