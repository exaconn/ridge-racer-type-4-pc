#include "types.h"

void SsSetMono(void) {
    s32 value = 1;
    /* ASM_BARRIER: preserve the original assembler temporary as the address base. */
    register s16 *base __asm__("$1") = (s16 *)0x80110000U;
    /* PAL RAM mapping: _svm_stereo_mono is base plus 0xE8. */
    /* ASM_BARRIER: retain the address and value registers for the return-slot store. */
    __asm__("" : "=r"(base), "=r"(value) : "0"(base), "1"(value));
    base[(0xE8) / 2] = value;
}
