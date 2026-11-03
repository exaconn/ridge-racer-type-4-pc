"""Fixed-point HSV/RGB against a64-bit stage oracle, including aliased storage."""
from pathlib import Path
from tempfile import TemporaryDirectory
import subprocess,unittest
ROOT=Path(__file__).resolve().parents[2]
class ColorConversionTest(unittest.TestCase):
 def test_full_component_encodings_clamps_negative_hue_and_aliasing(self):
  body=(ROOT/'build/legacy/func_8004C138.c').read_text()
  body='\n'.join(line for line in body.splitlines() if not line.startswith('#include'))
  code=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "rr/color_conversion.h"
'''+body+r'''
typedef union {s16 halves[16];u8 bytes[32];} ColorBuffer;
typedef struct {u32 guard[4];ColorBuffer input;u8 output[11];u8 tail[9];} Environment;
static Environment actual,expected;static unsigned serial,sectors[7],layouts[9],clamps[4],negativeFraction,nullCalls;
static s16 signed16(u16 bits){s16 result;memcpy(&result,&bits,2);return result;}
static int64_t clamp12(int64_t x){return x<0?0:x>4096?4096:x;}
static int64_t multiply12(int64_t a,int64_t b){return a*b/4096;}
static int64_t to_byte_scale(int64_t x){return x*255/4096;}
static int oracle(s16 hue,s16 saturation,s16 value,u8 rgb[3]){
 int64_t h=(int64_t)hue*4096,s=clamp12(saturation),v=clamp12(value),sector,fraction,c[4];
 static const unsigned order[6][3]={{0,3,1},{2,0,1},{1,0,3},{1,2,0},{3,1,0},{0,1,2}};
 unsigned i;
 if(saturation<0)clamps[0]++;if(saturation>4096)clamps[1]++;
 if(value<0)clamps[2]++;if(value>4096)clamps[3]++;
 if(h<0)h+=360LL*4096;
 h-=(h/(360LL*4096))*(360LL*4096);
 sector=h/(60LL*4096);
 if(sector<0||sector>=6){sectors[6]++;return 0;}
 sectors[sector]++;fraction=(h-sector*(60LL*4096))/60;
 if(fraction<0)negativeFraction++;
 c[0]=to_byte_scale(v);
 c[1]=to_byte_scale(multiply12(v,4096-s));
 c[2]=to_byte_scale(multiply12(v,4096-multiply12(s,fraction)));
 c[3]=to_byte_scale(multiply12(v,4096-multiply12(s,4096-fraction)));
 for(i=0;i<3;i++)rgb[i]=(u8)c[order[sector][i]];
 return 1;
}
static void run(s16 h,s16 s,s16 v,unsigned layout){unsigned i;u8 rgb[3],*out,*expectedOut;int written;
 u8 *bytes=(u8 *)&actual;assert(layout<9);layouts[layout]++;
 for(i=0;i<sizeof(actual);i++)bytes[i]=(u8)(i*37U+serial);
 actual.input.halves[2]=h;actual.input.halves[3]=s;actual.input.halves[4]=v;
 memcpy(&expected,&actual,sizeof(actual));written=oracle(h,s,v,rgb);
 if(layout==0||layout==8){out=actual.output+3;expectedOut=expected.output+3;}
 else {out=actual.input.bytes+4+layout-1;expectedOut=expected.input.bytes+4+layout-1;}
 if(!written&&layout==8){out=0;nullCalls++;}
 if(written)memcpy(expectedOut,rgb,3);
 func_8004C138(actual.input.halves+2,out);
 assert(!memcmp(&actual,&expected,sizeof(actual)));serial++;
}
int main(void){unsigned raw,a,b,j;uint64_t cases=0;
 static const s16 bounds[8]={INT16_MIN,-1,0,1,2048,4095,4096,INT16_MAX};
 static const s16 hues[16]={-720,-420,-419,-361,-360,-1,0,59,60,179,180,359,360,720,INT16_MAX,INT16_MIN};
 assert(sizeof(void*)==8&&((uintptr_t)&actual>>32));
 for(raw=0;raw<65536;raw++)for(a=0;a<8;a++)for(b=0;b<8;b++){
  run(signed16((u16)raw),bounds[a],bounds[b],0);cases++;
  run(signed16((u16)raw),bounds[a],bounds[b],(raw+a*8+b)%9);cases++;
 }
 for(raw=0;raw<65536;raw++)for(j=0;j<16;j++){
  run(hues[j],signed16((u16)raw),bounds[j%8],(raw+j)%9);cases++;
  run(hues[j],bounds[(j+3)%8],signed16((u16)raw),(raw+j+4)%9);cases++;
 }
 for(j=0;j<7;j++)assert(sectors[j]);for(j=0;j<9;j++)assert(layouts[j]);
 for(j=0;j<4;j++)assert(clamps[j]);assert(negativeFraction&&nullCalls);
 printf("%llu cases;all16-bit component encodings,clamp boundaries,negative sectors and seven overlap offsets\n",(unsigned long long)cases);
}
'''
  with TemporaryDirectory() as temp:
   p=Path(temp);(p/'test.c').write_text(code)
   subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror','-fsanitize=undefined','-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(p/'test.c'),'-o',str(p/'test')],check=True)
   subprocess.run([str(p/'test')],check=True)
if __name__=='__main__':unittest.main()
