/* CC1_FLAGS: -mno-split-addresses */
#include "psyq/libapi/intrdisp.h"
#include "psyq/libapi/intrdisp.h"
#include "psyq/libapi/vsyncini.h"
extern int func_8008B5D0();
extern int startIntrDMA();
extern int func_800963F4();
extern int func_80096E3C();
extern int func_80096EF4();
extern int func_80096F14();
extern unsigned short D_800A52C8;
extern char D_800A5304[];
extern int *D_800A6350;
extern unsigned short *D_800A6354;
extern unsigned short *D_800A6358;
char *func_8008B4F8()
{
    char *p, *r;
    unsigned short *q = &D_800A52C8;
    unsigned short *a, *b;
    if (*q == 0) {
        a = D_800A6354;
        b = D_800A6358;
        *b = 0;
        *a = *(volatile unsigned short *)b;
        *D_800A635C = 0x33333333;
        func_8008BA00(q, 0x41A);
        if (func_800963F4((char *)q + 0x38)) func_8008B5D0();
        p = D_800A5304;
        *(char **)p = p + 0xFDC;
        func_80096EF4(p - 4);
        *(short *)(p - 0x3C) = 1;
        D_800A6350[5] = func_8008BA24();
        D_800A6350[1] = startIntrDMA();
        func_80096E3C(D_800A6350);
        r = p - 0x3C;
        func_80096F14();
        return r;
    }
    return 0;
}
