"""Actual sequence initialization/playback on complete native owners and MIPS.

The oracle executes both original and linked instructions. Native row pointers,
stream pointers and output aliases map through their concrete owners and fields.
All public layout assertions are enabled. Hardware/voice callbacks are observed
at complete-state boundaries and can replace live roots and stream cursors.
"""
import ctypes as C
from pathlib import Path
import random
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from c_functions import address_of, definition_source, source_owners
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
ROWS, STREAMS = 0x80200010, 0x80210010
COUNT, STRIDE, BUFFER = 9, 176, 64
STACK, STOP = 0x803F0000, 0x803FF000
TABLE_BIAS = 0x4000 - 0xF98 // 4
FUNCTIONS = ('SsSetTableSize', '_SsReadDeltaValue', '_SsInitSoundSeq',
             '_SsVmGetSeqVol', 'SsSeqGetVol', 'Snd_SetPlayMode',
             '_SsSndSetPauseMode', '_SsSndSetReplayMode', '_SsSndPause',
             '_SsSndReplay', '_SsSndStop', '_SsSndNextSep', '_SsSeqGetEof',
             '_SsSeqPlay', '_SsSndTempo')
SCALARS = (('_snd_openflag', 4), ('_snd_seq_s_max', 2),
           ('_snd_seq_t_max', 2), ('VBLANK_MINUS', 4), ('_svm_cur_vab_id', 2))
