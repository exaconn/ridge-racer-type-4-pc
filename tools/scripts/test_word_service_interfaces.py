"""Public word results for checksum and PAL atan2, through actual C bodies.

Check byte-result zero extension and signed 32-bit angle arguments on LP64,
then compare original and linked MIPS against the same native results. Atan2
cases cover representable signed magnitudes; the old INT_MIN negation/division
edge is separate arithmetic reconstruction debt, not an interface guarantee.
"""
import ctypes as C
import math
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from c_functions import address_of, definition_source
from mips_preview_engine import signed
from test_vehicle_initializer import Cpu

ROOT=Path(__file__).resolve().parents[2]
BUFFER,STACK,STOP=0x80300000,0x803F0000,0x803FF000


class ArithmeticCpu(Cpu):
    def step(self):
        instruction=self.load(self.pc,4)
        if instruction>>26==0 and instruction&63 in (0x1A,0x1B):
            left=self.r[(instruction>>21)&31];right=self.r[(instruction>>16)&31]
            if instruction&63==0x1A:left,right=signed(left),signed(right)
            assert right and (left,right)!=(-0x80000000,-1)
            quotient=abs(left)//abs(right)
            if (left<0)!=(right<0):quotient=-quotient
            self.lo=quotient&0xFFFFFFFF;self.hi=(left-quotient*right)&0xFFFFFFFF
            target=self.delay;self.delay=None;self.pc=target if target is not None else self.pc+4
            return
        super().step()


class WordServiceInterfacesTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temporary=TemporaryDirectory();cls.addClassCleanup(cls.temporary.cleanup)
        directory=Path(cls.temporary.name)
        source='#include "game/ui/uicnt.h"\n#include "psyq/libgte/gteatan.h"\ns16 ratan_tbl[1025];\n'
        for unit,name in (('game/ui/uicnt','ChecksumBytes'),('psyq/libgte/gteatan','ratan2')):
            source+=definition_source((ROOT/f'src/main/PAL/{unit}.c').read_text(),name)
        (directory/'words.c').write_text(source)
        result=subprocess.run(['cc','-std=gnu11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                               '-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I',str(ROOT/'include'),
                               str(directory/'words.c'),'-o',str(directory/'words.so')],capture_output=True,text=True)
        if result.returncode:raise AssertionError(result.stderr)
        cls.library=C.CDLL(str(directory/'words.so'))
        cls.checksum=cls.library.ChecksumBytes;cls.checksum.argtypes=[C.c_void_p,C.c_int32];cls.checksum.restype=C.c_int32
        cls.angle=cls.library.ratan2;cls.angle.argtypes=[C.c_int32]*2;cls.angle.restype=C.c_int32
        image=(ROOT/'assets/PAL/main.exe').read_bytes();base=struct.unpack_from('<I',image,24)[0]
        at=0x800+address_of('ratan_tbl')-base;cls.table_bytes=image[at:at+2050]
        cls.table=(C.c_int16*1025).in_dll(cls.library,'ratan_tbl')
        C.memmove(C.addressof(cls.table),cls.table_bytes,2050)

    def machines(self):
        image=(ROOT/'assets/PAL/main.exe').read_bytes();base=struct.unpack_from('<I',image,24)[0]
        for linked in (False,True):
            memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000)
            memory.mem_write(base&0x1FFFFFFF,image[0x800:])
            if linked:
                with (ROOT/'build/PAL/main.elf').open('rb')as stream:
                    elf=ELFFile(stream)
                    for section in elf.iter_sections():
                        if section['sh_flags']&2 and section['sh_type']!='SHT_NOBITS':
                            memory.mem_write(section['sh_addr']&0x1FFFFFFF,section.data())
            yield linked,memory,ArithmeticCpu(memory,0,100,101,102,lambda *args:None)

    def invoke(self,cpu,name,*args):
        cpu.reset();cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)]
        cpu.r[29]=STACK;cpu.r[31]=STOP;cpu.pc=address_of(name)
        for i,arg in enumerate(args):cpu.r[4+i]=arg&0xFFFFFFFF
        for step in range(10000):
            if cpu.pc==STOP:break
            cpu.step()
        self.assertEqual(cpu.pc,STOP,(name,args));self.assertEqual(cpu.r[29],STACK)
        for register in (*range(16,24),28,30):self.assertEqual(cpu.r[register],0xBEEF0000+register)
        return signed(cpu.r[2])

    def test_checksum_zero_extends_every_byte_result_and_preserves_input(self):
        rng=random.Random(0xB17E);data=bytearray(rng.randbytes(512));native=C.create_string_buffer(bytes(data))
        self.assertGreater(C.addressof(native),0xFFFFFFFF)
        for linked,memory,cpu in self.machines():
            for value in range(256):
                data[0]=value;native[0]=bytes([value]);memory.mem_write(BUFFER&0x1FFFFFFF,bytes(data))
                for count in (-0x80000000,-1,0,1,2,6,255,256,512):
                    expected=sum(data[:max(count,0)])&255
                    self.assertEqual(self.checksum(native,count),expected)
                    self.assertEqual(self.invoke(cpu,'ChecksumBytes',BUFFER,count),expected,(linked,value,count))
                self.assertEqual(memory.mem_read(BUFFER&0x1FFFFFFF,512),bytes(data))
                self.assertEqual(native.raw[:512],bytes(data))

    def test_atan2_public_signed_word_api_against_math_and_both_mips_images(self):
        positive=(0,1,2,1023,1024,32767,32768,0x1FFFFF,0x200000,0x7FE00000,0x7FFFFFFF)
        values=sorted(set(positive)|{-n for n in positive})
        cases=[(y,x)for y in values for x in values]
        rng=random.Random(0x924D4)
        cases += [(rng.randint(-0x7FFFFFFF,0x7FFFFFFF),rng.randint(-0x7FFFFFFF,0x7FFFFFFF))for _ in range(1024)]
        expected=[]
        for y,x in cases:
            value=self.angle(y,x);expected.append(value)
            math_value=math.atan2(y,x)*4096/math.tau if y or x else 0
            delta=(value-math_value+2048)%4096-2048
            self.assertLessEqual(abs(delta),2,(y,x,value,math_value))
        for linked,memory,cpu in self.machines():
            for (y,x),value in zip(cases,expected):
                self.assertEqual(self.invoke(cpu,'ratan2',y,x),value,(linked,y,x))
        self.assertEqual(bytes(self.table),self.table_bytes)
