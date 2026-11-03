"""Actual encoding comparison versus a monotonic integer-key oracle, under UBSan."""
from pathlib import Path
from tempfile import TemporaryDirectory
import re, subprocess, unittest
ROOT=Path(__file__).resolve().parents[2]

class SoftfloatLessTest(unittest.TestCase):
 def test_all_exponent_pairs_and_encoding_boundaries(self):
  body=(ROOT/'build/legacy/func_8007E3B4.c').read_text()
  body=re.sub(r' __asm__\("\$\d+"\)','',body)
  body=re.sub(r'__asm__\(""[^;]*;', '',body)
  code=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <limits.h>
#include "rr/softfloat.h"
'''+body+r'''
static uint64_t cases;static u32 seed=0x7e3b4;static unsigned paths[9];
static u32 next(void){u32 x=seed;x^=x<<13;x^=x>>17;x^=x<<5;return seed=x;}
/* A single monotonic unsigned key, independent of the implementation's
 * exponent/fraction control flow. Normalize both zero encodings first. */
static uint64_t key(uint64_t bits){if((bits&0x7fffffffffffffffULL)==0)bits=0;
 return bits>>63?~bits:bits^0x8000000000000000ULL;}
static void check(uint64_t a,uint64_t b){s32 result=func_8007E3B4(a,b);int want=key(a)<key(b)?-1:0;
 assert(result==want);cases++;
 if(a==b)paths[0]++;else if(!(a&0x7fffffffffffffffULL)&&!(b&0x7fffffffffffffffULL))paths[1]++;
 else if((a^b)>>63)paths[2]++;else if((a>>52)!=(b>>52))paths[3]++;
 else if((a>>32)!=(b>>32))paths[4]++;else paths[5]++;
 if(((a>>52)&2047)==2047&&(a&0xfffffffffffffULL))paths[6]++;
 if((a>>63)&&result==-1)paths[7]++;if(!(a>>63)&&result==-1)paths[8]++;
}
int main(void){unsigned e,r,s,f,l,p,n;u32 fraction;uint64_t a,b;
 static const u32 lows[8]={0,1,2,0x3fffffff,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};
 _Static_assert(sizeof(u64)==8&&sizeof(RrWide64)==8,"wide ABI");
 _Static_assert(offsetof(RrWide64,hi)==4,"PAL high word");
 _Static_assert(INT_MIN==(-2147483647-1),"signed32 bits");
 {union {u64 bits;RrWide64 words;} x;x.bits=0x1234567887654321ULL;assert(x.words.lo==0x87654321U&&(u32)x.words.hi==0x12345678U);}
 for(e=0;e<2048;e++)for(r=0;r<2048;r++)for(s=0;s<4;s++)
  check((uint64_t)(s&1)<<63|(uint64_t)e<<52,(uint64_t)(s>>1)<<63|(uint64_t)r<<52);
 for(e=0;e<2048;e++)for(s=0;s<4;s++)for(f=0;f<16;f++)for(l=0;l<8;l++){
  switch(f){case 0:fraction=0;break;case 1:fraction=1;break;case 2:fraction=0xfffff;break;case 3:fraction=0x7ffff;break;default:fraction=next()&0xfffff;}
  a=(uint64_t)(s&1)<<63|(uint64_t)e<<52|(uint64_t)fraction<<32|lows[l];
  for(p=0;p<8;p++){
   switch(p){case 0:b=a;break;case 1:b=a^0x8000000000000000ULL;break;
   case 2:b=(a&0xffffffff00000000ULL)|lows[(l+1)%8];break;
   case 3:b=a+1;break;case 4:b=a-1;break;
   case 5:b=a^0x0008000000000000ULL;break;case 6:b=0;break;default:b=0x8000000000000000ULL;}
   b=(b&0x7fffffffffffffffULL)|(uint64_t)(s>>1)<<63;check(a,b);
  }
 }
 for(n=0;n<262144;n++){a=(uint64_t)next()<<32|next();b=(uint64_t)next()<<32|next();check(a,b);check(b,a);}
 for(p=0;p<9;p++)assert(paths[p]);assert(cases==25690112ULL);printf("%llu cases\n",(unsigned long long)cases);return 0;
}
'''
  with TemporaryDirectory() as d:
   p=Path(d);(p/'test.c').write_text(code)
   r=subprocess.run(['clang','-std=gnu11','-O2','-fsanitize=undefined','-fno-sanitize-recover=all','-I'+str(ROOT/'include'),str(p/'test.c'),'-o',str(p/'test')],capture_output=True,text=True)
   self.assertEqual(r.returncode,0,r.stderr)
   r=subprocess.run([str(p/'test')],capture_output=True,text=True)
   self.assertEqual(r.returncode,0,r.stderr)
   self.assertEqual(r.stdout.strip(),'25690112 cases')

if __name__=='__main__':unittest.main()
