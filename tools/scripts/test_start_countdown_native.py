"""Actual countdown body: complete frame owners, mutable services and MIPS ABI.

GPU emitters are instrumented services. Their packet implementations have
separate fixtures; this fixture checks the countdown's real public interface,
data lifetimes, complete native pointers and every service entry/exit.
"""
import ctypes as C
import os
from pathlib import Path
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32

from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
STACK, STOP = 0x803F0000, 0x803FF000
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "game/race/racegrid.h"
#include "game/ui/badgeprim.h"
#include "game/ui/badge1.h"
#include "rr/frame_storage_types.h"
#include "rr/gpu_scratch_types.h"
typedef struct {u32 before[4];FrameStorage value;u32 after[4];} FrameOwner;
typedef struct {u32 before[4];u8 bytes[1024];u32 after[4];} ArenaOwner;
typedef struct {u32 before[4],values[4],after[4];} HeadOwner;
typedef struct {u32 before[4];UiUvRect glyphs[8];u16 background[2],palettes[4];u8 offsets[4];u32 after[4];} RomOwner;
typedef struct {u32 before[4];s32 value;u32 after[4];} WordOwner;
typedef struct {u32 before[4];s16 value;u16 spare;u32 after[4];} HalfOwner;
typedef struct {FrameOwner frames[2];ArenaOwner arenas[3];HeadOwner heads;RomOwner rom;WordOwner race;HalfOwner mode;} World;
enum {MAX_EVENTS=48,WIRE_SIZE=sizeof(World)+12};
typedef struct {u8 entry[WIRE_SIZE],after[WIRE_SIZE];u32 op,args[7],result;u8 uv[4],uvAfter[4];} Event;
typedef struct {Event events[MAX_EVENTS];u8 final[WIRE_SIZE];unsigned count;} Capture;
static World live,initial;
static Capture actual,expected,*capture;
static GpuScratchPointers scratch;
FrameStorage *CurrentDrawBuf;
static unsigned scenario,serial,frameBank,orderKind;
static s32 inputFrame;
static unsigned branches[7],services[4],mutations[2],orders[4],races[2],modes[2],banks[2];
static const s32 times[]={INT32_MIN,INT32_MIN+74,INT32_MIN+75,-1000000,-100,-76,-75,-74,-26,-25,-24,-1,0,1,24,25,49,50,51,74,75,76,81,82,99,100,101,124,125,149,150,174,175,199,200,224,225,249,250,251,INT32_MAX};
static const s32 raceValues[]={0,1,-1,INT32_MIN,INT32_MAX};
static const s16 modeValues[]={1,0,2,-1,INT16_MIN,INT16_MAX};
#define PauseMenuLabelUvs (live.rom.glyphs)
#define PauseTitleClutIds (live.rom.background)
#define PauseRowClutIds (live.rom.palettes)
#define PauseRowOffsetX (live.rom.offsets)
#define RaceState (live.race.value)
#define GameModeType (live.mode.value)
#define SCRATCHPAD (&scratch)
static u32 next(u32 *seed){u32 n=*seed;n^=n<<13;n^=n>>17;n^=n<<5;return *seed=n;}
static const void *region(unsigned n){
 if(n<2)return &live.frames[n];if(n<5)return &live.arenas[n-2];
 if(n==5)return &live.heads;if(n==6)return &live.rom;if(n==7)return &live.race;assert(n==8);return &live.mode;
}
static unsigned regionSize(unsigned n){return n<2?sizeof(FrameOwner):n<5?sizeof(ArenaOwner):n==5?sizeof(HeadOwner):n==6?sizeof(RomOwner):n==7?sizeof(WordOwner):sizeof(HalfOwner);}
static u32 address(unsigned n){
 if(n<2)return 0x80180000U+n*0x30000U;if(n<5)return 0x80280000U+(n-2)*0x10000U;
 if(n==5)return 0x802B0000U;if(n==6)return 0x8009F69CU;
 /* Actual scalar addresses are supplied from the symbol registry. */
 return n==7?RR_RACE_ADDRESS-16U:RR_MODE_ADDRESS-16U;
}
static u32 pointer(const void *p){
 uintptr_t q=(uintptr_t)p;for(unsigned n=0;n<9;n++){uintptr_t b=(uintptr_t)region(n);if(q>=b&&q<b+regionSize(n))return address(n)+(u32)(q-b);}
 assert(!"unowned native pointer");return 0;
}
static void snapshot(u8 *out){
 memcpy(out,&live,sizeof(live));out+=sizeof(live);
 u32 values[3]={pointer(CurrentDrawBuf),pointer(scratch.cursor),pointer(scratch.order)};memcpy(out,values,sizeof(values));
}
static u32 *ordering(void){
 if(orderKind==0)return live.frames[frameBank].value.buffer.ordering70.words;
 if(orderKind==1)return live.frames[frameBank].value.buffer.secondaryOrderingB70.words;
 if(orderKind==2)return &live.heads.values[scenario%4];
 return (u32 *)(live.arenas[scenario%3].bytes+64);
}
static Event *begin(unsigned op,u32 *head,void *cursor){
 assert((uintptr_t)head>UINT32_MAX&&(uintptr_t)cursor>UINT32_MAX);
 unsigned n=capture->count++;assert(n<MAX_EVENTS);Event *e=&capture->events[n];
 memset(e->args,0,sizeof(e->args));memset(e->uv,0,4);memset(e->uvAfter,0,4);
 e->op=op;e->args[0]=pointer(head);e->args[1]=pointer(cursor);snapshot(e->entry);services[op]++;return e;
}
static void *finish(Event *e,UiUvRect *uv){
 unsigned n=serial++;
 /* Distinct returned and scratch cursors expose accidental reloads/caching. */
 live.arenas[n%3].bytes[960]^=(u8)(scenario+n);
 if(scenario&1){
  u8 *rom=(u8 *)&live.rom.glyphs;for(unsigned i=0;i<48;i++)rom[i]=(u8)(scenario*37U+n*17U+i*11U);
  RaceState=(s32)(scenario^n);GameModeType=(s16)(n%3);
  CurrentDrawBuf=&live.frames[(scenario+n)%2].value;
  scratch.cursor=live.arenas[(n+1)%3].bytes+((n*53U+20U)%800U&~3U);
  scratch.order=&live.heads.values[n%4];
  if(uv){uv->u^=0x31;uv->v^=0x72;uv->w^=0x13;uv->h^=0x54;}
 }
 void *result=live.arenas[(n+2)%3].bytes+((n*97U+40U)%800U&~3U);
 assert((uintptr_t)result>UINT32_MAX);e->result=pointer(result);
 if(uv)memcpy(e->uvAfter,uv,4);snapshot(e->after);return result;
}
void *AddDrawAreaPrim(u32 *head,void *cursor,s32 x,s32 y,s32 width,s32 height){
 Event *e=begin(1,head,cursor);e->args[2]=x;e->args[3]=y;e->args[4]=width;e->args[5]=height;return finish(e,0);
}
void *AddDrawModePrim(u32 *head,GpuDrawModePacket *packet,u16 page){Event *e=begin(2,head,packet);e->args[2]=page;return finish(e,0);}
void *AddSpritePrim(u32 *head,void *cursor,s32 x,s32 y,UiUvRect *uv,u16 clut,s32 flags){
 Event *e=begin(3,head,cursor);e->args[2]=x;e->args[3]=y;
 uintptr_t q=(uintptr_t)uv,b=(uintptr_t)PauseMenuLabelUvs;
 e->args[4]=q>=b&&q<b+32?pointer(uv):0;memcpy(e->uv,uv,4);e->args[5]=clut;e->args[6]=flags;return finish(e,uv);
}
'''
REFERENCE = r'''
static void reference(void){
 u32 *head=ordering();s32 delta=(s32)((u32)inputFrame-75U);
 if(!RaceState||delta>=175)return;
 s32 tick=inputFrame%25,phase=delta/25;
 unsigned branch=phase<0?0:phase>5?6:(unsigned)phase;branches[branch]++;
 s32 top=0;
 if(head==CurrentDrawBuf->buffer.ordering70.words){if(phase==6)top=-(tick*128)/25;}
 else{scratch.cursor=AddDrawAreaPrim(head,scratch.cursor,0,0,320,256);top=64;}
 void *cursor=AddDrawModePrim(head,(GpuDrawModePacket *)scratch.cursor,5);
 s32 intensity=phase==1?tick*64/25+64:phase>=5?128:phase>=4?164:phase<=0?1:128;
 unsigned palette=0;UiUvRect glyph;
 if(GameModeType==1){
  if(phase>=4){palette=((inputFrame>>2)&1)+2;glyph=PauseMenuLabelUvs[7];cursor=AddSpritePrim(head,cursor,PauseRowOffsetX[3]+96,top+57,&glyph,PauseRowClutIds[palette],0);}
  else if(phase>0){
   s32 offset=0;for(int i=0;i<16&&i<=tick-7;i++){unsigned parity=i&1;cursor=AddSpritePrim(head,cursor,offset+120,top+57,&PauseMenuLabelUvs[2+parity],PauseRowClutIds[parity*2],0);offset+=1<<(parity*2+1);}
   glyph=PauseMenuLabelUvs[6];glyph.u+=(phase-1)*12;cursor=AddSpritePrim(head,cursor,PauseRowOffsetX[2]+96,top+57,&glyph,PauseRowClutIds[0],0);
  }
 }else if(phase==0){
  s32 offset=0;for(int i=0;i<16&&i<=tick-6;i++){unsigned parity=i&1;cursor=AddSpritePrim(head,cursor,offset+120,top+57,&PauseMenuLabelUvs[2+parity],PauseRowClutIds[parity*2],0);offset+=1<<(parity*2+1);}
 }else if(phase>=4){
  palette=((inputFrame>>2)&1)+2;glyph=PauseMenuLabelUvs[5];glyph.u+=96;
  cursor=AddSpritePrim(head,cursor,PauseRowOffsetX[0]+96,top+57,&glyph,PauseRowClutIds[palette],0);
  cursor=AddSpritePrim(head,cursor,PauseRowOffsetX[1]+96,top+57,&glyph,PauseRowClutIds[palette],0);
  glyph=PauseMenuLabelUvs[7];cursor=AddSpritePrim(head,cursor,PauseRowOffsetX[3]+96,top+57,&glyph,PauseRowClutIds[palette],0);
 }else{
  glyph=PauseMenuLabelUvs[5];glyph.u+=tick*9/25*12;cursor=AddSpritePrim(head,cursor,PauseRowOffsetX[0]+96,top+57,&glyph,PauseRowClutIds[0],0);
  glyph.u=PauseMenuLabelUvs[5].u+phase*24;cursor=AddSpritePrim(head,cursor,PauseRowOffsetX[1]+96,top+57,&glyph,PauseRowClutIds[0],0);
  glyph=PauseMenuLabelUvs[6];glyph.u+=(phase-1)*12;cursor=AddSpritePrim(head,cursor,PauseRowOffsetX[2]+96,top+57,&glyph,PauseRowClutIds[0],0);
 }
 if(phase>0)for(int i=0;i<10;i++)cursor=AddSpritePrim(head,cursor,120+i*8,top+57,&PauseMenuLabelUvs[4],PauseRowClutIds[i>=3&&i<7?3:palette],0);
 cursor=AddSpritePrim(head,cursor,104,top+26,&PauseMenuLabelUvs[1],PauseTitleClutIds[phase>=4],intensity*2+1);
 cursor=AddSpritePrim(head,cursor,96,top,&PauseMenuLabelUvs[0],0x797C,0);
 scratch.cursor=AddDrawModePrim(head,(GpuDrawModePacket *)cursor,63);
 if(head==CurrentDrawBuf->buffer.secondaryOrderingB70.words){s32 height=phase==6?(25-tick)*24/25:24;scratch.cursor=AddDrawAreaPrim(head,scratch.cursor,112,top+(24-height)/2+52,96,height);}
}
static void initialize(unsigned n){
 scenario=n;serial=0;frameBank=n/41%2;orderKind=n/82%4;
 u32 seed=0x34BCD123U+n*0x9E3779B9U;u8 *p=(u8 *)&live;
 for(unsigned i=0;i<sizeof(live);i++)p[i]=(u8)next(&seed);
 inputFrame=times[n%41];if(n&8192)inputFrame=(s32)next(&seed);
 RaceState=raceValues[n/328%5];GameModeType=modeValues[n/1640%6];
 CurrentDrawBuf=&live.frames[frameBank].value;scratch.cursor=live.arenas[n%3].bytes+64;scratch.order=&live.heads.values[n%4];
 memcpy(&initial,&live,sizeof(live));mutations[n&1]++;orders[orderKind]++;races[RaceState!=0]++;modes[GameModeType==1]++;banks[frameBank]++;
}
unsigned exercise_native(void){
 _Static_assert(sizeof(FrameStorage)==0x22778,"complete frame owner");
 _Static_assert(offsetof(RomOwner,offsets)==60&&sizeof(RomOwner)==80,"one contiguous ROM owner");
 _Static_assert(sizeof(World)==2*sizeof(FrameOwner)+3*sizeof(ArenaOwner)+sizeof(HeadOwner)+sizeof(RomOwner)+sizeof(WordOwner)+sizeof(HalfOwner),"wire prefix has no hidden padding");
 for(unsigned n=0;n<16384;n++){
  initialize(n);capture=&expected;capture->count=0;reference();snapshot(capture->final);
  initialize(n);capture=&actual;capture->count=0;DrawStartCountdown(ordering(),inputFrame);snapshot(capture->final);
  assert(actual.count==expected.count);
  for(unsigned i=0;i<actual.count;i++)assert(!memcmp(&actual.events[i],&expected.events[i],sizeof(Event)));
  assert(!memcmp(actual.final,expected.final,WIRE_SIZE));
 }
 for(unsigned i=0;i<7;i++)assert(branches[i]);for(unsigned i=1;i<4;i++)assert(services[i]);
 for(unsigned i=0;i<2;i++)assert(mutations[i]&&races[i]&&modes[i]&&banks[i]);for(unsigned i=0;i<4;i++)assert(orders[i]);return 16384;
}
unsigned prepare_trace(unsigned n){initialize(n);capture=&expected;capture->count=0;reference();snapshot(capture->final);return capture->count;}
unsigned wire_size(void){return WIRE_SIZE;}
unsigned region_size(unsigned n){return regionSize(n);}u32 region_address(unsigned n){return address(n);}
const void *initial_world(void){return &initial;}
const void *final_wire(void){return capture->final;}
const void *event_wire(unsigned n,unsigned after){assert(n<capture->count);return after?capture->events[n].after:capture->events[n].entry;}
const void *event_uv(unsigned n,unsigned after){return after?capture->events[n].uvAfter:capture->events[n].uv;}
u32 event_value(unsigned n,unsigned field){Event *e=&capture->events[n];return field==0?e->op:field==8?e->result:e->args[field-1];}
u32 argument(unsigned n){return n?(u32)inputFrame:pointer(ordering());}
u32 initial_pointer(unsigned n){return n==0?pointer(&live.frames[frameBank].value):n==1?pointer(live.arenas[scenario%3].bytes+64):pointer(&live.heads.values[scenario%4]);}
'''


class StartCountdownNativeTest(unittest.TestCase):
    def test_full_pointer_callers_and_original(self):
        owner = source_owners(ROOT/'src/main/PAL')['DrawStartCountdown']
        body = definition_source(owner.read_text(), 'DrawStartCountdown')
        addresses = [address_of('RaceState'), address_of('GameModeType')]
        defines = f'#define RR_RACE_ADDRESS {addresses[0]}U\n#define RR_MODE_ADDRESS {addresses[1]}U\n'
        with TemporaryDirectory() as temporary:
            source, library = Path(temporary)/'native.c', Path(temporary)/'native.dylib'
            source.write_text(defines+PREFIX+body+REFERENCE)
            subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                            '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-I', str(ROOT/'include'),
                            str(source), '-o', str(library)], check=True)
            lib = C.CDLL(str(library))
            self.assertEqual(lib.exercise_native(), 16384)
            for name in ['initial_world', 'final_wire', 'event_wire', 'event_uv']:
                getattr(lib, name).restype = C.c_void_p
            for name in ['region_address', 'region_size', 'wire_size', 'event_value', 'argument', 'initial_pointer']:
                getattr(lib, name).restype = C.c_uint32
            regions = [(lib.region_address(i), lib.region_size(i)) for i in range(9)]
            prefix = sum(size for _, size in regions)
            self.assertEqual(lib.wire_size(), prefix+12)
            pointer_regions = [(address_of('CurrentDrawBuf'), 4), (0x1F800000, 8)]
            normalized = [(a & 0x1FFFFFFF, n) for a, n in regions+pointer_regions]
            for i, (a, n) in enumerate(normalized):
                for b, m in normalized[i+1:]:self.assertTrue(a+n <= b or b+m <= a, (hex(a), hex(b)))
            memory = Uc(UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN | UC_MODE_MIPS32)
            memory.mem_map(0, 0x400000);memory.mem_map(0x1F800000, 0x1000)
            read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
            write = lambda a, data: memory.mem_write(a & 0x1FFFFFFF, bytes(data))
            original = (ROOT/'assets/PAL/main.exe').read_bytes();base = struct.unpack_from('<I', original, 24)[0]
            write(base, original[0x800:])
            if os.getenv('RR_COMPILED_ELF'):
                with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                    elf = ELFFile(stream);symbol = elf.get_section_by_name('.symtab').get_symbol_by_name('DrawStartCountdown')[0]
                    section = elf.get_section(symbol['st_shndx']);offset = symbol['st_value']-section['sh_addr']
                    self.assertEqual(symbol['st_value'], address_of('DrawStartCountdown'))
                    write(symbol['st_value'], section.data()[offset:offset+symbol['st_size']])

            def set_world(data):
                offset = 0
                for a, n in regions+pointer_regions:write(a, data[offset:offset+n]);offset += n

            def world():return b''.join(read(a, n) for a, n in regions+pointer_regions)

            def store(cpu, access, address, size, value, user):
                a = address & 0x1FFFFFFF
                self.assertTrue(any(b <= a and a+size <= b+n for b, n in normalized+[(STACK-128 & 0x1FFFFFFF, 128)]), (hex(address), size))

            cpu = Cpu(memory, 0, 100, 101, 102, store)
            services = {address_of(name): (op, count) for name, op, count in
                        [('AddDrawAreaPrim', 1, 6), ('AddDrawModePrim', 2, 3), ('AddSpritePrim', 3, 7)]}
            for case in range(512):
                number = case*7919 & 16383;count = lib.prepare_trace(number)
                start = C.string_at(lib.initial_world(), prefix)+struct.pack('<3I', *(lib.initial_pointer(i) for i in range(3)))
                set_world(start);write(STACK-256, b'\xA5'*512)
                entry_stack = read(STACK, 256)
                cpu.reset();cpu.r = [0]+[0xBEEF0000+i for i in range(1, 32)]
                cpu.r[4:6] = [lib.argument(0), lib.argument(1)];cpu.r[29], cpu.r[31], cpu.pc = STACK, STOP, address_of('DrawStartCountdown')
                event = 0
                for instruction in range(15000):
                    if cpu.pc == STOP:break
                    if cpu.pc not in services:cpu.step();continue
                    op, argc = services[cpu.pc];self.assertLess(event, count);self.assertEqual(op, lib.event_value(event, 0))
                    args = cpu.r[4:4+min(argc, 4)]
                    if argc>4:args += list(struct.unpack('<'+'I'*(argc-4), read(cpu.r[29]+16, (argc-4)*4)))
                    expected_args = [lib.event_value(event, i+1) for i in range(argc)]
                    local = op == 3 and expected_args[4] == 0
                    if op == 3:
                        self.assertEqual(read(args[4], 4), C.string_at(lib.event_uv(event, 0), 4), (number, event, 'glyph'))
                    if local:
                        self.assertTrue(STACK-128 <= args[4] < STACK);expected_args[4] = args[4]
                    self.assertEqual(args, expected_args, (number, event, 'arguments'))
                    self.assertEqual(world(), C.string_at(lib.event_wire(event, 0), prefix+12), (number, event, 'entry'))
                    set_world(C.string_at(lib.event_wire(event, 1), prefix+12))
                    if local:write(args[4], C.string_at(lib.event_uv(event, 1), 4))
                    resume = cpu.r[31]
                    for register in (1, 2, 3, *range(4, 16), 24, 25):cpu.r[register] = 0xA1230000+event*257+register
                    cpu.r[2] = lib.event_value(event, 8);cpu.pc = resume;cpu.hi = 0x12345678;cpu.lo = 0x87654321;event += 1
                self.assertEqual(cpu.pc, STOP, number);self.assertEqual(event, count, number)
                self.assertEqual(world(), C.string_at(lib.final_wire(), prefix+12), (number, 'final'))
                self.assertEqual(cpu.r[29], STACK);self.assertEqual(cpu.r[31], STOP)
                for register in (*range(16, 24), 28, 30):self.assertEqual(cpu.r[register], 0xBEEF0000+register)
                self.assertEqual(read(STACK-256, 128), b'\xA5'*128);self.assertEqual(read(STACK, 256), entry_stack)
            print('PASS 16384 native and 512', 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
                  'MIPS countdown cases; complete frames/arenas/ROM, full pointers, mutable glyphs and callback owners, ABI guards and UBSan')
