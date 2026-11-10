"""Actual program payload copy/setter on complete owners and both MIPS images.

C resource inputs are byte aligned; the destination is the real nested record.
The original PAL snapshots and native pointer guards cover the complete owner.
"""
import ctypes as C
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from audio_fixture_transport import wire_types, transport, scalar_entry
from c_functions import address_of, definition_source, source_owners
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from test_sequence_channel_pipeline import SequenceCpu as Cpu

ROOT=Path(__file__).resolve().parents[2]
SIZE,PAYLOAD=0xA8D0,0x5BC
RESOURCE,STACK,STOP=0x80280010,0x803F0000,0x803FF000
PREFIX=r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "rr/audio_bank_setup.h"
#include "rr/audio_loop.h"
#include "rr/memory.h"
#include "game/sound/sndslot.h"
#include "game/sound/sndchan.h"
#include "rr/audio_pan.h"
static struct {u8 before[16];AudioWireState value;u8 after[16];} fixture;
static union {u32 alignment;u8 bytes[0x5BC+64];} resource;
#define SoundState fixture.value
#define SoundMasterGain fixture.value.gain18
#define VoiceVolumeScale fixture.value.master08
RuntimeSettings Options;
static struct {u32 guard[4];s16 pan[128];u16 offsets[288];u32 tail[4];} tables;
#define PanGainTable tables.pan
#define MenuCarTextureFileOffsets tables.offsets
'''
MOCKS=r'''
typedef struct {u32 kind,args[8];AudioWireState audio;} ProgramEvent;
static ProgramEvent events[32];
static unsigned event_count,play_scenario,play_slot,play_mutate;
static void observe(u32 kind,const u32 *arguments){
 unsigned row,i;audio_before_callback();assert(event_count<32);
 ProgramEvent *event=&events[event_count++];event->kind=kind;
 memcpy(event->args,arguments,sizeof(event->args));event->audio=fixture.value;
 if(kind==1&&event_count==2&&play_mutate){
  AudioWireSlot620 *slot=&fixture.value.slots38[play_slot];
  slot->bank04=(s16)(u16)(0x8123U+play_scenario*17U);
  slot->voice08=123+play_scenario;slot->count0C=1+play_scenario%5;slot->program14^=1;
  for(row=0;row<2;row++)for(i=0;i<5;i++)
   slot->programs5DC[row][i]^=(u16)((play_scenario+1U)*97U+i*131U+row*17U);
 }
 audio_after_callback();
}
void *PsyqMemset(void *destination,s32 value,u32 length){
 assert(event_count==0&&value==0&&length==4*sizeof(AudioUnsignedWord4));
 assert((uintptr_t)destination>UINT32_MAX);
 u32 args[8]={0,(u32)value,length};observe(0,args);
 memset(destination,value,length);return destination;
}
s32 SsUtKeyOnV(s32 voice,s32 bank,s32 program,s32 mode,s32 group,s32 unused,s32 left,s32 right){
 u32 args[8]={voice,bank,program,mode,group,unused,left,right};observe(1,args);
 return(s32)(0xA5A50000U|event_count);
}

