#include "types.h"

void func_8008A624(void) {
    s32 value = 1;
    /* ASM_BARRIER: preserve the original assembler temporary as the address base. */
    register s16 *base __asm__("$1") = (s16 *)0x80110000U;
    /* PAL RAM mapping: D_801100E8 is base plus 0xE8. */
    /* ASM_BARRIER: retain the address and value registers for the return-slot store. */
    __asm__("" : "=r"(base), "=r"(value) : "0"(base), "1"(value));
    base[(0xE8) / 2] = value;
}
