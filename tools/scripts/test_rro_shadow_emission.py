"""Current full shadow emitter vs original/compiled MIPS and a wire oracle.

Uses actual native command/work/packet types, complete owners and high pointers.
GTE arithmetic is scripted; validates the C emission and DMA protocol, not GTE
projection accuracy, flags or timing. Retains the separate projection test.
"""
import ctypes as C
import os
from pathlib import Path
import re
import struct
import subprocess
from tempfile import TemporaryDirectory

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32

from c_functions import address_of, definition_source, source_owners
from mips_preview_engine import signed
from test_rro_shadow_projection import DEPTHS, PREFIX as PROJECTION_PREFIX
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
WORK, MESH, STREAM, VERTICES = 0x1F80007C, 0x80200010, 0x80210010, 0x80220010
STACK, STOP = 0x803F0000, 0x803FF000
CASES, STATE, MAX_EVENTS = 8192, 248+12+8192, 64
OUTPUT = 4+MAX_EVENTS*12+STATE+128
FIRST_AREAS = (0, 1, -1, -2147483648)
SECOND_AREAS = (0, -1, 1, 2147483647)
UV_WORDS = (0, 1, 0x7FFFFFFF, 0x80000000, 0x8000FFFF, 0xFFFF0000, 0xFFFFFFFF, 0x12345678)

PREFIX = PROJECTION_PREFIX.replace(
    'static struct World {',
    'typedef union {u32 words[1024];GpuPolyFT4 packets[100];} GpuArena;\nstatic struct World {', 1)
PREFIX = PREFIX.replace('u32 gpu[64];', '_Alignas(8192) GpuArena gpu[2];', 1)
PREFIX = PREFIX.replace('static u32 address(const void *pointer) {', r'''
u32 gpu_address(void){
 uintptr_t p=(uintptr_t)world.gpu;
 assert(p>UINT32_MAX&&(p&8191)==0&&(p&0xFFFFFFU)+sizeof(world.gpu)<=0x1000000U);
 return 0x80180000U;
}
static u32 address(const void *pointer) {''', 1)
PREFIX = PREFIX.replace('uintptr_t p=(uintptr_t)pointer,b;', r'''
 uintptr_t p=(uintptr_t)pointer,b=(uintptr_t)world.gpu;
 if(p>=b&&p-b<sizeof(world.gpu))return gpu_address()+(u32)(p-b);''', 1)
PREFIX = PREFIX.replace(
    's32 area=clips++?((scenario&4)?(s32)scenario*131:0):((scenario&3)?-(s32)(scenario*113):0);', r'''
  static const s32 first[]={0,1,-1,-2147483647-1},second[]={0,-1,1,2147483647};
  s32 area=clips++?second[(scenario>>10)&3]:first[(scenario>>8)&3];''', 1)
assert '_Alignas(8192)' in PREFIX and 's32 area=clips++?second' in PREFIX

