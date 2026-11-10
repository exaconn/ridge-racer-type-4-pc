/* GCC_VERSION: 2.7.2 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/overlay668_dma.h"
#include "rr/diagnostics.h"
extern const char ovl668_mdec_reset_error_fmt[];

void ovl668_mdec_reset_hw(s32 mode) {
    switch (mode) {
    case 0:
        RR_STR_WRITE32(ovl668_mdec1_ctrl_ptr, 0x80000000);
        RR_STR_WRITE32(ovl668_dma0_chcr_ptr, 0);
        RR_STR_WRITE32(ovl668_dma1_chcr_ptr, 0);
        RR_STR_WRITE32(ovl668_mdec1_ctrl_ptr, 0x60000000);
        ovl668_mdec_start_in_dma(&ovl668_mdec_quant_cmd, 32);
        ovl668_mdec_start_in_dma(&ovl668_mdec_scale_cmd, 32);
        break;
    case 1:
        RR_STR_WRITE32(ovl668_mdec1_ctrl_ptr, 0x80000000);
        RR_STR_WRITE32(ovl668_dma0_chcr_ptr, 0);
        RR_STR_WRITE32(ovl668_dma1_chcr_ptr, 0);
        RR_STR_WRITE32(ovl668_mdec1_ctrl_ptr, (RR_STR_READ32(ovl668_dma1_chcr_ptr), 0x60000000));
        break;
    default:
        DebugPrintStub(ovl668_mdec_reset_error_fmt, mode);
        break;
    }
}
