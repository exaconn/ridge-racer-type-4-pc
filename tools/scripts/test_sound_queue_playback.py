"""Actual queue consumer and SPU status vs original/linked MIPS and a state oracle."""
import ctypes as C
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32
from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu
ROOT = Path(__file__).resolve().parents[2]
STACK, STOP, BANK, SIZE = 0x803F0000, 0x803FF000, 0x80320010, 0xA8D0
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "rr/audio_loop.h"
#include "rr/spu_key.h"
#include "psyq/libspu/spukeyst.h"
typedef struct {
 u8 before[16]; AudioState sound; u8 between[16]; RuntimeSettings options; s16 mode;
 u8 bankBefore[16]; SpuRegisterStorage bank; u8 bankAfter[16];
 SpuRegisterBank *root; u32 keyed; u8 after[16];
} World;
static World actual;
static s32 calls[2][8];
static unsigned count, mutate;
#define SoundState actual.sound
#define Options actual.options
#define GameModeType actual.mode
#define _spu_RXX actual.root
#define _spu_keystat actual.keyed
s32 SsUtKeyOnV(s32 voice,s32 bank,s32 program,s32 mode,s32 group,s32 unused,s32 left,s32 right) {
 assert(count<2);
 s32 args[8]={voice,bank,program,mode,group,unused,left,right};
 memcpy(calls[count],args,sizeof(args));
 if(count++==0 && mutate) {
  SoundState.bank24[4]=-1234; SoundState.sequence19D0.readIndex=6;
  SoundState.sequence19D0.programs[6]=-2345;
  Options.audio.volume31=17; Options.audio.mono32=-9;
 }
 return count==1?-123:456;
}
'''
TAIL = r'''
unsigned run_case(const u8 *sound,const u8 *options,s16 mode,u32 keyed,u16 envelope,
 unsigned mutation,u8 *outSound,u8 *outOptions,s32 *outCalls) {
 memset(&actual,0xA5,sizeof(actual));
 memcpy(&SoundState,sound,sizeof(SoundState));memcpy(&Options,options,sizeof(Options));
 GameModeType=mode; actual.root=&actual.bank.registers; actual.keyed=keyed;
 actual.bank.registers.voices[4].envx=envelope;
 assert((uintptr_t)&actual>UINT32_MAX && (uintptr_t)actual.root>UINT32_MAX);
 World before=actual;
 count=0;mutate=mutation;memset(calls,0,sizeof(calls));
 SoundQueueUpdate();
 memcpy(outSound,&SoundState,sizeof(SoundState));memcpy(outOptions,&Options,sizeof(Options));
 memcpy(outCalls,calls,sizeof(calls));
 before.sound=actual.sound;before.options=actual.options;
 assert(!memcmp(&actual,&before,sizeof(actual)));
 return count;
}
'''

def divide(v, d):
    return (abs(v)//d)*(-1 if v<0 else 1)


def oracle(initial, settings, mode, key, envelope, mutation):
    sound, options = bytearray(initial), bytearray(settings)
    word = lambda at: struct.unpack_from('<i', sound, at)[0]
    half = lambda at: struct.unpack_from('<h', sound, at)[0]
    put = lambda at, v: struct.pack_into('<i', sound, at, v)
    if mode==1 and word(0x1A28)>0:
        put(0x1A28,word(0x1A28)-1)
    calls, queried = [], False
    if word(0x19D0):
        index=word(0x19D8);program=half(0x19E8+index*2)
        if program<0:
            for at in (0x19D0,0x19D8,0x19DC): put(at,0)
        else:
            queried=word(0x19E0)==0 or index>0
            busy=queried and key&16 and envelope!=0
            if not busy:
                left=C.c_int16(divide(half(0x19E4)*options[0x31],16)).value
                right=C.c_int16(divide(half(0x19E6)*options[0x31],16)).value
                if struct.unpack_from('<h',options,0x32)[0]: left=right=divide(left+right,2)
                for voice in (4,5):
                    calls.append([voice,half(0x2C),program,voice-4,60,0,left,right])
                    if voice==4 and mutation:
                        struct.pack_into('<h',sound,0x2C,-1234);put(0x19D8,6)
                        struct.pack_into('<h',sound,0x19F4,-2345)
                        options[0x31]=17;struct.pack_into('<h',options,0x32,-9)
                put(0x19D8,word(0x19D8)+1)
    return bytes(sound),bytes(options),calls,queried


def cases():
    rng=random.Random(0x535055)
    for n in range(2048):
        sound=bytearray(rng.randbytes(SIZE));options=bytearray(rng.randbytes(64))
        index=(n//4)%8;mode=(0,1,-1,32767)[(n//96)%4]
        for at,value in ((0x19D0,(0,1,-7,2147483647)[n%4]),(0x19D8,index),
                         (0x19E0,(0,1,-1)[(n//32)%3]),
                         (0x1A28,(-2147483648,-1,0,1,2147483647)[(n//384)%5])):
            struct.pack_into('<i',sound,at,value)
        values=(-32768,-1,0,1,32767)
        struct.pack_into('<h',sound,0x19E8+index*2,values[(n//7)%5])
        for at in (0x19E4,0x19E6):struct.pack_into('<h',sound,at,rng.choice(values))
        options[0x31]=(0,1,16,127,255)[n%5]
        struct.pack_into('<h',options,0x32,(0,1,-32768)[n%3])
        key=(0xA5A5A5A5&~16)|((n//2)%2)*16
        yield sound,options,mode,key,(0,1,32768,65535)[(n//5)%4],n%2


def main():
    owners=source_owners(ROOT/'src/main/PAL')
    body=definition_source(owners['SpuGetKeyStatus'].read_text(),'SpuGetKeyStatus')
    caller=definition_source(owners['SoundQueueUpdate'].read_text(),'SoundQueueUpdate')
    assert caller.count(' asm("$2")')==1
    caller=caller.replace(' asm("$2")','') # Only the existing target register annotation.
    assert 'SpuGetKeyStatus(0x10)' in caller and 'asm(' not in caller
    entry,status,service=map(address_of,('SoundQueueUpdate','SpuGetKeyStatus','SsUtKeyOnV'))
    state,options_at,mode_at,pointer,software=map(address_of,('SoundState','Options','GameModeType','_spu_RXX','_spu_keystat'))
    exe=(ROOT/'assets/PAL/main.exe').read_bytes();base,=struct.unpack_from('<I',exe,0x18)
    memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000)
    write=lambda at,data:memory.mem_write(at&0x1FFFFFFF,bytes(data))
    read=lambda at,size:bytes(memory.mem_read(at&0x1FFFFFFF,size))
    def store(cpu,access,at,size,value,user):
        assert state<=at and at+size<=state+SIZE or STACK-256<=at<STACK,hex(at)
    cpu=Cpu(memory,0,100,101,102,store);write(base,exe[0x800:]);coverage=set()
    with TemporaryDirectory() as directory:
        source,library=Path(directory)/'native.c',Path(directory)/'native.dylib'
        source.write_text(PREFIX+body+caller+TAIL)
        subprocess.run(['cc','-std=c11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                        '-fsanitize=undefined,bounds','-fno-sanitize-recover=all',
                        '-I'+str(ROOT/'include'),str(source),'-o',str(library)],check=True)
        native=C.CDLL(str(library));native.run_case.restype=C.c_uint
        native.run_case.argtypes=[C.c_void_p,C.c_void_p,C.c_int16,C.c_uint32,C.c_uint16,
                                 C.c_uint,C.c_void_p,C.c_void_p,C.c_void_p]
        for version in ('original','linked'):
            if version=='linked':
                with (ROOT/'build/PAL/main.elf').open('rb') as stream:
                    elf=ELFFile(stream)
                    for name in ('SoundQueueUpdate','SpuGetKeyStatus'):
                        symbol=elf.get_section_by_name('.symtab').get_symbol_by_name(name)[0]
                        section=elf.get_section(symbol['st_shndx']);offset=symbol['st_value']-section['sh_addr']
                        write(symbol['st_value'],section.data()[offset:offset+symbol['st_size']])
            for number,(sound,options,mode,key,envelope,mutation) in enumerate(cases()):
                want_sound,want_options,wanted,queried=oracle(sound,options,mode,key,envelope,mutation)
                out_sound,out_options,out_calls=C.create_string_buffer(SIZE),C.create_string_buffer(64),(C.c_int32*16)()
                count=native.run_case(C.create_string_buffer(bytes(sound)),C.create_string_buffer(bytes(options)),
                                     mode,key,envelope,mutation,out_sound,out_options,out_calls)
                assert out_sound.raw==want_sound and out_options.raw==want_options,(version,number,'native state')
                assert [list(out_calls[i*8:i*8+8]) for i in range(count)]==wanted
                write(state-16,b'\xA6'*16+sound+b'\x6A'*16);write(options_at,options)
                write(mode_at,struct.pack('<h',mode));registers=bytearray(b'\xA5'*512)
                struct.pack_into('<H',registers,76,envelope)
                write(BANK-16,b'\xA6'*16+registers+b'\x6A'*16)
                write(pointer,struct.pack('<I',BANK));write(software,struct.pack('<I',key))
                cpu.reset();cpu.r=[0]+[0xBEEF0000+reg for reg in range(1,32)]
                cpu.r[29],cpu.r[31],cpu.pc=STACK,STOP,entry;calls=[];queries=0
                for _ in range(2048):
                    if cpu.pc==STOP:break
                    if cpu.pc==status:queries+=1
                    if cpu.pc==service:
                        assert cpu.delay is None
                        args=[C.c_int32(v).value for v in cpu.r[4:8]]
                        args+=list(struct.unpack('<4i',read(cpu.r[29]+16,16)));calls.append(args)
                        if len(calls)==1 and mutation:
                            write(state+0x2C,struct.pack('<h',-1234));write(state+0x19D8,struct.pack('<i',6))
                            write(state+0x19F4,struct.pack('<h',-2345))
                            write(options_at+0x31,b'\x11'+struct.pack('<h',-9))
                        cpu.r[2]=(-123 if len(calls)==1 else 456)&0xFFFFFFFF;cpu.pc=cpu.r[31]
                    else:cpu.step()
                assert cpu.pc==STOP and queries==int(queried),(version,number,'control')
                assert calls==wanted,(version,number,calls,wanted)
                assert read(state-16,SIZE+32)==b'\xA6'*16+want_sound+b'\x6A'*16,(version,number,'MIPS state')
                assert read(options_at,64)==want_options and read(mode_at,2)==struct.pack('<h',mode)
                assert read(BANK-16,544)==b'\xA6'*16+registers+b'\x6A'*16
                assert read(pointer,4)==struct.pack('<I',BANK) and read(software,4)==struct.pack('<I',key)
                for reg in (*range(16,24),28,29,30):
                    assert cpu.r[reg]==(STACK if reg==29 else 0xBEEF0000+reg)
                coverage.add((queried,bool(wanted),bool(mutation)))
            print(f'PASS {version}: 2048 real queue/SPU native/MIPS/oracle cases; complete owners, callback reloads, volume boundaries and ABI',flush=True)
    assert coverage=={(q,p,m) for q in (False,True) for p in (False,True) for m in (False,True)}

if __name__=='__main__':main()
