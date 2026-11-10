/* GCC_VERSION: 2.7.2 */
#include "rr/overlay668_decode.h"

u32 *ovl668_mdec_get_env(u32 *output){
 u32 *cursor=output,*source=ovl668_mdec_quant_cmd.luma;
 s32 n;
 for(n=15;n!=-1;n--)*cursor++=*source++;
 cursor=output+16;source=ovl668_mdec_quant_cmd.chroma;
 for(n=15;n!=-1;n--)*cursor++=*source++;
 cursor=output+32;source=ovl668_mdec_scale_cmd.matrix;
 for(n=31;n!=-1;n--)*cursor++=*source++;
 return output;
}
