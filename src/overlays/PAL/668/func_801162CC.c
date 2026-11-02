/* GCC_VERSION: 2.7.2 */
#include "rr/overlay668_decode.h"

u32 *ovl668_func_801162CC(u32 *output){
 u32 *cursor=output,*source=ovl668_D_80116D10.luma;
 s32 n;
 for(n=15;n!=-1;n--)*cursor++=*source++;
 cursor=output+16;source=ovl668_D_80116D10.chroma;
 for(n=15;n!=-1;n--)*cursor++=*source++;
 cursor=output+32;source=ovl668_D_80116D94.matrix;
 for(n=31;n!=-1;n--)*cursor++=*source++;
 return output;
}
