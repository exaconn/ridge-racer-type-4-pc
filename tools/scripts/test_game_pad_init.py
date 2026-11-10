"""Actual pad initialization: complete typed banks and original MIPS calls."""
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
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
STACK, STOP, SIZE = 0x803F0000, 0x803FF000, 376
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "game/system/pad.h"
#include "game/car/carstat.h"
#include "psyq/libpad/padgate.h"
#include "psyq/libpad/padctl.h"
#include "rr/bios_devices.h"
typedef struct {u32 before[4];ControllerState5C values[2];u32 after[4];} ControllerOwner;
typedef struct {u32 before[4];PadPulseState18 pulses[2];PadPort28 ports[2];u32 after[4];} DriverOwner;
typedef struct {ControllerOwner controllers;DriverOwner driver;} World;
static World live;
typedef uintptr_t (*Callback)(unsigned,const uintptr_t *);
static Callback callback;
void set_callback(Callback fn){callback=fn;}
#define PlayerControllers (live.controllers.values)
#define PadPulseState (live.driver.pulses)
#define PadPorts (live.driver.ports)
void PsyqBzero(void *p,s32 count){uintptr_t args[2]={(uintptr_t)p,(u32)count};callback(0,args);}
void PadInitDirect(u8 *a,u8 *b){uintptr_t args[2]={(uintptr_t)a,(uintptr_t)b};callback(1,args);}
void PadStartCom(void){uintptr_t args[1]={0};callback(2,args);}
void *region(unsigned n){assert(n<2);return n?(void *)&live.driver:(void *)&live.controllers;}
unsigned region_size(unsigned n){return n?sizeof(DriverOwner):sizeof(ControllerOwner);}
void load_world(const void *p){memcpy(&live,p,sizeof(live));}
void save_world(void *p){memcpy(p,&live,sizeof(live));}
'''
TAIL = r'''
void invoke(void){
 _Static_assert(sizeof(ControllerState5C)==92&&sizeof(PadPulseState18)==24&&sizeof(PadPort28)==40,"complete pad banks");
 _Static_assert(sizeof(World)==376&&sizeof(ControllerOwner)==216&&sizeof(DriverOwner)==160,"wire owners with guards");
 _Static_assert(offsetof(DriverOwner,pulses)==16&&offsetof(DriverOwner,ports)==64,"adjacent live driver banks");
 assert((uintptr_t)PlayerControllers>UINT32_MAX&&(uintptr_t)PadPulseState>UINT32_MAX&&(uintptr_t)PadPorts>UINT32_MAX);
 InitGamePads();
}
'''


class GamePadInitTest(unittest.TestCase):
    def test_complete_banks_and_native_transport_pointers(self):
        owner = source_owners(ROOT/'src/main/PAL')['InitGamePads']
        body = definition_source(owner.read_text(), 'InitGamePads')
        with TemporaryDirectory() as temporary:
            source, library = Path(temporary)/'native.c', Path(temporary)/'native.dylib'
            source.write_text(PREFIX+body+TAIL)
            subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                            '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
                            '-I', str(ROOT/'include'), str(source), '-o', str(library)], check=True)
            lib = C.CDLL(str(library));lib.region.restype = C.c_void_p
            lib.load_world.argtypes = [C.c_void_p];lib.save_world.argtypes = [C.c_void_p]
            bases = [address_of('PlayerControllers')-16, address_of('PadPulseState')-16]
            self.assertEqual(address_of('PadPorts'), address_of('PadPulseState')+48)
            regions = [(lib.region(i), bases[i], lib.region_size(i)) for i in range(2)]
            self.assertTrue(all(n>0xFFFFFFFF for n, _, _ in regions))
            self.assertEqual(sum(n for _, _, n in regions), SIZE)
            memory = Uc(UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN | UC_MODE_MIPS32);memory.mem_map(0, 0x400000)
            read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
            write = lambda a, data: memory.mem_write(a & 0x1FFFFFFF, bytes(data))
            original = (ROOT/'assets/PAL/main.exe').read_bytes();base = struct.unpack_from('<I', original, 24)[0]
            write(base, original[0x800:])
            if os.getenv('RR_COMPILED_ELF'):
                with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                    elf = ELFFile(stream);symbol = elf.get_section_by_name('.symtab').get_symbol_by_name('InitGamePads')[0]
                    section = elf.get_section(symbol['st_shndx']);offset = symbol['st_value']-section['sh_addr']
                    self.assertEqual(symbol['st_value'], address_of('InitGamePads'))
                    write(symbol['st_value'], section.data()[offset:offset+symbol['st_size']])
            scratch = (C.c_ubyte*SIZE)()

            def world(is_native):
                if is_native:lib.save_world(scratch);return bytes(scratch)
                return b''.join(read(a, n) for _, a, n in regions)

            def set_world(data, is_native):
                if is_native:lib.load_world(C.create_string_buffer(data));return
                offset = 0
                for _, a, n in regions:write(a, data[offset:offset+n]);offset += n

            def pal(pointer):
                for native, address, size in regions:
                    if native <= pointer < native+size:return address+pointer-native
                raise AssertionError(('unowned native transport pointer', hex(pointer)))

            def store(cpu, access, address, size, value, user):
                a = address & 0x1FFFFFFF
                allowed = [(b & 0x1FFFFFFF, n) for _, b, n in regions]+[(STACK-128 & 0x1FFFFFFF, 128)]
                self.assertTrue(any(b <= a and a+size <= b+n for b, n in allowed), (hex(address), size))

            cpu = Cpu(memory, 0, 100, 101, 102, store)
            services = {address_of(n): i for i, n in enumerate(['PsyqBzero', 'PadInitDirect', 'PadStartCom'])}
            traces = [[], []];number = 0;errors = []

            def service(op, args, is_native):
                side = int(is_native);serial = len(traces[side]);self.assertLess(serial, 4)
                self.assertEqual(op, [0, 0, 1, 2][serial])
                values = [pal(v) if is_native and (op == 1 or op == 0 and j == 0) else v & 0xFFFFFFFF for j, v in enumerate(args)]
                entry = world(is_native);data = bytearray(entry)
                if op == 0:
                    expected = (address_of('PlayerControllers'), 184) if serial == 0 else (address_of('PadPulseState'), 48)
                    self.assertEqual(values, list(expected))
                    offset = 16 if serial == 0 else 216+16;data[offset:offset+expected[1]] = bytes(expected[1])
                elif op == 1:
                    self.assertEqual(values, [address_of('PadPorts'), address_of('PadPorts')+40])
                    for offset in [216+64, 216+104]:data[offset:offset+2] = b'\xFF\0'
                if number & 1:
                    # Complete owner mutations retain all guard words. The
                    # initializer must restore both protocol words after clear.
                    for j in range(2):
                        at = 16+j*92
                        struct.pack_into('<I', data, at, (number*0x17171717+serial+j) & 0xFFFFFFFF)
                        struct.pack_into('<I', data, at+16, (number*0x9E3779B9+serial*17+j) & 0xFFFFFFFF)
                    for j in range(128):data[216+16+j] ^= (number+serial*31+j*13) & 255
                set_world(bytes(data), is_native)
                traces[side].append((op, values, entry, world(is_native)))
                return 0x13579BDF

            CALLBACK = C.CFUNCTYPE(C.c_size_t, C.c_uint, C.POINTER(C.c_size_t))
            def callback(op, args):
                try:return service(op, list(args[:2 if op != 2 else 0]), True)
                except BaseException as error:errors.append(error);return 0
            callback_ref = CALLBACK(callback);lib.set_callback(callback_ref)
            rng = random.Random(0x4C9F8)
            for number in range(4096):
                data = rng.randbytes(SIZE);traces = [[], []];set_world(data, False)
                write(STACK-256, b'\xA5'*512);entry_stack = read(STACK, 256)
                cpu.reset();cpu.r = [0]+[0xBEEF0000+i for i in range(1, 32)]
                cpu.r[29], cpu.r[31], cpu.pc = STACK, STOP, address_of('InitGamePads')
                for instruction in range(500):
                    if cpu.pc == STOP:break
                    if cpu.pc not in services:cpu.step();continue
                    op = services[cpu.pc];result = service(op, cpu.r[4:6] if op != 2 else [], False);resume = cpu.r[31]
                    for register in (1, 2, 3, *range(4, 16), 24, 25):cpu.r[register] = 0xA1230000+len(traces[0])*257+register
                    cpu.r[2], cpu.pc, cpu.hi, cpu.lo = result, resume, 0x12345678, 0x87654321
                self.assertEqual(cpu.pc, STOP, number);self.assertEqual(cpu.r[29], STACK);self.assertEqual(cpu.r[31], STOP)
                for register in (*range(16, 24), 28, 30):self.assertEqual(cpu.r[register], 0xBEEF0000+register)
                self.assertEqual(read(STACK-256, 128), b'\xA5'*128);self.assertEqual(read(STACK, 256), entry_stack)
                expected = world(False);set_world(data, True);lib.invoke()
                if errors:raise errors[0]
                self.assertEqual(len(traces[0]), 4);self.assertEqual(traces[1], traces[0], number);self.assertEqual(world(True), expected, number)
            print('PASS 4096 native versus', 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
                  'MIPS pad initialization cases; complete controller/pulse/port banks, full pointers, mutable services, exact cleared ranges and ABI guards under UBSan')
