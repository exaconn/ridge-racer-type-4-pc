"""Actual channel constructor, real callback registrar, native pointer and alias cases."""
from pathlib import Path
import re,subprocess,tempfile,unittest
from c_functions import source_owners,definition_source
ROOT=Path(__file__).resolve().parents[2]
def native_source(text):
    text=re.sub(r'^#include[^\n]*\n','',text,flags=re.M)
    text=re.sub(r'\s+__asm__\("\$\d+"\)','',text)
    return re.sub(r'__asm__\s*(?:volatile\s*)?\("".*?\);','',text)
def header(name):
    text=(ROOT/'include/rr'/name).read_text()
    return re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',text,flags=re.M)
owners=source_owners(ROOT/'src/main/PAL')
body=native_source(definition_source(owners['PadInitDirect'].read_text(),'PadInitDirect')).replace('PadInitDirect','compiled_init')
registrar=native_source(definition_source(owners['_padInitDirSeq'].read_text(),'_padInitDirSeq')).replace('_padInitDirSeq','native_registrar')
registrar,n=re.subn(r'\(u8 \*\)0x800B0000U','(callback_page.bytes+0x4380)',registrar)
assert n==1
code=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "types.h"
'''+''.join(header(n) for n in ['channels.h','serial.h','channel_advance.h','bios_devices.h','channel_init.h'])+r'''
#include "channel_legacy_inputs.h"
typedef struct {
 u8 prefix[32];LegacyChannelRecordF0Input records[2];u8 inputs[2][16];
 u8 buffers_a[70],buffers_b[70],suffix[32];
 } World;
static World actual,expected;
typedef struct {
 u8 prefix[32];PadChannel records[2];u8 inputs[2][16];
 u8 buffers_a[70],buffers_b[70],suffix[32];
} NativeWorld;
static NativeWorld native_actual,native_expected;
static void publish_legacy(void);

static PadChannel external_record;
static union {void *align;u8 bytes[0x4388];} callback_page;
#define PadChannelRecordArray native_actual.records
#define D_80114448 native_actual.buffers_a
#define PadTxBuffers native_actual.buffers_b
#define PadPacketDoneCallback (*(ChannelUpdateCallback *)(callback_page.bytes+8))
PadChannel *PadChannelRecords;
ChannelErrorCallback PadAdvanceChannelFn;
ChannelUpdateCallback PadResetChannelFn,PadAllocGatesFn;
ChannelByteCallback PadTxByteCallback;
s32 (*PadChannelToPortFn)(void *);
ChannelRecordCallback PadPortToChannelFn;
void (*PadSaveCommandCallback)(PadChannel *);
ChannelStatusCallback PadChannelDispatchFn,PadChannelBusyFn;
s32 PadComActive,PadMultitapEnabled;
static int stage,seed;
void _padAdvanceChannel(s32 n) {(void)n;}
void _padResetChannel(void *p) {(void)p;}
u32 _padGetTxByte(const PadChannel *p,s32 mode) {(void)p;return 0;}
void _padAllocGates(void *p) {(void)p;}
s32 _padChannelToPort(void *p) {(void)p;return 0;}
void *_padPortToChannel(s32 n) {(void)n;return 0;}
void _padSaveCommand(PadChannel *p) {(void)p;}
s32 _padChannelDispatch(void *p) {(void)p;return 0;}
s32 _padChannelBusy(void *p) {(void)p;return 0;}
void _padPacketDone(void *p) {(void)p;}
'''+registrar+r'''
static void check_callbacks(void) {
 assert(PadAdvanceChannelFn==_padAdvanceChannel && PadResetChannelFn==_padResetChannel);
 assert(PadTxByteCallback==_padGetTxByte && PadAllocGatesFn==_padAllocGates);
 assert(PadChannelToPortFn==_padChannelToPort && PadPortToChannelFn==_padPortToChannel);
 assert(PadSaveCommandCallback==_padSaveCommand);
 assert(PadChannelDispatchFn==_padChannelDispatch && PadChannelBusyFn==_padChannelBusy && PadPacketDoneCallback==_padPacketDone);
}
void _padInitDirSeq(void) {
 assert(stage==0 && PadComActive==0 && PadMultitapEnabled==0);
 native_registrar();stage=1;PadComActive=-17;PadMultitapEnabled=seed;
}
void PsyqBzero(void *p,s32 length) {
 assert(stage==1 && p==native_actual.records && length==sizeof(native_actual.records));
 assert(PadChannelRecords==native_actual.records && PadComActive==-17 && PadMultitapEnabled==seed);
 check_callbacks();memset(p,0,length);
 native_actual.records[0].responseE8=seed;native_actual.records[1].responseE8=seed+1;
 /* The local arena pointer must survive an opaque helper replacing this global. */
 PadChannelRecords=&external_record;stage=2;
}
void _padSetVsyncParam(void) {
 assert(stage==2 && PadComActive==-17 && PadMultitapEnabled==seed);
 assert(!memcmp(&native_actual,&native_expected,sizeof(native_actual)));
 publish_legacy();assert(!memcmp(&actual,&expected,sizeof(actual)));
 check_callbacks();PadComActive=-42;stage=3;
}
'''+body+r'''
static u8 *mapped(u8 *pointer) {
 size_t offset=(size_t)(pointer-(u8 *)&actual);
 assert(offset+1<sizeof(actual));return (u8 *)&expected+offset;
}
static u8 *input(int which) {
 switch(which) {
 case 0:return actual.inputs[0];case 1:return actual.inputs[1];
 case 2:return actual.inputs[0]+4;
 case 3:return actual.records[0].status.payload5D;
 case 4:return actual.records[1].status.payload5D;
 case 5:return (u8 *)&actual.records[0].status.reset0C;
 case 6:return (u8 *)&actual.records[1].status.reset0C;
 default:return actual.buffers_a+12;
 }
}
static void reference(u8 *first,u8 *second) {
 int i,j;
 memset(expected.records,0,sizeof(expected.records));
 expected.records[0].responseE8=seed;expected.records[1].responseE8=seed+1;
 expected.records[0].status.input30=first;expected.records[1].status.input30=second;
 for(i=0;i<2;i++) {
  LegacyChannelStatusViewInput *r=&expected.records[i].status;
  r->reset0C=0;r->owner10=&actual.records[i].status;
  *mapped(r->input30)=255;*mapped(r->input30+1)=0;
  r->packet=actual.buffers_a+i*35;r->buffer40=actual.buffers_b+i*35;
  for(j=0;j<6;j++)r->payload5D[j]=255;
 }
}

