"""Actual channel constructor, real callback registrar, native pointer and alias cases."""
from pathlib import Path
import re,subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[2]
def native_source(text):
    text=re.sub(r'^#include[^\n]*\n','',text,flags=re.M)
    text=re.sub(r'\s+__asm__\("\$\d+"\)','',text)
    return re.sub(r'__asm__\s*(?:volatile\s*)?\("".*?\);','',text)
def header(name):
    text=(ROOT/'include/rr'/name).read_text()
    return re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',text,flags=re.M)
body=native_source((ROOT/'src/main/PAL/func_80099CD4.c').read_text())
registrar=native_source((ROOT/'src/main/PAL/func_8009A334.c').read_text()).replace('func_8009A334','native_registrar')
registrar,n=re.subn(r'\(u8 \*\)0x800B0000U','(callback_page.bytes+0x4380)',registrar)
assert n==1
code=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "types.h"
'''+''.join(header(n) for n in ['channels.h','channel_advance.h','bios_devices.h','channel_init.h'])+r'''
typedef struct {
 u8 prefix[32];RrChannelRecordF0 records[2];u8 inputs[2][16];
 u8 buffers_a[70],buffers_b[70],suffix[32];
} World;
static World actual,expected;
static RrChannelRecordF0 external_record;
static union {void *align;u8 bytes[0x4388];} callback_page;
#define D_801144D8 actual.records
#define D_80114448 actual.buffers_a
#define D_80114490 actual.buffers_b
#define D_800ABC88 (*(RrChannelUpdateCallback *)(callback_page.bytes+8))
RrChannelRecordF0 *D_800ABC9C;
RrChannelErrorCallback D_800ABC68;
RrChannelUpdateCallback D_800ABC6C,D_800ABC74;
u32 (*D_800ABC70)(const RrChannelTransferView *);
s32 (*D_800ABC78)(void *);
RrChannelRecordCallback D_800ABC7C;
void (*D_800ABC8C)(RrChannelTransferView *);
RrChannelStatusCallback D_800ABC80,D_800ABC84;
s32 D_800ABCA0,D_800ABCB4;
static int stage,seed;
void func_80099E9C(s32 n) {(void)n;}
void func_80099E34(void *p) {(void)p;}
u32 func_80099FA0(const RrChannelTransferView *p) {(void)p;return 0;}
void func_8009A05C(void *p) {(void)p;}
s32 func_8009A2D8(void *p) {(void)p;return 0;}
void *func_8009A310(s32 n) {(void)n;return 0;}
void func_80099F90(RrChannelTransferView *p) {(void)p;}
s32 func_8009A368(void *p) {(void)p;return 0;}
s32 func_8009A7BC(void *p) {(void)p;return 0;}
void func_8009A478(void *p) {(void)p;}
'''+registrar+r'''
static void check_callbacks(void) {
 assert(D_800ABC68==func_80099E9C && D_800ABC6C==func_80099E34);
 assert(D_800ABC70==func_80099FA0 && D_800ABC74==func_8009A05C);
 assert(D_800ABC78==func_8009A2D8 && D_800ABC7C==func_8009A310);
 assert(D_800ABC8C==func_80099F90);
 assert(D_800ABC80==func_8009A368 && D_800ABC84==func_8009A7BC && D_800ABC88==func_8009A478);
}
void func_8009A334(void) {
 assert(stage==0 && D_800ABCA0==0 && D_800ABCB4==0);
 native_registrar();stage=1;D_800ABCA0=-17;D_800ABCB4=seed;
}
void func_80096434(void *p,s32 length) {
 assert(stage==1 && p==actual.records && length==sizeof(actual.records));
 assert(D_800ABC9C==actual.records && D_800ABCA0==-17 && D_800ABCB4==seed);
 check_callbacks();memset(p,0,length);
 actual.records[0].responseE8=seed;actual.records[1].responseE8=seed+1;
 /* The local arena pointer must survive an opaque helper replacing this global. */
 D_800ABC9C=&external_record;stage=2;
}
void func_80097E0C(void) {
 assert(stage==2 && D_800ABCA0==-17 && D_800ABCB4==seed);
 assert(!memcmp(&actual,&expected,sizeof(actual)));
 check_callbacks();D_800ABCA0=-42;stage=3;
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
  RrChannelStatusView *r=&expected.records[i].status;
  r->reset0C=0;r->owner10=&actual.records[i].status;
  *mapped(r->input30)=255;*mapped(r->input30+1)=0;
  r->packet=actual.buffers_a+i*35;r->buffer40=actual.buffers_b+i*35;
  for(j=0;j<6;j++)r->payload5D[j]=255;
 }
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
  D_800ABCA0=123;D_800ABCB4=456;D_800ABC9C=0;
  func_80099CD4(first,second);
  assert(stage==3 && D_800ABCA0==1 && D_800ABCB4==seed);
  assert(D_800ABC9C==&external_record);
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
            subprocess.run(['cc','-std=gnu99','-O2','-fsanitize=undefined','-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(c),'-o',str(exe)],check=True,capture_output=True)
            subprocess.run([str(exe)],check=True,capture_output=True)
if __name__=='__main__':unittest.main()