s32 SsUtSetVVol(s32 voice,s32 left,s32 right){
 u32 args[8]={voice,left,right};observe(2,args);return(s32)(0xB5B50000U|event_count);
}
s32 SsUtChangePitch(s32 voice,s32 bank,s32 program,s32 group,s32 unused,u32 first,u32 second){
 u32 args[8]={voice,bank,program,group,unused,first,second};observe(3,args);return(s32)(0xC5C50000U|event_count);
}
'''
TAIL=r'''
static void prepare(const u8 *initial){
 memset(&fixture,0xA7,sizeof(fixture));memcpy(&fixture.value,initial,sizeof(fixture.value));
 assert((uintptr_t)&fixture>UINT32_MAX&&(uintptr_t)&resource>UINT32_MAX);
}
static void finish(u8 *output){
 unsigned i;for(i=0;i<16;i++)assert(fixture.before[i]==0xA7&&fixture.after[i]==0xA7);
 memcpy(output,&fixture.value,sizeof(fixture.value));
}
void copy_case(const u8 *initial,const u8 *bytes,unsigned slot,unsigned alignment,u8 *output){
 u8 before[sizeof(resource.bytes)];prepare(initial);
 memset(resource.bytes,0xA7,sizeof(resource.bytes));
 memcpy(resource.bytes+16+alignment,bytes,sizeof(AudioProgramData));
 assert(((uintptr_t)(resource.bytes+16+alignment)&3)==alignment);
 memcpy(before,resource.bytes,sizeof(before));
 SlotLoadCurves(resource.bytes+16+alignment,(s32)slot);
 assert(!memcmp(before,resource.bytes,sizeof(before)));finish(output);
}
s32 set_case(const u8 *initial,s32 program,s32 group,s32 value,s32 slot,u8 *output){
 prepare(initial);s32 result=SlotSetProgram(program,group,value,slot);finish(output);return result;
}
unsigned play_case(const u8 *initial,const u8 *payload,unsigned scenario,unsigned slot,
                  unsigned mutation,u8 *output,ProgramEvent *out_events){
 prepare(initial);memcpy(resource.bytes+16,payload,sizeof(AudioProgramData));
 memset(events,0,sizeof(events));event_count=0;
 play_scenario=scenario;play_slot=slot;play_mutate=mutation;
 SlotLoadCurves(resource.bytes+16,(s32)slot);
 SlotSetProgram(0,fixture.value.slots38[slot].program14,(s32)(scenario*8191U),(s32)slot);
 SlotKeyOn((s32)slot);finish(output);memcpy(out_events,events,sizeof(events));return event_count;
}

