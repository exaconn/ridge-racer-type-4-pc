/* MASPSX_FLAGS: --assembler-profile=gnu */

/* GCC_VERSION: 2.7.2 */

#include "rr/overlay668_wait.h"
s32 ovl668_mdec_wait_in_dma(void)
{
    volatile s32 counter = 0x100000;
    while (*(u32 *)ovl668_mdec1_ctrl_ptr & 0x20000000) {
        if (--counter == -1) {
            ovl668_mdec_handle_timeout(ovl668_mdec_in_timeout_msg);
            return -1;
        }
    }
    return 0;
}
