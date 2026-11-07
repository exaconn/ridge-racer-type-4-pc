/* CC1_FLAGS: -mno-split-addresses */

#include "rr_asm.h"

#include "rr/serial.h"

/* legacy typing of shared globals in this part of the unit */
#define CardSio0Base ((S *)CardSio0Base)

#define I_STAT_CardSel ((int *)I_STAT_CardSel)

#define setRC2wait ((int (*)())setRC2wait)

#undef CardSio0Base

#undef I_STAT_CardSel

#undef setRC2wait

typedef struct { unsigned char b0; char p0[3]; unsigned short h4; char p1[4]; unsigned short hA; } S;

int _cardSelectSlot(a0) int a0; {
    volatile S *p, *q = CardSio0Base;
    q->hA = 0;
    setRC2wait(0x91);
    p = CardSio0Base;
    p->hA = a0 ? 0x3003 : 0x1003;
    if (p->h4 & 0x200) {
        p->hA |= 0x10;
        if (p->h4 & 0x200) {
            p->b0 = 1;
            p = CardSio0Base;
            /* ASM_BARRIER: orig has the load-delay nop BEFORE the loop label; maspsx emits it after the label (inside the loop) */
            RR_NOP_DEP();
            while ((p->h4 & 2) == 0) ;
            p = CardSio0Base;
            p->b0;
            return 0;
        }
        *I_STAT_CardSel = -0x81;
    }
    return 1;
}
