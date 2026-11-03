"""Actual disc probe and typed completion/sector wrappers against controlled I/O."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest


def native_body(text):
    text=re.sub(r'^#include[^\n]*\n','',text,flags=re.M)
    text=re.sub(r'\s+__asm__\("\$[0-9]+"\)','',text)
    return re.sub(r'\s*:\s*"\$[0-9]+"(?:\s*,\s*"\$[0-9]+")*(?=\))','',text)


class CdDiscProbeTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup)
        root=Path(__file__).resolve().parents[2];d=Path(cls.temp.name)
        header=(root/'include/rr/cd.h').read_text()
        header=re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',header,flags=re.M)
        body=native_body((root/'build/legacy/func_8008F3B0.c').read_text())
        assert body.count('(const char *)0x80010000U')==1
        body=body.replace('(const char *)0x80010000U','message_page')
        code='''#include <stdint.h>
#include <string.h>
#include <stdarg.h>
typedef uint8_t u8;typedef uint32_t u32;typedef int32_t s32;
'''+header+'''
char message_page[0x1D78+16],D_80011D88[]="CD001";
s32 initial_status,final_status,second_status,ready_attempt,poll_error,sector_return;
s32 polls,command_count,commands[16],diagnostics,transfers,comparisons,sector_words,layout_ok;
u8 sector[2048];u8 *status_pointer,*parameter_pointer,*sector_pointer;
s32 func_8008C140(s32 command,const u8 *parameters,u8 *result){
    commands[command_count++]=command;
    if(command==1){memset(result,0xA5,8);result[0]=initial_status;status_pointer=result;}
    else if(command==0x1B){
        parameter_pointer=(u8 *)parameters;
        if(parameters[0]!=0 || parameters[1]!=2 || parameters[2]!=0x16)layout_ok=0;
        if(result!=0)layout_ok=0;
    }else if(command==9 && (parameters!=0 || result!=0))layout_ok=0;
    return -123;
}
s32 func_8008CF00(s32 mode,u8 *status){
    polls++;if(mode!=0 || status!=status_pointer)layout_ok=0;
    status[0]=final_status;status[1]=second_status;
    return polls==ready_attempt?1:poll_error;
}
s32 func_8008DBB4(u8 *buffer,u32 words){
    transfers++;sector_words=words;sector_pointer=buffer;
    if(buffer+2048!=status_pointer || parameter_pointer+8!=buffer || (uintptr_t)buffer%4)layout_ok=0;
    memcpy(buffer,sector,2048);return sector_return;
}
s32 func_80096414(const void *left,const void *right,u32 count){
    comparisons++;if(left!=sector_pointer+1 || right!=D_80011D88 || count!=5)layout_ok=0;
    return memcmp(left,right,count);
}
s32 func_8001E84C(const char *message,...){
    diagnostics++;if(message!=message_page+0x1D78 || strcmp(message,"Command Error: "))layout_ok=0;
    return -456;
}
'''+body
        for name in ('func_8008C0F8','func_8008C51C','func_8008C5A0'):
            code+='\n'+native_body((root/'build/legacy'/f'{name}.c').read_text())
        (d/'f.c').write_text(code)
        subprocess.run(['cc','-shared','-fPIC','-O2',str(d/'f.c'),'-o',str(d/'f.so')],check=True,capture_output=True)
        cls.lib=C.CDLL(str(d/'f.so'));cls.f=cls.lib.func_8008F3B0;cls.f.argtypes=[];cls.f.restype=C.c_int32
        cls.values={name:C.c_int32.in_dll(cls.lib,name) for name in ('initial_status','final_status','second_status','ready_attempt','poll_error','sector_return','polls','command_count','diagnostics','transfers','comparisons','sector_words','layout_ok')}
        cls.commands=(C.c_int32*16).in_dll(cls.lib,'commands');cls.sector=(C.c_uint8*2048).in_dll(cls.lib,'sector')
        cls.page=(C.c_uint8*(0x1D78+16)).in_dll(cls.lib,'message_page')
        exe=(root/'assets/PAL/main.exe').read_bytes()
        message=exe[0x800+0x1D78:0x800+0x1D78+16]
        assert message==b'Command Error: \0'
        C.memmove(C.addressof(cls.page)+0x1D78,message,len(message))

    def reset(self,**values):
        for name,value in self.values.items():value.value=0
        self.values['layout_ok'].value=1;self.values['poll_error'].value=0
        self.commands[:]=[0]*16
        C.memset(self.sector,0x5A,2048);C.memmove(C.addressof(self.sector)+1,b'CD001',5)
        for name,value in values.items():self.values[name].value=value

    def run_probe(self):
        before=(bytes(self.sector),bytes(self.page))
        result=self.f()
        self.assertEqual((bytes(self.sector),bytes(self.page)),before)
        self.assertEqual(self.values['layout_ok'].value,1)
        return result

    def trace(self):return list(self.commands)[:self.values['command_count'].value]

    def test_all_initial_status_bytes_and_early_tray_gate(self):
        for status in range(256):
            self.reset(initial_status=status,ready_attempt=1)
            self.assertEqual(self.run_probe(),16 if status&0x10 else 2)
            self.assertEqual(self.trace(),[1] if status&0x10 else [1,27,9])
            self.assertEqual(self.values['polls'].value,0 if status&0x10 else 1)

    def test_success_on_each_poll_and_exact_retry_limit(self):
        for attempt in range(1,11):
            for error in (-2147483648,-1,0,2,2147483647):
                self.reset(ready_attempt=attempt,poll_error=error)
                self.assertEqual(self.run_probe(),2)
                self.assertEqual(self.values['polls'].value,attempt)
                self.assertEqual(self.trace(),[1]+[27]*attempt+[9])
                self.assertEqual((self.values['transfers'].value,self.values['sector_words'].value,self.values['comparisons'].value),(1,512,1))
        self.reset(ready_attempt=11)
        self.assertEqual(self.run_probe(),0);self.assertEqual(self.values['polls'].value,10)
        self.assertEqual(self.trace(),[1]+[27]*10)

    def test_failed_completion_status_precedence_and_diagnostic(self):
        for first in range(256):
            for second in (0,0x40,0xFF):
                self.reset(final_status=first,second_status=second,poll_error=-1)
                diagnostic=not(first&0x10) and bool(first&1 and second&0x40)
                expected=16 if first&0x10 else int(diagnostic or bool(first&2))
                self.assertEqual(self.run_probe(),expected)
                self.assertEqual(self.values['polls'].value,10)
                self.assertEqual(self.values['diagnostics'].value,int(diagnostic))
                self.assertEqual((self.values['transfers'].value,self.values['comparisons'].value),(0,0))

    def test_iso_signature_each_position_and_ignored_sector_return(self):
        for raw_result in (-2147483648,-1,0,1,2147483647):
            for position in (None,0,1,2,3,4):
                self.reset(ready_attempt=1,sector_return=raw_result,final_status=0xFF,second_status=0xFF)
                if position is not None:self.sector[position+1]^=0x80
                self.assertEqual(self.run_probe(),2 if position is None else 1)
                self.assertEqual(self.trace(),[1,27,9]);self.assertEqual(self.values['diagnostics'].value,0)
