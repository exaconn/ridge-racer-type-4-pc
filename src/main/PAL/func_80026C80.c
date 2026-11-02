#include "types.h"
#include "rr_asm.h"
typedef struct T16 { u16 a; u16 pa[3]; u16 c; u16 pc[3]; } T16;
typedef struct SP {
    char p0[8]; s32 idx;
    char p1[0x34 - 0xC]; s32 w34; s32 cnt; s32 r3c; s32 r40;
    char p2[0x64 - 0x44];
    u16 h64, h66, h68, h6a, h6c, h6e, h70, h72, h74, h76;
    s32 w78;
    char p3[0x80 - 0x7C]; s32 w80;
    char p4[0x110 - 0x84];
    T16 t110[4];
    s16 s150[16];
} SP;
extern s32 D_800ABEA4;
extern s32 D_800ABECC;
void func_80026C80(void)
{
    SP *g = (SP *)0x1F800080;
    s32 a, n, t, x;
    char *p, *q;
    s32 junk[2];
    /* ASM_BARRIER: GTE operand temps live in $t2/$t3/$t4 in the original PsyQ-style sequence */
    register s32 r1 __asm__("$10");
    register s32 r2 __asm__("$11");
    register s32 r3 __asm__("$12");
    g->cnt = 0;
    for (;;) {
        g->w34 = (g->s150[g->idx] + 1) % D_800ABECC;
        a = g->w34;
        p = (char *)D_800ABEA4 + g->s150[g->idx] * 0x3C;
        g->w78 = *(s32 *)(p + 8);
        g->w80 = *(s32 *)(p + 12);
        q = (char *)D_800ABEA4 + a * 0x3C;
        g->h74 = g->h70 = *(u16 *)(q + 8) - (u16)g->w78;
        g->h76 = g->h72 = *(u16 *)(q + 12) - (u16)g->w80;
        g->h64 = g->t110[g->idx].a - (u16)g->w78;
        g->h66 = g->t110[g->idx].c - (u16)g->w80;
        g->h70 += *(u16 *)(q + 0);
        g->h72 += *(u16 *)(q + 2);
        g->h74 += *(u16 *)(q + 4);
        g->h76 += *(u16 *)(q + 6);
        g->h68 = *(u16 *)(p + 0);
        g->h6a = *(u16 *)(p + 2);
        g->h6c = *(u16 *)(p + 4);
        g->h6e = *(u16 *)(p + 6);
        r3 = *(s32 *)&g->h64; r1 = *(s32 *)&g->h68; r2 = *(s32 *)&g->h6c;
        RR_GTE_NCLIP3(r1, r2, r3, &g->r3c);
        r1 = *(s32 *)&g->h74; r2 = *(s32 *)&g->h70; r3 = *(s32 *)&g->h64;
        RR_GTE_NCLIP3(r1, r2, r3, &g->r40);
        if (g->r3c >= 0 && g->r40 >= 0)
            return;
        n = g->cnt + 1;
        g->cnt = n;
        x = g->s150[g->idx];
        g->s150[g->idx] = (n & 1) ? x + n : x - n;
        a = g->s150[g->idx];
        if (a >= 0)
            g->s150[g->idx] = a % D_800ABECC;
        else
            g->s150[g->idx] = (D_800ABECC + a) % D_800ABECC;
    }
}
