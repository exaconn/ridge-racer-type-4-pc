"""Actual counter-read body and a one-site MMIO read observer."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest


class RootCounterReadTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        root=Path(__file__).resolve().parents[2]
        header=(root/'include/rr/counters.h').read_text()
        header=re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',header,flags=re.M)
        body=(root/'tools/scripts/fixtures/views/func_80097040.c').read_text().replace('#include "rr/counters.h"','')
        assert body.count('D_800ABC34[index].count')==1
        observed=body.replace('func_80097040','observed_read').replace('D_800ABC34[index].count','test_read(index)')
        code='''#include <stdint.h>
typedef uint8_t u8; typedef uint16_t u16; typedef uint32_t u32; typedef int32_t s32;
'''+header+'''
RrRootCounterRegisters registers[3];
RrRootCounterRegisters *D_800ABC34=registers;
s32 read_count, last_index;
u16 test_read(s32 index) {
    u16 result=D_800ABC34[index].count;
    read_count++;last_index=index;
    D_800ABC34[index].count=result+1;
    return result;
}
'''+body+observed
        d=Path(cls.temp.name);(d/'f.c').write_text(code)
        subprocess.run(['cc','-shared','-fPIC','-O2',str(d/'f.c'),'-o',str(d/'f.so')],check=True,capture_output=True)
        cls.lib=C.CDLL(str(d/'f.so'))
        cls.ram=(C.c_uint16*24).in_dll(cls.lib,'registers')
        cls.pointer=C.c_void_p.in_dll(cls.lib,'D_800ABC34')
        cls.reads=C.c_int.in_dll(cls.lib,'read_count')
        cls.last=C.c_int.in_dll(cls.lib,'last_index')
        for name in ('func_80097040','observed_read'):
            f=getattr(cls.lib,name);f.argtypes=[C.c_uint32];f.restype=C.c_int32

    def setUp(self):
        self.pointer.value=C.addressof(self.ram)
        for i in range(24):self.ram[i]=0xA500+i
        self.reads.value=0;self.last.value=-1

    def test_every_halfword_value_is_zero_extended_and_fresh(self):
        for index in range(3):
            for value in range(65536):
                self.ram[index*8]=value
                self.assertEqual(self.lib.func_80097040(index),value)
        before=bytes(self.ram)
        for index in range(3):self.assertEqual(self.lib.func_80097040(index),65535)
        self.assertEqual(bytes(self.ram),before)

    def test_selector_low_halfword_and_invalid_null_base(self):
        for high in (0,1,0x8000,0xFFFF):
            for low in range(65536):
                if low<3:
                    self.pointer.value=C.addressof(self.ram)
                    expected=self.ram[low*8]
                else:
                    self.pointer.value=None
                    expected=0
                self.assertEqual(self.lib.func_80097040((high<<16)|low),expected)

    def test_exactly_one_read_on_valid_indices_and_none_on_invalid(self):
        for high in (0,0xABCD,0xFFFF):
            for low in (0,1,2,3,4,0x7FFF,0xFFFF):
                self.reads.value=0;self.last.value=-1
                if low<3:
                    self.pointer.value=C.addressof(self.ram)
                    before=bytes(self.ram);value=self.ram[low*8]
                    self.assertEqual(self.lib.observed_read((high<<16)|low),value)
                    self.assertEqual((self.reads.value,self.last.value),(1,low))
                    expected=bytearray(before)
                    expected[low*16:low*16+2]=((value+1)&65535).to_bytes(2,'little')
                    self.assertEqual(bytes(self.ram),bytes(expected))
                else:
                    self.pointer.value=None
                    self.assertEqual(self.lib.observed_read((high<<16)|low),0)
                    self.assertEqual((self.reads.value,self.last.value),(0,-1))
