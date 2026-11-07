"""Actual sprite effect, real init/semi/link helpers, full native pointers, UBSan."""
from pathlib import Path
import re,subprocess,tempfile,unittest
root=Path(__file__).resolve().parents[2]
src=(root/'tools/scripts/fixtures/legacy/func_8002018C.c').read_text();src=re.sub(r'^#include[^\n]*\n|extern[^;]*;','',src,flags=re.M)
src=re.sub(r' asm\("\$\d+"\)','',src);src=re.sub(r'__asm__\("".*?\);','',src)
header=''
helpers=''
for n,new in [('80095B94','native_init'),('80095AF4','native_semi'),('80095A44','native_link')]:
 s=(root/f'tools/scripts/fixtures/legacy/func_{n}.c').read_text().replace('func_'+n,new).replace('SetSemiTrans' if n=='80095AF4' else 'func_'+n,new).replace({'80095A44':'AddPrim'}.get(n,'func_'+n),new)
 s=re.sub(r' asm\("\$\d+"\)','',s)
 # This is the actual serializer's intentional low24 packet tag, not a pointer reconstruction.
 s=s.replace('(unsigned)p','(uintptr_t)p')
 helpers+=s+'\n'
fixture=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "rr/layout.h"
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_OFFSET(type,field,offset) _Static_assert(offsetof(type,field)==offset,"native layout")
#include "rr/sprite_effect.h"
''' + header +helpers+r'''
static union {u32 align;u8 bytes[4096];} arena;
static union {u32 align;u8 bytes[256];} contexts[4];
u8 *D_800ACC14;
static RrSpriteEffectSource descriptor;
static u32 ordering;
static u32 *expected_ordering;
static int iteration,phase,count,semi,variant,rng_calls;
static s32 strength,carried_y;
static u32 rng_seed;
static s32 jitter,expected_color;
static u8 *expected_cursor;
static s32 bits(u32 value){s32 result;memcpy(&result,&value,4);return result;}
static void mutate(int n) {
 u32 seed=rng_seed+(u32)n*0x12345U+(u32)iteration*0x4321U;
 descriptor.x=(s16)(seed&0xFFFF);descriptor.y=(s16)(seed>>16);
 descriptor.page=(u16)(seed^0x8012U);descriptor.clut=(u16)(seed^0xFFFFU);
 descriptor.uv.u=seed;descriptor.uv.v=seed>>8;descriptor.uv.w=seed>>16;descriptor.uv.h=seed>>24;
 D_800ACC14=contexts[n&3].bytes;
}
void func_80095B94(void *p) {
 assert(phase==0 && p==expected_cursor);
 native_init(p);phase=1;mutate(0);
}
s32 func_80096464(void) {
 u32 raw;
 assert(count!=1 && (phase==1 || phase==3));rng_calls++;
 raw=rng_seed^(phase==1?0x55AA0000U:0xAA550000U)^(u32)iteration*0x11111111U;
 if(phase==1){jitter=3-(s32)(raw&7);phase=2;mutate(1);}
 else {
  u32 factor=iteration==1?(raw&15):((raw&31)+8);
  s32 product=bits((u32)strength*factor);
  expected_color=product/(iteration==1?24:48);
  phase=4;mutate(3);
 }
 return bits(raw);
}
void SetSemiTrans(void *p,s32 mode) {
 RrSpritePacket *sprite=p;s32 y;
 assert(p==expected_cursor && mode==semi && phase==(count==1?1:2));
 if(count==1)jitter=0;
 if(iteration==0)y=descriptor.y+(jitter>=0?jitter/2:-((-jitter+1)/2));
 else if(iteration==1)y=descriptor.y+3*jitter;
 else if(iteration==2)y=descriptor.y+jitter;
 else y=carried_y;
 carried_y=y;
 assert(sprite->x==descriptor.x && sprite->y==(s16)y);
 assert(sprite->w==descriptor.uv.w && sprite->h==descriptor.uv.h);
 assert(sprite->u==descriptor.uv.u && sprite->v==descriptor.uv.v);
 assert(sprite->len==4 && sprite->code==0x64);
 native_semi(p,mode);phase=3;mutate(2);expected_color=strength;
}
void AddPrim(u32 *ot,u32 *p) {
 RrSpritePacket *sprite=(RrSpritePacket *)p;
 assert(phase==(count==1?3:4) && ot==expected_ordering && p==(u32 *)expected_cursor);
 assert(sprite->r==(u8)expected_color && sprite->g==(u8)expected_color && sprite->b==(u8)expected_color);
 assert(sprite->clut==descriptor.clut && sprite->code==(semi?0x66:0x64));
 native_link(ot,p);
 assert((*expected_ordering&0xFFFFFFU)==((uintptr_t)p&0xFFFFFFU));
 phase=5;mutate(4);
}
u8 *AddDrawModePrim(u32 *ot,u8 *cursor,u16 page) {
 u16 expected=iteration?(u16)((descriptor.page&255)|0x60):descriptor.page;
 assert(phase==5 && ot==(u32 *)(D_800ACC14+0x70));
 assert(cursor==expected_cursor+20 && page==expected);
 /* Deliberate opaque-helper return variation proves full-width cursor threading. */
 expected_cursor=cursor+12+(variant?(iteration&1)*4:0);
 iteration++;phase=0;return expected_cursor;
}
''' + src+r'''
static void run(s32 n,s32 power,s32 transparency,u32 seed,int varied) {
 u8 *result;
 memset(&arena,0xB6,sizeof(arena));iteration=phase=rng_calls=0;count=n;
 strength=power;semi=transparency;rng_seed=seed;variant=varied&1;
 expected_cursor=arena.bytes;ordering=0xAB123456U;carried_y=0x12345678;
 expected_ordering=varied/2==0?&ordering:(u32 *)(arena.bytes+(varied/2==1?0:4));
 *expected_ordering=0xAB123456U;
 result=func_8002018C(expected_ordering,arena.bytes,&descriptor,semi,strength,count);
 assert(result==expected_cursor && iteration==(n>0?n:0) && !phase);
 assert(rng_calls==(n>1?n*2:0));
 {
  unsigned offset=0,j;int sprite_index;
  for(sprite_index=0;sprite_index<(n>0?n:0);sprite_index++) {
   unsigned gap=12+(variant?(sprite_index&1)*4:0);
   for(j=offset+20;j<offset+20+gap;j++)assert(arena.bytes[j]==0xB6);
   offset+=20+gap;
  }
  for(j=offset;j<4096;j++) {
   /* An ordering head may intentionally occupy the first packet words. */
   if(expected_ordering!=&ordering && j<8)continue;
   assert(arena.bytes[j]==0xB6);
  }
 }
}
int main(void) {
 s32 powers[]={INT32_MIN,INT32_MAX,-1,0,1,24,48,255,256,65536,0x40000001};
 s32 counts[]={INT32_MIN,-1,0,1,2,3,4,7};
 s32 semis[]={0,1,-1,INT32_MIN,INT32_MAX};
 unsigned i,j,k,r;
 assert(sizeof(RrSpriteEffectSource)==12 && offsetof(RrSpriteEffectSource,uv)==8);
 for(i=0;i<8;i++)for(j=0;j<11;j++)for(k=0;k<5;k++)for(r=0;r<32;r++)run(counts[i],powers[j],semis[k],0x89ABC000U|r,r%6);
 /* Inactive paths must not inspect pointer arguments. */
 assert(func_8002018C(0,0,0,INT32_MIN,INT32_MAX,0)==0);
 assert(func_8002018C(0,0,0,0,0,INT32_MIN)==0);
 return 0;
}
'''
class SpriteEffectTests(unittest.TestCase):
    def test_actual_body_and_gpu_helpers_with_ubsan(self):
        with tempfile.TemporaryDirectory() as tmp:
            c=Path(tmp)/'effect.c';exe=Path(tmp)/'effect';c.write_text(fixture)
            subprocess.run(['cc','-std=gnu99','-O2','-fsanitize=undefined','-fno-sanitize-recover=all','-I',str(root/'include'),str(c),'-o',str(exe)],check=True,capture_output=True)
            subprocess.run([str(exe)],check=True,capture_output=True)
if __name__=='__main__':unittest.main()
