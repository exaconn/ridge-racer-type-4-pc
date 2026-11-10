/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/counters.h"
#include "psyq/libpad/padtm22.h"
#include "rr/serial.h"

void setRC2wait(s32 limit) {
    u16 counter = ((RootCounterRegisters *)0x1F801120)->count;
    s32 *base;
    RC2WaitLimit = limit;
    /* PAL RAM mapping for PadRcnt2Start. */
    base = (s32 *)0x80110000U;
    base[0x46B8 / 4] = counter;
}

int chkRC2wait() {
    unsigned int a0;
    /* ASM_BARRIER: pin MMIO pointer to $v1 (original keeps it in v1, accumulator in a0) */
    register RootCounterRegisters *p __asm__("$3");
    unsigned int v0, lim, b;
    a0 = ((RootCounterRegisters *)0x1F801120)->count & 0xFFFF;
    if (a0 < PadRcnt2Start) {
        p = (RootCounterRegisters *)0x1F801128;
        if (p->count != 0) {
            a0 += p->count;
        } else {
            a0 += 0x10000;
        }
    }
    if (!(((RootCounterRegisters *)0x1F801124)->count & 0x200)) {
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

