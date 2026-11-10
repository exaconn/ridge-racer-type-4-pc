#include "rr/overlay668_decode.h"
u32 *ovl668_mdec_put_env(u32 *input){
 u32 *original=input;
 u32 *source=input;
 /* ASM_BARRIER: Preserve the loop destination register until both copies end. */
 register u32 *destination __asm__("$5")=ovl668_mdec_quant_cmd.luma;
 s32 n;
 for(n=15;n!=-1;n--)*destination++=*source++;
 destination=ovl668_mdec_quant_cmd.chroma;source=original+16;
 for(n=15;n!=-1;n--)*destination++=*source++;
 ovl668_mdec_start_in_dma(&ovl668_mdec_quant_cmd,32);
 ovl668_mdec_start_in_dma(&ovl668_mdec_scale_cmd,32);
 return original;
}
