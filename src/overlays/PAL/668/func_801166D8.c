/* GCC_VERSION: 2.7.2 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/overlay668_dma.h"
void ovl668_mdec_start_out_dma(void *output,u32 words)
{
    ovl668_mdec_wait_out_dma();
    *(u32 *)ovl668_dpcr_ptr |= 0x88;
    *(u32 *)ovl668_dma1_chcr_ptr = 0;
    *ovl668_dma1_madr_ptr = output;
    *(u32 *)ovl668_dma1_bcr_ptr = ((words >> 5) << 16) | 32;
    *(u32 *)ovl668_dma1_chcr_ptr = 0x01000200;
}
