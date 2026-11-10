"""Exercise actual RRO prefetch bodies at signed and unsigned index boundaries.

Compare native C, original MIPS and linked MIPS with an independent byte oracle.
Only existing physical register annotations are removed for the LP64 build.
Counts are nonpositive; the existing raw, lit and envmap fixtures cover emission.
"""
import ctypes as C
from pathlib import Path
import re
import struct
import subprocess
from tempfile import TemporaryDirectory

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32

from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
NAMES = ('EmitRroRawTexturedQuads', 'EmitRroLitGouraudQuads', 'EmitRroEnvMapQuads')
VERTEX_COUNT, ORIGIN_INDEX = 98304, 32768
VERTICES, STREAM, GPU = 0x80180010, 0x80300010, 0x80310010
FRAME, STACK, STOP = 0x80320010, 0x803F0000, 0x803FF000
WORK = 0x1F80007C

HEADER = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rr/frame_storage_types.h"
#include "rr/car_render_types.h"
#include "rr/rro_model.h"
#include "rr/gpu_address.h"

static struct {
    u8 before[16];
    Vec3s8 vertices[98304];
    u8 after[16];
} geometry, originalGeometry;

typedef union {
    RroRawTexturedQuad raw;
    RroLitGouraudQuad lit;
    RroEnvMapQuad env;
    u32 words[8];
} CommandStream;

static struct World {
    u8 before[16];
    RroModelScratch scratch;
    CommandStream stream;
    u32 gpu[8];
    u32 order[128];
    FrameStorage frame;
    u8 after[16];
} world, expected;

static s32 MaskBitSetting;
FrameStorage *CurrentDrawBuf;
#define SCRATCHPAD ((u8 *)&world.scratch.scene)

static void noProjection(void) {
    assert(!"projection or emission in a nonpositive-count case");
    abort();
}
#define RR_NOP_DEP() noProjection()
'''

TAIL = r'''
void init_geometry(void) {
    memset(&geometry, 0xA5, sizeof(geometry));
    for (u32 i = 0; i < 98304; i++) {
        geometry.vertices[i].words[0] = i * 0x31415927U + 0x1234567U;
        geometry.vertices[i].words[1] = i * 0x17171717U + 0x9E3779B9U;
    }
    originalGeometry = geometry;
}

void finish_geometry(void) {
    assert(!memcmp(&geometry, &originalGeometry, sizeof(geometry)));
}

