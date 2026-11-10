"""Shared RotMatrix ABI against the actual original hand-written routine.

The native implementation here is an independent contract witness, not a
replacement for the production assembly. Checks all signed halfword angles
on each axis, mixed angles, real overlapping union owners and preserved bytes.
"""
import ctypes as C
from pathlib import Path
import struct
import subprocess
from tempfile import TemporaryDirectory

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from unicorn.mips_const import UC_MIPS_REG_A0, UC_MIPS_REG_A1, UC_MIPS_REG_V0, UC_MIPS_REG_RA, UC_MIPS_REG_SP

from c_functions import address_of

ROOT = Path(__file__).resolve().parents[2]
ENTRY, ARENA, STOP, STACK = 0x80091844, 0x80200010, 0x803FF000, 0x803F0000
TABLE = 0x800A7260
SOURCE = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "psyq/libgte/rotmat.h"
typedef union {MatrixWords32 matrix;Vec3sFields8 vectors[4];} RotationOwner;
static struct {u8 before[16];RotationOwner owner;u8 after[16];Vec3sFields8 input;u8 guard[16];} arena;
static u32 table[4096];
static s32 half(u32 x){return (x&32768)?(s32)(x&65535)-65536:(s32)(x&65535);}
static void trig(s16 angle,s32 *sine,s32 *cosine){
 s32 a=angle;u32 v=table[(a<0?-a:a)&4095];
 *sine=half(v);if(a<0)*sine=-*sine;*cosine=half(v>>16);
}
static s32 product(s32 a,s32 b){return (s32)(((int64_t)a*b)>>12);}
MatrixWords32 *RotMatrix(const Vec3sFields8 *angles,MatrixWords32 *matrix){
 s32 sx,cx,sy,cy,sz,cz;
 trig(angles->x,&sx,&cx);trig(angles->y,&sy,&cy);trig(angles->z,&sz,&cz);
 s32 a=product(cz,-sy),b=product(sz,-sy);
 s32 result[9]={product(cz,cy),product(-sz,cy),sy,
  product(sz,cx)-product(a,sx),product(cz,cx)+product(b,sx),product(-cy,sx),
  product(sz,sx)+product(a,cx),product(cz,sx)-product(b,cx),product(cy,cx)};
 for(unsigned i=0;i<9;i++)matrix->halves[i]=(u16)result[i];
 return matrix;
}
void init_table(const void *p){memcpy(table,p,sizeof(table));}
void run_case(u32 seed,unsigned overlap,s32 x,s32 y,s32 z,u8 *before,u8 *after){
 assert(sizeof(void*)==8&&(uintptr_t)&arena>UINT32_MAX);
 assert(sizeof(arena)==88&&offsetof(__typeof__(arena),input)==64);
 for(unsigned i=0;i<sizeof(arena);i++)((u8*)&arena)[i]=(u8)(seed*19+i*29);
 Vec3sFields8 *input=overlap<4?&arena.owner.vectors[overlap]:&arena.input;
 input->x=x;input->y=y;input->z=z;
 memcpy(before,&arena,sizeof(arena));
 assert(RotMatrix(input,&arena.owner.matrix)==&arena.owner.matrix);
 memcpy(after,&arena,sizeof(arena));
 assert(!memcmp(before,&arena,16)&&!memcmp(before+34,(u8*)&arena+34,sizeof(arena)-34));
}
'''


def main():
    assert address_of('RotMatrix') == ENTRY
    executable = (ROOT/'assets/PAL/main.exe').read_bytes()
    base, = struct.unpack_from('<I',executable,0x18)
    table = executable[0x800+TABLE-base:0x800+TABLE-base+16384]
    assert len(table)==16384
    memory = Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN)
    memory.mem_map(0,0x400000);memory.mem_write(base&0x1FFFFFFF,executable[0x800:])
    with TemporaryDirectory() as directory:
        source,library = Path(directory)/'native.c',Path(directory)/'native.dylib'
        source.write_text(SOURCE)
        subprocess.run(['cc','-std=gnu11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                        '-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I'+str(ROOT/'include'),
                        str(source),'-o',str(library)],check=True)
        native=C.CDLL(str(library));native.init_table.argtypes=[C.c_void_p]
        native.run_case.argtypes=[C.c_uint32,C.c_uint,C.c_int32,C.c_int32,C.c_int32,C.c_void_p,C.c_void_p]
        native.init_table(C.create_string_buffer(table))
        before,after=C.create_string_buffer(88),C.create_string_buffer(88)
        cases=[]
        for axis in range(3):
            for angle in range(-32768,32768):
                values=[0,0,0];values[axis]=angle;cases.append(tuple(values))
        seed=0x91844
        for i in range(4096):
            values=[]
            for j in range(3):
                seed=(seed*1664525+1013904223)&0xFFFFFFFF
                values.append(C.c_int16(seed).value)
            cases.append(tuple(values))
        for version in ('original','linked'):
            if version=='linked':
                with (ROOT/'build/PAL/main.elf').open('rb') as stream:
                    elf=ELFFile(stream);symbol=elf.get_section_by_name('.symtab').get_symbol_by_name('RotMatrix')[0]
                    assert symbol['st_value']==ENTRY and symbol['st_size']==0x28C
                    section=elf.get_section(symbol['st_shndx']);at=ENTRY-section['sh_addr']
                    memory.mem_write(ENTRY&0x1FFFFFFF,section.data()[at:at+symbol['st_size']])
            overlap_seen=set()
            for i,angles in enumerate(cases):
                overlap=i%5;native.run_case(i,overlap,*angles,before,after)
                memory.mem_write(ARENA&0x1FFFFFFF,before.raw)
                memory.reg_write(UC_MIPS_REG_A0,ARENA+(16+overlap*8 if overlap<4 else 64))
                memory.reg_write(UC_MIPS_REG_A1,ARENA+16);memory.reg_write(UC_MIPS_REG_RA,STOP);memory.reg_write(UC_MIPS_REG_SP,STACK)
                memory.emu_start(ENTRY,STOP,count=500)
                assert memory.reg_read(UC_MIPS_REG_V0)==ARENA+16
                assert bytes(memory.mem_read(ARENA&0x1FFFFFFF,88))==after.raw,(version,i,angles,overlap)
                overlap_seen.add(overlap)
            assert overlap_seen==set(range(5))
            print('PASS',version,len(cases),'RotMatrix contract cases; each signed angle domain on all axes, mixed angles, five owner layouts, exact rotation/preserved padding/translation/input guards and pointer return',flush=True)


if __name__=='__main__':main()
