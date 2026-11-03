"""Recovered binary64-to-signed32 conversion with the actual structure-return shift.

The independent oracle uses integer bit operations, including the original
encoding-based saturation for NaNs/infinities. The error service is simulated.
"""
from pathlib import Path
from tempfile import TemporaryDirectory
import re, subprocess, unittest
ROOT=Path(__file__).resolve().parents[2]

class SoftfloatToIntTest(unittest.TestCase):
 def test_all_exponents_signs_and_mantissa_boundaries(self):
  body=(ROOT/'build/legacy/func_8007E0B4.c').read_text()
  body=re.sub(r' __asm__\("\$\d+"\)','',body)
  helper=(ROOT/'build/legacy/func_8007DC04.c').read_text()
  helper=re.sub(r' asm\("\$\d+"\)','',helper).replace('func_8007DC04','actual_shift')
  code=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>
#include "rr/softfloat.h"
static unsigned errors,shifts;static s32 category,operation;
static u32 seed=0x7E0B4U;
static u32 next(void){u32 n=seed;n^=n<<13;n^=n>>17;n^=n<<5;return seed=n;}
static s32 signed_bits(u32 n){s32 r;memcpy(&r,&n,4);return r;}
'''+helper+r'''
RrWide64 func_8007DC04(s32 direction,RrWide64 value,s32 count){
 struct {u32 guard[4];RrWide64 value;u32 tail[4];} box;
 uint64_t bits=(uint64_t)(u32)value.hi<<32|value.lo,shifted;
 memset(&box,0xA5,sizeof(box));assert(direction==0&&count==10);shifts++;
 box.value=actual_shift(direction,value,count);shifted=bits<<10;
 assert(box.value.lo==(u32)shifted&&(u32)box.value.hi==(u32)(shifted>>32));
 for(unsigned i=0;i<4;i++){assert(box.guard[i]==0xA5A5A5A5U);assert(box.tail[i]==0xA5A5A5A5U);}return box.value;
}
s32 func_8007E9C4(s32 a,s32 b){errors++;category=a;operation=b;return signed_bits(next());}
'''+body+r'''
static s32 oracle(uint64_t bits){u32 e=(u32)(bits>>52)&2047U;uint64_t magnitude=bits&0x7FFFFFFFFFFFFFFFULL;u32 result;
 if(!magnitude)return 0;
 if(e>1053)return (bits>>63)?INT32_MIN:INT32_MAX;
 if(e<1022)return 0;
 result=(u32)(((bits&0xFFFFFFFFFFFFFULL)|0x10000000000000ULL)>>(1075-e));
 if(bits>>63)result=0U-result;
 return signed_bits(result);
}
static uint64_t cases;static unsigned exponentSeen[2048][2],branches[5];
static void check(uint64_t bits){struct {u32 guard[4];u64 bits;u32 tail[4];} input,saved;s32 expected,result;u32 e=(u32)(bits>>52)&2047;unsigned sign=(unsigned)(bits>>63);int zero=(bits&0x7FFFFFFFFFFFFFFFULL)==0;
 memset(&input,0x5A,sizeof(input));input.bits=bits;saved=input;errors=shifts=0;category=operation=0;
 expected=oracle(bits);result=func_8007E0B4(input.bits);
 assert(result==expected);assert(!memcmp(&input,&saved,sizeof(input)));
 assert(errors==(!zero&&e>1053));assert(shifts==(!zero&&e<=1053));if(errors){assert(category==34&&operation==17);}
 if(zero)branches[0]++;else if(e>1053)branches[1]++;else if(e<1022)branches[2]++;else if(result)branches[3]++;else branches[4]++;
 exponentSeen[e][sign]++;cases++;
}
int main(void){unsigned e,sign,f,l,n;u32 frac;static const u32 lows[8]={0,1,3,0x3FFFFF,0x400000,0x7FFFFFFF,0x80000000,0xFFFFFFFF};
 _Static_assert(sizeof(RrWide64)==8&&sizeof(u64)==8,"word pair");_Static_assert(offsetof(RrWide64,hi)==4,"high word");_Static_assert(INT_MIN==(-2147483647-1),"signed32 encoding");
 assert((uintptr_t)&exponentSeen>UINT32_MAX);
 for(e=0;e<2048;e++)for(sign=0;sign<2;sign++)for(f=0;f<32;f++)for(l=0;l<8;l++){
  switch(f){case 0:frac=0;break;case 1:frac=1;break;case 2:frac=2;break;case 3:frac=0xFFFFF;break;case 4:frac=0xFFFFE;break;case 5:frac=0x7FFFF;break;default:if(f<26)frac=1U<<(f-6);else frac=next()&0xFFFFF;}
  check((uint64_t)sign<<63|(uint64_t)e<<52|(uint64_t)frac<<32|lows[l]);
 }
 for(n=0;n<262144;n++)check((uint64_t)next()<<32|next());
 assert(cases==1310720);for(e=0;e<2048;e++)for(sign=0;sign<2;sign++)assert(exponentSeen[e][sign]);for(n=0;n<5;n++)assert(branches[n]);return 0;
}
'''
  with TemporaryDirectory() as tmp:
   p=Path(tmp);(p/'test.c').write_text(code)
   r=subprocess.run(['cc','-std=gnu11','-O2','-fsanitize=undefined','-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(p/'test.c'),'-o',str(p/'test')],capture_output=True,text=True)
   self.assertEqual(r.returncode,0,r.stderr);subprocess.run([str(p/'test')],check=True)
if __name__=='__main__':unittest.main()
