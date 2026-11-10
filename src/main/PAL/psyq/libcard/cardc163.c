/* RODATA 0x800121a0 0x3c */
/* GCC_VERSION: 970318 */
#include "psyq/libcard/cardc163.h"
#include "rr/commands.h"

int _cardCmd5ATx() {
    /* ASM_BARRIER: pin return value to $v0 across the comparison. */
    register int r asm("$2") = 0x5A;
    int d;
    d = CardTxCount;
    if (d != 2) r = 0;
    return r;
}

int _cardCmd5ARx(a0) int a0; {
    int v;
    int r;
    char *c;
    switch (CardRxCount) {
    case 3:
        CardBytesRemaining = a0 & 0xFF;
        break;
    case 4:
        CardCmdBuffer[1] = a0;
        break;
    case 5:
        CardCmdBuffer[0] = a0;
        break;
    case 6:
        CardCmdBuffer[0x10] = a0;
        break;
    case 17:
        CardCmdBuffer[0xC] = a0 - 1;
        break;
    default:
        v = CardRxCount;
        if ((v - 9) < 4U) {
            c = CardCmdBuffer + 0x10;
            c -= v;
            *c = a0;
            break;
        }
        if (v < 7) break;
        CardCmdBuffer[v - 5] = a0;
        break;
    }
    r = 0;
out:
    return r;
}
