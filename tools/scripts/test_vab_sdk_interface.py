"""Current VAB wrappers, parser and transfer on real native buffers and both MIPS images.

The oracle operates on resource bytes and independently models table publication.
Only the allocator and hardware transfer boundary are services. The sticky callback,
parser, transfer wrapper, SpuWrite and busy-state helpers execute their current bodies.
"""
import copy
import ctypes as C
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN, UC_HOOK_CODE
from unicorn.mips_const import *
from c_functions import address_of, definition_source, source_owners

ROOT = Path(__file__).resolve().parents[2]
SIZE = 0x12000
BUFFERS = (0x80180010, 0x801A0010)
TONE_POOL, TONE_SIZE = 0x801C0010, 0x200000
STACK, STOP, ALLOCATOR = 0x803F0000, 0x803FF000, 0x803FE000
FIELDS = ('_svm_vab_used', '_svm_vab_count', '_svm_vab_start', '_svm_vab_total',
          '_svm_vab_vh', '_svm_vab_pg', '_svm_vab_tn', 'kMaxPrograms',
          '_spu_inTransfer', '_spu_transferCallback', '_spu_transMode', '_spu_trans_mode',
          '_svm_pg', '_svm_vh', '_svm_tn', '_svm_cur')
FORMATS = ('16B', 'H', '16I', '16I', '16I', '16I', '16I', 'h', 'i', 'I', 'i', 'i',
           'I', 'I', 'I', '26B')
POINTER_FIELDS = {4: 16, 5: 16, 6: 16, 9: 1, 12: 1, 13: 1, 14: 1}
PREFIX = r'''
#include <stdint.h>
#include <string.h>
#include "rr/spu.h"
#include "rr/channels.h"
#include "psyq/libsnd/ssvab.h"
#include "psyq/libspu/spumem.h"
#include "psyq/libsnd/vmmain.h"
#include "rr/spu_tone_snapshot.h"
#include "rr/spu_tone_update.h"
#include "psyq/libsnd/vmnote2.h"
unsigned short SsPitchFromNote(int,int,int,int);
u16 SsSemitonePitchTable[12],_svm_ptable[128];
static struct {
 u8 before[16], used[16]; u16 count; u32 start[16], total[16];
 u8 *heads[16]; ProgAtrBuffer *programs[16]; VagAtrBuffer *tones[16];
 s16 maximum; s32 busy; void (*volatile callback)(void);
 s32 selected_mode,raw_mode;
 ProgAtrBuffer *selected_program; u8 *selected_head; VagAtrBuffer *selected_tone;
 SvmCur current; u8 after[16];
} state;
static struct {u8 before[16];u32 words[0x12000/4];u8 after[16];} buffers[2];
static struct {u8 before[16];u32 words[0x200000/4];u8 after[16];} tone_storage;
void *tone_buffer(void){return tone_storage.words;}
#define _svm_vab_used state.used
#define _svm_vab_count state.count
#define _svm_vab_start state.start
#define _svm_vab_total state.total
#define _svm_vab_vh state.heads
#define _svm_vab_pg state.programs
#define _svm_vab_tn state.tones
#define kMaxPrograms state.maximum
#define _spu_inTransfer state.busy
#define _spu_transferCallback state.callback
#define _spu_transMode state.selected_mode
#define _spu_trans_mode state.raw_mode
#define _svm_pg state.selected_program
#define _svm_vh state.selected_head
#define _svm_tn state.selected_tone
#define _svm_cur state.current
typedef u32 (*Observer)(u32,uintptr_t,uintptr_t,uintptr_t);
static Observer observer;
void set_observer(Observer value){observer=value;}
static s32 allocate(u32 size,s32 mode,s16 bank){return observer(0,size,(u32)mode,(u32)(s32)bank);}
void test_actual_mode(s32 mode);
void SpuSetTransferMode(s32 mode){observer(1,(u32)mode,0,0);test_actual_mode(mode);}
u32 SpuSetTransferStartAddr(s32 address){return observer(2,(u32)address,0,0);}
u32 _spu_Fw(const u16 *source,u32 bytes){return observer(3,(uintptr_t)source,bytes,0);}
void completion(void){}
volatile void *field(unsigned index){
 volatile void *values[]={state.used,&state.count,state.start,state.total,state.heads,
 state.programs,state.tones,&state.maximum,&state.busy,&state.callback,
 &state.selected_mode,&state.raw_mode,&state.selected_program,
 &state.selected_head,&state.selected_tone,&state.current};
 return values[index];
}
void *buffer(unsigned index){return buffers[index].words;}
void *state_owner(void){return &state;}
unsigned state_size(void){return sizeof(state);}
void reset_storage(void){memset(&state,0xA5,sizeof(state));memset(buffers,0xA5,sizeof(buffers));}
s32 open_bank(unsigned kind,u8 *data,s32 bank,s32 mode){
 if(kind==0)return SsVabOpenHeadSticky(data,bank,mode);
 if(kind==1)return SsVabFakeHead(data,bank,mode);
 return _SsVabOpenHeadWithMode(data,(s16)bank,allocate,mode);
}
'''


def word(value):
    return value & 0xFFFFFFFF


def signed(value):
    return C.c_int32(value).value


class VabSdkInterfaceTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temporary = TemporaryDirectory()
        cls.addClassCleanup(cls.temporary.cleanup)
        owners = source_owners(ROOT/'src/main/PAL')
        code = PREFIX + owners['_SsVabOpenHeadWithMode'].read_text()
        for name in ('SsVabOpenHeadSticky', 'SsVabFakeHead', 'VsGetAddr',
                     '_spu_getInTransfer', '_spu_setInTransfer', 'SsVabTransBody', 'SpuWrite',
                     '_SsVmVSetUp', 'SsUtGetProgAtr', 'SsUtGetVagAtr', 'SsUtSetVagAtr', 'note2pitch2', 'SsPitchFromNote'):
            code += definition_source(owners[name].read_text(), name)
        # Observe calls before the actual mode helper without changing its body.
        code += definition_source(owners['SpuSetTransferMode'].read_text(), 'SpuSetTransferMode').replace(
            'void SpuSetTransferMode(', 'void test_actual_mode(')
        directory = Path(cls.temporary.name)
        source = directory/'sdk.c'
        source.write_text(code)
        result = subprocess.run(['cc', '-std=gnu11', '-O2', '-shared', '-fPIC',
            '-Wall', '-Wextra', '-Werror',
            '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
            '-I'+str(ROOT/'include'), str(source), '-o', str(directory/'sdk.so')],
            capture_output=True, text=True)
        if result.returncode:
            raise AssertionError(result.stderr)
        cls.lib = C.CDLL(str(directory/'sdk.so'))
        image = (ROOT/'assets/PAL/main.exe').read_bytes()
        base = struct.unpack_from('<I', image, 24)[0]
        cls.coarse_pitch = struct.unpack_from('<12H', image, 0x800+address_of('SsSemitonePitchTable')-base)
        cls.fine_pitch = struct.unpack_from('<128H', image, 0x800+address_of('_svm_ptable')-base)
        (C.c_uint16*12).in_dll(cls.lib, 'SsSemitonePitchTable')[:] = cls.coarse_pitch
        (C.c_uint16*128).in_dll(cls.lib, '_svm_ptable')[:] = cls.fine_pitch
        cls.lib.note2pitch2.argtypes = [C.c_int32, C.c_int32]
        cls.lib.note2pitch2.restype = C.c_uint16
        cls.lib.field.argtypes = [C.c_uint]
        cls.lib.field.restype = C.c_void_p
        cls.lib.buffer.argtypes = [C.c_uint]
        cls.lib.buffer.restype = C.c_void_p
        cls.lib.tone_buffer.restype = C.c_void_p
        cls.host_tone = cls.lib.tone_buffer()
        if cls.host_tone <= 0xFFFFFFFF:
            raise AssertionError("Expected full native tone pointer above 4 GiB")
        cls.lib.state_owner.restype = C.c_void_p
        cls.host_fields = [cls.lib.field(i) for i in range(len(FIELDS))]
        cls.host_buffers = [cls.lib.buffer(i) for i in range(2)]
        if not all(p > 0xFFFFFFFF for p in cls.host_fields + cls.host_buffers):
            raise AssertionError('Expected full native pointers above 4 GiB')
        cls.lib.open_bank.argtypes = [C.c_uint, C.c_void_p, C.c_int32, C.c_int32]
        cls.lib.open_bank.restype = C.c_int32
        cls.lib.SsVabTransBody.argtypes = [C.c_void_p, C.c_int32]
        cls.lib.SsVabTransBody.restype = C.c_int32
        cls.lib.SpuSetTransferMode.argtypes = [C.c_int32]
        cls.lib.SpuSetTransferMode.restype = None
        cls.lib._SsVmVSetUp.argtypes = [C.c_int32, C.c_int32]
        cls.lib._SsVmVSetUp.restype = C.c_int32
        cls.lib.SsUtGetProgAtr.argtypes = [C.c_int32, C.c_int16, C.c_void_p]
        cls.lib.SsUtGetProgAtr.restype = C.c_int32
        cls.lib.SsUtGetVagAtr.argtypes = [C.c_int32, C.c_int16, C.c_int32, C.c_void_p]
        cls.lib.SsUtGetVagAtr.restype = C.c_int32
        cls.lib.SsUtSetVagAtr.argtypes = [C.c_int32, C.c_int32, C.c_int32, C.c_void_p]
        cls.lib.SsUtSetVagAtr.restype = C.c_int32
        cls.lib.VsGetAddr.argtypes = [C.c_uint32, C.c_int32, C.c_int16]
        cls.lib.VsGetAddr.restype = C.c_int32
        cls.completion = C.cast(cls.lib.completion, C.c_void_p).value
        # A separate observer checks the wrapper's complete raw-word contract;
        # the parser test above always executes the actual parser and callback.
        wrapper_code = r'''
#include "psyq/libsnd/ssvab.h"
#include "psyq/libspu/spumem.h"
#include "psyq/libspu/spumode.h"
#include "psyq/libspu/spuwait.h"
void *seen_data;
s32 seen_bank,seen_address,seen_callback,seen_wait;
static s32 parser_result;
s32 _SsVabOpenHeadWithMode(void *data,s16 bank,VabTransFunc callback,s32 address){
 seen_data=data;seen_bank=bank;seen_address=address;seen_callback=callback==VsGetAddr;
 return parser_result;
}
s32 invoke_wrapper(unsigned kind,u8 *data,s32 bank,s32 address,s32 result){
 parser_result=result;
 return kind ? SsVabFakeHead(data,bank,address) : SsVabOpenHeadSticky(data,bank,address);
}
int SpuIsTransferCompleted(int wait){seen_wait=wait;return parser_result;}
s32 invoke_completion(s32 wait,s32 result){parser_result=result;return SsVabTransCompleted(wait);}
'''
        for name in ('SsVabOpenHeadSticky', 'SsVabFakeHead', 'VsGetAddr', 'SsVabTransCompleted'):
            wrapper_code += definition_source(owners[name].read_text(), name)
        source = directory/'wrappers.c'
        source.write_text(wrapper_code)
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
                        '-I'+str(ROOT/'include'), str(source), '-o', str(directory/'wrappers.so')], check=True)
        cls.wrappers = C.CDLL(str(directory/'wrappers.so'))
        cls.wrappers.invoke_wrapper.argtypes = [C.c_uint, C.c_void_p, C.c_int32, C.c_int32, C.c_int32]
        cls.wrappers.invoke_wrapper.restype = C.c_int32
        cls.wrappers.invoke_completion.argtypes = [C.c_int32, C.c_int32]
        cls.wrappers.invoke_completion.restype = C.c_int32

    def machines(self):
        image = (ROOT/'assets/PAL/main.exe').read_bytes()
        for linked in (False, True):
            cpu = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
            cpu.mem_map(0, 0x400000)
            cpu.mem_write(struct.unpack_from('<I', image, 24)[0] & 0x1FFFFFFF, image[0x800:])
            if linked:
                with (ROOT/'build/PAL/main.elf').open('rb') as stream:
                    elf = ELFFile(stream)
                    for section in elf.iter_sections():
                        if section['sh_flags'] & 2 and section['sh_type'] == 'SHT_PROGBITS':
                            cpu.mem_write(section['sh_addr'] & 0x1FFFFFFF, section.data())
            yield linked, cpu

    def read(self, address, size):
        return bytes(self.cpu.mem_read(address & 0x1FFFFFFF, size))

    def write(self, address, data):
        self.cpu.mem_write(address & 0x1FFFFFFF, bytes(data))

    def resource_regions(self):
        regions = [(p, a, SIZE) for p, a in zip(self.host_buffers, BUFFERS)]
        if getattr(self, 'with_tones', False):
            regions.append((self.host_tone, TONE_POOL, TONE_SIZE))
        return regions

    def wire_pointer(self, pointer):
        if not pointer:
            return 0
        if pointer == self.completion:
            return ALLOCATOR
        for native, wire, size in self.resource_regions():
            if native <= pointer < native + size:
                return wire + pointer - native
        raise AssertionError(('Unowned native pointer', hex(pointer)))

    def native_pointer(self, address):
        if not address:
            return None
        if address == ALLOCATOR:
            return self.completion
        for native, wire, size in self.resource_regions():
            if wire <= address < wire + size:
                return native + address - wire
        raise AssertionError(('Unowned PAL pointer', hex(address)))

    def snapshot(self, native):
        values = []
        for i, (name, fmt) in enumerate(zip(FIELDS, FORMATS)):
            if native and i in POINTER_FIELDS:
                count = POINTER_FIELDS[i]
                pointers = (C.c_void_p * count).from_address(self.host_fields[i])
                values.append(tuple(self.wire_pointer(p) for p in pointers))
            else:
                size = struct.calcsize('<'+fmt)
                data = C.string_at(self.host_fields[i], size) if native else self.read(address_of(name), size)
                values.append(struct.unpack('<'+fmt, data))
        buffers = tuple(C.string_at(p-16, size+32) if native else self.read(a-16, size+32)
                        for p, a, size in self.resource_regions())
        if native:
            base, size = self.lib.state_owner(), self.lib.state_size()
            untouched = bytearray(b'\1'*size)
            for i, fmt in enumerate(FORMATS):
                size_field = POINTER_FIELDS[i]*C.sizeof(C.c_void_p) if i in POINTER_FIELDS else struct.calcsize('<'+fmt)
                offset = self.host_fields[i] - base
                untouched[offset:offset+size_field] = b'\0'*size_field
            raw = C.string_at(base, size)
            self.assertTrue(all(v == 0xA5 for v, keep in zip(raw, untouched) if keep))
        return tuple(values), buffers

    def set_field(self, field, index, value, native):
        if native:
            kind = C.c_void_p if field in POINTER_FIELDS else C.c_uint8 if field in (0, 15) else C.c_uint16 if field in (1, 7) else C.c_uint32
            cell = kind.from_address(self.host_fields[field] + index*C.sizeof(kind))
            cell.value = self.native_pointer(value) if field in POINTER_FIELDS else value
        else:
            fmt = 'B' if field in (0, 15) else 'H' if field in (1, 7) else 'I'
            self.write(address_of(FIELDS[field]) + index*struct.calcsize(fmt), struct.pack('<'+fmt, value & ((1 << (8*struct.calcsize(fmt)))-1)))

    def reset(self, model, native):
        values, buffers = model
        if native:
            self.lib.reset_storage()
        for field, row in enumerate(values):
            for index, value in enumerate(row):
                self.set_field(field, index, value, native)
        for (p, a, size), data in zip(self.resource_regions(), buffers):
            self.assertEqual(len(data), size+32)
            if native:
                C.memmove(p-16, bytes(data), len(data))
            else:
                self.write(a-16, data)

    def initial(self, case):
        rng = random.Random(0x56414200 + case)
        buffers = [bytearray(b'\xA5'*16 + rng.randbytes(SIZE) + b'\xA5'*16) for _ in range(2)]
        values = [[2 if i % 3 == 0 else 0 for i in range(16)], [case*251 & 65535],
                  [0x8000+i*8 for i in range(16)], [31+i for i in range(16)]]
        values += [[BUFFERS[(case+i)&1]+32+i*16 for i in range(16)] for _ in range(3)]
        values += [[-123], [1], [0], [-77], [12345]]
        values += [[BUFFERS[1]+96], [BUFFERS[0]+32], [BUFFERS[1]+128],
                   [(case*17+i*29) & 255 for i in range(26)]]
        variant = case % 16
        if case & 32:
            values[0][15] = 0
        if variant == 4:
            values[1][0] = 65535
        version = (4, 5, 6, -1)[case//16 % 4]
        form = 0x56414270 if case & 1 else 0x56414271
        maximum = 128 if form & 255 == 0x70 and version >= 5 else 64
        programs = (0, 1, 7, maximum)[case//4 % 4]
        samples = (0, 1, 7, maximum*2-1, 0x101)[case//3 % 5]
        if variant == 0:
            values[8][0] = 0  # Actual getter reports an already-active transfer.
        if variant == 1:
            values[0] = [1]*16
        if variant == 2:
            form = 0xDEADBEEF
        if variant == 3:
            programs = maximum+1
        data = buffers[0]
        struct.pack_into('<Ii', data, 16, form, version)
        struct.pack_into('<HHH', data, 16+18, programs, 0x7171, samples)
        for i in range(maximum):
            data[16+32+i*16] = (i+case) % 4
        offset = 16+32+maximum*16+programs*512
        for i in range(256):
            units = (i*17+case*3) % 64 if case & 4 else (i*8191+case) & 65535
            if variant == 4:
                units = 0
            elif variant == 15 and i == 0:
                units = 4096
            struct.pack_into('<H', data, offset+i*2, units)
        return values, buffers

    @staticmethod
    def freeze(model):
        return tuple(tuple(row) for row in model[0]), tuple(bytes(b) for b in model[1])

    def service(self, kind, args, native=None, model=None):
        args = tuple(word(a) for a in args)
        state = self.freeze(model) if model is not None else self.snapshot(native)
        self.events.append((kind, args, state))
        bank = self.current_bank
        def assign(field, index, value):
            if model is not None:
                model[0][field][index] = value
            else:
                self.set_field(field, index, value, native)
        if kind == 0:
            bank = signed(args[2])
            if self.case & 8:
                assign(4, bank, BUFFERS[1]+32)
                assign(5, bank, BUFFERS[1]+64)
            return self.allocation
        if kind == 1 and self.case & 8:
            assign(2, bank, 0x76543210)
            assign(3, bank, 111)
            assign(0, bank, 9)
        if kind == 1 and model is not None:
            model[0][10][0] = int(args[0] == 1)
            model[0][11][0] = signed(args[0])
        if kind == 2 and self.case & 8:
            assign(3, bank, 0x7EFF1)
        if kind == 3 and self.case & 8:
            assign(0, bank, 7)
            assign(9, 0, ALLOCATOR)
        return 0 if kind == 2 and self.case % 16 == 7 else args[0] if kind == 2 else 0xA55AA55A

    def oracle_open(self, model, kind, raw_bank, mode):
        values, buffers = model
        flags, count, starts, totals, heads, programs, tones, maximum, busy, callback = values[:10]
        if busy[0] != 1:
            self.branches.add('busy')
            return -1
        busy[0] = 0
        bank = C.c_int16(raw_bank).value
        if bank == -1:
            self.branches.add('auto')
            bank = next((i for i, used in enumerate(flags) if used == 0), 16)
        if bank >= 16 or flags[bank] != 0:
            self.branches.add('unavailable')
            busy[0] = 1
            return -1
        flags[bank] = 1
        count[0] = (count[0]+1) & 65535
        heads[bank] = BUFFERS[0]
        data = buffers[0]
        form, version = struct.unpack_from('<Ii', data, 16)
        def reject():
            flags[bank] = 0
            busy[0] = 1
            count[0] = (count[0]-1) & 65535
            return -1
        if form >> 8 != 0x564142:
            self.branches.add('bad-form')
            return reject()
        maximum[0] = 128 if form & 255 == 0x70 and version >= 5 else 64
        self.branches.add('programs-'+str(maximum[0]))
        number = struct.unpack_from('<H', data, 16+18)[0]
        if number > maximum[0]:
            self.branches.add('program-overflow')
            return reject()
        programs[bank] = BUFFERS[0]+32
        prior = 0
        for i in range(maximum[0]):
            struct.pack_into('<I', data, 16+32+i*16+8, prior)
            prior += data[16+32+i*16] != 0
        tones[bank] = BUFFERS[0]+32+maximum[0]*16
        offset = 16+32+maximum[0]*16+number*512
        samples = data[16+22]
        if struct.unpack_from('<H', data, 16+22)[0] != samples:
            self.branches.add('sample-low8')
        sizes = [v << (3 if version >= 5 else 2) for v in struct.unpack_from('<256H', data, offset)[:samples+1]]
        total = word(sum(sizes))
        self.branches.add('zero-samples' if total == 0 else 'nonzero-samples')
        allocation = self.service(0, (total, mode, bank), model=model) if kind == 2 else word(mode)
        if allocation == 0xFFFFFFFF:
            self.branches.add('allocation-failure-without-rollback')
            return -1  # The original deliberately does not roll back this failure.
        if word(allocation+total) > 0x80000:
            self.branches.add('ram-overflow')
            return reject()
        if allocation+total > 0xFFFFFFFF:
            self.branches.add('wrapped-end')
        starts[bank] = allocation
        accumulated = 0
        for i, size in enumerate(sizes):
            accumulated = word(accumulated+size)
            struct.pack_into('<H', data, 16+32+(i//2)*16+12+(i&1)*2,
                             (word(allocation+accumulated) >> 3) & 65535)
        totals[bank] = accumulated
        flags[bank] = 2
        return bank

    def oracle_transfer(self, model, bank):
        values = model[0]
        self.current_bank = bank
        allocation = values[2][bank]
        self.service(1, (0,), model=model)
        if self.service(2, (allocation,), model=model):
            self.service(3, (BUFFERS[1]+0x11000, min(values[3][bank], 0x7EFF0)), model=model)
            if values[9][0] == 0:
                values[8][0] = 0
            values[0][bank] = 1
            return bank
        values[8][0] = 1
        return -1

    def buffer_cell(self, model, address):
        for index, (_, base, size) in enumerate(self.resource_regions()):
            if base <= address < base+size:
                return model[1][index], address-base+16
        raise AssertionError(('Unowned oracle address', hex(address)))

    def oracle_select(self, model, raw_bank, raw_program):
        values = model[0]
        bank, program = C.c_int16(raw_bank).value, C.c_int16(raw_program).value
        if not 0 <= bank < 16 or values[0][bank] != 1 or program >= values[7][0]:
            return -1
        values[13][0], values[12][0], values[14][0] = values[4][bank], values[5][bank], values[6][bank]
        current = values[15]
        current[1], current[6] = bank & 255, program & 255
        data, offset = self.buffer_cell(model, values[12][0]+program*16+8)
        current[7] = data[offset]
        return 0

    def oracle_attributes(self, model, bank, program, output):
        values = model[0]
        if values[0][bank] != 1:
            return -1
        # The original copy deliberately ignores a selector failure.
        self.oracle_select(model, bank, program)
        destination, offset = self.buffer_cell(model, output)
        for field in (0, 1, 2, 3, 4):
            source, at = self.buffer_cell(model, values[12][0]+program*16+field)
            destination[offset+field] = source[at]
        source, at = self.buffer_cell(model, values[12][0]+program*16+6)
        destination[offset+6:offset+8] = source[at:at+2]
        return 0

    def invoke(self, function, args):
        self.write(STACK-0x2000, b'\x5A'*0x2040)
        for reg in range(UC_MIPS_REG_1, UC_MIPS_REG_31+1):
            self.cpu.reg_write(reg, word(0xA5A50000 + self.case*31 + reg))
        self.cpu.reg_write(UC_MIPS_REG_SP, STACK)
        self.cpu.reg_write(UC_MIPS_REG_RA, STOP)
        for reg, value in zip((UC_MIPS_REG_A0, UC_MIPS_REG_A1, UC_MIPS_REG_A2, UC_MIPS_REG_A3), args):
            self.cpu.reg_write(reg, word(value))
        preserved = {reg: self.cpu.reg_read(reg) for reg in
                     (*range(UC_MIPS_REG_S0, UC_MIPS_REG_S7+1), UC_MIPS_REG_FP, UC_MIPS_REG_GP, UC_MIPS_REG_SP, UC_MIPS_REG_RA)}
        self.cpu.emu_start(address_of(function), STOP, count=50000)
        self.assertEqual(self.cpu.reg_read(UC_MIPS_REG_PC), STOP)
        self.assertEqual({reg: self.cpu.reg_read(reg) for reg in preserved}, preserved)
        self.assertEqual(self.read(STACK, 0x40), b'\x5A'*0x40)
        self.assertEqual(self.read(STACK-0x2000, 32), b'\x5A'*32)
        return signed(self.cpu.reg_read(UC_MIPS_REG_V0))

    def test_current_parser_wrappers_and_transfer_against_independent_oracle(self):
        callback_type = C.CFUNCTYPE(C.c_uint32, C.c_uint32, C.c_uint64, C.c_uint64, C.c_uint64)
        def observer(kind, a, b, c):
            try:
                args = (self.wire_pointer(a), b) if kind == 3 else (a, b, c) if kind == 0 else (a,)
                return self.service(kind, args, native=True)
            except BaseException as error:
                self.errors.append(error)
                return 0
        observer = callback_type(observer)
        self.lib.set_observer(observer)
        coverage = set()
        self.branches = set()
        for linked, self.cpu in self.machines():
            targets = {ALLOCATOR: 0, address_of('SpuSetTransferMode'): 1,
                       address_of('SpuSetTransferStartAddr'): 2, address_of('_spu_Fw'): 3}
            def hook(cpu, pc, size, opaque):
                if pc not in targets:
                    return
                kind = targets[pc]
                args = tuple(cpu.reg_read(r) for r in (UC_MIPS_REG_A0, UC_MIPS_REG_A1, UC_MIPS_REG_A2))
                result = self.service(kind, args[:3 if kind == 0 else 2 if kind == 3 else 1], native=False)
                if kind == 1:
                    # The observer records/mutates the call boundary; the real
                    # mode helper then executes all of its target instructions.
                    return
                ra = cpu.reg_read(UC_MIPS_REG_RA)
                for reg in (UC_MIPS_REG_AT, UC_MIPS_REG_V0, UC_MIPS_REG_V1,
                            *range(UC_MIPS_REG_A0, UC_MIPS_REG_T7+1), UC_MIPS_REG_T8, UC_MIPS_REG_T9):
                    cpu.reg_write(reg, word(0xDEAD0000 + reg + self.case))
                cpu.reg_write(UC_MIPS_REG_HI, 0x12345678)
                cpu.reg_write(UC_MIPS_REG_LO, 0x87654321)
                cpu.reg_write(UC_MIPS_REG_V0, word(result))
                cpu.reg_write(UC_MIPS_REG_PC, ra)
            self.cpu.hook_add(UC_HOOK_CODE, hook)
            for self.case in range(192):
                kind = self.case//16 % 3
                raw_bank = (-1, 1, 15, 16)[self.case//3 % 4]
                raw_bank = signed(word(raw_bank) | (0xA55A0000 if self.case & 1 else 0)) if raw_bank >= 0 else raw_bank
                mode = (0x2000, 0x7FFF8, -1, -0x1000)[self.case % 4]
                if self.case % 16 == 7:
                    mode = 0x2000
                self.allocation = word((0x3000, -1, 0x80000, -0x1000)[self.case % 4])
                if self.case % 16 == 7:
                    self.allocation = 0x3000
                initial = self.initial(self.case)
                model = copy.deepcopy(initial)
                self.events = []
                opened = self.oracle_open(model, kind, raw_bank, mode)
                self.current_bank = opened
                transferred = self.oracle_transfer(model, opened) if opened >= 0 else None
                selected = copied = None
                if opened >= 0:
                    program = (-1, 0, 1, model[0][7][0]-1, model[0][7][0])[self.case % 5]
                    raw_program = signed(0x12340000 | (program & 65535))
                    selected = self.oracle_select(model, opened | 0xABCD0000, raw_program)
                    copied = self.oracle_attributes(model, opened, program, BUFFERS[1]+0x11020)
                expected_events, expected = self.events, self.freeze(model)
                coverage.add((kind, opened >= 0, transferred == -1))
                self.reset(initial, False)
                self.events = []
                function = ('SsVabOpenHeadSticky', 'SsVabFakeHead', '_SsVabOpenHeadWithMode')[kind]
                args = (BUFFERS[0], raw_bank, mode) if kind != 2 else (BUFFERS[0], C.c_int16(raw_bank).value, ALLOCATOR, mode)
                actual = self.invoke(function, args)
                self.assertEqual(actual, opened, (linked, self.case, 'open'))
                if opened >= 0:
                    self.assertEqual(self.invoke('SsVabTransBody', (BUFFERS[1]+0x11000, opened)), transferred)
                    self.assertEqual(self.invoke('_SsVmVSetUp', (opened | 0xABCD0000, raw_program)), selected)
                    self.assertEqual(self.invoke('SsUtGetProgAtr', (opened, program, BUFFERS[1]+0x11020)), copied)
                self.assertEqual(self.events, expected_events, (linked, self.case, 'MIPS events'))
                self.assertEqual(self.snapshot(False), expected, (linked, self.case, 'MIPS final'))
                self.reset(initial, True)
                self.events, self.errors = [], []
                actual = self.lib.open_bank(kind, self.host_buffers[0], raw_bank, mode)
                self.assertEqual(actual, opened, (linked, self.case, 'native open'))
                if opened >= 0:
                    self.assertEqual(self.lib.SsVabTransBody(self.host_buffers[1]+0x11000, opened), transferred)
                    self.assertEqual(self.lib._SsVmVSetUp(signed(opened | 0xABCD0000), raw_program), selected)
                    self.assertEqual(self.lib.SsUtGetProgAtr(opened, program, self.host_buffers[1]+0x11020), copied)
                if self.errors:
                    raise self.errors[0]
                self.assertEqual(self.events, expected_events, (linked, self.case, 'native events'))
                self.assertEqual(self.snapshot(True), expected, (linked, self.case, 'native final'))
        self.assertTrue({'busy', 'auto', 'unavailable', 'bad-form', 'program-overflow',
                         'programs-64', 'programs-128', 'sample-low8', 'zero-samples',
                         'nonzero-samples', 'allocation-failure-without-rollback', 'ram-overflow',
                         'wrapped-end'} <= self.branches, self.branches)
        self.assertTrue(any(failed for _, _, failed in coverage))
        for kind in range(3):
            self.assertTrue(any(k == kind and success for k, success, _ in coverage))
            self.assertTrue(any(k == kind and not success for k, success, _ in coverage))

    def oracle_tone_copy(self, model, bank, program, tone, buffer, writing):
        values = model[0]
        bank, program = C.c_int16(bank).value, C.c_int16(program).value
        if values[0][bank] != 1:
            return -1
        self.oracle_select(model, bank, program)
        group = C.c_int8(values[15][7]).value
        index = C.c_int16(word(tone + group*16)).value
        record = values[14][0] + index*32
        source, destination = (buffer, record) if writing else (record, buffer)
        for field, size in [(i, 1) for i in (0,1,2,3,4,5,7,6,8,9,10,11,12,13)] + [(i,2) for i in (16,18,20,22)]:
            data, at = self.buffer_cell(model, source+field)
            captured = bytes(data[at:at+size])
            data, at = self.buffer_cell(model, destination+field)
            data[at:at+size] = captured
        return 0

    def oracle_pitch(self, model, note, fine):
        current = model[0][15]
        index = C.c_int8(current[7]).value*16 + C.c_int8(current[12]).value
        data, at = self.buffer_cell(model, model[0][14][0]+index*32)
        fraction = fine + data[at+5]
        semitone = note + fraction//128 - data[at+4]
        octave, scale = divmod(semitone, 12)
        octave -= 2
        # This pipeline uses MIDI notes 0..127 and fine -128..127. Centers span all byte
        # values, and the resulting shifts are within the SDK's C domain.
        if octave >= 0:
            return 0x3FFF
        shift = -octave
        self.assertTrue(1 <= shift <= 31)
        value = signed(self.coarse_pitch[scale]*self.fine_pitch[fraction%128]) >> 16
        return ((word(value) + (1 << (shift-1))) & 0xFFFFFFFF) >> shift & 65535

    def test_real_tone_copy_after_parser_transfer_and_resource_relocation(self):
        # The old 192-case corpus and its RNG/resources remain unchanged. The
        # extra guarded allocation supports the complete signed tone-index range.
        self.with_tones = True
        self.addCleanup(setattr, self, 'with_tones', False)
        pool = b'\xA5'*16 + bytes((i*73+(i>>7)+19)&255 for i in range(TONE_SIZE)) + b'\xA5'*16
        callback_type = C.CFUNCTYPE(C.c_uint32, C.c_uint32, C.c_uint64, C.c_uint64, C.c_uint64)
        def observe(kind, a, b, c):
            try:
                args = (self.wire_pointer(a), b) if kind == 3 else (a, b, c) if kind == 0 else (a,)
                return self.service(kind, args, native=True)
            except BaseException as error:
                self.errors.append(error)
                return 0
        observer = callback_type(observe)
        self.lib.set_observer(observer)
        groups = (0,1,127,128,129,254,255,7)
        tones = (0,1,-1,32767,-32768,0x7FFFFFFF,-0x80000000,signed(0xA5A58001))
        programs = (-1,0,1,63,64)
        flags = (1,1,1,0,2,255)
        for linked, self.cpu in self.machines():
            targets = {address_of('SpuSetTransferMode'): 1,
                       address_of('SpuSetTransferStartAddr'): 2, address_of('_spu_Fw'): 3}
            def hook(cpu, pc, size, opaque):
                if pc not in targets:
                    return
                kind = targets[pc]
                args = tuple(cpu.reg_read(r) for r in (UC_MIPS_REG_A0, UC_MIPS_REG_A1))
                result = self.service(kind, args if kind == 3 else args[:1], native=False)
                if kind == 1:
                    return  # Execute the current helper after observing it.
                ra = cpu.reg_read(UC_MIPS_REG_RA)
                for reg in (UC_MIPS_REG_AT, UC_MIPS_REG_V0, UC_MIPS_REG_V1,
                            *range(UC_MIPS_REG_A0, UC_MIPS_REG_T7+1), UC_MIPS_REG_T8, UC_MIPS_REG_T9):
                    cpu.reg_write(reg, word(0xDEAD0000+reg))
                cpu.reg_write(UC_MIPS_REG_HI, 0x12345678)
                cpu.reg_write(UC_MIPS_REG_LO, 0x87654321)
                cpu.reg_write(UC_MIPS_REG_V0, word(result))
                cpu.reg_write(UC_MIPS_REG_PC, ra)
            self.cpu.hook_add(UC_HOOK_CODE, hook)
            for case in range(48):
                self.case, self.current_bank = 4, 1
                self.events, self.errors, self.branches = [], [], set()
                initial = self.initial(4)
                initial[1].append(bytearray(pool))
                model = copy.deepcopy(initial)
                self.assertEqual(self.oracle_open(model, 0, 1, 0x2000), 1)
                self.assertEqual(self.oracle_transfer(model, 1), 1)
                opened = copy.deepcopy(model)
                expected_events = self.events
                for native in (False, True):
                    self.reset(initial, native)
                    self.events = []
                    if native:
                        self.assertEqual(self.lib.open_bank(0, self.host_buffers[0], 1, 0x2000), 1)
                        self.assertEqual(self.lib.SsVabTransBody(self.host_buffers[1]+0x11000, 1), 1)
                    else:
                        self.assertEqual(self.invoke('SsVabOpenHeadSticky', (BUFFERS[0],1,0x2000)), 1)
                        self.assertEqual(self.invoke('SsVabTransBody', (BUFFERS[1]+0x11000,1)), 1)
                    if self.errors:
                        raise self.errors[0]
                    self.assertEqual(self.events, expected_events, (linked,case,native,'load events'))
                    self.assertEqual(self.snapshot(native), self.freeze(opened), (linked,case,native,'loaded'))
                # An external resource relocation publishes a real backing
                # allocation; no address truncation or native low mapping occurs.
                model = copy.deepcopy(opened)
                center = TONE_POOL+TONE_SIZE//2
                model[0][6][1] = model[0][14][0] = center
                group, tone, program = groups[case%8], tones[case//6], programs[case%5]
                model[0][0][1] = flags[case%6]
                model[0][15][7] = group
                if program < 64:
                    data, at = self.buffer_cell(model, model[0][5][1]+program*16+8)
                    data[at] = group
                index = C.c_int16(word(tone+C.c_int8(group).value*16)).value
                output = center+index*32+2 if case%4 == 0 else BUFFERS[1]+0x11020
                ready = copy.deepcopy(model)
                raw_bank = signed(0xABCD0001)
                raw_program = signed(0x12340000 | (program&65535))
                got = self.oracle_tone_copy(model, raw_bank, program, tone, output, False)
                after_get = self.freeze(model)
                replacement = bytes((case*17+i*29)&255 for i in range(24))
                data, at = self.buffer_cell(model, output)
                data[at:at+24] = replacement
                put = self.oracle_tone_copy(model, raw_bank, raw_program, tone, output, True)
                expected = self.freeze(model)
                for native in (False, True):
                    self.reset(ready, native)
                    if native:
                        result = self.lib.SsUtGetVagAtr(raw_bank, program, tone, self.native_pointer(output))
                    else:
                        result = self.invoke('SsUtGetVagAtr', (raw_bank,program,tone,output))
                    self.assertEqual(result, got, (linked,case,native,'get'))
                    self.assertEqual(self.snapshot(native), after_get, (linked,case,native,'after get'))
                    if native:
                        C.memmove(self.native_pointer(output), replacement, 24)
                        result = self.lib.SsUtSetVagAtr(raw_bank, raw_program, tone, self.native_pointer(output))
                    else:
                        self.write(output, replacement)
                        result = self.invoke('SsUtSetVagAtr', (raw_bank,raw_program,tone,output))
                    self.assertEqual(result, put, (linked,case,native,'set'))
                    self.assertEqual(self.snapshot(native), expected, (linked,case,native,'after set'))
                    pitched = copy.deepcopy(model)
                    pitched[0][15][12] = (case*73)&255
                    self.set_field(15, 12, pitched[0][15][12], native)
                    note = case*17%128
                    # Retain the original positive-fine cases and also execute
                    # the negative remainder/carry branch with valid shifts.
                    for fine in (case*29%128, -128, -1):
                        pitch = self.oracle_pitch(pitched, note, fine)
                        raw_note = signed(0xABCD0000|note)
                        raw_fine = signed(0x12340000|(fine&65535))
                        result = self.lib.note2pitch2(raw_note,raw_fine) if native else self.invoke('note2pitch2',(raw_note,raw_fine))
                        self.assertEqual(result, pitch, (linked,case,native,fine,'pitch'))
                        self.assertEqual(self.snapshot(native), self.freeze(pitched), (linked,case,native,fine,'after pitch'))

    def test_sticky_allocation_callback_keeps_all_address_bits(self):
        self.case = 0
        for linked, self.cpu in self.machines():
            for address in (0, 1, 0x7FFF8, 0x80000000, 0xFFFFFFFF, 0x1234FFFF):
                for bank in (-32768, -1, 0, 32767):
                    self.assertEqual(word(self.lib.VsGetAddr(0xFEDCBA98, signed(address), bank)), address)
                    self.assertEqual(word(self.invoke('VsGetAddr', (0xFEDCBA98, address, bank))), address)

    def test_mode_helper_keeps_raw_request_and_normalizes_the_selected_mode(self):
        self.case = 0
        callback_type = C.CFUNCTYPE(C.c_uint32, C.c_uint32, C.c_uint64, C.c_uint64, C.c_uint64)
        observed, errors = [], []
        def observer(kind, a, b, c):
            try:
                observed.append((kind, word(a), self.snapshot(True)))
            except BaseException as error:
                errors.append(error)
            return 0
        observer = callback_type(observer)
        self.lib.set_observer(observer)
        for linked, self.cpu in self.machines():
            target_events = []
            def hook(cpu, pc, size, opaque):
                if pc == address_of('SpuSetTransferMode'):
                    target_events.append((1, cpu.reg_read(UC_MIPS_REG_A0), self.snapshot(False)))
            self.cpu.hook_add(UC_HOOK_CODE, hook)
            for mode in (0, 1, 2, 0xFFFF, 0x10001, 0x7FFF0001, 0x80000000, 0xFFFFFFFF):
                initial = self.initial(17)
                expected = copy.deepcopy(initial)
                expected[0][10][0] = int(mode == 1)
                expected[0][11][0] = signed(mode)
                events = [(1, mode, self.freeze(initial))]
                self.reset(initial, False)
                target_events.clear()
                self.invoke('SpuSetTransferMode', (mode,))
                self.assertEqual(target_events, events)
                self.assertEqual(self.snapshot(False), self.freeze(expected))
                self.reset(initial, True)
                observed.clear()
                self.lib.SpuSetTransferMode(signed(mode))
                if errors:
                    raise errors[0]
                self.assertEqual(observed, events)
                self.assertEqual(self.snapshot(True), self.freeze(expected))

    def test_wrappers_narrow_raw_bank_and_parser_return_at_the_actual_boundaries(self):
        self.case = 0
        for linked, self.cpu in self.machines():
            observed = []
            def hook(cpu, pc, size, opaque):
                if pc != address_of('_SsVabOpenHeadWithMode'):
                    return
                observed.append(tuple(cpu.reg_read(r) for r in
                                      (UC_MIPS_REG_A0, UC_MIPS_REG_A1, UC_MIPS_REG_A2, UC_MIPS_REG_A3)))
                ra = cpu.reg_read(UC_MIPS_REG_RA)
                for reg in (UC_MIPS_REG_AT, UC_MIPS_REG_V0, UC_MIPS_REG_V1,
                            *range(UC_MIPS_REG_A0, UC_MIPS_REG_T7+1), UC_MIPS_REG_T8, UC_MIPS_REG_T9):
                    cpu.reg_write(reg, 0xA5A50000+reg)
                cpu.reg_write(UC_MIPS_REG_HI, 0x11223344)
                cpu.reg_write(UC_MIPS_REG_LO, 0x88776655)
                cpu.reg_write(UC_MIPS_REG_V0, parser_result)
                cpu.reg_write(UC_MIPS_REG_PC, ra)
            self.cpu.hook_add(UC_HOOK_CODE, hook)
            for high in (0, 1, 0x7FFF, 0x8000, 0xFFFF):
                for low in (0, 1, 15, 16, 0x7FFF, 0x8000, 0xFFFF):
                    bank = high << 16 | low
                    for parser_result in (0, 0x7FFF, 0x8000, 0xFFFF, 0x1234FFFF, 0x80008000, 0xFFFF0001, 0x7FFFFFFF):
                        for kind, name in enumerate(('SsVabOpenHeadSticky', 'SsVabFakeHead')):
                            with self.subTest(linked=linked, bank=hex(bank), result=hex(parser_result), wrapper=name):
                                observed.clear()
                                expected = C.c_int16(parser_result).value
                                self.assertEqual(self.invoke(name, (BUFFERS[0], bank, 0xB4567890)), expected)
                                self.assertEqual(observed, [(BUFFERS[0], word(C.c_int16(bank).value),
                                                            address_of('VsGetAddr'), 0xB4567890)])
                                self.assertEqual(self.wrappers.invoke_wrapper(kind, self.host_buffers[0], signed(bank),
                                                                            signed(0xB4567890), signed(parser_result)), expected)
                                self.assertEqual(C.c_void_p.in_dll(self.wrappers, 'seen_data').value, self.host_buffers[0])
                                self.assertEqual(C.c_int32.in_dll(self.wrappers, 'seen_bank').value, C.c_int16(bank).value)
                                self.assertEqual(C.c_int32.in_dll(self.wrappers, 'seen_address').value, signed(0xB4567890))
                                self.assertEqual(C.c_int32.in_dll(self.wrappers, 'seen_callback').value, 1)

    def test_completion_wrapper_narrows_wait_and_status_at_the_actual_boundaries(self):
        self.case = 0
        for linked, self.cpu in self.machines():
            observed = []
            def hook(cpu, pc, size, opaque):
                if pc != address_of('SpuIsTransferCompleted'):
                    return
                observed.append(cpu.reg_read(UC_MIPS_REG_A0))
                ra = cpu.reg_read(UC_MIPS_REG_RA)
                for reg in (UC_MIPS_REG_AT, UC_MIPS_REG_V0, UC_MIPS_REG_V1,
                            *range(UC_MIPS_REG_A0, UC_MIPS_REG_T7+1), UC_MIPS_REG_T8, UC_MIPS_REG_T9):
                    cpu.reg_write(reg, 0x5A5A0000+reg)
                cpu.reg_write(UC_MIPS_REG_V0, status)
                cpu.reg_write(UC_MIPS_REG_PC, ra)
            self.cpu.hook_add(UC_HOOK_CODE, hook)
            for high in (0, 1, 0x7FFF, 0x8000, 0xFFFF):
                for low in (0, 1, 15, 16, 0x7FFF, 0x8000, 0xFFFF):
                    wait = high << 16 | low
                    for status in (0, 0x7FFF, 0x8000, 0xFFFF, 0x1234FFFF, 0x80008000, 0xFFFF0001, 0x7FFFFFFF):
                        observed.clear()
                        expected = C.c_int16(status).value
                        self.assertEqual(self.invoke('SsVabTransCompleted', (wait,)), expected)
                        self.assertEqual(observed, [word(C.c_int16(wait).value)])
                        self.assertEqual(self.wrappers.invoke_completion(signed(wait), signed(status)), expected)
                        self.assertEqual(C.c_int32.in_dll(self.wrappers, 'seen_wait').value, C.c_int16(wait).value)
