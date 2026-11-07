#include "types.h"

extern s32 CD_read_dma_mode;

s32 CdReadMode(s32 value) {
    s32 previous = CD_read_dma_mode;
    /* ASM_BARRIER: use the original assembler temporary for the store address. */
    register s32 *base __asm__("$1");
    /* ASM_BARRIER: load the prior value before materializing the store address. */
    __asm__ volatile("" : "=r"(previous) : "0"(previous) : "$1");
    /* PAL RAM mapping: CD_read_dma_mode is base plus the signed offset below. */
    base = (s32 *)0x800A0000U;
    /* ASM_BARRIER: expose a register-relative store so it can occupy the return slot. */
    __asm__("" : "=r"(base) : "0"(base));
    base[(0x63F4) / 4] = value;
    return previous;
}
