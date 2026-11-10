/* GCC_VERSION: 2.7.2 */
#include "rr/overlay668_dma.h"
#include "rr/diagnostics.h"
extern const char ovl668_mdec_timeout_fmt[];
s32 ovl668_mdec_handle_timeout(const char *message) {
    /* Preserve the full native pointer; PAL uses32-bit pointers. */
    u32 *port;
    s32 result;
    DebugPrintStub(ovl668_mdec_timeout_fmt, message);
    RR_STR_WRITE32(ovl668_mdec1_ctrl_ptr, 0x80000000);
    RR_STR_WRITE32(ovl668_dma0_chcr_ptr, 0);
    port = ovl668_dma1_chcr_ptr;
    RR_STR_WRITE32(port, 0);
    result = 0;
    RR_STR_WRITE32(ovl668_mdec1_ctrl_ptr, (RR_STR_READ32(ovl668_dma1_chcr_ptr), 0x60000000));
    return result;
}
