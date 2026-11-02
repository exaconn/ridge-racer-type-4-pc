/* GCC_VERSION: 2.7.2 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/overlay668_dma.h"
void ovl668_func_80116648(const void *command,u32 words)
{
    ovl668_func_80116764();
    *(volatile u32 *)ovl668_D_80116E58 |= 0x88;
    *ovl668_D_80116E20 = (const u8 *)command + 4;
    *(volatile u32 *)ovl668_D_80116E24 = ((words >> 5) << 16) | 32;
    *(volatile u32 *)ovl668_D_80116E50 = *(const u32 *)command;
    *(volatile u32 *)ovl668_D_80116E28 = 0x01000201;
}
