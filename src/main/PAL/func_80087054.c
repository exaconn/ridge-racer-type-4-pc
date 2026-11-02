#include "types.h"

void func_80087054(void) {
    s32 value = 2;
    /* ASM_BARRIER: preserve the original assembler temporary as the address base. */
    register s16 *base __asm__("$1") = (s16 *)0x80110000U;
    /* PAL RAM mapping: D_801100A0 is base plus 0xA0. */
    /* ASM_BARRIER: retain the address and value registers for the return-slot store. */
    __asm__("" : "=r"(base), "=r"(value) : "0"(base), "1"(value));
    base[(0xA0) / 2] = value;
}
