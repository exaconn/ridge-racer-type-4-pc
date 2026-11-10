"""Actual image wrappers: native driver pointers and original-MIPS service traces.

Rectangle validation and queue dispatch are instrumented services. This checks
the wrappers' C/API/ownership and submission protocol, not GPU execution.
"""
import ctypes as C
import os
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32

from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
FUNCTIONS = ('ClearImage', 'ClearImage2', 'LoadImage', 'StoreImage')
DRIVERS, RECTANGLES, PIXELS = 0x80200010, 0x80210010, 0x80220010
STACK, STOP = 0x803F0000, 0x803FF000
DISPATCH = (0x803E0010, 0x803E0020)
CLEAR, DOWNLOAD, UPLOAD = (0x803E0030, 0x803E0040), (0x803E0050, 0x803E0060), (0x803E0070, 0x803E0080)
LABELS = tuple(address_of(n) for n in ('StrClearImage', 'ClearImage2Name', 'LoadImageName', 'StrStoreImage'))
STATE_SIZE, EVENT_SIZE, CASES = 420, 444, 4096
OUTPUT_SIZE = 2*EVENT_SIZE+STATE_SIZE
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "rr/gpu_reset.h"
#include "psyq/libgpu/gpuenv.h"
static struct {u8 before[16];GpuDriver40 drivers[2];GpuRectangle rectangles[4];u32 pixels[4][16];u8 after[16];} world;
GpuResetDriver40 *GpuFuncTable;
char StrClearImage[]="clear",ClearImage2Name[]="clear2",LoadImageName[]="load",StrStoreImage[]="store";
static unsigned scenario,kind,events;static u8 *trace;
static s32 dispatch0(GpuOpFunc,void*,s32,unsigned long),dispatch1(GpuOpFunc,void*,s32,unsigned long);
static s32 clear0(GpuRectangle *p,u32 n){(void)p;(void)n;return 0;}
static s32 clear1(GpuRectangle *p,u32 n){(void)p;(void)n;return 1;}
static s32 download0(GpuRectangle *p,u32 *n){(void)p;(void)n;return 0;}
static s32 download1(GpuRectangle *p,u32 *n){(void)p;(void)n;return 1;}
static s32 upload0(GpuRectangle *p,const u32 *n){(void)p;(void)n;return 0;}
static s32 upload1(GpuRectangle *p,const u32 *n){(void)p;(void)n;return 1;}
static void put32(u8 **p,u32 x){memcpy(*p,&x,4);*p+=4;}
static u32 operation(GpuOpFunc f){
 GpuOpFunc functions[]={(GpuOpFunc)dispatch0,(GpuOpFunc)dispatch1,(GpuOpFunc)clear0,(GpuOpFunc)clear1,
  (GpuOpFunc)download0,(GpuOpFunc)download1,(GpuOpFunc)upload0,(GpuOpFunc)upload1};unsigned i;
 if(!f)return 0;for(i=0;i<8;i++)if(f==functions[i])return 0x803E0010U+i*16;assert(!"unknown callback");return 0;
}
static u32 address(const void *p){
 uintptr_t x=(uintptr_t)p,b=(uintptr_t)world.rectangles;
 if(x>=b&&x-b<sizeof(world.rectangles))return 0x80210010U+(u32)(x-b);
 b=(uintptr_t)world.pixels;if(x>=b&&x-b<sizeof(world.pixels))return 0x80220010U+(u32)(x-b);
 assert(!"unknown data pointer");return 0;
}
static void snapshot(u8 *p){
 unsigned i,j;put32(&p,GpuFuncTable==&world.drivers[1]);assert(GpuFuncTable==&world.drivers[0]||GpuFuncTable==&world.drivers[1]);
 for(i=0;i<2;i++){
  GpuDriver40 *d=&world.drivers[i];u32 cells[16]={0};
  assert(!d->name00&&!d->enqueue04&&!d->control10&&!d->writeBuffer14&&!d->draw_ordering_table);
  assert(!d->execute24&&!d->readControl28&&!d->clearOrdering2C&&!d->parameter30&&!d->reset34&&!d->status38&&!d->sync3C);
  cells[2]=operation((GpuOpFunc)d->dispatch);cells[3]=operation((GpuOpFunc)d->clear0C);
  cells[7]=operation((GpuOpFunc)d->download1C);cells[8]=operation((GpuOpFunc)d->upload20);
  for(j=0;j<16;j++)put32(&p,cells[j]);
 }
 memcpy(p,world.rectangles,32);memcpy(p+32,world.pixels,256);
}
static void event(u32 a,u32 b,u32 c,u32 d,u32 e,u32 f){
 assert(events++<2);put32(&trace,a);put32(&trace,b);put32(&trace,c);put32(&trace,d);put32(&trace,e);put32(&trace,f);
 snapshot(trace);trace+=420;
}
void checkRECT(const char *label,GpuRectangle *rectangle){
 u32 labels[]={@LABELS@};GpuDriver40 *driver;
 const char *names[]={StrClearImage,ClearImage2Name,LoadImageName,StrStoreImage};assert(label==names[kind]);
 event(1,labels[kind],address(rectangle),0,0,0);
 rectangle->words[0]^=scenario*0x17171717U+3;
 world.pixels[0][scenario&15]^=scenario*0x9999U+7;
 GpuFuncTable=&world.drivers[(scenario>>1)&1];driver=GpuFuncTable;
 driver->dispatch=(scenario&4)?dispatch1:dispatch0;driver->clear0C=(scenario&8)?clear1:clear0;
 driver->upload20=(scenario&16)?upload1:upload0;driver->download1C=(scenario&32)?download1:download0;
}
static s32 submit(unsigned which,GpuOpFunc op,void *payload,s32 length,unsigned long parameter){
 GpuRectangle *rectangle=payload;u32 encoded;
 assert(length==8&&address(payload)>=0x80210010U&&address(payload)<0x80210030U);
 if(kind<2){assert(parameter<=UINT32_MAX);encoded=(u32)parameter;}else encoded=address((const void*)parameter);
 event(2,0x803E0010U+which*16,operation(op),address(payload),(u32)length,encoded);
 rectangle->words[1]^=scenario*0x31313131U+11;
 world.pixels[3][(scenario>>4)&15]^=scenario*0x12345U+13;
 GpuFuncTable=&world.drivers[(scenario>>5)&1];return (s32)(0x80000000U^scenario);
}
static s32 dispatch0(GpuOpFunc op,void *p,s32 n,unsigned long v){return submit(0,op,p,n,v);}
static s32 dispatch1(GpuOpFunc op,void *p,s32 n,unsigned long v){return submit(1,op,p,n,v);}
'''
TAIL = r'''
void run_case(unsigned number,unsigned function,unsigned rectangle,unsigned pixels,s32 r,s32 g,s32 b,const u8 *input,u8 *output){
 unsigned i;scenario=number;kind=function;events=0;trace=output;memset(&world,0xA5,sizeof(world));
 assert(sizeof(void*)==8&&sizeof(unsigned long)>=sizeof(void*)&&(uintptr_t)&world>UINT32_MAX);
 assert(sizeof(GpuDriver40)==16*sizeof(void*));
 assert(offsetof(GpuDriver40,dispatch)==2*sizeof(void*));
 assert(offsetof(GpuDriver40,draw_ordering_table)==6*sizeof(void*));
 assert(offsetof(GpuDriver40,upload20)==8*sizeof(void*));
 assert(offsetof(GpuDriver40,reset34)==13*sizeof(void*));
 assert(rectangle<4&&pixels<4&&function<4);
 for(i=0;i<2;i++){
  world.drivers[i]=(GpuDriver40){0};world.drivers[i].dispatch=i?dispatch1:dispatch0;
  world.drivers[i].clear0C=i?clear1:clear0;world.drivers[i].download1C=i?download1:download0;
  world.drivers[i].upload20=i?upload1:upload0;
 }
 GpuFuncTable=&world.drivers[number&1];memcpy(world.rectangles,input,32);memcpy(world.pixels,input+32,256);
 switch(function){
 case 0:ClearImage(&world.rectangles[rectangle],r,g,b);break;
 case 1:ClearImage2(&world.rectangles[rectangle],r,g,b);break;
 case 2:LoadImage(&world.rectangles[rectangle],world.pixels[pixels]);break;
 case 3:StoreImage(&world.rectangles[rectangle],world.pixels[pixels]);break;
 }
 assert(events==2);snapshot(trace);assert(trace-output==888);
 for(i=0;i<16;i++)assert(world.before[i]==0xA5&&world.after[i]==0xA5);
}
'''


def main():
    os.chdir(ROOT)
    owners = source_owners(ROOT/'src/main/PAL')
    bodies = '\n'.join(definition_source(owners[n].read_text(), n) for n in FUNCTIONS)
    with TemporaryDirectory() as temporary:
        source, library = Path(temporary)/'native.c', Path(temporary)/'native.dylib'
        source.write_text(PREFIX.replace('@LABELS@', ','.join(hex(n) for n in LABELS))+bodies+TAIL)
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-Iinclude',
                        str(source), '-o', str(library)], check=True)
        native = C.CDLL(str(library));native.run_case.restype = None
        native.run_case.argtypes = [C.c_uint]*4+[C.c_int32]*3+[C.c_void_p]*2
        memory = Uc(UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN | UC_MODE_MIPS32);memory.mem_map(0, 0x400000)
        write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b))
        read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
        original = (ROOT/'assets/PAL/main.exe').read_bytes();base = struct.unpack_from('<I', original, 0x18)[0]
        write(base, original[0x800:])
        if os.getenv('RR_COMPILED_ELF'):
            with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                elf = ELFFile(stream)
                for name in FUNCTIONS:
                    symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(name)[0]
                    section = elf.get_section(symbol['st_shndx']);offset = symbol['st_value']-section['sh_addr']
                    assert symbol['st_value'] == address_of(name)
                    write(symbol['st_value'], section.data()[offset:offset+symbol['st_size']])

        def store(cpu, access, address, size, value, user):
            address &= 0x1FFFFFFF
            assert (STACK & 0x1FFFFFFF)-64 <= address and address+size <= (STACK & 0x1FFFFFFF)

        cpu = Cpu(memory, 0, 100, 101, 102, store);rng = random.Random(0x92ED4);layouts = set()
        def snapshot():
            root = struct.unpack('<I', read(address_of('GpuFuncTable'), 4))[0]
            assert root in (DRIVERS, DRIVERS+64)
            return struct.pack('<I', (root-DRIVERS)//64)+read(DRIVERS, 128)+read(RECTANGLES, 32)+read(PIXELS, 256)

        def xor(address, value):
            old = struct.unpack('<I', read(address, 4))[0];write(address, struct.pack('<I', old^(value&0xFFFFFFFF)))

        for function, name in enumerate(FUNCTIONS):
            for case in range(CASES):
                initial = rng.randbytes(288);rectangle, pixels = (case>>6)&3, (case>>8)&3
                colors = [rng.getrandbits(32) for _ in range(3)]
                if case<2048:colors = [((case+j*73)&255)|((case*0x171717)&0xFFFFFF00) for j in range(3)]
                output = (C.c_ubyte*OUTPUT_SIZE)()
                native.run_case(case, function, rectangle, pixels, *map(lambda n:C.c_int32(n).value, colors), C.create_string_buffer(initial), output)
                expected = bytes(output)
                tables = []
                for index in range(2):
                    table = [0]*16;table[2] = DISPATCH[index];table[3] = CLEAR[index];table[7] = DOWNLOAD[index];table[8] = UPLOAD[index]
                    tables.extend(table)
                write(DRIVERS-16, b'\xA5'*16+struct.pack('<32I', *tables)+b'\xA5'*16)
                write(RECTANGLES-16, b'\xA5'*16+initial[:32]+b'\xA5'*16)
                write(PIXELS-16, b'\xA5'*16+initial[32:]+b'\xA5'*16)
                write(address_of('GpuFuncTable'), struct.pack('<I', DRIVERS+(case&1)*64))
                cpu.reset();cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
                rect = RECTANGLES+rectangle*8;image = PIXELS+pixels*64
                cpu.r[4:8] = [rect, *colors] if function<2 else [rect, image, 0xDEAD0006, 0xDEAD0007]
                cpu.r[29] = STACK;cpu.r[31] = STOP;cpu.pc = address_of(name);trace = bytearray();events = 0
                for instruction in range(300):
                    if cpu.pc == STOP:break
                    if cpu.pc == address_of('checkRECT'):
                        assert events == 0 and cpu.r[4:6] == [LABELS[function], rect]
                        trace.extend(struct.pack('<6I', 1, LABELS[function], rect, 0, 0, 0)+snapshot());events += 1
                        xor(rect, case*0x17171717+3);xor(PIXELS+(case&15)*4, case*0x9999+7)
                        root = DRIVERS+((case>>1)&1)*64
                        write(address_of('GpuFuncTable'), struct.pack('<I', root))
                        for offset, value in [(8, DISPATCH[(case>>2)&1]), (12, CLEAR[(case>>3)&1]),
                                              (28, DOWNLOAD[(case>>5)&1]), (32, UPLOAD[(case>>4)&1])]:
                            write(root+offset, struct.pack('<I', value))
                        cpu.pc = cpu.r[31]
                        for reg in (*range(2, 16), 24, 25):cpu.r[reg] = 0xDEAD0000+reg
                    elif cpu.pc in DISPATCH:
                        assert events == 1
                        color = (colors[0]&255)|((colors[1]&255)<<8)|((colors[2]&255)<<16)
                        if function == 1:color |= 0x80000000
                        op = CLEAR[(case>>3)&1] if function<2 else UPLOAD[(case>>4)&1] if function==2 else DOWNLOAD[(case>>5)&1]
                        parameter = color if function<2 else image
                        assert cpu.r[4:8] == [op, rect, 8, parameter], (name, case, cpu.r[4:8], parameter)
                        trace.extend(struct.pack('<6I', 2, cpu.pc, op, rect, 8, parameter)+snapshot());events += 1
                        xor(rect+4, case*0x31313131+11);xor(PIXELS+192+((case>>4)&15)*4, case*0x12345+13)
                        write(address_of('GpuFuncTable'), struct.pack('<I', DRIVERS+((case>>5)&1)*64))
                        cpu.pc = cpu.r[31]
                        for reg in (*range(2, 16), 24, 25):cpu.r[reg] = 0xCAFE0000+reg
                    else:cpu.step()
                assert cpu.pc == STOP and events == 2
                trace.extend(snapshot());assert bytes(trace) == expected, (name, case, 'whole state and exact service protocol')
                assert cpu.r[29] == STACK and cpu.r[31] == STOP
                for reg in (*range(16, 24), 28, 30):assert cpu.r[reg] == 0xBEEF0000+reg
                for address, size in [(DRIVERS, 128), (RECTANGLES, 32), (PIXELS, 256)]:
                    assert read(address-16, 16) == b'\xA5'*16 and read(address+size, 16) == b'\xA5'*16
                layouts.add((function, rectangle, pixels, case&63))
        assert len(layouts) == 4*4*4*64
        print('PASS', len(FUNCTIONS)*CASES, 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
              'MIPS/native image dispatch cases; complete native driver, mutable diagnostics/roots/callbacks, RGB low bytes and fill flag, full pixel pointers, complete owners, service protocol, ABI, guards and UBSan')


if __name__ == '__main__':
    main()
