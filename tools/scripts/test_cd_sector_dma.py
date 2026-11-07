"""Actual sector DMA routines with counted readiness/status read adapters."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest
from test_cd_disc_probe import native_body


class CdSectorDmaTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup)
        root=Path(__file__).resolve().parents[2];d=Path(cls.temp.name)
        header=(root/'include/rr/cd.h').read_text()
        header=re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',header,flags=re.M)
        code='''#include <stdint.h>
typedef uint8_t u8;typedef uint32_t u32;typedef int32_t s32;
'''+header+'''
u8 ports[6];u32 regs[8];u8 *address;
volatile u8 *CD_reg0=&ports[1],*CD_reg3=&ports[2];
volatile u32 *COM_DELAY=&regs[1],*D_800A6780=&regs[2],*CD_dpcr=&regs[3],*DMA3_BCR=&regs[4],*DMA3_CHCR=&regs[5];
u8 * volatile *DMA3_MADR=&address;
s32 status_reads,control_reads,status_ready,control_ready,swap_status;
s32 status_ports[16];u32 setup_snapshot[5];u8 *setup_address;
u8 test_status(volatile u8 *p){
    s32 n=status_reads++;status_ports[n]=p==&ports[4];
    if(!n){setup_snapshot[0]=regs[1];setup_snapshot[1]=regs[2];setup_snapshot[2]=regs[3];setup_snapshot[3]=regs[4];setup_snapshot[4]=regs[5];setup_address=address;}
    if(swap_status && n==0)CD_reg0=&ports[4];
    return status_reads>=status_ready?0x40:0;
}
u32 test_control(volatile u32 *p){
    control_reads++;return control_reads>=control_ready?(*p&~0x01000000U):*p;
}
#define RR_NOP_DEP() ((void)0)
'''
        for name in ('CD_getsector','CD_getsector2','CdGetSector'):
            body=native_body((root/'tools/scripts/fixtures/views'/f'{name}.c').read_text())
            if name=='CD_getsector':
                assert body.count('*p & 0x40')==1 and body.count('*DMA3_CHCR & 0x01000000')==2
                body=body.replace('*p & 0x40','test_status(p) & 0x40').replace('*DMA3_CHCR & 0x01000000','test_control(DMA3_CHCR) & 0x01000000')
            elif name=='CD_getsector2':
                assert body.count('*CD_reg0 & 0x40')==1 and body.count('x = *DMA3_CHCR;')==1
                body=body.replace('*CD_reg0 & 0x40','test_status(CD_reg0) & 0x40').replace('x = *DMA3_CHCR;','x = test_control(DMA3_CHCR);')
            code+=body+'\n'
        (d/'f.c').write_text(code)
        subprocess.run(['cc','-shared','-fPIC','-O2',str(d/'f.c'),'-o',str(d/'f.so')],check=True,capture_output=True)
        cls.lib=C.CDLL(str(d/'f.so'))
        cls.regs=(C.c_uint32*8).in_dll(cls.lib,'regs');cls.ports=(C.c_uint8*6).in_dll(cls.lib,'ports')
        cls.address=C.c_void_p.in_dll(cls.lib,'address');cls.status_port=C.c_void_p.in_dll(cls.lib,'CD_reg0')
        cls.values={n:C.c_int32.in_dll(cls.lib,n) for n in ('status_reads','control_reads','status_ready','control_ready','swap_status')}
        for name in ('CD_getsector','CD_getsector2','CdGetSector'):
            f=getattr(cls.lib,name);f.argtypes=[C.c_void_p,C.c_uint32];f.restype=C.c_int32

    def reset(self):
        self.regs[:]=[0xCAFE1234,0xAAAAAAAA,0xBBBBBBBB,0x12340001,0xCCCCCCCC,0xDDDDDDDD,0xDEADBEEF,0x5A5A5A5A]
        self.ports[:]=[0x12,0x34,0x56,0x78,0x9A,0xBC]
        self.status_port.value=C.addressof(self.ports)+1
        for v in self.values.values():v.value=0
        self.values['status_ready'].value=3;self.values['control_ready'].value=3
        self.address.value=None

    def test_raw_and_boolean_wrapper_preserve_native_addresses_and_word_masks(self):
        buffer=(C.c_uint8*32)(*range(32));before=bytes(buffer)
        if C.sizeof(C.c_void_p)>4:self.assertGreater(C.addressof(buffer),0xFFFFFFFF)
        for name,expected in [('CD_getsector',0),('CdGetSector',1)]:
            for words in (0,1,512,65535,0x80000000,0xFFFF0000,0xFFFFFFFF):
                self.reset();self.assertEqual(getattr(self.lib,name)(buffer,words),expected)
                self.assertEqual(self.address.value,C.addressof(buffer))
                self.assertEqual(list(self.regs),[0xCAFE1234,0x1325,0x20943,0x12348001,words|0x10000,0x11000000,0xDEADBEEF,0x5A5A5A5A])
                self.assertEqual(list(self.ports),[0x12,0,0x80,0x78,0x9A,0xBC])
                self.assertEqual((self.values['status_reads'].value,self.values['control_reads'].value),(3,3))
                self.assertEqual(list((C.c_uint32*5).in_dll(self.lib,'setup_snapshot')),[0x1323,0x20943,0x12348001,words|0x10000,0xDDDDDDDD])
                self.assertEqual(C.c_void_p.in_dll(self.lib,'setup_address').value,C.addressof(buffer))
                self.assertEqual(bytes(buffer),before)

    def test_nonblocking_variant_has_distinct_setup_and_single_control_read(self):
        buffer=(C.c_uint8*4)(1,2,3,4)
        for words in (0,512,0xFFFFFFFF):
            self.reset();self.values['control_ready'].value=10
            self.assertEqual(self.lib.CD_getsector2(buffer,words),0)
            self.assertEqual(self.address.value,C.addressof(buffer))
            self.assertEqual(list(self.regs),[0xCAFE1234,0x1325,0x21020843,0x12348001,words|0x10000,0x11400100,0xDEADBEEF,0x5A5A5A5A])
            self.assertEqual((self.values['status_reads'].value,self.values['control_reads'].value),(3,1))
            self.assertEqual(bytes(buffer),b'\1\2\3\4')

    def test_blocking_captures_status_pointer_other_variant_reloads_it(self):
        buffer=(C.c_uint8*4)()
        for name,ports in [('CD_getsector',[0,0,0]),('CD_getsector2',[0,1,1])]:
            self.reset();self.values['swap_status'].value=1
            self.assertEqual(getattr(self.lib,name)(buffer,1),0)
            self.assertEqual(list((C.c_int32*16).in_dll(self.lib,'status_ports'))[:3],ports)
            self.assertEqual(self.status_port.value,C.addressof(self.ports)+4)
