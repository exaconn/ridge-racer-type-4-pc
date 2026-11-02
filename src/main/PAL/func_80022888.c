#include "rr_asm.h"
extern void func_80091844();
#define H(p, o) (*(unsigned short *)((char *)(p) + (o)))
void func_80022888(char *a0)
{
    unsigned short v[3];
    int m[8];
    int i, n, j;
    char *p, *q;
    v[0] = 0;
    v[1] = H(a0, 0x54);
    v[2] = 0;
    func_80091844(v, m);
    RR_GTE_SETROT(m);
    v[0] = H(a0, 0x110);
    v[1] = H(a0, 0x8C);
    i = 0;
    v[2] = H(a0, 0x112);
    do {
        RR_GTE_MVMVA_ROT_V0_NOCV(v);
        n = i + 1;
        q = a0 + n * 4;
        v[0] = H(q, 0x110);
        j = i << 4;
        v[1] = H(a0 + j, 0x8C);
        j += 0x88;
        v[2] = H(q, 0x112);
        p = a0 + j;
        RR_GTE_STORE_IR(p);
        i = n;
    } while (i < 4);
}
