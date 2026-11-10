"""Real actuator metadata storage, alignment and PAL protocol limits."""
from pathlib import Path
import subprocess
from tempfile import TemporaryDirectory
import unittest
from c_functions import source_owners, definition_source
ROOT=Path(__file__).resolve().parents[2]
FUNCTIONS=['_padSetAct','_padSetCmd','_padSendAtLoadInfo','_padRecvAtLoadInfo','_padGetActSize','_padLoadActInfo',
 '_padSendActInfoCmd','_padRecvActInfoReply','_padChannelBusy','_padResetChannel',
 '_padCmd45','_padCmd4C','_padCmd46','_padCmd47','_padCmd4B']
SETUP=r'''
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "psyq/libpad/padstat2.h"
#include "psyq/libpad/padpatch.h"
#include "psyq/libpad/padmeta.h"
#include "rr/channel_init.h"
static struct {u8 before[32];PadChannel channel;u8 after[32];} storage;
static PadChannel *record=&storage.channel;
static u8 packet[8];
static int readyCalls;
u8 *PadPatchScanPtr;
static s32 ready(void *pointer) {
 assert(pointer==record);readyCalls++;return _padChannelBusy(pointer);
}
ChannelStatusCallback PadChannelBusyFn=ready;
ChannelUpdateCallback PadResetChannelFn=_padResetChannel;
'''
CHECKS=r'''
static void reset(int modes,int actuators,int combinations) {
 memset(&storage,0xA5,sizeof(storage));memset(record,0,sizeof(*record));
 memset(record->metadata63,0xD7,sizeof(record->metadata63));
 record->modeCountE3=modes;record->entry_countE9=actuators;record->combCountEA=combinations;
 record->scanOffsetEC=combinations*8;record->phase=255;record->pending=1;
 record->packet=packet;readyCalls=0;
}
static void guards(void) {unsigned i;for(i=0;i<32;i++)assert(storage.before[i]==0xA5 && storage.after[i]==0xA5);}
static u8 datum(unsigned index) {return (u8)(index*37+13);}
static void send_row(int length) {
 unsigned at=0;int first=1;
 do {
  unsigned n=first?3:5,i;memset(packet,0,sizeof(packet));
  if(first)packet[4]=length;
  for(i=0;i<n;i++)packet[(first?5:3)+i]=datum(at+i);
  assert(_padRecvActInfoReply(record)==0);
  if(record->flag48==0)break;
  at+=n;first=0;
 }while(1);
}
int main(void) {
 unsigned modes,actuators,combos,scan,i,length,allocations=0,scans=0;
 static const u16 sizes[]={0,1,7,8,127,128,129,255,32768,65535};
 for(modes=0;modes<256;modes++)for(actuators=0;actuators<256;actuators++)
 for(scan=0;scan<sizeof(sizes)/sizeof(sizes[0]);scan++) {
  PadChannel before;reset(modes,actuators,0);record->scanOffsetEC=sizes[scan];before=*record;
  assert(_padGetActSize(record)==(int)(((modes+1)/2)*4+((actuators*5+3)/4)*4+4+sizes[scan]));
  assert(!memcmp(&before,record,sizeof(before)));guards();
 }
 for(modes=0;modes<=64;modes++)for(actuators=0;actuators<=25;actuators++)for(combos=0;combos<=16;combos++) {
  PadChannel expected;uintptr_t start,entries,descriptors;int busy;
  reset(modes,actuators,combos);if(_padGetActSize(record)>128)continue;
  for(busy=0;busy<2;busy++) {
   reset(modes,actuators,combos);record->phase=busy?1:255;expected=*record;
   start=((uintptr_t)record->metadata63+7)&~(uintptr_t)7;
   entries=start+((modes+1)/2)*4;descriptors=(entries+((actuators*5+3)/4)*4+7)&~(uintptr_t)7;
   if(!busy) {
    expected.modes00=(u16 *)start;expected.entries04=(ChannelGateEntry5 *)entries;
    expected.combs08=(ChannelCombEntry8 *)descriptors;
    expected.phase=1;expected.state49=4;expected.state47=0;
    expected.update14=_padSendActInfoCmd;expected.complete18=_padRecvActInfoReply;
   }
   assert(_padLoadActInfo(record,record->metadata63)==!busy);
   assert(readyCalls==1 && !memcmp(&expected,record,sizeof(expected)));guards();
   if(!busy) {
    u8 *end=PadActInfoLimit(record);
    assert((uintptr_t)record->combs08%_Alignof(ChannelCombEntry8)==0);
    assert(end<=record->metadata63+sizeof(record->metadata63));
    assert(end-(u8 *)(record->combs08+combos)==127-(int)(((modes+1)/2)*4+((actuators*5+3)/4)*4+8*combos));
   }
   allocations++;
  }
 }
 reset(2,1,1);
 {PadChannel before=*record;assert(_padLoadActInfo(record,0)==0);assert(readyCalls==0);assert(!memcmp(&before,record,sizeof(before)));}
 record->entries04=(ChannelGateEntry5 *)record->metadata63;
 {PadChannel before=*record;assert(_padLoadActInfo(record,record->metadata63)==0);assert(readyCalls==0);assert(!memcmp(&before,record,sizeof(before)));}
 /* Spare native capacity must not enlarge the PAL-equivalent scan limit. */
 for(modes=0;modes<7;modes++)for(actuators=0;actuators<5;actuators++)for(length=0;length<256;length++) {
  u8 *payload,*end,*tail;size_t remaining,tailBytes;u8 saved[512];
  reset(modes,actuators,1);assert(_padLoadActInfo(record,record->metadata63)==1);
  record->phase=4;record->state47=0;record->flag48=0;record->scanSeenEE=0;
  payload=(u8 *)(record->combs08+1);end=PadActInfoLimit(record);remaining=(size_t)(end-payload);
  tail=end;tailBytes=(record->metadata63+sizeof(record->metadata63))-end;
  assert(tailBytes<=sizeof(saved));memcpy(saved,tail,tailBytes);send_row(length);
  assert(record->combs08[0].count00==length && record->combs08[0].buttons04==payload);
  for(i=0;i<length && i<remaining;i++)assert(payload[i]==datum(i));
  assert(PadPatchScanPtr==payload+(length<remaining?length:remaining));assert(!memcmp(saved,tail,tailBytes));
  if(length>remaining)assert(record->phase==4 && record->state49==4 && record->flag48==0 && record->state47==0);
  else {
   if(length) {assert(record->phase==4 && record->scanSeenEE==0);send_row(length);}
   assert(record->phase==254 && record->state49==6 && record->state47==1);
  }
  guards();scans++;
 }
 /* Discover counts and mode word through the actual initial reply parser,
  * then allocate and replay the actual installed metadata callbacks. */
 for(modes=1;modes<=4;modes++)for(actuators=1;actuators<=4;actuators++)
 for(length=0;length<256;length++) {
  unsigned row;reset(0,0,0);record->phase=2;record->pending=0;
  memset(packet,0,8);packet[3]=modes;packet[4]=1;packet[5]=actuators;packet[6]=1;
  assert(_padRecvAtLoadInfo(record)==0);
  assert(record->modeCountE3==modes && record->entry_countE9==actuators && record->combCountEA==1);
  assert(_padRecvAtLoadInfo(record)==1);record->phase=3;
  memset(packet,0,8);packet[4]=1;packet[5]=length;
  assert(_padRecvAtLoadInfo(record)==0);
  assert(record->pending==256+length);
  assert(_padRecvAtLoadInfo(record)==1);record->phase=4;
  memset(packet,0,8);packet[4]=2;
  assert(_padRecvAtLoadInfo(record)==0 && record->scanOffsetEC==0);
  assert(_padRecvAtLoadInfo(record)==0);
  assert(readyCalls==1 && record->phase==2 && record->state49==4);
  assert(record->update14==_padSendActInfoCmd && record->complete18==_padRecvActInfoReply);
  for(row=0;row<modes;row++) {
   record->update14(record);assert(record->command37==0x4C && record->argument24==row);
   memset(packet,0,8);packet[4]=1+row;packet[5]=length;
   assert(record->complete18(record)==0);
   assert(record->modes00[row]==((1+row)*256+length));
   assert(record->complete18(record)==(row+1==modes));
  }
  assert(record->state47==0);record->phase=3;
  for(row=0;row<actuators;row++) {
   record->update14(record);assert(record->command37==0x46 && record->argument24==row);
   memset(packet,0,8);packet[4]=row+1;packet[5]=0x83;packet[6]=1;packet[7]=2;
   assert(record->complete18(record)==0);
   assert(record->entries04[row].opaque00[0]==row+1 && record->entries04[row].opaque00[1]==3);
   assert(record->entries04[row].all_bits02==1 && record->entries04[row].cost03==2 && record->entries04[row].opaque04==1);
   assert(record->complete18(record)==(row+1==actuators));
  }
  assert(record->state47==0);record->phase=4;
  record->update14(record);assert(record->command37==0x47 && record->argument24==0);
  send_row(2);assert(record->state49==4 && record->phase==4);
  send_row(2);assert(record->state49==6 && record->phase==254);
  assert(record->combs08[0].count00==2);
  assert(record->combs08[0].buttons04[0]==datum(0) && record->combs08[0].buttons04[1]==datum(1));
  guards();
 }
 puts("4096 complete discovery/allocation/callback-replay handshakes passed");
 printf("655360 wire-size cases; %u allocation/readiness cases; %u bounded reply scans passed\n",allocations,scans);
 return 0;
}
'''
class ChannelActuatorMetadataTest(unittest.TestCase):
 def test_real_metadata_storage_alignment_and_wire_budget(self):
  owners=source_owners(ROOT/'src/main/PAL')
  source=SETUP+'\n'.join(definition_source(owners[name].read_text(),name) for name in FUNCTIONS)+CHECKS
  with TemporaryDirectory() as directory:
   c=Path(directory)/'metadata.c';exe=Path(directory)/'metadata';c.write_text(source)
   r=subprocess.run(['cc','-std=gnu11','-O2','-Wall','-Wextra','-Werror','-Wno-deprecated-non-prototype',
    '-Wno-unused-parameter','-fsanitize=address,undefined','-fno-sanitize-recover=all',
    '-I',str(ROOT/'include'),str(c),'-o',str(exe)],capture_output=True,text=True)
   self.assertEqual(r.returncode,0,r.stderr)
   r=subprocess.run([str(exe)],capture_output=True,text=True)
   self.assertEqual(r.returncode,0,r.stdout+r.stderr)
   self.assertIn('655360 wire-size cases',r.stdout)
   print(r.stdout.strip())
