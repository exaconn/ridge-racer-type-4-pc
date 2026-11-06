/* CC1_FLAGS: -mno-split-addresses */

#include "rr_asm.h"

#include "rr/serial.h"

/* legacy typing of shared globals in this part of the unit */
#define D_800ABDD0 ((S *)D_800ABDD0)

#define D_800ABDD4 ((int *)D_800ABDD4)

#define setRC2wait ((int (*)())setRC2wait)

#undef D_800ABDD0

#undef D_800ABDD4

#undef setRC2wait

typedef struct { unsigned char b0; char p0[3]; unsigned short h4; char p1[4]; unsigned short hA; } S;

int func_8009BD44(a0) int a0; {
    volatile S *p, *q = D_800ABDD0;
    q->hA = 0;
    setRC2wait(0x91);
    p = D_800ABDD0;
    p->hA = a0 ? 0x3003 : 0x1003;
    if (p->h4 & 0x200) {
        p->hA |= 0x10;
        if (p->h4 & 0x200) {
            p->b0 = 1;
            p = D_800ABDD0;
            /* ASM_BARRIER: orig has the load-delay nop BEFORE the loop label; maspsx emits it after the label (inside the loop) */
            RR_NOP_DEP();
            while ((p->h4 & 2) == 0) ;
            p = D_800ABDD0;
            p->b0;
            return 0;
        }
        *D_800ABDD4 = -0x81;
    }
    return 1;
}
