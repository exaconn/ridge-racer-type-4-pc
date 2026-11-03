/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/library.h"

extern s32 func_8001E84C(const char *, ...);

s32 func_8008BE44(void) {
    s32 i=4;
    /* ASM_BARRIER: Keep the success value in the common return register. */
    register s32 result __asm__("$2");
loop:
    {
    s32 status=func_8008BED4();
    s32 expected=1;
    /* ASM_BARRIER: Keep the probe result comparison separate from returned success. */
    __asm__("" : "+r"(expected));
    if(status==expected) {
        result=1;
        D_800A6488=func_8008BF10;
        D_800A648C=func_8008BF38;
        D_800A63F0=func_8008BF60;
        {
            /* ASM_BARRIER: Preserve the assembler temporary for the final state store. */
            register u32 *page __asm__("$1");
            /* ASM_BARRIER: Publish callback pointers before the completion state. */
            __asm__("" : : : "$1", "memory");
            page=(u32 *)0x800A0000U;
            /* ASM_BARRIER: Expose the success cleanup store in the jump delay slot. */
            __asm__("" : "+r"(page));
            page[0x63F4/4]=0;
        }
        goto out;
    }
    }
    /* ASM_BARRIER: Keep the retry decrement on the unsuccessful probe path. */
    __asm__ volatile("" : : : "memory");
    i--;
    if(i!=-1) goto loop;
    {
        const char *message;
        /* ASM_BARRIER: Load the error message only after all retry attempts fail. */
        __asm__("" : : : "$4");
        message=(const char *)0x80010000U;
        /* ASM_BARRIER: Expose the message offset for the print-call delay slot. */
        __asm__("" : "+r"(message));
        func_8001E84C(message+0x18A8);
    }
    result=0;
out:
    return result;
}
