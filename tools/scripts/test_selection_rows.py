"""Actual row initializer with PAL lookup aliases mapped into one native ROM image."""
import ctypes as C
from pathlib import Path
import random
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

FRAME=0x4D98
CHOICES=0xF424
VARIANTS=0xF420
CONFIG=0xD4D8


class SelectionRowsTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup)
        root=Path(__file__).resolve().parents[2];d=Path(cls.temp.name)
        header=(root/'include/rr/selection_types.h').read_text() + (root/'include/rr/selection.h').read_text()
        header=re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',header,flags=re.M)
        body=(root/'tools/scripts/fixtures/views/func_80031E3C.c').read_text().replace('#include "rr/selection.h"','')
        body=re.sub(r'\s+__asm__\("\$[0-9]+"\)','',body)
        body=re.sub(r' : "\$[0-9]+"','',body)
        adapters={'(unsigned char *)0x800F0000U':('test_ram',1),'(unsigned char *)0x800A0000U':('(test_rom+0x10000)',2),'(unsigned char *)0x800B0000U':('(test_config+0x10000)',1)}
        for old,(new,count) in adapters.items():
            assert body.count(old)==count
            body=body.replace(old,new)
        assert body.count('attribute=config->parameter;')==1
        observed=body.replace('InitSelectionRows','observed_rows').replace('attribute=config->parameter;','attribute=next_parameter(config);')
        code='''#include <stdint.h>
#include <stddef.h>
typedef uint8_t u8; typedef int8_t s8; typedef uint16_t u16; typedef int16_t s16;
typedef uint32_t u32; typedef int32_t s32;
#define RR_FIELD_OFFSET(type,field) offsetof(type,field)
'''+header+'''
u8 test_ram[0x10001],test_rom[0x10001],test_config[0x10001];
u32 parameter_reads;uintptr_t parameter_pointer;
u32 next_parameter(const SelectionConfigView *p) {
    parameter_pointer=(uintptr_t)p;
    return (u8)(p->parameter+parameter_reads++);
}
_Static_assert(sizeof(SelectionRows32)==32,"native frame");
_Static_assert(offsetof(SelectionRows32,rows)==4,"native row start");
'''+body+observed
        (d/'f.c').write_text(code)
        subprocess.run(['cc','-shared','-fPIC','-O2',str(d/'f.c'),'-o',str(d/'f.so')],check=True,capture_output=True)
        cls.lib=C.CDLL(str(d/'f.so'))
        for name in ('InitSelectionRows','observed_rows'):
            f=getattr(cls.lib,name);f.argtypes=[];f.restype=None
        cls.ram=(C.c_uint8*0x10001).in_dll(cls.lib,'test_ram');cls.rom=(C.c_uint8*0x10001).in_dll(cls.lib,'test_rom');cls.config=(C.c_uint8*0x10001).in_dll(cls.lib,'test_config')
        exe=(root/'assets/PAL/main.exe').read_bytes();offset=0x800+0x80090000-0x80010000
        cls.original=exe[offset:offset+0x10000];assert len(cls.original)==0x10000

    def setUp(self):
        C.memset(C.addressof(self.ram),0xA5,len(self.ram));C.memset(C.addressof(self.rom),0xCC,len(self.rom));C.memset(C.addressof(self.config),0x5A,len(self.config))
        C.memmove(C.addressof(self.rom),self.original,len(self.original))
        self.config[CONFIG+0x1E]=0xEF

    def expected(self,frame,parameter,sequence=False):
        result=bytearray(frame);primary,own_variant=frame[:2]
        for i in range(7):
            selection=self.rom[CHOICES+primary*8+i]
            variant=own_variant if selection==primary else self.rom[VARIANTS+selection]
            if selection!=primary and variant==own_variant:variant=self.rom[VARIANTS+primary]
            offset=4+i*4
            result[offset:offset+3]=bytes((selection,variant,(parameter+i if sequence else parameter)&255))
        return bytes(result)

    def frame(self):return bytes(self.ram[FRAME:FRAME+32])

    def test_pal_data_for_all_primary_and_variant_bytes(self):
        before_rom=bytes(self.rom);before_config=bytes(self.config);before_ram=bytearray(self.ram)
        for primary in range(256):
            self.ram[FRAME]=primary
            for variant in range(256):
                self.ram[FRAME+1]=variant
                expected=self.expected(self.frame(),0xEF)
                self.lib.InitSelectionRows()
                self.assertEqual(self.frame(),expected)
        before_ram[FRAME:FRAME+32]=self.frame()
        self.assertEqual(bytes(self.ram),bytes(before_ram));self.assertEqual(bytes(self.rom),before_rom);self.assertEqual(bytes(self.config),before_config)

    def test_same_selection_variant_fallback_and_reserved_bytes(self):
        patterns=((0,1,2,3),(7,7,7,7),(255,0,128,64))
        for palette in patterns:
            self.rom[VARIANTS:VARIANTS+4]=palette
            for primary in range(4):
                self.rom[CHOICES+primary*8:CHOICES+primary*8+8]=[(primary+i)%4 for i in range(8)]
                for variant in range(256):
                    for i in range(32):self.ram[FRAME+i]=(i*23+variant)&255
                    self.ram[FRAME]=primary;self.ram[FRAME+1]=variant
                    before=bytearray(self.ram);before[FRAME:FRAME+32]=self.expected(self.frame(),0xEF)
                    self.lib.InitSelectionRows();self.assertEqual(bytes(self.ram),bytes(before))

    def test_random_overlapping_rom_views_and_parameter_bytes(self):
        rng=random.Random(0x31E3C)
        for _ in range(1000):
            data=rng.randbytes(0x10000);C.memmove(C.addressof(self.rom),data,len(data))
            for i in range(32):self.ram[FRAME+i]=rng.randrange(256)
            parameter=rng.randrange(256);self.config[CONFIG+0x1E]=parameter
            before=bytearray(self.ram);before[FRAME:FRAME+32]=self.expected(self.frame(),parameter)
            config_before=bytes(self.config);rom_before=bytes(self.rom)
            self.lib.InitSelectionRows()
            self.assertEqual(bytes(self.ram),bytes(before));self.assertEqual(bytes(self.config),config_before);self.assertEqual(bytes(self.rom),rom_before)

    def test_configuration_is_read_for_each_row(self):
        self.ram[FRAME]=2;self.ram[FRAME+1]=128;self.config[CONFIG+0x1E]=252
        C.c_uint32.in_dll(self.lib,'parameter_reads').value=0
        expected=self.expected(self.frame(),252,True)
        self.lib.observed_rows()
        self.assertEqual(self.frame(),expected)
        self.assertEqual(C.c_uint32.in_dll(self.lib,'parameter_reads').value,7)
        self.assertEqual(C.c_size_t.in_dll(self.lib,'parameter_pointer').value,C.addressof(self.config)+CONFIG)
