"""Actual scene lighting setup: typed inputs, publication order and MIPS.

Rotation, matrix and GPU/GTE services are instrumented. They seed complete
matrix outputs so every published byte has an independent oracle; this fixture
does not claim to simulate trigonometry or GTE arithmetic. Color-matrix padding
and translation are not consumed by SetColorMatrix and are never inspected.
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
NAME = 'SetupObjectLightMatrices'
BASE, STACK, STOP = 0x80180000, 0x803F0000, 0x803FF000
SERVICES = ['BuildRotMatrixY', 'BuildRotMatrixX', 'BuildRotMatrixZ', 'MulMatrix2', 'MulMatrix',
            'MulMatrix0', 'SetLightMatrix', 'SetColorMatrix', 'SetBackColor', 'ScaleMatrix']
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "game/gfx/scene1.h"
#include "rr/matrix_workspace.h"
#include "psyq/libgte/gtemat.h"
#include "psyq/libgte/gtemat3.h"
typedef struct {
 u32 before[4];SceneCameraPose18 pose;PreviewLighting40 lighting;MatrixWords32 view,outputs[2];
 PreviewMatrixWorkspace light;VectorWords16 scale;u32 after[4];
} World;
typedef struct {
 World entry,after;u32 operation,args[3],sizes[3],output;
 u8 inputs[3][32],result[32];
} Event;
typedef struct {World final;unsigned count;Event events[24];} Capture;
static World live,initial;static Capture actual,expected,*capture;
static unsigned scenario,serial,lightIndex,rotationIndex,recordWire;
static unsigned aliasSeen[16],serviceSeen[11],mutationSeen[2],colorSeen[8];
static u8 firstWire[240],lastWire[240],beforeWire[24][240],afterWire[24][240];
#define LightMatrix live.light
#define D_800111D4 live.scale
#define SCRATCH_ROT_MATRIX (&live.view)
static u32 next(u32 *seed){u32 n=*seed;n^=n<<13;n^=n>>17;n^=n<<5;return *seed=n;}
static s32 word(u32 n){s32 v;memcpy(&v,&n,4);return v;}
static MatrixWords32 *output(unsigned index){
 assert(index<4);return index<2?&live.outputs[index]:index==2?&live.light.matrix:&live.view;
}
static u32 pointer(const void *p){
 if(p==&live.view)return 0x1F800028U;
 if(p==&live.outputs[0])return 0x80180070U;
 if(p==&live.outputs[1])return 0x80180090U;
 if(p==&live.light)return @LIGHT@;
 if(p==&live.scale)return @SCALE@;
 assert((uintptr_t)p>UINT32_MAX&&((uintptr_t)p&3)==0);return 0;
}
static void mutate(void){
 unsigned n=serial++;if(!(scenario&1))return;
 live.pose.rotation10.x=(s16)(scenario*73U+n);
 live.pose.rotation10.y=(s16)(scenario*37U+n*3U);
 live.pose.rotation10.z=(s16)~(scenario+n);
 for(unsigned i=0;i<3;i++){
  PreviewColor4 *c=&live.lighting.matrix.fields.colors[i];
  c->red=(u8)(scenario+n+i);c->green=(u8)(scenario+n+i+17);c->blue=(u8)(scenario+n+i+71);
  live.lighting.matrix.fields.vectors[i].x=(s16)(scenario+n+i*0x1717U);
  live.lighting.matrix.fields.vectors[i].y=(s16)~(scenario+n+i*0x1234U);
  live.lighting.matrix.fields.vectors[i].z=(s16)(scenario*3U+n+i);
  live.lighting.ambient[i]=(u8)(scenario/2+n+i*73U);
 }
 for(unsigned i=0;i<8;i++)live.view.w[i]=word(scenario*0x1234567U+n*257U+i);
 for(unsigned i=0;i<4;i++)live.scale.words[i]=word(scenario*0x17171717U+n+i);
 live.light.words[n%8]^=0xA55AA55AU;
}
static Event *begin(unsigned operation,void *p0,unsigned s0,void *p1,unsigned s1,void *p2,unsigned s2,u32 scalar){
 unsigned index=capture->count++;assert(index<24);Event *e=&capture->events[index];memset(e,0,sizeof(*e));
 memcpy(&e->entry,&live,sizeof(live));e->operation=operation;e->output=3;
 void *p[3]={p0,p1,p2};unsigned sizes[3]={s0,s1,s2};
 for(unsigned i=0;i<3;i++)if(p[i]){e->args[i]=pointer(p[i]);e->sizes[i]=sizes[i];if(sizes[i])memcpy(e->inputs[i],p[i],sizes[i]);}
 if(operation<=3)e->args[1]=scalar;
 if(recordWire)memcpy(beforeWire[index],&live,240);serviceSeen[operation]++;return e;
}
static void finish(Event *e,MatrixWords32 *result,unsigned index){
 if(result)e->output=index;mutate();
 if(result)memcpy(e->result,result,32);
 memcpy(&e->after,&live,sizeof(live));if(recordWire)memcpy(afterWire[capture->count-1],&live,240);
}
static void rotation(unsigned operation,MatrixWords32 *out,s32 angle){
 Event *e=begin(operation,out,0,0,0,0,0,(u32)angle);u32 seed=(u32)angle^operation*0x9E3779B9U;
 for(unsigned i=0;i<8;i++)out->w[i]=word(next(&seed));finish(e,out,0);
}
static void traceBuildRotY(MatrixWords32 *out,s32 angle){rotation(1,out,angle);}
static void traceBuildRotX(MatrixWords32 *out,s32 angle){rotation(2,out,angle);}
static void traceBuildRotZ(MatrixWords32 *out,s32 angle){rotation(3,out,angle);}
static void product(MatrixWords32 *out,const u8 *left,const u8 *right){
 u32 seed=0x5147A31U;for(unsigned i=0;i<5;i++){u32 a,b;memcpy(&a,left+4*i,4);memcpy(&b,right+4*i,4);seed^=a+0x9E3779B9U+(seed<<6)+(seed>>2);seed^=b*0x17171717U;out->w[i]=word(seed);}
}
MatrixWords32 *MulMatrix2(const MatrixWords32 *left,MatrixWords32 *right){
 Event *e=begin(4,(void*)left,32,right,32,0,0,0);product(right,e->inputs[0],e->inputs[1]);finish(e,right,1);return right;
}
MatrixWords32 *MulMatrix(MatrixWords32 *left,const MatrixWords32 *right){
 Event *e=begin(5,left,32,(void*)right,32,0,0,0);product(left,e->inputs[0],e->inputs[1]);finish(e,left,0);return left;
}
MatrixWords32 *MulMatrix0(const MatrixWords32 *left,const MatrixWords32 *right,MatrixWords32 *out){
 Event *e=begin(6,(void*)left,32,(void*)right,32,out,32,0);product(out,e->inputs[0],e->inputs[1]);finish(e,out,2);return out;
}
void SetLightMatrix(const MatrixWords32 *input){Event *e=begin(7,(void*)input,32,0,0,0,0,0);finish(e,0,3);}
void SetColorMatrix(const MatrixWords32 *input){Event *e=begin(8,(void*)input,18,0,0,0,0,0);finish(e,0,3);}
void SetBackColor(s32 red,s32 green,s32 blue){
 Event *e=begin(9,0,0,0,0,0,0,0);e->args[0]=(u32)red;e->args[1]=(u32)green;e->args[2]=(u32)blue;
 assert((u32)red<256&&(u32)green<256&&(u32)blue<256);colorSeen[(unsigned)red/32]|=1U<<((unsigned)red%32);finish(e,0,3);
}
int *ScaleMatrix(int *matrix,int *scale){
 Event *e=begin(10,matrix,32,scale,16,0,0,0);MatrixWords32 *out=(MatrixWords32*)matrix;
 for(unsigned row=0;row<3;row++)for(unsigned column=0;column<3;column++){
  u32 factor;memcpy(&factor,e->inputs[1]+column*4,4);
  out->fields.rotation00[row][column]=(s16)(u16)((u32)(u16)out->fields.rotation00[row][column]*factor);
 }
 finish(e,out,0);return matrix;
}
'''
REFERENCE = r'''
static void reference(void){
 MatrixWords32 basis,temporary,axis,color;VectorWords16 scale=live.scale;
 traceBuildRotY(&axis,-live.pose.rotation10.y);MulMatrix2(&live.view,&axis);
 traceBuildRotX(&basis,-live.pose.rotation10.x);traceBuildRotZ(&temporary,-live.pose.rotation10.z);
 MulMatrix(&basis,&temporary);MulMatrix2(&axis,&basis);
 traceBuildRotY(&axis,live.pose.rotation10.y);traceBuildRotX(&temporary,live.pose.rotation10.x);
 MulMatrix(&axis,&temporary);traceBuildRotZ(&temporary,live.pose.rotation10.z);MulMatrix2(&axis,&temporary);
 memcpy(output(rotationIndex),&temporary,32);memset(&color,0,sizeof(color));
 for(unsigned row=0;row<3;row++){
  const PreviewVector8 *d=&live.lighting.matrix.fields.vectors[row];
  s16 direction[3]={d->x,d->y,d->z};
  const PreviewColor4 *c=&live.lighting.matrix.fields.colors[row];u8 channels[3]={c->red,c->green,c->blue};
  for(unsigned column=0;column<3;column++){
   live.light.matrix.fields.rotation00[row][column]=direction[column];
   color.fields.rotation00[column][row]=(s16)(channels[column]*16U);
  }
 }
 MulMatrix0(&live.light.matrix,&basis,&temporary);SetLightMatrix(&temporary);SetColorMatrix(&color);
 SetBackColor(live.lighting.ambient[0],live.lighting.ambient[1],live.lighting.ambient[2]);
 ScaleMatrix(basis.w,scale.words);memcpy(output(lightIndex),&basis,32);
}
static void initialize(unsigned number){
 scenario=number;serial=0;lightIndex=(number/2)%4;rotationIndex=(number/8)%4;
 u32 seed=number*0x9E3779B9U+0x5599CU;u8 *p=(u8*)&live;
 for(unsigned i=0;i<sizeof(live);i++)p[i]=(u8)next(&seed);
 live.pose.rotation10.x=(s16)(number*37U);live.pose.rotation10.y=(s16)(number*73U);live.pose.rotation10.z=(s16)~number;
 live.lighting.ambient[0]=(u8)number;aliasSeen[lightIndex+4*rotationIndex]++;mutationSeen[number&1]++;
 memcpy(&initial,&live,sizeof(live));
}
unsigned exercise_native(void){
 _Static_assert(sizeof(World)==240&&offsetof(World,outputs)==112&&offsetof(World,light)==176,"complete fixture owners");
 for(unsigned n=0;n<8192;n++){
  initialize(n);capture=&expected;capture->count=0;reference();memcpy(&capture->final,&live,sizeof(live));
  memcpy(&live,&initial,sizeof(live));serial=0;capture=&actual;capture->count=0;
  SetupObjectLightMatrices(&live.pose,&live.lighting,output(lightIndex),output(rotationIndex));
  memcpy(&capture->final,&live,sizeof(live));assert(actual.count==expected.count);
  for(unsigned i=0;i<actual.count;i++)assert(!memcmp(&actual.events[i],&expected.events[i],sizeof(Event)));
  assert(!memcmp(&actual.final,&expected.final,sizeof(World)));
 }
 for(unsigned i=0;i<16;i++)assert(aliasSeen[i]);for(unsigned i=1;i<11;i++)assert(serviceSeen[i]);
 for(unsigned i=0;i<8;i++)assert(colorSeen[i]==0xFFFFFFFFU);assert(mutationSeen[0]&&mutationSeen[1]);return 8192;
}
unsigned prepare_trace(unsigned number){initialize(number);memcpy(firstWire,&live,240);capture=&expected;capture->count=0;recordWire=1;reference();memcpy(lastWire,&live,240);recordWire=0;return capture->count;}
const void *initial_wire(void){return firstWire;}const void *final_wire(void){return lastWire;}
const void *event_wire(unsigned i,unsigned after){assert(i<capture->count);return after?afterWire[i]:beforeWire[i];}
u32 event_value(unsigned i,unsigned field){assert(i<capture->count&&field<8);Event *e=&capture->events[i];return field==0?e->operation:field<4?e->args[field-1]:field<7?e->sizes[field-4]:e->output;}
const void *event_data(unsigned i,unsigned argument){assert(i<capture->count&&argument<4);return argument==3?capture->events[i].result:capture->events[i].inputs[argument];}
u32 argument(unsigned index){return index==0?0x80180010U:index==1?0x80180028U:pointer(output(index==2?lightIndex:rotationIndex));}
'''


class SceneLightMatrixTest(unittest.TestCase):
    def test_current_lighting_pipeline_and_original_mips(self):
        owner = source_owners(ROOT/'src/main/PAL')[NAME]
        body = definition_source(owner.read_text(), NAME)
        # The old rotation helpers expose raw SDK buffers; adapters observe the
        # complete MatrixWords32 owners used by this caller without changing ABI.
        for axis in 'XYZ':
            body = body.replace('BuildRotMatrix'+axis+'(', 'traceBuildRot'+axis+'(')
        prefix = PREFIX.replace('@LIGHT@', hex(address_of('LightMatrix'))+'U').replace('@SCALE@', hex(address_of('D_800111D4'))+'U')
        with TemporaryDirectory() as temporary:
            source, library = Path(temporary)/'native.c', Path(temporary)/'native.dylib'
            source.write_text(prefix+body+REFERENCE)
            subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                            '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-I', str(ROOT/'include'),
                            str(source), '-o', str(library)], check=True)
            native = C.CDLL(str(library));native.exercise_native.restype = C.c_uint
            self.assertEqual(native.exercise_native(), 8192)
            native.prepare_trace.argtypes = [C.c_uint];native.prepare_trace.restype = C.c_uint
            native.initial_wire.restype = native.final_wire.restype = C.c_void_p
            native.event_wire.argtypes = [C.c_uint, C.c_uint];native.event_wire.restype = C.c_void_p
            native.event_value.argtypes = [C.c_uint, C.c_uint];native.event_value.restype = C.c_uint32
            native.event_data.argtypes = [C.c_uint, C.c_uint];native.event_data.restype = C.c_void_p
            native.argument.argtypes = [C.c_uint];native.argument.restype = C.c_uint32
            regions = [(BASE, 80), (0x1F800028, 32), (BASE+112, 64), (address_of('LightMatrix'), 32),
                       (address_of('D_800111D4'), 16), (BASE+224, 16)]
            intervals = sorted((a, a+n) for a, n in regions)
            self.assertTrue(all(a[1] <= b[0] for a, b in zip(intervals, intervals[1:])))
            memory = Uc(UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN | UC_MODE_MIPS32)
            memory.mem_map(0, 0x400000);memory.mem_map(0x1F800000, 4096)
            write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b))
            read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
            original = (ROOT/'assets/PAL/main.exe').read_bytes();base = struct.unpack_from('<I', original, 24)[0]
            write(base, original[0x800:])
            if os.getenv('RR_COMPILED_ELF'):
                with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                    elf = ELFFile(stream);symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(NAME)[0]
                    section = elf.get_section(symbol['st_shndx']);offset = symbol['st_value']-section['sh_addr']
                    self.assertEqual(symbol['st_value'], address_of(NAME));write(symbol['st_value'], section.data()[offset:offset+symbol['st_size']])
            services = {address_of(name): i+1 for i, name in enumerate(SERVICES)}

            def store(cpu, access, address, length, value, user):
                address &= 0x1FFFFFFF
                allowed = regions+[(STACK-256, 256)]
                self.assertTrue(any((a & 0x1FFFFFFF) <= address and address+length <= (a & 0x1FFFFFFF)+n
                                    for a, n in allowed), (hex(address), length))

            cpu = Cpu(memory, 0, 100, 101, 102, store)

            def write_state(data):
                offset = 0
                for address, length in regions:write(address, data[offset:offset+length]);offset += length

            def state():return b''.join(read(a, n) for a, n in regions)

            for case in range(512):
                number = (case*7919) & 8191;events = native.prepare_trace(number)
                write_state(C.string_at(native.initial_wire(), 240));write(STACK-384, b'\xA5'*512)
                cpu.reset();cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
                for i in range(4):cpu.r[4+i] = native.argument(i)
                cpu.r[29], cpu.r[31], cpu.pc = STACK, STOP, address_of(NAME);event = 0
                for instruction in range(2000):
                    if cpu.pc == STOP:break
                    if cpu.pc in services:
                        self.assertLess(event, events);operation = services[cpu.pc]
                        self.assertEqual(operation, native.event_value(event, 0), (number, event))
                        arguments = [2, 2, 2, 2, 2, 3, 1, 1, 3, 2][operation-1]
                        for i in range(arguments):
                            scalar = operation == 9 or (operation <= 3 and i == 1)
                            value = cpu.r[4+i];expected = native.event_value(event, 1+i)
                            if scalar:self.assertEqual(value, expected, (number, event, i))
                            elif expected:self.assertEqual(value, expected, (number, event, i))
                            else:self.assertTrue(STACK-256 <= value < STACK and value % 4 == 0, (number, event, i, hex(value)))
                            length = native.event_value(event, 4+i)
                            if length:self.assertEqual(read(value, length), C.string_at(native.event_data(event, i), length), (number, event, i))
                        self.assertEqual(state(), C.string_at(native.event_wire(event, 0), 240), (number, event, 'entry'))
                        output = native.event_value(event, 7);result = cpu.r[4+output] if output < 3 else 0
                        if output < 3:write(result, C.string_at(native.event_data(event, 3), 32))
                        write_state(C.string_at(native.event_wire(event, 1), 240))
                        for register in (1, 2, 3, *range(4, 16), 24, 25):cpu.r[register] = (0xA1230000+event*257+register) & 0xFFFFFFFF
                        cpu.r[2] = result;cpu.pc = cpu.r[31];event += 1
                    else:cpu.step()
                self.assertEqual(cpu.pc, STOP, number);self.assertEqual(event, events)
                self.assertEqual(state(), C.string_at(native.final_wire(), 240), (number, 'final'))
                self.assertEqual(cpu.r[29], STACK);self.assertEqual(cpu.r[31], STOP)
                for register in (*range(16, 24), 28, 30):self.assertEqual(cpu.r[register], 0xBEEF0000+register)
                self.assertEqual(read(STACK-384, 128), b'\xA5'*128);self.assertEqual(read(STACK, 128), b'\xA5'*128)
            print('PASS 8192 native and 512', 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
                  'MIPS scene lighting cases; typed poses/colors, complete matrices, output aliases, callbacks and UBSan')


if __name__ == '__main__':
    unittest.main()
