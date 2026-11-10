/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/library.h"

LibInterruptExchange startIntrDMA(void) {
    DMA_memclr(InterruptCallbackTable, 8);
    /* The ordinary write view permits the original call-delay store. */
    *(u32 *)DICR_VSync = 0;
    InterruptCallback(3, trapIntrDMA);
    return setIntrDMA;
}
