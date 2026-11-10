"""Recovered inverse pitch conversion against fixed PAL tuning tables and an independent oracle."""
from bisect import bisect_right
import ctypes as C
from pathlib import Path
import random
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest


class SpuPitchConversionTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup)
        cls.directory=Path(cls.temp.name)
        root=Path(__file__).resolve().parents[2]
        header = '#include "rr/spu.h"\n'
        body=(root/'tools/scripts/fixtures/views/func_80084D44.c').read_text()
        body=re.sub(r'^#include[^\n]*\n','',body,flags=re.M)
        body=re.sub(r'\s+__asm__\("\$[0-9]+"\)','',body)
        body=re.sub(r'"\$[0-9]+",\s*','',body)
        body=re.sub(r':\s*"\$[0-9]+"(?:,\s*"\$[0-9]+")*(?=\))','',body)
        cls.source='''#include <stdint.h>
typedef uint8_t u8;typedef uint16_t u16;typedef int16_t s16;
typedef uint32_t u32;typedef int32_t s32;
''' + header + '\nu16 SvmSemitoneTable[12],SvmPitchFineTable[128];\n' + body
        (cls.directory/'pitch.c').write_text(cls.source)
        result=subprocess.run(['cc', '-I'+str(Path(__file__).resolve().parents[2]/'include'),'-shared','-fPIC','-O2',str(cls.directory/'pitch.c'),
                               '-o',str(cls.directory/'pitch.so')],capture_output=True,text=True)
        if result.returncode:raise AssertionError(result.stderr)
        cls.lib=C.CDLL(str(cls.directory/'pitch.so'))
        cls.convert=cls.lib._spu_pitch2note
        cls.convert.argtypes=[C.c_int32,C.c_int32,C.c_uint32];cls.convert.restype=C.c_uint32
        exe=(root/'assets/PAL/main.exe').read_bytes()
        cls.coarse=struct.unpack_from('<12H',exe,0x800+0x800A4D58-0x80010000)
        cls.fine=struct.unpack_from('<128H',exe,0x800+0x800A4D70-0x80010000)
        cls.coarse_native=(C.c_uint16*12).in_dll(cls.lib,'SvmSemitoneTable')
        cls.fine_native=(C.c_uint16*128).in_dll(cls.lib,'SvmPitchFineTable')
        cls.coarse_native[:]=cls.coarse;cls.fine_native[:]=cls.fine

    def oracle(self,first,second,pitch):
        pitch=min(pitch&65535,0x3FFF)
        self.assertNotEqual(pitch,0)
        top=pitch.bit_length()-1
        normalized=(pitch<<(15-top))&65535
        note=bisect_right(self.coarse,normalized)-1
        self.assertGreaterEqual(note,0)
        ratio=((normalized<<15)//self.coarse[note])&65535
        fine=bisect_right(self.fine,ratio)-1
        self.assertGreaterEqual(fine,0)
        fraction=(second+fine+1)&65535
        semitone=(first+12*(top-12)+note+(fraction>>7))&65535
        return (semitone<<8)|(fraction&0x7E)

    def test_pal_tables_and_every_nonzero_low16_input(self):
        self.assertEqual((len(self.coarse),len(self.fine)),(12,128))
        self.assertEqual((self.coarse[0],self.fine[0]),(0x8000,0x8000))
        self.assertTrue(all(a<b for a,b in zip(self.coarse,self.coarse[1:])))
        self.assertTrue(all(a<b for a,b in zip(self.fine,self.fine[1:])))
        for first,second in ((0,0),(60,64),(-2**31,2**31-1),(2**31-1,-2**31)):
            for pitch in range(1,65536):
                self.assertEqual(self.convert(first,second,pitch),self.oracle(first,second,pitch))
        self.assertEqual(tuple(self.coarse_native),self.coarse)
        self.assertEqual(tuple(self.fine_native),self.fine)

    def test_full_words_clamp_and_rounding_boundaries(self):
        values=(-2**31,-65537,-65536,-32769,-129,-128,-127,-1,0,1,127,128,129,32767,65535,2**31-1)
        pitches=(1,2,3,4,7,8,15,16,31,32,0x7FF,0x800,0x1FFF,0x2000,0x3FFE,0x3FFF,0x4000,0xFFFF)
        for first in values:
            for second in values:
                for low in pitches:
                    for high in (0,0x80000000,0xFFFF0000):
                        pitch=high|low
                        self.assertEqual(self.convert(first,second,pitch),self.oracle(first,second,pitch))
        generator=random.Random(0x84D44)
        for _ in range(10000):
            first=generator.randrange(-2**31,2**31);second=generator.randrange(-2**31,2**31)
            pitch=generator.randrange(2**32)
            if not pitch&65535:continue
            self.assertEqual(self.convert(first,second,pitch),self.oracle(first,second,pitch))

    def test_supported_domain_has_no_undefined_arithmetic(self):
        coarse=','.join(map(str,self.coarse));fine=','.join(map(str,self.fine))
        main='''
int main(void) {
 static const u16 coarse[]={COARSE};static const u16 fine[]={FINE};
 static const u32 words[]={0U,0xFFFFFFFFU,0x80000000U,0x7FFFFFFFU};
 volatile u32 digest=0;u32 p,i;
 for(i=0;i<12;i++)SvmSemitoneTable[i]=coarse[i];
 for(i=0;i<128;i++)SvmPitchFineTable[i]=fine[i];
 for(p=1;p<65536;p++)for(i=0;i<4;i++)
  digest^=_spu_pitch2note((s32)words[i],(s32)words[3-i],p|(i<<16));
 (void)digest;return 0;
}
'''.replace('COARSE',coarse).replace('FINE',fine)
        path=self.directory/'sanitize.c';path.write_text(self.source+main)
        result=subprocess.run(['cc', '-I'+str(Path(__file__).resolve().parents[2]/'include'),'-O2','-fsanitize=undefined','-fno-sanitize-recover=all',
                               str(path),'-o',str(self.directory/'sanitize')],capture_output=True,text=True)
        self.assertEqual(result.returncode,0,result.stderr)
        result=subprocess.run([str(self.directory/'sanitize')],capture_output=True,text=True)
        self.assertEqual(result.returncode,0,result.stderr)


if __name__=='__main__':unittest.main()
