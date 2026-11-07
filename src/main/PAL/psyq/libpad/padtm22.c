/* GCC_VERSION: 2.7.2 */
#include "types.h"
#include "psyq/libpad/padtm22.h"
#include "rr/serial.h"

void setRC2wait(s32 limit) {
    /* ASM_BARRIER: keep the zero-extended timer read in its original register. */
    register s32 counter __asm__("$2") = *(volatile u16 *)0x1F801120;
    /* ASM_BARRIER: retain the original assembler temporary for the mapped store. */
    register s32 *base __asm__("$1");
    RC2WaitLimit = limit;
    /* ASM_BARRIER: finish the limit store before binding the saved-counter address. */
    __asm__ volatile("" : : : "$1", "memory");
    /* PAL RAM mapping for PadRcnt2Start. */
    base = (s32 *)0x80110000U;
    /* ASM_BARRIER: expose the register-relative return-slot store. */
    __asm__("" : "=r"(base) : "0"(base));
    base[0x46B8 / 4] = counter;
}

/* legacy typing of shared globals in this part of the unit */
#define RC2WaitLimit (*(unsigned int (*))&RC2WaitLimit)

int chkRC2wait() {
    unsigned int a0;
    /* ASM_BARRIER: pin MMIO pointer to $v1 (original keeps it in v1, accumulator in a0) */
    register volatile unsigned short *p __asm__("$3");
    unsigned int v0, lim, b;
    a0 = *(volatile unsigned short *)0x1F801120 & 0xFFFF;
    if (a0 < PadRcnt2Start) {
        p = (unsigned short *)0x1F801128;
        if (*p != 0) {
            a0 += *p;
        } else {
            a0 += 0x10000;
        }
    }
    if (!(*(volatile unsigned short *)0x1F801124 & 0x200)) {
        b = PadRcnt2Start;
        lim = RC2WaitLimit;
        v0 = (a0 - b) >> 3;
    } else {
        b = PadRcnt2Start;
        lim = RC2WaitLimit;
        v0 = a0 - b;
    }
    return v0 >= lim;
}

#undef RC2WaitLimit
