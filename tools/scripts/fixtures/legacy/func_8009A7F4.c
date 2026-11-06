/* GCC_VERSION: 2.7.2 */
#include "types.h"
extern s32 D_801146BC;
void setRC2wait(s32 limit) {
    /* ASM_BARRIER: keep the zero-extended timer read in its original register. */
    register s32 counter __asm__("$2") = *(volatile u16 *)0x1F801120;
    /* ASM_BARRIER: retain the original assembler temporary for the mapped store. */
    register s32 *base __asm__("$1");
    D_801146BC = limit;
    /* ASM_BARRIER: finish the limit store before binding the saved-counter address. */
    __asm__ volatile("" : : : "$1", "memory");
    /* PAL RAM mapping for D_801146B8. */
    base = (s32 *)0x80110000U;
    /* ASM_BARRIER: expose the register-relative return-slot store. */
    __asm__("" : "=r"(base) : "0"(base));
    base[0x46B8 / 4] = counter;
}
