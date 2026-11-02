"""Actual SPU allocation scan, unsigned endpoint wrap and short-circuit reads."""
import ctypes as C
from pathlib import Path
import random
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest


class Allocation(C.Structure):
    _fields_=[('flags',C.c_uint32),('size',C.c_uint32)]


class SpuAllocationScanTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup)
        root=Path(__file__).resolve().parents[2];d=Path(cls.temp.name)
        header=(root/'include/rr/spu.h').read_text()
        header=re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',header,flags=re.M)
        body=(root/'src/main/PAL/func_8007FC54.c').read_text().replace('#include "rr/spu.h"','')
        body=re.sub(r' : "\$[0-9]+"','',body)
        assert body.count('value=p->address_flags;')==1 and body.count('value+p->size')==1
        observed=body.replace('func_8007FC54','observed_scan').replace('value=p->address_flags;','value=read_flags(p);').replace('value+p->size','value+read_size(p)')
        code='''#include <stdint.h>
typedef uint8_t u8; typedef int8_t s8; typedef uint16_t u16; typedef int16_t s16;
typedef uint32_t u32; typedef int32_t s32;
'''+header+'''
RrSpuAllocation table[80];
RrSpuAllocation *D_800A48C0=table;
s32 reads,kind[160],entry[160],replace_base;
u32 read_flags(RrSpuAllocation *p) {
    kind[reads]=0;entry[reads++]=p-table;
    if(replace_base)D_800A48C0=0;
    return p->address_flags;
}
u32 read_size(RrSpuAllocation *p) {kind[reads]=1;entry[reads++]=p-table;return p->size;}
'''+body+observed
        (d/'f.c').write_text(code)
        subprocess.run(['cc','-shared','-fPIC','-O2',str(d/'f.c'),'-o',str(d/'f.so')],check=True,capture_output=True)
        cls.lib=C.CDLL(str(d/'f.so'))
        cls.table=(Allocation*80).in_dll(cls.lib,'table');cls.base=C.c_void_p.in_dll(cls.lib,'D_800A48C0')
        cls.reads=C.c_int32.in_dll(cls.lib,'reads');cls.replace=C.c_int32.in_dll(cls.lib,'replace_base')
        cls.kind=(C.c_int32*160).in_dll(cls.lib,'kind');cls.entry=(C.c_int32*160).in_dll(cls.lib,'entry')
        for name in ('func_8007FC54','observed_scan'):
            f=getattr(cls.lib,name);f.argtypes=[C.c_uint32];f.restype=C.c_int32

    def setUp(self):
        C.memset(C.addressof(self.table),0xA5,C.sizeof(self.table))
        self.base.value=C.addressof(self.table);self.reads.value=0;self.replace.value=0

    def install(self,entries):
        for i,(flags,size) in enumerate(entries):self.table[i]=Allocation(flags,size)
        self.table[len(entries)]=Allocation(0x40000000,0xDEADBEEF)
        self.base.value=C.addressof(self.table)

    @staticmethod
    def expected(entries,address):
        trace=[]
        for i,(flags,size) in enumerate(entries+[(0x40000000,0)]):
            trace.append((0,i))
            if flags&0x80000000:continue
            if flags&0x40000000:return 0,trace
            start=flags&0x0FFFFFFF
            if address<=start:return 1,trace
            trace.append((1,i))
            end=(start+size)&0xFFFFFFFF
            if address<end:return 1,trace
        raise AssertionError('missing end marker')

    def check(self,entries,address,observe=False):
        self.install(entries);before=bytes(self.table);self.reads.value=0
        expected,trace=self.expected(entries,address)
        fn=self.lib.observed_scan if observe else self.lib.func_8007FC54
        self.assertEqual(fn(address),expected)
        self.assertEqual(bytes(self.table),before)
        if observe:
            self.assertEqual([(self.kind[i],self.entry[i]) for i in range(self.reads.value)],trace)
        if not self.replace.value:self.assertEqual(self.base.value,C.addressof(self.table))

    def test_null_table_and_flag_precedence(self):
        self.base.value=None
        for address in (0,1,0x0FFFFFFF,0xFFFFFFFF):self.assertEqual(self.lib.func_8007FC54(address),0)
        cases=[[],[(0x80000100,0xFFFFFFFF)],[(0xC0000100,0xFFFFFFFF),(0x100,0)],[(0x40000000,0),(0,0xFFFFFFFF)],[(0x30000100,0)],[(0xF0000100,0),(0x20000200,0x20)]]
        for entries in cases:
            for address in (0,1,0xFF,0x100,0x101,0x200,0x220,0xFFFFFFFF):self.check(entries,address,True)
        self.check([(0x100,0)],0,True)  # Before-start queries are accepted, even for size zero.

    def test_unsigned_boundaries_and_wrapped_endpoints(self):
        for start in (0,1,0x100,0x0FFFFFFE,0x0FFFFFFF):
            for size in (0,1,0x10,0x80000000,0xF0000001,0xFFFFFFFF):
                end=(start+size)&0xFFFFFFFF
                queries={0,0xFFFFFFFF,start,(start-1)&0xFFFFFFFF,(start+1)&0xFFFFFFFF,end,(end-1)&0xFFFFFFFF,(end+1)&0xFFFFFFFF}
                for high in (0,0x10000000,0x20000000,0x30000000):
                    for address in queries:self.check([(high|start,size)],address,True)

    def test_random_terminated_tables_are_read_only(self):
        rng=random.Random(0x7FC54)
        for _ in range(10000):
            entries=[(rng.getrandbits(28)|rng.choice((0,0x10000000,0x20000000,0x30000000,0x40000000,0x80000000,0xC0000000)),rng.getrandbits(32)) for _ in range(rng.randrange(65))]
            self.check(entries,rng.getrandbits(32))

    def test_short_circuit_trace_and_initial_base_snapshot(self):
        entries=[(0xC0000100,0xFFFFFFFF),(0x80000000,0xFFFFFFFF),(0x100,0x20),(0x200,0x20)]
        for address in (0,0x100,0x101,0x11F,0x120,0x1FF,0x200,0x201,0x220,0xFFFFFFFF):self.check(entries,address,True)
        self.replace.value=1
        self.check(entries,0xFFFFFFFF,True)
        self.assertIsNone(self.base.value)
