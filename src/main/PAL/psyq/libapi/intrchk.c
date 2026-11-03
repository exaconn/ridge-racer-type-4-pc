/* GCC_VERSION: 2.7.2 */
#include "rr/interrupts.h"
s32 func_80097680(void) {
    RrInterruptRegisters *registers=D_800ABC58;
    /* ASM_BARRIER: Keep the predicate result in the original return register. */
    register s32 result;
    if(registers->mask&1) {
        if(registers->pending&1) result=1;
        else {
            result=0;
            /* ASM_BARRIER: Preserve the conditional false-result block instead of folding the predicate. */
            __asm__ volatile("" : : "r"(result));
        }
    } else result=0;
    return result;
}
