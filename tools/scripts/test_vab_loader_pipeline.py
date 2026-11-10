"""Real VAB constructor and two-file loader, native LP64 and both MIPS images.

The original scalar snapshots keep their PAL protocol. Production code uses
the complete pointer-bearing AudioState; unused pointers and native alignment
gaps are guarded. VabData and loading cursors retain complete native pointers.
SDK/CD services can mutate live state and descriptors at call boundaries.
"""
import ctypes as C
from pathlib import Path
import random
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from c_functions import address_of, definition_source
from audio_fixture_transport import wire_types, transport, scalar_entry
from test_vehicle_initializer import Cpu

ROOT=Path(__file__).resolve().parents[2]
AUDIO_SIZE=0xA8D0
PAYLOADS=(0x80200000,0x80210000,0x80220000)
STACK,STOP=0x803F0000,0x803FF000
SERVICES=(('SsVabClose',1,()),('SsVabOpenHeadSticky',3,(0,)),
          ('SsVabTransBody',2,(0,)),('SsVabTransCompleted',1,()),
          ('DebugPrintStub',1,(0,)),('PsyqExit',1,()),
          ('GetR4FileIndex',2,()),('CdFileReadStart',2,(1,)),('CdFileBytesLoaded',0,()))
PREFIX=r'''
#include <stdint.h>
#include <stddef.h>
#include <setjmp.h>
#include "game/sound/sndvab.h"
#include "game/data/trkld1.h"
#include "game/data/cdfile2.h"
#include "game/course/trkres2.h"
#include "rr/audio_bank_setup.h"
#include "rr/diagnostics.h"
#include "psyq/libc/exit.h"
AudioState SoundState;
VabData VabLoadArgs;
char *RaceLoadCursor;
s32 RaceLoadStepIndex, RaceLoadState;
VabLoadConfig D_800A0228[4];
char VabOpenHeadErrorStr[64],D_800110F0[64];
typedef uint64_t (*Observer)(u32,const uintptr_t *);
static Observer observer;
static jmp_buf exit_context;
void set_observer(Observer value){observer=value;}
void SsVabClose(s16 bank){uintptr_t args[]={(u32)(s32)bank};observer(0,args);}
s32 SsVabOpenHeadSticky(u8 *head,s32 bank,s32 mode){uintptr_t args[]={(uintptr_t)head,(u32)bank,(u32)mode};return(s32)observer(1,args);}
s32 SsVabTransBody(const u16 *samples,s32 bank){uintptr_t args[]={(uintptr_t)samples,(u32)bank};return(s32)observer(2,args);}
s32 SsVabTransCompleted(s32 wait){uintptr_t args[]={(u32)wait};return(s32)observer(3,args);}
s32 DebugPrintStub(const char *format,...){uintptr_t args[]={(uintptr_t)format};return(s32)observer(4,args);}
/* ROM entry is BIOS B(38h): termination, not a returning service.
 * https://psx-spx.consoledev.net/ps1/kernelbios/misc-functions/ */
void PsyqExit(s32 status){uintptr_t args[]={(u32)status};observer(5,args);longjmp(exit_context,1);}
s32 GetR4FileIndex(s32 group,s32 entry){uintptr_t args[]={(u32)group,(u32)entry};return(s32)observer(6,args);}
s32 CdFileReadStart(s32 entry,void *buffer){uintptr_t args[]={(u32)entry,(uintptr_t)buffer};return(s32)observer(7,args);}
s32 CdFileBytesLoaded(void){uintptr_t args[1]={0};return(s32)observer(8,args);}
_Static_assert(sizeof(void*)==8,"LP64 exercise");
_Static_assert(sizeof(VabData)==16,"two complete native pointers");
_Static_assert(sizeof(AudioBankSetup)==32,"complete public native bank descriptor");
_Static_assert(offsetof(AudioBankSetup,slot0C)==24,"native pointer fields precede the scalar slot");
'''


class Pair(C.Structure):
    _fields_=[('header',C.c_void_p),('samples',C.c_void_p)]


class Config(C.Structure):
    _fields_=[('file_index',C.c_int32),('open_mode',C.c_int32),('bank_slot',C.c_int32)]


class VabLoaderPipelineTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temporary=TemporaryDirectory();cls.addClassCleanup(cls.temporary.cleanup)
        directory=Path(cls.temporary.name)
        prefix=wire_types()+PREFIX.replace('AudioState SoundState;',
             'AudioWireState audio_wire_model;\n#define SoundState audio_wire_model')
        start=prefix.index('void SsVabClose(')
        bridge=transport('audio_wire_model')+r'''
static uint64_t audio_observe(u32 kind,const uintptr_t *arguments){
 audio_before_callback();uint64_t result=observer(kind,arguments);
 audio_after_callback();return result;
}
'''
        prefix=prefix[:start]+bridge+prefix[start:]
        prefix=prefix.replace('observer(0,args)', 'audio_observe(0,args)')
        for kind in range(1,9):
            prefix=prefix.replace(f'observer({kind},args)', f'audio_observe({kind},args)')
        code=prefix
        for unit,names in (('game/sound/sndvab',('LoadVab',)),
                           ('game/data/trkld1',('SetRaceLoadCursor','GetRaceLoadCursor','LoadVabPairStep')),
                           ('game/course/trkres2',('AlignUp4',))):
            text=(ROOT/f'src/main/PAL/{unit}.c').read_text()
            for name in names:
                body=definition_source(text,name)
                body=re.sub(r'\s+(?:__asm__|asm)\("\$[0-9]+"\)','',body)
                if name=='LoadVab':
                    entry=scalar_entry(body,name,'audio_wire_model',parameters='const VabData *data,s32 mode,s32 slot',arguments='data,mode,slot')
                    code+=entry[len(transport('audio_wire_model')):]
                else:code+=body
        code += r'''
s32 invoke_vab(u32 caller,char *header,s32 mode,s32 bank,s32 *result){
 if(setjmp(exit_context)){audio_active=audio_callbacks=0;return 1;}
 if(caller)*result=LoadVabPairStep(header);
 else {LoadVab(&VabLoadArgs,mode,bank);*result=0;}
 return 0;
}
'''
        source=directory/'vab.c';source.write_text(code)
        result=subprocess.run(['cc','-std=gnu11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                               '-fsanitize=undefined,bounds','-fno-sanitize-recover=all',
                               '-I',str(ROOT/'include'),str(source),'-o',str(directory/'vab.so')],capture_output=True,text=True)
        if result.returncode:raise AssertionError(result.stderr)
        cls.library=C.CDLL(str(directory/'vab.so'))
        cls.library.LoadVab.argtypes=[C.POINTER(Pair),C.c_int32,C.c_int32];cls.library.LoadVab.restype=None
        cls.library.invoke_vab.argtypes=[C.c_uint32,C.c_void_p,C.c_int32,C.c_int32,C.POINTER(C.c_int32)];cls.library.invoke_vab.restype=C.c_int32
        cls.library.LoadVabPairStep.argtypes=[C.c_void_p];cls.library.LoadVabPairStep.restype=C.c_int32
        cls.library.SetRaceLoadCursor.argtypes=[C.c_void_p];cls.library.SetRaceLoadCursor.restype=None
        cls.library.GetRaceLoadCursor.argtypes=[];cls.library.GetRaceLoadCursor.restype=C.c_void_p
        cls.library.AlignUp4.argtypes=[C.c_ulong];cls.library.AlignUp4.restype=C.c_ulong
        cls.audio=(C.c_ubyte*AUDIO_SIZE).in_dll(cls.library,'audio_wire_model')
        cls.pair=Pair.in_dll(cls.library,'VabLoadArgs')
        cls.cursor=C.c_void_p.in_dll(cls.library,'RaceLoadCursor')
        cls.phase=C.c_int32.in_dll(cls.library,'RaceLoadStepIndex')
        cls.load_state=C.c_int32.in_dll(cls.library,'RaceLoadState')
        cls.table=(Config*4).in_dll(cls.library,'D_800A0228')
        cls.messages=[(C.c_ubyte*64).in_dll(cls.library,n)for n in ('VabOpenHeadErrorStr','D_800110F0')]
        cls.payload=[(C.c_uint16*128)()for _ in range(3)]
        cls.native_map=[(C.addressof(cls.audio),address_of('SoundState'),AUDIO_SIZE)]
        cls.native_map += [(C.addressof(p),a,256)for p,a in zip(cls.payload,PAYLOADS)]
        cls.native_map += [(C.addressof(p),address_of(n),64)for p,n in zip(cls.messages,('VabOpenHeadErrorStr','D_800110F0'))]
        if not all(n>0xFFFFFFFF for n,_,_ in cls.native_map):raise AssertionError('Native owners were not allocated above 4 GiB')

    def machines(self):
        image=(ROOT/'assets/PAL/main.exe').read_bytes();base=struct.unpack_from('<I',image,24)[0]
        for linked in (False,True):
            memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000)
            memory.mem_write(base&0x1FFFFFFF,image[0x800:])
            if linked:
                with (ROOT/'build/PAL/main.elf').open('rb')as stream:
                    elf=ELFFile(stream)
                    for section in elf.iter_sections():
                        if section['sh_flags']&2 and section['sh_type']!='SHT_NOBITS':
                            memory.mem_write(section['sh_addr']&0x1FFFFFFF,section.data())
            self.memory=memory
            allowed=[(address_of('SoundState'),AUDIO_SIZE),(address_of('VabLoadArgs'),8),
                     (address_of('RaceLoadCursor'),4),(address_of('RaceLoadStepIndex'),4),(address_of('RaceLoadState'),4),(STACK-96,96)]
            def store(cpu,access,address,size,value,user):
                p=address&0x1FFFFFFF
                self.assertTrue(any((a&0x1FFFFFFF)<=p and p+size<=(a&0x1FFFFFFF)+n for a,n in allowed),(hex(address),size))
            yield linked,Cpu(memory,0,100,101,102,store)

    def read(self,address,size):return bytes(self.memory.mem_read(address&0x1FFFFFFF,size))
    def write(self,address,data):self.memory.mem_write(address&0x1FFFFFFF,bytes(data))
    def word(self,address):return struct.unpack('<I',self.read(address,4))[0]
    def put(self,address,value):self.write(address,struct.pack('<I',value&0xFFFFFFFF))

    def native_address(self,address):
        for host,pal,size in self.native_map:
            if pal<=address<pal+size:return host+address-pal
        raise AssertionError(('unowned PAL address',hex(address)))

    def wire_address(self,pointer):
        for host,pal,size in self.native_map:
            if host<=pointer<host+size:return pal+pointer-host
        raise AssertionError(('unowned or truncated native pointer',hex(pointer)))

    def snapshot(self,native):
        if native:
            audio=bytes(self.audio);pair=(self.wire_address(self.pair.header),self.wire_address(self.pair.samples))
            cursor=self.wire_address(self.cursor.value);phase=self.phase.value&0xFFFFFFFF
            load_state=self.load_state.value&0xFFFFFFFF
            table=bytes(self.table);payload=tuple(C.string_at(C.addressof(p),256)for p in self.payload)
        else:
            audio=self.read(address_of('SoundState'),AUDIO_SIZE)
            pair=struct.unpack('<II',self.read(address_of('VabLoadArgs'),8));cursor=self.word(address_of('RaceLoadCursor'))
            phase=self.word(address_of('RaceLoadStepIndex'));load_state=self.word(address_of('RaceLoadState'));table=self.read(address_of('D_800A0228'),48)
            payload=tuple(self.read(p,256)for p in PAYLOADS)
        return audio,pair,cursor,phase,table,payload,load_state

    def initialize(self,case,state,bank,native):
        self.case,self.active_bank=case,bank
        audio=bytearray([case&255])*AUDIO_SIZE
        handles=(-32768,-1,0,1,32767)
        for i in range(10):struct.pack_into('<h',audio,0x24+i*2,handles[(case+i)%5])
        payload=[bytes((case*17+i*29+index*53)&255 for i in range(256))for index in range(3)]
        table=struct.pack('<12I',0xA5A5A5A5,case*0x31415927&0xFFFFFFFF,bank,*([0x69696969]*9))
        self.mode=struct.unpack_from('<i',table,4)[0]
        if native:
            C.memmove(C.addressof(self.audio),bytes(audio),AUDIO_SIZE)
            for obj,data in zip(self.payload,payload):C.memmove(C.addressof(obj),data,256)
            C.memmove(C.addressof(self.table),table,48)
            self.pair.header=self.native_address(PAYLOADS[0]+16);self.pair.samples=self.native_address(PAYLOADS[1]+32)
            self.library.SetRaceLoadCursor(self.native_address(PAYLOADS[1]));self.assertEqual(self.library.GetRaceLoadCursor(),self.cursor.value);self.assertEqual(self.load_state.value,0)
            self.phase.value=state
        else:
            self.write(address_of('SoundState'),audio)
            for address,data in zip(PAYLOADS,payload):self.write(address,data)
            self.write(address_of('D_800A0228'),table);self.write(address_of('VabLoadArgs'),struct.pack('<II',PAYLOADS[0]+16,PAYLOADS[1]+32))
            self.put(address_of('RaceLoadCursor'),PAYLOADS[1]);self.put(address_of('RaceLoadStepIndex'),state);self.put(address_of('RaceLoadState'),0)

    def service(self,index,args,native):
        name,count,pointers=SERVICES[index];args=list(args)
        for at in range(count):args[at]=self.wire_address(args[at])if native and at in pointers else args[at]&0xFFFFFFFF
        self.events.append((name,tuple(args),self.snapshot(native)))
        if name=='SsVabClose'and self.case&1:
            self.set_pair_header(PAYLOADS[2]+16,native)
        if name=='SsVabOpenHeadSticky'and self.case&2:
            self.set_pair_samples(PAYLOADS[2]+64,native)
        if name in ('SsVabClose','SsVabOpenHeadSticky')and self.case&4:
            address=address_of('SoundState')+0x24+self.active_bank*2
            if native:C.c_int16.from_address(self.native_address(address)).value=12345
            else:self.write(address,struct.pack('<h',12345))
        if name=='SsVabOpenHeadSticky'and self.case&8:
            self.set_cursor(PAYLOADS[0]+128,native)
            if native:self.phase.value=59
            else:self.put(address_of('RaceLoadStepIndex'),59)
        if name=='CdFileBytesLoaded'and self.case&16:
            self.set_cursor(PAYLOADS[2]+32,native)
            self.active_bank=(self.active_bank+1)%10
            if native:self.table[0].bank_slot=self.active_bank
            else:self.put(address_of('D_800A0228')+8,self.active_bank)
        returns=(-1,0,1,15,32767,32768,65535,0x1234FFFF,-0x80000000,0x7FFFFFFF)
        if name=='SsVabOpenHeadSticky':return returns[self.case%len(returns)]&0xFFFFFFFF
        if name=='SsVabTransBody':return returns[(self.case//3+1)%len(returns)]&0xFFFFFFFF
        if name=='GetR4FileIndex':return (args[1]*7+0x100+self.case)&0xFFFFFFFF
        if name=='CdFileReadStart':return (0,1,0xFFFFFFFF)[self.case%3]
        if name=='CdFileBytesLoaded':return (0,1,73,0xFFFFFFFF)[(self.case//3)%4]
        return 0

    def set_pair_header(self,value,native):
        if native:self.pair.header=self.native_address(value)
        else:self.put(address_of('VabLoadArgs'),value)
    def set_pair_samples(self,value,native):
        if native:self.pair.samples=self.native_address(value)
        else:self.put(address_of('VabLoadArgs')+4,value)
    def set_cursor(self,value,native):
        if native:self.cursor.value=self.native_address(value)
        else:self.put(address_of('RaceLoadCursor'),value)

    def run_mips(self,cpu,name,args):
        targets={address_of(n):i for i,(n,_,_)in enumerate(SERVICES)}
        cpu.reset();cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)];cpu.r[29]=STACK;cpu.r[31]=STOP;cpu.pc=address_of(name)
        for i,arg in enumerate(args):cpu.r[4+i]=arg&0xFFFFFFFF
        for instruction in range(4000):
            if cpu.pc==STOP:break
            if cpu.pc not in targets:cpu.step();continue
            index=targets[cpu.pc];count=SERVICES[index][1]
            result=self.service(index,cpu.r[4:4+count],False)
            if SERVICES[index][0]=='PsyqExit':
                # No return or caller continuation exists after BIOS termination.
                self.assertEqual(cpu.r[4],1)
                return None
            resume=cpu.r[31]
            for register in (2,3,4,5,6,7,8,9,10,11,12,13,14,15,24,25):cpu.r[register]=0xDEAD0000+register
            cpu.r[2]=result;cpu.pc=resume
        self.assertEqual(cpu.pc,STOP,(self.case,name,args));self.assertEqual(cpu.r[29],STACK)
        for register in (*range(16,24),28,30):self.assertEqual(cpu.r[register],0xBEEF0000+register)
        return C.c_int32(cpu.r[2]).value

    def test_real_constructor_and_two_file_caller_keep_high_pointers_and_reload_fields(self):
        callback_type=C.CFUNCTYPE(C.c_uint64,C.c_uint32,C.POINTER(C.c_size_t));errors=[]
        def observe(index,args):
            try:return self.service(index,[args[j]for j in range(SERVICES[index][1])],True)
            except BaseException as error:errors.append(error);return 0
        callback=callback_type(observe);self.library.set_observer.argtypes=[callback_type];self.library.set_observer(callback)
        image=(ROOT/'assets/PAL/main.exe').read_bytes();base=struct.unpack_from('<I',image,24)[0]
        for obj,name in zip(self.messages,('VabOpenHeadErrorStr','D_800110F0')):
            at=0x800+address_of(name)-base;C.memmove(C.addressof(obj),image[at:at+64],64)
        coverage=set()
        for linked,cpu in self.machines():
            for case in range(360):
                bank=case%10;state=(-1,0,1,2,3,4)[(case//10)%6]
                for caller in (False,True):
                    self.initialize(case,state,bank,True);self.events=[];errors.clear()
                    output=C.c_int32(0x12345678)
                    terminated=bool(self.library.invoke_vab(caller,self.native_address(PAYLOADS[0]),self.mode,bank,C.byref(output)))
                    expected_result=output.value
                    if terminated:self.assertEqual(expected_result,0x12345678)
                    if errors:raise errors[0]
                    expected_events=self.events;expected_final=self.snapshot(True)
                    coverage.update(event[0]for event in expected_events)
                    if not caller:
                        statuses=(-1,0,1,15,32767,32768,65535,0x1234FFFF,-0x80000000,0x7FFFFFFF)
                        opened=C.c_int16(statuses[case%10]).value
                        transferred=C.c_int16(statuses[(case//3+1)%10]).value
                        self.assertEqual(terminated,opened==-1 or transferred==-1)
                        self.assertEqual(expected_events[-1][0],'PsyqExit'if terminated else'SsVabTransCompleted')
                        expected_handle=-1 if opened==-1 else transferred
                        self.assertEqual(struct.unpack_from('<h',expected_final[0],0x24+bank*2)[0],expected_handle)
                    self.initialize(case,state,bank,False);self.events=[]
                    result=self.run_mips(cpu,'LoadVabPairStep'if caller else'LoadVab',
                                         [PAYLOADS[0]]if caller else[address_of('VabLoadArgs'),self.mode,bank])
                    self.assertEqual(result is None,terminated,(linked,case,state,caller))
                    if caller and not terminated:self.assertEqual(result,expected_result,(linked,case,state))
                    self.assertEqual(self.events,expected_events,(linked,case,state,caller))
                    self.assertEqual(self.snapshot(False),expected_final,(linked,case,state,caller))
        self.assertEqual(coverage,{n for n,_,_ in SERVICES})

    def test_cursor_accessors_and_alignment_preserve_native_address_width(self):
        self.case=0;self.events=[]
        for linked,cpu in self.machines():
            for i in range(0,128):
                pointer=self.native_address(PAYLOADS[0]+i)
                self.load_state.value=42;self.library.SetRaceLoadCursor(pointer);self.assertEqual(self.library.GetRaceLoadCursor(),pointer);self.assertEqual(self.load_state.value,0)
                self.put(address_of('RaceLoadState'),42);self.run_mips(cpu,'SetRaceLoadCursor',[PAYLOADS[0]+i]);self.assertEqual(self.word(address_of('RaceLoadState')),0);self.assertEqual(self.run_mips(cpu,'GetRaceLoadCursor',[])&0xFFFFFFFF,PAYLOADS[0]+i)
                aligned=(pointer+3)&~3;self.assertEqual(self.library.AlignUp4(pointer),aligned)
                self.assertEqual(self.run_mips(cpu,'AlignUp4',[PAYLOADS[0]+i])&0xFFFFFFFF,(PAYLOADS[0]+i+3)&~3)
            # PAL word projection is test transport only; native addresses above
            # are compared at full width, without a low-word restoration path.
            for value in (0,1,2,3,4,0x7FFFFFFE,0x7FFFFFFF,0x80000000,0xFFFFFFFD,0xFFFFFFFE,0xFFFFFFFF):
                native=self.library.AlignUp4(value)
                self.assertEqual(native,(value+3)&~3)
                self.assertEqual(self.run_mips(cpu,'AlignUp4',[value])&0xFFFFFFFF,native&0xFFFFFFFF)
        for value in range((1<<64)-8,1<<64):
            self.assertEqual(self.library.AlignUp4(value),((value+3)&~3)&((1<<64)-1))
