"""Actual BIOS/SIO buffer path: native LP64, original MIPS and linked MIPS.

Native buffers, file blocks and callbacks have real high addresses. The snapshot
serializes the buffer pointer only after checking that it lies in its real owner;
this is test transport, not a runtime pointer repair. Device polling is supplied
by the same deterministic callback service on native and MIPS. Existing target
register annotations are removed for the host; function bodies are otherwise used
unchanged. Init/control callback-slot unification remains separate source debt.
"""
import ctypes as C
from pathlib import Path
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32
from c_functions import address_of, definition_source
from overlay_units import symbol_addresses
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
BUFFER, FILE, IRQ, PORT = 0x80180010, 0x80190010, 0x1F801070, 0x1F801050
STACK, STOP, POLL = 0x803F0000, 0x803FF000, 0x80380000
SIZE = 12*4 + 16 + 128 + 44
HEADER = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "rr/serial.h"
#include "rr/bios_file_types.h"
#include "rr/interrupts.h"
#include "psyq/libsio/siorecv.h"
#include "psyq/libsio/siosend.h"
#include "psyq/libsio/siostart.h"
#include "psyq/libsio/siowait.h"
static struct {u8 before[16];union {u32 words[32];u8 bytes[128];} data;u8 after[16];} storage;
static union {SerialPortRegisters regs;u32 words[4];} port;
static BiosFileControlBlock file;
static InterruptRegisters irq;
InterruptRegisters *I_STAT_Sio=&irq;
SerialPortRegisters *SIO1_REGS=&port.regs;
SerialPortRegisters SioShadowRegs;
volatile SerialRequest SioRequest;
volatile SerialReceiveRequest SioReceiveRequest;
volatile s32 *SioDataReg=(s32 *)&port.words[0];
s16 SioRxFifoSizeTable[4]={1,2,4,8};
static unsigned scenario,direction,polls,events,eventFlag;
static int poll(int operation,int count) {
 assert(operation==(int)direction && count==(int)polls && polls<1000);polls++;
 if(scenario==2)return 0;
 port.regs.status=(port.regs.status^(u16)0x80)|(direction==1?2:5);
 return 1;
}
int (*SioPollHook)(int,int)=poll;
int DeliverEvent(u32 id,int flag) {
 assert(id==0xF000000BU);events++;eventFlag=flag;return -77;
}
#define KEEP_BASE(p) __asm__ volatile("" : "=r"(p) : "0"(p))
'''
TAIL = r'''
static u32 pointerWord(const u8 *p){
 uintptr_t n=(uintptr_t)p,base=(uintptr_t)storage.data.bytes;
 assert(n>=base && n<=base+128);return 0x80180010U+(u32)(n-base);
}
void run_case(unsigned kind,unsigned dir,unsigned fifo,unsigned length,unsigned plan,u8 *out){
 assert(sizeof(void*)==8 && (uintptr_t)&storage>UINT32_MAX && (uintptr_t)&file>UINT32_MAX);
 assert((uintptr_t)poll>UINT32_MAX && length<=32 && fifo<4);
 memset(&storage,0xA5,sizeof(storage));memset(storage.before,0xA6,16);memset(storage.after,0x6A,16);
 for(unsigned i=0;i<128;i++)storage.data.bytes[i]=(u8)(i*29+7);
 memset(&file,0xA5,sizeof(file));file.status=kind?0x8000:0;
 file.transfer_address=storage.data.bytes;file.transfer_length=(s32)length;file.device=0;
 BiosFileControlBlock expected=file;expected.error=0;
 memset(&port,0x5A,sizeof(port));port.regs.data=0xA7;
 port.regs.status=(plan==2||plan==3)?0:(dir==1?2:5);
 if(plan==4 && dir==1)port.regs.status=0x3A;
 port.regs.control=0x123;port.regs.mode=0x55;port.regs.baud=0x4567;
 memset(&SioShadowRegs,0,sizeof(SioShadowRegs));SioShadowRegs.control=fifo<<8;
 SioShadowRegs.mode=0x9F;SioShadowRegs.baud=0xDF;
 SioRequest.active=plan==1?-7:0;SioRequest.cursor=storage.data.bytes;SioRequest.remaining=0;
 SioReceiveRequest.request.active=plan==1?-7:0;SioReceiveRequest.request.cursor=storage.data.bytes;
 SioReceiveRequest.request.remaining=0;SioReceiveRequest.receive_status=0;
 irq.pending=0x100;irq.mask=0xFFFFFFFFU;
 direction=dir;scenario=plan;polls=events=eventFlag=0;
 SIO1_REGS=plan==1?0:&port.regs;
 int result=_sio_inout(&file,(int)dir);
 SIO1_REGS=&port.regs;
 if(dir!=1&&dir!=2){assert(result==-1);expected.error=0x16;}
 else if(plan==1){assert(result==-1);expected.error=kind?0x10:5;}
 else if(kind){
  assert(result==0);
  if(kind==2){
   unsigned limit=dir==1?(length?length/SioRxFifoSizeTable[fifo]:0):length+1;
   for(unsigned i=0;i<limit;i++){
    if(dir==2)port.regs.status=(port.regs.status&~0x80)|SioReceiveRequest.receive_status;
    assert(_sio_irq_handler()==0);
   }
   if(length || dir==2){
    assert((dir==1?SioRequest.active:SioReceiveRequest.request.active)==0);
    assert(events==1 && eventFlag==(dir==1?0x400U:0x800U));
   }
  }
 }else{
  int count=(plan==2||plan==4)?0:(int)length;
  assert(result==count);
  if(length && plan==2)assert(events==1&&eventFlag==0x100);
  if(length && plan==4 && dir==1)assert(events==1&&eventFlag==0x8000);
 }
 assert(!memcmp(&file,&expected,sizeof(file)));
 for(unsigned i=0;i<16;i++)assert(storage.before[i]==0xA6&&storage.after[i]==0x6A);
 for(unsigned i=length;i<128;i++)assert(storage.data.bytes[i]==(u8)(i*29+7));
 if(dir==2||plan==1||dir==0||dir>2||plan==2||plan==4||kind==1)
  for(unsigned i=0;i<128;i++)assert(storage.data.bytes[i]==(u8)(i*29+7));
 u32 words[12]={(u32)result,(u32)file.error,(u32)SioRequest.active,pointerWord(SioRequest.cursor),
  (u32)SioRequest.remaining,(u32)SioReceiveRequest.request.active,pointerWord(SioReceiveRequest.request.cursor),
  (u32)SioReceiveRequest.request.remaining,SioReceiveRequest.receive_status,events,eventFlag,polls};
 memcpy(out,words,sizeof(words));out+=sizeof(words);memcpy(out,&port,16);out+=16;
 memcpy(out,storage.data.bytes,128);out+=128;
 u32 fileWords[11]={file.status,file.disk_or_port,pointerWord(file.transfer_address),
  (u32)file.transfer_length,file.position,file.device_flags,(u32)file.error,0,file.size,file.logical_block,file.number};
 memcpy(out,fileWords,sizeof(fileWords));
}
'''


class SerialBufferPipelineTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        directory = Path(cls.temp.name)
        bodies = ''
        for unit, names in (
            ('siorecv', ('_sio_read',)), ('siosend', ('_sio_write',)),
            ('siostart', ('_sio_write_async',)), ('siowait', ('_sio_read_async',)),
            ('sioint', ('_sio_inout', '_sio_irq_handler'))):
            source = (ROOT/f'src/main/PAL/psyq/libsio/{unit}.c').read_text()
            for name in names:
                body = definition_source(source, name)
                # Only target register names/clobbers are host-specific.
                body = re.sub(r'\s+(?:__asm__|asm)\("\$[0-9]+"\)', '', body)
                body = re.sub(r'"\$[0-9]+",\s*', '', body)
                bodies += body
        path=directory/'serial.c';path.write_text(HEADER+bodies+TAIL)
        binary=directory/'serial.so'
        result=subprocess.run(['cc','-std=gnu11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                               '-fsanitize=undefined,bounds','-fno-sanitize-recover=all',
                               '-I',str(ROOT/'include'),str(path),'-o',str(binary)],capture_output=True,text=True)
        if result.returncode:raise AssertionError(result.stderr)
        cls.lib=C.CDLL(str(binary));cls.lib.run_case.argtypes=[C.c_uint]*5+[C.c_void_p]

    def native(self,kind,direction,fifo,length,plan):
        out=(C.c_uint8*SIZE)()
        self.lib.run_case(kind,direction,fifo,length,plan,out)
        return bytes(out)

    def test_native_complete_buffers_busy_errors_polling_and_fifo_widths(self):
        for fifo in range(4):
            for direction in (0,1,2,3,0xFFFFFFFF):
                for length in (0,1,2,4,8,16,31,32):
                    for plan in range(5):
                        if plan==4 and direction!=1:continue
                        self.native(0,direction,fifo,length,plan)
                        self.native(1,direction,fifo,length,0 if plan>1 else plan)
                for chunks in range(4):
                    if direction in (1,2):self.native(2,direction,fifo,chunks*(1<<fifo),0)

    def test_original_and_linked_mips_match_the_native_pipeline(self):
        addresses=symbol_addresses()
        for elf_path in (None,ROOT/'build/PAL/main.elf'):
            memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN)
            memory.mem_map(0,0x400000);memory.mem_map(0x1F800000,0x2000)
            write=lambda a,b:memory.mem_write(a&0x1FFFFFFF,bytes(b))
            read=lambda a,n:bytes(memory.mem_read(a&0x1FFFFFFF,n))
            put32=lambda a,v:write(a,struct.pack('<I',v&0xFFFFFFFF))
            image=(ROOT/'assets/PAL/main.exe').read_bytes();base=struct.unpack_from('<I',image,24)[0]
            write(base,image[0x800:])
            if elf_path:
                with elf_path.open('rb') as stream:
                    elf=ELFFile(stream)
                    for section in elf.iter_sections():
                        if section['sh_flags']&2 and section['sh_type']!='SHT_NOBITS':
                            write(section['sh_addr'],section.data())
            cpu=Cpu(memory,0,100,101,102,lambda *args:None)
            events=[];polls=[]
            def invoke(name,*args):
                cpu.reset();cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)]
                cpu.r[29]=STACK;cpu.r[31]=STOP;cpu.pc=address_of(name)
                for i,arg in enumerate(args):cpu.r[4+i]=arg&0xFFFFFFFF
                for step in range(10000):
                    if cpu.pc==STOP:break
                    if cpu.pc==address_of('DeliverEvent'):
                        self.assertEqual(cpu.r[4],0xF000000B);events.append(cpu.r[5]);result=0xFFFFFFB3
                    elif cpu.pc==POLL:
                        self.assertEqual((cpu.r[4],cpu.r[5]),(direction,len(polls)))
                        polls.append(cpu.r[5]);result=int(plan!=2)
                        if result:
                            status=int.from_bytes(read(PORT+4,2),'little')
                            write(PORT+4,struct.pack('<H',(status^0x80)|(2 if direction==1 else 5)))
                    else:cpu.step();continue
                    resume=cpu.r[31]
                    for register in (2,3,4,5,6,7,8,9,10,11,12,13,14,15,24,25):cpu.r[register]=0xDEAD0000+register
                    cpu.r[2]=result;cpu.pc=resume
                self.assertEqual(cpu.pc,STOP,(name,kind,direction,fifo,length,plan))
                self.assertEqual(cpu.r[29],STACK)
                for register in (*range(16,24),28,30):self.assertEqual(cpu.r[register],0xBEEF0000+register)
                return cpu.r[2]
            cases=[]
            for fifo in range(4):
                for direction in (0,1,2,3,0xFFFFFFFF):
                    for kind in (0,1):
                        for length in (0,1,8,32):
                            for plan in range(5):
                                if plan==4 and direction!=1:continue
                                cases.append((kind,direction,fifo,length,plan))
                    if direction in (1,2):
                        for chunks in range(4):cases.append((2,direction,fifo,chunks*(1<<fifo),0))
            for kind,direction,fifo,length,plan in cases:
                expected=self.native(kind,direction,fifo,length,plan)
                events.clear();polls.clear()
                write(BUFFER-16,b'\xA6'*16+bytes((i*29+7)&255 for i in range(128))+b'\x6A'*16)
                words=[0x8000 if kind else 0,0xA5A5A5A5,BUFFER,length,0xA5A5A5A5,0xA5A5A5A5,0xA5A5A5A5,0,0xA5A5A5A5,0xA5A5A5A5,0xA5A5A5A5]
                write(FILE,struct.pack('<11I',*words));write(PORT,b'\x5A'*16)
                write(PORT,b'\xA7');status=0 if plan in (2,3) else (2 if direction==1 else 5)
                if plan==4 and direction==1:status=0x3A
                write(PORT+4,struct.pack('<H',status));write(PORT+8,struct.pack('<HHHH',0x55,0x123,0x5A5A,0x4567))
                write(IRQ,struct.pack('<II',0x100,0xFFFFFFFF))
                put32(addresses['SIO1_REGS'],0 if plan==1 else PORT)
                put32(addresses['SioDataReg'],PORT);put32(addresses['I_STAT_Sio'],IRQ)
                put32(addresses['SioPollHook'],POLL)
                write(addresses['SioShadowRegs'],b'\0'*16)
                write(addresses['SioShadowRegs']+8,struct.pack('<HHHH',0x9F,fifo<<8,0,0xDF))
                active=0xFFFFFFF9 if plan==1 else 0
                write(addresses['SioRequest'],struct.pack('<III',active,BUFFER,0))
                write(addresses['SioReceiveRequest'],struct.pack('<IIII',active,BUFFER,0,0))
                result=invoke('_sio_inout',FILE,direction);put32(addresses['SIO1_REGS'],PORT)
                if kind==2:
                    limit=(length//(1<<fifo) if length else 0) if direction==1 else length+1
                    for i in range(limit):
                        if direction==2:
                            receive=struct.unpack('<4I',read(addresses['SioReceiveRequest'],16))
                            status=int.from_bytes(read(PORT+4,2),'little')
                            write(PORT+4,struct.pack('<H',(status&~0x80)|receive[3]))
                        self.assertEqual(invoke('_sio_irq_handler'),0)
                send=read(addresses['SioRequest'],12);receive=read(addresses['SioReceiveRequest'],16)
                error=struct.unpack('<I',read(FILE+24,4))[0]
                actual=struct.pack('<II',result,error)+send+receive+struct.pack('<III',len(events),events[-1]if events else 0,len(polls))+read(PORT,16)+read(BUFFER,128)+read(FILE,44)
                self.assertEqual(actual,expected,(str(elf_path),kind,direction,fifo,length,plan))
                self.assertEqual(read(BUFFER-16,16),b'\xA6'*16)
                self.assertEqual(read(BUFFER+128,16),b'\x6A'*16)

    def test_init_resets_complete_native_buffer_pointers_and_named_registers(self):
        body=definition_source((ROOT/'src/main/PAL/psyq/libsio/sioint.c').read_text(),'_sio_init')
        source=r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "rr/serial.h"
#include "rr/interrupts.h"
SerialPortRegisters SioShadowRegs,port,*SIO1_REGS=&port;
volatile SerialRequest SioRequest;
volatile SerialReceiveRequest SioReceiveRequest;
static InterruptRegisters irq;
InterruptRegisters *I_STAT_Sio=&irq;
/* The existing control/init slot is still a scalar; this test covers buffer
 * ownership only and does not pretend to fix the callback interface. */
volatile s32 SioPollHook;
char SioIrqQueueEntry[32];
static u8 buffers[2][64];
static unsigned calls;
void SysDeqIntRP(int channel,char *entry){
 assert(calls++==0 && channel==3 && entry==SioIrqQueueEntry);
 assert(SioRequest.cursor==buffers[0] && SioReceiveRequest.request.cursor==buffers[1]);
 SioRequest.cursor=buffers[1];SioReceiveRequest.request.cursor=buffers[0];
}
void SysEnqIntRP(int channel,char *entry){assert(calls++==1 && channel==3 && entry==SioIrqQueueEntry);}
'''+body+r'''
int main(void){
 assert(sizeof(void*)==8 && (uintptr_t)buffers>UINT32_MAX);
 for(u32 i=0;i<1024;i++){
  memset(&SioShadowRegs,0xA5,sizeof(SioShadowRegs));memset(&port,0x5A,sizeof(port));
  SioRequest.active=-7;SioRequest.cursor=buffers[0];SioRequest.remaining=0x12345678;
  SioReceiveRequest.request.active=17;SioReceiveRequest.request.cursor=buffers[1];
  SioReceiveRequest.request.remaining=-123;SioReceiveRequest.receive_status=0x12345678U;
  irq.pending=0x87654321;irq.mask=i*0x31415927U;SioPollHook=-1;calls=0;
  assert(_sio_init()==0 && calls==2);
  assert(!SioRequest.cursor && !SioReceiveRequest.request.cursor);
  assert(!SioRequest.active && !SioRequest.remaining && !SioReceiveRequest.request.active && !SioReceiveRequest.request.remaining);
  assert(SioReceiveRequest.receive_status==0x12345678U && !SioPollHook);
  assert(irq.pending==0x87654321 && irq.mask==((i*0x31415927U)|0x100U));
  assert(SioShadowRegs.mode==0xCE && SioShadowRegs.control==5 && SioShadowRegs.baud==0xD8);
  assert(port.mode==0xCE && port.control==5 && port.baud==0xD8);
  assert(port.data==0x5A && port.status==0x5A5A);
 }
 return 0;
}
'''
        with TemporaryDirectory() as temporary:
            directory=Path(temporary);path=directory/'init.c';path.write_text(source)
            binary=directory/'init'
            result=subprocess.run(['cc','-std=gnu11','-O2','-Wall','-Wextra','-Werror',
                                   '-fsanitize=undefined,bounds','-fno-sanitize-recover=all',
                                   '-I',str(ROOT/'include'),str(path),'-o',str(binary)],capture_output=True,text=True)
            self.assertEqual(result.returncode,0,result.stderr)
            subprocess.run([str(binary)],check=True)
