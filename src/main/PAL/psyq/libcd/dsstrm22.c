/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/library.h"
s32 StGetNext(u8 **data,LibQueueRecord32 **header) {
    LibQueueRecord32 *selected=StRingHeaders+StRingReadIndex;
    if(selected->state==1) {
        s32 reset=StEndFrame;
        s32 *page;
        page=(s32 *)0x80110000U;
        page[0x2FC/4]=0;
        if(reset) selected->state=0;
        selected=StRingHeaders+StRingReadIndex;
    }
    /* ASM_BARRIER: Re-read queue state on every path after the index reset gate. */
    __asm__ volatile("" : : : "memory");
    if(selected->state!=2) return 1;
    {
        selected->state=4;
        /* ASM_BARRIER: Publish acquisition before reading the payload location. */
        __asm__ volatile("" : : : "memory");
        *data=(u8 *)StRingHeaders+StRingSize*sizeof(LibQueueRecord32)+StRingReadIndex*0x7E0U;
        *header=selected;
    }
    return 0;
}
