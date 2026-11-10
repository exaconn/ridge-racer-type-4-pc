"""Actual controller-label callback and settings application with full owners.

The callback is invoked through the real PanelDraw type and returns a full
native cursor. Renderer/pad services are instrumented and may change live
rows, settings, controllers, glyphs and cursors between calls. The reference
decodes the original field offsets independently of the production records.
"""
import ctypes as C
import hashlib
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
NAMES = ['DrawControllerConfigPanel', 'ApplyOptionsPadButtonMaps']
SERVICES = ['AddSpritePrim', 'AddTextSprites', 'AddDrawModePrim', 'SetPadButtonMapLayout']
STACK, STOP = 0x803F0000, 0x803FF000
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "game/menu/mnlist.h"
#include "game/car/carstat.h"
#include "game/gfx/fontdraw.h"
#include "game/system/pad.h"
#include "rr/ui_panel.h"
#include "rr/runtime_settings.h"
typedef struct {u32 before[4];BadgeRow1C values[6];u32 after[4];} RowOwner;
typedef struct {u32 before[4];RuntimeSettings value;u32 after[4];} SettingsOwner;
/* The PAL BadgeRows pointer follows this controller bank. Eight trailing
 * guard bytes leave its separate global pointer slot disjoint. */
typedef struct {u32 before[4];ControllerState5C values[2];u32 after[2];} ControllerOwner;
typedef struct {u32 before[4];UiUvRect value;u32 after[4];} GlyphOwner;
typedef struct {u32 before[4];s32 value;u32 after[4];} CounterOwner;
typedef struct {u32 before[4],values[4],after[4];} HeadOwner;
typedef struct {u32 before[4],words[128],after[4];} ArenaOwner;
typedef struct {
 RowOwner rows[2];SettingsOwner settings;ControllerOwner controllers;GlyphOwner glyph;
 CounterOwner counter;HeadOwner heads;ArenaOwner arenas[2];BadgeRow1C *root;
} World;
typedef struct {World entry,after;u32 operation,args[8];u8 payload[8];u32 result;} Event;
typedef struct {World final;unsigned count;Event events[3];u32 result;} Capture;
enum {PREFIX_SIZE=1912,WIRE_SIZE=1916};
static World live,initial;static Capture actual,expected,*capture;
static unsigned scenario,kind,serial,rootIndex,headIndex,arenaIndex;
static s32 rowIndex;
static unsigned controllerSeen[2],protocolSeen[7],rootSeen[2],rowSeen[6],selectorSeen[10],layoutEdgeSeen[8],mutationSeen[2],serviceSeen[5];
static u8 firstWire[WIRE_SIZE],lastWire[WIRE_SIZE],eventWire[WIRE_SIZE];
static const s32 selectors[]={INT32_MIN,INT32_MIN+1,-3,-2,-1,0,1,2,3,INT32_MAX};
static const s32 protocols[]={INT32_MIN,-1,0,1,2,3,INT32_MAX};
static const s16 layoutEdges[]={INT16_MIN,INT16_MIN+1,-65,-1,0,1,65,INT16_MAX};
#define BadgeRows live.root
#define Options live.settings.value
#define PlayerControllers live.controllers.values
#define MenuStepArrowUv live.glyph.value
#define MenuAvailableCourseMask live.counter.value
/* The original table contains15 entries; slot12 names this resident callback. */
PanelDraw UiRowDrawHandlers[15]={[12]=DrawControllerConfigPanel};
static u32 next(u32 *seed){u32 n=*seed;n^=n<<13;n^=n>>17;n^=n<<5;return *seed=n;}
static s16 signed16(const void *p){s16 n;memcpy(&n,p,2);return n;}
static s32 signed32(const void *p){s32 n;memcpy(&n,p,4);return n;}
static u32 *head(void){return headIndex<4?&live.heads.values[headIndex]:&live.arenas[headIndex-4].words[120];}
static void *cursor(void){return &live.arenas[arenaIndex].words[120];}
static u32 pointer(const void *p){
 uintptr_t a=(uintptr_t)p;
 for(unsigned i=0;i<2;i++){
  uintptr_t b=(uintptr_t)&live.rows[i];if(a>=b&&a<b+sizeof(RowOwner))return 0x80180000U+i*4096U+(u32)(a-b);
  b=(uintptr_t)&live.arenas[i];if(a>=b&&a<b+sizeof(ArenaOwner))return 0x801A0000U+i*65536U+(u32)(a-b);
 }
 uintptr_t b=(uintptr_t)&live.heads;if(a>=b&&a<b+sizeof(HeadOwner))return 0x80190000U+(u32)(a-b);
 b=(uintptr_t)&live.settings;if(a>=b&&a<b+sizeof(SettingsOwner))return @OPTIONS@-16U+(u32)(a-b);
 b=(uintptr_t)&live.controllers;if(a>=b&&a<b+sizeof(ControllerOwner))return @CONTROLLERS@-16U+(u32)(a-b);
 b=(uintptr_t)&live.glyph;if(a>=b&&a<b+sizeof(GlyphOwner))return @GLYPH@-16U+(u32)(a-b);
 b=(uintptr_t)&live.counter;if(a>=b&&a<b+sizeof(CounterOwner))return @COUNTER@-16U+(u32)(a-b);
 assert(!"pointer outside complete native callback owners");return 0;
}
static void wire(const World *w,u8 *out){memcpy(out,w,PREFIX_SIZE);u32 root=pointer(w->root);memcpy(out+PREFIX_SIZE,&root,4);}
static void mutate(void){
 unsigned n=serial++;if(!(scenario&4))return;
 BadgeRows=&live.rows[(scenario+n)%2].values[2];
 Options.words[(scenario+n)%16]^=scenario*0x17171717U+n;
 for(unsigned i=0;i<2;i++){
  PlayerControllers[i].protocol10=protocols[(scenario+n+i)%7];
  Options.config.f0[i].buttonLayout00=layoutEdges[(scenario+n+i)%8];
  Options.config.p[i].buttonLayout0C=layoutEdges[(scenario+n+i+3)%8];
  Options.config.w[i].buttonLayout03=(s8)(scenario*37U+n+i);
 }
 for(unsigned i=0;i<4;i++)((u8*)&MenuStepArrowUv)[i]=(u8)(scenario*73U+n*17U+i);
 MenuAvailableCourseMask=selectors[(scenario+n)%10];
 live.heads.values[n%4]^=scenario*0x9E3779B9U+n;
 live.rows[n%2].values[(scenario+n)%6].x04=(s16)(scenario+n*37U);
}
static Event *begin(unsigned operation){
 unsigned index=capture->count++;assert(index<3);Event *e=&capture->events[index];memset(e,0,sizeof(*e));
 memcpy(&e->entry,&live,sizeof(live));e->operation=operation;serviceSeen[operation]++;return e;
}
static void *finish(Event *e,unsigned output){
 void *result=0;
 if(output){
  u32 *p=&live.arenas[(scenario/2+e->operation)%2].words[(scenario/8+e->operation*17U)%120];
  *p=scenario*0x1234567U+e->operation;result=p;e->result=pointer(p);
 }
 mutate();memcpy(&e->after,&live,sizeof(live));return result;
}
void *AddSpritePrim(u32 *ordering,void *packet,s32 x,s32 y,UiUvRect *region,u16 clut,s32 flags){
 Event *e=begin(1);u32 a[8]={pointer(ordering),pointer(packet),(u32)x,(u32)y,pointer(region),clut,(u32)flags,0};
 memcpy(e->args,a,sizeof(a));memcpy(e->payload,region,4);return finish(e,1);
}
void *AddTextSprites(u32 *ordering,void *packet,s32 x,s32 y,const char *text,s32 clut,s32 spacing,s32 color){
 assert((uintptr_t)text>UINT32_MAX);Event *e=begin(2);
 u32 a[8]={pointer(ordering),pointer(packet),(u32)x,(u32)y,0,(u32)clut,(u32)spacing,(u32)color};
 memcpy(e->args,a,sizeof(a));memcpy(e->payload,text,8);return finish(e,1);
}
void *AddDrawModePrim(u32 *ordering,GpuDrawModePacket *packet,u16 page){
 Event *e=begin(3);e->args[0]=pointer(ordering);e->args[1]=pointer(packet);e->args[2]=page;return finish(e,1);
}
void SetPadButtonMapLayout(s32 player,s32 standard,s32 calibrated,s32 wheel){
 Event *e=begin(4);e->args[0]=(u32)player;e->args[1]=(u32)standard;e->args[2]=(u32)calibrated;e->args[3]=(u32)wheel;(void)finish(e,0);
}
'''
REFERENCE = r'''
static void *referenceCallback(u32 *ordering,void *packet,s32 row){
 const u8 *record=(const u8*)(BadgeRows+row);
 s32 x=(s32)signed16(record+4)+8,y=(s32)signed16(record+6)+36;
 unsigned controller=((u32)MenuAvailableCourseMask-1U)%2U;controllerSeen[controller]++;
 s32 protocol=signed32((const u8*)&PlayerControllers+controller*92U+16U),layout;
 for(unsigned i=0;i<7;i++)if(protocol==protocols[i])protocolSeen[i]++;
 const u8 *settings=(const u8*)&Options;
 if(protocol==1)layout=signed16(settings+20U+controller*16U);
 else if(protocol==2){s8 n;memcpy(&n,settings+43U+controller*4U,1);layout=n;}
 else layout=signed16(settings+controller*4U);
 for(unsigned i=0;i<8;i++)if(layout==layoutEdges[i])layoutEdgeSeen[i]++;
 void *next=AddSpritePrim(ordering,packet,x,y,&MenuStepArrowUv,32760,0);
 char text[8]={0};text[0]=(char)(u8)((u32)layout+65U);
 next=AddTextSprites(ordering,next,x+2+((const u8*)&MenuStepArrowUv)[2],y,text,32760,0,0);
 return AddDrawModePrim(ordering,next,56);
}
static void referenceApply(void){
 for(unsigned player=0;player<2;player++){
  const u8 *settings=(const u8*)&Options;
  s32 a=signed16(settings+4U*player),b=signed16(settings+20U+16U*player);s8 c;
  memcpy(&c,settings+43U+4U*player,1);SetPadButtonMapLayout((s32)player,a,b,c);
 }
}
static void initialize(unsigned n){
 scenario=n;kind=n&1;serial=0;rowIndex=(s32)((n/2)%6)-2;rootIndex=(n/12)%2;headIndex=(n/24)%6;arenaIndex=(n/144)%2;
 u32 seed=n*0x9E3779B9U+0x42DBCU;u8 *p=(u8*)&live;for(unsigned i=0;i<sizeof(live);i++)p[i]=(u8)next(&seed);
 BadgeRows=&live.rows[rootIndex].values[2];MenuAvailableCourseMask=selectors[(n/288)%10];selectorSeen[(n/288)%10]++;
 for(unsigned i=0;i<2;i++){
  PlayerControllers[i].protocol10=protocols[(n/2880+i)%7];
  Options.config.f0[i].buttonLayout00=layoutEdges[(n/2+i)%8];
  Options.config.p[i].buttonLayout0C=layoutEdges[(n/16+i+3)%8];
  Options.config.w[i].buttonLayout03=(s8)(n*37U+i);
 }
 static const s16 coordinates[]={INT16_MIN,INT16_MIN+1,-1,0,1,INT16_MAX-1,INT16_MAX};
 for(unsigned owner=0;owner<2;owner++)for(unsigned row=0;row<6;row++){
  live.rows[owner].values[row].x04=coordinates[(n/4+owner+row)%7];
  live.rows[owner].values[row].y06=coordinates[(n/28+owner+row)%7];
 }
 rootSeen[rootIndex]++;rowSeen[rowIndex+2]++;mutationSeen[!!(n&4)]++;memcpy(&initial,&live,sizeof(live));
}
static u32 run(unsigned reference){
 if(kind){if(reference)referenceApply();else ApplyOptionsPadButtonMaps();return 0;}
 assert(UiRowDrawHandlers[12]==DrawControllerConfigPanel);
 void *p=reference?referenceCallback(head(),cursor(),rowIndex):UiRowDrawHandlers[12](head(),cursor(),rowIndex);
 assert((uintptr_t)p>UINT32_MAX);return pointer(p);
}
unsigned exercise_native(void){
 _Static_assert(sizeof(RuntimeSettings)==64&&sizeof(ControllerState5C)==92,"actual settings/controller owners");
 _Static_assert(sizeof(World)==1920&&offsetof(World,root)==PREFIX_SIZE,"complete native world and pointer");
 _Static_assert(sizeof(UiRowDrawHandlers)==15*sizeof(PanelDraw)&&sizeof(PanelDraw)==sizeof(void*),"full-width callback slots");
 for(unsigned n=0;n<65536;n++){
  initialize(n);assert((uintptr_t)BadgeRows>UINT32_MAX&&(uintptr_t)head()>UINT32_MAX&&(uintptr_t)cursor()>UINT32_MAX);
  capture=&expected;capture->count=0;capture->result=run(1);memcpy(&capture->final,&live,sizeof(live));
  memcpy(&live,&initial,sizeof(live));serial=0;capture=&actual;capture->count=0;capture->result=run(0);memcpy(&capture->final,&live,sizeof(live));
  assert(actual.count==expected.count&&actual.result==expected.result);
  for(unsigned i=0;i<actual.count;i++)assert(!memcmp(&actual.events[i],&expected.events[i],sizeof(Event)));
  assert(!memcmp(&actual.final,&expected.final,sizeof(World)));
 }
 for(unsigned i=0;i<2;i++)assert(controllerSeen[i]&&rootSeen[i]&&mutationSeen[i]);
 for(unsigned i=0;i<7;i++)assert(protocolSeen[i]);for(unsigned i=0;i<6;i++)assert(rowSeen[i]);
 for(unsigned i=0;i<10;i++)assert(selectorSeen[i]);for(unsigned i=0;i<8;i++)assert(layoutEdgeSeen[i]);
 for(unsigned i=1;i<5;i++)assert(serviceSeen[i]);return 65536;
}
unsigned prepare_trace(unsigned n){initialize(n);wire(&live,firstWire);capture=&expected;capture->count=0;capture->result=run(1);wire(&live,lastWire);return capture->count;}
const void *initial_wire(void){return firstWire;}const void *final_wire(void){return lastWire;}
const void *event_wire(unsigned i,unsigned after){assert(i<capture->count);wire(after?&capture->events[i].after:&capture->events[i].entry,eventWire);return eventWire;}
const void *event_payload(unsigned i){return capture->events[i].payload;}
u32 event_value(unsigned i,unsigned field){Event *e=&capture->events[i];return field==0?e->operation:field<9?e->args[field-1]:e->result;}
u32 call_argument(unsigned slot){return slot==0?pointer(head()):slot==1?pointer(cursor()):(u32)rowIndex;}
u32 result(void){return capture->result;}
'''


class ControllerPanelCallbackTest(unittest.TestCase):
    def test_real_callback_type_and_live_settings(self):
        owners = source_owners(ROOT/'src/main/PAL')
        bodies = '\n'.join(definition_source(owners[name].read_text(), name) for name in NAMES)
        prefix = PREFIX
        for token, name in [('OPTIONS', 'Options'), ('CONTROLLERS', 'PlayerControllers'), ('GLYPH', 'MenuStepArrowUv'), ('COUNTER', 'MenuAvailableCourseMask')]:
            prefix = prefix.replace('@'+token+'@', hex(address_of(name))+'U')
        original = (ROOT/'assets/PAL/main.exe').read_bytes();base = struct.unpack_from('<I', original, 24)[0]
        self.assertEqual(struct.unpack_from('<I', original, 0x800+address_of('UiRowDrawHandlers')+12*4-base)[0], address_of(NAMES[0]))
        with TemporaryDirectory() as temporary:
            source, library = Path(temporary)/'native.c', Path(temporary)/'native.dylib'
            source.write_text(prefix+bodies+REFERENCE)
            subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                            '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-I', str(ROOT/'include'),
                            str(source), '-o', str(library)], check=True)
            native = C.CDLL(str(library));native.exercise_native.restype = C.c_uint
            self.assertEqual(native.exercise_native(), 65536)
            for name in ['initial_wire', 'final_wire', 'event_wire', 'event_payload']:
                getattr(native, name).restype = C.c_void_p
            for name, count in [('prepare_trace', 1), ('event_wire', 2), ('event_value', 2), ('event_payload', 1), ('call_argument', 1)]:
                getattr(native, name).argtypes = [C.c_uint]*count
            for name in ['prepare_trace', 'event_value', 'call_argument', 'result']:
                getattr(native, name).restype = C.c_uint32
            # Freeze the complete generator/model matrix before future owner
            # migrations. PAL pointer tokens stay independent of host ASLR.
            inputs, traces = hashlib.sha256(), hashlib.sha256()
            for number in range(65536):
                events = native.prepare_trace(number)
                header = struct.pack('<6I', number, events, number & 1,
                                     *(native.call_argument(i) for i in range(3)))
                initial = C.string_at(native.initial_wire(), 1916)
                inputs.update(header+initial);traces.update(header+initial)
                for event in range(events):
                    traces.update(struct.pack('<10I', *(native.event_value(event, field) for field in range(10))))
                    traces.update(C.string_at(native.event_payload(event), 8))
                    traces.update(C.string_at(native.event_wire(event, 0), 1916))
                    traces.update(C.string_at(native.event_wire(event, 1), 1916))
                traces.update(C.string_at(native.final_wire(), 1916))
                traces.update(struct.pack('<I', native.result()))
            self.assertEqual(inputs.hexdigest(), '660409268dced5504fa0c2014e740673adff683483a6248b0900f2098967fcad')
            self.assertEqual(traces.hexdigest(), 'bf425ec1ed0df5b88c2ca83be240c24e5f7cb83a7d0f36ec44e99c72b2c773d5')
            regions = [(0x80180000, 200), (0x80181000, 200), (address_of('Options')-16, 96),
                       (address_of('PlayerControllers')-16, 208), (address_of('MenuStepArrowUv')-16, 36),
                       (address_of('MenuAvailableCourseMask')-16, 36), (0x80190000, 48), (0x801A0000, 544),
                       (0x801B0000, 544), (address_of('BadgeRows'), 4)]
            self.assertEqual(sum(n for _, n in regions), 1916)
            intervals = sorted((a, a+n) for a, n in regions)
            self.assertTrue(all(a[1] <= b[0] for a, b in zip(intervals, intervals[1:])))
            memory = Uc(UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN | UC_MODE_MIPS32);memory.mem_map(0, 0x400000)
            write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b))
            read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
            write(base, original[0x800:])
            if os.getenv('RR_COMPILED_ELF'):
                with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                    elf = ELFFile(stream)
                    for name in NAMES:
                        symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(name)[0]
                        section = elf.get_section(symbol['st_shndx']);offset = symbol['st_value']-section['sh_addr']
                        self.assertEqual(symbol['st_value'], address_of(name));write(symbol['st_value'], section.data()[offset:offset+symbol['st_size']])

            def store(cpu, access, address, length, value, user):
                address &= 0x1FFFFFFF
                self.assertTrue(any((a & 0x1FFFFFFF) <= address and address+length <= (a & 0x1FFFFFFF)+n
                                    for a, n in regions+[(STACK-72, 72)]), (hex(address), length))

            cpu = Cpu(memory, 0, 100, 101, 102, store)

            def write_state(data):
                offset = 0
                for address, length in regions:
                    write(address, data[offset:offset+length]);offset += length

            def state():
                return b''.join(read(a, n) for a, n in regions)

            services = {address_of(name): i+1 for i, name in enumerate(SERVICES)}
            for case in range(1536):
                number = (case*7919) & 65535;kind = number & 1;events = native.prepare_trace(number)
                write_state(C.string_at(native.initial_wire(), 1916));write(STACK-128, b'\xA5'*256)
                cpu.reset();cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
                if not kind:cpu.r[4:7] = [native.call_argument(i) for i in range(3)]
                cpu.r[29], cpu.r[31], cpu.pc = STACK, STOP, address_of(NAMES[kind]);event = 0
                for instruction in range(1000):
                    if cpu.pc == STOP:break
                    if cpu.pc not in services:cpu.step();continue
                    operation = services[cpu.pc];self.assertLess(event, events);self.assertEqual(operation, native.event_value(event, 0))
                    count = [7, 8, 3, 4][operation-1]
                    arguments = cpu.r[4:8]+[struct.unpack('<I', read(cpu.r[29]+16+i*4, 4))[0] for i in range(max(0, count-4))]
                    for slot in range(count):
                        if operation == 2 and slot == 4:
                            self.assertTrue(STACK-72 <= arguments[slot] and arguments[slot]+8 <= STACK)
                        else:self.assertEqual(arguments[slot], native.event_value(event, slot+1), (number, event, slot))
                    if operation in (1, 2):
                        size = 4 if operation == 1 else 8
                        self.assertEqual(read(arguments[4], size), C.string_at(native.event_payload(event), size), (number, event, 'payload'))
                    self.assertEqual(state(), C.string_at(native.event_wire(event, 0), 1916), (number, event, 'entry'))
                    write_state(C.string_at(native.event_wire(event, 1), 1916));resume = cpu.r[31]
                    for register in (1, 2, 3, *range(4, 16), 24, 25):cpu.r[register] = (0xA1230000+event*257+register) & 0xFFFFFFFF
                    cpu.r[2] = native.event_value(event, 9);cpu.pc = resume;event += 1
                self.assertEqual(cpu.pc, STOP, number);self.assertEqual(event, events)
                self.assertEqual(state(), C.string_at(native.final_wire(), 1916), (number, 'final'))
                if not kind:self.assertEqual(cpu.r[2], native.result(), (number, 'cursor return'))
                self.assertEqual(cpu.r[29], STACK);self.assertEqual(cpu.r[31], STOP)
                for register in (*range(16, 24), 28, 30):self.assertEqual(cpu.r[register], 0xBEEF0000+register)
                self.assertEqual(read(STACK-128, 56), b'\xA5'*56);self.assertEqual(read(STACK, 128), b'\xA5'*128)
            print('PASS 65536 native and 1536', 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
                  'MIPS controller callback/settings cases; real PanelDraw return, full owners/pointers, live mutations, guards and UBSan')


if __name__ == '__main__':
    unittest.main()
