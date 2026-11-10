"""Real bank registration, renderer setup, selection, patching and detachment.

Native owners retain full pointers, including relative displacements backwards
within their allocation. The original and compiled PAL paths are compared to a
byte oracle after every phase, with exact ordered stores and ABI checks.
The native path additionally feeds the registered bank into the real dispatcher.
"""
import ctypes as C
from pathlib import Path
import random
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from unicorn.mips_const import UC_MIPS_REG_0, UC_MIPS_REG_PC

from c_functions import address_of, definition_source, source_owners
from verify_c_unit import verify
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
NAMES = ['RegisterModelBank', 'SetupRenderer', 'SelectModelBank',
         'PatchModelBankTexPages', 'UnrelocateModelBank']
ARENA, BANK, SCRATCH = 0x80200000, 0x80200080, 0x1F800000
STACK, STOP, SIZE, CAPACITY = 0x803F0000, 0x803FF000, 4096, 12
SNAPSHOT = SIZE + 124 + 96 + 44 + 36
SLOTS = (*range(16), -2147483648, -65537, -32768, -1, 32767, 2147483647)
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "game/gfx/model.h"
#include "game/data/cdfile2.h"
#include "game/gfx/gtepoly1.h"
#include "rr/renderer_setup.h"
enum {ARENA_BYTES=4096,BANK_OFFSET=128,BANK_BYTES=128,CAPACITY=12,SNAPSHOT=4096+124+96+44+36};
typedef struct {u8 before[16];RroModelBank *slots[16];u8 after[16];} TableOwner;
typedef struct {u8 before[16];RroModelScratch value;u8 after[16];} ScratchOwner;
typedef struct {u8 before[16];GpuColorCommand gouraud;u32 gap;GpuColorCommand flat;u8 after[16];} ColorOwner;
typedef struct {u8 before[16];s32 value;u8 after[16];} ScalarOwner;
typedef struct {TableOwner table;ScratchOwner scratch;ColorOwner color;ScalarOwner count,flags;
 Vector position;MatrixWords32 matrix;u32 order[256];u8 gpu[128];s32 page;} World;
