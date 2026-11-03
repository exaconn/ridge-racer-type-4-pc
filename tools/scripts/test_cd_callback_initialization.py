"""Execute the initializer, actual readiness probe and installed event callbacks."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

Hook = C.CFUNCTYPE(None, C.c_int32)
DataCallback = C.CFUNCTYPE(None, C.c_int32, C.c_void_p)
Notification = C.CFUNCTYPE(None, C.c_int32, C.c_int32)


class CdCallbackInitializationTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        d=Path(cls.temp.name);root=Path(__file__).resolve().parents[2]
        header=(root/'include/rr/library.h').read_text()
        header=re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',header,flags=re.M)
        code='''#include <stdint.h>
typedef uint8_t u8; typedef int8_t s8; typedef uint16_t u16; typedef int16_t s16;
typedef uint32_t u32; typedef int32_t s32;
'''+header+'''
u32 test_page[0x10000/4];
#define D_800A63F4 test_page[0x63F4/4]
char test_text[0x1900];
RrLibDataCallback D_800A6488,D_800A648C;
RrLibNotificationCallback D_800A63F0;
s32 primary[5],secondary[5],attempts,secondary_calls,prints,events;
u32 event_class[8],event_spec[8];
const char *printed;
void (*on_probe)(s32);
s32 CD_init(void) {
    s32 n=attempts++;
    if(on_probe) on_probe(n);
    return n<5 ? primary[n] : -99;
}
s32 CD_initvol(void) {secondary_calls++;return secondary[attempts-1];}
s32 func_8001E84C(const char *format,...) {prints++;printed=format;return -123;}
int func_80096E64(int kind,int spec) {
    event_class[events]=(u32)kind;event_spec[events++]=(u32)spec;return -999;
}
'''
        for name in ('func_8008BE44','func_8008BED4','func_8008BF10','func_8008BF38','func_8008BF60'):
            body=(root/'tools/scripts/fixtures/views'/f'{name}.c').read_text()
            body=re.sub(r'^#include[^\n]*\n','',body,flags=re.M)
            body=re.sub(r'\s+__asm__\("\$[0-9]+"\)','',body)
            body=re.sub(r'"\$[0-9]+", ','',body)
            body=re.sub(r' : "\$[0-9]+"','',body)
            if name=='func_8008BE44':
                assert body.count('(u32 *)0x800A0000U')==1
                assert body.count('(const char *)0x80010000U')==1
                body=body.replace('(u32 *)0x800A0000U','test_page').replace('(const char *)0x80010000U','test_text')
            code+=body+'\n'
        (d/'f.c').write_text(code)
        subprocess.run(['cc','-shared','-fPIC','-O2',str(d/'f.c'),'-o',str(d/'f.so')],check=True,capture_output=True)
        cls.lib=C.CDLL(str(d/'f.so'))
        cls.lib.func_8008BE44.argtypes=[];cls.lib.func_8008BE44.restype=C.c_int32
        cls.page=(C.c_uint32*(0x10000//4)).in_dll(cls.lib,'test_page')
        cls.primary=(C.c_int32*5).in_dll(cls.lib,'primary')
        cls.secondary=(C.c_int32*5).in_dll(cls.lib,'secondary')
        cls.slots=[C.c_void_p.in_dll(cls.lib,name) for name in ('D_800A6488','D_800A648C','D_800A63F0')]
        cls.expected=[C.cast(getattr(cls.lib,name),C.c_void_p).value for name in ('func_8008BF10','func_8008BF38','func_8008BF60')]
        cls.hook=C.c_void_p.in_dll(cls.lib,'on_probe')

    def integer(self,name):return C.c_int32.in_dll(self.lib,name)

    def setUp(self):
        for i in range(len(self.page)):self.page[i]=0xA5A5A5A5
        for i in range(5):self.primary[i]=1;self.secondary[i]=0
        for name in ('attempts','secondary_calls','prints','events'):self.integer(name).value=0
        for slot in self.slots:slot.value=None
        C.c_void_p.in_dll(self.lib,'printed').value=None
        self.hook.value=None
        self.retained=[]

    def install_hook(self,fn):
        hook=Hook(fn);self.retained.append(hook);self.hook.value=C.cast(hook,C.c_void_p).value

    def test_success_on_each_of_five_attempts(self):
        for n in range(5):
            self.setUp();self.primary[n]=0
            before=bytes(self.page)
            observations=[]
            self.install_hook(lambda i:observations.append((i,[s.value for s in self.slots],self.page[0x63F4//4])))
            self.assertEqual(self.lib.func_8008BE44(),1)
            self.assertEqual(self.integer('attempts').value,n+1)
            self.assertEqual(self.integer('secondary_calls').value,1)
            self.assertEqual(self.integer('prints').value,0)
            self.assertEqual([s.value for s in self.slots],self.expected)
            expected=bytearray(before);expected[0x63F4:0x63F8]=b'\0'*4
            self.assertEqual(bytes(self.page),bytes(expected))
            self.assertEqual(observations,[(i,[None]*3,0xA5A5A5A5) for i in range(n+1)])

    def test_exhaustion_preserves_callbacks_and_completion_state(self):
        saved=[DataCallback(lambda s,p:None),DataCallback(lambda s,p:None),Notification(lambda e,a:None)]
        self.retained.extend(saved)
        for slot,cb in zip(self.slots,saved):slot.value=C.cast(cb,C.c_void_p).value
        before=([s.value for s in self.slots],bytes(self.page))
        self.primary[:]=[2,-1,0,0,-2147483648]
        self.secondary[:]=[0,0,1,-1,0]
        self.assertEqual(self.lib.func_8008BE44(),0)
        self.assertEqual((self.integer('attempts').value,self.integer('secondary_calls').value,self.integer('prints').value),(5,2,1))
        self.assertEqual(([s.value for s in self.slots],bytes(self.page)),before)
        text=(C.c_char*0x1900).in_dll(self.lib,'test_text')
        self.assertEqual(C.c_void_p.in_dll(self.lib,'printed').value,C.addressof(text)+0x18A8)

    def test_actual_published_callbacks_deliver_their_events(self):
        self.primary[0]=0
        self.assertEqual(self.lib.func_8008BE44(),1)
        buffer=(C.c_uint8*8)(*range(8));before=bytes(buffer)
        DataCallback(self.slots[0].value)(-123,C.addressof(buffer))
        DataCallback(self.slots[1].value)(2147483647,C.addressof(buffer))
        Notification(self.slots[2].value)(2,-999)
        self.assertEqual(self.integer('events').value,3)
        self.assertEqual(list((C.c_uint32*8).in_dll(self.lib,'event_class'))[:3],[0xF0000003]*3)
        self.assertEqual(list((C.c_uint32*8).in_dll(self.lib,'event_spec'))[:3],[0x20,0x40,0x40])
        self.assertEqual(bytes(buffer),before)

    def test_probe_mutations_survive_failure_and_are_replaced_on_success(self):
        saved=[DataCallback(lambda s,p:None),DataCallback(lambda s,p:None),Notification(lambda e,a:None)]
        self.retained.extend(saved);addresses=[C.cast(x,C.c_void_p).value for x in saved]
        def mutate(i):
            for slot,addr in zip(self.slots,addresses):slot.value=addr
            self.page[0x63F4//4]=0x12340000+i
        self.install_hook(mutate)
        self.assertEqual(self.lib.func_8008BE44(),0)
        self.assertEqual(([s.value for s in self.slots],self.page[0x63F4//4]),(addresses,0x12340004))
        self.integer('attempts').value=0;self.primary[2]=0
        self.assertEqual(self.lib.func_8008BE44(),1)
        self.assertEqual(([s.value for s in self.slots],self.page[0x63F4//4]),(self.expected,0))