TAIL = r'''
static void put(void *p,u32 value){memcpy(p,&value,4);}
static void *expectedGpu(const void *p){
 uintptr_t offset=(uintptr_t)p-(uintptr_t)world.gpu;
 assert(offset<sizeof(world.gpu));return (u8*)expected.gpu+offset;
}
static void reference(unsigned stream){
 RroEffectWork *work=&expected.scratch.work7C;
 s32 depth=depths[scenario&15];if(depth<=0)return;
 depth>>=expected.scratch.scene.depthShift60;work->depth70=depth;
 if(depth>=448)return;
 s32 first=word((u32)work->firstArea74^(u32)work->windingBC);
 s32 second=word((u32)work->secondArea78^(u32)work->windingBC);
 if(first<=0&&second>=0)return;
 RroTexturedQuadPrefix *quad=&expected.streams[stream].decoded.quad;
 u32 *head=(u32*)expectedGpu(expected.scratch.scene.view.projection.pointers.order)+128+depth+quad->orderingBias19;
 u8 *actual=expected.scratch.scene.view.projection.pointers.cursor;
 u8 *packet=expectedGpu(actual);u32 previous;memcpy(&previous,head,4);
 /* Independent wire protocol. Keep the writes in order for packet/head aliases. */
 put(packet,previous);packet[3]=9;put(head,address(actual));((u8*)head)[3]=0;
 for(unsigned i=0;i<4;i++)put(packet+8+i*8,(u32)work->projection188.shadow.xy[i]);
 put(packet+4,(u32)work->colors15C.palette.rgb160[4]);packet[7]=0x2E;
 put(packet+12,(u32)quad->uv0C);put(packet+20,(u32)quad->uv10);put(packet+28,(u32)quad->uv14);
 put(packet+36,(u32)(quad->uv14>>16));
 expected.scratch.scene.view.projection.pointers.cursor=actual+40;
}
void run_case(unsigned number,const u8 *input,u8 *output){
 installGpuMap();
 assert(sizeof(void*)==8&&(uintptr_t)&world>UINT32_MAX);
 scenario=number;events=clips=0;memset(&world,0xA5,sizeof(world));memset(data,0,sizeof(data));memset(output,0,OUTPUT_SIZE);trace=(u32*)(output+4);
 memcpy(world.streams,input,72);memcpy(world.vertices,input+72,128);
 u32 mesh[3];memcpy(mesh,input+200,12);
 world.mesh.vertices=world.vertices[(mesh[0]-0x80220010)/64];world.mesh.vertexCount=mesh[1];world.mesh.commands=world.streams[(mesh[2]-0x80210010)/36].words;
 memcpy(&world.scratch.work7C.depth70,input+212,12);memcpy(&world.scratch.work7C.windingBC,input+224,4);
 memcpy(&world.scratch.scene.depthShift60,input+228,4);memcpy(world.scratch.work7C.projection188.shadow.xy,input+232,16);
 memcpy(&world.scratch.work7C.colors15C.palette.rgb160[4],input+248,4);
 for(unsigned i=0;i<2048;i++)put((u8*)world.gpu+i*4,number*0x31415927U+i*0x17171717U);
 unsigned stream=(number>>4)&1,orderBank=(number>>6)&1;
 s32 depth=depths[number&15];if(depth>0)depth>>=world.scratch.scene.depthShift60;
 s32 bucket=128+(depth>=0&&depth<448?depth:0)+world.streams[stream].decoded.quad.orderingBias19;
 unsigned packetWord=(number&4096)?128+bucket-((number>>4)%10):900;
 unsigned packetBank=(number&4096)?orderBank:orderBank^1;
 assert(packetWord+10<=1024);
 world.scratch.scene.view.projection.pointers.cursor=(u8*)&world.gpu[packetBank].words[packetWord];
 world.scratch.scene.view.projection.pointers.order=&world.gpu[orderBank].words[128];expected=world;
 EmitCarShadowQuads(&world.mesh,&world.scratch.work7C);
 assert(clips==2);reference(stream);assert(!memcmp(&world,&expected,sizeof(world)));
 memcpy(output,&events,4);u8 *p=output+4+64*12;
 memcpy(p,world.streams,72);p+=72;memcpy(p,world.vertices,128);p+=128;
 mesh[0]=address(world.mesh.vertices);mesh[1]=world.mesh.vertexCount;mesh[2]=address(world.mesh.commands);memcpy(p,mesh,12);p+=12;
 memcpy(p,&world.scratch.work7C.depth70,12);p+=12;memcpy(p,&world.scratch.work7C.windingBC,4);p+=4;
 memcpy(p,&world.scratch.scene.depthShift60,4);p+=4;memcpy(p,world.scratch.work7C.projection188.shadow.xy,16);p+=16;
 put(p,address(world.scratch.scene.view.projection.pointers.cursor));p+=4;
 put(p,address(world.scratch.scene.view.projection.pointers.order));p+=4;
 memcpy(p,&world.scratch.work7C.colors15C.palette.rgb160[4],4);p+=4;
 memcpy(p,world.gpu,sizeof(world.gpu));p+=sizeof(world.gpu);memcpy(p,data,128);
}
'''


