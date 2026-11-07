/* GCC_VERSION: 2.7.2 */

#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define RC2WaitLimit (*(unsigned int (*))&RC2WaitLimit)

#undef RC2WaitLimit

extern s32 RC2WaitLimit;

extern unsigned int D_801146B8;

int chkRC2wait() {
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
        lim = RC2WaitLimit;
        v0 = (a0 - b) >> 3;
    } else {
        b = D_801146B8;
        lim = RC2WaitLimit;
        v0 = a0 - b;
    }
    return v0 >= lim;
}
