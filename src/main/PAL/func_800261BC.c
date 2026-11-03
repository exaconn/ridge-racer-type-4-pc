#include "types.h"
#include "rr_asm.h"

typedef struct T16 {
    s32 a;
    s32 pa;
    s32 c;
    s32 pc;
} T16;

typedef struct SP {
    char p0[8];
    s32 idx;
    char p1[0x34 - 0xC];
    s32 w34;
    s32 cnt;
    s32 r3c;
    s32 r40;
    char p2[0x64 - 0x44];
    u16 h64, h66, h68, h6a, h6c, h6e, h70, h72, h74, h76;
    s32 w78;
    char p3[0x80 - 0x7C];
    s32 w80;
    char p4[0x110 - 0x84];
    T16 t110[4];
    s16 s150[16];
} SP;

extern s32 D_800ABEA4;
extern s32 D_800ABECC;

s32 func_800261BC(s32 *pos, s32 startIdx) {
    SP *g = (SP *)0x1F800080;
    s32 a, n, t, x;
    s32 r;
    /* ASM_BARRIER: keep g->s150[0] load in $v0 across g->cnt increment */
    register s32 u __asm__("$2");
    char *p, *q;
    s32 junk[2];
    /* ASM_BARRIER: GTE operand temps live in $t2/$t3/$t4 in the original PsyQ-style sequence */
    register s32 r1 __asm__("$10");
    /* ASM_BARRIER: GTE operand temps live in $t2/$t3/$t4 in the original PsyQ-style sequence */
    register s32 r2 __asm__("$11");
    /* ASM_BARRIER: GTE operand temps live in $t2/$t3/$t4 in the original PsyQ-style sequence */
    register s32 r3 __asm__("$12");

    g->cnt = 0;
    g->t110[0].a = pos[0];
    g->t110[0].c = pos[2];
    g->s150[0] = startIdx;
    g->s150[1] = D_800ABECC;
    for (;;) {
        g->w34 = (g->s150[0] + 1) % D_800ABECC;
        a = g->w34;
        p = (char *)D_800ABEA4 + g->s150[0] * 0x3C;
        /* ASM_BARRIER: boost p reference weight so local-alloc assigns $a2 to p and $a3 to q */
        __asm__("" : "+r"(p));
        g->w78 = *(s32 *)(p + 8);
        g->w80 = *(s32 *)(p + 12);
        q = (char *)D_800ABEA4 + a * 0x3C;
        g->h74 = g->h70 = *(u16 *)(q + 8) - (u16)g->w78;
        g->h76 = g->h72 = *(u16 *)(q + 12) - (u16)g->w80;
        g->h64 = (u16)g->t110[0].a - (u16)g->w78;
        g->h66 = (u16)g->t110[0].c - (u16)g->w80;
        g->h70 += *(u16 *)(q + 0);
        g->h72 += *(u16 *)(q + 2);
        g->h74 += *(u16 *)(q + 4);
        g->h76 += *(u16 *)(q + 6);
        g->h68 = *(u16 *)(p + 0);
        /* ASM_BARRIER: keep r3 load from g->h64 after g->h68 store */
        __asm__ volatile("" : : : "memory");
        g->h6a = *(u16 *)(p + 2);
        g->h6c = *(u16 *)(p + 4);
        g->h6e = *(u16 *)(p + 6);
        r3 = *(s32 *)&g->h64;
        r1 = *(s32 *)&g->h68;
        r2 = *(s32 *)&g->h6c;
        RR_GTE_NCLIP3(r1, r2, r3, &g->r3c);
        r1 = *(s32 *)&g->h74;
        r2 = *(s32 *)&g->h70;
        r3 = *(s32 *)&g->h64;
        RR_GTE_NCLIP3(r1, r2, r3, &g->r40);
        if (g->r3c >= 0 && g->r40 >= 0) {
            break;
        }
        n = g->cnt + 1;
        u = *(u16 *)&g->s150[0];
        /* ASM_BARRIER: schedule g->cnt + 1 before sign-extending g->s150[0] */
        __asm__("" : "+r"(u) : "r"(n));
        x = (s16)u;
        g->cnt = n;
        x = (n & 1) ? x + n : x - n;
        t = g->s150[1];
        a = g->s150[0] = x;
        if (a >= 0) {
            g->s150[0] = a % t;
        } else {
            r = (t + a) % t;
            /* ASM_BARRIER: keep modulo result in $v0 before storing to g->s150[0] */
            __asm__("" : "+r"(r));
            g->s150[0] = r;
        }
    }
    return g->s150[0];
}
