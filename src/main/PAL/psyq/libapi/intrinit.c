/* CC1_FLAGS: -mno-split-addresses */
#include "psyq/libapi/intrdisp.h"
#include "psyq/libapi/intrdisp.h"
#include "psyq/libapi/vsyncini.h"
extern int trapIntr();
extern int startIntrDMA();
extern int func_800963F4();
extern int _96_remove();
extern int HookEntryInt();
extern int ExitCriticalSection();
extern unsigned short D_800A52C8;
extern char D_800A5304[];
extern int *D_800A6350;
extern unsigned short *D_800A6354;
extern unsigned short *I_MASK;
char *startIntr()
{
    char *p, *r;
    unsigned short *q = &D_800A52C8;
    unsigned short *a, *b;
    if (*q == 0) {
        a = D_800A6354;
        b = I_MASK;
        *b = 0;
        *a = *(volatile unsigned short *)b;
        *D_800A635C = 0x33333333;
        memclr(q, 0x41A);
        if (func_800963F4((char *)q + 0x38)) trapIntr();
        p = D_800A5304;
        *(char **)p = p + 0xFDC;
        HookEntryInt(p - 4);
        *(short *)(p - 0x3C) = 1;
        D_800A6350[5] = startIntrVSync();
        D_800A6350[1] = startIntrDMA();
        _96_remove(D_800A6350);
        r = p - 0x3C;
        ExitCriticalSection();
        return r;
    }
    return 0;
}
