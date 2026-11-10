"""Current voice-attribute writer: complete MMIO owners and original MIPS.

Service-entry snapshots check write order, captured masks and live register
roots. The independent model uses numbered hardware halfwords, while production
uses named channel fields. Both full register banks and all guards are compared.
"""
import ctypes as C
import os
from pathlib import Path
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32

from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
NAME = 'SpuSetVoiceAttr'
STACK, STOP = 0x803F0000, 0x803FF000
BANKS = (0x80180000, 0x80190000)
ATTRIBUTES = 0x801A0000
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "rr/spu.h"
#include "psyq/libsnd/vmvinit.h"
#include "psyq/libsnd/vmpitch.h"
typedef struct {u32 before[4];SpuRegisterStorage bank;u32 after[4];} BankOwner;
typedef struct {u32 before[4];SpuVoiceAttr value;u32 after[4];} AttributeOwner;
typedef struct {u32 before[4];u16 values[24];u32 after[4];} NoteOwner;
typedef struct {BankOwner banks[2];AttributeOwner attributes;NoteOwner notes;u32 root,seed;} State;
typedef struct {State entry;u32 operation,args[4],result;} Event;
typedef struct {State final;unsigned count;Event events[72];} Capture;
enum {WIRE_SIZE=2*sizeof(BankOwner)+sizeof(AttributeOwner)+sizeof(NoteOwner)+4};
static State live,initial;
static Capture actual,expected,*capture;
static unsigned scenario,serial,recordWire;
static unsigned voiceSeen[24],flagSeen[19],modeSeen[10],rootSeen[2],redirectSeen,mutationSeen;
static u8 firstWire[WIRE_SIZE],lastWire[WIRE_SIZE],beforeWire[72][WIRE_SIZE],afterWire[72][WIRE_SIZE];
SpuRegisterBank *_spu_RXX;
#define _spu_voice_centerNote live.notes.values
static const u32 masks[]={0,1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,
                         16384,32768,65536,131072,262144,0x7FFFF,0xFFFFFFFF};