/* The constructor corpus is still the original800-byte World. These are
 * explicit logical bindings to actual owners, including aliases into the
 * input bytes, payload IDs and the complete native child-pointer field. */
static u8 *to_native(u8 *pointer) {
 size_t off;int i;
 if(pointer>=(u8 *)actual.inputs && pointer<(u8 *)actual.inputs+sizeof(actual.inputs))
  return (u8 *)native_actual.inputs+(pointer-(u8 *)actual.inputs);
 if(pointer>=actual.buffers_a && pointer<actual.buffers_a+sizeof(actual.buffers_a))
  return native_actual.buffers_a+(pointer-actual.buffers_a);
 if(pointer>=actual.buffers_b && pointer<actual.buffers_b+sizeof(actual.buffers_b))
  return native_actual.buffers_b+(pointer-actual.buffers_b);
 for(i=0;i<2;i++) {
  u8 *old=actual.records[i].status.payload5D;
  if(pointer>=old && pointer<old+6) {
   off=pointer-old;return native_actual.records[i].output54.fields.payload+off;
  }
  old=(u8 *)&actual.records[i].status.reset0C;
  if(pointer>=old && pointer<old+4) {
   off=pointer-old;return (u8 *)&native_actual.records[i].children0C+off;
  }
 }
 assert(0);return 0;
}
static u8 *to_legacy(u8 *pointer) {
 int i;
 if(pointer>=(u8 *)native_actual.inputs && pointer<(u8 *)native_actual.inputs+sizeof(native_actual.inputs))
  return (u8 *)actual.inputs+(pointer-(u8 *)native_actual.inputs);
 if(pointer>=native_actual.buffers_a && pointer<native_actual.buffers_a+sizeof(native_actual.buffers_a))
  return actual.buffers_a+(pointer-native_actual.buffers_a);
 if(pointer>=native_actual.buffers_b && pointer<native_actual.buffers_b+sizeof(native_actual.buffers_b))
  return actual.buffers_b+(pointer-native_actual.buffers_b);
 for(i=0;i<2;i++) {
  u8 *base=native_actual.records[i].output54.fields.payload;
  if(pointer>=base && pointer<base+6)return actual.records[i].status.payload5D+(pointer-base);
  base=(u8 *)&native_actual.records[i].children0C;
  if(pointer>=base && pointer<base+4)return (u8 *)&actual.records[i].status.reset0C+(pointer-base);
 }
 assert(0);return 0;
}
static void decode_expected(void) {
 int i;
 memcpy(&native_expected,&native_actual,sizeof(native_actual));
 memcpy(native_expected.prefix,expected.prefix,sizeof(expected.prefix));
 memcpy(native_expected.inputs,expected.inputs,sizeof(expected.inputs));
 memcpy(native_expected.buffers_a,expected.buffers_a,sizeof(expected.buffers_a));
 memcpy(native_expected.buffers_b,expected.buffers_b,sizeof(expected.buffers_b));
 memcpy(native_expected.suffix,expected.suffix,sizeof(expected.suffix));
 memset(native_expected.records,0,sizeof(native_expected.records));
 for(i=0;i<2;i++) {
  LegacyChannelStatusViewInput *old=&expected.records[i].status;
  PadChannel *record=&native_expected.records[i];
  /* The original input aliases may write two bytes into a cleared child
   * pointer. Preserve that exact byte effect across the full native slot. */
  memcpy(&record->children0C,&old->reset0C,4);
  assert(old->owner10==&actual.records[i].status);
  record->owner10=&native_actual.records[i];
  record->input30=to_native(old->input30);
  record->packet=to_native(old->packet);record->buffer40=to_native(old->buffer40);
  memcpy(record->output54.fields.payload,old->payload5D,6);
  record->responseE8=expected.records[i].responseE8;
 }
}
static void publish_legacy(void) {
 int i;size_t j;
 memcpy(actual.prefix,native_actual.prefix,sizeof(actual.prefix));
 memcpy(actual.inputs,native_actual.inputs,sizeof(actual.inputs));
 memcpy(actual.buffers_a,native_actual.buffers_a,sizeof(actual.buffers_a));
 memcpy(actual.buffers_b,native_actual.buffers_b,sizeof(actual.buffers_b));
 memcpy(actual.suffix,native_actual.suffix,sizeof(actual.suffix));
 memset(actual.records,0,sizeof(actual.records));
 for(i=0;i<2;i++) {
  LegacyChannelStatusViewInput *old=&actual.records[i].status;
  PadChannel *record=&native_actual.records[i];
  for(j=4;j<sizeof(record->children0C);j++)assert(((u8 *)&record->children0C)[j]==0);
  memcpy(&old->reset0C,&record->children0C,4);
  assert(record->owner10==&native_actual.records[i]);old->owner10=&actual.records[i].status;
  old->input30=to_legacy(record->input30);
  old->packet=to_legacy(record->packet);old->buffer40=to_legacy(record->buffer40);
  memcpy(old->payload5D,record->output54.fields.payload,6);
  actual.records[i].responseE8=record->responseE8;
 }
}
static void test_init(u8 *first,u8 *second) {
 size_t j;
 assert(sizeof(World)==800);
 for(j=0;j<sizeof(native_actual);j++)((u8 *)&native_actual)[j]=((u8 *)&actual)[j%sizeof(actual)];
 memcpy(native_actual.prefix,actual.prefix,sizeof(actual.prefix));
 memcpy(native_actual.inputs,actual.inputs,sizeof(actual.inputs));
 memcpy(native_actual.buffers_a,actual.buffers_a,sizeof(actual.buffers_a));
 memcpy(native_actual.buffers_b,actual.buffers_b,sizeof(actual.buffers_b));
 memcpy(native_actual.suffix,actual.suffix,sizeof(actual.suffix));
 decode_expected();compiled_init(to_native(first),to_native(second));
 assert(!memcmp(&native_actual,&native_expected,sizeof(native_actual)));
 publish_legacy();
}
int main(void) {
 int a,b,n;unsigned j;u8 *first,*second;
 for(n=0;n<256;n++)for(a=0;a<8;a++)for(b=0;b<8;b++) {
  seed=n;stage=0;
  for(j=0;j<sizeof(actual);j++)((u8 *)&actual)[j]=(u8)(j*17+n);
  memset(&external_record,0xB7,sizeof(external_record));
  memset(&callback_page,0xA5,sizeof(callback_page));
  memcpy(&expected,&actual,sizeof(actual));first=input(a);second=input(b);
  reference(first,second);
  PadComActive=123;PadMultitapEnabled=456;PadChannelRecords=0;
  test_init(first,second);
  assert(stage==3 && PadComActive==1 && PadMultitapEnabled==seed);
  assert(PadChannelRecords==&external_record);
  assert(!memcmp(&actual,&expected,sizeof(actual)));
  for(j=0;j<sizeof(external_record);j++)assert(((u8 *)&external_record)[j]==0xB7);
  assert(callback_page.bytes[0]==0xA5 && callback_page.bytes[7]==0xA5 && callback_page.bytes[16]==0xA5 && callback_page.bytes[0x4387]==0xA5);
 }
 return 0;
}
'''
class ChannelInitTests(unittest.TestCase):
    def test_actual_constructor_and_registrar_with_ubsan(self):
        with tempfile.TemporaryDirectory() as tmp:
            c=Path(tmp)/'init.c';exe=Path(tmp)/'init';c.write_text(code)
            subprocess.run(['cc','-std=gnu99','-O2','-fsanitize=undefined','-fno-sanitize-recover=all','-I',str(ROOT/'include'),'-I',str(ROOT/'tools/scripts'),str(c),'-o',str(exe)],check=True,capture_output=True)
            subprocess.run([str(exe)],check=True,capture_output=True)
if __name__=='__main__':unittest.main()
