/* RODATA 0x800121a0 0x3c */
/* GCC_VERSION: 970318 */
#include "psyq/libcard/cardc163.h"
#include "rr/commands.h"
extern volatile int D_800ABDE8;
extern int D_800ABDF8;
extern int D_80114730;

int func_8009D7A0() {
 register int r asm("$2") = 0x5A; int d; d = D_800ABDE8; if (d != 2) r = 0; return r;
}

int func_8009D7C4(a0) int a0; {
    int v;
    int r;
    char *c;
    switch (D_800ABDF8) {
    case 3:
        D_80114730 = a0 & 0xFF;
        break;
    case 4:
        D_80114718[1] = a0;
        break;
    case 5:
        D_80114718[0] = a0;
        break;
    case 6:
        D_80114718[0x10] = a0;
        break;
    case 17:
        D_80114718[0xC] = a0 - 1;
        break;
    default:
        v = D_800ABDF8;
        if ((unsigned)(v - 9) < 4) {
            c = D_80114718 + 0x10;
            c -= v;
            *c = a0;
            break;
        }
        if (v < 7) break;
        D_80114718[v - 5] = a0;
        break;
    }
    r = 0;
out:
    return r;
}
