"""Actual DMA registration, callback-array clear and setup using native pointers."""
import ctypes as C
from pathlib import Path
import random
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

Callback=C.CFUNCTYPE(None)
ReadHook=C.CFUNCTYPE(None)
Exchange=C.CFUNCTYPE(C.c_void_p,C.c_int32,C.c_void_p)


class DmaCallbackExchangeTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup)
        root=Path(__file__).resolve().parents[2];d=Path(cls.temp.name)
        header=(root/'include/rr/library.h').read_text()
        header=re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',header,flags=re.M)
        bodies={}
        for name in ('func_8008BD10','func_8008BDBC','func_8008BB44'):
            body=(root/'build/legacy'/f'{name}.c').read_text().replace('#include "rr/library.h"','')
            body=re.sub(r'\s+__asm__\("\$[0-9]+"\)','',body)
            bodies[name]=body
        observed=bodies['func_8008BD10'].replace('func_8008BD10','observed_exchange')
        sites={'word=*control;':'word=test_read(control);','*control=word|bit;':'test_write(control,word|bit);','*control=word&~(1U<<shift);':'test_write(control,word&~(1U<<shift));'}
        for old,new in sites.items():
            assert observed.count(old)==(2 if old=='word=*control;' else 1)
            observed=observed.replace(old,new)
        code='''#include <stdint.h>
#include <stddef.h>
typedef uint8_t u8;typedef int8_t s8;typedef uint16_t u16;typedef int16_t s16;
typedef uint32_t u32;typedef int32_t s32;
#define RR_FIELD_OFFSET(type,field) offsetof(type,field)
'''+header+'''
RrLibInterruptCallback D_800A63A4[8];
u32 ports[5];volatile u32 *D_800A63A0=&ports[1];
s32 called[3],io_count,io_kind[4],io_port[4];u32 io_value[4];
RrLibInterruptCallback io_slot[4];s32 selected;
void (*on_read)(void);
void a(void){called[0]++;}void b(void){called[1]++;}void c(void){called[2]++;}
void func_8008BB90(void){called[2]++;}
s32 setup_count,setup_selector;RrLibInterruptCallback setup_callback;
u32 setup_word;RrLibInterruptCallback setup_slots[8];
RrLibInterruptCallback func_8008B394(s32 selector,RrLibInterruptCallback callback){
    s32 i;setup_count++;setup_selector=selector;setup_callback=callback;setup_word=*D_800A63A0;
    for(i=0;i<8;i++)setup_slots[i]=D_800A63A4[i];return a;
}
u32 test_read(volatile u32 *p){
    u32 value=*p;s32 i=io_count++;io_kind[i]=0;io_port[i]=p==&ports[3];io_value[i]=value;io_slot[i]=D_800A63A4[selected];
    if(on_read)on_read();return value;
}
void test_write(volatile u32 *p,u32 value){
    s32 i=io_count++;io_kind[i]=1;io_port[i]=p==&ports[3];io_value[i]=value;io_slot[i]=D_800A63A4[selected];*p=value;
}
'''+''.join(bodies.values())+observed
        (d/'f.c').write_text(code)
        subprocess.run(['cc','-shared','-fPIC','-O2',str(d/'f.c'),'-o',str(d/'f.so')],check=True,capture_output=True)
        cls.lib=C.CDLL(str(d/'f.so'))
        cls.slots=(C.c_void_p*8).in_dll(cls.lib,'D_800A63A4');cls.ports=(C.c_uint32*5).in_dll(cls.lib,'ports')
        cls.port=C.c_void_p.in_dll(cls.lib,'D_800A63A0');cls.hook=C.c_void_p.in_dll(cls.lib,'on_read')
        cls.callbacks=[C.cast(getattr(cls.lib,n),C.c_void_p).value for n in ('a','b','c')]
        cls.called=(C.c_int32*3).in_dll(cls.lib,'called');cls.selected=C.c_int32.in_dll(cls.lib,'selected')
        cls.io_count=C.c_int32.in_dll(cls.lib,'io_count');cls.io_kind=(C.c_int32*4).in_dll(cls.lib,'io_kind')
        cls.io_port=(C.c_int32*4).in_dll(cls.lib,'io_port');cls.io_value=(C.c_uint32*4).in_dll(cls.lib,'io_value');cls.io_slot=(C.c_void_p*4).in_dll(cls.lib,'io_slot')
        for name in ('func_8008BD10','observed_exchange'):
            f=getattr(cls.lib,name);f.argtypes=[C.c_int32,C.c_void_p];f.restype=C.c_void_p
        cls.lib.func_8008BDBC.argtypes=[C.POINTER(C.c_void_p),C.c_int32];cls.lib.func_8008BDBC.restype=None
        cls.lib.func_8008BB44.argtypes=[];cls.lib.func_8008BB44.restype=C.c_void_p

    def setUp(self):
        self.slots[:]=[self.callbacks[0]]*8;self.called[:]=[0,0,0]
        self.ports[:]=[0x12345678,0xFF123456,0x87654321,0xAAAAAAAA,0x55555555]
        self.port.value=C.addressof(self.ports)+4;self.hook.value=None;self.io_count.value=0
        C.c_int32.in_dll(self.lib,'setup_count').value=0

    def test_equal_callback_returns_without_null_hardware_access(self):
        self.port.value=None
        for i in range(8):
            for callback in (None,*self.callbacks):
                self.slots[i]=callback;before=bytes(self.slots)
                for f in (self.lib.func_8008BD10,self.lib.observed_exchange):
                    self.assertEqual(f(i,callback),callback)
                    self.assertEqual(bytes(self.slots),before)
        self.assertEqual(self.io_count.value,0);self.assertEqual(list(self.called),[0,0,0])

    def test_enable_disable_masks_and_publication_before_hardware_read(self):
        rng=random.Random(0x8BD10)
        masks=[0,0xFFFFFFFF,0x800000,0xFF000000,0x00FFFFFF,0xAAAAAAAA,0x55555555]+[rng.getrandbits(32) for _ in range(100)]
        for f in (self.lib.func_8008BD10,self.lib.observed_exchange):
            for i in range(8):
                self.selected.value=i
                for requested in (None,self.callbacks[1]):
                    for mask in masks:
                        self.slots[:]=[self.callbacks[0]]*8;self.ports[1]=mask;self.io_count.value=0
                        expected=list(self.slots);expected[i]=requested
                        result=((mask&0xFFFFFF)|0x800000)
                        result=result|(1<<(i+16)) if requested else result&~(1<<(i+16))
                        self.assertEqual(f(i,requested),self.callbacks[0])
                        self.assertEqual(list(self.slots),expected);self.assertEqual(self.ports[1],result)
                        self.assertEqual((self.ports[0],self.ports[2],self.ports[3],self.ports[4]),(0x12345678,0x87654321,0xAAAAAAAA,0x55555555))
                        if f==self.lib.observed_exchange:
                            self.assertEqual(self.io_count.value,2)
                            self.assertEqual(list(self.io_kind)[:2],[0,1]);self.assertEqual(list(self.io_port)[:2],[0,0])
                            self.assertEqual(list(self.io_value)[:2],[mask,result]);self.assertEqual(list(self.io_slot)[:2],[requested,requested])
        self.assertEqual(list(self.called),[0,0,0])

    def test_hardware_pointer_is_captured_and_read_side_mutations_survive(self):
        self.selected.value=3
        def mutate():
            self.port.value=C.addressof(self.ports)+12
            self.slots[3]=self.callbacks[2]
        hook=ReadHook(mutate);self.hook.value=C.cast(hook,C.c_void_p).value
        self.assertEqual(self.lib.observed_exchange(3,self.callbacks[1]),self.callbacks[0])
        self.assertEqual(self.ports[1],(0x123456|0x800000|0x80000))
        self.assertEqual(self.ports[3],0xAAAAAAAA);self.assertEqual(list(self.io_port)[:2],[0,0])
        self.assertEqual(list(self.io_slot)[:2],[self.callbacks[1],self.callbacks[2]])
        self.assertEqual(self.slots[3],self.callbacks[2])

    def test_clear_uses_native_callback_stride_and_preserves_guards(self):
        for count in (0,1,7,8,15,32):
            words=(C.c_void_p*34)(*[self.callbacks[i%3] for i in range(34)])
            expected=list(words);expected[1:1+count]=[None]*count
            self.lib.func_8008BDBC(C.cast(C.byref(words,C.sizeof(C.c_void_p)),C.POINTER(C.c_void_p)),count)
            self.assertEqual(list(words),expected)
        self.lib.func_8008BDBC(None,0)

    def test_setup_publishes_cleared_table_and_returns_callable_exchange(self):
        result=self.lib.func_8008BB44()
        self.assertEqual(result,C.cast(self.lib.func_8008BD10,C.c_void_p).value)
        self.assertEqual(C.c_int32.in_dll(self.lib,'setup_count').value,1)
        self.assertEqual(C.c_int32.in_dll(self.lib,'setup_selector').value,3)
        self.assertEqual(C.c_void_p.in_dll(self.lib,'setup_callback').value,C.cast(self.lib.func_8008BB90,C.c_void_p).value)
        self.assertEqual(C.c_uint32.in_dll(self.lib,'setup_word').value,0)
        self.assertEqual(list((C.c_void_p*8).in_dll(self.lib,'setup_slots')),[None]*8)
        self.assertEqual(list(self.slots),[None]*8)
        exchange=Exchange(result)
        self.assertIsNone(exchange(2,self.callbacks[1]))
        previous=exchange(2,self.callbacks[0]);self.assertEqual(previous,self.callbacks[1])
        Callback(previous)();self.assertEqual(list(self.called),[0,1,0])
