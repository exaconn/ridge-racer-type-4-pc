"""Actual packet driver with native child owners and typed SIO bus observations."""
from pathlib import Path
import subprocess
from tempfile import TemporaryDirectory
import unittest
from c_functions import source_owners, definition_source
ROOT=Path(__file__).resolve().parents[2]
SETUP=r'''
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "rr/channel_init.h"
#include "rr/serial.h"
static struct World {
 u8 before[16];PadChannel main[2],children[2][4];u8 packets[2][64],data[2][32];
 SerialPortRegisters hardware[2];u8 after[16];
} world,expected;
PadChannel *PadChannelRecords=world.main;
s32 PadMultitapEnabled,PadPacketBytesLeft,PadActiveChannel,PadMultitapSlotIndex[2];
SerialPortRegisters *JOY_DATA;
enum {GATE,BYTE,RW2,RW,TIMER,CLEAR,DONE,SAVE,WAIT,ADVANCE,BAUD,DATA};
static struct Event {s32 kind,value;const void *pointer;} script[256];
static int count,seen,flip,mainBank;
static int failRW,failClear,ioCalls,clearCalls,failureCode;
static void expect(s32 kind,const void *pointer,s32 value) {
 assert(count<256);script[count++]=(struct Event){kind,value,pointer};
}
static void event(s32 kind,const void *pointer,s32 value) {
 assert(seen<count);
 if(script[seen].kind!=kind || script[seen].pointer!=pointer || script[seen].value!=value) {
  fprintf(stderr,"event%d expected(%d,%p,%d) actual(%d,%p,%d)\n",seen,
   script[seen].kind,script[seen].pointer,script[seen].value,kind,pointer,value);assert(0);
 }
 seen++;
}
static void gate(void *p) {event(GATE,p,0);}
static u32 byte(const PadChannel *p,s32 mode) {event(BYTE,p,mode);return _padGetTxByte(p,mode);}
s32 _padSioRW2(PadChannel *p,s32 value) {event(RW2,p,value);p->b45++;return ++ioCalls==failRW?failureCode:0;}
s32 _padSioRW(PadChannel *p,s32 value) {event(RW,p,value);p->b45++;return ++ioCalls==failRW?failureCode:0;}
void setRC2wait(s32 value) {event(TIMER,0,value);}
s32 _padClrIntSio0(void) {int result=(++clearCalls!=failClear);event(CLEAR,0,result);return result;}
static void done(void *p) {event(DONE,p,0);}
static void save(PadChannel *p) {event(SAVE,p,0);_padSaveCommand(p);}
void _padWaitRXready(void) {event(WAIT,0,0);JOY_DATA=&world.hardware[flip];}
static void advance(s32 value) {event(ADVANCE,0,value);}
static u16 observe_baud(SerialPortRegisters *p) {u16 v=p->baud;event(BAUD,p,v);return v;}
static u8 observe_data(SerialPortRegisters *p) {u8 v=p->data;event(DATA,p,v);return v;}
ChannelUpdateCallback PadAllocGatesFn=gate,PadPacketDoneCallback=done;
ChannelByteCallback PadTxByteCallback=byte;
void (*PadSaveCommandCallback)(PadChannel *)=save;
ChannelErrorCallback PadAdvanceChannelFn=advance;
'''
CHECKS=r'''
/* Completion protocol scripts: child0..3, whole peer4, save peer8.
 * These describe the externally observable fanout, independently of the
 * recovered driver's switch/loop implementation. */
static const int slots[]={-1,0,1,3,4};
static const int cycles[]={0,1,1,2,3};
static const int actions[5][3][3]={
 {{-1,-1,-1},{-1,-1,-1},{-1,-1,-1}},
 {{4,8,-1},{-1,-1,-1},{-1,-1,-1}},
 {{0,4,8},{-1,-1,-1},{-1,-1,-1}},
 {{2,1,-1},{0,4,8},{-1,-1,-1}},
 {{3,-1,-1},{2,1,-1},{0,4,8}}};
static const int slotAfter[5][3]={{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{1,-1,-1},{3,1,-1}};
static int maximum(int a,int b) {return a>b?a:b;}
static int minimum(int a,int b) {return a<b?a:b;}
static void io(int kind,int mode,int *index,int timer,int observed) {
 PadChannel *p=&world.main[mainBank];
 expect(BYTE,p,mode);expect(kind,p,world.data[mainBank][3+(*index)++]);
 if(observed && kind==RW && !timer)expect(BAUD,&world.hardware[0],0x22);
 if(observed && kind==RW && timer)expect(BAUD,&world.hardware[0],0x88);
 if(timer){expect(TIMER,0,60);expect(CLEAR,0,1);}
}
static void check(int bank,int prefix,int slow,int slotCase,int length,int observed,int changedPort) {
 int i,j,peer=bank^1,nPrefix,budget,nCleanup,nTail,index=0,finalSlot=slots[slotCase],saved=0;
 mainBank=bank;flip=changedPort;count=seen=0;
 failRW=failClear=ioCalls=clearCalls=0;
 memset(&world,0xA5,sizeof(world));
 for(i=0;i<2;i++) {
  PadChannel *p=&world.main[i];p->children0C=world.children[i];p->packet=world.packets[i];
  p->data28=world.data[i]+3;p->length34=16;p->b45=3;p->b44=4;
  p->command37=i==bank?0:0x42;
  memset(p->output54.fields.gates,1,9);
  for(j=0;j<32;j++)world.data[i][j]=(u8)(j*37+i*19);
  world.packets[i][0]=0x81;world.hardware[i].baud=slow?0x88:0x22;
  world.hardware[i].data=0xE0+i;
 }
 JOY_DATA=&world.hardware[0];PadChannelRecords=world.main;
 PadActiveChannel=bank;PadMultitapEnabled=prefix;PadPacketBytesLeft=length;
 PadMultitapSlotIndex[bank]=-1;PadMultitapSlotIndex[peer]=slots[slotCase];
 memcpy(&expected,&world,sizeof(world));expect(GATE,&world.main[bank],0);
 nPrefix=prefix?minimum(5,maximum(length-1,0)):0;
 budget=length-nPrefix-(prefix && length<=5);
 nCleanup=minimum(cycles[slotCase],maximum(budget-1,0));
 nTail=maximum(budget-nCleanup-1,0);
 for(i=0;i<nPrefix;i++) {
  if(i)expect(GATE,&world.children[bank][i-1],0);
  io(RW2,1,&index,1,observed);
 }
 for(i=0;i<nCleanup;i++) {
  for(j=0;j<3 && actions[slotCase][i][j]>=0;j++) {
   int a=actions[slotCase][i][j];
   if(a==8){expect(SAVE,&world.main[peer],0);saved=1;}
   else expect(DONE,a==4?&world.main[peer]:&world.children[peer][a],0);
  }
  finalSlot=slotAfter[slotCase][i];io(RW,prefix,&index,1,0);
 }
 for(i=0;i<nTail;i++)io(RW,prefix,&index,slow,observed);
 expect(WAIT,0,0);
 if(observed)expect(DATA,&world.hardware[flip],world.hardware[flip].data);
 expect(ADVANCE,0,0);
 expected.main[bank].b45=3+index;expected.main[bank].b44=5;
 expected.packets[bank][4]=world.hardware[flip].data;
 if(saved){expected.main[peer].previous38=0x42;expected.main[peer].command37=0;}
 assert((observed?observed_transfer(&world.main[bank]):_padTransferPacket(&world.main[bank]))==0);
 assert(seen==count && PadPacketBytesLeft==budget-nCleanup-nTail-1);
 assert(PadMultitapSlotIndex[peer]==finalSlot && PadMultitapSlotIndex[bank]==-1);
 assert(JOY_DATA==&world.hardware[flip] && PadActiveChannel==bank);
 assert(!memcmp(&world,&expected,sizeof(world)));
}

static void check_error(int bank,int prefix,int step,int code,int clearFailure,int observed) {
 int i;PadChannel *record=&world.main[bank];
 mainBank=bank;flip=0;count=seen=0;ioCalls=clearCalls=0;
 failRW=clearFailure?0:step;failClear=clearFailure?step:0;failureCode=code;
 memset(&world,0xA5,sizeof(world));
 record->children0C=world.children[bank];record->packet=world.packets[bank];
 record->data28=world.data[bank]+3;record->length34=16;record->b45=3;record->b44=4;record->command37=0;
 memset(record->output54.fields.gates,1,9);
 for(i=0;i<32;i++)world.data[bank][i]=(u8)(i*37+bank*19);
 world.packets[bank][0]=0x81;world.hardware[0].baud=0x88;
 JOY_DATA=&world.hardware[0];PadChannelRecords=world.main;
 PadActiveChannel=bank;PadMultitapEnabled=prefix;PadPacketBytesLeft=4;
 PadMultitapSlotIndex[0]=PadMultitapSlotIndex[1]=-1;
 memcpy(&expected,&world,sizeof(world));expect(GATE,record,0);
 for(i=1;i<=step;i++) {
  if(prefix && i>1)expect(GATE,&world.children[bank][i-2],0);
  expect(BYTE,record,prefix);expect(prefix?RW2:RW,record,world.data[bank][i+2]);
  if(i==step && !clearFailure)break;
  if(observed && !prefix)expect(BAUD,&world.hardware[0],0x88);
  expect(TIMER,0,60);expect(CLEAR,0,i==step?0:1);
 }
 expected.main[bank].b45=3+step;
 assert((observed?observed_transfer(record):_padTransferPacket(record))==(clearFailure?-3:code));
 assert(seen==count && PadPacketBytesLeft==4-step);
 assert(JOY_DATA==&world.hardware[0] && PadActiveChannel==bank);
 assert(PadMultitapSlotIndex[0]==-1 && PadMultitapSlotIndex[1]==-1);
 assert(!memcmp(&world,&expected,sizeof(world)));
}
int main(void) {
 int b,p,s,c,n,o,f;unsigned cases=0,errors=0;
 static const s32 codes[]={-1,-20,INT32_MIN};
 assert(sizeof(void *)>4);
 for(b=0;b<2;b++)for(p=0;p<2;p++)for(s=0;s<2;s++)for(c=0;c<5;c++)
 for(n=0;n<=12;n++)for(o=0;o<2;o++)for(f=0;f<2;f++){check(b,p,s,c,n,o,f);cases++;}

 for(b=0;b<2;b++)for(p=0;p<2;p++)for(n=1;n<=3;n++)for(o=0;o<2;o++) {
  for(c=0;c<3;c++){check_error(b,p,n,codes[c],0,o);errors++;}
  check_error(b,p,n,0,1,o);errors++;
 }
 assert(errors==96);puts("96 read/timeout failures stop before further SIO access and completion");
 printf("%u complete native packet traces; direct/observed SIO; both banks and all completion scripts passed\n",cases);
 return 0;
}
'''
class PadPacketTransportTest(unittest.TestCase):
 def test_actual_driver_uses_complete_native_children_and_live_sio_pointer(self):
  owners=source_owners(ROOT/'src/main/PAL')
  bodies='\n'.join(definition_source(owners[n].read_text(),n) for n in ('_padGetTxByte','_padSaveCommand','_padTransferPacket'))
  driver=definition_source(owners['_padTransferPacket'].read_text(),'_padTransferPacket')
  self.assertEqual(driver.count('JOY_DATA->baud'),1);self.assertEqual(driver.count('JOY_DATA->data'),1)
  observed=driver.replace('_padTransferPacket','observed_transfer').replace('JOY_DATA->baud','observe_baud(JOY_DATA)').replace('JOY_DATA->data','observe_data(JOY_DATA)')
  with TemporaryDirectory() as directory:
   c=Path(directory)/'driver.c';exe=Path(directory)/'driver';c.write_text(SETUP+bodies+observed+CHECKS)
   r=subprocess.run(['cc','-std=gnu99','-O2','-Wall','-Wextra','-Werror','-Wno-unused-parameter',
    '-fsanitize=address,undefined','-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(c),'-o',str(exe)],capture_output=True,text=True)
   self.assertEqual(r.returncode,0,r.stderr)
   r=subprocess.run([str(exe)],capture_output=True,text=True)
   self.assertEqual(r.returncode,0,r.stdout+r.stderr)
   self.assertIn('2080 complete native packet traces',r.stdout)
   self.assertIn('96 read/timeout failures',r.stdout)
   print(r.stdout.strip())