void set_profile(unsigned volume,s16 mono){
 unsigned i;memset(&Options,0xA7,sizeof(Options));memset(&tables,0xA7,sizeof(tables));
 Options.audio.volume31=volume;Options.audio.mono32=mono;
 for(i=0;i<128;i++)PanGainTable[i]=(i*127U)%16130U;
 for(i=0;i<288;i++)MenuCarTextureFileOffsets[i]=i*97U;
}
unsigned update_case(const u8 *initial,const u8 *payload,unsigned scenario,unsigned slot,
                     unsigned mutation,u8 *output,ProgramEvent *out_events){
 u8 old_options[sizeof(Options)],old_tables[sizeof(tables)];
 prepare(initial);memcpy(resource.bytes+16,payload,sizeof(AudioProgramData));
 memset(events,0,sizeof(events));event_count=0;
 play_scenario=scenario;play_slot=slot;play_mutate=mutation;
 SlotLoadCurves(resource.bytes+16,(s32)slot);
 SlotSetProgram(0,fixture.value.slots38[slot].program14,(s32)(scenario*8191U),(s32)slot);
 memcpy(old_options,&Options,sizeof(Options));memcpy(old_tables,&tables,sizeof(tables));
 UpdateSoundSlot((s32)slot);finish(output);memcpy(out_events,events,sizeof(events));
 assert(!memcmp(old_options,&Options,sizeof(Options))&&!memcmp(old_tables,&tables,sizeof(tables)));
 return event_count;
}
'''


class ProgramEvent(C.Structure):
    _fields_=[('kind',C.c_uint32),('args',C.c_uint32*8),('audio',C.c_ubyte*SIZE)]


class AudioProgramOwnerTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temporary=TemporaryDirectory();cls.addClassCleanup(cls.temporary.cleanup)
        directory=Path(cls.temporary.name);owners=source_owners(ROOT/'src/main/PAL')
        code=wire_types()+PREFIX+transport('fixture.value')+MOCKS
        for name,result,parameters,arguments in (
            ('SlotLoadCurves','void','const void *data,s32 slot','data,slot'),
            ('SlotSetProgram','s32','s32 program,s32 group,s32 value,s32 slot','program,group,value,slot'),
            ('SlotKeyOn','void','s32 slot','slot'),
            ('SlotInterpCurve','s32','s32 curve,s32 value,s32 group,s32 slot','curve,value,group,slot'),
            ('SetVoiceVolumeScaled','void','s32 voice,s32 left,s32 right','voice,left,right'),
            ('UpdateSoundSlot','void','s32 slot','slot')):
            body=definition_source(owners[name].read_text(),name)
            entry=scalar_entry(body,name,'fixture.value',result,parameters,arguments,
                               aliases=({'SoundSlots':'slots38','SoundMasterGain':'gain18'} if name in ('SlotKeyOn','UpdateSoundSlot')
                                        else {'VoiceVolumeScale':'master08'} if name=='SetVoiceVolumeScaled' else None))
            code+=entry[len(transport('fixture.value')):]
        source=directory/'audio.c';source.write_text(code+TAIL)
        result=subprocess.run(['cc','-std=gnu11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
            '-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(source),
            '-o',str(directory/'audio.so')],capture_output=True,text=True)
        if result.returncode:raise AssertionError(result.stderr)
        cls.library=C.CDLL(str(directory/'audio.so'))
        cls.library.copy_case.argtypes=[C.c_void_p,C.c_void_p,C.c_uint,C.c_uint,C.c_void_p]
        cls.library.set_case.argtypes=[C.c_void_p,*([C.c_int32]*4),C.c_void_p];cls.library.set_case.restype=C.c_int32
        cls.library.play_case.argtypes=[C.c_void_p,C.c_void_p,C.c_uint,C.c_uint,C.c_uint,C.c_void_p,C.c_void_p]
        cls.library.play_case.restype=C.c_uint
        cls.library.update_case.argtypes=cls.library.play_case.argtypes;cls.library.update_case.restype=C.c_uint
        cls.library.set_profile.argtypes=[C.c_uint,C.c_int16]

    def machines(self):
        image=(ROOT/'assets/PAL/main.exe').read_bytes();base=struct.unpack_from('<I',image,24)[0]
        for version in ('original','linked'):
            memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000)
            self.write=lambda a,b:memory.mem_write(a&0x1FFFFFFF,bytes(b))
            self.read=lambda a,n:bytes(memory.mem_read(a&0x1FFFFFFF,n))
            self.write(base,image[0x800:]);state=address_of('SoundState')
            if version=='linked':
                with(ROOT/'build/PAL/main.elf').open('rb')as stream:
                    elf=ELFFile(stream)
                    for section in elf.iter_sections():
                        if section['sh_flags']&2 and section['sh_type']!='SHT_NOBITS':self.write(section['sh_addr'],section.data())
            def store(cpu,access,address,size,value,user):
                self.assertTrue(state<=address and address+size<=state+SIZE or STACK-256<=address and address+size<=STACK,(hex(address),size))
            self.cpu=Cpu(memory,0,100,101,102,store)
            yield version

    def call(self,name,arguments,initial,services=None):
        state=address_of('SoundState');self.write(state-16,b'\xA7'*16+initial+b'\xA7'*16)
        self.write(STACK-512,b'\xA7'*528)
        cpu=self.cpu;cpu.reset();cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)]
        cpu.r[4:4+len(arguments)]=[v&0xFFFFFFFF for v in arguments]
        cpu.r[29],cpu.r[31],cpu.pc=STACK,STOP,address_of(name)
        for _ in range(10000):
            if cpu.pc==STOP:break
            if services and cpu.pc in services:
                self.assertIsNone(cpu.delay);returned=services[cpu.pc](cpu)
                cpu.pc=cpu.r[31]
                for reg in (2,3,*range(4,16),24,25):cpu.r[reg]=0xBAD00000+reg
                cpu.r[2]=returned&0xFFFFFFFF
            else:cpu.step()
        self.assertEqual(cpu.pc,STOP)
        self.assertEqual(self.read(state-16,16),b'\xA7'*16);self.assertEqual(self.read(state+SIZE,16),b'\xA7'*16)
        self.assertEqual(self.read(STACK-512,256),b'\xA7'*256);self.assertEqual(self.read(STACK,16),b'\xA7'*16)
        for reg in (*range(16,24),28,29,30):self.assertEqual(cpu.r[reg],STACK if reg==29 else 0xBEEF0000+reg)
        return self.read(state,SIZE),C.c_int32(cpu.r[2]).value

    def test_unaligned_resource_copies_preserve_complete_owners(self):
        for version in self.machines():
            rng=random.Random(0x5BC);seen=set()
            for case in range(256):
                slot,alignment=case%4,(case//4)%4;seen.add((slot,alignment))
                initial,payload=rng.randbytes(SIZE),rng.randbytes(PAYLOAD)
                expected=bytearray(initial);at=0x74+slot*0x620;expected[at:at+PAYLOAD]=payload
                output=C.create_string_buffer(SIZE)
                self.library.copy_case(C.create_string_buffer(initial),C.create_string_buffer(payload),slot,alignment,output)
                self.assertEqual(output.raw,expected,(version,case,'native'))
                source=b'\xA7'*16+payload+b'\xA7'*16;self.write(RESOURCE+alignment-16,source)
                actual,_=self.call('SlotLoadCurves',(RESOURCE+alignment,slot),initial)
                self.assertEqual(actual,expected,(version,case,'MIPS'))
                self.assertEqual(self.read(RESOURCE+alignment-16,len(source)),source)
            self.assertEqual(len(seen),16)

    def test_program_selection_both_groups_all_slots_and_conversion_edges(self):
        for version in self.machines():
            rng=random.Random(0x50844)
            for slot in range(4):
                for group in range(2):
                    for program in range(5):
                        for value in (-2147483648,-1,0,1,32767,32768,65535,65536,2147483647):
                            initial=rng.randbytes(SIZE);expected=bytearray(initial)
                            at=0x5DC+0x38+slot*0x620+(group*5+program)*2
                            old=struct.unpack_from('<H',initial,at)[0]
                            if value>=0:struct.pack_into('<H',expected,at,value&65535)
                            output=C.create_string_buffer(SIZE)
                            result=self.library.set_case(C.create_string_buffer(initial),program,group,value,slot,output)
                            self.assertEqual((result,output.raw),(old,expected))
                            actual,result=self.call('SlotSetProgram',(program,group,value,slot),initial)
                            self.assertEqual((result,actual),(old,expected),(version,slot,group,program,value))

    def test_loaded_programs_play_from_real_arrays_and_keep_callback_reloads(self):
        state=address_of('SoundState')
        for version in self.machines():
            rng=random.Random(0x620);coverage=set()
            for case in range(128):
                slot,mode,mutation=case%4,(case//4)%2,(case//8)%2;base=0x38+slot*0x620
                initial=bytearray(rng.randbytes(SIZE));payload=rng.randbytes(PAYLOAD)
                struct.pack_into('<i',initial,base+8,(0,20,32765,65535)[case%4])
                struct.pack_into('<i',initial,base+12,(-1,0,1,4,5)[(case//16)%5])
                struct.pack_into('<i',initial,base+20,mode)
                prepared=bytearray(initial);prepared[base+0x3C:base+0x5F8]=payload
                struct.pack_into('<H',prepared,base+0x5DC+mode*10,case*8191&65535)

                def mutate(data):
                    struct.pack_into('<H',data,base+4,(0x8123+case*17)&65535)
                    struct.pack_into('<i',data,base+8,123+case)
                    struct.pack_into('<i',data,base+12,1+case%5)
                    current=struct.unpack_from('<i',data,base+20)[0];struct.pack_into('<i',data,base+20,current^1)
                    for group in range(2):
                        for i in range(5):
                            at=base+0x5DC+group*10+i*2
                            value=struct.unpack_from('<H',data,at)[0]^(((case+1)*97+i*131+group*17)&65535)
                            struct.pack_into('<H',data,at,value)

                expected=bytearray(prepared);wanted=[(0,[0,0,16,0,0,0,0,0],bytes(expected))]
                voice=struct.unpack_from('<i',expected,base+8)[0];i=0;group_value=62 if slot==3 else 60
                while i<struct.unpack_from('<i',expected,base+12)[0]:
                    selected=struct.unpack_from('<i',expected,base+20)[0]
                    bank=struct.unpack_from('<h',expected,base+4)[0]
                    program=struct.unpack_from('<h',expected,base+0x5DC+selected*10+i*2)[0]
                    args=[C.c_int16(voice).value,bank,program,0,group_value,0,0,0]
                    wanted.append((1,[v&0xFFFFFFFF for v in args],bytes(expected)))
                    if i==0 and mutation:mutate(expected)
                    voice+=1;i+=1
                struct.pack_into('<HH',expected,base+0x28,group_value,0)
                output=C.create_string_buffer(SIZE);native_events=(ProgramEvent*32)()
                count=self.library.play_case(C.create_string_buffer(bytes(initial)),C.create_string_buffer(payload),case,slot,mutation,output,native_events)
                observed=[(e.kind,list(e.args),bytes(e.audio))for e in native_events[:count]]
                self.assertEqual((output.raw,observed),(expected,wanted),(version,case,'native'))

                # Exercise the same producer -> setter -> consumer chain in MIPS.
                self.write(RESOURCE,payload)
                copied,_=self.call('SlotLoadCurves',(RESOURCE,slot),initial)
                selected,_=self.call('SlotSetProgram',(0,mode,case*8191,slot),copied)
                self.assertEqual(selected,prepared);events=[]
                def memset_service(cpu):
                    destination,value,length=cpu.r[4:7]
                    self.assertTrue(STACK-256<=destination and destination+length<=STACK)
                    self.assertEqual((value,length),(0,16))
                    events.append((0,[0,0,16,0,0,0,0,0],self.read(state,SIZE)))
                    self.write(destination,bytes(length));return destination
                def keyon_service(cpu):
                    args=cpu.r[4:8]+list(struct.unpack('<4I',self.read(cpu.r[29]+16,16)))
                    events.append((1,args,self.read(state,SIZE)))
                    if len(events)==2 and mutation:
                        data=bytearray(self.read(state,SIZE));mutate(data);self.write(state,data)
                    return 0xA5A50000|len(events)
                final,_=self.call('SlotKeyOn',(slot,),selected,
                    {address_of('PsyqMemset'):memset_service,address_of('SsUtKeyOnV'):keyon_service})
                self.assertEqual((final,events),(expected,wanted),(version,case,'MIPS'))
                self.assertEqual(self.read(RESOURCE,PAYLOAD),payload)
                coverage.add((slot,mode,mutation,bool(i)))
            self.assertEqual(len(coverage),32)


    def test_loaded_curves_drive_typed_pitch_rows_with_real_volume_helper(self):
        state=address_of('SoundState')
        for version in self.machines():
            rng=random.Random(0x5F8);seen=set()
            for case in range(128):
                slot,mode,mutation=case%4,(case//4)%2,(case//8)%2;base=0x38+slot*0x620
                initial=bytearray(rng.randbytes(SIZE));payload=bytearray(rng.randbytes(PAYLOAD))
                struct.pack_into('<i',initial,8,128);struct.pack_into('<h',initial,24,(0,64,128)[case%3])
                struct.pack_into('<I',initial,base,4 if case&32 else 1)
                struct.pack_into('<i',initial,base+8,20)
                count=(0,1,4,5)[(case//16)%4];struct.pack_into('<i',initial,base+12,count)
                struct.pack_into('<iii',initial,base+16,case*7,mode,mode if case&64 else mode^1)
                struct.pack_into('<hhhh',initial,base+32,(0,64,128)[case%3],128,case%128,-99)
                struct.pack_into('<h',initial,base+40,60)
                struct.pack_into('<h',initial,base+44,case*13%128)
                struct.pack_into('<i',initial,base+48,(-32768,-1,0,0x2000,0x234F,0x2351,0x3000)[case%7])
                for group in range(2):
                    for curve in range(10):
                        at=(group*10+curve)*72
                        struct.pack_into('<9i',payload,at,*[i*2048 for i in range(9)])
                        struct.pack_into('<9i',payload,at+36,*[(case+curve*7+i*13)%129 for i in range(9)])
                    for i in range(5):
                        struct.pack_into('<H',payload,0x5A0+group*10+i*2,(case*31+i*113+(group*17 if i&1 else 0))&65535)
                        struct.pack_into('<HH',initial,base+0x5F8+(group*5+i)*4,(case*101+i*7001)&65535,(case*8191+group*17+i*1009)&65535)
                struct.pack_into('<4H',payload,0x5B4,(1,256,10240,65535)[case%4],(0,64,128)[case%3],(0,32768,65535)[case%3],0xA55A)
                prepared=bytearray(initial);prepared[base+0x3C:base+0x5F8]=payload
                struct.pack_into('<H',prepared,base+0x5DC+mode*10,case*8191&65535)
                volume=(0,1,16)[case%3];mono=(case//64)%2
                self.library.set_profile(volume,mono)
                output=C.create_string_buffer(SIZE);native_events=(ProgramEvent*32)()
                n=self.library.update_case(C.create_string_buffer(bytes(initial)),C.create_string_buffer(bytes(payload)),case,slot,mutation,output,native_events)
                actual=[(e.kind,list(e.args),bytes(e.audio))for e in native_events[:n]]
                self.write(RESOURCE,payload)
                copied,_=self.call('SlotLoadCurves',(RESOURCE,slot),initial)
                selected,_=self.call('SlotSetProgram',(0,mode,case*8191,slot),copied)
                self.assertEqual(selected,prepared)
                self.write(address_of('PanGainTable'),struct.pack('<128h',*[(i*127)%16130 for i in range(128)]))
                self.write(0x800A053C,struct.pack('<288H',*[i*97 for i in range(288)]))
                options=bytearray(b'\xA7'*64);options[49]=volume;struct.pack_into('<h',options,50,mono)
                self.write(address_of('Options'),options);events=[]
                def sdk_service(kind,argc):
                    def run(cpu):
                        args=cpu.r[4:4+min(argc,4)]
                        if argc>4:args+=list(struct.unpack('<'+str(argc-4)+'I',self.read(cpu.r[29]+16,(argc-4)*4)))
                        args+=([0]*(8-len(args)));events.append((kind,args,self.read(state,SIZE)))
                        if kind==1 and len(events)==2 and mutation:
                            data=bytearray(self.read(state,SIZE))
                            struct.pack_into('<H',data,base+4,(0x8123+case*17)&65535)
                            struct.pack_into('<i',data,base+8,123+case);struct.pack_into('<i',data,base+12,1+case%5)
                            old=struct.unpack_from('<i',data,base+20)[0];struct.pack_into('<i',data,base+20,old^1)
                            for group in range(2):
                                for i in range(5):
                                    at=base+0x5DC+group*10+i*2
                                    value=struct.unpack_from('<H',data,at)[0]^(((case+1)*97+i*131+group*17)&65535)
                                    struct.pack_into('<H',data,at,value)
                            self.write(state,data)
                        return 0xA5A50000|len(events)
                    return run
                services={address_of(name):sdk_service(kind,argc)for name,kind,argc in
                    (('SsUtKeyOnV',1,8),('SsUtSetVVol',2,3),('SsUtChangePitch',3,7))}
                final,_=self.call('UpdateSoundSlot',(slot,),selected,services)
                self.assertEqual((output.raw,actual),(final,events),(version,case))
                self.assertEqual(self.read(address_of('Options'),64),options)
                self.assertEqual(self.read(RESOURCE,PAYLOAD),payload)
                seen.add((slot,mode,mutation,bool(count),bool(mono)))
            self.assertEqual(len(seen),64)


if __name__=='__main__':unittest.main()
