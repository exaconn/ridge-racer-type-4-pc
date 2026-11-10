/* GCC_VERSION: 2.7.2 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/overlay668_dma.h"
void ovl668_mdec_start_in_dma(const void *command,u32 words)
{
    ovl668_mdec_wait_in_dma();
    *(u32 *)ovl668_dpcr_ptr |= 0x88;
    *ovl668_dma0_madr_ptr = (const u8 *)command + 4;
    *(u32 *)ovl668_dma0_bcr_ptr = ((words >> 5) << 16) | 32;
    *(u32 *)ovl668_mdec0_cmd_ptr = *(const u32 *)command;
    *(u32 *)ovl668_dma0_chcr_ptr = 0x01000201;
}
