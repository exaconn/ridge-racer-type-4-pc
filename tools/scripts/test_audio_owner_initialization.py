"""Whole audio constructor: native pointer owner, original and linked MIPS.

The clear service must receive the complete owner for its architecture. Every
native pointer cell, native alignment gap, exterior guard and context byte is
checked. The actual SDK table constructor runs between observed service calls.
"""
import ctypes as C
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from c_functions import address_of, definition_source, source_owners
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from test_vehicle_initializer import Cpu

ROOT=Path(__file__).resolve().parents[2]
SIZE,CONTEXT=0xA8D0,0x57C
STACK,STOP=0x803F0000,0x803FF000
SERVICES=(('CdReset',1),('SsSetSerialAttr',3),('SsSetSerialVol',3),
          ('SsSetTickMode',1),('SsStart',0),('SsSetMVol',2),('PsyqBzero',2),
          ('InitSceneOverlayState',0),('ResetSlotOrder',0),('CdAudioInit',0),
          ('CdAudioFindStreamFile',0))
PREFIX=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "rr/audio_state_types.h"
#include "rr/scene_camera_state.h"
#include "rr/sequencer_init.h"
#include "game/data/trkld8.h"
#include "psyq/libspu/s_i.h"
#include "rr/spu_tick.h"
#include "rr/bios_devices.h"
#include "game/gfx/motion1.h"
#include "game/sound/cdpos.h"
static struct {u8 before[16];AudioState value;u8 after[16];} owner;
#define SoundState owner.value
static struct {u8 before[16];SceneRaceContext value;u8 after[16];} context;
#define RaceContext context.value
SequenceChannel *_ss_score[32];
u32 _snd_openflag;
s16 _snd_seq_s_max,_snd_seq_t_max;
static u8 pointer_targets[14],stream_table[32];
static unsigned scenario,count,clears;
static u32 trace[12][4];
static void event(unsigned kind,u32 a,u32 b,u32 c){
 assert(count<12);trace[count][0]=kind;trace[count][1]=a;
 trace[count][2]=b;trace[count++][3]=c;
}
s32 CdReset(s32 value){event(0,value,0,0);return -123;}
void SsSetSerialAttr(s8 a,s8 b,s8 c){event(1,a,b,c);}
s32 SsSetSerialVol(s8 a,s32 b,s32 c){event(2,a,b,c);return -456;}
void SsSetTickMode(s32 mode){event(3,mode,0,0);}
void SsStart(void){
 assert(_ss_score[0]==&SoundState.sequences[0]);
 assert(_ss_score[1]==&SoundState.sequences[1]);
 assert((uintptr_t)_ss_score[0]>UINT32_MAX);
 event(4,0,0,0);
}
void SsSetMVol(s16 left,s16 right){event(5,left,right,0);}
void PsyqBzero(void *buffer,s32 size){
 if(!clears++){
  assert(buffer==&SoundState&&size==sizeof(SoundState));
  event(6,0,0xA8D0,0);
 }else{
  assert(clears==2&&buffer==&RaceContext&&size==sizeof(context.value));
  event(6,1,0x57C,0);
 }
 memset(buffer,0,(size_t)size);
}
void InitSceneOverlayState(void){event(7,0,0,0);}
void ResetSlotOrder(void){event(8,0,0,0);}
void CdAudioInit(void){event(9,0,0,0);}
void CdAudioFindStreamFile(void){
 event(10,0,0,0);
 SoundState.streamBanks[0].sector=scenario*7919U;
 SoundState.streamBanks[0].table=stream_table;
 SoundState.streamBanks[1].table=AUDIO_STREAM_TABLE_UNAVAILABLE;
}
'''
TAIL=r'''
void run_case(unsigned number,u32 flags,u32 *out){
 unsigned i;AudioState expected;
 _Static_assert(sizeof(void*)==8,"full native pointers");
 _Static_assert(sizeof(AudioState)==0xA920,"complete native owner");
 scenario=number;count=clears=0;memset(trace,0,sizeof(trace));
 memset(&owner,0xA7,sizeof(owner));memset(&context,0xA7,sizeof(context));
 memset(&SoundState,(u8)(number*29U),sizeof(SoundState));
 memset(&RaceContext,(u8)(number*31U),sizeof(context.value));
 memset(_ss_score,0,sizeof(_ss_score));
 for(i=0;i<2;i++){
  SequenceChannel *r=&SoundState.sequences[i];
  SoundState.streamBanks[i].table=&pointer_targets[i];
  r->c0=&pointer_targets[2+i*5];r->c4=&pointer_targets[3+i*5];
  r->c8=&pointer_targets[4+i*5];r->cC=&pointer_targets[5+i*5];
  r->p10=&pointer_targets[6+i*5];SoundState.seqAddr2858[i]=&pointer_targets[12+i];
 }
 _snd_openflag=flags;_snd_seq_s_max=_snd_seq_t_max=-1;
 SoundSystemInit();
 assert(count==12&&clears==2&&_snd_openflag==(flags|0xFFFFFFFCU));
 assert(_snd_seq_s_max==2&&_snd_seq_t_max==1);
 assert(_ss_score[0]==&SoundState.sequences[0]&&_ss_score[1]==&SoundState.sequences[1]);
 for(i=2;i<32;i++)assert(!_ss_score[i]);
 memset(&expected,0,sizeof(expected));expected.master08=128;
 for(i=0;i<4;i++)expected.slots38[i].bank04.word=-1;
 for(i=0;i<8;i++)expected.banks24.ids[i]=-1;
 expected.seqInit2860=expected.seqInit2862=-1;
 expected.streamBanks[0].sector=number*7919U;
 expected.streamBanks[0].table=stream_table;
 expected.streamBanks[1].table=AUDIO_STREAM_TABLE_UNAVAILABLE;
 assert(!memcmp(&SoundState,&expected,sizeof(expected)));
 for(i=0;i<sizeof(context.value);i++)assert(!((const u8*)&context.value)[i]);
 for(i=0;i<16;i++)assert(owner.before[i]==0xA7&&owner.after[i]==0xA7&&context.before[i]==0xA7&&context.after[i]==0xA7);
 memcpy(out,trace,sizeof(trace));
}
'''


class AudioOwnerInitializationTest(unittest.TestCase):
    def test_complete_owner_clear_and_sdk_table_on_native_and_both_mips_images(self):
        owners=source_owners(ROOT/'src/main/PAL')
        code=PREFIX+definition_source(owners['SsSetTableSize'].read_text(),'SsSetTableSize')
        code+=definition_source(owners['SoundSystemInit'].read_text(),'SoundSystemInit')+TAIL
        with TemporaryDirectory()as temporary:
            directory=Path(temporary);source=directory/'audio.c';source.write_text(code)
            result=subprocess.run(['cc','-std=gnu11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                '-Wno-error=deprecated-non-prototype','-fsanitize=undefined,bounds','-fno-sanitize-recover=all',
                '-I',str(ROOT/'include'),str(source),'-o',str(directory/'audio.so')],capture_output=True,text=True)
            self.assertEqual(result.returncode,0,result.stderr)
            library=C.CDLL(str(directory/'audio.so'));library.run_case.argtypes=[C.c_uint,C.c_uint,C.c_void_p]
            image=(ROOT/'assets/PAL/main.exe').read_bytes();base=struct.unpack_from('<I',image,24)[0]
            state,context,table,flags,banks,rows=map(address_of,('SoundState','RaceContext','_ss_score','_snd_openflag','_snd_seq_s_max','_snd_seq_t_max'))
            services={address_of(name):(index,argc)for index,(name,argc)in enumerate(SERVICES)}
            for version in ('original','linked'):
                memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000)
                read=lambda a,n:bytes(memory.mem_read(a&0x1FFFFFFF,n))
                write=lambda a,b:memory.mem_write(a&0x1FFFFFFF,bytes(b))
                write(base,image[0x800:])
                if version=='linked':
                    with(ROOT/'build/PAL/main.elf').open('rb')as stream:
                        elf=ELFFile(stream)
                        for section in elf.iter_sections():
                            if section['sh_flags']&2 and section['sh_type']!='SHT_NOBITS':write(section['sh_addr'],section.data())
                def store(cpu,access,address,size,value,user):
                    self.assertTrue(any(a<=address and address+size<=a+n for a,n in
                        ((state,SIZE),(context,CONTEXT),(table,128),(flags,4),(banks,2),(rows,2),(STACK-512,512))),hex(address))
                cpu=Cpu(memory,0,100,101,102,store);rng=random.Random(0xA8D0)
                for case in range(96):
                    initial_flags=rng.getrandbits(32);trace=(C.c_uint32*48)();library.run_case(case,initial_flags,trace)
                    write(state-16,b'\xA7'*16+bytes([case*29&255])*SIZE+b'\xA7'*16)
                    write(context-16,b'\xA7'*16+bytes([case*31&255])*CONTEXT+b'\xA7'*16)
                    write(table,bytes(128));write(flags,struct.pack('<I',initial_flags))
                    write(banks,b'\xFF\xFF');write(rows,b'\xFF\xFF')
                    cpu.reset();cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)]
                    cpu.r[29],cpu.r[31],cpu.pc=STACK,STOP,address_of('SoundSystemInit');events=[];clears=0
                    for _ in range(20000):
                        if cpu.pc==STOP:break
                        if cpu.pc in services:
                            self.assertIsNone(cpu.delay);kind,argc=services[cpu.pc];args=cpu.r[4:4+argc]
                            if kind==6:
                                target,length=args;self.assertEqual((target,length),(state,SIZE)if not clears else(context,CONTEXT))
                                args=[clears,length];clears+=1;write(target,bytes(length))
                            events.append([kind,*args,*([0]*(3-len(args)))])
                            if kind==10:
                                write(state+0x2634,struct.pack('<I',case*7919));write(state+0x2638,struct.pack('<I',address_of('CdAudioTrackTable')))
                                write(state+0x2640,b'\xFF'*4)
                            cpu.pc=cpu.r[31]
                            for reg in (2,3,*range(4,16),24,25):cpu.r[reg]=0xBAD00000+reg
                            cpu.r[2]=0
                        else:cpu.step()
                    self.assertEqual(cpu.pc,STOP,(version,case));self.assertEqual(clears,2)
                    self.assertEqual(events,[list(trace[i*4:i*4+4])for i in range(12)])
                    expected=bytearray(SIZE);struct.pack_into('<i',expected,8,128)
                    for i in range(4):struct.pack_into('<i',expected,0x3C+i*0x620,-1)
                    for i in range(8):struct.pack_into('<h',expected,0x24+i*2,-1)
                    struct.pack_into('<hh',expected,0x2860,-1,-1)
                    struct.pack_into('<II',expected,0x2634,case*7919,address_of('CdAudioTrackTable'))
                    struct.pack_into('<I',expected,0x2640,0xFFFFFFFF)
                    self.assertEqual(read(state-16,SIZE+32),b'\xA7'*16+expected+b'\xA7'*16)
                    self.assertEqual(read(context-16,CONTEXT+32),b'\xA7'*16+bytes(CONTEXT)+b'\xA7'*16)
                    self.assertEqual(read(table,128),struct.pack('<II',state+0x26F8,state+0x26F8+176)+bytes(120))
                    self.assertEqual(read(flags,4),struct.pack('<I',initial_flags|0xFFFFFFFC))
                    self.assertEqual(read(banks,2),b'\x02\0');self.assertEqual(read(rows,2),b'\x01\0')
                    for reg in (*range(16,24),28,29,30):self.assertEqual(cpu.r[reg],STACK if reg==29 else 0xBEEF0000+reg)


if __name__=='__main__':unittest.main()