SERVICES = (('_SsVmSeqKeyOff', 1), ('_SsVmDamperOff', 0),
            ('_SsVmSetSeqVol', 4), ('_SsGetSeqData', 2), ('DebugPrintStub', 1))
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "rr/sequence_channel.h"
#include "rr/sequencer_init.h"
#include "rr/channel_stream.h"
#include "psyq/libsnd/sspause.h"
#include "psyq/libsnd/sspause2.h"
#include "psyq/libsnd/ssresume.h"
#include "psyq/libsnd/sstick3.h"
#include "psyq/libsnd/vmkoff.h"
#include "psyq/libsnd/vmchval.h"
#include "psyq/libsnd/ssvolset.h"
#include "psyq/libsnd/ssatr2.h"
static struct {u8 before[16];SequenceChannel values[9];u8 after[16];} records;
static struct {u8 before[16];u8 values[9][64];u8 after[16];} streams;
SequenceChannel *native_table[0x4100];
#define _ss_score (native_table+0x4000-0xF98/4)
u32 _snd_openflag,VBLANK_MINUS;
s16 _snd_seq_s_max,_snd_seq_t_max,_svm_cur_vab_id;
char StrNotSeqData[1],SeqOldFormatMsg[1];
typedef u32 (*Observer)(u32,const uintptr_t *);
static Observer observer;
void set_observer(Observer value){observer=value;}
static void notify(unsigned kind,u32 a,u32 b,u32 c,u32 d){
 uintptr_t arguments[4]={a,b,c,d};observer(kind,arguments);
}
void _SsVmSeqKeyOff(s32 input){notify(0,(u32)input,0,0,0);}
void _SsVmDamperOff(void){notify(1,0,0,0,0);}
s16 _SsVmSetSeqVol(s32 packed,s32 left,s32 right,s32 mode){notify(2,packed,left,right,mode);return(s16)packed;}
void _SsGetSeqData(s32 bank,s32 row){notify(3,bank,row,0,0);}
void DebugPrintStub(const char *message,...){notify(4,message==StrNotSeqData,0,0,0);}
static void *native_pointer(u32 address){
 if(!address)return 0;
 if(address>=0x80210010U && address<0x80210010U+sizeof(streams.values))
  return (u8 *)&streams.values+(address-0x80210010U);
 if(address>=0x80200010U && address<0x80200010U+9*176){
  unsigned offset=address-0x80200010U,index=offset/176,field=offset%176;
  if(!field)return &records.values[index];
  assert(field>=20);return (u8 *)&records.values[index]+offsetof(SequenceChannel,b14)+field-20;
 }
 assert(!"unowned wire pointer");return 0;
}
static u32 pal_pointer(const void *pointer){
 uintptr_t value=(uintptr_t)pointer,first=(uintptr_t)&streams.values;
 if(!pointer)return 0;
 if(value>=first&&value-first<sizeof(streams.values))return 0x80210010U+(u32)(value-first);
 first=(uintptr_t)&records.values;
 if(value>=first&&value-first<sizeof(records.values)){
  size_t offset=value-first,index=offset/sizeof(SequenceChannel),field=offset%sizeof(SequenceChannel);
  if(!field)return 0x80200010U+(u32)index*176;
  assert(field>=offsetof(SequenceChannel,b14)&&field<offsetof(SequenceChannel,wac)+4);
  return 0x80200010U+(u32)index*176+20+(u32)(field-offtorr);
 }
 assert(!"unowned native pointer");return 0;
}
void *resolve_native(u32 address){return native_pointer(address);}
void reset_native(const u8 *input,const u8 *data){
 unsigned i,j;memset(&records,0xA7,sizeof(records));memset(&streams,0xA7,sizeof(streams));
 memset(native_table,0,sizeof(native_table));memcpy(streams.values,data,sizeof(streams.values));
 assert((uintptr_t)&records>UINT32_MAX&&(uintptr_t)&streams>UINT32_MAX);
 for(i=0;i<9;i++){
  SequenceChannel *r=&records.values[i];u8 **fields[5]={&r->c0,&r->c4,&r->c8,&r->cC,&r->p10};
  for(j=0;j<5;j++){u32 value;memcpy(&value,input+i*176+j*4,4);*fields[j]=native_pointer(value);}
  memcpy((u8 *)r+offsetof(SequenceChannel,b14),input+i*176+20,156);
 }
}
void guard_native(void){unsigned i,j;
 for(i=0;i<16;i++)assert(records.before[i]==0xA7&&records.after[i]==0xA7&&streams.before[i]==0xA7&&streams.after[i]==0xA7);
 for(i=0;i<9;i++)for(j=offsetof(SequenceChannel,wac)+4;j<sizeof(SequenceChannel);j++)
  assert(((u8 *)&records.values[i])[j]==0xA7);
 for(i=0;i<0x4100;i++)if(i<0x4000-0xF98/4||i>=0x4000-0xF98/4+32)assert(!native_table[i]);
}
void snapshot_native(u8 *output){unsigned i,j;u32 value;
 guard_native();
 for(i=0;i<9;i++){
  SequenceChannel *r=&records.values[i];u8 *fields[5]={r->c0,r->c4,r->c8,r->cC,r->p10};
  for(j=0;j<5;j++){value=pal_pointer(fields[j]);memcpy(output+i*176+j*4,&value,4);}
  memcpy(output+i*176+20,(u8 *)r+offsetof(SequenceChannel,b14),156);
 }
 output+=9*176;memcpy(output,streams.values,sizeof(streams.values));output+=sizeof(streams.values);
 for(i=0;i<32;i++){value=pal_pointer(_ss_score[i]);memcpy(output+i*4,&value,4);}
}
void set_root(unsigned bank,u32 address){assert(bank<32);_ss_score[bank]=native_pointer(address);}
void set_row_pointer(unsigned index,unsigned field,u32 address){
 SequenceChannel *r=&records.values[index];u8 **fields[5]={&r->c0,&r->c4,&r->c8,&r->cC,&r->p10};
 assert(index<9&&field<5);*fields[field]=native_pointer(address);
}
_Static_assert(sizeof(SequenceChannel)==200,"complete LP64 row");
'''.replace('offtorr', 'offsetof(SequenceChannel,b14)')


class SequenceCpu(Cpu):
    """Add signed/unsigned division to the existing MIPS-I fixture engine."""
    def step(self):
        instruction = self.load(self.pc, 4)
        function = instruction & 63
        if instruction >> 26 or function not in (26, 27):
            return super().step()
        assert ((instruction >> 6) & 0x3FF) == 0
        left = self.r[(instruction >> 21) & 31]
        right = self.r[(instruction >> 16) & 31]
        if function == 26:
            left, right = C.c_int32(left).value, C.c_int32(right).value
        assert right and not (left == -2147483648 and right == -1)
        quotient = abs(left) // abs(right)
        if (left < 0) != (right < 0):
            quotient = -quotient
        self.lo = quotient & 0xFFFFFFFF
        self.hi = (left - quotient * right) & 0xFFFFFFFF
        self.pc, self.delay = (self.pc + 4 if self.delay is None else self.delay), None


class SequenceChannelPipelineTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temporary = TemporaryDirectory()
        cls.addClassCleanup(cls.temporary.cleanup)
        directory = Path(cls.temporary.name)
        owners = source_owners(ROOT / 'src/main/PAL')
        code = PREFIX
        code += definition_source(owners['_SsInitSoundSeq'].read_text(), 'read_tempo')
        for name in FUNCTIONS:
            body = definition_source(owners[name].read_text(), name)
            # Existing target register constraints have no host equivalent.
            body = re.sub(r'\s+(?:__asm__|asm)\("\$\d+"\)', '', body)
            body = re.sub(r'__asm__(?: volatile)?\(""[^;]*;', '', body)
            code += body
        path = directory / 'sequence.c'
        path.write_text(code)
        result = subprocess.run(['cc', '-std=gnu11', '-O2', '-shared', '-fPIC',
                                 '-Wall', '-Wextra', '-Werror',
                                 '-Wno-error=deprecated-non-prototype',
                                 '-Wno-error=unused-variable',
                                 '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
                                 '-I', str(ROOT / 'include'), str(path),
                                 '-o', str(directory / 'sequence.so')],
                                capture_output=True, text=True)
        if result.returncode:
            raise AssertionError(result.stderr)
        cls.library = C.CDLL(str(directory / 'sequence.so'))
        cls.library.resolve_native.argtypes = [C.c_uint32]
        cls.library.resolve_native.restype = C.c_void_p
        cls.library.reset_native.argtypes = [C.c_void_p, C.c_void_p]
        cls.library.snapshot_native.argtypes = [C.c_void_p]
        cls.library.set_row_pointer.argtypes = [C.c_uint, C.c_uint, C.c_uint32]
        cls.native_table = (C.c_void_p * 0x4100).in_dll(cls.library, 'native_table')
        cls.cells = {name: (C.c_uint8 * size).in_dll(cls.library, name)
                     for name, size in SCALARS}
        with (ROOT / 'build/PAL/main.elf').open('rb') as stream:
            elf = ELFFile(stream)
            cls.linked = [(s['sh_addr'], s.data()) for s in elf.iter_sections()
                          if s['sh_flags'] & 2 and s['sh_type'] == 'SHT_PROGBITS']
        executable = (ROOT / 'assets/PAL/main.exe').read_bytes()
        cls.original = (struct.unpack_from('<I', executable, 0x18)[0], executable[0x800:])
        cls.functions = {name: address_of(name) for name in FUNCTIONS}
        cls.services = {address_of(name): i for i, (name, _) in enumerate(SERVICES)}
        cls.memory_size = COUNT * STRIDE + COUNT * BUFFER + 32 * 4

    def read(self, address, size):
        return bytes(self.memory.mem_read(address & 0x1FFFFFFF, size))

    def write(self, address, data):
        self.memory.mem_write(address & 0x1FFFFFFF, bytes(data))

    def put(self, address, value, size=4):
        self.write(address, (value & ((1 << (size * 8)) - 1)).to_bytes(size, 'little'))

    def scalar(self, name, value, native):
        size = dict(SCALARS)[name]
        data = (value & ((1 << (size * 8)) - 1)).to_bytes(size, 'little')
        if native:
            C.memmove(C.addressof(self.cells[name]), data, size)
        else:
            self.write(address_of(name), data)

    def field(self, index, offset, value, size, native):
        address = ROWS + index * STRIDE + offset
        if native:
            C.memmove(self.library.resolve_native(address),
                      (value & ((1 << (size * 8)) - 1)).to_bytes(size, 'little'), size)
        else:
            self.put(address, value, size)

    def snapshot(self, native):
        if native:
            output = (C.c_uint8 * self.memory_size)()
            self.library.snapshot_native(output)
            return bytes(output), tuple(bytes(self.cells[n]) for n, _ in SCALARS)
        self.assertEqual(self.read(ROWS - 16, 16), b'\xA7' * 16)
        self.assertEqual(self.read(ROWS + COUNT * STRIDE, 16), b'\xA7' * 16)
        self.assertEqual(self.read(STREAMS - 16, 16), b'\xA7' * 16)
        self.assertEqual(self.read(STREAMS + COUNT * BUFFER, 16), b'\xA7' * 16)
        data = (self.read(ROWS, COUNT * STRIDE) + self.read(STREAMS, COUNT * BUFFER)
                + self.read(address_of('_ss_score'), 32 * 4))
        return data, tuple(self.read(address_of(n), size) for n, size in SCALARS)

    def service(self, kind, arguments, native):
        count = SERVICES[kind][1]
        args = tuple(int(v) & 0xFFFFFFFF for v in arguments[:count])
        if kind == 4 and not native:
            args = (int(args[0] == address_of('StrNotSeqData')),)
        self.events.append((kind, args, self.snapshot(native)))
        if kind in (0, 1) and self.case & 1:
            # Keep captured records and the current bank root observably distinct.
            address = ROWS + (6 if kind == 0 else 3) * STRIDE
            if native:
                self.library.set_root(0, address)
            else:
                self.put(address_of('_ss_score'), address)
            for index in (0, 3, 6):
                target = STREAMS + index * BUFFER + 23 + kind
                if native:
                    self.library.set_row_pointer(index, 1, target)
                else:
                    self.put(ROWS + index * STRIDE + 4, target)
        if kind == 3:
            self.assertLess(len(self.events), 100)
            for index in range(COUNT):
                self.field(index, 0x90, 1 + len(self.events) % 5, 4, native)
        return 0

    def initialize(self, native):
        randomizer = random.Random(0x534551 + self.case)
        rows = bytearray(randomizer.randbytes(COUNT * STRIDE))
        streams = bytearray(randomizer.randbytes(COUNT * BUFFER))
        for index in range(COUNT):
            for field in range(5):
                struct.pack_into('<I', rows, index * STRIDE + field * 4,
                                 STREAMS + index * BUFFER + 16 + field)
            rows[index * STRIDE + 0x17] = index % 16
            # A real valid SEQ header followed by a bounded VLQ delta.
            at = index * BUFFER
            streams[at:at + 19] = b'S\0\0\0\0\0\0\1\0\x78\x07\xA1\x20\4\2\x81\1\0\0'
            streams[at] = (ord('S'), ord('p'), ord('x'))[self.case % 3]
            streams[at + 7] = 2 if self.case % 7 == 0 else 1
            ppq = (1, 24, 120, 480, 32767)[(self.case // 3) % 5]
            tempo = (1, 120000, 500000, 0xFFFFFF)[(self.case // 5) % 4]
            streams[at + 8:at + 10] = ppq.to_bytes(2, 'big')
            streams[at + 10:at + 13] = tempo.to_bytes(3, 'big')
        if native:
            self.library.reset_native(C.create_string_buffer(bytes(rows)),
                                      C.create_string_buffer(bytes(streams)))
        else:
            self.write(ROWS - 16, b'\xA7' * 16 + rows + b'\xA7' * 16)
            self.write(STREAMS - 16, b'\xA7' * 16 + streams + b'\xA7' * 16)
            self.write(address_of('_ss_score'), bytes(32 * 4))
        for name, value in (('_snd_openflag', 0x11223344), ('_snd_seq_s_max', 0),
                            ('_snd_seq_t_max', 0), ('VBLANK_MINUS', 50 if self.case & 2 else 60),
                            ('_svm_cur_vab_id', -123)):
            self.scalar(name, value, native)

    def call(self, name, arguments, pointers, native, returns=False):
        if native:
            fn = getattr(self.library, name)
            fn.argtypes = [C.c_void_p if i in pointers else C.c_int32
                           for i in range(len(arguments))]
            fn.restype = C.c_int32 if returns else None
            values = [self.library.resolve_native(v) if i in pointers else C.c_int32(v).value
                      for i, v in enumerate(arguments)]
            result = fn(*values)
        else:
            self.cpu.reset()
            self.cpu.r = [0] + [0xBEEF0000 + i for i in range(1, 32)]
            self.cpu.r[4:4 + len(arguments)] = [v & 0xFFFFFFFF for v in arguments]
            self.cpu.r[29], self.cpu.r[31], self.cpu.pc = STACK, STOP, self.functions[name]
            for _ in range(30000):
                if self.cpu.pc == STOP:
                    break
                if self.cpu.pc in self.services:
                    self.assertIsNone(self.cpu.delay)
                    kind = self.services[self.cpu.pc]
                    self.service(kind, self.cpu.r[4:8], False)
                    self.cpu.pc = self.cpu.r[31]
                    for reg in (2, 3, *range(4, 16), 24, 25):
                        self.cpu.r[reg] = 0xBAD00000 + reg
                    self.cpu.r[2] = 0
                else:
                    self.cpu.step()
            self.assertEqual(self.cpu.pc, STOP, (name, 'did not return'))
            for reg in (*range(16, 24), 28, 29, 30):
                self.assertEqual(self.cpu.r[reg], STACK if reg == 29 else 0xBEEF0000 + reg)
            result = C.c_int32(self.cpu.r[2]).value if returns else None
        self.events.append((name, result, self.snapshot(native)))

    def pipeline(self, native, selector_prefix=0):
        self.events = []
        self.initialize(native)
        self.call('SsSetTableSize', (ROWS, 2, 3), (0,), native)
        for bank in (0, 1):
            self.call('_SsInitSoundSeq', (selector_prefix | bank, self.case * 13 - 128, STREAMS + bank * 3 * BUFFER),
                      (2,), native, True)
        self.call('SsSeqGetVol', (0, 0, ROWS + 0x5A, ROWS + 0x5C), (2, 3), native)
        self.call('Snd_SetPlayMode', (0, 0, self.case % 3 - 1, self.case - 20), (), native)
        self.call('_SsSndSetPauseMode', (0, 0), (), native)
        self.call('_SsSndSetReplayMode', (0, 0), (), native)
        self.call('_SsSndReplay', (0, 0), (), native)
        self.call('_SsSndPause', (0, 0), (), native)
        self.call('_SsSndStop', (0, 0), (), native)
        self.call('_SsSndNextSep', (0, 0), (), native)
        # Prepare bounded, identical state for completion, polling and tempo.
        root = 3 if self.case & 1 else 0
        for offset, value, size in ((0x20, (0, 1, 5, -128)[self.case % 4], 1),
                                    (0x21, 0, 1), (0x22, 1, 1), (0x23, 1, 1),
                                    (0x54, 8, 2)):
            self.field(root, offset, value, size, native)
        self.call('_SsSeqGetEof', (0, 0), (), native)
        # A callback may have changed the live root once more.
        if self.case & 1:
            root = 6
        for index in range(COUNT):
            self.field(index, 0x90, 0 if self.case & 4 else 10, 4, native)
            self.field(index, 0x54, 8, 2, native)
            self.field(index, 0x52, (-1, 0, 2)[self.case % 3], 2, native)
        self.call('_SsSeqPlay', (0, 0), (), native)
        for index in range(COUNT):
            for offset, value, size in ((0x4E, (1, -2, 0)[self.case % 3], 2),
                                        (0x50, 120, 2), (0x94, 120, 4),
                                        (0xA8, 2, 4), (0xAC, 121, 4)):
                self.field(index, offset, value, size, native)
        self.call('_SsSndTempo', (0, 0), (), native)
        return self.events

    def test_real_chain_full_pointers_aliases_and_live_roots(self):
        observer_type = C.CFUNCTYPE(C.c_uint32, C.c_uint32, C.POINTER(C.c_size_t))
        for version in ('original', 'linked'):
            self.memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
            self.memory.mem_map(0, 0x400000)
            self.write(*self.original)
            if version == 'linked':
                for address, data in self.linked:
                    self.write(address, data)
            def store(cpu, access, address, size, value, user):
                address &= 0x1FFFFFFF
                spans = [(ROWS - 16, COUNT * STRIDE + 32), (STREAMS - 16, COUNT * BUFFER + 32),
                         (address_of('_ss_score'), 128), (STACK - 1024, 1024)]
                spans += [(address_of(n), size) for n, size in SCALARS]
                self.assertTrue(any((a & 0x1FFFFFFF) <= address and
                                    address + size <= (a & 0x1FFFFFFF) + n for a, n in spans),
                                (hex(address), size))
            self.cpu = SequenceCpu(self.memory, 0, 100, 101, 102, store)
            for selector_prefix in (0, 0x12340000, 0x80000000, 0xFFFF0000):
                for self.case in range(60):
                    failures = []
                    def callback(kind, args):
                        try:
                            return self.service(kind, [args[i] for i in range(4)], True)
                        except BaseException as error:
                            failures.append(error)
                            return 0
                    observer = observer_type(callback)
                    self.library.set_observer.argtypes = [observer_type]
                    self.library.set_observer(observer)
                    actual = self.pipeline(True, selector_prefix)
                    if failures:
                        raise failures[0]
                    expected = self.pipeline(False, selector_prefix)
                    self.assertEqual(actual, expected, (version, selector_prefix, self.case))


    def test_replay_stop_selectors_captured_rows_and_callback_cursors(self):
        """Signed row selectors and callback-replaced roots retain the captured row."""
        observer_type = C.CFUNCTYPE(C.c_uint32, C.c_uint32, C.POINTER(C.c_size_t))
        self.library.set_observer.argtypes = [observer_type]
        self.library.set_root.argtypes = [C.c_uint, C.c_uint32]
        for version in ('original', 'linked'):
            self.memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
            self.memory.mem_map(0, 0x400000)
            self.write(*self.original)
            if version == 'linked':
                for address, data in self.linked:
                    self.write(address, data)

            def store(cpu, access, address, size, value, user):
                address &= 0x1FFFFFFF
                spans = [(ROWS - 16, COUNT * STRIDE + 32),
                         (STREAMS - 16, COUNT * BUFFER + 32),
                         (address_of('_ss_score'), 128), (STACK - 1024, 1024)]
                spans += [(address_of(n), nbytes) for n, nbytes in SCALARS]
                self.assertTrue(any((start & 0x1FFFFFFF) <= address and
                                    address + size <= (start & 0x1FFFFFFF) + length
                                    for start, length in spans), (hex(address), size))

            self.cpu = SequenceCpu(self.memory, 0, 100, 101, 102, store)
            for bank in (0, 1, 31):
                for row in (-3, -1, 0, 2, 5):
                    for prefix in (0, 0x12340000, 0x80000000, 0xFFFF0000):
                        for mutate in (False, True):
                            self.case = (bank * 7 + row + 3) * 2
                            failures = []

                            def callback(kind, arguments, native):
                                result = service(kind, arguments, native)
                                if mutate:
                                    # Both callbacks replace the live bank root and
                                    # every start cursor. Stop must reset the row it
                                    # captured before either service ran.
                                    address = ROWS + (6 if kind == 0 else 0) * STRIDE
                                    if native:
                                        self.library.set_root(bank, address)
                                    else:
                                        self.put(address_of('_ss_score') + bank * 4, address)
                                    for index in range(COUNT):
                                        target = STREAMS + index * BUFFER + 31 + kind
                                        if native:
                                            self.library.set_row_pointer(index, 1, target)
                                        else:
                                            self.put(ROWS + index * STRIDE + 4, target)
                                return result

                            def observer_callback(kind, args):
                                try:
                                    return callback(kind, [args[i] for i in range(4)], True)
                                except BaseException as error:
                                    failures.append(error)
                                    return 0

                            observer = observer_type(observer_callback)
                            self.library.set_observer(observer)
                            service = self.service

                            def run(native):
                                self.events = []
                                self.initialize(native)
                                root = ROWS + 3 * STRIDE
                                if native:
                                    self.library.set_root(bank, root)
                                else:
                                    self.put(address_of('_ss_score') + bank * 4, root)
                                args = (prefix | bank, prefix | (row & 0xFFFF))
                                # Replay's public row argument is s16: a MIPS C
                                # caller supplies the sign-extended short. Stop
                                # accepts s32 and must decode its low half itself.
                                self.call('_SsSndReplay', (prefix | bank, row), (), native)
                                self.call('_SsSndStop', args, (), native)
                                return self.events

                            try:
                                actual = run(True)
                                if failures:
                                    raise failures[0]
                                # The same callback effects apply to the independent
                                # original/linked MIPS execution.
                                self.service = lambda kind, args, native: callback(kind, args, native)
                                expected = run(False)
                                self.assertEqual(actual, expected,
                                                 (version, bank, row, prefix, mutate))
                            finally:
                                self.service = service



if __name__ == '__main__':
    unittest.main()
