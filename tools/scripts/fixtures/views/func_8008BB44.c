/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/library.h"

RrLibInterruptExchange startIntrDMA(void) {
    DMA_memclr(D_800A63A4, 8);
    /* The ordinary write view permits the original call-delay store. */
    *(u32 *)D_800A63A0 = 0;
    InterruptCallback(3, trapIntrDMA);
    return setIntrDMA;
}
