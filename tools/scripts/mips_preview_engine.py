"""Integer MIPS-I subset used by the preview controller, including delay slots.

The byte-addressed backing is Unicorn memory only; no Unicorn execution state
is used. Target instruction bytes and relocated jump tables are unchanged.
"""
MASK=0xFFFFFFFF
def signed(v,bits=32):
 v&=(1<<bits)-1
 return v-(1<<bits) if v&(1<<(bits-1)) else v
class Mips:
 def __init__(self,memory,reg0,pc_reg,hi_reg,lo_reg,store_check):
  self.memory=memory;self.reg0=reg0;self.pc_reg=pc_reg;self.hi_reg=hi_reg;self.lo_reg=lo_reg;self.store_check=store_check;self.reset()
 def reset(self):self.r=[0]*32;self.pc=0;self.hi=0;self.lo=0;self.delay=None
 def mem_read(self,a,n):return self.memory.mem_read(a&MASK&0x1fffffff,n)
 def mem_write(self,a,b):self.memory.mem_write(a&MASK&0x1fffffff,b)
 def reg_read(self,n):
  if n==self.pc_reg:return self.pc
  if n==self.hi_reg:return self.hi
  if n==self.lo_reg:return self.lo
  assert self.reg0<=n<self.reg0+32,n
  return self.r[n-self.reg0]
 def reg_write(self,n,v):
  v&=MASK
  if n==self.pc_reg:self.pc=v
  elif n==self.hi_reg:self.hi=v
  elif n==self.lo_reg:self.lo=v
  else:
   assert self.reg0<=n<self.reg0+32,n
   if n!=self.reg0:self.r[n-self.reg0]=v
 def load(self,a,n):return int.from_bytes(self.mem_read(a,n),'little')
 def store(self,a,v,n):
  self.store_check(self,None,a,n,v,None);self.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
 def step(self):
  pc=self.pc;w=self.load(pc,4);op=w>>26;rs=(w>>21)&31;rt=(w>>16)&31;rd=(w>>11)&31;sh=(w>>6)&31;f=w&63;imm=w&65535;simm=signed(imm,16);old=self.delay;self.delay=None
  a=self.r[rs];b=self.r[rt];value=None;dest=rt;branch=False
  if op==0:
   if f in [0,2,3]:assert rs==0
   if f in [0x10,0x12]:assert rs==rt==sh==0
   if f in [0x18,0x19]:assert rd==sh==0
   if f==8:assert rt==rd==sh==0
   if f==9:assert rt==sh==0
   dest=rd
   if f==0:value=b<<sh
   elif f==2:value=b>>sh
   elif f==3:value=signed(b)>>sh
   elif f==4:value=b<<(a&31)
   elif f==6:value=b>>(a&31)
   elif f==7:value=signed(b)>>(a&31)
   elif f==8:self.delay=a;branch=True
   elif f==9:value=pc+8;self.delay=a;branch=True
   elif f==0x10:value=self.hi
   elif f==0x12:value=self.lo
   elif f in [0x18,0x19]:
    product=(signed(a)*signed(b) if f==0x18 else a*b)&0xFFFFFFFFFFFFFFFF;self.lo=product&MASK;self.hi=product>>32
   elif f==0x21:value=a+b
   elif f==0x23:value=a-b
   elif f==0x24:value=a&b
   elif f==0x25:value=a|b
   elif f==0x26:value=a^b
   elif f==0x27:value=~(a|b)
   elif f==0x2A:value=int(signed(a)<signed(b))
   elif f==0x2B:value=int(a<b)
   else:raise AssertionError(('unsupported SPECIAL',hex(pc),hex(w)))
  elif op in [2,3]:
   if op==3:self.r[31]=(pc+8)&MASK
   self.delay=((pc+4)&0xF0000000)|((w&0x3FFFFFF)<<2);branch=True
  elif op in [4,5,6,7]:
   if op in [6,7]:assert rt==0
   take=(a==b) if op==4 else (a!=b) if op==5 else (signed(a)<=0) if op==6 else (signed(a)>0)
   self.delay=(pc+4+(simm<<2))&MASK if take else (pc+8)&MASK;branch=True
  elif op==9:value=a+simm
  elif op==0xA:value=int(signed(a)<simm)
  elif op==0xB:value=int(a<(simm&MASK))
  elif op==0xC:value=a&imm
  elif op==0xD:value=a|imm
  elif op==0xE:value=a^imm
  elif op==0xF:
   assert rs==0;value=imm<<16
  elif op in [0x20,0x21,0x23,0x24,0x25]:
   size={0x20:1,0x21:2,0x23:4,0x24:1,0x25:2}[op];address=(a+simm)&MASK;assert address%size==0,(hex(pc),hex(address),size)
   value=self.load(address,size)
   if op in [0x20,0x21]:value=signed(value,size*8)
  elif op in [0x22,0x26]:
   address=(a+simm)&MASK;k=address&3;word=self.load(address&~3,4)
   if op==0x22:
    shift=(3-k)*8;value=(b&((1<<shift)-1))|(word<<shift)
   else:
    shift=k*8;keep=MASK^((1<<(32-shift))-1);value=(b&keep)|(word>>shift)
  elif op in [0x28,0x29,0x2B]:
   size={0x28:1,0x29:2,0x2B:4}[op];address=(a+simm)&MASK;assert address%size==0,(hex(pc),hex(address),size);self.store(address,b,size)
  elif op in [0x2A,0x2E]:
   address=(a+simm)&MASK;k=address&3;aligned=address&~3;word=self.load(aligned,4)
   if op==0x2A:
    mask=(1<<(8*(k+1)))-1;value2=(word&~mask)|((b>>((3-k)*8))&mask)
   else:
    mask=(MASK<<(k*8))&MASK;value2=(word&~mask)|((b<<(k*8))&mask)
   self.store(aligned,value2,4)
  else:raise AssertionError(('unsupported opcode',hex(pc),hex(w)))
  if value is not None and dest:self.r[dest]=value&MASK
  if old is not None:
   assert not branch,('branch in delay slot',hex(pc),hex(w));self.pc=old
  else:self.pc=(pc+4)&MASK
  self.r[0]=0
