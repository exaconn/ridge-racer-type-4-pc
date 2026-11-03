/* GCC_VERSION: 2.7.2 */
#include "rr/interrupts.h"

s32 func_8009BAD8(void) {
    RrInterruptRegisters *registers=D_800ABDAC;
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

s32 func_8009BB18(void) {
    RrInterruptRegisters *registers=D_800ABDAC;
    register s32 result;
    if(registers->mask&0x80) {
        if(registers->pending&0x80) result=1;
        else {
            result=0;
            /* ASM_BARRIER: Preserve the conditional false-result block instead of folding the predicate. */
            __asm__ volatile("" : : "r"(result));
        }
    } else result=0;
    return result;
}
