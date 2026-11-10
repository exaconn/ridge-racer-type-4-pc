/* GCC_VERSION: 2.7.2 */
#include "rr/library.h"
s32 StGetNext(u8 **data,LibQueueRecord32 **header) {
    LibQueueRecord32 *selected=StRingHeaders+StRingReadIndex;
    /* ASM_BARRIER: Keep the status word in the original common return register. */
    register s32 result __asm__("$2");
    if(selected->state==1) {
        s32 reset=StEndFrame;
        /* ASM_BARRIER: Preserve the original assembler temporary for index publication. */
        register s32 *page __asm__("$1");
        /* ASM_BARRIER: Read the reset flag before materializing the index address. */
        __asm__("" : : "r"(reset) : "$1");
        page=(s32 *)0x80110000U;
        /* ASM_BARRIER: Expose the index reset for the conditional branch delay slot. */
        __asm__("" : "=r"(page) : "0"(page));
        page[0x2FC/4]=0;
        if(reset) selected->state=0;
        selected=StRingHeaders+StRingReadIndex;
    }
    /* ASM_BARRIER: Re-read queue state on every path after the index reset gate. */
    __asm__ volatile("" : : : "memory");
    if(selected->state!=2) {result=1;goto out;}
    {
        selected->state=4;
        /* ASM_BARRIER: Publish acquisition before reading the payload location. */
        __asm__ volatile("" : : : "memory");
        result=0;
        *data=(u8 *)StRingHeaders+StRingSize*sizeof(LibQueueRecord32)+StRingReadIndex*0x7E0U;
        *header=selected;
    }
out:
    return result;
}