static World live,wanted;
#define ModelBankTable live.table.slots
#define ModelBankFormat live.count.value
#define SCRATCHPAD ((u8*)&live.scratch.value)
#define MirrorRenderFlag live.flags.value
#define LoadingScreenClearColor live.color.gouraud
#define DefaultScreenClearColor live.color.flat
#define ModelTexturePage live.page
static char UnknownRroTypeFmt[]="unexpected bank command";
static unsigned events,expectedEvents,colors;
typedef struct {unsigned kind;const u32 *commands;s32 count;} Event;
static Event actualEvents[16],wantedEvents[16];
static void record(unsigned kind,const u32 *commands,s32 count) {
 assert(events<16);actualEvents[events++]=(Event){kind,commands,count};
}
void SetGteObjectTransform(ObjectTransform38 *out,const s16 *position,s32 *matrix) {
 assert(out==&live.scratch.value.work7C.transform);
 assert(position==(const s16*)&live.position&&matrix==live.matrix.w);
 record(1,0,0);
}
const u32 *EmitRroLitGouraudQuads(const u32 *commands,RroRenderWork *work,s32 count) {
 assert(work==&live.scratch.value.work7C.render&&(uintptr_t)commands>UINT32_MAX);
 record(2,commands,count);return commands+(unsigned)count*6U;
}
const u32 *EmitRroEnvMapQuads(const u32 *commands,RroRenderWork *work,s32 count) {
 assert(work==&live.scratch.value.work7C.render&&(uintptr_t)commands>UINT32_MAX);
 record(3,commands,count);return commands+(unsigned)count*8U;
}
static void TestSetColor(u32 color) {assert(color==0x3C808080U);colors++;record(4,0,0);}
s32 DebugPrintStub(const char *format,...) {(void)format;assert(!"unexpected dispatcher command");return -1;}
'''
TAIL = r'''
static void put(u8 *p,u32 x){memcpy(p,&x,4);}
static u32 get(const u8 *p){u32 x;memcpy(&x,p,4);return x;}
static u32 relative(const u8 *input,unsigned field){return get(input+BANK_OFFSET+field*4);}
static const u8 *resolved(u8 *arena,u32 offset){return arena+BANK_OFFSET+(s32)offset;}
static void header_pointer(u8 *expected,size_t offset,const void *pointer){memcpy(expected+offset,&pointer,sizeof(pointer));}
static void snapshot(u8 *output,u8 *arena,const u8 *input,RroModelBank *bank,unsigned count,unsigned slot,unsigned phase) {
 memcpy(output,arena,ARENA_BYTES);
 /* Normalize only the bank's host representation, after validating every
  * native header byte and full pointer against its real allocation. */
 memcpy(output+BANK_OFFSET,input+BANK_OFFSET,BANK_BYTES);
 put(output+BANK_OFFSET,count);
 for(unsigned field=1;field<count+3;field++) {
  u32 bits;
  if(phase==4)bits=field==1?bank->vertices.relative:field==2?bank->normals.relative:bank->streams[field-3].relative;
  else bits=0x80200080U+relative(input,field);
  put(output+BANK_OFFSET+field*4,bits);
 }
 output+=ARENA_BYTES;memset(output,0xA5,124);
 put(output,0x80204000U);put(output+4,0x80203000U);
 memcpy(output+8,&live.scratch.value.scene.view.projection.pose08.position00,16);
 memcpy(output+0x18,&live.scratch.value.scene.view.projection.pose08.angles10,16);
 memcpy(output+0x28,&live.scratch.value.scene.view.matrix28,32);
 if(phase>=2) {
  put(output+0x50,0x8020008CU);
  put(output+0x54,0x80200080U+relative(input,1));
  put(output+0x58,0x80200080U+relative(input,2));
 }
 memcpy(output+0x60,&live.scratch.value.scene.depthShift60,4);
 memcpy(output+0x64,&live.scratch.value.scene.renderFlags64,4);
 memcpy(output+0x6C,&live.scratch.value.scene.viewport6C,8);
 output+=124;memset(output,0xA5,96);
 for(unsigned i=0;i<16;i++){assert(ModelBankTable[i]==(i==slot?bank:0));put(output+16+i*4,i==slot?0x80200080U:0);}
 output+=96;memcpy(output,&live.color,44);output+=44;memcpy(output,&live.count,36);
}
void run_case(unsigned scenario,const u8 *input,unsigned slot,s32 textureSlot,s32 shift,u32 flags,s32 delta,u8 *output) {
 u8 *allocation=malloc(ARENA_BYTES+32),expectedHeader[BANK_BYTES];assert(allocation);
 u8 *arena=allocation+16;RroModelBank *bank=(RroModelBank*)(arena+BANK_OFFSET);
 _Static_assert(sizeof(void*)==8&&sizeof(RroBankCommand)==sizeof(void*),"full bank slots");
 _Static_assert(offsetof(RroModelBank,streams)+CAPACITY*sizeof(RroBankCommand)<=BANK_BYTES,"complete bank owner");
 assert((uintptr_t)bank>UINT32_MAX);memset(allocation,0xA6,ARENA_BYTES+32);memcpy(arena,input,ARENA_BYTES);
 memset(arena+BANK_OFFSET,0xB5,BANK_BYTES);unsigned count=get(input+BANK_OFFSET);assert(count<=CAPACITY&&slot<16);
 bank->streamCount=count;bank->vertices.relative=relative(input,1);bank->normals.relative=relative(input,2);
 for(unsigned i=0;i<CAPACITY;i++)bank->streams[i].relative=relative(input,i+3);
 memcpy(expectedHeader,arena+BANK_OFFSET,BANK_BYTES);
 memset(&live,0xA5,sizeof(live));for(unsigned i=0;i<16;i++)ModelBankTable[i]=0;
 live.scratch.value.scene.view.projection.pointers.cursor=live.gpu;
 live.scratch.value.scene.view.projection.pointers.order=live.order;
 memcpy(&MirrorRenderFlag,&flags,4);wanted=live;
 RegisterModelBank(bank,(s32)slot);
 wanted.table.slots[slot]=bank;
 header_pointer(expectedHeader,offsetof(RroModelBank,vertices),resolved(arena,relative(input,1)));
 header_pointer(expectedHeader,offsetof(RroModelBank,normals),resolved(arena,relative(input,2)));
 for(unsigned i=0;i<count;i++)header_pointer(expectedHeader,offsetof(RroModelBank,streams)+i*sizeof(RroBankCommand),resolved(arena,relative(input,i+3)));
 for(unsigned phase=0;phase<5;phase++) {
  if(phase==1) {
   SetupRenderer(shift);wanted.scratch.value.scene.depthShift60=shift;memcpy(&wanted.scratch.value.scene.renderFlags64,&flags,4);
   wanted.scratch.value.scene.viewport6C.rect.x=0;wanted.scratch.value.scene.viewport6C.rect.y=0;
   wanted.scratch.value.scene.viewport6C.rect.w=320;wanted.scratch.value.scene.viewport6C.rect.h=256;
   wanted.color.flat=(GpuColorCommand){0};memcpy(&wanted.color.flat,"\200\200\200\054",4);
   memcpy(&wanted.color.gouraud,"\377\377\377\074",4);
  }else if(phase==2) {
   SelectModelBank((s32)slot);wanted.scratch.value.scene.vertices54=(const Vec3s8*)resolved(arena,relative(input,1));
   wanted.scratch.value.scene.normals58=(const Vec3s8*)resolved(arena,relative(input,2));
   wanted.scratch.value.scene.commands50=bank->streams;wanted.count.value=(s32)count;
   assert((uintptr_t)live.scratch.value.scene.commands50>UINT32_MAX);
  }else if(phase==3)PatchModelBankTexPages(bank,textureSlot);
  else if(phase==4) {
   /* The real dispatcher consumes the actual selected and patched bank.
    * Services remain observable boundaries; none substitutes bank logic. */
   events=expectedEvents=colors=0;memset(actualEvents,0,sizeof(actualEvents));memset(wantedEvents,0,sizeof(wantedEvents));
   unsigned object=count?(scenario/13)%count:0;live.page=count?(s32)(scenario%32):-1;wanted.page=live.page;
   s32 drawFlags=(s32)(scenario*0x17171717U);
   if(count) {
    wantedEvents[expectedEvents++]=(Event){1,0,0};
    const u32 *cursor=(const u32*)resolved(arena,relative(input,object+3));
    while(*cursor) {
     u32 word=*cursor++,command=word&65535U,records=word>>16;
     if(command==2)wantedEvents[expectedEvents++]=(Event){2,cursor,(s32)records};
     if(command==3){wantedEvents[expectedEvents++]=(Event){4,0,0};wantedEvents[expectedEvents++]=(Event){3,cursor,(s32)records};}
     assert(command<4&&expectedEvents<=16);cursor+=records*(command==0?4U:command==3?8U:6U);
    }
    RroRenderWork *work=&wanted.scratch.value.work7C.render;u32 page=(u32)live.page;
    work->flags08=drawFlags;work->envClut0C=((((page&16U)*16U+127U)*64U)|((page&15U)*4U)|3U)<<16;
    work->envTpage10=(0x20U|(page&31U))<<16;work->vertices00=wanted.scratch.value.scene.vertices54;
    work->normals04=wanted.scratch.value.scene.normals58;work->ordering14=live.order+128;
   }
   assert(DrawRroModel(&live.position,&live.matrix,(s32)object,drawFlags)==(count?1:0));
   assert(events==expectedEvents&&!memcmp(actualEvents,wantedEvents,events*sizeof(Event)));
   UnrelocateModelBank(bank,arena+BANK_OFFSET+delta);
   for(unsigned field=1;field<count+3;field++) {
    u32 bits=relative(input,field)-(u32)delta;
    size_t at=field==1?offsetof(RroModelBank,vertices):field==2?offsetof(RroModelBank,normals):offsetof(RroModelBank,streams)+(field-3)*sizeof(RroBankCommand);
    memcpy(expectedHeader+at,&bits,4);
   }
  }
  assert(!memcmp(arena+BANK_OFFSET,expectedHeader,BANK_BYTES)&&!memcmp(&live,&wanted,sizeof(live)));
  for(unsigned i=0;i<16;i++)assert(allocation[i]==0xA6&&arena[ARENA_BYTES+i]==0xA6);
  snapshot(output+phase*SNAPSHOT,arena,input,bank,count,slot,phase);
 }
 free(allocation);
}
'''


def fixture(seed):
    rng = random.Random(seed ^ 0x70108)
    data = bytearray(rng.randbytes(SIZE))
    count = seed % 13
    vertex, normal = (16 if seed & 1 else 512), (640 if seed & 2 else 32)
    struct.pack_into('<III', data, 128, count, (vertex - 128) & 0xFFFFFFFF, (normal - 128) & 0xFFFFFFFF)
    for i in range(CAPACITY):
        start = 1024 + ((seed + i) % 4) * 512
        struct.pack_into('<I', data, 140 + i * 4, start - 128)
    for stream in range(4):
        at = 1024 + stream * 512
        for command in range(4):
            records = 1 + (seed + stream + command) % 3
            struct.pack_into('<I', data, at, command | records << 16)
            at += 4
            for record in range(records):
                if command == 3:
                    struct.pack_into('<H', data, at + 22, (0x8123 if (seed + stream + record) & 1 else 0x7123))
                at += (16, 24, 24, 32)[command]
        struct.pack_into('<I', data, at, 0)
    slot = (seed // 13) % 16
    texture_slot = SLOTS[(seed // (13 * 16)) % len(SLOTS)]
    shift = (-2147483648, -65536, -1, 0, 1, 31, 32, 2147483647)[seed % 8]
    flags = (seed * 0x9E3779B9) & 0xFFFFFFFF
    delta = (-64, 0, 64)[seed % 3]
    return bytes(data), slot, texture_slot, shift, flags, delta


def oracle(data, slot, texture_slot, shift, flags, delta):
    arena = bytearray(data)
    count, = struct.unpack_from('<I', arena, 128)
    scratch = bytearray([0xA5] * 124)
    struct.pack_into('<II', scratch, 0, 0x80204000, 0x80203000)
    table = bytearray([0xA5] * 96)
    table[16:80] = bytes(64)
    colors, format_owner = bytearray([0xA5] * 44), bytearray([0xA5] * 36)
    snapshots, trace = [], []
    banks, model_format = address_of('ModelBankTable'), address_of('ModelBankFormat')
    flat, gouraud = address_of('DefaultScreenClearColor'), address_of('LoadingScreenClearColor')
    def put(target, at, size, value, address):
        value &= (1 << (8 * size)) - 1
        target[at:at + size] = value.to_bytes(size, 'little')
        trace.append((address, size, value))
    def bank_word(field):
        at = 128 + field * 4
        return at, struct.unpack_from('<I', arena, at)[0]
    for phase in range(5):
        trace = []
        if phase == 0:
            put(table, 16 + slot * 4, 4, BANK, banks + slot * 4)
            for field in range(1, count + 3):
                at, old = bank_word(field)
                put(arena, at, 4, old + BANK, ARENA + at)
        elif phase == 1:
            for offset, value in ((0x60, shift), (0x64, flags)):
                put(scratch, offset, 4, value, SCRATCH + offset)
            for address, offset, value in ((flat + 2, 26, 128), (flat + 1, 25, 128), (flat, 24, 128), (flat + 3, 27, 44),
                                           (gouraud + 2, 18, 255), (gouraud + 1, 17, 255), (gouraud, 16, 255), (gouraud + 3, 19, 60)):
                put(colors, offset, 1, value, address)
            for offset, value in ((0x70, 320), (0x6C, 0), (0x6E, 0), (0x72, 256)):
                put(scratch, offset, 2, value, SCRATCH + offset)
        elif phase == 2:
            for offset, field in ((0x54, 1), (0x58, 2)):
                put(scratch, offset, 4, bank_word(field)[1], SCRATCH + offset)
            put(format_owner, 16, 4, count, model_format)
            put(scratch, 0x50, 4, BANK + 12, SCRATCH + 0x50)
        elif phase == 3:
            x = ((texture_slot & 0xFFFFFFFF) * 64 + 384) & 0xFFFFFFFF
            page, clut, uv_add, special_clut = (x >> 6) & 15, (x >> 4) & 63, (texture_slot * 64) & 65535, (texture_slot + 4136) & 65535
            for i in range(count):
                at = bank_word(i + 3)[1] - ARENA
                while True:
                    header, = struct.unpack_from('<I', arena, at)
                    at += 4
                    if not header:
                        break
                    command, records = header & 65535, header >> 16
                    for record in range(records):
                        start = at + record * (16, 24, 24, 32)[command]
                        def patch(field, mask, value):
                            old, = struct.unpack_from('<H', arena, start + field)
                            put(arena, start + field, 2, (old & mask) | value, ARENA + start + field)
                        if command == 1:
                            patch(14, 0xFFE0, page)
                            patch(10, 0xBFC3, clut)
                        elif command == 3:
                            old_page, = struct.unpack_from('<H', arena, start + 22)
                            if old_page & 32768:
                                put(arena, start + 22, 2, 10, ARENA + start + 22)
                                for field in (16, 20, 24, 26):
                                    patch(field, 0xFF3F, uv_add)
                                patch(18, 0xBFC3, special_clut)
                            else:
                                patch(22, 0xFFE0, page)
                                patch(18, 0xBFC3, clut)
                            patch(30, 65535, 0x78)
                    at += records * (16, 24, 24, 32)[command]
        else:
            for field in range(1, count + 3):
                at, old = bank_word(field)
                put(arena, at, 4, old - (BANK + delta), ARENA + at)
        snapshots.append(bytes(arena + scratch + table + colors + format_owner))
        yield snapshots[-1], trace


class ModelBankLifecycleTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        cls.directory = Path(cls.temp.name)
        owners = source_owners(ROOT / 'src/main/PAL')
        bodies = definition_source(owners['RegisterModelBank'].read_text(), 'RroModelBankAddress')
        bodies += '\n'.join(definition_source(owners[name].read_text(), name) for name in NAMES)
        bodies += definition_source(owners['DrawRroModel'].read_text(), 'DrawRroModel')
        bodies = re.sub(r'\s+(?:__asm__|asm)\("\$\d+"\)', '', bodies)
        operation = 'RR_GTE_MTC2_RGB(0x3C808080);'
        assert bodies.count(operation) == 1
        bodies = bodies.replace(operation, 'TestSetColor(0x3C808080);')
        source, library = cls.directory / 'native.c', cls.directory / 'native.dylib'
        source.write_text(PREFIX + bodies + TAIL)
        result = subprocess.run(['cc', '-std=gnu11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                                 '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-I', str(ROOT / 'include'),
                                 str(source), '-o', str(library)], capture_output=True, text=True)
        if result.returncode:
            raise AssertionError(result.stderr)
        cls.library = C.CDLL(str(library))
        cls.native = cls.library.run_case
        cls.native.argtypes = [C.c_uint32, C.c_void_p, C.c_uint32, C.c_int32, C.c_int32, C.c_uint32, C.c_int32, C.c_void_p]
        cls.native.restype = None
        cls.owners = owners

    def test_native_complete_lifecycle_and_real_dispatcher(self):
        # Every stream-count/bank-slot/texture-slot combination, plus signed
        # shift extremes, backward offsets, repeated streams and both page paths.
        for seed in range(13 * 16 * len(SLOTS)):
            data, slot, texture, shift, flags, delta = fixture(seed)
            wanted = b''.join(snapshot for snapshot, _ in oracle(data, slot, texture, shift, flags, delta))
            output = C.create_string_buffer(5 * SNAPSHOT)
            self.native(seed, C.create_string_buffer(data), slot, texture, shift, flags, delta, output)
            self.assertEqual(output.raw, wanted, seed)

    def test_original_and_compiled_pal_lifecycle_stores_and_abi(self):
        compiled_sections = []
        for owner in sorted({self.owners[name] for name in NAMES}):
            report = verify(owner, self.directory / owner.stem)
            self.assertTrue(report['rodata_identical'] and report['text_extent_correct'])
            self.assertTrue(all(r['levenshtein'] == 0 and r['address_correct'] for r in report['functions']))
            with (self.directory / owner.stem / (owner.stem + '.elf')).open('rb') as stream:
                elf = ELFFile(stream)
                text = elf.get_section_by_name('.text')
                compiled_sections.append((text['sh_addr'], text.data()))
        image = (ROOT / 'assets/PAL/main.exe').read_bytes()
        load, = struct.unpack_from('<I', image, 0x18)
        for variant, engine in ((v,e) for v in ('original','compiled') for e in ('instruction_runner','unicorn')):
            memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
            memory.mem_map(0, 0x400000)
            memory.mem_map(SCRATCH, 4096)
            write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b))
            read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
            write(load, image[0x800:])
            if variant == 'compiled':
                for address, code in compiled_sections:
                    write(address, code)
            stores = []
            def store(cpu, access, address, size, value, user):
                normalized = address if address >= SCRATCH else (address & 0x1FFFFFFF) | 0x80000000
                stores.append((normalized, size, value & ((1 << (size * 8)) - 1)))
            # The established instruction runner observes exact stores without
            # Unicorn's write-hook exception on the original store-delay-slot/JR
            # sequence. An independent uninstrumented Unicorn execution must
            # produce the same complete states and ABI on every fixture too.
            cpu = Cpu(memory,0,100,101,102,store) if engine=='instruction_runner' else None
            for seed in range(13 * 16):
                data, slot, texture, shift, flags, delta = fixture(seed * len(SLOTS))
                expected = list(oracle(data, slot, texture, shift, flags, delta))
                table = bytes([0xA5] * 16) + bytes(64) + bytes([0xA5] * 16)
                write(ARENA - 16, bytes([0xA6] * 16) + data + bytes([0xA6] * 16))
                scratch = bytearray([0xA5] * 512)
                struct.pack_into('<II', scratch, 0, 0x80204000, 0x80203000)
                write(SCRATCH, scratch)
                write(address_of('ModelBankTable') - 16, table)
                write(address_of('ModelBankFormat') - 16, bytes([0xA5] * 36))
                write(address_of('LoadingScreenClearColor') - 16, bytes([0xA5] * 44))
                write(address_of('MirrorRenderFlag') - 16, bytes([0xA7] * 16) + struct.pack('<I', flags) + bytes([0xA7] * 16))
                write(STACK - 64, bytes([0xAC] * 128))
                args = [(BANK, slot), (shift & 0xFFFFFFFF,), (slot,), (BANK, texture & 0xFFFFFFFF), (BANK, BANK + delta)]
                for phase, name in enumerate(NAMES):
                    initial = [0] + [0xBEEF0000+r for r in range(1,32)]
                    for register, value in enumerate(args[phase],4):initial[register]=value
                    initial[29],initial[31]=STACK,STOP
                    stores.clear()
                    if cpu is not None:
                        cpu.reset();cpu.r=initial;cpu.pc=address_of(name)
                        for _ in range(200000):
                            if cpu.pc==STOP:break
                            cpu.step()
                        pc=cpu.pc
                        register_value=lambda r:cpu.r[r]
                        self.assertEqual(stores,expected[phase][1],(variant,engine,seed,name))
                    else:
                        for register in range(1,32):memory.reg_write(UC_MIPS_REG_0+register,initial[register])
                        memory.emu_start(address_of(name),STOP,count=200000)
                        pc=memory.reg_read(UC_MIPS_REG_PC)
                        register_value=lambda r:memory.reg_read(UC_MIPS_REG_0+r)
                    self.assertEqual(pc,STOP,(variant,engine,seed,name))
                    actual = (read(ARENA, SIZE) + read(SCRATCH, 124) + read(address_of('ModelBankTable') - 16, 96)
                              + read(address_of('LoadingScreenClearColor') - 16, 44) + read(address_of('ModelBankFormat') - 16, 36))
                    self.assertEqual(actual, expected[phase][0], (variant, engine, seed, name))
                    self.assertEqual(read(ARENA - 16, 16) + read(ARENA + SIZE, 16), bytes([0xA6] * 32))
                    self.assertEqual(read(SCRATCH + 124, 512 - 124), bytes([0xA5] * (512 - 124)))
                    self.assertEqual(read(STACK - 64, 128), bytes([0xAC] * 128))
                    self.assertEqual(read(address_of('MirrorRenderFlag') - 16, 36), bytes([0xA7] * 16) + struct.pack('<I', flags) + bytes([0xA7] * 16))
                    for register in (*range(16, 24), 28, 30):
                        self.assertEqual(register_value(register), 0xBEEF0000 + register)
                    self.assertEqual(register_value(29), STACK)
                    self.assertEqual(register_value(31), STOP)


if __name__ == '__main__':
    unittest.main()
