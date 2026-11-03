"""Actual SQR wrappers with software adapters for GTE transfers and arithmetic.

This checks wrapper sequencing, aliasing, and return pointers. Instruction matching
checks the real hardware commands; this adapter does not simulate GTE timing/FLAG.
"""
import ctypes as C
from pathlib import Path
import random
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest


class Vec(C.Structure):
    _fields_=[('x',C.c_int32),('y',C.c_int32),('z',C.c_int32)]


class GteSquareTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup)
        root=Path(__file__).resolve().parents[2];d=Path(cls.temp.name)
        header=(root/'include/rr/geometry.h').read_text()
        header=re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',header,flags=re.M)
        body=(root/'src/main/PAL/psyq/libgte/gtecolo4.c').read_text()
        body=re.sub(r'^#include[^\n]*\n','',body,flags=re.M)
        assert body.count('__asm__("" : : "r"(output) : "$2");')==2
        body=body.replace('__asm__("" : : "r"(output) : "$2");','')
        code='''#include <stdint.h>
typedef int16_t s16;typedef int32_t s32;
'''+header+'''
s32 ir[3],mac[3],trace[4],count;
const void *loaded;void *stored;
void load(const RrVec3i *p) {
    s32 i;const s32 words[3]={p->x,p->y,p->z};
    loaded=p;count=0;trace[count++]=1;
    for(i=0;i<3;i++){s32 low=(uint32_t)words[i]&65535;ir[i]=low<32768?low:low-65536;}
}
void square(s32 shift) {s32 i;trace[count++]=shift?12:2;for(i=0;i<3;i++)mac[i]=(ir[i]*ir[i])>>shift;}
void store(RrVec3i *p) {stored=p;trace[count++]=3;p->x=mac[0];p->y=mac[1];p->z=mac[2];}
#define RR_GTE_LWC2_IR(p) load(p)
#define RR_NOP_DEP() (trace[count++]=0)
#define RR_GTE_SQR12() square(12)
#define RR_GTE_SQR0() square(0)
#define RR_GTE_SWC2_MAC123(p) store(p)
'''+body
        (d/'f.c').write_text(code)
        subprocess.run(['cc','-shared','-fPIC','-O2',str(d/'f.c'),'-o',str(d/'f.so')],check=True,capture_output=True)
        cls.lib=C.CDLL(str(d/'f.so'));cls.functions=[]
        for name,shift in [('func_80091574',12),('func_8009159C',0)]:
            f=getattr(cls.lib,name);f.argtypes=[C.POINTER(Vec),C.POINTER(Vec)];f.restype=C.c_void_p
            cls.functions.append((f,shift))
        cls.trace=(C.c_int32*4).in_dll(cls.lib,'trace');cls.count=C.c_int32.in_dll(cls.lib,'count')
        cls.loaded=C.c_void_p.in_dll(cls.lib,'loaded');cls.stored=C.c_void_p.in_dll(cls.lib,'stored')

    @staticmethod
    def oracle(values,shift):
        return [((v&32767)-(v&32768))**2//(1<<shift) for v in values]

    def check(self,values,f,shift):
        source=Vec(*values);before=bytes(source)
        words=(C.c_int32*5)(0x12345678,-1,-1,-1,0x6543210)
        output=C.cast(C.byref(words,4),C.POINTER(Vec))
        self.assertEqual(f(C.byref(source),output),C.addressof(words)+4)
        self.assertEqual(list(words)[1:4],self.oracle(values,shift))
        self.assertEqual((words[0],words[4]),(0x12345678,0x6543210))
        self.assertEqual(bytes(source),before)
        self.assertEqual((self.loaded.value,self.stored.value),(C.addressof(source),C.addressof(words)+4))
        self.assertEqual(self.count.value,4)
        self.assertEqual(list(self.trace),[1,0,12 if shift else 2,3])

    def test_every_signed_halfword_with_nonzero_upper_words(self):
        for f,shift in self.functions:
            for low in range(65536):
                self.check([C.c_int32(0xABCD0000|low).value,C.c_int32(0x12340000|(low^65535)).value,low^32768],f,shift)

    def test_full_words_and_saturation_boundaries_return_mac_not_ir(self):
        rng=random.Random(0x91574)
        values=[0,1,-1,181,182,11585,11586,32767,-32768,65535,65536,-65536,-2147483648,2147483647]
        values += [rng.randint(-2147483648,2147483647) for _ in range(1000)]
        for f,shift in self.functions:
            for i,v in enumerate(values):self.check([v,values[(i+1)%len(values)],values[-1-i]],f,shift)
            self.check([-32768]*3,f,shift)

    def test_in_place_and_partial_overlap_load_all_components_first(self):
        for f,shift in self.functions:
            for delta in (-2,-1,0,1,2):
                words=(C.c_int32*12)(*[C.c_int32(0xFFFF8000+i*7919).value for i in range(12)])
                before=list(words);input_index=4;output_index=input_index+delta
                source=C.cast(C.byref(words,4*input_index),C.POINTER(Vec))
                output=C.cast(C.byref(words,4*output_index),C.POINTER(Vec))
                expected=before[:];expected[output_index:output_index+3]=self.oracle(before[input_index:input_index+3],shift)
                self.assertEqual(f(source,output),C.addressof(words)+4*output_index)
                self.assertEqual(list(words),expected)
