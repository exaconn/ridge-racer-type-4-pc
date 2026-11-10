"""Actual bank setup/restore body, full native pointers and live-count aliases."""
from pathlib import Path
import re,subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[2]
from c_functions import source_owners, definition_source
owner=source_owners(ROOT/'src/main/PAL')['SetupRaceMemoryArena']
body=definition_source(owner.read_text(),'SetupRaceMemoryArena')
body=re.sub(r'\s+__asm__\("\$\d+"\)','',body);body=re.sub(r',?\s*"\$\d+"','',body)
body,n=re.subn(r'\(u8\s*\*\)0x800B0000U','((u8*)&actual.count_record.flags1E4+0x3A64)',body);assert n==1
code=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "rr/record_banks.h"

typedef struct {
 u8 guard0[32];
 s32 reset,mode,other,limit;
 RaceVehicle count_record;
 RaceVehicle *sources[8],*destinations[8];
 ReplayFrame4 *banks[4];
 ReplayStorage arena;
 RaceVehicle source_data[8],destination_data[8];
 u8 guard1[32];
} World;
static World actual,expected;
static CarRecord record_data[2];
#define ReplayFrame actual.reset
#define ReplayState actual.mode
#define SavedRaceState actual.other
#define ReplayLength actual.limit
#define NumCars ((s16)actual.count_record.flags1E4)
#define CarSnapshots actual.sources
#define CarTable actual.destinations
#define D_800FFC48 ((u8*)&actual.arena)
#define ReplayInputs actual.banks
''' + body + r'''
static s32 bits(u32 x){s32 y;memcpy(&y,&x,4);return y;}
static s16 half(u16 x){s16 y;memcpy(&y,&x,2);return y;}
static void *mapped(World *w,const void *pointer) {
 uintptr_t offset=(uintptr_t)pointer-(uintptr_t)&actual;assert(offset<sizeof(actual));
 return (u8*)w+offset;
}
static void reference(s32 layout,s32 mode) {
 expected.reset=0;expected.mode=mode;
 if(mode==2) {
  for(int i=0;i<(s16)expected.count_record.flags1E4;i++) {
   assert(i<8);
   RaceVehicle *destination=mapped(&expected,expected.destinations[i]);
   RaceVehicle *source=mapped(&expected,expected.sources[i]);
   if(destination!=source)memcpy(destination,source,sizeof(*destination));
   destination=mapped(&expected,expected.destinations[i]);
   if((s16)destination->flags1E4>16)destination->flags1E4=(u16)destination->flags1E4%256;
  }
 } else {
  expected.other=0;
  unsigned count;RaceVehicle *first;
  if(layout==1){
   expected.limit=2700;expected.banks[0]=actual.arena.two.inputs[0];
   expected.banks[1]=actual.arena.two.inputs[1];count=4;first=actual.arena.two.snapshots;
  }else if(layout==2){
   expected.limit=900;count=4;first=actual.arena.link.snapshots;
   for(unsigned i=0;i<4;i++)expected.banks[i]=actual.arena.link.inputs[i];
  }else{
   expected.limit=3600;expected.banks[0]=actual.arena.single.inputs;count=8;first=actual.arena.single.snapshots;
  }
  for(unsigned i=0;i<count;i++)expected.sources[i]=first+i;
 }
}
static void check(s32 layout,s32 mode,s16 count,u32 seed,int alias) {
 memset(&actual,(u8)seed,sizeof(actual));
 actual.reset=17;actual.other=-29;actual.mode=bits(seed^0x80000000U);actual.limit=-1;
 actual.count_record.flags1E4=count;
 for(unsigned i=0;i<8;i++) {
  u8 *p=(u8*)&actual.source_data[i];
  for(unsigned j=0;j<sizeof(RaceVehicle);j++)p[j]=(u8)(seed+i*17+j*31);
  actual.source_data[i].flags1E4=half((u16)(seed+i*1717));
  actual.source_data[i].control.record18=&record_data[i%2];
  actual.source_data[i].control.handling1C=&record_data[i%2].handling1C8;
  actual.sources[i]=&actual.source_data[i];actual.destinations[i]=&actual.destination_data[i];
  if(alias==1)actual.sources[i]=actual.destinations[i];
  if(alias==2&&i)actual.sources[i]=actual.destinations[i-1];
 }
 if(mode!=2){
  RaceVehicle *snapshots=layout==1?actual.arena.two.snapshots:layout==2?actual.arena.link.snapshots:actual.arena.single.snapshots;
  unsigned n=layout==1||layout==2?4:8;
  for(unsigned i=0;i<n;i++)snapshots[i]=actual.source_data[(i+seed)%8];
 }
 for(unsigned i=0;i<4;i++)actual.banks[i]=(ReplayFrame4*)((u8*)&actual.arena+i*16);
 if(alias==3) {
  assert(mode==2&&count>0);actual.destinations[0]=&actual.count_record;
  const u16 bounds[]={0,1,257,0xFFFF};actual.source_data[0].flags1E4=half(bounds[seed%4]);
 }
 if(mode==2&&count<=0)for(unsigned i=0;i<8;i++)actual.sources[i]=actual.destinations[i]=0;
 memcpy(&expected,&actual,sizeof(actual));
 assert((uintptr_t)&actual.arena>UINT32_MAX&&sizeof(void*)==sizeof(unsigned long));
 reference(layout,mode);SetupRaceMemoryArena(layout,mode);
 if(mode!=2) {
  actual.count_record.flags1E4=expected.count_record.flags1E4=(layout==1||layout==2)?4:8;
  reference(layout,2);SetupRaceMemoryArena(layout,2);
 }
 if(memcmp(&actual,&expected,sizeof(actual))) {
  fprintf(stderr,"layout=%d mode=%d count=%d seed=%u alias=%d\n",layout,mode,count,seed,alias);assert(0);
 }
}
int main(int argc,char **argv) {
 assert(argc==2);int group=atoi(argv[1]);
 assert(sizeof(void*)==8&&sizeof(RaceVehicle)==816&&offsetof(RaceVehicle,flags1E4)==484);
 assert(offsetof(ReplaySingleStorage,snapshots)%8==0);
 const s32 modes[]={(-2147483647-1),-1,0,1,2,3,2147483647};
 if(group==0) {
 for(unsigned layout=0;layout<7;layout++)for(unsigned mode=0;mode<7;mode++) {
  for(int count=-1;count<=8;count++)check(modes[layout],modes[mode],count,layout*1717+mode,0);
 }
 for(u32 seed=0;seed<65536;seed++)check(bits(seed*1717U),bits(seed*3571U),half(seed),seed,0);
 } else if(group==1) {
  for(u32 seed=0;seed<65536;seed++)check(bits(seed*2654435761U),2,8,seed,seed%3);
 } else {
 for(u32 seed=0;seed<256;seed++)check(2,2,8,seed,3);
 for(u32 raw=0x8000;raw<65536;raw++)check(0,2,half(raw),raw,0);
 check(0,2,0,0,0);
 }
 return 0;
}
'''
class RecordBanksTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temporary=tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.temporary.cleanup)
        source=Path(cls.temporary.name)/'record_banks.c'
        cls.binary=source.with_suffix('')
        source.write_text(code)
        subprocess.run(['cc','-std=gnu99','-O2','-fno-strict-aliasing',
                        '-fsanitize=undefined','-fno-sanitize-recover=all',
                        '-I',str(ROOT/'include'),str(source),'-o',str(cls.binary)],check=True)

    def test_layouts_raw_modes_and_setup_restore_sequences(self):
        subprocess.run([str(self.binary),'0'],check=True)

    def test_all_signed_halfwords_identity_and_chained_copies(self):
        subprocess.run([str(self.binary),'1'],check=True)

    def test_live_count_alias_and_nonpositive_count_null_slots(self):
        subprocess.run([str(self.binary),'2'],check=True)


if __name__=='__main__':
    unittest.main()
