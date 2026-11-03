#include "game/car/carphys2.h"
#define W(p,o) (*(int *)((char *)(p) + (o)))
#define H(p,o) (*(short *)((char *)(p) + (o)))

struct S { char pad[0x12]; short x; char pad2[0x3C-0x14]; };

extern struct S *D_800ABEA4;
extern int D_800ABECC;

void func_80025D0C(int a, int b);
void func_80025D98();

void func_80025D0C(int a, int b)
{
  int n = (a + 1) % D_800ABECC;
  register struct S *t asm("$5"); register struct S *p asm("$3"); struct S *q; t = D_800ABEA4; p = &t[a]; q = &t[n]; func_80025C94(p->x, q->x, b, n);
}

/* legacy typing of shared globals in this part of the unit */
#define D_800ABEA4 ((char *)D_800ABEA4)

void func_80025D98(a, r, c) int a; int *r; int c; {
    int n = (a + 1) % D_800ABECC;
    int c2 = c;
    int t = 0x400 - c2;
    char *p = D_800ABEA4 + a * 0x3C;
    char *q = D_800ABEA4 + n * 0x3C;
    r[0] = (t * W(p,8) + c2 * W(q,8)) / 1024;
    r[1] = (t * H(p,0x10) + c2 * H(q,0x10)) / 1024;
    r[2] = (t * W(p,0xC) + c2 * W(q,0xC)) / 1024;
}

#undef D_800ABEA4