void run_case(unsigned kind, u32 low0, s32 high1, u32 low2, s32 high3,
              s32 count, u8 *output) {
    assert(sizeof(void *) == 8 && (uintptr_t)&world > UINT32_MAX);
    assert((uintptr_t)geometry.vertices > UINT32_MAX);
    assert(low0 <= 65535 && low2 <= 65535);
    assert(high1 >= -32768 && high1 <= 32767);
    assert(high3 >= -32768 && high3 <= 32767);
    assert(count >= -32768 && count <= 0);

    memset(&world, 0xA5, sizeof(world));
    RroSceneScratchPrefix *scene = &world.scratch.scene;
    scene->view.projection.pointers.cursor = (u8 *)world.gpu;
    scene->view.projection.pointers.order = world.order;
    scene->modelEntries48 = 0;
    scene->commands50 = 0;
    scene->vertices54 = geometry.vertices + 32768;
    scene->normals58 = geometry.vertices;
    scene->draw74.draw.primitive = 0;
    scene->draw74.draw.counter = 0;
    RroRenderWork *work = &world.scratch.work7C.render;
    work->vertices00 = geometry.vertices + 32768;
    work->normals04 = geometry.vertices;
    work->ordering14 = world.order;
    u32 words[2] = {
        low0 | ((u32)(u16)high1 << 16),
        low2 | ((u32)(u16)high3 << 16)
    };
    memcpy(world.stream.words, words, sizeof(words));
    memset(&world.frame, 0, sizeof(world.frame));
    world.frame.buffer.environment00.draw00.clip.h = 256;
    CurrentDrawBuf = &world.frame;

    expected = world;
    s32 indices[4] = {(s32)low0, high1, (s32)low2, high3};
    for (unsigned i = 0; i < 4; i++) {
        expected.scratch.work7C.render.vertices28[i] = geometry.vertices[32768 + indices[i]];
    }
    const u32 *end;
    switch (kind) {
    case 0:
        end = EmitRroRawTexturedQuads(world.stream.words, work, count);
        break;
    case 1:
        end = EmitRroLitGouraudQuads(world.stream.words, work, count);
        break;
    default:
        assert(kind == 2);
        end = EmitRroEnvMapQuads(world.stream.words, work, count);
        break;
    }
    assert(end == world.stream.words && CurrentDrawBuf == &world.frame);
    assert(!memcmp(&world, &expected, sizeof(world)));
    memcpy(output, work->vertices28, 32);
}
'''


def cases():
    lows = (0, 1, 32767, 32768, 65534, 65535)
    highs = (-32768, -32767, -257, -256, -1, 0, 1, 255, 256, 32766, 32767)
    result = [
        (lows[i % 6], C.c_int16(i * 131 + 0x8000).value,
         lows[(i // 6) % 6], C.c_int16(i * 137 + 0xFFFF).value,
         (-32768, -1, 0)[i % 3])
        for i in range(4096)
    ]
    result.extend((low, a, 65535 - low, b, 0)
                  for a in highs for b in highs for low in lows)
    return result


def main():
    owners = source_owners(ROOT / 'src/main/PAL')
    bodies = [definition_source(owners[name].read_text(), name) for name in NAMES]
    bodies = [re.sub(r'(?:__asm__|asm)\("\$\d+"\)', '', body) for body in bodies]
    hardware = sorted(set(re.findall(r'\b(RR_GTE\w*)\(', '\n'.join(bodies))))
    stubs = '\n'.join(f'#define {name}(...) noProjection()' for name in hardware)
    executable = (ROOT / 'assets/PAL/main.exe').read_bytes()
    base, = struct.unpack_from('<I', executable, 0x18)
    memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
    memory.mem_map(0, 0x400000)
    memory.mem_map(0x1F800000, 4096)

    def write(address, data):
        memory.mem_write(address & 0x1FFFFFFF, bytes(data))

    def read(address, size):
        return bytes(memory.mem_read(address & 0x1FFFFFFF, size))

    def store_guard(cpu, access, address, size, value, user):
        at = address & 0x1FFFFFFF
        vertex_start, vertex_end = (WORK + 0x28) & 0x1FFFFFFF, (WORK + 0x48) & 0x1FFFFFFF
        assert (vertex_start <= at and at + size <= vertex_end
                or at == 0x1F800000
                or (STACK - 64) & 0x1FFFFFFF <= at and at + size <= STACK & 0x1FFFFFFF), (hex(address), size)

    cpu = Cpu(memory, 0, 100, 101, 102, store_guard)
    write(base, executable[0x800:])
    table = b''.join(
        struct.pack('<II', (i * 0x31415927 + 0x1234567) & 0xFFFFFFFF,
                    (i * 0x17171717 + 0x9E3779B9) & 0xFFFFFFFF)
        for i in range(VERTEX_COUNT)
    )
    write(VERTICES - 16, b'\xA6' * 16 + table + b'\x6A' * 16)
    mask_address, frame_pointer = address_of('MaskBitSetting'), address_of('CurrentDrawBuf')
    inputs = cases()
    with TemporaryDirectory() as directory:
        source, library = Path(directory) / 'native.c', Path(directory) / 'native.dylib'
        source.write_text(HEADER + stubs + '\n' + '\n'.join(bodies) + TAIL)
        subprocess.run([
            'cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
            '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
            '-I' + str(ROOT / 'include'), str(source),
            str(ROOT / 'src/native/gpu_address.c'), '-o', str(library)
        ], check=True)
        native = C.CDLL(str(library))
        native.init_geometry()
        native.run_case.argtypes = [C.c_uint, C.c_uint32, C.c_int32, C.c_uint32,
                                   C.c_int32, C.c_int32, C.c_void_p]
        for kind, name in enumerate(NAMES):
            entry = address_of(name)
            for version in ('original', 'linked'):
                if version == 'linked':
                    with (ROOT / 'build/PAL/main.elf').open('rb') as stream:
                        elf = ELFFile(stream)
                        symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(name)[0]
                        assert symbol['st_value'] == entry
                        section = elf.get_section(symbol['st_shndx'])
                        offset = symbol['st_value'] - section['sh_addr']
                        write(entry, section.data()[offset:offset + symbol['st_size']])
                seen = set()
                for number, (lo0, hi1, lo2, hi3, count) in enumerate(inputs):
                    output = C.create_string_buffer(32)
                    native.run_case(kind, lo0, hi1, lo2, hi3, count, output)
                    wanted = b''.join(table[(ORIGIN_INDEX + j) * 8:(ORIGIN_INDEX + j + 1) * 8]
                                      for j in (lo0, hi1, lo2, hi3))
                    assert output.raw == wanted, (name, number, 'native/oracle')
                    scene = bytearray(b'\xA5' * 124)
                    struct.pack_into('<I', scene, 0, GPU)
                    write(0x1F800000, scene)
                    work = bytearray(b'\xA5' * 96)
                    struct.pack_into('<I', work, 0, VERTICES + ORIGIN_INDEX * 8)
                    write(WORK, work)
                    commands = struct.pack('<II', lo0 | ((hi1 & 65535) << 16),
                                           lo2 | ((hi3 & 65535) << 16)) + b'\xA5' * 24
                    write(STREAM, commands)
                    write(mask_address, bytes(4))
                    write(frame_pointer, struct.pack('<I', FRAME))
                    frame = b'\xA6' * 16 + bytes(6) + struct.pack('<H', 256) + b'\x6A' * 16
                    write(FRAME - 16, frame)
                    write(STACK - 256, b'\xA6' * 512)
                    cpu.reset()
                    cpu.r = [0] + [0xBEEF0000 + reg for reg in range(1, 32)]
                    cpu.r[4:7] = [STREAM, WORK, count & 0xFFFFFFFF]
                    cpu.r[29], cpu.r[31], cpu.pc = STACK, STOP, entry
                    for _ in range(150):
                        if cpu.pc == STOP:
                            break
                        cpu.step()
                    assert cpu.pc == STOP and cpu.r[2] == STREAM
                    work[40:72] = wanted
                    assert read(WORK, 96) == work and read(0x1F800000, 124) == scene
                    assert read(STREAM, 32) == commands
                    assert read(FRAME - 16, len(frame)) == frame
                    assert read(frame_pointer, 4) == struct.pack('<I', FRAME)
                    assert read(mask_address, 4) == bytes(4)
                    assert read(STACK - 256, 192) == b'\xA6' * 192
                    assert read(STACK, 256) == b'\xA6' * 256
                    for reg in (*range(16, 24), 28, 29, 30):
                        assert cpu.r[reg] == (STACK if reg == 29 else 0xBEEF0000 + reg)
                    seen.add((hi1 < 0, hi3 < 0))
                assert seen == {(False, False), (False, True), (True, False), (True, True)}
                assert read(VERTICES, len(table)) == table
                assert read(VERTICES - 16, 16) == b'\xA6' * 16
                assert read(VERTICES + len(table), 16) == b'\x6A' * 16
                print(f'PASS {name} {version}: {len(inputs)} boundary prefetch cases; '
                      'signed upper/unsigned lower indices, native owners, guards and ABI', flush=True)
        native.finish_geometry()


if __name__ == '__main__':
    main()
