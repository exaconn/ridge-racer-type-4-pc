#include "rr/overlay667_course_initializer.h"
typedef struct {s8 first,second;} Pair;
void ovl667_func_80116918(void){
 s32 i;s32 j;register s32 offset __asm__("$7");s32 pos;
 register u8 *repeat __asm__("$5");
 u8 *destBase;
 u8 *sourceBase;
 func_80092CB8(0);
 func_8001ECE8(0,0,0,2);
 func_80035024(1);
 i=0;
 func_8002A604();
 for(;i<2;i++) {D_8010C0D8[i]=0;D_8010C0D0[i]=0;D_8010C080[i]=0;}
 for(i=0;i<4;i++) {((RrMenuChoice4 *)((u8 *)&D_800F4D98+i*4))->bytes[3]=0;D_8010C080[i]=0;}
 i=0;
 destBase=(u8 *)&D_8010BFD8;
 sourceBase=(u8 *)&D_800ABE08;
 __asm__("" : "+r"(destBase),"+r"(sourceBase));
 offset=0;
 D_800F3914=0;D_800AC648=0;D_800F3A30=3;
 D_800AC5D4=(1-(s16)D_800AD508)*2;
 for(;i<2;i++,offset+=36)for(j=0,repeat=sourceBase,pos=offset;j<9;repeat+=4,j++,pos+=4) {
  u8 *dest=((u8 *)((unsigned long)(long)pos+(unsigned long)destBase));
  __builtin_memcpy(dest,(u8 *)((unsigned long)(long)pos+(unsigned long)sourceBase),4);
  __builtin_memcpy(dest+72,repeat,4);
  __asm__("" : : "r"(j),"r"(pos),"r"(repeat));
 }
 for(i=0;i<2;i++)func_8002B75C(D_8010C088[D_800AC5D4+i],ovl667_D_80118E00[D_800AC5D4+i]);
 ovl667_func_801161A8((1-(s16)D_800AD508)*2);
 func_80037040((s8*)((u8 *)&D_800F4D98+D_800AC5D4*4),D_800ABE08[0][0].glyph);
 func_80037040((s8*)((u8 *)&D_800F4D98+4+D_800AC5D4*4),D_800ABE08[0][0].glyph);
 D_8010C078=8;func_80053BB4(0);D_800F4C6A=1;D_800F3A26=60;D_800F4D8A=0;D_8010C0B8=0;
 if(((Pair *)((s8 *)&D_800ABEBC+D_800AC5D4))->second!=1)D_8010C0D4=0;else D_8010C0D4=2;
 func_8002AF8C(0,1,1);
}
