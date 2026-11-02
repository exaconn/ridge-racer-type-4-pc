#include "rr/overlay668_decode.h"
u32 *ovl668_func_80116358(u32 *input){
 u32 *original=input;
 register u32 *source=input;
 /* ASM_BARRIER: Preserve the loop destination register until both copies end. */
 register u32 *destination __asm__("$5")=ovl668_D_80116D10.luma;
 s32 n;
 for(n=15;n!=-1;n--)*destination++=*source++;
 destination=ovl668_D_80116D10.chroma;source=original+16;
 for(n=15;n!=-1;n--)*destination++=*source++;
 ovl668_func_80116648(&ovl668_D_80116D10,32);
 ovl668_func_80116648(&ovl668_D_80116D94,32);
 return original;
}
