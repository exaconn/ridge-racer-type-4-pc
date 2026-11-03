/* GCC_VERSION: 2.7.2 */

#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define D_801146BC (*(unsigned int (*))&D_801146BC)

#undef D_801146BC

extern s32 D_801146BC;

extern unsigned int D_801146B8;

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
