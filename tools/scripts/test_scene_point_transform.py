"""Actual scene point transform: native owners, aliases and original MIPS.

ApplyMatrix is an instrumented service. Its inputs, entry/final states, callback
mutations and the three defined short components are checked independently.
The SDK's ignored fourth input halfword is deliberately outside the oracle.
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
NAME = 'TransformPoint'
BASE, STACK, STOP = 0x80180000, 0x803F0000, 0x803FF000
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "game/gfx/scene1.h"
#include "psyq/libgte/gtemat5.h"
typedef struct {u32 before[4];VectorWords16 vector;u32 after[4];} VectorOwner;
typedef struct {u32 before[4];MatrixWords32 matrix;u32 after[4];} MatrixOwner;
typedef struct {VectorOwner vectors[3];MatrixOwner matrices[2];} World;
typedef struct {World entry,after;u16 coordinates[3];u32 matrix,output;} Event;
static World live,initial,expectedFinal;
static Event actual,expected,*event;
static unsigned scenario,originIndex,pointIndex,outputIndex,matrixIndex,workspaceIndex;
static unsigned aliases[27],mutations[8],wraps,seen[3][2048];
static u32 next(u32 *seed){u32 n=*seed;n^=n<<13;n^=n>>17;n^=n<<5;return *seed=n;}
static s32 word(u32 n){s32 v;memcpy(&v,&n,4);return v;}
static u32 vector_address(unsigned index){return 0x80180000U+48U*index+16U;}
static u32 matrix_address(unsigned index){return 0x80180000U+144U+64U*index+16U;}
static Vec3i *vector(unsigned index){assert(index<3);return &live.vectors[index].vector.xyz;}
static void store_component(Vec3i *v,unsigned axis,s32 value){
 if(axis==0)v->x=value;else if(axis==1)v->y=value;else v->z=value;
}
static s32 component(const Vec3i *v,unsigned axis){return axis==0?v->x:axis==1?v->y:v->z;}
static void service(const MatrixWords32 *matrix,const u16 coordinates[3],Vec3i *output){
 static const s32 edges[]={INT32_MIN,INT32_MAX,-65537,-32768,-1,0,1,32767,65535,65536};
 assert(matrix==&live.matrices[matrixIndex].matrix&&output==vector(outputIndex));
 assert((uintptr_t)matrix>UINT32_MAX&&(uintptr_t)output>UINT32_MAX);
 memcpy(&event->entry,&live,sizeof(live));memcpy(event->coordinates,coordinates,6);
 event->matrix=matrix_address(matrixIndex);event->output=vector_address(outputIndex);
 for(unsigned axis=0;axis<3;axis++){
  unsigned n=(scenario/7+axis*3)%10;u32 bits=(u32)edges[n];
  if(scenario&8)bits^=scenario*0x17171717U+axis*0x1234567U;
  store_component(output,axis,word(bits));seen[axis][coordinates[axis]/32]|=1U<<(coordinates[axis]%32);
 }
 if(scenario&1)for(unsigned axis=0;axis<3;axis++)store_component(vector(originIndex),axis,edges[(scenario+axis)%10]);
 if(scenario&2)for(unsigned axis=0;axis<3;axis++)store_component(vector(pointIndex),axis,word(scenario*0x9E3779B9U+axis));
 if(scenario&4)live.matrices[matrixIndex].matrix.w[scenario%8]^=(s32)0xA55AA55AU;
 memcpy(&event->after,&live,sizeof(live));
}
Vec3i *ApplyMatrix(const MatrixWords32 *matrix,const Vec3s8 *input,Vec3i *output){
 u16 components[3]={(u16)input->fields.x,(u16)input->fields.y,(u16)input->fields.z};
 assert(((uintptr_t)input&3)==0);service(matrix,components,output);return output;
}
'''
REFERENCE = r'''
static void reference(void){
 u16 components[3];Vec3i *origin=vector(originIndex),*point=vector(pointIndex),*output=vector(outputIndex);
 for(unsigned axis=0;axis<3;axis++)components[axis]=(u16)component(point,axis);
 service(&live.matrices[matrixIndex].matrix,components,output);
 for(unsigned axis=0;axis<3;axis++){
  int64_t sum=(int64_t)component(output,axis)+component(origin,axis);
  wraps+=sum<INT32_MIN||sum>INT32_MAX;
  store_component(output,axis,word((u32)(uint64_t)sum));
 }
}
static void initialize(unsigned number){
 scenario=number;u32 seed=number*0x9E3779B9U+0x55BF8U;u8 *bytes=(u8*)&live;
 for(unsigned i=0;i<sizeof(live);i++)bytes[i]=(u8)next(&seed);
 originIndex=number%3;pointIndex=(number/3)%3;outputIndex=(number/9)%3;
 matrixIndex=(number/27)&1;workspaceIndex=(number/54)&1;
 for(unsigned axis=0;axis<3;axis++){
  u32 low=(number*(axis*2+1)+axis)&65535,high=(number^0xA55AU)<<16;
  store_component(vector(pointIndex),axis,word(high|low));
 }
 memcpy(&initial,&live,sizeof(live));aliases[originIndex+3*pointIndex+9*outputIndex]++;mutations[number&7]++;
}
unsigned exercise_native(void){
 _Static_assert(sizeof(VectorOwner)==48&&sizeof(MatrixOwner)==64&&sizeof(World)==272,"complete owners");
 _Static_assert(sizeof(Vec3s8)==8&&sizeof(Vec3i)==12,"SDK vector objects");
 _Static_assert(sizeof(SceneCameraPose18)==24&&sizeof(((SceneCameraState160*)0)->tracks138)==sizeof(void*),"shared camera owns its native pointer");
 _Static_assert(offsetof(SceneRaceContext,camera24C)==((offsetof(SceneRaceContext,cameraEC)+sizeof(SceneCameraState160)+sizeof(void*)-1)&~(sizeof(void*)-1)),"native camera pointer alignment");
 _Static_assert(offsetof(SceneRaceContext,overlayToggle3DC)==offsetof(SceneRaceContext,camera24C)+sizeof(void*)+396,"native context tail follows pointer");
 for(unsigned n=0;n<65536;n++){
  initialize(n);memset(&actual,0,sizeof(actual));memset(&expected,0,sizeof(expected));
  event=&expected;reference();memcpy(&expectedFinal,&live,sizeof(live));
  memcpy(&live,&initial,sizeof(live));event=&actual;
  TransformPoint(&live.matrices[matrixIndex].matrix,vector(originIndex),vector(pointIndex),
                 vector(outputIndex),&live.matrices[workspaceIndex].matrix);
  assert(!memcmp(&actual,&expected,sizeof(actual)));assert(!memcmp(&live,&expectedFinal,sizeof(live)));
 }
 for(unsigned i=0;i<27;i++)assert(aliases[i]);for(unsigned i=0;i<8;i++)assert(mutations[i]);
 for(unsigned axis=0;axis<3;axis++)for(unsigned i=0;i<2048;i++)assert(seen[axis][i]==0xFFFFFFFFU);
 assert(wraps);return 65536;
}
void prepare_trace(unsigned number){initialize(number);memset(&expected,0,sizeof(expected));event=&expected;reference();memcpy(&expectedFinal,&live,sizeof(live));}
const void *initial_world(void){return &initial;}const void *final_world(void){return &expectedFinal;}
const void *event_world(unsigned after){return after?&expected.after:&expected.entry;}
u32 argument(unsigned index){
 assert(index<5);return index==0?matrix_address(matrixIndex):index==1?vector_address(originIndex):
 index==2?vector_address(pointIndex):index==3?vector_address(outputIndex):matrix_address(workspaceIndex);
}
u32 input_component(unsigned axis){assert(axis<3);return expected.coordinates[axis];}
'''


class ScenePointTransformTest(unittest.TestCase):
    def test_current_api_owners_aliases_and_original_mips(self):
        owner = source_owners(ROOT/'src/main/PAL')[NAME]
        body = definition_source(owner.read_text(), NAME)
        with TemporaryDirectory() as temporary:
            source, library = Path(temporary)/'native.c', Path(temporary)/'native.dylib'
            source.write_text(PREFIX+body+REFERENCE)
            subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                            '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-I', str(ROOT/'include'),
                            str(source), '-o', str(library)], check=True)
            native = C.CDLL(str(library))
            native.exercise_native.restype = C.c_uint
            self.assertEqual(native.exercise_native(), 65536)
            native.prepare_trace.argtypes = [C.c_uint]
            native.initial_world.restype = native.final_world.restype = C.c_void_p
            native.event_world.argtypes = [C.c_uint]
            native.event_world.restype = C.c_void_p
            native.argument.argtypes = [C.c_uint]
            native.argument.restype = C.c_uint32
            native.input_component.argtypes = [C.c_uint]
            native.input_component.restype = C.c_uint32
            memory = Uc(UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN | UC_MODE_MIPS32)
            memory.mem_map(0, 0x400000)
            write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b))
            read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
            original = (ROOT/'assets/PAL/main.exe').read_bytes()
            base = struct.unpack_from('<I', original, 0x18)[0]
            write(base, original[0x800:])
            if os.getenv('RR_COMPILED_ELF'):
                with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                    elf = ELFFile(stream)
                    symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(NAME)[0]
                    section = elf.get_section(symbol['st_shndx'])
                    offset = symbol['st_value']-section['sh_addr']
                    self.assertEqual(symbol['st_value'], address_of(NAME))
                    write(symbol['st_value'], section.data()[offset:offset+symbol['st_size']])

            def store(cpu, access, address, length, value, user):
                address &= 0x1FFFFFFF
                if ((STACK-48) & 0x1FFFFFFF) <= address < (STACK & 0x1FFFFFFF):
                    self.assertIn(length, (2, 4))
                else:
                    self.assertEqual(length, 4)
                    self.assertIn(address, [((native.argument(3)+axis*4) & 0x1FFFFFFF) for axis in range(3)])

            cpu = Cpu(memory, 0, 100, 101, 102, store)
            for case in range(1024):
                number = (case*7919) & 65535
                native.prepare_trace(number)
                write(BASE, C.string_at(native.initial_world(), 272))
                write(STACK-128, b'\xA5'*256)
                write(STACK+16, struct.pack('<I', native.argument(4)))
                cpu.reset()
                cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
                for index in range(4):
                    cpu.r[4+index] = native.argument(index)
                cpu.r[29], cpu.r[31], cpu.pc = STACK, STOP, address_of(NAME)
                calls = 0
                for instruction in range(100):
                    if cpu.pc == STOP:
                        break
                    if cpu.pc == address_of('ApplyMatrix'):
                        calls += 1
                        self.assertEqual(cpu.r[4], native.argument(0))
                        self.assertEqual(cpu.r[6], native.argument(3))
                        self.assertEqual(cpu.r[5] & 3, 0)
                        self.assertTrue(STACK-40 <= cpu.r[5] <= STACK-8)
                        self.assertEqual(struct.unpack('<3H', read(cpu.r[5], 6)),
                                         tuple(native.input_component(i) for i in range(3)), number)
                        self.assertEqual(read(BASE, 272), C.string_at(native.event_world(0), 272), number)
                        write(BASE, C.string_at(native.event_world(1), 272))
                        for register in (1, 2, 3, *range(4, 16), 24, 25):
                            cpu.r[register] = 0xA1230000+register
                        cpu.r[2] = native.argument(3)
                        cpu.pc = cpu.r[31]
                    else:
                        cpu.step()
                self.assertEqual(cpu.pc, STOP, number)
                self.assertEqual(calls, 1)
                self.assertEqual(read(BASE, 272), C.string_at(native.final_world(), 272), number)
                self.assertEqual(cpu.r[29], STACK)
                self.assertEqual(cpu.r[31], STOP)
                for register in (*range(16, 24), 28, 30):
                    self.assertEqual(cpu.r[register], 0xBEEF0000+register)
                self.assertEqual(read(STACK-128, 80), b'\xA5'*80)
                self.assertEqual(read(STACK, 16), b'\xA5'*16)
                self.assertEqual(read(STACK+16, 4), struct.pack('<I', native.argument(4)))
                self.assertEqual(read(STACK+20, 108), b'\xA5'*108)
            print('PASS 65536 native and 1024', 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
                  'MIPS scene point cases; complete owners, all halfwords, aliases, wrap and UBSan')


if __name__ == '__main__':
    unittest.main()
