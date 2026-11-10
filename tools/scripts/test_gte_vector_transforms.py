"""Actual vector-transform C bodies versus original/compiled MIPS and arithmetic.

Only coprocessor transfers/commands and empty compiler constraints are adapted.
The model covers the documented rotation/MAC/IR results used by these routines;
GTE FLAG, timing and undocumented coprocessor behavior are outside this test.
"""
import ast
import ctypes as C
import os
from pathlib import Path
import random
import re
import struct
import subprocess
from tempfile import TemporaryDirectory

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN

from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
FUNCTIONS = ('ApplyMatrix', 'ApplyMatrixSV', 'ApplyMatrixLV', 'ApplyRotMatrix')
BASE, STACK, STOP = 0x80200010, 0x803F0000, 0x803FF000
STATE_SIZE, MAX_EVENTS, CASES = 512, 64, 4096
OUTPUT_SIZE = 8 + MAX_EVENTS * 16 + STATE_SIZE
COMMANDS = (0x4A486012, 0x4A41E012, 0x4A49E012)


def adapted_body(source, name):
    body = definition_source(source, name)
    body = re.sub(r'\s+__asm__\("\$\d+"\)', '', body)
    body = body.replace('register int zero;', 'int zero = 0;')

    def transfer(match):
        command = ast.literal_eval('"' + match[1] + '"').strip()
        operands = re.findall(r'"[+=]?[r0]"\((\w+)\)', match[2])
        if not command:
            returned = re.search(r'"=r"\((\w+)\)\s*:\s*"0"\((\w+)\)', match[2])
            if returned:
                return f'{returned[1]} = {returned[2]};'
            assert operands and all(x in {'t0', 't1', 't2', 't3', 't4', 't5'} for x in operands), match[0]
            return ';'
        lines = []
        for instruction in command.splitlines():
            instruction = instruction.strip()
            if instruction == 'nop':
                continue
            encoded = re.fullmatch(r'\.word (0x[0-9A-Fa-f]+)', instruction)
            if encoded:
                assert int(encoded[1], 16) in COMMANDS
                lines.append(f'multiply({encoded[1]});')
                continue
            move = re.fullmatch(r'(ctc2|mtc2|mfc2) %0, \$(\d+)', instruction)
            if move:
                kind, reg = move[1], int(move[2])
                assert len(operands) == 1
                if kind == 'mfc2':
                    lines.append(f'{operands[0]} = (s32)gte_read({reg});')
                else:
                    lines.append(f'gte_write({0 if kind == "ctc2" else 1}, {reg}, (u32){operands[0]});')
                continue
            memory = re.fullmatch(r'(lwc2|swc2) \$(\d+), (\d+)\(%0\)', instruction)
            assert memory and len(operands) == 1, (name, instruction, operands)
            kind, reg, offset = memory[1], int(memory[2]), int(memory[3])
            pointer = f'(u8*){operands[0]}+{offset}'
            if kind == 'lwc2':
                lines.append(f'gte_load({reg}, {pointer});')
            else:
                lines.append(f'gte_store({pointer}, data[{reg}], 4);')
        return '\n'.join(lines)

    body = re.sub(r'__asm__\s+volatile\s*\(\s*"((?:\\.|[^"\\])*)"\s*(.*?)\);', transfer, body, flags=re.S)
    assert '__asm__' not in body, (name, body)
    return body


PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "psyq/libgte/gtemat3.h"
#include "psyq/libgte/gtemat5.h"
typedef union {MatrixWords32 matrix;Vec3s8 packed;Vec3i vector;u32 words[8];} Payload;
static struct {u8 before[16];Payload owners[8];u8 after[16];} arena;
static u32 control[32],data[32],events[64][4];static unsigned count;
static void event(u32 kind,u32 a,u32 b,u32 c){assert(count<64);u32 *p=events[count++];p[0]=kind;p[1]=a;p[2]=b;p[3]=c;}
static u32 address(const void *p){uintptr_t delta=(uintptr_t)p-(uintptr_t)arena.owners;assert(delta<256);return 0x80200010U+(u32)delta;}
static void gte_write(unsigned kind,unsigned reg,u32 value){event(kind,reg,value,0);(kind?data:control)[reg]=value;}
static u32 gte_read(unsigned reg){event(2,reg,data[reg],0);return data[reg];}
static void gte_load(unsigned reg,const void *p){u32 value;assert(address(p)<=0x8020010CU);memcpy(&value,p,4);gte_write(1,reg,value);}
static void gte_store(void *p,u32 value,unsigned size){assert(size==2||size==4);assert(address(p)+size<=0x80200110U);event(size==4?4:5,address(p),size==4?value:value&65535,0);memcpy(p,&value,size);}
static s16 half(u16 bits){s16 value;memcpy(&value,&bits,2);return value;}
static void multiply(u32 command){
 s16 vector[3];unsigned row,col;int64_t sum,scaled;event(3,command,0,0);
 if(command==0x4A486012){vector[0]=half(data[0]);vector[1]=half(data[0]>>16);vector[2]=half(data[1]);}
 else {assert(command==0x4A41E012||command==0x4A49E012);for(row=0;row<3;row++)vector[row]=half(data[9+row]);}
 for(row=0;row<3;row++){
  sum=0;for(col=0;col<3;col++){unsigned n=row*3+col;sum+=(int64_t)half(control[n/2]>>((n&1)*16))*vector[col];}
  scaled=command==0x4A41E012?sum:(sum>=0?sum/4096:-((-sum+4095)/4096));data[25+row]=(u32)scaled;
  if(scaled>32767)scaled=32767;if(scaled<-32768)scaled=-32768;data[9+row]=(u32)scaled;
 }
}
#define RR_GTE_MVMVA_ROT_V0_1NOP() multiply(0x4A486012)
#define RR_GTE_SH_AT(value,base,index) gte_store((u8*)(base)+(index)*2,(u32)(value),2)
#define RR_GTE_CTC2(value,reg) gte_write(0,reg,(u32)(value))
#define RR_GTE_MTC2(value,reg) gte_write(1,reg,(u32)(value))
#define RR_GTE_MFC2(dst,reg) ((dst)=(s32)gte_read(reg))
#define RR_GTE_CMD_1NOP(cmd) multiply(cmd)
#define RR_GTE_LWC2_PAIR_AT(r0,off0,r1,off1,base) do { gte_load(r0,(const u8*)(base)+(off0)); gte_load(r1,(const u8*)(base)+(off1)); } while (0)
#define RR_GTE_SWC2_TRIPLE_AT(r0,off0,r1,off1,r2,off2,base) do { gte_store((u8*)(base)+(off0),data[r0],4); gte_store((u8*)(base)+(off1),data[r1],4); gte_store((u8*)(base)+(off2),data[r2],4); } while (0)
#define RR_GTE_SETROT_5W(p) do { const MatrixWords32 *_m=(p); gte_write(0,0,_m->w[0]); gte_write(0,1,_m->w[1]); gte_write(0,2,_m->w[2]); gte_write(0,3,_m->w[3]); gte_write(0,4,_m->w[4]); } while (0)
#define RR_GTE_LDV0_2W(p) do { const Vec3s8 *_v=(p); gte_write(1,0,_v->words[0]); gte_write(1,1,_v->words[1]); } while (0)
#define RR_GTE_STORE_IR_S16_T012(p) do { u32 _t0=gte_read(9),_t1=gte_read(10),_t2=gte_read(11); RR_GTE_SH_AT(_t0,p,0); RR_GTE_SH_AT(_t1,p,1); RR_GTE_SH_AT(_t2,p,2); } while (0)
'''
TAIL = r'''
void run_case(unsigned kind,unsigned matrix,unsigned input,unsigned destination,const u8 *initial,u8 *output){
 void *result=0;unsigned i;u32 pointer;
 assert(sizeof(Payload)==32&&sizeof(void*)==8&&(uintptr_t)arena.owners>UINT32_MAX);
 memset(&arena,0xA5,sizeof(arena));memcpy(arena.owners,initial,256);memcpy(control,initial+256,128);memcpy(data,initial+384,128);
 memset(events,0,sizeof(events));count=0;
 switch(kind){
 case 0:result=ApplyMatrix(&arena.owners[matrix].matrix,&arena.owners[input].packed,&arena.owners[destination].vector);break;
 case 1:result=ApplyMatrixSV(&arena.owners[matrix].matrix,&arena.owners[input].packed,arena.owners[destination].packed.halves);break;
 case 2:result=ApplyMatrixLV(&arena.owners[matrix].matrix,&arena.owners[input].vector,&arena.owners[destination].vector);break;
 case 3:result=ApplyRotMatrix(&arena.owners[input].packed,&arena.owners[destination].vector);break;
 default:assert(0);
 }
 pointer=address(result);memcpy(output,&pointer,4);memcpy(output+4,&count,4);memcpy(output+8,events,sizeof(events));
 output+=8+sizeof(events);memcpy(output,arena.owners,256);memcpy(output+256,control,128);memcpy(output+384,data,128);
 for(i=0;i<16;i++)assert(arena.before[i]==0xA5&&arena.after[i]==0xA5);
}
'''


def main():
    os.chdir(ROOT)
    owners = source_owners(ROOT/'src/main/PAL')
    bodies = '\n'.join(adapted_body(owners[name].read_text(), name) for name in FUNCTIONS)
    with TemporaryDirectory() as temporary:
        source, library = Path(temporary)/'native.c', Path(temporary)/'native.dylib'
        source.write_text(PREFIX+bodies+TAIL)
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-Iinclude',
                        str(source), '-o', str(library)], check=True)
        native = C.CDLL(str(library));native.run_case.restype = None
        native.run_case.argtypes = [C.c_uint]*4+[C.c_void_p, C.c_void_p]
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN);memory.mem_map(0, 0x400000)
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
        trace, control, data = [], [], []
        function = destination = 0

        def store(cpu, access, address, size, value, user):
            address &= 0x1FFFFFFF;start = BASE & 0x1FFFFFFF
            if start <= address < start+256:
                footprint = 6 if function == 1 else 12
                assert start+destination*32 <= address and address+size <= start+destination*32+footprint
                assert size == (2 if function == 1 else 4)
                if function != 2:
                    trace.append((5 if size == 2 else 4, BASE+address-start, value & ((1 << (size*8))-1), 0))
            else:
                assert (STACK & 0x1FFFFFFF)-64 <= address and address+size <= (STACK & 0x1FFFFFFF)

        class VectorCpu(Cpu):
            def step(self):
                instruction = self.load(self.pc, 4);op = instruction >> 26
                if op not in (0x12, 0x32, 0x3A):
                    return super().step()
                previous = self.delay;self.delay = None;rt = (instruction >> 16) & 31
                if op in (0x32, 0x3A):
                    rs = (instruction >> 21) & 31;offset = (instruction & 32767)-(instruction & 32768)
                    pointer = (self.r[rs]+offset) & 0xFFFFFFFF
                    if op == 0x3A:
                        self.store(pointer, data[rt], 4)
                    else:
                        data[rt] = self.load(pointer, 4);trace.append((1, rt, data[rt], 0))
                elif instruction in COMMANDS:
                    trace.append((3, instruction, 0, 0))
                    matrix = struct.unpack('<9h', struct.pack('<5I', *control[:5])[:18])
                    vector = (struct.unpack('<2h', struct.pack('<I', data[0]))+(C.c_int16(data[1]).value,)
                              if instruction == COMMANDS[0] else tuple(C.c_int16(data[9+j]).value for j in range(3)))
                    for row in range(3):
                        product = sum(matrix[row*3+j]*vector[j] for j in range(3))
                        if instruction != COMMANDS[1]:product //= 4096
                        data[25+row] = product & 0xFFFFFFFF
                        data[9+row] = min(32767, max(-32768, product)) & 0xFFFFFFFF
                else:
                    rs = (instruction >> 21) & 31;rd = (instruction >> 11) & 31
                    if rs in (4, 6):
                        kind = 0 if rs == 6 else 1;value = self.r[rt]
                        trace.append((kind, rd, value, 0));(control if kind == 0 else data)[rd] = value
                    else:
                        assert rs == 0, hex(instruction)
                        trace.append((2, rd, data[rd], 0));self.r[rt] = data[rd]
                self.r[0] = 0;self.pc = previous if previous is not None else self.pc+4

        cpu = VectorCpu(memory, 0, 100, 101, 102, store)
        rng = random.Random(0x90B04);layouts = set();saturated = extremes = 0
        profiles = (0, 1, -1, 4096, -4096, 32767, -32768)
        wide = (0, 1, -1, 32767, -32768, 32768, -32769, 65535, -65536, 0x7FFFFFFF, -0x80000000)
        for function, name in enumerate(FUNCTIONS):
            for case in range(CASES):
                initial = bytearray(rng.randbytes(STATE_SIZE))
                matrix, input_owner, destination = (case >> 6) % 8, (case >> 3) % 8, case % 8
                if case < 2048:
                    for owner in range(8):
                        for cell in range(9):struct.pack_into('<h', initial, owner*32+cell*2, profiles[(case//512+owner+cell) % len(profiles)])
                    if function == 2:
                        for cell in range(3):struct.pack_into('<i', initial, input_owner*32+cell*4, wide[(case+cell) % len(wide)])
                    elif case % 2 == 0:
                        struct.pack_into('<3h', initial, input_owner*32, *(profiles[(case+j) % len(profiles)] for j in range(3)))
                    if function == 3:
                        for cell in range(9):struct.pack_into('<h', initial, 256+cell*2, profiles[(case//512+cell) % len(profiles)])
                output = (C.c_ubyte*OUTPUT_SIZE)();source_buffer = C.create_string_buffer(bytes(initial))
                native.run_case(function, matrix, input_owner, destination, source_buffer, output)
                expected = bytes(output);pointer, events = struct.unpack_from('<II', expected)
                expected_state = expected[8+MAX_EVENTS*16:];assert events <= MAX_EVENTS
                write(BASE-16, b'\xA5'*16+initial[:256]+b'\xA5'*16)
                control[:] = struct.unpack_from('<32I', initial, 256);data[:] = struct.unpack_from('<32I', initial, 384);trace.clear()
                cpu.reset();cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
                cpu.r[4:7] = [BASE+matrix*32, BASE+input_owner*32, BASE+destination*32]
                if function == 3:cpu.r[4:6] = [BASE+input_owner*32, BASE+destination*32]
                cpu.r[29] = STACK;cpu.r[31] = STOP;cpu.pc = address_of(name)
                for instruction in range(1000):
                    if cpu.pc == STOP:break
                    cpu.step()
                assert cpu.pc == STOP and len(trace) == events, (name, case, hex(cpu.pc), len(trace), events)
                assert b''.join(struct.pack('<4I', *row) for row in trace) == expected[8:8+events*16], (name, case, 'protocol')
                actual = read(BASE, 256)+struct.pack('<32I', *control)+struct.pack('<32I', *data)
                assert actual == expected_state, (name, case, 'complete state')
                assert cpu.r[2] == pointer == BASE+destination*32, (name, case, 'return pointer')
                assert cpu.r[29] == STACK and cpu.r[31] == STOP
                for register in (*range(16, 24), 28, 30):assert cpu.r[register] == 0xBEEF0000+register
                assert read(BASE-16, 16) == b'\xA5'*16 and read(BASE+256, 16) == b'\xA5'*16
                coefficients = struct.unpack_from('<9h', initial, 256 if function == 3 else matrix*32)
                if function == 2:
                    vector = struct.unpack_from('<3i', initial, input_owner*32)
                    high = [C.c_int16((-1 if x < 0 else 1)*(abs(x)//32768)).value for x in vector]
                    low = [(-1 if x < 0 else 1)*(abs(x) % 32768) for x in vector]
                    values = [((sum(coefficients[row*3+j]*high[j] for j in range(3)) << 3)
                               +sum(coefficients[row*3+j]*low[j] for j in range(3))//4096) & 0xFFFFFFFF for row in range(3)]
                    extremes += -0x80000000 in vector
                else:
                    vector = struct.unpack_from('<3h', initial, input_owner*32)
                    products = [sum(coefficients[row*3+j]*vector[j] for j in range(3))//4096 for row in range(3)]
                    saturated += sum(x > 32767 or x < -32768 for x in products)
                    values = products if function == 0 else [min(32767, max(-32768, x)) for x in products]
                independent = bytearray(initial[:256])
                if function == 1:struct.pack_into('<3h', independent, destination*32, *values)
                else:struct.pack_into('<3I', independent, destination*32, *(x & 0xFFFFFFFF for x in values))
                assert actual[:256] == independent, (name, case, 'independent arithmetic and untouched bytes')
                layouts.add((function, input_owner, destination) if function == 3 else (function, matrix, input_owner, destination))
        assert len(layouts) == 3*512+64 and saturated and extremes
        print('PASS', len(FUNCTIONS)*CASES, 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
              'MIPS/native vector cases; every argument-owner alias, MAC versus saturated IR, full-word extremes, actual return pointers, whole owners, exact transfers, output footprint, ABI, guards and UBSan')


if __name__ == '__main__':
    main()
