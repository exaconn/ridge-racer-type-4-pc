"""Current CD file request APIs with full pointers and original-MIPS protocols."""
import ctypes as C
import os
from pathlib import Path
import struct
import subprocess
from tempfile import TemporaryDirectory

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32

from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
BUFFERS, STACK, STOP = 0x80250010, 0x803F0000, 0x803FF000
CASES, STATE = 6912, 8*4+16*8+64
OUTPUT = 12+3*STATE+16
NAMES = ('CdFileReadStart', 'CdFileReadRange', 'CdFileFinish')
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "rr/cd.h"
#include "game/data/cdfile2.h"
static struct World {
 u8 before[16];s32 state;void *buffer;s32 index,offset,sectors,bytes;
 CdLocation location;CdFileEntry files[16];u8 buffers[2][32];u8 after[16];
} world,expected;
#define CdFileState world.state
#define CdActiveReadBuffer world.buffer
#define CdFileIndexCurrent world.index
#define CdFileSectorOffset world.offset
#define CdFileSectors world.sectors
#define CdFileBytesRead world.bytes
#define CdFileLocation world.location
#define CdFileTable world.files
static unsigned scenario,calls;static u8 *trace;static u32 args[4];
static s32 word(u32 x){s32 y;memcpy(&y,&x,4);return y;}
static u32 address(const void *p) {
 for(unsigned i=0;i<2;i++)if(p==world.buffers[i])return 0x80250010U+i*32;
 if(p==world.location.c)return LOCATION_ADDRESS;
 if(!p)return 0;
 uintptr_t x=(uintptr_t)p,b=(uintptr_t)world.files;
 assert(x>=b&&x-b<sizeof(world.files));return TABLE_ADDRESS+(u32)(x-b);
}
static void snapshot(u8 *out) {
 u32 values[]={(u32)world.state,address(world.buffer),(u32)world.index,(u32)world.offset,
  (u32)world.sectors,(u32)world.bytes,0,0};
 memcpy(&values[6],world.location.c,4);memcpy(out,values,32);out+=32;
 memcpy(out,world.files,128);out+=128;memcpy(out,world.buffers,64);
}
static void change(unsigned serial) {
 world.state=expected.state=word(scenario*0x1234567U+serial);
 world.buffer=expected.buffer=world.buffers[(scenario+serial)&1];
 world.index=expected.index=(scenario+serial)&15;
 world.files[(scenario+serial)&15].sectors=expected.files[(scenario+serial)&15].sectors=word(0x80000000U|scenario);
 world.buffers[serial&1][scenario&31]^=(u8)(scenario+serial);
 expected.buffers[serial&1][scenario&31]=world.buffers[serial&1][scenario&31];
}
s32 CdPosToInt(void *p) {
 assert(calls++==0);args[0]=address(p);snapshot(trace);trace+=STATE_SIZE;
 change(1);return word(scenario*0x9E3779B9U^0x80000000U);
}
u8 *CdIntToPos(s32 sector,u8 *p) {
 assert(calls++==1&&p==world.location.c);args[1]=(u32)sector;args[2]=address(p);
 snapshot(trace);trace+=STATE_SIZE;change(2);
 for(unsigned i=0;i<4;i++)p[i]=(u8)(((u32)sector>>(i*8))^(scenario+i*17));
 return p;
}
'''
TAIL = r'''
void run_case(unsigned number,unsigned function,s32 file,s32 offset,s32 count,const u8 *input,u8 *output) {
 assert(sizeof(void*)==8&&(uintptr_t)&world>UINT32_MAX);
 scenario=number;calls=0;trace=output+12;memset(output,0,OUTPUT_SIZE);memset(args,0,sizeof(args));
 memset(&world,0xA5,sizeof(world));u32 values[8];memcpy(values,input,32);
 world.state=word(values[0]);world.buffer=world.buffers[(values[1]-0x80250010U)/32];
 world.index=word(values[2]);world.offset=word(values[3]);world.sectors=word(values[4]);world.bytes=word(values[5]);
 memcpy(world.location.c,values+6,4);memcpy(world.files,input+32,128);memcpy(world.buffers,input+160,64);expected=world;
 void *buffer=(number&32)?NULL:world.buffers[(number>>4)&1];s32 result=0;
 if(function==0)result=CdFileReadStart(file,buffer);
 else if(function==1)result=CdFileReadRange(file,offset,count,buffer);
 else CdFileFinish();
 expected.state=world.state;expected.buffer=world.buffer;expected.index=world.index;
 expected.offset=world.offset;expected.sectors=world.sectors;expected.bytes=world.bytes;expected.location=world.location;
 assert(!memcmp(&world,&expected,sizeof(world)));
 u32 head[]={calls,(u32)result,address(buffer)};memcpy(output,head,12);
 snapshot(output+12+2*STATE_SIZE);memcpy(output+12+3*STATE_SIZE,args,16);
}
'''


def main():
    os.chdir(ROOT); owners = source_owners(ROOT/'src/main/PAL')
    addresses = {n: address_of(n) for n in ('CdFileState', 'CdActiveReadBuffer', 'CdFileIndexCurrent', 'CdFileSectorOffset', 'CdFileSectors', 'CdFileBytesRead', 'CdFileLocation', 'CdFileTable')}
    with TemporaryDirectory() as directory:
        source, library = Path(directory)/'native.c', Path(directory)/'native.dylib'
        prefix = PREFIX.replace('LOCATION_ADDRESS', hex(addresses['CdFileLocation'])+'U').replace('TABLE_ADDRESS', hex(addresses['CdFileTable'])+'U').replace('STATE_SIZE', str(STATE))
        bodies = '\n'.join(definition_source(owners[n].read_text(), n) for n in NAMES)
        source.write_text(prefix+bodies+TAIL.replace('STATE_SIZE', str(STATE)).replace('OUTPUT_SIZE', str(OUTPUT)))
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-Iinclude', str(source), '-o', str(library)], check=True)
        native = C.CDLL(str(library)); native.run_case.argtypes = [C.c_uint, C.c_uint, C.c_int32, C.c_int32, C.c_int32, C.c_void_p, C.c_void_p]
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN); memory.mem_map(0, 0x400000)
        write = lambda a, value: memory.mem_write(a & 0x1FFFFFFF, bytes(value))
        read = lambda a, size: bytes(memory.mem_read(a & 0x1FFFFFFF, size))
        put = lambda a, value: write(a, struct.pack('<I', value & 0xFFFFFFFF))
        get = lambda a: int.from_bytes(read(a, 4), 'little')
        original = (ROOT/'assets/PAL/main.exe').read_bytes(); base, = struct.unpack_from('<I', original, 0x18); write(base, original[0x800:])
        if os.getenv('RR_COMPILED_ELF'):
            with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                elf = ELFFile(stream)
                for name in NAMES:
                    symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(name)[0]
                    section = elf.get_section(symbol['st_shndx']); at = symbol['st_value']-section['sh_addr']; assert symbol['st_value'] == address_of(name)
                    write(symbol['st_value'], section.data()[at:at+symbol['st_size']])
        services = {address_of(n): i for i, n in enumerate(('CdPosToInt', 'CdIntToPos'))}

        def store(cpu, access, address, size, value, user):
            address &= 0x1FFFFFFF
            regions = [(addresses[n], 4) for n in addresses if n != 'CdFileTable']+[(STACK-64, 64)]
            assert any((a & 0x1FFFFFFF) <= address and address+size <= (a & 0x1FFFFFFF)+n for a, n in regions), (hex(address), size)

        cpu = Cpu(memory, 0, 100, 101, 102, store); statuses = (0, 1, -1, 6); profiles = (0, 1, -1, -2147483648, 2147483647, 0x12345678)
        paths = set()
        for case in range(CASES):
            function, file = case % 3, (case//3)&15
            offset, count = profiles[(case//48)%6], profiles[(case//288)%6]
            status = statuses[(case//1728)%4]
            values = [status & 0xFFFFFFFF, BUFFERS+(case&1)*32, file, 0x12345678, count & 0xFFFFFFFF, 0x80000001, case*17, 0]
            data = struct.pack('<8I', *(v & 0xFFFFFFFF for v in values))+bytes((case*19+i*29)&255 for i in range(192))
            incoming = C.create_string_buffer(data); output = (C.c_ubyte*OUTPUT)(); native.run_case(case, function, file, offset, count, incoming, output)
            expected = bytes(output)
            for i, name in enumerate(list(addresses)[:7]): put(addresses[name], values[i])
            write(addresses['CdFileTable'], data[32:160]); write(BUFFERS, data[160:224])
            cpu.reset(); cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
            buffer = 0 if case&32 else BUFFERS+((case >> 4)&1)*32
            cpu.r[4:8] = [file, buffer, 0, 0] if function == 0 else [file, offset & 0xFFFFFFFF, count & 0xFFFFFFFF, buffer]
            cpu.r[29] = STACK; cpu.r[31] = STOP; cpu.pc = address_of(NAMES[function]); calls, trace, args = 0, [], [0]*4

            def snapshot():
                return b''.join(read(addresses[n], 4) for n in list(addresses)[:7])+bytes(4)+read(addresses['CdFileTable'], 128)+read(BUFFERS, 64)

            def change(serial):
                put(addresses['CdFileState'], case*0x1234567+serial); put(addresses['CdActiveReadBuffer'], BUFFERS+((case+serial)&1)*32)
                put(addresses['CdFileIndexCurrent'], (case+serial)&15)
                put(addresses['CdFileTable']+((case+serial)&15)*8+4, 0x80000000|case)
                at = BUFFERS+(serial&1)*32+(case&31); write(at, bytes([read(at, 1)[0]^((case+serial)&255)]))

            for _ in range(200):
                if cpu.pc == STOP: break
                if cpu.pc not in services: cpu.step(); continue
                service = services[cpu.pc]; assert calls == service; trace.append(snapshot())
                if service == 0: args[0] = cpu.r[4]; change(1); result = (case*0x9E3779B9^0x80000000) & 0xFFFFFFFF
                else:
                    args[1:3] = cpu.r[4:6]; assert args[2] == addresses['CdFileLocation']; change(2)
                    write(args[2], bytes(((args[1] >> (i*8))^(case+i*17))&255 for i in range(4))); result = args[2]
                calls += 1; resume = cpu.r[31]
                for reg in (*range(2, 16), 24, 25): cpu.r[reg] = 0xDEAD0000+reg
                cpu.r[2] = result; cpu.pc = resume
            assert cpu.pc == STOP
            actual = struct.pack('<3I', calls, cpu.r[2] if function != 2 else 0, buffer)+b''.join(trace)+bytes((2-calls)*STATE)+snapshot()+struct.pack('<4I', *args)
            assert actual == expected, (case, NAMES[function], 'complete owners / exact conversion protocol / full native pointer')
            assert cpu.r[29] == STACK and cpu.r[31] == STOP
            for reg in (*range(16, 24), 28, 30): assert cpu.r[reg] == 0xBEEF0000+reg
            paths.add((function, status == 0))
        assert paths == {(f, idle) for f in range(3) for idle in (False, True)}
        print('PASS', CASES, 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
              'MIPS/native CD request cases; full destination pointers, busy paths, complete owners, mutable conversion services, wrapped sectors/byte counts, ABI, guards, UBSan')


if __name__ == '__main__':
    main()
