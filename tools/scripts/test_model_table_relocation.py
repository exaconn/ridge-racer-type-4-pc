"""Actual counted model-table relocator: wire words, full native roots and MIPS.

The resource remains a PAL wire table. This does not model a native resource
decoder: only the published table root is a native pointer at this boundary.
"""
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
TABLE, STACK, STOP = 0x80200010, 0x803F0000, 0x803FF000
CASES, CAPACITY = 4096, 16
ALLOCATION = 16 + 4 + CAPACITY * 12 + 16
OUTPUT = ALLOCATION + 0x7C + 4
COUNTS = (-2147483648, -17, -1, 0, *range(1, CAPACITY + 1))
WORDS = (0, 1, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF,
         (-TABLE) & 0xFFFFFFFF, (0xFFFFFFFF - TABLE) & 0xFFFFFFFF)
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "game/gfx/model.h"
#include "rr/rro_model.h"
static struct World {
 u8 before[16];RroSceneScratchPrefix scene;s32 count;u8 after[16];
} world,expected;
#define SCRATCHPAD ((u8*)&world.scene)
#define ActiveModelVertexCount world.count
'''
TAIL = r'''
void run_case(const u8 *input,u8 *output){
 const size_t bytes=16+sizeof(RroModelTableWire)+16*12+16;
 u8 *allocation=malloc(bytes),*wanted=malloc(bytes);
 assert(allocation&&wanted&&sizeof(void*)==8&&sizeof(unsigned long)==8);
 RroModelTableWire *table=(RroModelTableWire*)(allocation+16);
 assert((uintptr_t)table>UINT32_MAX&&(uintptr_t)table%_Alignof(RroModelTableWire)==0);
 memcpy(allocation,input,bytes);memcpy(wanted,input,bytes);
 s32 count;memcpy(&count,input+16,4);
 u32 base=(u32)(uintptr_t)table;
 for(int i=0;i<count;i++)for(unsigned field=0;field<3;field+=2){
  size_t at=16+4+(i*3+field)*4;u32 word;
  memcpy(&word,wanted+at,4);word+=base;memcpy(wanted+at,&word,4);
 }
 memset(&world,0xA5,sizeof(world));expected=world;
 expected.scene.modelEntries48=table->words;expected.count=count;
 RelocateModelTable(table);
 assert(world.scene.modelEntries48==table->words&&world.count==count);
 assert(!memcmp(&world,&expected,sizeof(world))&&!memcmp(allocation,wanted,bytes));
 memcpy(output,allocation,bytes);
 /* Normalize only relocated PAL words. Native-pointer correctness was checked
  * against the real heap owner before converting the result to a wire snapshot. */
 for(int i=0;i<count;i++)for(unsigned field=0;field<3;field+=2){
  size_t at=16+4+(i*3+field)*4;u32 word;
  memcpy(&word,output+at,4);word=word-base+0x80200010U;memcpy(output+at,&word,4);
 }
 memset(output+bytes,0xA5,0x7C);u32 published=0x80200014U;
 memcpy(output+bytes+0x48,&published,4);memcpy(output+bytes+0x7C,&world.count,4);
 free(wanted);free(allocation);
}
'''


def main():
    os.chdir(ROOT)
    name = 'RelocateModelTable'
    source = source_owners(ROOT / 'src/main/PAL')[name]
    body = definition_source(source.read_text(), name)
    with TemporaryDirectory() as directory:
        source, library = Path(directory) / 'native.c', Path(directory) / 'native.dylib'
        source.write_text(PREFIX + body + TAIL)
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC',
                        '-Wall', '-Wextra', '-Werror', '-fsanitize=undefined,bounds',
                        '-fno-sanitize-recover=all', '-Iinclude', str(source),
                        '-o', str(library)], check=True)
        native = C.CDLL(str(library))
        native.run_case.argtypes = [C.c_void_p, C.c_void_p]
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0, 0x400000)
        memory.mem_map(0x1F800000, 4096)
        write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b))
        read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
        executable = (ROOT / 'assets/PAL/main.exe').read_bytes()
        base, = struct.unpack_from('<I', executable, 0x18)
        write(base, executable[0x800:])
        if os.getenv('RR_COMPILED_ELF'):
            with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                elf = ELFFile(stream)
                symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(name)[0]
                section = elf.get_section(symbol['st_shndx'])
                offset = symbol['st_value'] - section['sh_addr']
                assert symbol['st_value'] == address_of(name)
                write(symbol['st_value'], section.data()[offset:offset + symbol['st_size']])
        count_address = address_of('ActiveModelVertexCount')
        stores = []

        def check_store(cpu, access, address, size, value, user):
            address |= 0x80000000 if address < 0x400000 else 0
            stores.append((address, size, value & 0xFFFFFFFF))

        cpu = Cpu(memory, 0, 100, 101, 102, check_store)
        for case in range(CASES):
            incoming = bytearray((case * 19 + i * 29) & 255 for i in range(ALLOCATION))
            count = COUNTS[case % len(COUNTS)]
            struct.pack_into('<i', incoming, 16, count)
            for i in range(CAPACITY * 3):
                word = WORDS[(case + i) % len(WORDS)] if case & 1 else (case * 0x31415927 + i * 0x17171717) & 0xFFFFFFFF
                struct.pack_into('<I', incoming, 20 + i * 4, word)
            wanted = bytearray(incoming)
            wanted_stores = [(0x1F800048, 4, TABLE + 4),
                             (count_address, 4, count & 0xFFFFFFFF)]
            for i in range(max(count, 0)):
                for field in (0, 2):
                    at = 20 + i * 12 + field * 4
                    word, = struct.unpack_from('<I', incoming, at)
                    word = (word + TABLE) & 0xFFFFFFFF
                    struct.pack_into('<I', wanted, at, word)
                    wanted_stores.append((TABLE + 4 + i * 12 + field * 4, 4, word))
            scene = bytearray([0xA5] * 0x7C)
            struct.pack_into('<I', scene, 0x48, TABLE + 4)
            expected = wanted + scene + struct.pack('<i', count)
            output = C.create_string_buffer(OUTPUT)
            native.run_case(C.create_string_buffer(bytes(incoming)), output)
            assert output.raw == expected, (case, 'native/wire oracle; complete owners and guards')
            write(TABLE - 16, incoming)
            write(0x1F800000, bytes([0xA5] * 0x7C))
            write(count_address, bytes([0xA5] * 4))
            cpu.reset()
            cpu.r = [0] + [0xBEEF0000 + r for r in range(1, 32)]
            cpu.r[4], cpu.r[29], cpu.r[31], cpu.pc = TABLE, STACK, STOP, address_of(name)
            stores.clear()
            for _ in range(1000):
                if cpu.pc == STOP:
                    break
                cpu.step()
            assert cpu.pc == STOP and stores == wanted_stores, (case, stores, wanted_stores)
            actual = read(TABLE - 16, ALLOCATION) + read(0x1F800000, 0x7C) + read(count_address, 4)
            assert actual == expected, (case, 'MIPS/wire oracle; full table, scene and count')
            for r in (*range(16, 24), 28, 30):
                assert cpu.r[r] == 0xBEEF0000 + r
            assert cpu.r[29] == STACK and cpu.r[31] == STOP
        print('PASS', CASES, 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
              'RelocateModelTable MIPS/native cases; signed counts, wrapped PAL words, '
              'full native roots, exact stores, complete owners/guards, ABI, UBSan', flush=True)


if __name__ == '__main__':
    main()
