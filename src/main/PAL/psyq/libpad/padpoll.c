/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "psyq/libpad/padtm22.h"
#include "psyq/libpad/padpoll.h"
struct HW { volatile unsigned char b0; char p1[3]; volatile unsigned short s4; short p6; short s8; unsigned short sA; char pC[2]; volatile short sE; };

struct Q { int a; };

struct W { char p[0x3C]; unsigned char *f3c; char p2[4]; unsigned char f44, f45; char p3[0xE8 - 0x46]; unsigned char fE8; };

struct R { short a; short b; unsigned short c; short d; short e; unsigned short f; };

struct Q_1 { int a, b; };

extern int setRC2wait();
extern int chkRC2wait();
extern int PadRecvStage;
extern struct HW *PadSio0Base;
extern struct Q *I_STAT_Pad;

int _padSioRW2(a0, a1) struct W *a0; int a1; {
    int s1;
    int s2;
    unsigned v1;
    struct HW *hw;
    /* ASM_BARRIER: keep *a0->f3c in $v0 so combine preserves sra. */
    register int c asm("$2");
    s1 = 0x88;
    c = *a0->f3c;
    if ((c >> 4) == 8) {
        if (a0->f44 >= 9) s1 = 0x22;
    }
    do { } while (!(PadSio0Base->s4 & 2));
    setRC2wait(0x190);
    hw = PadSio0Base;
    s2 = hw->b0;
    if (a0->f44 != 0 || (s2 >> 4) != 8) {
        hw->sE = s1;
    } else {
        hw->sE = 0x22;
    }
    while (!(*(volatile int *)I_STAT_Pad & 0x80)) {
        v1 = *(volatile unsigned short *)0x1F801120;
        if (v1 < D_801146B8) {
            if (*(volatile unsigned short *)0x1F801128 != 0) v1 += *(volatile unsigned short *)0x1F801128;
            else v1 += 0x10000;
        }
        if (*(volatile unsigned short *)0x1F801124 & 0x200) {
            if (v1 - D_801146B8 >= RC2WaitLimit) return -2;
        } else {
            if (((v1 - D_801146B8) >> 3) >= RC2WaitLimit) return -2;
        }
    }
    if (a0->fE8 != 8) {
        if (PadRecvStage == 2) {
            setRC2wait(0x3C);
            do { } while (chkRC2wait() == 0);
        }
    }
    PadSio0Base->b0 = a1;
    if (PadRecvStage == 3) {
        if (s2 == 0x80) {
            I_STAT_Pad->a = -0x81;
            PadSio0Base->sA |= 0x10;
        }
    }
    a0->f45++;
    if (a0->f44 != 0xFF) {
        /* ASM_BARRIER: keep f3c base in $v0 and reloaded f44 index in $v1. */
        register unsigned char *p asm("$2");
        /* ASM_BARRIER: keep reloaded f44 index in $v1 so f3c base uses $v0. */
        register int idx asm("$3");
        idx = *(volatile unsigned char *)&a0->f44;
        /* ASM_BARRIER: order the index load before the pointer load. */
        __asm__("" : "=r"(idx) : "0"(idx));
        p = a0->f3c + idx;
        *p = s2;
    }
    a0->f44++;
    return s2;
}

/* legacy typing of shared globals in this part of the unit */
#define I_STAT_Pad ((struct Q_1 *)I_STAT_Pad)
#define PadSio0Base ((struct R *)PadSio0Base)
#define chkRC2wait ((int (*)())chkRC2wait)

int _padClrIntSio0() {
    I_STAT_Pad->a = -0x81;
    if (PadSio0Base->c & 0x80) {
        do {
            if (chkRC2wait()) return 0;
        } while (PadSio0Base->c & 0x80);
    }
    PadSio0Base->f |= 0x10;
    return 1;
}

#undef I_STAT_Pad
#undef PadSio0Base
#undef chkRC2wait
