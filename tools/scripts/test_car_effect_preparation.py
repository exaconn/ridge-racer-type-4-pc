"""Actual effect preparation vs original MIPS, typed native owners and services.

GTE color transfers and the three render kernels are instrumented. The test
validates preparation/dispatch, not the kernels or numerical GTE behavior.
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
WORK, SLOT = 0x1F80007C, 0x80250010
MODELS, VEHICLES, SPARKS = (0x80260010, 0x80261010), (0x80180010, 0x80190010), (0x80290010, 0x802A0010)
VERTICES, COMMANDS, STACK, STOP = 0x80270010, 0x80280010, 0x803F0000, 0x803FF000
CASES, STATE, MAX_OPS = 4096, 612, 64
OUTPUT = 8 + MAX_OPS*12 + 6*(16+STATE) + STATE + 256
PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "car_render_wire.h"
#include "rr/rro_effects.h"
static struct World {
 u8 before[16];RroEffectScratch scratch;CarSlotBufferC8 slot;
 CarModelHeader models[2];RaceVehicle vehicles[2];RroSpark sparks[2][16];
 Vec3s8 vertices[8];u32 streams[16];RroEffectColorTable table;
 u32 frame;s8 paused;u8 after[16];
} world,expected;
#define ScreenFrame world.frame
#define RacePaused world.paused
#define RroEffectColorPresets world.table
static const u32 modelAddress[2]={0x80260010,0x80261010};
static const u32 vehicleAddress[2]={0x80180010,0x80190010};
static const u32 sparkAddress[2]={0x80290010,0x802A0010};
static unsigned scenario,operations,services,randoms,colors;
static u32 data[32],control[32],*opTrace;static u8 *serviceTrace;
static s32 word(u32 x){s32 y;memcpy(&y,&x,4);return y;}
static void *resolve(u32 p) {
 if(!p)return NULL;
 for(unsigned i=0;i<2;i++){
  if(p==modelAddress[i])return &world.models[i];
  if(p==sparkAddress[i])return world.sparks[i];
 }
 if(p>=0x80270010U&&p<0x80270050U)return (u8*)world.vertices+(p-0x80270010U);
 if(p>=0x80280010U&&p<0x80280050U)return (u8*)world.streams+(p-0x80280010U);
 assert(!"unmapped PAL pointer");return 0;
}
static u32 address(const void *p) {
 uintptr_t x=(uintptr_t)p,b;
 if(!p)return 0;
 if(p==&world.scratch.work7C)return 0x1F80007C;
 if(p==world.scratch.work7C.object00.fields.translation14)return 0x1F800090;
 if(p==&world.scratch.work7C.jitterTranslationAC)return 0x1F800128;
 b=(uintptr_t)world.scratch.work7C.colors15C.words;
 if(x>=b&&x-b<28)return 0x1F8001D8U+(u32)(x-b);
 b=(uintptr_t)world.table.input;
 if(x>=b&&x-b<24)return TABLE_ADDRESS+(u32)(x-b);
 for(unsigned i=0;i<2;i++){
  if(p==&world.models[i])return modelAddress[i];
  if(p==&world.models[i].shadow58)return modelAddress[i]+0x58;
  for(unsigned j=0;j<4;j++)if(p==&world.models[i].body64[j])return modelAddress[i]+0x64+j*12;
  if(p==&world.vehicles[i])return vehicleAddress[i];
  if(p==world.sparks[i])return sparkAddress[i];
 }
 b=(uintptr_t)world.vertices;if(x>=b&&x-b<64)return 0x80270010U+(u32)(x-b);
 b=(uintptr_t)world.streams;if(x>=b&&x-b<64)return 0x80280010U+(u32)(x-b);
 assert(!"truncated native pointer");return 0;
}
static void snapshot(u8 *p) {
 u8 *start=p;car_slot_store(p,&world.slot,address);p+=200;
 for(unsigned i=0;i<2;i++){car_model_store(p,&world.models[i],address);p+=0x94;}
 memcpy(p,world.scratch.work7C.object00.fields.translation14,12);p+=12;
 memcpy(p,&world.scratch.work7C.jitterTranslationAC,12);p+=12;
 memcpy(p,world.scratch.work7C.colors15C.words,28);p+=28;
 car_put32(&p,address(world.scratch.work7C.vehicle90));
 for(unsigned i=0;i<2;i++){memcpy(p,&world.vehicles[i].input,2);p+=2;}
 car_put32(&p,(u32)(s32)world.paused);car_put32(&p,world.frame);
 memcpy(p,&world.table,48);p+=48;assert(p-start==STATE_SIZE);
}
static void op(u32 kind,u32 p,u32 value) {
 assert(operations++<64);*opTrace++=kind;*opTrace++=p;*opTrace++=value;
}
static void change(unsigned serial) {
 world.slot.model=expected.slot.model=&world.models[(scenario+serial)&1];
 world.slot.sparksC4=expected.slot.sparksC4=world.sparks[(scenario+serial+1)&1];
 world.scratch.work7C.vehicle90=expected.scratch.work7C.vehicle90=&world.vehicles[(scenario+serial)&1];
 world.vehicles[(scenario+serial)&1].input=expected.vehicles[(scenario+serial)&1].input=(s16)(scenario*37+serial*107);
 world.frame=expected.frame=scenario*0x9E3779B9U+serial;
 world.scratch.work7C.object00.fields.translation14[0]=expected.scratch.work7C.object00.fields.translation14[0]=word((scenario&1)?0x7FFFFFFFU:0x80000000U);
 world.scratch.work7C.object00.fields.translation14[1]=expected.scratch.work7C.object00.fields.translation14[1]=word((scenario&1)?0x80000000U:0x7FFFFFFFU);
 world.scratch.work7C.object00.fields.translation14[2]=expected.scratch.work7C.object00.fields.translation14[2]=word(scenario*0x17171717U+serial);
 world.slot.intensity64=expected.slot.intensity64=word(0x80000000U|serial);
 world.table.input[serial%6]=expected.table.input[serial%6]=scenario*0x31415927U+serial;
}
static void service(u32 kind,const void *p,s32 value) {
 assert(services++<6);car_put32(&serviceTrace,kind);car_put32(&serviceTrace,p?address(p):0);
 car_put32(&serviceTrace,0x1F80007C);car_put32(&serviceTrace,(u32)value);snapshot(serviceTrace);serviceTrace+=STATE_SIZE;
}
s32 PsyqRand(void) {
 service(1,NULL,0);change(20+randoms);return word(scenario*0x9E3779B9U+randoms++*0x1234567U);
}
void DrawSparkLines(RroSpark *sparks,RroEffectWork *work,s32 speed) {
 assert(work==&world.scratch.work7C);service(2,sparks,speed);change(31);
 data[8]^=0x12345678U;control[5]^=0x80000000U;
}
void EmitCarBodyQuads(const RroEffectMesh *mesh,RroEffectWork *work) {
 assert(work==&world.scratch.work7C);service(3,mesh,0);change(32);
 data[8]^=0x23456789U;control[5]^=0x40000000U;
}
void EmitCarShadowQuads(const RroEffectMesh *mesh,RroEffectWork *work) {
 assert(work==&world.scratch.work7C);service(4,mesh,0);change(33);
 data[8]^=0x3456789AU;control[5]^=0x20000000U;
}
static void writeControl(unsigned reg,u32 value){op(0x500+reg,0,value);control[reg]=value;}
static void setTranslation(const void *p) {
 u32 values[3];memcpy(values,p,12);for(unsigned i=0;i<3;i++)writeControl(5+i,values[i]);
}
static void setIR(s32 value){op(0x408,0,(u32)value);data[8]=(u32)value;}
static void loadRGB(const void *p){u32 value;memcpy(&value,p,4);op(0x106,address(p),value);data[6]=value;}
static void depthCue(const void *p) {
 const u8 *rgb=p;u32 r=rgb[0],g=rgb[1],b=rgb[2];
 writeControl(21,r<<4);writeControl(22,g<<4);writeControl(23,b<<4);
 op(0x600,0,0x4A780010);data[22]=data[6]^(data[8]*0x01010101U+control[21]*3U+control[22]*5U+control[23]*7U);
 change(40+colors++);
}
static void saveRGB(void *p) {
 assert(colors>=1&&colors<=6&&p==&world.scratch.work7C.colors15C.words[colors]);
 op(0x216,address(p),data[22]);memcpy(p,&data[22],4);
 memcpy(&expected.scratch.work7C.colors15C.words[colors],&data[22],4);
}
#define RR_GTE_SETTR(p) setTranslation(p)
#define RR_GTE_MTC2_IR0(v) setIR(v)
#define RR_GTE_LWC2_RGB(p) loadRGB(p)
#define RR_GTE_DPCS_FC4(p) depthCue(p)
#define RR_GTE_SWC2_RGB2(p) saveRGB(p)
'''
TAIL = r'''
void run_case(unsigned number,const u8 *input,u8 *output) {
 assert(sizeof(void*)==8&&(uintptr_t)&world>UINT32_MAX);
 scenario=number;operations=services=randoms=colors=0;
 opTrace=(u32*)(output+8);serviceTrace=output+8+64*12;
 memset(output,0,OUTPUT_SIZE);memset(data,0,sizeof(data));memset(control,0,sizeof(control));
 memset(&world,0xA5,sizeof(world));const u8 *p=input;
 car_slot_load(&world.slot,p,resolve);p+=200;
 for(unsigned i=0;i<2;i++){car_model_load(&world.models[i],p,resolve);p+=0x94;}
 memcpy(world.scratch.work7C.object00.fields.translation14,p,12);p+=12;
 memcpy(&world.scratch.work7C.jitterTranslationAC,p,12);p+=12;
 memcpy(world.scratch.work7C.colors15C.words,p,28);p+=28;
 world.scratch.work7C.vehicle90=&world.vehicles[car_get32(&p)==0x80180010?0:1];
 for(unsigned i=0;i<2;i++){memcpy(&world.vehicles[i].input,p,2);p+=2;}
 world.paused=(s8)car_get32(&p);world.frame=car_get32(&p);memcpy(&world.table,p,48);expected=world;
 TransformRroVertices(&world.slot,&world.scratch.work7C);
 assert(colors==6&&randoms==((number&1)?2U:3U)&&services==randoms+3);
 if(!(number&1))expected.slot.intensity64=world.slot.intensity64;
 expected.scratch.work7C.jitterTranslationAC=world.scratch.work7C.jitterTranslationAC;
 assert(!memcmp(&world,&expected,sizeof(world)));
 memcpy(output,&operations,4);memcpy(output+4,&services,4);
 u8 *out=output+8+64*12+6*(16+STATE_SIZE);snapshot(out);out+=STATE_SIZE;
 memcpy(out,data,128);out+=128;memcpy(out,control,128);
}
'''


def main():
    os.chdir(ROOT); owner = source_owners(ROOT/'src/main/PAL')['TransformRroVertices']
    body = definition_source(owner.read_text(), 'TransformRroVertices')
    table, paused, frame = (address_of(n) for n in ('RroEffectColorPresets', 'RacePaused', 'ScreenFrame'))
    with TemporaryDirectory() as directory:
        source, library = Path(directory)/'native.c', Path(directory)/'native.dylib'
        source.write_text(PREFIX.replace('TABLE_ADDRESS', hex(table)+'U').replace('STATE_SIZE', str(STATE))+body+TAIL.replace('STATE_SIZE', str(STATE)).replace('OUTPUT_SIZE', str(OUTPUT)))
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-Iinclude', '-Itools/scripts',
                        str(source), '-o', str(library)], check=True)
        native = C.CDLL(str(library)); native.run_case.argtypes = [C.c_uint, C.c_void_p, C.c_void_p]
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN); memory.mem_map(0, 0x400000); memory.mem_map(0x1F800000, 4096)
        write = lambda a, value: memory.mem_write(a & 0x1FFFFFFF, bytes(value))
        read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
        put = lambda a, value: write(a, struct.pack('<I', value & 0xFFFFFFFF))
        get = lambda a: int.from_bytes(read(a, 4), 'little')
        executable = (ROOT/'assets/PAL/main.exe').read_bytes(); base, = struct.unpack_from('<I', executable, 0x18); write(base, executable[0x800:])
        start = address_of('TransformRroVertices')
        if os.getenv('RR_COMPILED_ELF'):
            with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                elf = ELFFile(stream); symbol = elf.get_section_by_name('.symtab').get_symbol_by_name('TransformRroVertices')[0]
                section = elf.get_section(symbol['st_shndx']); at = symbol['st_value']-section['sh_addr']; assert symbol['st_value'] == start
                write(start, section.data()[at:at+symbol['st_size']])
        entries = {address_of(n): i+1 for i, n in enumerate(('PsyqRand', 'DrawSparkLines', 'EmitCarBodyQuads', 'EmitCarShadowQuads'))}

        def store(cpu, access, address, size, value, user):
            address &= 0x1FFFFFFF
            assert any((a & 0x1FFFFFFF) <= address and address+size <= (a & 0x1FFFFFFF)+n
                       for a, n in [(SLOT+0x64, 4), (WORK+0xAC, 12), (WORK+0x160, 24), (STACK-128, 128)]), (hex(address), size)

        cpu = Cpu(memory, 0, 100, 101, 102, store); inputs = (-32768, -1, 991, 992, 993, 3039, 3040, 32767)
        paths, speeds = set(), set()
        for case in range(CASES):
            state = bytearray((case*19+i*29)&255 for i in range(STATE))
            struct.pack_into('<I', state, 0, MODELS[(case >> 2)&1]); struct.pack_into('<I', state, 0xC4, SPARKS[(case >> 3)&1])
            for off in [*(0x18+j*8 for j in range(4)), *(0x40+j*8 for j in range(4)), *(0x88+j*8 for j in range(8))]: struct.pack_into('<I', state, off, VERTICES)
            for m in range(2):
                at = 200+m*0x94; struct.pack_into('<I', state, at+12, COMMANDS)
                for off in [0x10, 0x1C, 0x28, 0x34, 0x40, 0x58, 0x64, 0x70, 0x7C, 0x88]: struct.pack_into('<3I', state, at+off, VERTICES, 3+m, COMMANDS)
            at = 200+2*0x94+12+12+28
            struct.pack_into('<IhhII', state, at, VEHICLES[(case >> 4)&1], inputs[(case >> 5)&7], inputs[(case >> 8)&7], case&1, (case*0x123457)&0xFFFFFFFF)
            incoming = C.create_string_buffer(bytes(state)); output = (C.c_uint32*((OUTPUT+3)//4))(); native.run_case(case, incoming, output); expected = bytes(output)[:OUTPUT]
            write(SLOT, state[:200])
            for i, a in enumerate(MODELS): write(a, state[200+i*0x94:200+(i+1)*0x94])
            write(WORK+0x14, state[496:508]); write(WORK+0xAC, state[508:520]); write(WORK+0x15C, state[520:548]); write(WORK+0x90, state[548:552])
            for i, a in enumerate(VEHICLES): write(a+0x1D8, state[552+i*2:554+i*2])
            write(paused, bytes([case&1])); put(frame, case*0x123457); write(table, state[564:612])
            cpu.reset(); cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]; cpu.r[4:6] = [SLOT, WORK]; cpu.r[29] = STACK; cpu.r[31] = STOP; cpu.pc = start
            d, c, ops, service_trace, randoms, colors = [0]*32, [0]*32, [], [], 0, 0

            def snapshot():
                return (read(SLOT, 200)+b''.join(read(a, 0x94) for a in MODELS)+read(WORK+0x14, 12)+read(WORK+0xAC, 12)+read(WORK+0x15C, 28)+read(WORK+0x90, 4)
                        +b''.join(read(a+0x1D8, 2) for a in VEHICLES)+struct.pack('<II', signed(read(paused, 1)[0], 8)&0xFFFFFFFF, get(frame))+read(table, 48))

            def change(serial):
                put(SLOT, MODELS[(case+serial)&1]); put(SLOT+0xC4, SPARKS[(case+serial+1)&1]); put(WORK+0x90, VEHICLES[(case+serial)&1])
                write(VEHICLES[(case+serial)&1]+0x1D8, struct.pack('<H', (case*37+serial*107)&65535)); put(frame, case*0x9E3779B9+serial)
                for i, value in enumerate([0x7FFFFFFF if case&1 else 0x80000000, 0x80000000 if case&1 else 0x7FFFFFFF, case*0x17171717+serial]): put(WORK+0x14+i*4, value)
                put(SLOT+0x64, 0x80000000|serial); put(table+(serial%6)*4, case*0x31415927+serial)

            for _ in range(2000):
                if cpu.pc == STOP: break
                if cpu.pc in entries:
                    service = entries[cpu.pc]; arg = 0 if service == 1 else cpu.r[4]; value = cpu.r[6] if service == 2 else 0
                    if service != 1: assert cpu.r[5] == WORK
                    service_trace.append(struct.pack('<4I', service, arg, WORK, value)+snapshot()); result = 0
                    if service == 1: change(20+randoms); result = (case*0x9E3779B9+randoms*0x1234567)&0xFFFFFFFF; randoms += 1
                    else:
                        change(29+service); d[8] ^= {2:0x12345678,3:0x23456789,4:0x3456789A}[service]; c[5] ^= {2:0x80000000,3:0x40000000,4:0x20000000}[service]
                    resume = cpu.r[31]
                    for reg in (*range(2, 16), 24, 25): cpu.r[reg] = 0xDEAD0000+reg
                    cpu.r[2] = result; cpu.pc = resume; continue
                instruction = get(cpu.pc); op = instruction >> 26
                if op not in (0x12, 0x32, 0x3A): cpu.step(); continue
                previous = cpu.delay; cpu.delay = None; rs, rt, rd = (instruction >> 21)&31, (instruction >> 16)&31, (instruction >> 11)&31
                if op in (0x32, 0x3A):
                    a = (cpu.r[rs]+signed(instruction, 16))&0xFFFFFFFF
                    if op == 0x32: value = get(a); d[rt] = value; ops.extend([0x100+rt, a, value])
                    else: ops.extend([0x200+rt, a, d[rt]]); cpu.store(a, d[rt], 4)
                elif instruction & 0x02000000:
                    assert instruction == 0x4A780010; ops.extend([0x600, 0, instruction])
                    d[22] = d[6]^((d[8]*0x01010101+c[21]*3+c[22]*5+c[23]*7)&0xFFFFFFFF); change(40+colors); colors += 1
                elif rs == 4: ops.extend([0x400+rd, 0, cpu.r[rt]]); d[rd] = cpu.r[rt]
                else: assert rs == 6; ops.extend([0x500+rd, 0, cpu.r[rt]]); c[rd] = cpu.r[rt]
                cpu.pc = previous if previous is not None else cpu.pc+4
            assert cpu.pc == STOP and colors == 6
            trace = struct.pack('<II', len(ops)//3, len(service_trace))+struct.pack('<'+'I'*len(ops), *ops)+bytes(MAX_OPS*12-len(ops)*4)
            trace += b''.join(service_trace)+bytes((6-len(service_trace))*(16+STATE))+snapshot()+struct.pack('<64I', *d, *c)
            assert trace == expected, (case, 'typed service arguments / complete owners / exact GTE color transfers')
            assert cpu.r[29] == STACK and cpu.r[31] == STOP
            for reg in (*range(16, 24), 28, 30): assert cpu.r[reg] == 0xBEEF0000+reg
            paths.add((case&1, inputs[(case >> 5)&7], (case >> 4)&1)); speeds.add(randoms)
        assert speeds == {2, 3} and len(paths) == 2*8*2
        print('PASS', CASES, 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
              'MIPS/native effect preparation cases; full slot/model/vehicle/spark pointers, live roots/frame, signed clamp/speed and wrapped jitter, six color transfers, service protocol, complete native owners, ABI, UBSan')


if __name__ == '__main__':
    main()
