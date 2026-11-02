#include "rr/ui.h"

extern int func_80053E88();
extern int func_80096464();
extern RrUiParticle20 D_800F2AE4[];
extern unsigned char D_800A0CF4[];

void func_80053EC4()
{
    RrUiParticle20 *s0;
    unsigned char *s1;
    /* ASM_BARRIER: pin table index to $v1 to match original register allocation */
    register int ix asm("$3");
    unsigned char *tb;
    int v0, r1, r2, r3, r4, r5, r6, one;
    int s5, s2;
    v0 = func_80053E88();
    if (v0 >= 0) {
        s0 = &D_800F2AE4[v0];
        one = 1;
        s0->active00 = one;
        s0->state04 = 0;
        s0->timer08 = 0;
        r1 = func_80096464();
        v0 = r1 % 240 - 0x28;
        s0->x0C = v0;
        s5 = (short)v0;
        r2 = func_80096464();
        s0->spanX14 = 0x168 - (u16)s0->x0C;
        s0->y0E = r2 % 140 + 0xAA;
        r3 = func_80096464();
        s2 = ((r3 & 3) + one) << one;
        s0->speed16 = s2;
        r4 = func_80096464();
        ix = r4 - (r4 / 2) * 2; tb = D_800A0CF4; s1 = tb + (ix << 2);
        s0->r10 = s1[0];
        s0->g11 = s1[1];
        s0->b12 = s1[2];
        s0->rot18 = 0;
        s0->velX1C = 0;
        s0->velY1E = 0;
        s0->scale1A = 0x100;
        v0 = func_80053E88();
        if (v0 >= 0) {
            s0 = &D_800F2AE4[v0];
            s0->active00 = one;
            s0->state04 = 0;
            s0->timer08 = 0;
            r5 = func_80096464();
            s0->x0C = s5 + r5 % 60 - 0x1E;
            r6 = func_80096464();
            s0->speed16 = s2;
            s0->spanX14 = 0x168 - (u16)s0->x0C;
            s0->y0E = r6 % 140 + 0xAA;
            s0->r10 = s1[0];
            s0->g11 = s1[1];
            s0->b12 = s1[2];
            s0->rot18 = 0;
            s0->velX1C = 0;
            s0->velY1E = 0;
            s0->scale1A = 0x100;
        }
    }
}
