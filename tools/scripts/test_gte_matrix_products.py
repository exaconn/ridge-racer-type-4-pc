"""Real SDK matrix bodies with GTE transfer adapters and original MIPS traces.

The adapter models packed columns and saturated Q12 products. It does not model
GTE FLAG, pipeline timing or undocumented coprocessor behavior.
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
FUNCTIONS = ('MulMatrix0', 'MulRotMatrix0', 'MulMatrix', 'MulMatrix2', 'SetLightMatrix')
BASE, STACK, STOP = 0x80200010, 0x803F0000, 0x803FF000
STATE_SIZE, MAX_EVENTS, CASES = 512, 32, 4096
OUTPUT_SIZE = 8+MAX_EVENTS*16+STATE_SIZE


def adapted_body(source, name):
    body = definition_source(source, name)
    body = re.sub(r'\s+__asm__\("\$\d+"\)', '', body)

    def transfer(match):
        command = ast.literal_eval('"'+match[1]+'"').strip()
        operands = re.findall(r'"[+=]?[r0]"\((\w+)\)', match[2])
        if not command:
            assert len(operands) == 2, (name, match[0])
            return operands[0]+' = '+operands[1]+';'
        instruction = re.fullmatch(r'(ctc2|mtc2|mfc2) %0, \$(\d+)', command)
        if instruction:
            kind, register = instruction[1], int(instruction[2])
            assert len(operands) == 1
            if kind == 'mfc2':return f'{operands[0]} = gte_read({register});'
            return f'gte_write({0 if kind == "ctc2" else 1}, {register}, (u32){operands[0]});'
        store = re.fullmatch(r'swc2 \$(\d+), (\d+)\(%0\)', command)
        assert store and len(operands) == 1, (name, command, operands)
        return f'gte_store((u8*){operands[0]}+{store[2]}, data[{store[1]}]);'

    body = re.sub(r'__asm__\s+volatile\s*\(\s*"((?:\\.|[^"\\])*)"\s*(.*?)\);', transfer, body, flags=re.S)
    assert '__asm__' not in body, (name, body)
    return body


PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "psyq/libgte/gtemat.h"
#include "psyq/libgte/gtemat2.h"
#include "psyq/libgte/gtemat3.h"
#include "psyq/libgte/gtemat4.h"
#include "psyq/libgte/gtemat7.h"
static struct {u8 before[16];MatrixWords32 matrices[8];u8 after[16];} arena;
static u32 control[32],data[32],events[32][4];static unsigned count;
static void event(u32 kind,u32 a,u32 b,u32 c){assert(count<32);u32 *p=events[count++];p[0]=kind;p[1]=a;p[2]=b;p[3]=c;}
static void gte_write(unsigned kind,unsigned reg,u32 value){event(kind,reg,value,0);(kind?data:control)[reg]=value;}
static u32 gte_read(unsigned reg){event(2,reg,data[reg],0);return data[reg];}
static u32 address(const void *p){uintptr_t delta=(uintptr_t)p-(uintptr_t)arena.matrices;assert(delta<256);return 0x80200010U+(u32)delta;}
static void gte_store(void *p,u32 value){event(4,address(p),value,0);assert(address(p)<=0x8020010CU);memcpy(p,&value,4);}
static s16 half(u16 bits){s16 x;memcpy(&x,&bits,2);return x;}
static void multiply(void) {
 unsigned row,col;int64_t sum,scaled;u32 packed[2]={data[0],data[1]};s16 vector[3];
 event(3,0x4A486012,0,0);vector[0]=half(packed[0]);vector[1]=half(packed[0]>>16);vector[2]=half(packed[1]);
 for(row=0;row<3;row++) {
  sum=0;for(col=0;col<3;col++){unsigned n=row*3+col;s16 coefficient=half(control[n/2]>>((n&1)*16));sum+=(int64_t)coefficient*vector[col];}
  scaled=sum>=0?sum/4096:-((-sum+4095)/4096);data[25+row]=(u32)scaled;
  if(scaled>32767)scaled=32767;if(scaled<-32768)scaled=-32768;data[9+row]=(u32)scaled;
 }
}
#define RR_GTE_MASK_HI16(x) ((x)&=0xFFFF0000U)
#define RR_GTE_MVMVA_ROT_V0_1NOP() multiply()
#define RR_GTE_SW_AT(value,base,index) gte_store((u8*)(base)+(index)*4,(u32)(value))
#define RR_GTE_CTC2(value,reg) gte_write(0,reg,(u32)(value))
#define RR_GTE_MTC2(value,reg) gte_write(1,reg,(u32)(value))
#define RR_GTE_MFC2(dst,reg) ((dst)=gte_read(reg))
#define RR_GTE_SWC2_AT(reg,off,base) gte_store((u8*)(base)+(off),data[reg])
#define RR_GTE_SETLIGHT_5W(p) do { const MatrixWords32 *_m=(p); gte_write(0,8,_m->w[0]); gte_write(0,9,_m->w[1]); gte_write(0,10,_m->w[2]); gte_write(0,11,_m->w[3]); gte_write(0,12,_m->w[4]); } while (0)
'''
TAIL = r'''
void run_case(unsigned kind,unsigned first,unsigned right,unsigned destination,const u8 *input,u8 *output) {
 MatrixWords32 *result=0;unsigned i;u32 pointer;
 assert(sizeof(void*)==8&&(uintptr_t)arena.matrices>UINT32_MAX);
 memset(&arena,0xA5,sizeof(arena));memcpy(arena.matrices,input,256);
 memcpy(control,input+256,128);memcpy(data,input+384,128);memset(events,0,sizeof(events));count=0;
 switch(kind){
 case 0:result=MulMatrix0(&arena.matrices[first],&arena.matrices[right],&arena.matrices[destination]);break;
 case 1:result=MulRotMatrix0(&arena.matrices[right],&arena.matrices[destination]);break;
 case 2:result=MulMatrix(&arena.matrices[first],&arena.matrices[right]);break;
 case 3:result=MulMatrix2(&arena.matrices[first],&arena.matrices[right]);break;
 case 4:SetLightMatrix(&arena.matrices[first]);break;
 default:assert(0);
 }
 pointer=result?address(result):0;memcpy(output,&pointer,4);memcpy(output+4,&count,4);memcpy(output+8,events,sizeof(events));
 output+=8+sizeof(events);memcpy(output,arena.matrices,256);memcpy(output+256,control,128);memcpy(output+384,data,128);
 for(i=0;i<16;i++)assert(arena.before[i]==0xA5&&arena.after[i]==0xA5);
}
'''


def main():
    os.chdir(ROOT)
    owners = source_owners(ROOT/'src/main/PAL')
    bodies = '\n'.join(adapted_body(owners[n].read_text(), n) for n in FUNCTIONS)
    with TemporaryDirectory() as temporary:
        source, library = Path(temporary)/'native.c', Path(temporary)/'native.dylib'
        source.write_text(PREFIX+bodies+TAIL)
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
                        '-Iinclude', str(source), '-o', str(library)], check=True)
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

        def store(cpu, access, address, size, value, user):
            address &= 0x1FFFFFFF;start = BASE & 0x1FFFFFFF
            if start <= address and address+size <= start+256:
                assert size == 4;trace.append((4, BASE+address-start, value & 0xFFFFFFFF, 0))
            else:
                assert (STACK & 0x1FFFFFFF)-32 <= address and address+size <= (STACK & 0x1FFFFFFF)

        class MatrixCpu(Cpu):
            def step(self):
                instruction = self.load(self.pc, 4);op = instruction >> 26
                if op not in (0x12, 0x3A):return super().step()
                previous = self.delay;self.delay = None;rt = (instruction >> 16) & 31
                if op == 0x3A:
                    rs = (instruction >> 21) & 31;offset = (instruction & 32767)-(instruction & 32768)
                    self.store((self.r[rs]+offset) & 0xFFFFFFFF, data[rt], 4)
                elif instruction == 0x4A486012:
                    trace.append((3, instruction, 0, 0))
                    matrix = struct.unpack('<9h', struct.pack('<5I', *control[:5])[:18])
                    vector = struct.unpack('<2h', struct.pack('<I', data[0]))+(C.c_int16(data[1]).value,)
                    for row in range(3):
                        product = sum(matrix[row*3+j]*vector[j] for j in range(3))//4096
                        data[25+row] = product & 0xFFFFFFFF;data[9+row] = min(32767, max(-32768, product)) & 0xFFFFFFFF
                else:
                    rs = (instruction >> 21) & 31;rd = (instruction >> 11) & 31
                    if rs in (4, 6):
                        kind = 0 if rs == 6 else 1;value = self.r[rt]
                        trace.append((kind, rd, value, 0));(control if kind == 0 else data)[rd] = value
                    else:
                        assert rs == 0, hex(instruction);trace.append((2, rd, data[rd], 0));self.r[rt] = data[rd]
                self.r[0] = 0;self.pc = previous if previous is not None else self.pc+4

        cpu = MatrixCpu(memory, 0, 100, 101, 102, store)
        rng = random.Random(0x90914);profiles = (0, 1, -1, 4096, -4096, 32767, -32768)
        layouts = set();saturated = 0
        for function, name in enumerate(FUNCTIONS):
            for case in range(CASES):
                initial = bytearray(rng.randbytes(STATE_SIZE));first, right, destination = (case >> 6) % 8, (case >> 3) % 8, case % 8
                if case < 2048:
                    for matrix in range(8):
                        for cell in range(9):struct.pack_into('<h', initial, matrix*32+cell*2, profiles[(case//512+matrix+cell) % len(profiles)])
                source_buffer = C.create_string_buffer(bytes(initial));output = (C.c_ubyte*OUTPUT_SIZE)()
                native.run_case(function, first, right, destination, source_buffer, output);expected = bytes(output)
                pointer, events = struct.unpack_from('<II', expected);expected_state = expected[8+MAX_EVENTS*16:]
                assert events <= MAX_EVENTS
                write(BASE-16, b'\xA5'*16+initial[:256]+b'\xA5'*16)
                control[:] = struct.unpack_from('<32I', initial, 256);data[:] = struct.unpack_from('<32I', initial, 384);trace.clear()
                cpu.reset();cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
                cpu.r[4:7] = [BASE+first*32, BASE+right*32, BASE+destination*32]
                if function == 1:cpu.r[4:6] = [BASE+right*32, BASE+destination*32]
                cpu.r[29] = STACK;cpu.r[31] = STOP;cpu.pc = address_of(name)
                for instruction in range(1000):
                    if cpu.pc == STOP:break
                    cpu.step()
                assert cpu.pc == STOP and len(trace) == events, (name, case, hex(cpu.pc), len(trace), events)
                assert b''.join(struct.pack('<4I', *row) for row in trace) == expected[8:8+events*16], (name, case, 'protocol')
                actual = read(BASE, 256)+struct.pack('<32I', *control)+struct.pack('<32I', *data)
                assert actual == expected_state, (name, case, 'complete state')
                if function != 4:assert cpu.r[2] == pointer, (name, case, 'return pointer')
                assert cpu.r[29] == STACK and cpu.r[31] == STOP
                for register in (*range(16, 24), 28, 30):assert cpu.r[register] == 0xBEEF0000+register
                assert read(BASE-16, 16) == b'\xA5'*16 and read(BASE+256, 16) == b'\xA5'*16
                independent = bytearray(initial[:256])
                if function == 4:
                    assert pointer == 0 and actual[:256] == independent
                    assert control[8:13] == list(struct.unpack_from('<5I', initial, first*32))
                else:
                    left = struct.unpack('<9h', initial[256:274]) if function == 1 else struct.unpack_from('<9h', initial, first*32)
                    other = struct.unpack_from('<9h', initial, right*32)
                    products = [sum(left[row*3+k]*other[k*3+column] for k in range(3))//4096
                                for row in range(3) for column in range(3)]
                    saturated += sum(abs(n) > 32767 for n in products)
                    values = [min(32767, max(-32768, n)) for n in products]
                    out = first if function == 2 else right if function == 3 else destination
                    struct.pack_into('<9h', independent, out*32, *values)
                    struct.pack_into('<I', independent, out*32+16, values[8] & 0xFFFFFFFF)
                    assert actual[:256] == independent and pointer == BASE+out*32, (name, case, 'independent saturated product')
                    if function == 0:layouts.add((function, first, right, destination))
                    elif function == 1:layouts.add((function, right, destination))
                    else:layouts.add((function, first, right))
        assert len(layouts) == 512+3*64 and saturated
        print('PASS', len(FUNCTIONS)*CASES, ('compiled' if os.getenv('RR_COMPILED_ELF') else 'original'),
              'MIPS/native matrix cases; all argument-owner aliases among eight matrices, actual return pointers, whole records/translation, transfer/store protocol, saturation, ABI, guards and UBSan')


if __name__ == '__main__':
    main()