def main():
    os.chdir(ROOT)
    owner = source_owners(ROOT/'src/main/PAL')['EmitCarShadowQuads']
    body = re.sub(r'__asm__\("\$\d+"\)', '', definition_source(owner.read_text(), 'EmitCarShadowQuads'))
    with TemporaryDirectory() as directory:
        source, library = Path(directory)/'native.c', Path(directory)/'native.dylib'
        source.write_text(PREFIX+body+TAIL.replace('OUTPUT_SIZE', str(OUTPUT)))
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-Iinclude', str(source), str(ROOT/'src/native/gpu_address.c'), '-o', str(library)], check=True)
        native = C.CDLL(str(library)); native.run_case.argtypes = [C.c_uint, C.c_void_p, C.c_void_p]
        native.gpu_address.restype = C.c_uint32; gpu = native.gpu_address()
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0, 0x400000); memory.mem_map(0x1000000, 0x1000000); memory.mem_map(0x1F800000, 4096)
        write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b))
        read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
        put = lambda a, v: write(a, struct.pack('<I', v & 0xFFFFFFFF))
        get = lambda a: int.from_bytes(read(a, 4), 'little')
        executable = (ROOT/'assets/PAL/main.exe').read_bytes(); base, = struct.unpack_from('<I', executable, 0x18)
        write(base, executable[0x800:]); start = address_of('EmitCarShadowQuads')
        if os.getenv('RR_COMPILED_ELF'):
            with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                elf = ELFFile(stream); symbol = elf.get_section_by_name('.symtab').get_symbol_by_name('EmitCarShadowQuads')[0]
                section = elf.get_section(symbol['st_shndx']); offset = symbol['st_value']-section['sh_addr']
                assert symbol['st_value'] == start; write(start, section.data()[offset:offset+symbol['st_size']])

        def check_store(cpu, access, address, size, value, user):
            address &= 0x1FFFFFFF
            assert any((a&0x1FFFFFFF) <= address and address+size <= (a&0x1FFFFFFF)+n
                       for a, n in [(WORK+0x70, 12), (WORK+0x1A0, 16), (0x1F800000, 4), (gpu, 8192)]), (hex(address), size)

        cpu = Cpu(memory, 0, 100, 101, 102, check_store)
        paths, aliases, biases, uv_signs, uv_words, shifts = set(), set(), set(), set(), set(), set()
        for case in range(CASES):
            state = bytearray((case*19+i*29)&255 for i in range(252))
            for stream in range(2):
                struct.pack_into('<I4H', state, stream*36, 0x10003, *[(case+stream+i)&7 for i in range(4)])
                state[stream*36+29]=((case*257>>5)+stream*37)&255
                struct.pack_into('<I', state, stream*36+24, UV_WORDS[((case>>6)+stream)&7])
            stream = (case>>4)&1; order_bank = (case>>6)&1
            struct.pack_into('<3I', state, 200, VERTICES+((case>>5)&1)*64, 8, STREAM+stream*36)
            winding = 0x80000000 if case&64 else 0
            shift = (case>>5)&31; struct.pack_into('<II', state, 224, winding, shift)
            depth = DEPTHS[case&15]; shifted = depth >> shift if depth > 0 else depth
            bias = signed(state[stream*36+29], 8)
            bucket = 128+(shifted if 0 <= shifted < 448 else 0)+bias
            packet_word = 128+bucket-((case>>4)%10) if case&4096 else 900
            packet_bank = order_bank if case&4096 else order_bank^1
            packet, ordering = gpu+packet_bank*4096+packet_word*4, gpu+order_bank*4096+128*4
            incoming = C.create_string_buffer(bytes(state)); output = (C.c_uint32*((OUTPUT+3)//4))()
            native.run_case(case, incoming, output); expected = bytes(output)[:OUTPUT]
            write(STREAM, state[:72]); write(VERTICES, state[72:200]); write(MESH, state[200:212])
            write(WORK+0x70, state[212:224]); write(WORK+0xBC, state[224:228]); write(0x1F800060, state[228:232]); write(WORK+0x1A0, state[232:248]); write(WORK+0x170, state[248:252])
            put(0x1F800000, packet); put(0x1F800004, ordering)
            write(gpu, struct.pack('<2048I', *[(case*0x31415927+i*0x17171717)&0xFFFFFFFF for i in range(2048)]))
            cpu.reset(); cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
            cpu.r[4:6] = [MESH, WORK]; cpu.r[29] = STACK; cpu.r[31] = STOP; cpu.pc = start
            data, events, clips = [0]*32, [], 0
            for _ in range(1000):
                if cpu.pc == STOP: break
                instruction = get(cpu.pc); opcode = instruction >> 26
                if opcode not in (0x12, 0x32, 0x3A): cpu.step(); continue
                previous = cpu.delay; cpu.delay = None
                rs, rt = (instruction >> 21)&31, (instruction >> 16)&31
                if opcode in (0x32, 0x3A):
                    a = (cpu.r[rs]+signed(instruction, 16))&0xFFFFFFFF
                    if opcode == 0x32:
                        value = get(a); data[rt] = value; events.extend([0x100+rt, a, value])
                    else:
                        events.extend([0x200+rt, a, data[rt]]); cpu.store(a, data[rt], 4)
                        if rt == 24 and clips == 1:
                            write(STREAM+stream*36+10, struct.pack('<H', (case+5)&7))
                            put(MESH, VERTICES+(((case>>5)&1)^1)*64); put(MESH+8, STREAM+(stream^1)*36)
                else:
                    assert instruction & 0x02000000; events.extend([0x600, 0, instruction])
                    if instruction == 0x4A280030:
                        data[12]=(case*0x1234567)&0xFFFFFFFF; data[13]=~data[12]&0xFFFFFFFF; data[14]=(case*0x9E3779B9)&0xFFFFFFFF
                    elif instruction == 0x4A180001: data[14]=(case*0x31415927)&0xFFFFFFFF
                    elif instruction == 0x4B68002E: data[7]=DEPTHS[case&15]&0xFFFFFFFF
                    else:
                        assert instruction == 0x4B400006
                        area = SECOND_AREAS[(case>>10)&3] if clips else FIRST_AREAS[(case>>8)&3]
                        data[24]=(area&0xFFFFFFFF)^winding; clips += 1
                cpu.pc = previous if previous is not None else cpu.pc+4
            assert cpu.pc == STOP and clips == 2
            snapshot = read(STREAM, 72)+read(VERTICES, 128)+read(MESH, 12)+read(WORK+0x70, 12)+read(WORK+0xBC, 4)+read(0x1F800060, 4)+read(WORK+0x1A0, 16)+read(0x1F800000, 8)+read(WORK+0x170, 4)+read(gpu, 8192)
            actual = struct.pack('<I', len(events)//3)+struct.pack('<'+'I'*len(events), *events)+bytes(MAX_EVENTS*12-len(events)*4)+snapshot+struct.pack('<32I', *data)
            assert actual == expected, (case, 'full emission, DMA word protocol, aliases and complete owners')
            for reg in (*range(16, 24), 28, 30): assert cpu.r[reg] == 0xBEEF0000+reg
            assert cpu.r[29] == STACK and cpu.r[31] == STOP
            if get(0x1F800000) != packet:
                assert get(0x1F800000) == packet+40; paths.add('emitted'); biases.add(bias)
                uv_signs.add(signed(get(STREAM+stream*36+24), 32)<0)
                uv_words.add(get(STREAM+stream*36+24)); shifts.add(shift)
                aliases.add((case>>4)%10 if case&4096 else 'separate')
            else: paths.add('nonpositive' if depth <= 0 else 'too_far' if shifted >= 448 else 'backface')
        assert paths == {'emitted', 'nonpositive', 'too_far', 'backface'} and aliases == set(range(10))|{'separate'}
        assert biases == set(range(-128, 128)) and uv_signs == {False, True}, (len(biases), uv_signs)
        assert uv_words == set(UV_WORDS) and shifts == set(range(32)), (uv_words, shifts)
        print('PASS', CASES, 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
              'MIPS/native full shadow cases; full pointers, signed biases, all packet/head aliases, DMA low24 links, packed XY/UV/color, signed fourth UV, complete owners, ABI, UBSan; scripted GTE math')


if __name__ == '__main__':
    main()
