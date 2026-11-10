"""Actual model texture patcher: native mesh owners and original PAL stores.

The byte oracle follows the documented command extents independently of the
production field views. Both original and freshly compiled MIPS execute against
the same fixtures, including shared command streams and untouched owner guards.
"""
import ctypes as C
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32, UC_HOOK_MEM_WRITE
from unicorn.mips_const import UC_MIPS_REG_0, UC_MIPS_REG_PC

from c_functions import address_of, definition_source, source_owners
from verify_c_unit import verify

ROOT = Path(__file__).resolve().parents[2]
NAME = 'PatchOffsetTableTexPages'
TABLE, ARENA, STACK, STOP = 0x80170010, 0x80180000, 0x803F0000, 0x803FF000
SIZES = {1: 28, 3: 20, 4: 12, 5: 24, 6: 8, 7: 20, 8: 28,
         9: 20, 10: 24, 11: 32, 14: 28, 15: 36}
# command -> (CLUT halfword, texture-page halfword)
ATTRIBUTES = {1: (14, 18), 11: (22, 26), 14: (18, 22), 15: (22, 26)}
COUNTS = (-2147483648, -17, -1, 0, *range(1, 9))
SLOTS = (-2147483648, -65537, -32768, -1, 0, 1, 3, 4, 15, 32767, 2147483647)
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "game/data/cdfile2.h"
'''
TAIL = r'''
void run_case(const u8 *input,u32 bytes,s32 count,const u32 *offsets,
              s32 slot,u8 *output) {
    const size_t tableBytes=offsetof(RroModelTable,entries)+8*sizeof(RroEffectMesh);
    u8 *arenaOwner=malloc((size_t)bytes+32),*tableOwner=malloc(tableBytes+32);
    u8 *saved=malloc(tableBytes+32);
    assert(arenaOwner&&tableOwner&&saved);
    u8 *arena=arenaOwner+16;
    RroModelTable *table=(RroModelTable*)(tableOwner+16);
    _Static_assert(sizeof(void*)==8&&sizeof(RroEffectMesh)==24,"full native mesh pointers");
    _Static_assert(offsetof(RroModelTable,entries)==8,"native table alignment");
    assert((uintptr_t)table>UINT32_MAX&&(uintptr_t)arena>UINT32_MAX);
    memset(arenaOwner,0xA6,(size_t)bytes+32);memcpy(arena,input,bytes);
    memset(tableOwner,0xB5,tableBytes+32);table->count=count;
    for(unsigned i=0;i<8;i++) {
        assert(offsets[i]+4<=bytes&&(offsets[i]&3)==0);
        table->entries[i].vertices=(const Vec3s8*)(arena+(i*8U)%bytes);
        table->entries[i].vertexCount=0x91827364U+i;
        table->entries[i].commands=(const u32*)(arena+offsets[i]);
        assert((uintptr_t)table->entries[i].commands>UINT32_MAX);
    }
    memcpy(saved,tableOwner,tableBytes+32);
    PatchOffsetTableTexPages(table,slot);
    assert(!memcmp(saved,tableOwner,tableBytes+32));
    for(unsigned i=0;i<16;i++)assert(arenaOwner[i]==0xA6&&arena[bytes+i]==0xA6);
    memcpy(output,arena,bytes);
    free(saved);free(tableOwner);free(arenaOwner);
}
'''


def fixture(seed):
    rng = random.Random(seed ^ 0x12C7C)
    data = bytearray(rng.randbytes(16))
    offsets = []
    for stream in range(4):
        offsets.append(len(data))
        commands = list(SIZES)
        rng.shuffle(commands)
        for command in commands:
            count = (seed + command + stream) % 5
            data += struct.pack('<I', command | count << 16)
            data += rng.randbytes(SIZES[command] * count)
        # Unhandled commands consume only their header. Zero-count commands
        # also leave the cursor immediately at the following header.
        for command in (0, 2, 12, 13, 16, 65535):
            data += struct.pack('<I', command | 3 << 16)
        data += bytes(4) + rng.randbytes(16)
    order = [offsets[i] for i in (0, 1, 0, 2, 3, 1, 3, 0)]
    return bytes(data), order, COUNTS[seed % len(COUNTS)], SLOTS[(seed // len(COUNTS)) % len(SLOTS)]


def oracle(data, offsets, count, slot):
    result, stores = bytearray(data), []
    texture_x = ((slot & 0xFFFFFFFF) * 64 + 384) & 0xFFFFFFFF
    page_x, clut_x = (texture_x // 64) & 15, (texture_x // 16) & 63
    for offset in offsets[:max(count, 0)]:
        at = offset
        while True:
            header, = struct.unpack_from('<I', result, at)
            at += 4
            if header == 0:
                break
            command, records = header & 65535, header >> 16
            size = SIZES.get(command, 0)
            if command in ATTRIBUTES:
                clut, page = ATTRIBUTES[command]
                for record in range(records):
                    start = at + record * size
                    for field, mask, value in ((page, 0xFFE0, page_x), (clut, 0xBFC3, clut_x)):
                        old, = struct.unpack_from('<H', result, start + field)
                        new = (old & mask) | value
                        struct.pack_into('<H', result, start + field, new)
                        stores.append((ARENA + start + field, 2, new))
            at += records * size
    return bytes(result), stores


class ModelTexturePatchTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        cls.directory = Path(cls.temp.name)
        cls.owner = source_owners(ROOT / 'src/main/PAL')[NAME]
        body = definition_source(cls.owner.read_text(), NAME)
        source = cls.directory / 'native.c'
        library = cls.directory / 'native.dylib'
        source.write_text(PREFIX + body + TAIL)
        result = subprocess.run([
            'cc', '-std=gnu11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
            '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
            '-I', str(ROOT / 'include'), str(source), '-o', str(library),
        ], capture_output=True, text=True)
        if result.returncode:
            raise AssertionError(result.stderr)
        cls.library = C.CDLL(str(library))
        cls.native = cls.library.run_case
        cls.native.argtypes = [C.c_void_p, C.c_uint32, C.c_int32, C.POINTER(C.c_uint32), C.c_int32, C.c_void_p]
        cls.native.restype = None

    def check_native(self, data, offsets, count, slot):
        wanted, _ = oracle(data, offsets, count, slot)
        output = C.create_string_buffer(len(data))
        self.native(C.create_string_buffer(data), len(data), count, (C.c_uint32 * 8)(*offsets), slot, output)
        self.assertEqual(output.raw, wanted)

    def test_native_streams_counts_slots_aliases_and_guards(self):
        for seed in range(2112):
            self.check_native(*fixture(seed))

    def test_every_attribute_encoding_and_maximum_record_count(self):
        for command, (clut, page) in ATTRIBUTES.items():
            size = SIZES[command]
            data = bytearray([0xA7] * 16)
            start = len(data)
            index = 0
            for count in (65535, 1):
                data += struct.pack('<I', command | count << 16)
                for _ in range(count):
                    record = bytearray([0x5C] * size)
                    struct.pack_into('<H', record, clut, index)
                    struct.pack_into('<H', record, page, 65535 - index)
                    data += record
                    index += 1
            data += bytes(4) + bytes([0x7A] * 16)
            self.assertEqual(index, 65536)
            self.check_native(bytes(data), [start] * 8, 2, -65537)

    def test_original_and_compiled_mips_stores_and_abi(self):
        report = verify(self.owner, self.directory / 'compiled')
        self.assertTrue(report['text_extent_correct'] and report['rodata_identical'])
        self.assertTrue(all(row['levenshtein'] == 0 and row['address_correct'] for row in report['functions']))
        compiled = self.directory / 'compiled' / (self.owner.stem + '.elf')
        with compiled.open('rb') as stream:
            elf = ELFFile(stream)
            symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(NAME)[0]
            section = elf.get_section(symbol['st_shndx'])
            offset = symbol['st_value'] - section['sh_addr']
            code = section.data()[offset:offset + symbol['st_size']]
        image = (ROOT / 'assets/PAL/main.exe').read_bytes()
        base, = struct.unpack_from('<I', image, 0x18)
        for variant in ('original', 'compiled'):
            memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
            memory.mem_map(0, 0x400000)
            write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b))
            read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
            write(base, image[0x800:])
            if variant == 'compiled':
                write(address_of(NAME), code)
            stores = []

            def store(cpu, access, address, size, value, user):
                address = (address & 0x1FFFFFFF) | 0x80000000
                stores.append((address, size, value & ((1 << (size * 8)) - 1)))

            memory.hook_add(UC_HOOK_MEM_WRITE, store)
            for seed in range(264):
                data, offsets, count, slot = fixture(seed)
                wanted, expected_stores = oracle(data, offsets, count, slot)
                table = bytearray([0xB5] * (16 + 4 + 8 * 12 + 16))
                struct.pack_into('<i', table, 16, count)
                for i, start in enumerate(offsets):
                    struct.pack_into('<III', table, 20 + i * 12, ARENA + i * 8, 0x91827364 + i, ARENA + start)
                write(TABLE - 16, table)
                write(ARENA - 16, bytes([0xA6] * 16) + data + bytes([0xA6] * 16))
                write(STACK - 64, bytes([0xAC] * 128))
                for register in range(1, 32):
                    memory.reg_write(UC_MIPS_REG_0 + register, 0xBEEF0000 + register)
                for register, value in ((4, TABLE), (5, slot & 0xFFFFFFFF), (29, STACK), (31, STOP)):
                    memory.reg_write(UC_MIPS_REG_0 + register, value)
                stores.clear()
                memory.emu_start(address_of(NAME), STOP, count=200000)
                self.assertEqual(memory.reg_read(UC_MIPS_REG_PC), STOP, (variant, seed))
                self.assertEqual(stores, expected_stores, (variant, seed))
                self.assertEqual(read(ARENA - 16, len(data) + 32), bytes([0xA6] * 16) + wanted + bytes([0xA6] * 16))
                self.assertEqual(read(TABLE - 16, len(table)), bytes(table))
                self.assertEqual(read(STACK - 64, 128), bytes([0xAC] * 128))
                for register in (*range(16, 24), 28, 30):
                    self.assertEqual(memory.reg_read(UC_MIPS_REG_0 + register), 0xBEEF0000 + register)
                self.assertEqual(memory.reg_read(UC_MIPS_REG_0 + 29), STACK)
                self.assertEqual(memory.reg_read(UC_MIPS_REG_0 + 31), STOP)


if __name__ == '__main__':
    unittest.main()
