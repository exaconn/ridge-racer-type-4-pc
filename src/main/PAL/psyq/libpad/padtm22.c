/* GCC_VERSION: 970318 */
#include "types.h"

extern s32 D_801146BC;
extern unsigned int D_801146B8;

void func_8009A7F4(s32 limit);
int func_8009A814();

void func_8009A7F4(s32 limit) {
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

int func_8009A814() {
    unsigned int a0;
    /* ASM_BARRIER: pin MMIO pointer to $v1 (original keeps it in v1, accumulator in a0) */
    register volatile unsigned short *p __asm__("$3");
    unsigned int v0, lim, b;
    a0 = *(volatile unsigned short *)0x1F801120 & 0xFFFF;
    if (a0 < D_801146B8) {
        p = (unsigned short *)0x1F801128;
        if (*p != 0) {
            a0 += *p;
        } else {
            a0 += 0x10000;
        }
    }
    if (!(*(volatile unsigned short *)0x1F801124 & 0x200)) {
        b = D_801146B8;
        lim = D_801146BC;
        v0 = (a0 - b) >> 3;
    } else {
        b = D_801146B8;
        lim = D_801146BC;
        v0 = a0 - b;
    }
    return v0 >= lim;
}
