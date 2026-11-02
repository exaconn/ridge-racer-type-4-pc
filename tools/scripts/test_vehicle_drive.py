"""Vehicle drive controller against original and compiled scheduled MIPS.

Full canonical320B records are accessed through the explicit GNU alias view.
Angle/response mocks are readonly; road sampling initializes three actual locals;
lane/final update mocks mutate the live record. No vehicle hardware is emulated.
"""
from pathlib import Path
from tempfile import TemporaryDirectory
import hashlib,json,os,re,struct,subprocess,sys
from unicorn import Uc,UC_ARCH_MIPS,UC_MODE_MIPS32,UC_MODE_LITTLE_ENDIAN
from elftools.elf.elffile import ELFFile
sys.path.insert(0,'tools/scripts');from mips_preview_engine import Mips,signed
ROOT=Path(__file__).resolve().parents[2];SOURCE=ROOT/'src/main/PAL/func_800486C4.c';CASES=4096
C=r'''
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
#include "rr/vehicle_drive.h"
typedef struct {u8 pre[16];RrRaceVehicle vehicle;u8 post[16];} World;
static World world;
static unsigned scenario,event;
static const s16 limits[7]={0,1,-1,32767,-32768,16,-16};
static const u32 returns[8]={0,1,100,1000,134217728,0x7fffffffU,0x80000000U,0xffffffffU};
static s32 word(u32 x){s32 v;memcpy(&v,&x,4);return v;}
static s16 half(u16 x){s16 v;memcpy(&v,&x,2);return v;}
static void dump(char tag,s32 a,s32 b){unsigned i;printf("%c %08x %08x ",tag,(u32)a,(u32)b);for(i=0;i<sizeof world;i++)printf("%02x",((u8*)&world)[i]);puts("");}
s32 func_800924D4(s32 x,s32 z){assert(event++==0);dump('A',x,z);return (s32)(scenario*137%4096)-2048;}
s32 func_80047CE0(RrRaceVehicle *v){assert(v==&world.vehicle&&event++==1);dump('B',0,0);return (scenario/64)%8<7?limits[(scenario/64)%8]:half(scenario*293+0x8000);}
s32 func_80047DF8(char *raw,s32 lane,s32 *x,s32 *y,s32 *z){
 RrRaceVehicle *v=(RrRaceVehicle *)raw;
 assert(v==&world.vehicle&&event++==2);assert((uintptr_t)x>UINT32_MAX&&(uintptr_t)y>UINT32_MAX&&(uintptr_t)z>UINT32_MAX);
 assert(x!=y&&y!=z&&x!=z);dump('C',lane,0);
 *x=word(scenario*0x9E3779B9U);*y=word(~(scenario*0x85EBCA6BU));*z=word(scenario*0xC2B2AE35U);
 return word(returns[(scenario/8)%8]);
}
void func_80047FBC(RrRaceVehicle *v,s32 speed){unsigned i;assert(v==&world.vehicle&&event++==3);dump('D',speed,0);if(scenario&1)for(i=0;i<800;i++)((u8*)v)[i]=(u8)(scenario*29+(16+i)*7+31);}
void func_800483D8(RrRaceVehicle *v){unsigned i;assert(v==&world.vehicle&&(event==3||event==4));event++;dump('E',0,0);for(i=0;i<800;i++)((u8*)v)[i]=(u8)(scenario*29+(16+i)*7+63);}
'''
TAIL=r'''
int main(void){unsigned i;const s16 shifts[3]={-1,0,1};const s16 inputs[8]={0,1,-1,32767,-32768,8,128,-128};
 _Static_assert(sizeof(World)==832,"whole vehicle and guards");
 _Static_assert(sizeof(RrDriveVehicleView)==800,"full native alias view");
 _Static_assert(sizeof(RrDriveControlView)==0x12C,"full native drive subrecord");
 _Static_assert(offsetof(RrDriveVehicleView,control1F4)==0x1F4,"native control offset");
 for(scenario=0;scenario<4096;scenario++){
  for(i=0;i<sizeof world;i++)((u8*)&world)[i]=(u8)(scenario*29+i*7);
  world.vehicle.gear25C=2;world.vehicle.shiftDir256=shifts[scenario%3];
  world.vehicle.input=inputs[(scenario/3)%8];world.vehicle.steeringLock1B8=(scenario/24)%2;
  world.vehicle.holdFlag23C=inputs[(scenario/48)%8];world.vehicle.pad23E=inputs[(scenario/384)%8];
  world.vehicle.steering288.mode00=(scenario/128)%2;world.vehicle.steeringEnabled318=1;
  event=0;func_800486C4((RrDriveVehicleView *)&world.vehicle);dump('F',event,0);
  for(i=0;i<16;i++)assert(world.pre[i]==(u8)(scenario*29+i*7)&&world.post[i]==(u8)(scenario*29+(816+i)*7));
 }return 0;
}
'''
def main():
 body=SOURCE.read_text();body=body[body.index('void func_800486C4'):];body=re.sub(r' __asm__\("\$\d+"\)','',body)
 with TemporaryDirectory() as tmp:
  c=Path(tmp)/'test.c';exe=Path(tmp)/'test';c.write_text(C+body+TAIL)
  subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror','-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(c),'-o',str(exe)],check=True)
  native=iter(subprocess.check_output([str(exe)],text=True).splitlines())
 code=(ROOT/'assets/PAL/main.exe').read_bytes()[0x38EC4:0x3926C];elf=os.environ.get('RR_COMPILED_ELF')
 if elf:
  with open(elf,'rb') as f:code=ELFFile(f).get_section_by_name('.text').data()
 memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x200000)
 start=0x800486C4;vehicle=0x800D0010;sp=0x801F0000;stop=0x80010000
 write=lambda a,b:memory.mem_write(a&0x1fffffff,bytes(b));read=lambda a,n:bytes(memory.mem_read(a&0x1fffffff,n))
 w32=lambda a,v:write(a,struct.pack('<I',v&0xffffffff));r32=lambda a:struct.unpack('<I',read(a,4))[0]
 w16=lambda a,v:write(a,struct.pack('<H',v&65535));world=lambda:read(vehicle-16,832)
 write(start,code);services=[0x800924D4,0x80047CE0,0x80047DF8,0x80047FBC,0x800483D8];counts=[0]*5;lane_mutations=0
 def store(cpu,access,address,size,value,_):
  a=address&0x1fffffff
  assert (vehicle&0x1fffffff)<=a and a+size<=(vehicle&0x1fffffff)+800 or (sp&0x1fffffff)-56<=a and a+size<=(sp&0x1fffffff),hex(address)
 class Cpu(Mips):
  def step(self):
   w=self.load(self.pc,4)
   if w>>26!=1:return super().step()
   assert self.delay is None;rs=(w>>21)&31;rt=(w>>16)&31;assert rt in [0,1]
   take=signed(self.r[rs])<0 if rt==0 else signed(self.r[rs])>=0
   self.delay=(self.pc+4+(signed(w,16)<<2))&0xffffffff if take else self.pc+8;self.pc+=4
 cpu=Cpu(memory,0,100,101,102,store);inputs=[0,1,-1,32767,-32768,8,128,-128];returns=[0,1,100,1000,134217728,0x7fffffff,0x80000000,0xffffffff]
 for scenario in range(CASES):
  initial=bytes((scenario*29+i*7)&255 for i in range(832));write(vehicle-16,initial)
  w16(vehicle+0x25c,2);w16(vehicle+0x256,[-1,0,1][scenario%3]);w16(vehicle+0x1d8,inputs[(scenario//3)%8]);w16(vehicle+0x1b8,(scenario//24)%2);w16(vehicle+0x23c,inputs[(scenario//48)%8]);w16(vehicle+0x23e,inputs[(scenario//384)%8])
  w16(vehicle+0x288,(scenario//128)%2);w16(vehicle+0x318,1)
  cpu.reset();cpu.r=[0]+[0xBEEF0000+r for r in range(1,32)];cpu.r[4]=vehicle;cpu.r[29]=sp;cpu.r[31]=stop;cpu.pc=start;event=0
  for n in range(1000):
   if cpu.pc==stop:break
   if cpu.pc not in services:cpu.step();continue
   service=services.index(cpu.pc);counts[service]+=1;assert cpu.delay is None
   a=b=0
   if service==0:
    assert event==0;a=cpu.r[4];b=cpu.r[5];result=(scenario*137%4096-2048)&0xffffffff
   else:
    assert cpu.r[4]==vehicle
    if service==1:assert event==1;result=([0,1,-1,32767,-32768,16,-16][(scenario//64)%8] if (scenario//64)%8<7 else signed(scenario*293+0x8000,16))&0xffffffff
    elif service==2:
     assert event==2;a=cpu.r[5];result=returns[(scenario//8)%8]
    elif service==3:assert event==3;a=cpu.r[5];result=0
    else:assert event in [3,4];result=0
   line=f'{"ABCDE"[service]} {a:08x} {b:08x} {world().hex()}';assert next(native)==line,(scenario,service)
   event+=1
   if service==2:
    addresses=[cpu.r[6],cpu.r[7],r32(cpu.r[29]+16)];assert sorted(addresses)==[sp-32,sp-28,sp-24]
    for v,x in zip(addresses,[scenario*0x9e3779b9,~(scenario*0x85ebca6b),scenario*0xc2b2ae35]):w32(v,x)
   elif service in [3,4]:
    if service==3 and scenario&1:lane_mutations+=1
    if service==4 or scenario&1:write(vehicle,bytes((scenario*29+(16+i)*7+(63 if service==4 else 31))&255 for i in range(800)))
   ret=cpu.r[31]
   for r in [2,3,4,5,6,7,8,9,10,11,12,13,14,15,24,25]:cpu.r[r]=0xDEAD0000+r
   cpu.r[2]=result;cpu.hi=0x12345678;cpu.lo=0x87654321;cpu.pc=ret
  assert cpu.pc==stop and cpu.r[29]==sp
  for r in range(16,24):assert cpu.r[r]==0xBEEF0000+r
  assert next(native)==f'F {event:08x} 00000000 {world().hex()}',scenario
  assert read(vehicle-16,16)==initial[:16] and read(vehicle+800,16)==initial[816:]
 assert next(native,None) is None and all(counts) and 0<lane_mutations<counts[3]
 sha=lambda f:hashlib.sha256(Path(f).read_bytes()).hexdigest()
 audit=dict(engine_sha256=sha(ROOT/'tools/scripts/mips_preview_engine.py'),cases=CASES,engine='compiled integer MIPS/native' if elf else 'original integer MIPS/native',source_sha256=sha(SOURCE),test_sha256=sha(__file__),header_sha256=sha(ROOT/'include/rr/vehicle_drive.h'),services=counts,lane_mutations=lane_mutations,full_owner_guards=True,full_host_pointers=True,caller_save_hi_lo_poison=True,mips_store_bounds=True,readonly_services=['angle924D4','limit47CE0','road47DF8 (writes three output locals only)'],sanitizers=['undefined','bounds'])
 if elf:audit['elf_sha256']=sha(elf)
 if os.environ.get('RR_AUDIT_JSON'):Path(os.environ['RR_AUDIT_JSON']).write_text(json.dumps(audit,indent=2)+'\n')
 print('PASS',CASES,audit['engine'],'service calls',counts)
if __name__=='__main__':main()
