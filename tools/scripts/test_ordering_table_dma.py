"""Actual channel-6 DMA setup/wait and timer initializer with native pointers."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest
from test_cd_disc_probe import native_body


class OrderingTableDmaTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup)
        root=Path(__file__).resolve().parents[2];d=Path(cls.temp.name)
        header=(root/'include/rr/gpu.h').read_text()
        header=re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',header,flags=re.M)
        body=native_body((root/'tools/scripts/fixtures/views/func_800942E4.c').read_text())
        assert body.count('*DMA6_CHCR &')==2
        body=body.replace('*DMA6_CHCR &','test_control(DMA6_CHCR) &')
        timer=native_body((root/'tools/scripts/fixtures/views/func_80095360.c').read_text())
        code='''#include <stdint.h>
typedef uint8_t u8;typedef uint16_t u16;typedef int16_t s16;typedef uint32_t u32;typedef int32_t s32;
'''+header+'''
u32 ports[6];u8 *address;
u8 * volatile *DMA6_MADR=&address;
volatile u32 *DMA6_BCR=&ports[2],*DMA6_CHCR=&ports[3],*DPCR=&ports[1];
s32 D_800ABBFC,D_800ABC00;
s32 init_calls,init_mode,timeout_calls,reads,ready_after,abort_after,abort_value;
s32 init_swap,timeout_swap,read_ports[64];u32 read_values[64];
u32 setup[4];u8 *setup_address;
s32 VSync(s32 mode){
    init_calls++;init_mode=mode;setup[0]=ports[1];setup[1]=ports[2];setup[2]=ports[3];setup[3]=ports[4];setup_address=address;
    if(init_swap)DMA6_CHCR=&ports[4];return 100;
}
s32 get_alarm(void){
    timeout_calls++;if(timeout_swap && timeout_calls==1)DMA6_CHCR=&ports[4];
    return abort_after && timeout_calls>=abort_after?abort_value:0;
}
u32 test_control(volatile u32 *p){
    s32 i=reads++;u32 value=*p;read_ports[i]=p==&ports[4];
    if(reads>=ready_after)value&=~0x01000000U;
    read_values[i]=value;return value;
}
'''+timer+'\n'+body
        (d/'f.c').write_text(code)
        subprocess.run(['cc','-shared','-fPIC','-O2',str(d/'f.c'),'-o',str(d/'f.so')],check=True,capture_output=True)
        cls.lib=C.CDLL(str(d/'f.so'));cls.f=cls.lib._otc
        cls.f.argtypes=[C.c_void_p,C.c_int32];cls.f.restype=C.c_int32
        cls.ports=(C.c_uint32*6).in_dll(cls.lib,'ports');cls.address=C.c_void_p.in_dll(cls.lib,'address')
        cls.control=C.c_void_p.in_dll(cls.lib,'DMA6_CHCR')
        cls.values={n:C.c_int32.in_dll(cls.lib,n) for n in ('D_800ABBFC','D_800ABC00','init_calls','init_mode','timeout_calls','reads','ready_after','abort_after','abort_value','init_swap','timeout_swap')}
        cls.read_ports=(C.c_int32*64).in_dll(cls.lib,'read_ports');cls.read_values=(C.c_uint32*64).in_dll(cls.lib,'read_values')

    def reset(self):
        self.ports[:]=[0xCAFE1234,0x12345678,0x87654321,0xFFFFFFFF,0x00123456,0xDEADBEEF]
        self.control.value=C.addressof(self.ports)+12;self.address.value=None
        for v in self.values.values():v.value=0
        self.values['D_800ABBFC'].value=-1;self.values['D_800ABC00'].value=999
        self.values['ready_after'].value=1;self.values['abort_value'].value=-1

    def check_setup(self,buffer,count,initial_config):
        offset=C.c_int32((count*4-4)&0xFFFFFFFF).value
        expected=(C.addressof(buffer)+offset)&((1<<(8*C.sizeof(C.c_void_p)))-1)
        self.assertEqual(self.address.value,expected)
        self.assertEqual(C.c_void_p.in_dll(self.lib,'setup_address').value,expected)
        self.assertEqual(list((C.c_uint32*4).in_dll(self.lib,'setup')),[initial_config|0x08000000,count&0xFFFFFFFF,0x11000002,0x00123456])
        self.assertEqual((self.values['init_calls'].value,self.values['init_mode'].value),(1,-1))
        self.assertEqual((self.values['D_800ABBFC'].value,self.values['D_800ABC00'].value),(340,0))
        self.assertEqual((self.ports[0],self.ports[5]),(0xCAFE1234,0xDEADBEEF))

    def test_address_count_wrap_native_width_and_initial_completion(self):
        buffer=(C.c_uint8*32)(*range(32));before=bytes(buffer)
        if C.sizeof(C.c_void_p)>4:self.assertGreater(C.addressof(buffer),0xFFFFFFFF)
        for count in (-2147483648,-1073741824,-2,-1,0,1,2,512,1073741824,2147483647):
            for config in (0,0x12345678,0x80000000,0xFFFFFFFF):
                self.reset();self.ports[1]=config
                self.assertEqual(self.f(buffer,count),count)
                self.check_setup(buffer,count,config)
                self.assertEqual((self.values['reads'].value,self.values['timeout_calls'].value),(1,0))
                self.assertEqual(self.read_values[0],0x10000002)
                self.assertEqual(bytes(buffer),before)

    def test_wait_and_each_timeout_exit_before_another_hardware_read(self):
        buffer=(C.c_uint8*32)()
        for ready in (2,3,7,12):
            for abort in (0,1,2,5,12):
                for error in (-2147483648,-1,1,2147483647):
                    self.reset();self.values['ready_after'].value=ready
                    self.values['abort_after'].value=abort;self.values['abort_value'].value=error
                    failed=0<abort<ready
                    self.assertEqual(self.f(buffer,7),-1 if failed else 7)
                    checks=abort if failed else ready-1
                    reads=abort if failed else ready
                    self.assertEqual((self.values['timeout_calls'].value,self.values['reads'].value),(checks,reads))
                    self.assertEqual(list(self.read_ports)[:reads],[0]*reads)
                    self.assertEqual(list(self.read_values)[:reads],[0x11000002]*(reads-int(not failed))+([0x10000002] if not failed else []))
                    self.check_setup(buffer,7,0x12345678)

    def test_hardware_pointer_reloads_after_initializer_and_timeout_call(self):
        buffer=(C.c_uint8*32)()
        for phase,expected in [('init_swap',[1]),('timeout_swap',[0,1])]:
            self.reset();self.values[phase].value=1;self.values['ready_after'].value=50
            self.assertEqual(self.f(buffer,3),3)
            self.assertEqual(list(self.read_ports)[:self.values['reads'].value],expected)
            self.assertEqual(self.control.value,C.addressof(self.ports)+16)
            self.assertEqual(self.values['timeout_calls'].value,len(expected)-1)
            self.assertEqual((self.ports[3],self.ports[4]),(0x11000002,0x00123456))
