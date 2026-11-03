"""Common SPU attributes against an independent complete-bank RAM model.

Volatile register accesses are retained. This does not execute SPU hardware or
model asynchronous device changes between accesses.
"""
from pathlib import Path
from tempfile import TemporaryDirectory
import re,subprocess,unittest
ROOT=Path(__file__).resolve().parents[2]
class SpuCommonTest(unittest.TestCase):
 def test_all_masks_modes_volumes_and_control_words(self):
  body=(ROOT/'tools/scripts/fixtures/views/func_80082674.c').read_text().split('#define SPU(index)',1)[1]
  body='#define SPU(index)'+body
  body=re.sub(r' __asm__\("\$[0-9]+"\)','',body)
  code=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "rr/spu_common.h"
RrSpuRegisterBank *D_800A47D8;
static struct {u32 guard[4];RrSpuRegisterStorage bank;u32 tail[4];} actual;
static u16 expected[256];
static u32 rng=0x82674U;
static unsigned seen[6],sweeps[2][8],clamps[2][3],externalWrites[4][2],controls[4][3];
static unsigned maskSeen[65536],modeSeen[2][65536],volumeSeen[2][65536],controlSeen[65536],oneHot[32];
static u32 next(void){u32 n=rng;n^=n<<13;n^=n>>17;n^=n<<5;return rng=n;}
static s16 half(u16 n){s16 v;memcpy(&v,&n,2);return v;}
static s32 word(u32 n){s32 v;memcpy(&v,&n,4);return v;}
'''+body+r'''
static u16 volume(s16 value,s16 mode,int sweep,unsigned side){s32 v=value;
 if(!sweep||mode<1||mode>7){sweeps[side][0]++;return (u16)value&32767U;}
 sweeps[side][mode]++;
 if(v<0){v=0;clamps[side][0]++;}else if(v>127){v=127;clamps[side][2]++;}else clamps[side][1]++;
 return (u16)((0x7000U+(u32)mode*4096U)|(u32)v);
}
static void model(const RrSpuCommonAttributes28 *a){unsigned all=a->mask==0,i;u32 flags[4]={0x100,0x200,0x1000,0x2000};u16 bits[4]={4,1,8,2};s32 values[4]={a->cd.reverb,a->cd.mix,a->external.reverb,a->external.mix};s16 vols[4]={a->cd.volume.left,a->cd.volume.right,a->external.volume.left,a->external.volume.right};u32 masks[4]={0x40,0x80,0x400,0x800};
 seen[all]++;
 if(all||(a->mask&1)){expected[192]=volume(a->master.left,a->mode.left,all||(a->mask&4),0);seen[2]++;}else seen[3]++;
 if(all||(a->mask&2)){expected[193]=volume(a->master.right,a->mode.right,all||(a->mask&8),1);seen[4]++;}else seen[5]++;
 for(i=0;i<4;i++){
  if(all||(a->mask&masks[i])){expected[216+i]=(u16)vols[i];externalWrites[i][0]++;}else externalWrites[i][1]++;
  if(all||(a->mask&flags[i])){if(values[i]){expected[213]|=bits[i];controls[i][1]++;}else{expected[213]&=(u16)~bits[i];controls[i][0]++;}}else controls[i][2]++;
 }
}
static s32 booleanValue(unsigned which){switch(which%5){case 0:return 0;case 1:return INT32_MIN;case 2:return INT32_MAX;case 3:return -1;default:return word(next());}}
int main(void){unsigned n,phase,i,j;uint64_t cases=0;RrSpuCommonAttributes28 a,saved;RrSpuRegisterBank *root;u32 guards[4],tails[4];
 _Static_assert(sizeof(RrSpuRegisterStorage)==512,"complete bank");_Static_assert(sizeof(a)==40,"complete attributes");
 _Static_assert(offsetof(RrSpuCommonAttributes28,cd)==16,"CD fields");_Static_assert(offsetof(RrSpuCommonAttributes28,external)==28,"external fields");
 assert((uintptr_t)&actual.bank>UINT32_MAX);assert((uintptr_t)&a>UINT32_MAX);
 for(n=0;n<65536;n++)for(phase=0;phase<10;phase++){
  memset(&a,0,sizeof(a));
  a.mask=phase==0?n:0;
  a.master.left=half((u16)n);a.master.right=half((u16)(n^32768U));a.output.left=half((u16)next());a.output.right=half((u16)next());
  if(phase<2){a.mode.left=half((u16)n);a.mode.right=half((u16)~n);}else{a.mode.left=phase-1;a.mode.right=9-phase;}
  a.cd.volume.left=half((u16)(n*40503U+phase));a.cd.volume.right=half((u16)(n*73U+phase));a.external.volume.left=half((u16)(n*19U+phase));a.external.volume.right=half((u16)(n*65535U+phase));
  a.cd.reverb=booleanValue(n+phase);a.cd.mix=booleanValue((n>>2)+phase);a.external.reverb=booleanValue((n>>4)+phase);a.external.mix=booleanValue((n>>6)+phase);
  if(phase==9){switch(n%4){case 0:i=(n>>2)%32;a.mask=1U<<i;oneHot[i]++;break;case 1:a.mask=next();break;case 2:a.mask=n<<16;break;default:a.mask=~(u32)n;}}
  if(phase==0)maskSeen[n]++;if(phase==1){modeSeen[0][(u16)a.mode.left]++;modeSeen[1][(u16)a.mode.right]++;}
  volumeSeen[0][(u16)a.master.left]++;volumeSeen[1][(u16)a.master.right]++;
  for(i=0;i<256;i++){expected[i]=(u16)next();actual.bank.halves[i]=expected[i];}expected[213]=(u16)n;actual.bank.halves[213]=(u16)n;controlSeen[n]++;
  for(i=0;i<4;i++){guards[i]=next();tails[i]=next();actual.guard[i]=guards[i];actual.tail[i]=tails[i];}
  root=a.mask&&!(a.mask&0x3FC3U)?0:&actual.bank.registers;D_800A47D8=root;saved=a;model(&saved);func_80082674(&a);
  assert(D_800A47D8==root);assert(!memcmp(&a,&saved,sizeof(a)));assert(!memcmp(actual.guard,guards,sizeof(guards)));assert(!memcmp(actual.tail,tails,sizeof(tails)));
  for(i=0;i<256;i++)assert(actual.bank.halves[i]==expected[i]);cases++;
 }
 assert(cases==655360);for(i=0;i<65536;i++){assert(maskSeen[i]);assert(controlSeen[i]);for(j=0;j<2;j++){assert(modeSeen[j][i]);assert(volumeSeen[j][i]);}}
 for(i=0;i<32;i++)assert(oneHot[i]);for(i=0;i<6;i++)assert(seen[i]);for(i=0;i<2;i++){for(j=0;j<8;j++)assert(sweeps[i][j]);for(j=0;j<3;j++)assert(clamps[i][j]);}
 for(i=0;i<4;i++){assert(externalWrites[i][0]&&externalWrites[i][1]);for(j=0;j<3;j++)assert(controls[i][j]);}return 0;
}
'''
  with TemporaryDirectory() as tmp:
   p=Path(tmp);(p/'test.c').write_text(code)
   r=subprocess.run(['cc','-std=gnu11','-O2','-fsanitize=undefined','-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(p/'test.c'),'-o',str(p/'test')],capture_output=True,text=True)
   self.assertEqual(r.returncode,0,r.stderr);subprocess.run([str(p/'test')],check=True)
if __name__=='__main__':unittest.main()
