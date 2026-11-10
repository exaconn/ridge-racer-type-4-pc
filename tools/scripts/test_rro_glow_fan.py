"""Current glow-fan C vs original/compiled MIPS, with scripted GTE transfers.

The GTE model supplies deterministic projection outputs and root mutations; this
checks the C glue, buffer ownership, packet/DMA protocol and native arithmetic,
not numerical projection accuracy, GTE flags or hardware timing.
"""
import ctypes as C
import os
from pathlib import Path
import struct
import subprocess
from tempfile import TemporaryDirectory

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32

from c_functions import address_of, definition_source, source_owners
from mips_preview_engine import signed
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
SCRATCH, WORK, SLOT = 0x1F800000, 0x1F80007C, 0x80300010
STACK, STOP = 0x803F0000, 0x803FF000
CASES, MAX_EVENTS = 4096, 96
SNAPSHOT_SIZE = 16 + 8 + 84 + 8192 + 72 + 256
OUTPUT_SIZE = 4 + MAX_EVENTS*12 + SNAPSHOT_SIZE
DEPTHS = (-32768, -1, 0, 1, 4, 92, 104, 112, 120, 1788, 1792, 32767, 1787, 1791, 1793, 16384)

PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "rr/rro_effects.h"
#include "rr/gpu_address.h"
typedef union {u32 words[1024];RroGlowPacket packets[128];} GpuArena;
static struct World {
 u8 before[16];RroEffectScratch scratch;u8 gap[16];
 CarSlotBufferC8 slot;u8 middle[16];GpuArena gpu[2];s32 fan[18];u8 after[16];
} world,expected;
#define SCRATCHPAD ((u8*)&world.scratch.scene)
#define SpotlightConeRingInner world.fan
static unsigned scenario,events,commands;
static u32 data[32],control[32],*trace;
static const s32 depths[]={-32768,-1,0,1,4,92,104,112,120,1788,1792,32767,1787,1791,1793,16384};
static s32 word(u32 x){s32 y;memcpy(&y,&x,4);return y;}
static s32 half(u32 x){return (x&32768)?(s32)(x&65535)-65536:(s32)(x&65535);}
u32 gpu_address(void) {
 uintptr_t p=(uintptr_t)world.gpu;
 assert(p>UINT32_MAX&&(p&0xFFFFFFU)+sizeof(world.gpu)<0x1000000U);
 return 0x80180000U;
}
static GpuDmaRegion gpuRegion;
static void installGpuMap(void) {
 gpuRegion=(GpuDmaRegion){world.gpu,0x80180000U,sizeof(world.gpu)};
 assert(GpuDmaSetRegions(&gpuRegion,1));
}
static u32 address(const void *pointer) {
 uintptr_t p=(uintptr_t)pointer,b;unsigned i;
 const struct {const void *p;u32 a,n;} regions[]={
  {world.scratch.work7C.object00.fields.translation14,0x1F800090,12},
  {&world.scratch.work7C.inputC8,0x1F800144,8},
  {world.scratch.work7C.projection188.fan.xy,0x1F800214,84},
  {world.fan,FAN_ADDRESS,72},{world.gpu,0,sizeof(world.gpu)}};
 for(i=0;i<sizeof(regions)/sizeof(regions[0]);i++) {
  b=(uintptr_t)regions[i].p;
  if(p>=b&&p-b<regions[i].n)return (i==4?gpu_address():regions[i].a)+(u32)(p-b);
 }
 assert(!"unmapped full native pointer");return 0;
}
static void event(u32 kind,u32 target,u32 value) {
 assert(events++<96);*trace++=kind;*trace++=target;*trace++=value;
}
static u32 readData(unsigned reg) {u32 value=data[reg];event(0x300+reg,0,value);return value;}
static void writeData(unsigned reg,u32 value){event(0x400+reg,0,value);data[reg]=value;}
static void writeControl(unsigned reg,u32 value){event(0x500+reg,0,value);control[reg]=value;}
static void load(unsigned reg,const void *p) {
 u32 value;memcpy(&value,p,4);event(0x100+reg,address(p),value);data[reg]=value;
}
static void store(unsigned reg,void *p) {
 event(0x200+reg,address(p),data[reg]);memcpy(p,&data[reg],4);
}
static void mutate(void) {
 unsigned g=(scenario+commands)&1,packet=16+(((scenario>>7)+commands)%3)*16;
 unsigned orderBank=g^((scenario>>6)&1),order=(((scenario>>9)+commands)&1)*128;
 world.scratch.scene.view.projection.pointers.cursor=expected.scratch.scene.view.projection.pointers.cursor=(u8*)&world.gpu[g].packets[packet];
 world.scratch.scene.view.projection.pointers.order=expected.scratch.scene.view.projection.pointers.order=&world.gpu[orderBank].words[order];
 world.scratch.scene.depthShift60=expected.scratch.scene.depthShift60=(scenario>>4)&7;
 world.slot.color08=expected.slot.color08=word(scenario*0x9E3779B9U+commands*0x1234567U);
 unsigned next=(commands*3)%18;u32 value=(u32)world.fan[next]^(scenario*0x17171717U+commands);
 world.fan[next]=expected.fan[next]=word(value);
}
static void command(u32 code) {
 event(0x600,0,code);
 if(code==0x4A180001U) {
  assert(commands==0);
  data[14]=data[0]^(scenario*0x01010101U)^0x12340000U;
  data[9]=(u32)half(scenario*13+7);data[10]=(u32)half(scenario*19+9);
  data[11]=(u32)depths[scenario&15];
 }else{
  assert(code==0x4A280030U&&commands>=1&&commands<=6);
  for(unsigned i=0;i<3;i++)data[12+i]=data[i*2]+control[5]+scenario*0x01010101U+i*0x10001U;
 }
 mutate();commands++;
}
static void setTranslation(const void *p) {
 u32 values[3];memcpy(values,p,12);for(unsigned i=0;i<3;i++)writeControl(5+i,values[i]);
}
static void rtps(const void *p){load(0,p);load(1,(const u8*)p+4);command(0x4A180001U);}
static void saveXY(void *p){store(14,p);}
static void translationFromIR(void) {
 u32 x=readData(9),y=readData(10),z=readData(11);
 writeControl(5,x);writeControl(6,y);writeControl(7,z);
}
static void identity(void) {
 writeControl(0,0x1000);writeControl(1,0);writeControl(2,0x1000);writeControl(3,0);writeControl(4,0x1000);
 writeData(1,0);writeData(3,0);writeData(5,0);
}
static void rtpt(const void *p){load(0,p);load(2,(const u8*)p+4);load(4,(const u8*)p+8);command(0x4A280030U);}
static void saveTriple(void *p){store(12,p);store(13,(u8*)p+4);store(14,(u8*)p+8);}
#define RR_GTE_SETTR(p) setTranslation(p)
#define RR_GTE_RTPS_LDV(p) rtps(p)
#define RR_GTE_SWC2_SXY2(p) saveXY(p)
#define RR_GTE_TR_FROM_IR() translationFromIR()
#define RR_GTE_READ_IR3_NOP(out) ((out)=word(readData(11)))
#define RR_GTE_SETROT_IDENTITY_CLEAR_Z() identity()
#define RR_GTE_RTPT_PACKED_XY(p) rtpt(p)
#define RR_GTE_SWC2_SXYZ(p) saveTriple(p)
'''

TAIL = r'''
static void snapshot(u8 *p) {
 u32 values[]={address(world.scratch.scene.view.projection.pointers.cursor),
  address(world.scratch.scene.view.projection.pointers.order),(u32)world.scratch.scene.depthShift60,(u32)world.slot.color08};
 memcpy(p,values,16);p+=16;memcpy(p,&world.scratch.work7C.inputC8,8);p+=8;
 memcpy(p,world.scratch.work7C.projection188.fan.xy,84);p+=84;
 memcpy(p,world.gpu,sizeof(world.gpu));p+=sizeof(world.gpu);memcpy(p,world.fan,72);p+=72;
 memcpy(p,data,128);p+=128;memcpy(p,control,128);
}
void run_case(unsigned number,u8 *output) {
 installGpuMap();
 assert(sizeof(void*)==8&&(uintptr_t)&world>UINT32_MAX);
 assert(offsetof(CarSlotBufferC8,color08)==sizeof(void*)+4);
 assert(offsetof(RroEffectScratch,work7C)==sizeof(RroSceneScratchPrefix));
 assert(offsetof(RroEffectWork,projection188)>0x188);
 scenario=number;events=commands=0;trace=(u32*)(output+4);
 memset(output,0,OUTPUT_SIZE);memset(data,0,sizeof(data));memset(control,0,sizeof(control));
 for(size_t i=0;i<sizeof(world);i++)((u8*)&world)[i]=(u8)(number*19+i*29);
 /* Logical regions have identical initial contents regardless of native layout. */
 memset(&world.scratch.work7C.inputC8,0xA5,8);memset(world.scratch.work7C.projection188.fan.xy,0xA5,84);
 for(unsigned i=0;i<sizeof(world.gpu);i++)((u8*)world.gpu)[i]=(u8)(number*19+i*29);
 for(unsigned i=0;i<18;i++)world.fan[i]=word(number*0x31415927U+i*0x1020304U);
 for(unsigned i=0;i<3;i++)world.scratch.work7C.object00.fields.translation14[i]=word(number*0x1234567U+i*0x88776655U);
 world.scratch.scene.view.projection.pointers.cursor=(u8*)&world.gpu[0].packets[16];
 world.scratch.scene.view.projection.pointers.order=world.gpu[0].words;
 world.scratch.scene.depthShift60=29;world.slot.color08=word(number*0x17171717U);
 expected=world;
 DrawGlowFan(&world.slot,&world.scratch.work7C);
 /* Check the whole native owners. Only the documented C output regions may
  * change; scripted hardware mutations already update both copies. Values in
  * those regions are compared separately with the original MIPS below. */
 memcpy(&expected.scratch.work7C.inputC8,&world.scratch.work7C.inputC8,6);
 expected.scratch.work7C.projection188.fan.xy[2]=world.scratch.work7C.projection188.fan.xy[2];
 s32 raw=depths[number&15];unsigned shift=((number>>4)&7)+2;
 int drawn=raw>=0&&((u32)raw>>shift)<448;
 assert(commands==(drawn?7U:1U));
 if(drawn) {
  unsigned g=(number+6)&1,packet=16+(((number>>7)+6)%3)*16;
  unsigned bank=g^((number>>6)&1),index=(((number>>9)+6)&1)*128+((u32)raw>>shift)+105;
  memcpy(expected.scratch.work7C.projection188.fan.xy+3,world.scratch.work7C.projection188.fan.xy+3,72);
  memcpy(expected.gpu[g].packets+packet,world.gpu[g].packets+packet,16*sizeof(RroGlowPacket));
  expected.gpu[bank].words[index]=world.gpu[bank].words[index];
  expected.scratch.scene.view.projection.pointers.cursor=world.scratch.scene.view.projection.pointers.cursor;
 }
 assert(!memcmp(&world,&expected,sizeof(world)));
 memcpy(output,&events,4);snapshot(output+4+96*12);
}
'''


def main():
    os.chdir(ROOT)
    owner = source_owners(ROOT/'src/main/PAL')['DrawGlowFan']
    body = definition_source(owner.read_text(), 'DrawGlowFan')
    assert '__asm__' not in body and 'volatile' not in body
    fan_address = address_of('SpotlightConeRingInner')
    with TemporaryDirectory() as directory:
        source, library = Path(directory)/'native.c', Path(directory)/'native.dylib'
        prefix = PREFIX.replace('FAN_ADDRESS', hex(fan_address)+'U')
        tail = TAIL.replace('OUTPUT_SIZE', str(OUTPUT_SIZE))
        source.write_text(prefix+body+tail)
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-Iinclude',
                        str(source), str(ROOT/'src/native/gpu_address.c'), '-o', str(library)], check=True)
        native = C.CDLL(str(library)); native.gpu_address.restype = C.c_uint32
        native.run_case.argtypes = [C.c_uint, C.c_void_p]
        gpu = native.gpu_address()
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0, 0x4000000); memory.mem_map(SCRATCH, 4096)
        write = lambda a, value: memory.mem_write(a & 0x1FFFFFFF, bytes(value))
        read = lambda a, size: bytes(memory.mem_read(a & 0x1FFFFFFF, size))
        put = lambda a, value: write(a, struct.pack('<I', value & 0xFFFFFFFF))
        get = lambda a: int.from_bytes(read(a, 4), 'little')
        executable = (ROOT/'assets/PAL/main.exe').read_bytes()
        base, = struct.unpack_from('<I', executable, 0x18); write(base, executable[0x800:])
        start = address_of('DrawGlowFan')
        if os.getenv('RR_COMPILED_ELF'):
            with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                elf = ELFFile(stream); symbol = elf.get_section_by_name('.symtab').get_symbol_by_name('DrawGlowFan')[0]
                section = elf.get_section(symbol['st_shndx']); offset = symbol['st_value']-section['sh_addr']
                assert symbol['st_value'] == start
                write(start, section.data()[offset:offset+symbol['st_size']])

        def store_check(cpu, access, address, size, value, user):
            address &= 0x1FFFFFFF
            regions = [(WORK+0xC8, 6), (WORK+0x1A0, 76), (SCRATCH, 4), (STACK-64, 64), (gpu, 8192)]
            assert any((a & 0x1FFFFFFF) <= address and address+size <= (a & 0x1FFFFFFF)+n
                       for a, n in regions), (hex(address), size)

        cpu = Cpu(memory, 0, 100, 101, 102, store_check)
        layouts, alias_fields, paths = set(), set(), set()
        for case in range(CASES):
            output = (C.c_uint32*((OUTPUT_SIZE+3)//4))(); native.run_case(case, output)
            expected = bytes(output)[:OUTPUT_SIZE]
            write(SCRATCH, b'\xA5'*1024); write(SLOT-16, b'\xA6'*16+b'\xA5'*200+b'\x6A'*16)
            for i in range(3): put(WORK+0x14+i*4, case*0x1234567+i*0x88776655)
            put(SCRATCH, gpu+16*32); put(SCRATCH+4, gpu); put(SCRATCH+0x60, 29); put(SLOT+8, case*0x17171717)
            write(gpu-16, b'\xA6'*16+bytes((case*19+i*29)&255 for i in range(8192))+b'\x6A'*16)
            for i in range(18): put(fan_address+i*4, case*0x31415927+i*0x1020304)
            data, control, trace, commands = [0]*32, [0]*32, [], 0
            cpu.reset(); cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
            cpu.r[4:6] = [SLOT, WORK]; cpu.r[29] = STACK; cpu.r[31] = STOP; cpu.pc = start

            def event(kind, address, value): trace.extend([kind, address & 0xFFFFFFFF, value & 0xFFFFFFFF])

            def mutate():
                g = (case+commands)&1; packet = 16+(((case >> 7)+commands)%3)*16
                bank = g^((case >> 6)&1); index = (((case >> 9)+commands)&1)*128
                put(SCRATCH, gpu+g*4096+packet*32); put(SCRATCH+4, gpu+bank*4096+index*4)
                put(SCRATCH+0x60, (case >> 4)&7); put(SLOT+8, case*0x9E3779B9+commands*0x1234567)
                at = fan_address+((commands*3)%18)*4; put(at, get(at)^(case*0x17171717+commands))

            for _ in range(2000):
                if cpu.pc == STOP: break
                instruction = get(cpu.pc); op = instruction >> 26
                if op not in (0x12, 0x32, 0x3A): cpu.step(); continue
                rs, rt, rd = (instruction >> 21)&31, (instruction >> 16)&31, (instruction >> 11)&31
                previous = cpu.delay; cpu.delay = None
                if op in (0x32, 0x3A):
                    address = (cpu.r[rs]+signed(instruction, 16)) & 0xFFFFFFFF
                    if op == 0x32:
                        value = get(address); event(0x100+rt, address, value); data[rt] = value
                    else:
                        event(0x200+rt, address, data[rt]); cpu.store(address, data[rt], 4)
                elif instruction & 0x02000000:
                    event(0x600, 0, instruction)
                    if instruction == 0x4A180001:
                        assert commands == 0
                        data[14] = (data[0]^(case*0x01010101)^0x12340000) & 0xFFFFFFFF
                        data[9] = signed(case*13+7, 16) & 0xFFFFFFFF; data[10] = signed(case*19+9, 16) & 0xFFFFFFFF
                        data[11] = DEPTHS[case&15] & 0xFFFFFFFF
                    else:
                        assert instruction == 0x4A280030 and 1 <= commands <= 6
                        for i in range(3): data[12+i] = (data[i*2]+control[5]+case*0x01010101+i*0x10001) & 0xFFFFFFFF
                    mutate(); commands += 1
                elif rs == 0:
                    event(0x300+rd, 0, data[rd])
                    if rt: cpu.r[rt] = data[rd]
                elif rs == 4:
                    event(0x400+rd, 0, cpu.r[rt]); data[rd] = cpu.r[rt]
                else:
                    assert rs == 6; event(0x500+rd, 0, cpu.r[rt]); control[rd] = cpu.r[rt]
                cpu.pc = previous if previous is not None else cpu.pc+4
            assert cpu.pc == STOP
            assert len(trace)//3 <= MAX_EVENTS
            wire = struct.pack('<I', len(trace)//3)+struct.pack('<'+'I'*len(trace), *trace)
            wire += bytes(MAX_EVENTS*12-len(trace)*4)
            wire += struct.pack('<4I', get(SCRATCH), get(SCRATCH+4), get(SCRATCH+0x60), get(SLOT+8))
            wire += read(WORK+0xC8, 8)+read(WORK+0x198, 84)+read(gpu, 8192)+read(fan_address, 72)
            wire += struct.pack('<64I', *data, *control)
            assert len(wire) == OUTPUT_SIZE and wire == expected, (case, 'GTE transfer trace / complete owners / DMA links')
            assert cpu.r[29] == STACK and cpu.r[31] == STOP
            for reg in (*range(16, 24), 28, 30): assert cpu.r[reg] == 0xBEEF0000+reg
            for address, size in [(gpu, 8192), (SLOT, 200)]:
                assert read(address-16, 16) == b'\xA6'*16 and read(address+size, 16) == b'\x6A'*16
            raw, shift = DEPTHS[case&15], ((case >> 4)&7)+2
            drawn = raw >= 0 and raw >> shift < 448; paths.add((raw >= 0, drawn))
            if drawn:
                g = (case+6)&1; packet = 16+(((case >> 7)+6)%3)*16
                bank = g^((case >> 6)&1); index = (((case >> 9)+6)&1)*128+(raw >> shift)+105
                head, first = gpu+bank*4096+index*4, gpu+g*4096+packet*32
                if first <= head < first+512: alias_fields.add((head-first)%32)
            layouts.add((case&15, (case >> 4)&7))
        assert len(layouts) == 16*8 and paths == {(False, False), (True, False), (True, True)}
        assert {0, 12, 20, 28} <= alias_fields, alias_fields
        print('PASS', CASES, 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
              'MIPS/native glow-fan cases; exact GTE transfer protocol, signed depth/clipping, 64-bit owners, live roots/colors, complete buffers, OT/packet aliases, DMA low24 links, ABI, guards, UBSan')


if __name__ == '__main__':
    main()
