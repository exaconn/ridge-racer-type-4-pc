"""Actual bank transfer with real mode/address/public transfer wrappers and PAL table pages."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest


class SpuBankTransferTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup)
        root=Path(__file__).resolve().parents[2]
        header=(root/'include/rr/spu.h').read_text()
        header=re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',header,flags=re.M)
        source='''#include <stdint.h>
#include <stdarg.h>
#include <string.h>
typedef uint8_t u8;typedef uint16_t u16;typedef int16_t s16;
typedef uint32_t u32;typedef int32_t s32;
''' + header + '''
u8 test_table[65536] __attribute__((aligned(8)));
u8 test_mode_page[65536] __attribute__((aligned(8)));
#define D_800A47F4 (*(s32*)(test_mode_page+0x47F4))
#define D_800A47F0 (*(volatile u16*)(test_mode_page+0x47F0))
#define D_800A4800 (*(s32*)(test_mode_page+0x4800))
#define D_800A480C (*(s32*)(test_mode_page+0x480C))
s32 D_800A4848;
void (*volatile D_800A4810)(void);
s32 test_commands[8],test_command_count,test_pio_calls,test_mode_calls,test_address_calls;
u32 test_dma_address,test_dma_size,test_input_address;const u16 *test_dma_source;
s32 test_forced_address_result;
void (*test_mode_hook)(void),(*test_address_hook)(void),(*test_dma_hook)(void);
void test_actual_mode(s32);
void func_8008B044(s32 mode) {test_mode_calls++;test_actual_mode(mode);test_mode_hook();}
s32 func_8007F414(s32 index,s32 address) {
 test_address_calls++;test_input_address=(u32)address;
 if(index!=-1)test_pio_calls++;
 test_address_hook();
 return test_forced_address_result>=0 ? test_forced_address_result : (((u32)address+7U)&~7U)>>3;
}
s32 func_8007F068(s32 command,...) {
 va_list args;test_commands[test_command_count++]=command;va_start(args,command);
 if(command==2)test_dma_address=va_arg(args,u32);
 if(command==3) {test_dma_source=va_arg(args,const u16 *);test_dma_size=va_arg(args,u32);test_dma_hook();}
 va_end(args);return -123;
}
void func_8007ED44(const u16 *source,u32 bytes) {test_pio_calls++;}
void test_completion(void) {}
void test_reset(void) {
 s32 i;memset(test_table,0xA5,sizeof(test_table));memset(test_mode_page,0x5A,sizeof(test_mode_page));
 for(i=0;i<17;i++) {
  test_table[0x120+i]=2;
  *(u32*)(test_table+0x138+i*4)=100+i;
  *(u32*)(test_table+0x180+i*4)=0x2000+i*8;
 }
 D_800A47F4=1;D_800A4800=3;D_800A480C=99;D_800A4810=0;D_800A4848=123;
 test_command_count=test_mode_calls=test_address_calls=test_pio_calls=0;
 test_dma_address=test_dma_size=test_input_address=0;test_dma_source=0;
 test_forced_address_result=-1;
}
'''
        mapped_tables=mapped_modes=0
        for name in ('8008AEC4','8008B044','80087DA4','8008AFE4','8008AF84','8007F2E8'):
            body=(root/('src/main/PAL/func_'+name+'.c')).read_text()
            body=re.sub(r'^#include[^\n]*\n|^extern[^\n]*\n','',body,flags=re.M)
            body=re.sub(r'\s+__asm__\("\$[0-9]+"\)','',body)
            body=re.sub(r'"\$[0-9]+",\s*','',body)
            body=re.sub(r':\s*"\$[0-9]+"(?:,\s*"\$[0-9]+")*(?=\))','',body)
            if name=='8008AEC4':
                assert body.count('=(u8*)0x80110000U;')==3
                assert body.count('=0x80110000U;')==1
                body=body.replace('=(u8*)0x80110000U;','=test_table;')
                body=body.replace('=0x80110000U;','=(unsigned long)test_table;');mapped_tables+=4
            if name in ('8008B044','80087DA4'):
                assert body.count('(s32 *)0x800A0000U')==1
                body=body.replace('(s32 *)0x800A0000U','(s32 *)test_mode_page');mapped_modes+=1
            if name=='8008B044':
                assert body.count('void func_8008B044(')==1
                body=body.replace('void func_8008B044(','void test_actual_mode(')
            source+=body
        assert (mapped_tables,mapped_modes)==(4,2)
        directory=Path(cls.temp.name);(directory/'bank.c').write_text(source)
        result=subprocess.run(['cc','-shared','-fPIC','-O2',str(directory/'bank.c'),
                               '-o',str(directory/'bank.so')],capture_output=True,text=True)
        if result.returncode:raise AssertionError(result.stderr)
        cls.lib=C.CDLL(str(directory/'bank.so'))
        cls.table=(C.c_uint8*65536).in_dll(cls.lib,'test_table')
        cls.mode=(C.c_uint8*65536).in_dll(cls.lib,'test_mode_page')
        cls.transfer=cls.lib.func_8008AEC4;cls.transfer.argtypes=[C.POINTER(C.c_uint16),C.c_int32]
        cls.transfer.restype=C.c_int32

    def scalar(self,name):return C.c_int32.in_dll(self.lib,name)
    def mode_scalar(self,offset):return C.c_int32.from_address(C.addressof(self.mode)+offset)
    def word(self,offset):return C.c_uint32.from_address(C.addressof(self.table)+offset)

    def setUp(self):
        self.lib.test_reset();self.errors=[];self.events=[]
        self.values=(C.c_uint16*(0x7EFF0//2))()
        self.values[:16]=range(16)
        self.on_mode=lambda:None;self.on_address=lambda:None;self.on_dma=lambda:None
        self.callbacks=[]
        for name,kind in (('test_mode_hook','mode'),('test_address_hook','address'),('test_dma_hook','dma')):
            def hook(kind=kind):
                try:self.events.append(kind);getattr(self,'on_'+kind)()
                except BaseException as error:self.errors.append(error)
            callback=C.CFUNCTYPE(None)(hook);self.callbacks.append(callback)
            C.c_void_p.in_dll(self.lib,name).value=C.cast(callback,C.c_void_p).value

    def run_case(self,raw,success):
        before=bytes(self.table);index=raw&65535
        result=self.transfer(self.values,raw)
        if self.errors:raise self.errors[0]
        self.assertEqual(result,index if success else -1)
        expected=bytearray(before)
        if success:expected[0x120+index]=1
        self.assertEqual(bytes(self.table),bytes(expected))
        self.assertEqual(self.scalar('test_pio_calls').value,0)

    def test_all_low16_selectors_high_words_and_flag_states(self):
        for raw in range(65536):
            valid=raw<=16
            self.table[0x120+(raw if valid else 0)]=2
            self.scalar('test_command_count').value=0
            result=self.transfer(self.values,raw)
            self.assertEqual(result,raw if valid else -1)
            self.assertEqual(self.scalar('test_command_count').value,3 if valid else 0)
        self.assertEqual(self.scalar('test_mode_calls').value,17)
        for high in (0x10000,0x7FFF0000,0x80000000,0xFFFF0000):
            for index in (0,1,15,16,17,255,32768,65535):
                self.setUp();self.run_case(C.c_int32(high|index).value,index<=16)
        for flag in range(256):
            self.setUp();self.table[0x120+16]=flag
            self.run_case(16,flag==2)
            self.assertEqual(self.scalar('test_mode_calls').value,int(flag==2))

    def test_actual_address_validation_and_clamped_dma_transfer(self):
        for address in (0,0x100F,0x1010,0x1011,0x7FFF0,0x7FFF8,0x7FFF9,0xFFFFFFFF,0x80000000):
            for size in (0,1,0x7EFF0,0x7EFF1,0xFFFFFFFF):
                self.setUp();index=7;self.word(0x180+index*4).value=address;self.word(0x138+index*4).value=size
                valid=0x1010<=address<=0x7FFF8;self.run_case(index,valid)
                self.assertEqual(self.scalar('test_mode_calls').value,1)
                self.assertEqual(self.scalar('test_address_calls').value,int(valid))
                self.assertEqual(self.scalar('D_800A4848').value,0)
                self.assertEqual(self.mode_scalar(0x47F4).value,0)
                self.assertEqual(self.mode_scalar(0x480C).value,0 if valid else 1)
                if valid:
                    self.assertEqual(list((C.c_int32*8).in_dll(self.lib,'test_commands'))[:3],[2,1,3])
                    self.assertEqual(C.c_void_p.in_dll(self.lib,'test_dma_source').value,C.addressof(self.values))
                    self.assertEqual(C.c_uint32.in_dll(self.lib,'test_dma_size').value,min(size,0x7EFF0))
                    self.assertEqual(C.c_uint32.in_dll(self.lib,'test_dma_address').value,(address+7)&~7)
        self.setUp();self.scalar('test_forced_address_result').value=0
        self.run_case(0,False)
        self.assertEqual(self.events,['mode','address'])
        self.assertEqual(self.mode_scalar(0x480C).value,1)

    def test_captured_allocation_fresh_size_and_final_publication(self):
        index=16;captured=self.word(0x180+index*4).value
        def mode_change():
            self.word(0x180+index*4).value=0xFFFFFFFF
            self.word(0x138+index*4).value=111
            self.table[0x120+index]=9
        def address_change():self.word(0x138+index*4).value=222
        def dma_change():
            self.table[0x120+index]=7
            C.c_void_p.in_dll(self.lib,'D_800A4810').value=C.cast(self.lib.test_completion,C.c_void_p).value
        self.on_mode=mode_change;self.on_address=address_change;self.on_dma=dma_change
        self.assertEqual(self.transfer(self.values,index),index)
        if self.errors:raise self.errors[0]
        self.assertEqual(self.events,['mode','address','dma'])
        self.assertEqual(C.c_uint32.in_dll(self.lib,'test_input_address').value,captured)
        self.assertEqual(C.c_uint32.in_dll(self.lib,'test_dma_size').value,222)
        self.assertEqual(self.table[0x120+index],1)
        self.assertEqual(self.mode_scalar(0x480C).value,99)
        self.assertEqual(C.c_void_p.in_dll(self.lib,'test_dma_source').value,C.addressof(self.values))


if __name__=='__main__':unittest.main()
