/* GCC_VERSION: 2.7.2 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/overlay668_dma.h"
void ovl668_func_801166D8(void *output,u32 words)
{
    ovl668_func_801167F8();
    *(u32 *)ovl668_D_80116E58 |= 0x88;
    *(u32 *)ovl668_D_80116E34 = 0;
    *ovl668_D_80116E2C = output;
    *(u32 *)ovl668_D_80116E30 = ((words >> 5) << 16) | 32;
    *(u32 *)ovl668_D_80116E34 = 0x01000200;
}
