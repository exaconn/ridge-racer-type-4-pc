"""Track target: actual C versus original/compiled MIPS, with mutable services."""
import ctypes as C
import os
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32
from c_functions import address_of, definition_source, source_owners
from test_scene_camera_tracks import CameraCpu as Cpu

ROOT = next(p for p in Path(__file__).resolve().parents if (p/'tools/scripts/cc.sh').is_file())
CAR, STACK, STOP = 0x80180010, 0x803F0000, 0x803FF000
SIZE = 16+800+16+4
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "vehicle_wire.h"
#include "game/car/carphys2.h"
#include "game/course/trknode.h"
#include "game/course/trkintp.h"
#include "rr/trig.h"
#include "game/math/rotmat.h"
static struct {u32 before[4];RaceVehicle car;u32 after[4];} owner;
s32 TrackNodeCount;
typedef uintptr_t (*Callback)(unsigned,const uintptr_t *);
static Callback callback;
void set_callback(Callback fn){callback=fn;}
void InterpTrackPosition(int index,int *position,int fraction){uintptr_t args[3]={(u32)index,(uintptr_t)position,(u32)fraction};callback(0,args);}
s32 GetSmoothedTrackHeading(int index,int fraction){uintptr_t args[2]={(u32)index,(u32)fraction};return (s32)callback(1,args);}
s32 rcos(s32 angle){uintptr_t args[1]={(u32)angle};return (s32)callback(2,args);}
s32 rsin(s32 angle){uintptr_t args[1]={(u32)angle};return (s32)callback(3,args);}
s32 Atan2Table(s32 x,s32 z){uintptr_t args[2]={(u32)x,(u32)z};return (s32)callback(4,args);}
void load_world(const u8 *data){
 memcpy(owner.before,data,16);rr_vehicle_wire_load(&owner.car,data+16);
 memcpy(owner.after,data+816,16);memcpy(&TrackNodeCount,data+832,4);
}
void save_world(u8 *data){
 rr_vehicle_wire_padding_value(&owner.car,0xA5);
 memcpy(data,owner.before,16);rr_vehicle_wire_store_fields(data+16,&owner.car);
 memcpy(data+816,owner.after,16);memcpy(data+832,&TrackNodeCount,4);
}
'''
TAIL = r'''
u32 invoke(void){assert((uintptr_t)&owner.car>UINT32_MAX);return (u32)GetTrackTargetHeading(&owner.car);}
'''


class TrackTargetNativeTest(unittest.TestCase):
    def test_full_vehicle_and_mips_services(self):
        owner = source_owners(ROOT/'src/main/PAL')['GetTrackTargetHeading']
        body = definition_source(owner.read_text(), 'GetTrackTargetHeading')
        with TemporaryDirectory() as temporary:
            source, library = Path(temporary)/'native.c', Path(temporary)/'native.dylib'
            source.write_text(PREFIX+body+TAIL)
            subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                            '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
                            '-I', str(ROOT/'include'), '-I', str(ROOT/'tools/scripts'), str(source), '-o', str(library)], check=True)
            native = C.CDLL(str(library));native.invoke.restype = C.c_uint32
            native.load_world.argtypes = [C.c_void_p];native.save_world.argtypes = [C.c_void_p]
            memory = Uc(UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN | UC_MODE_MIPS32);memory.mem_map(0, 0x400000)
            read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
            write = lambda a, data: memory.mem_write(a & 0x1FFFFFFF, bytes(data))
            original = (ROOT/'assets/PAL/main.exe').read_bytes();base = struct.unpack_from('<I', original, 24)[0]
            write(base, original[0x800:])
            if os.getenv('RR_COMPILED_ELF'):
                with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                    elf = ELFFile(stream);symbol = elf.get_section_by_name('.symtab').get_symbol_by_name('GetTrackTargetHeading')[0]
                    section = elf.get_section(symbol['st_shndx']);offset = symbol['st_value']-section['sh_addr']
                    self.assertEqual(symbol['st_value'], address_of('GetTrackTargetHeading'))
                    write(symbol['st_value'], section.data()[offset:offset+symbol['st_size']])
            count_address = address_of('TrackNodeCount')
            regions = [(CAR-16, 832), (count_address, 4)]
            scratch = (C.c_ubyte*SIZE)()

            def world(is_native):
                if is_native:native.save_world(scratch);return bytes(scratch)
                return b''.join(read(a, n) for a, n in regions)

            def set_world(data, is_native):
                if is_native:native.load_world(C.create_string_buffer(data));return
                write(CAR-16, data[:832]);write(count_address, data[832:])

            def store(cpu, access, address, size, value, user):
                a = address & 0x1FFFFFFF
                self.assertTrue((STACK-128 & 0x1FFFFFFF) <= a and a+size <= (STACK & 0x1FFFFFFF), (hex(address), size))

            cpu = Cpu(memory, 0, 100, 101, 102, store)
            counts = [3, 2, 1, 1, 2]
            names = ['InterpTrackPosition', 'GetSmoothedTrackHeading', 'rcos', 'rsin', 'Atan2Table']
            services = {address_of(name): i for i, name in enumerate(names)}
            traces = [[], []];point = [0, 0];errors = [];number = 0
            edges = [0x80000000, 0x80000001, 0xFFFF0000, 0xFFFF8000, 0xFFFFFFFF, 0, 1, 2, 0x7FFF, 0x10000, 0x7FFFFFFF]
            divisors = [1, 2, 3, 17, 32767, 0x7FFFFFFF]
            directions = [-32768, -1, 0, 1, 32767]
            seen = [set() for _ in range(5)]

            def service(op, args, is_native):
                side = int(is_native);serial = len(traces[side]);self.assertEqual(op, serial)
                seen[op].add(number & 1)
                if op == 0:
                    point[side] = args[1]
                    if is_native:self.assertGreater(args[1], 0xFFFFFFFF)
                    else:self.assertTrue(STACK-128 <= args[1] <= STACK-16)
                def vector():
                    return C.string_at(point[side], 16) if is_native else read(point[side], 16)
                entry = world(is_native);before = b'' if op == 0 else vector()
                normalized = list(args)
                if op == 0:normalized[1] = 0
                data = bytearray(entry)
                if number & 1:
                    for j, offset in enumerate([0x20, 0x28, 0xF0, 0xF4, 0x170]):
                        struct.pack_into('<I', data, 16+offset, edges[(number+serial*3+j)%len(edges)])
                    struct.pack_into('<h', data, 16+0x1EA, directions[(number+serial)%5])
                    struct.pack_into('<I', data, 832, divisors[(number+serial)%6])
                    set_world(bytes(data), is_native)
                if op == 0 or number & 1:
                    payload = struct.pack('<4I', *(edges[(number+serial*7+j*3)%len(edges)] for j in range(4)))
                    if is_native:C.memmove(point[side], payload, 16)
                    else:write(point[side], payload)
                result = edges[(number//3+op*7)%len(edges)]
                traces[side].append((op, normalized, entry, before, world(is_native), vector(), result))
                return result

            CALLBACK = C.CFUNCTYPE(C.c_size_t, C.c_uint, C.POINTER(C.c_size_t))
            def callback(op, args):
                try:return service(op, list(args[:counts[op]]), True)
                except BaseException as error:errors.append(error);return 0
            callback_ref = CALLBACK(callback);native.set_callback(callback_ref)
            randomizer = random.Random(0x27C44)
            for number in range(8192):
                data = bytearray(randomizer.randbytes(SIZE))
                struct.pack_into('<II', data, 16+0x20C, 0, 0)
                for j, offset in enumerate([0x20, 0x28, 0xF0, 0xF4, 0x170]):
                    struct.pack_into('<I', data, 16+offset, edges[(number//(j+1)+j)%len(edges)] if number<4096 else randomizer.getrandbits(32))
                struct.pack_into('<h', data, 16+0x1EA, directions[number%5]);struct.pack_into('<I', data, 832, divisors[number//5%6])
                traces = [[], []];point = [0, 0];set_world(bytes(data), False)
                write(STACK-256, b'\xA5'*512);entry_stack = read(STACK, 256)
                cpu.reset();cpu.r = [0]+[0xBEEF0000+i for i in range(1, 32)]
                cpu.r[4] = CAR;cpu.r[29], cpu.r[31], cpu.pc = STACK, STOP, address_of('GetTrackTargetHeading')
                for instruction in range(2000):
                    if cpu.pc == STOP:break
                    if cpu.pc not in services:cpu.step();continue
                    op = services[cpu.pc];args = cpu.r[4:4+counts[op]];result = service(op, args, False);resume = cpu.r[31]
                    for register in (1, 2, 3, *range(4, 16), 24, 25):cpu.r[register] = 0xA1230000+op*257+register
                    cpu.r[2], cpu.pc, cpu.hi, cpu.lo = result, resume, 0x12345678, 0x87654321
                self.assertEqual(cpu.pc, STOP, number);self.assertEqual(cpu.r[29], STACK);self.assertEqual(cpu.r[31], STOP)
                for register in (*range(16, 24), 28, 30):self.assertEqual(cpu.r[register], 0xBEEF0000+register)
                self.assertEqual(read(STACK-256, 128), b'\xA5'*128);self.assertEqual(read(STACK, 256), entry_stack)
                expected_world, expected_result = world(False), cpu.r[2]
                set_world(bytes(data), True);result = native.invoke()
                if errors:raise errors[0]
                self.assertEqual(traces[1], traces[0], number);self.assertEqual(len(traces[0]), 5)
                self.assertEqual(result, expected_result, number);self.assertEqual(world(True), expected_world, number)
            self.assertTrue(all(branches == {0, 1} for branches in seen))
            print('PASS 8192 native versus', 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
                  'MIPS track-target cases; complete vehicle, wrapping arithmetic, mutable services, full native pointer, stack/store guards and UBSan')
