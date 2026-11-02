#include "types.h"

extern s32 D_800ABCD0;

s32 func_80098030(void) {
    s32 previous = D_800ABCD0;
    /* ASM_BARRIER: use the original assembler temporary for the store address. */
    register s32 *base __asm__("$1");
    /* ASM_BARRIER: load the prior value before materializing the store address. */
    __asm__ volatile("" : "=r"(previous) : "0"(previous) : "$1");
    /* PAL RAM mapping: D_800ABCD0 is base plus the signed offset below. */
    base = (s32 *)0x800B0000U;
    /* ASM_BARRIER: expose a register-relative store so it can occupy the return slot. */
    __asm__("" : "=r"(base) : "0"(base));
    base[(-0x4330) / 4] = 0;
    return previous;
}
