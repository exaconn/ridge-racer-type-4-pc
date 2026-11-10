"""Actual SPU key-status C vs original/linked MIPS and a mask/ENVX oracle.

Both native register banks are complete owners above 4 GiB. MIPS executes one
bank in MMIO space and one in RAM, checking exact reads and unchanged state.
No body rewriting, address truncation or alternate native implementation.
"""
import ctypes as C
from pathlib import Path
import struct
import subprocess
from tempfile import TemporaryDirectory

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32

from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
BANKS = (0x1F801C00, 0x80320010)
STACK, STOP = 0x803F0000, 0x803FF000
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "rr/spu_key.h"
#include "psyq/libspu/spukeyst.h"

typedef struct {
    u8 before[16];
    SpuRegisterStorage bank;
    u8 after[16];
} GuardedBank;
static struct World {
    u8 before[16];
    GuardedBank banks[2];
    SpuRegisterBank *root;
    u32 keyed;
    u8 after[16];
} actual, expected;
#define _spu_RXX actual.root
#define _spu_keystat actual.keyed
'''
TAIL = r'''
int run_case(u32 mask, u32 keyed, unsigned bank, unsigned nullRoot,
             const u8 *registers, s32 wanted) {
    assert(sizeof(void *) == 8 && sizeof(unsigned long) >= sizeof(void *));
    assert((uintptr_t)&actual > UINT32_MAX);
    memset(&actual, 0xA5, sizeof(actual));
    for (unsigned i = 0; i < 2; i++) {
        memcpy(&actual.banks[i].bank, registers + i * 512, 512);
        assert((uintptr_t)&actual.banks[i].bank > UINT32_MAX);
    }
    assert(bank < 2);
    actual.root = nullRoot ? 0 : &actual.banks[bank].bank.registers;
    actual.keyed = keyed;
    expected = actual;
    int result = SpuGetKeyStatus((s32)mask);
    assert(result == wanted);
    assert(!memcmp(&actual, &expected, sizeof(actual)));
    return result;
}
'''


def inputs():
    result = [(mask, key, bank, null_root, 0)
              for mask in (0, 0x1000000, 0x80000000, 0xFF000000)
              for key in (0, 0xFFFFFFFF, 0x12345678)
              for bank in range(2) for null_root in range(2)]
    for index in range(24):
        for shape in range(4):
            mask = (1 << index) if shape & 1 else ((0xFFFFFF << index) & 0xFFFFFF)
            if shape & 2:
                mask |= 0xFF000000
            for on in range(2):
                key = ((0xA55AA55A & ~(1 << index)) | (on << index))
                for envelope in (0, 1, 32767, 32768, 65535):
                    result.append((mask, key, (index + shape + on) & 1, 0, envelope))
    for envelope in range(65536):
        index = envelope % 24
        mask = (1 << index) | (0xFF000000 if envelope & 1 else 0)
        key = ((envelope * 0x31415927) ^ 0x9E3779B9) & 0xFFFFFFFF
        result.append((mask, key, (envelope // 24) & 1, 0, envelope))
    return result


def main():
    owner = source_owners(ROOT / 'src/main/PAL')['SpuGetKeyStatus']
    body = definition_source(owner.read_text(), 'SpuGetKeyStatus')
    assert 'asm' not in body and '(int)' not in body
    entry = address_of('SpuGetKeyStatus')
    pointer, software = address_of('_spu_RXX'), address_of('_spu_keystat')
    executable = (ROOT / 'assets/PAL/main.exe').read_bytes()
    base, = struct.unpack_from('<I', executable, 0x18)
    memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
    memory.mem_map(0, 0x400000)
    memory.mem_map(0x1F801000, 4096)

    def write(address, data):
        memory.mem_write(address & 0x1FFFFFFF, bytes(data))

    def read(address, size):
        return bytes(memory.mem_read(address & 0x1FFFFFFF, size))

    def no_store(cpu, access, address, size, value, user):
        raise AssertionError(('unexpected store', hex(address), size))

    class StatusCpu(Cpu):
        def load(self, address, size):
            at = address & 0x1FFFFFFF
            if not (entry & 0x1FFFFFFF <= at < (entry & 0x1FFFFFFF) + 0x90):
                self.reads.append((at, size))
            return super().load(address, size)

    cpu = StatusCpu(memory, 0, 100, 101, 102, no_store)
    write(base, executable[0x800:])
    initial = b''.join(struct.pack('<H', (i * 257 + bank * 613 + 0x1234) & 65535)
                       for bank in range(2) for i in range(256))
    cases = inputs()
    with TemporaryDirectory() as directory:
        source = Path(directory) / 'native.c'
        library = Path(directory) / 'native.dylib'
        source.write_text(PREFIX + body + TAIL)
        subprocess.run([
            'cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
            '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
            '-I' + str(ROOT / 'include'), str(source), '-o', str(library)
        ], check=True)
        native = C.CDLL(str(library))
        native.run_case.argtypes = [C.c_uint32, C.c_uint32, C.c_uint, C.c_uint,
                                   C.c_void_p, C.c_int32]
        native.run_case.restype = C.c_int32
        for version in ('original', 'linked'):
            if version == 'linked':
                with (ROOT / 'build/PAL/main.elf').open('rb') as stream:
                    elf = ELFFile(stream)
                    symbol = elf.get_section_by_name('.symtab').get_symbol_by_name('SpuGetKeyStatus')[0]
                    assert symbol['st_value'] == entry and symbol['st_size'] == 0x90
                    section = elf.get_section(symbol['st_shndx'])
                    offset = symbol['st_value'] - section['sh_addr']
                    write(entry, section.data()[offset:offset + symbol['st_size']])
            voices, statuses, bank_ids, envelopes = set(), set(), set(), set()
            for number, (mask, key, bank, null_root, envelope) in enumerate(cases):
                selected = mask & 0xFFFFFF
                index = (selected & -selected).bit_length() - 1
                values = bytearray(initial)
                if selected:
                    assert not null_root
                    struct.pack_into('<H', values, bank * 512 + index * 16 + 12, envelope)
                    wanted = (1 if envelope else 3) if key & (1 << index) else (2 if envelope else 0)
                else:
                    wanted = -1
                native.run_case(mask, key, bank, null_root, C.create_string_buffer(bytes(values)), wanted)
                for slot, address in enumerate(BANKS):
                    write(address - 16, b'\xA6' * 16 + values[slot * 512:(slot + 1) * 512] + b'\x6A' * 16)
                root = 0 if null_root else BANKS[bank]
                write(pointer, struct.pack('<I', root))
                write(software, struct.pack('<I', key))
                cpu.reset()
                cpu.reads = []
                cpu.r = [0] + [0xBEEF0000 + reg for reg in range(1, 32)]
                cpu.r[4], cpu.r[29], cpu.r[31], cpu.pc = mask, STACK, STOP, entry
                for _ in range(256):
                    if cpu.pc == STOP:
                        break
                    cpu.step()
                assert cpu.pc == STOP and C.c_int32(cpu.r[2]).value == wanted, (version, number)
                trace = [] if not selected else [(pointer & 0x1FFFFFFF, 4),
                         (software & 0x1FFFFFFF, 4), ((root + index * 16 + 12) & 0x1FFFFFFF, 2)]
                assert cpu.reads == trace, (version, number, cpu.reads, trace)
                assert read(pointer, 4) == struct.pack('<I', root)
                assert read(software, 4) == struct.pack('<I', key)
                for slot, address in enumerate(BANKS):
                    assert read(address - 16, 544) == b'\xA6' * 16 + values[slot * 512:(slot + 1) * 512] + b'\x6A' * 16
                for reg in (*range(16, 24), 28, 29, 30):
                    assert cpu.r[reg] == (STACK if reg == 29 else 0xBEEF0000 + reg)
                if selected:
                    voices.add(index)
                    envelopes.add(envelope)
                statuses.add(wanted)
                bank_ids.add(bank)
            assert voices == set(range(24)) and statuses == {-1, 0, 1, 2, 3}
            assert bank_ids == {0, 1} and envelopes == set(range(65536))
            print(f'PASS {version}: {len(cases)} native/MIPS/oracle cases; all ENVX values, '
                  '24 voices, first-bit selection, exact MMIO reads, full native owners and ABI', flush=True)


if __name__ == '__main__':
    main()
