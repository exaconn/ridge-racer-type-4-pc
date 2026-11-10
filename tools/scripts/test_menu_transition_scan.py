"""Actual menu-transition scan with full native row roots and original MIPS.

The independent oracle checks both readiness flags and the MIPS early-exit
read sequence. Complete native owners and PAL storage must remain unchanged.
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
from mips_preview_engine import signed
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
BADGES, IMAGES = 0x80200010, 0x80210010
STACK, STOP = 0x803F0000, 0x803FF000
CASES, STATE = 8192 + 256, 400
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "game/gfx/gfxmtx.h"
#include "game/ui/uitext.h"
#include "rr/badge_rows.h"
static struct World {
 u8 before[16];BadgeRow1C badges[8];ImageRow20 images[8];
 s32 badgeCount,imageCount;BadgeRow1C *badgeRoot;ImageRow20 *imageRoot;
 u8 after[16];
} world,expected;
#define BadgeRows world.badgeRoot
#define ImageRows world.imageRoot
#define BadgeRowCount world.badgeCount
#define ImageRowCount world.imageCount
static u32 get(const u8 *p){u32 value;memcpy(&value,p,4);return value;}
'''
TAIL = r'''
s32 run_case(s32 flags,const u8 *input){
 assert(sizeof(void*)==8&&(uintptr_t)&world>UINT32_MAX);
 memset(&world,0xA5,sizeof(world));memcpy(world.badges,input,224);memcpy(world.images,input+224,160);
 memcpy(&world.badgeCount,input+384,4);memcpy(&world.imageCount,input+388,4);
 u32 b=get(input+392),i=get(input+396);
 if(b){assert(b>=0x80200010U&&(b-0x80200010U)%28==0&&(b-0x80200010U)/28<8);world.badgeRoot=world.badges+(b-0x80200010U)/28;}else world.badgeRoot=0;
 if(i){assert(i>=0x80210010U&&(i-0x80210010U)%20==0&&(i-0x80210010U)/20<8);world.imageRoot=world.images+(i-0x80210010U)/20;}else world.imageRoot=0;
 assert(!world.badgeRoot||(uintptr_t)world.badgeRoot>UINT32_MAX);
 assert(!world.imageRoot||(uintptr_t)world.imageRoot>UINT32_MAX);
 memcpy(&expected,&world,sizeof(world));s32 result=StepMenuCameraTransition(flags);
 assert(!memcmp(&world,&expected,sizeof(world)));return result;
}
'''


def oracle(flags, state):
    result, reads = 3, []
    bc, ic, br, ir = struct.unpack_from('<iiII', state, 384)
    for flag, count, root, start, stride, offset, width, pending in (
        (1, bc, br, 0, 28, 16, 2, 11),
        (2, ic, ir, 224, 20, 1, 1, 5),
    ):
        if not flags & flag:
            continue
        base = BADGES if flag == 1 else IMAGES
        for index in range(max(count, 0)):
            at = start + root - base + index * stride + offset
            bits = int.from_bytes(state[at:at + width], 'little')
            reads.append((root + index * stride + offset, width, bits))
            if bits not in (0, pending):
                result &= 3 ^ flag
                break
    return result, reads


def main():
    os.chdir(ROOT)
    name = 'StepMenuCameraTransition'
    owner = source_owners(ROOT / 'src/main/PAL')[name]
    body = definition_source(owner.read_text(), name)
    with TemporaryDirectory() as directory:
        source, library = Path(directory) / 'native.c', Path(directory) / 'native.dylib'
        source.write_text(PREFIX + body + TAIL)
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-Iinclude',
                        str(source), '-o', str(library)], check=True)
        native = C.CDLL(str(library))
        native.run_case.argtypes = [C.c_int32, C.c_void_p]
        native.run_case.restype = C.c_int32
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0, 0x400000)
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
        bc, ic, br, ir = [address_of(n) for n in ('BadgeRowCount', 'ImageRowCount', 'BadgeRows', 'ImageRows')]
        reads = []

        class ScanCpu(Cpu):
            def load(self, address, width):
                value = super().load(address, width)
                address = (address & 0x1FFFFFFF) | 0x80000000
                if BADGES - 16 <= address < BADGES + 224 + 16 or IMAGES - 16 <= address < IMAGES + 160 + 16:
                    reads.append((address, width, value))
                return value

        def check_store(cpu, access, address, width, value, user):
            address &= 0x1FFFFFFF
            assert STACK - 256 <= (address | 0x80000000) < STACK, (hex(address), width)

        cpu = ScanCpu(memory, 0, 100, 101, 102, check_store)
        counts, starts, flags_seen, outputs, image_states, observed_image_states, observed_badge_states = (set() for _ in range(7))
        for case in range(CASES):
            state = bytearray((case * 19 + i * 29) & 255 for i in range(STATE))
            badge_start, image_start = (case >> 4) & 7, (case >> 7) & 7
            bcounts = (-2147483648, -1, *range(9 - badge_start))
            icounts = (-2147483648, -1, *range(9 - image_start))
            badge_count, image_count = bcounts[(case >> 2) % len(bcounts)], icounts[(case >> 3) % len(icounts)]
            if case >= 8192:
                badge_count, image_count = 8 - badge_start, 8 - image_start
            for i in range(8):
                struct.pack_into('<h', state, i * 28 + 16, 11 if (case + i) & 1 else 0)
                state[224 + i * 20 + 1] = 5 if (case + i) & 1 else 0
            if badge_count > 0 and case & 1024:
                value = (-32768, -1, 1, 10, 12, 32767)[(case >> 5) % 6]
                at = badge_start + (case >> 8) % badge_count
                struct.pack_into('<h', state, at * 28 + 16, value)
            if image_count > 0 and (case & 2048 or case >= 8192):
                # The final 256 cases independently sweep values through an
                # active scan, with only pending/zero states before the value.
                value = (case >> 3) & 255 if case < 8192 else case - 8192
                at = image_start + (case >> 5) % image_count
                state[224 + at * 20 + 1] = value
                image_states.add(signed(value, 8))
            badge_root = BADGES + badge_start * 28 if badge_count > 0 or case & 4096 else 0
            image_root = IMAGES + image_start * 20 if image_count > 0 or case & 4096 else 0
            struct.pack_into('<iiII', state, 384, badge_count, image_count, badge_root, image_root)
            flags = ((case * 0x31415927) & 0xFFFFFFFC) | ((case & 3) if case < 8192 else 3)
            expected, expected_reads = oracle(flags, state)
            for a, width, value in expected_reads:
                if IMAGES <= a < IMAGES + 160:
                    observed_image_states.add(signed(value, 8))
                else:
                    observed_badge_states.add(signed(value, 16))
            actual = native.run_case(signed(flags, 32), C.create_string_buffer(bytes(state)))
            assert actual == expected, (case, 'native/oracle readiness; complete owners, full roots, guards')
            write(BADGES - 16, bytes([0xA5] * 16) + state[:224] + bytes([0x5A] * 16))
            write(IMAGES - 16, bytes([0xA5] * 16) + state[224:384] + bytes([0x5A] * 16))
            for address, value in ((bc, badge_count), (ic, image_count), (br, badge_root), (ir, image_root)):
                write(address, struct.pack('<I', value & 0xFFFFFFFF))
            cpu.reset(); cpu.r = [0] + [0xBEEF0000 + r for r in range(1, 32)]
            cpu.r[4], cpu.r[29], cpu.r[31], cpu.pc = flags, STACK, STOP, address_of(name)
            reads.clear()
            for _ in range(1000):
                if cpu.pc == STOP:
                    break
                cpu.step()
            assert cpu.pc == STOP and cpu.r[2] == expected and reads == expected_reads, (case, reads, expected_reads)
            snapshot = read(BADGES, 224) + read(IMAGES, 160) + b''.join(read(a, 4) for a in (bc, ic, br, ir))
            assert snapshot == state, (case, 'MIPS scan modified row owners/counts/roots')
            assert read(BADGES - 16, 16) == read(IMAGES - 16, 16) == bytes([0xA5] * 16)
            assert read(BADGES + 224, 16) == read(IMAGES + 160, 16) == bytes([0x5A] * 16)
            for r in (*range(16, 24), 28, 30):
                assert cpu.r[r] == 0xBEEF0000 + r
            assert cpu.r[29] == STACK and cpu.r[31] == STOP
            counts.update((badge_count, image_count)); starts.update((badge_start, image_start))
            flags_seen.add(flags & 3); outputs.add(expected)
        assert counts == {-2147483648, -1, *range(9)} and starts == set(range(8))
        assert flags_seen == set(range(4)) and outputs == set(range(4))
        assert image_states == observed_image_states == set(range(-128, 128)), (len(image_states), len(observed_image_states))
        assert observed_badge_states == {-32768, -1, 0, 1, 10, 11, 12, 32767}, observed_badge_states
        print('PASS', CASES, 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original', name,
              'MIPS/native cases; actual row owners/full roots, flags, signed states/counts, '
              'all image-state bytes, exact MIPS early-exit reads, unchanged storage/guards, ABI, UBSan', flush=True)


if __name__ == '__main__':
    main()
