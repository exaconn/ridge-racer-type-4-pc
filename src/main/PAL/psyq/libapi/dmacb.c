#include "types.h"
#include "m2c_macros.h"
#include "rr/spu_tick.h"

extern s32 D_800A63D0;

s32 func_8008BDE4(s32 value);

s32 func_8008BDE4(s32 value) {
    s32 previous = D_800A63D0;
    /* ASM_BARRIER: use the original assembler temporary for the store address. */
    register s32 *base __asm__("$1");
    /* ASM_BARRIER: load the prior value before materializing the store address. */
    __asm__ volatile("" : "=r"(previous) : "0"(previous) : "$1");
    /* PAL RAM mapping: D_800A63D0 is base plus the signed offset below. */
    base = (s32 *)0x800A0000U;
    /* ASM_BARRIER: expose a register-relative store so it can occupy the return slot. */
    __asm__("" : "=r"(base) : "0"(base));
    base[(0x63D0) / 4] = value;
    return previous;
}

s32 func_8008BDF8(void) {
    return D_800A63D0;
}