static const s32 modes[]={INT32_MIN,-1,0,1,2,3,4,5,6,7,8,INT32_MAX};
static const s16 volumes[]={INT16_MIN,-129,-128,-1,0,1,126,127,128,129,16383,32767};
static const u16 rates[]={0,1,14,15,16,30,31,32,126,127,128,65535};
static u32 next(u32 *seed){u32 n=*seed;n^=n<<13;n^=n>>17;n^=n<<5;return *seed=n;}
static s32 signed_word(u32 n){s32 r;memcpy(&r,&n,4);return r;}
static void bind(void){assert(live.root<2);_spu_RXX=&live.banks[live.root].bank.registers;}
static void wire(u8 *out){
 u32 root=live.root?0x80190010U:0x80180010U;
 memcpy(out,live.banks,sizeof(live.banks));out+=sizeof(live.banks);
 memcpy(out,&live.attributes,sizeof(live.attributes));out+=sizeof(live.attributes);
 memcpy(out,&live.notes,sizeof(live.notes));out+=sizeof(live.notes);memcpy(out,&root,4);
}
static void mutate(unsigned operation){
 if(!(scenario&1))return;
 SpuVoiceAttr *a=&live.attributes.value;u32 n=serial++,r=next(&live.seed);
 live.root^=1;redirectSeen++;bind();
 a->voice=next(&live.seed);a->mask=next(&live.seed);
 a->pitch=(u16)r;a->note=(u16)(r>>16);a->sample_note=(u16)~r;
 a->volume.left=volumes[(scenario+n)%12];a->volume.right=volumes[(scenario+n+5)%12];
 a->volmode.left=(s16)modes[(scenario+n)%12];a->volmode.right=(s16)modes[(scenario+n+3)%12];
 a->addr=next(&live.seed);a->loop_addr=next(&live.seed);
 a->a_mode=modes[(scenario+n)%12];a->s_mode=modes[(scenario+n+1)%12];a->r_mode=modes[(scenario+n+2)%12];
 a->ar=rates[(scenario+n)%12];a->dr=rates[(scenario+n+2)%12];a->sr=rates[(scenario+n+4)%12];
 a->rr=rates[(scenario+n+6)%12];a->sl=rates[(scenario+n+8)%12];a->adsr1=(u16)r;a->adsr2=(u16)~r;
 for(unsigned i=0;i<24;i++)live.notes.values[i]=(u16)next(&live.seed);
 /* Hardware may also change envelope/readback and non-voice registers. */
 live.banks[live.root].bank.halves[(r>>8)&255]=(u16)(r^operation);
 mutationSeen++;
}
static u32 service(unsigned operation,u32 a,u32 b,u32 c,u32 d){
 unsigned i=capture->count++;assert(i<72);Event *e=&capture->events[i];
 memcpy(&e->entry,&live,sizeof(live));e->operation=operation;
 e->args[0]=a;e->args[1]=b;e->args[2]=c;e->args[3]=d;
 e->result=(a*0x1020305U)^(b*0x17171717U)^(c<<16)^d^0xA55A8000U;
 if(recordWire)wire(beforeWire[i]);
 if(operation==2){assert(a<192&&((a&7)==3||(a&7)==7));live.banks[live.root].bank.halves[a]=(u16)(b>>3);}
 mutate(operation);if(recordWire)wire(afterWire[i]);return e->result;
}
u32 _spu_note2pitch(s32 a,s32 b,s32 c,s32 d){
 assert((u32)a<256&&(u32)b<256&&(u32)c<256&&(u32)d<256);
 return service(1,(u32)a,(u32)b,(u32)c,(u32)d);
}
s32 _spu_FsetRXXa(s32 index,s32 value){return signed_word(service(2,(u32)index,(u32)value,0,0));}
'''
REFERENCE = r'''
static u16 model_volume(s16 value,s16 mode,int useMode){
 unsigned result=(u16)value&32767,control=0;
 if(useMode&&mode>=1&&mode<=7){control=0x7000U+(unsigned)mode*4096U;
  result=value<0?0:value>127?127:(unsigned)value;}
 if(mode>=0&&mode<=8)modeSeen[mode]++;else modeSeen[9]++;
 return (u16)(control|result);
}
static u16 read_register(unsigned voice,unsigned index){return live.banks[live.root].bank.halves[voice*8+index];}
static void write_register(unsigned voice,unsigned index,u16 value){live.banks[live.root].bank.halves[voice*8+index]=value;rootSeen[live.root]++;}
static unsigned cap(unsigned value,unsigned limit){return value>limit?limit:value;}
static void reference(void){
 SpuVoiceAttr *a=&live.attributes.value;const u32 fields=a->mask;const int all=fields==0;
 for(unsigned v=0;v<24;v++){
  if(!((a->voice>>v)&1))continue;
  voiceSeen[v]++;for(unsigned bit=0;bit<19;bit++)flagSeen[bit]+=all||((fields>>bit)&1);
  if(all||(fields&16))write_register(v,2,a->pitch);
  if(all||(fields&64))live.notes.values[v]=a->sample_note;
  if(all||(fields&32)){
   u16 center=live.notes.values[v],note=a->note;
   u32 pitch=_spu_note2pitch(center/256,center%256,note/256,note%256);
   write_register(v,2,(u16)pitch);
  }
  if(all||(fields&1))write_register(v,0,model_volume(a->volume.left,a->volmode.left,all||(fields&4)));
  if(all||(fields&2))write_register(v,1,model_volume(a->volume.right,a->volmode.right,all||(fields&8)));
  if(all||(fields&128))_spu_FsetRXXa((s32)(v*8+3),signed_word(a->addr));
  if(all||(fields&65536))_spu_FsetRXXa((s32)(v*8+7),signed_word(a->loop_addr));
  if(all||(fields&131072))write_register(v,4,a->adsr1);
  if(all||(fields&262144))write_register(v,5,a->adsr2);
  if(all||(fields&2048)){
   unsigned exponential=(all||(fields&256))&&a->a_mode==5;
   write_register(v,4,(u16)((read_register(v,4)&255)|(cap(a->ar,127)<<8)|(exponential<<15)));
  }
  if(all||(fields&4096))write_register(v,4,(u16)((read_register(v,4)&0xFF0F)|(cap(a->dr,15)<<4)));
  if(all||(fields&8192)){
   unsigned mode=1;if(all||(fields&512)){
    if(a->s_mode==1)mode=0;else if(a->s_mode==5)mode=2;else if(a->s_mode==7)mode=3;
   }
   write_register(v,5,(u16)((read_register(v,5)&63)|(cap(a->sr,127)<<6)|(mode<<14)));
  }
  if(all||(fields&16384)){
   unsigned exponential=(all||(fields&1024))&&a->r_mode==7;
   write_register(v,5,(u16)((read_register(v,5)&0xFFC0)|cap(a->rr,31)|(exponential<<5)));
  }
  if(all||(fields&32768))write_register(v,4,(u16)((read_register(v,4)&0xFFF0)|cap(a->sl,15)));
 }
}
static void initialize(unsigned number){
 scenario=number;serial=0;u32 seed=number*0x9E3779B9U+0x84674U;u8 *bytes=(u8*)&live;
 for(unsigned i=0;i<sizeof(live);i++)bytes[i]=(u8)next(&seed);
 live.seed=seed;live.root=(number>>1)&1;SpuVoiceAttr *a=&live.attributes.value;
 unsigned index=number%24;a->mask=index<22?masks[index]:next(&live.seed);
 switch((number/24)%7){
 case 0:a->voice=1U<<((number/168)%24);break;
 case 1:a->voice=0;break;case 2:a->voice=0xFF000000U;break;
 case 3:a->voice=0xFFFFFFU;break;case 4:a->voice=0x55555555U;break;
 case 5:a->voice=0xAAAAAAAAU;break;default:a->voice=next(&live.seed);break;
 }
 a->pitch=(u16)(number*37U);a->sample_note=(u16)(number*73U);a->note=(u16)~number;
 a->volume.left=volumes[(number/24)%12];a->volume.right=volumes[(number/288)%12];
 a->volmode.left=(s16)modes[(number/7)%12];a->volmode.right=(s16)modes[(number/11)%12];
 a->a_mode=modes[(number/13)%12];a->s_mode=modes[(number/17)%12];a->r_mode=modes[(number/19)%12];
 a->ar=rates[(number/3)%12];a->dr=rates[(number/5)%12];a->sr=rates[(number/7)%12];
 a->rr=rates[(number/11)%12];a->sl=rates[(number/13)%12];bind();memcpy(&initial,&live,sizeof(live));
}
unsigned exercise_native(void){
 _Static_assert(sizeof(SpuVoiceAttr)==64,"public attribute record");
 _Static_assert(sizeof(SpuVoiceRegisters)==16&&sizeof(SpuRegisterBank)==512,"complete hardware bank");
 _Static_assert(sizeof(BankOwner)==544&&sizeof(NoteOwner)==80&&sizeof(AttributeOwner)==96,"complete owners");
 _Static_assert(offsetof(SpuVoiceRegisters,start_address)==6&&offsetof(SpuVoiceRegisters,repeat_address)==14,"address registers");
 _Static_assert(offsetof(SpuVoiceRegisters,adsr1)==8&&offsetof(SpuVoiceRegisters,adsr2)==10,"envelope registers");
 for(unsigned n=0;n<65536;n++){
  initialize(n);assert((uintptr_t)_spu_RXX>UINT32_MAX&&(uintptr_t)&live.attributes.value>UINT32_MAX);
  capture=&expected;capture->count=0;reference();memcpy(&capture->final,&live,sizeof(live));
  memcpy(&live,&initial,sizeof(live));bind();serial=0;capture=&actual;capture->count=0;
  SpuSetVoiceAttr(&live.attributes.value);memcpy(&capture->final,&live,sizeof(live));
  assert(_spu_RXX==&live.banks[live.root].bank.registers);
  assert(actual.count==expected.count);
  for(unsigned i=0;i<actual.count;i++)assert(!memcmp(&actual.events[i],&expected.events[i],sizeof(Event)));
  if(memcmp(&actual.final,&expected.final,sizeof(State))){
   const u8 *a=(const u8*)&actual.final,*e=(const u8*)&expected.final;
   for(unsigned i=0;i<sizeof(State);i++)if(a[i]!=e[i]){
    fprintf(stderr,"native case %u state offset %u: actual %02x expected %02x\n",n,i,a[i],e[i]);break;
   }
   assert(!"native voice state differs from independent model");
  }
 }
 for(unsigned i=0;i<24;i++)assert(voiceSeen[i]);for(unsigned i=0;i<19;i++)assert(flagSeen[i]);
 for(unsigned i=0;i<10;i++)assert(modeSeen[i]);assert(rootSeen[0]&&rootSeen[1]&&redirectSeen&&mutationSeen);
 return 65536;
}
unsigned prepare_trace(unsigned number){initialize(number);wire(firstWire);capture=&expected;capture->count=0;recordWire=1;reference();wire(lastWire);recordWire=0;return capture->count;}
const void *initial_wire(void){return firstWire;}const void *final_wire(void){return lastWire;}
const void *event_wire(unsigned index,unsigned after){assert(index<capture->count);return after?afterWire[index]:beforeWire[index];}
u32 event_value(unsigned index,unsigned field){assert(index<capture->count&&field<6);Event *e=&capture->events[index];return field==0?e->operation:field==5?e->result:e->args[field-1];}
unsigned wire_size(void){return WIRE_SIZE;}
'''


class SpuVoiceAttributeTest(unittest.TestCase):
    def test_full_native_owners_live_roots_and_original_mips(self):
        owner = source_owners(ROOT/'src/main/PAL')[NAME]
        body = definition_source(owner.read_text(), NAME)
        # Remove target-only register annotations, retaining all actual accesses.
        body = re.sub(r'\s+__asm__\("\$[0-9]+"\)', '', body)
        with TemporaryDirectory() as temporary:
            source, library = Path(temporary)/'native.c', Path(temporary)/'native.dylib'
            source.write_text(PREFIX+body+REFERENCE)
            subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                            '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
                            '-I', str(ROOT/'include'), str(source), '-o', str(library)], check=True)
            native = C.CDLL(str(library))
            native.exercise_native.restype = C.c_uint
            self.assertEqual(native.exercise_native(), 65536)
            native.prepare_trace.argtypes = [C.c_uint]
            native.prepare_trace.restype = C.c_uint
            native.initial_wire.restype = native.final_wire.restype = C.c_void_p
            native.event_wire.argtypes = [C.c_uint, C.c_uint]
            native.event_wire.restype = C.c_void_p
            native.event_value.argtypes = [C.c_uint, C.c_uint]
            native.event_value.restype = C.c_uint32
            native.wire_size.restype = C.c_uint
            regions = [(BANKS[0], 544), (BANKS[1], 544), (ATTRIBUTES, 96),
                       (address_of('_spu_voice_centerNote')-16, 80), (address_of('_spu_RXX'), 4)]
            size = native.wire_size()
            self.assertEqual(size, sum(length for _, length in regions))
            intervals = sorted((a, a+n) for a, n in regions)
            self.assertTrue(all(a[1] <= b[0] for a, b in zip(intervals, intervals[1:])))
            memory = Uc(UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN | UC_MODE_MIPS32)
            memory.mem_map(0, 0x400000)
            write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b))
            read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
            original = (ROOT/'assets/PAL/main.exe').read_bytes()
            base = struct.unpack_from('<I', original, 0x18)[0]
            write(base, original[0x800:])
            if os.getenv('RR_COMPILED_ELF'):
                with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                    elf = ELFFile(stream)
                    symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(NAME)[0]
                    section = elf.get_section(symbol['st_shndx'])
                    offset = symbol['st_value']-section['sh_addr']
                    self.assertEqual(symbol['st_value'], address_of(NAME))
                    write(symbol['st_value'], section.data()[offset:offset+symbol['st_size']])
            services = {address_of('_spu_note2pitch'): 1, address_of('_spu_FsetRXXa'): 2}
            direct_stores = {((bank+16+voice*16+offset) & 0x1FFFFFFF)
                             for bank in BANKS for voice in range(24) for offset in (0, 2, 4, 8, 10)}
            center = address_of('_spu_voice_centerNote') & 0x1FFFFFFF
            direct_stores.update(center+2*i for i in range(24))

            def store(cpu, access, address, length, value, user):
                address &= 0x1FFFFFFF
                if ((STACK-64) & 0x1FFFFFFF) <= address < (STACK & 0x1FFFFFFF):
                    self.assertEqual(length, 4)
                else:
                    self.assertEqual(length, 2)
                    self.assertIn(address, direct_stores)

            cpu = Cpu(memory, 0, 100, 101, 102, store)

            def write_state(data):
                offset = 0
                for address, length in regions:
                    write(address, data[offset:offset+length])
                    offset += length

            def state():
                return b''.join(read(address, length) for address, length in regions)

            for case in range(1024):
                number = case if case < 512 else (case*7919) & 65535
                events = native.prepare_trace(number)
                write_state(C.string_at(native.initial_wire(), size))
                write(STACK-128, b'\xA5'*256)
                cpu.reset()
                cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
                cpu.r[4] = ATTRIBUTES+16
                cpu.r[29], cpu.r[31], cpu.pc = STACK, STOP, address_of(NAME)
                event = 0
                for instruction in range(16000):
                    if cpu.pc == STOP:
                        break
                    if cpu.pc in services:
                        self.assertLess(event, events)
                        operation = services[cpu.pc]
                        self.assertEqual(operation, native.event_value(event, 0), (number, event))
                        for index in range(4 if operation == 1 else 2):
                            self.assertEqual(cpu.r[4+index], native.event_value(event, index+1),
                                             (number, event, index))
                        self.assertEqual(state(), C.string_at(native.event_wire(event, 0), size),
                                         (number, event, 'entry'))
                        write_state(C.string_at(native.event_wire(event, 1), size))
                        for register in (1, 2, 3, *range(4, 16), 24, 25):
                            cpu.r[register] = (0xA1230000+event*257+register) & 0xFFFFFFFF
                        cpu.r[2] = native.event_value(event, 5)
                        cpu.pc = cpu.r[31]
                        event += 1
                    else:
                        cpu.step()
                self.assertEqual(cpu.pc, STOP, number)
                self.assertEqual(event, events, number)
                self.assertEqual(state(), C.string_at(native.final_wire(), size), (number, 'final'))
                self.assertEqual(cpu.r[29], STACK)
                self.assertEqual(cpu.r[31], STOP)
                for register in (*range(16, 24), 28, 30):
                    self.assertEqual(cpu.r[register], 0xBEEF0000+register)
                self.assertEqual(read(STACK-128, 64), b'\xA5'*64)
                self.assertEqual(read(STACK, 128), b'\xA5'*128)
            print('PASS 65536 native and 1024', 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
                  'MIPS SPU voice cases; live roots, captured masks, all channels, guards and UBSan')


if __name__ == '__main__':
    unittest.main()
