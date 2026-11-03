#include "types.h"
#include "rr_asm.h"
#include "rr/menu_overlay_runtime.h"
#include "rr/overlay668_camera.h"

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

struct Q4 {
    s32 w[4];
};

typedef struct Car {
    s32 w0;
    s32 w4;
    s32 w8;
    s32 wC;
    s32 w10;
    s32 w14;
    s32 w18;
    s32 w1C;
    s32 w20;
    s32 w24;
    s32 w28;
    char pad2C[0x88 - 0x2C];
    struct Q4 q88;
    struct Q4 q98;
    struct Q4 qA8;
    struct Q4 qB8;
    char padC8[0xF0 - 0xC8];
    s32 wF0;
    s32 wF4;
    char padF8[0x170 - 0xF8];
    s16 s170;
    s16 s172;
    char pad174[0x184 - 0x174];
    s32 w184;
    s32 w188;
    s32 w18C;
    s32 w190;
    s16 s194;
    char pad196[0x1A0 - 0x196];
    s16 s1A0;
    char pad1A2[0x1EC - 0x1A2];
    s16 s1EC;
} Car;

typedef struct Node {
    char pad0[8];
    s32 w8;
    s32 wC;
    char pad10[2];
    s16 s12;
    char pad14[4];
    s16 s18;
    s16 s1A;
    s16 s1C;
    s16 s1E;
    s16 s20;
    s16 s22;
    s32 w24;
    char pad28[4];
    s16 s2C;
    s16 s2E;
    char pad30[0x3C - 0x30];
} Node;

typedef struct T16_1 {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
} T16_1;

typedef struct SP_1 {
    Car *p0;
    char pad4[0x10 - 0x04];
    s32 w10;
    s32 w14;
    s32 w18;
    s32 w1C;
    s32 w20;
    s16 s24;
    s16 s26;
    s16 s28;
    s16 s2A;
    s32 w2C;
    s32 w30;
    s32 w34;
    s32 w38;
    s32 w3C;
    s32 w40;
    char pad44[0xB8 - 0x44];
    s16 sB8;
    s16 sBA;
    s16 sBC;
    s16 sBE;
    s16 sC0;
    s16 sC2;
    s16 sC4;
    s16 sC6;
    s16 sC8;
    char padCA[0xF8 - 0xCA];
    s16 sF8;
    s16 sFA;
    s16 sFC;
    char padFE[2];
    s32 w100;
    s32 w104;
    s32 w108;
    char pad10C[4];
    T16_1 t110[4];
    char pad150[0x15A - 0x150];
    s16 s15A;
    s16 s15C;
    char pad15E[2];
    s16 s160;
    s16 s162;
    char pad164[0x174 - 0x164];
    s16 s174;
    s16 s176;
} SP_1;

void func_80026C80(void);
void func_80026F5C(Car *arg0);

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

/* legacy typing of shared globals in this part of the unit */
#define D_800ABEA4 ((Node *)D_800ABEA4)

void func_80026F5C(Car *arg0) {
    SP_1 *g = (SP_1 *)0x1F800080;
    s32 idx, next_idx;
    Node *s1, *s2;
    s32 i;
    s16 c, s;

    g->p0 = arg0;
    *(struct Q4 *)0x1F800190 = arg0->q88;
    *(struct Q4 *)0x1F8001A0 = arg0->q98;
    *(struct Q4 *)0x1F8001B0 = arg0->qA8;
    *(struct Q4 *)0x1F8001C0 = arg0->qB8;
    g->p0->s1A0 = 0;
    g->w2C = 0;
    g->sC0 = 0;
    g->sBA = 0;
    g->sC8 = 0;
    g->sC6 = 0;
    g->sC4 = 0;
    idx = g->p0->wF0;
    next_idx = (idx + 1) % D_800ABECC;
    s1 = &D_800ABEA4[idx];
    g->s174 = s1->s22;
    g->s176 = s1->s20;
    s2 = &D_800ABEA4[next_idx];
    g->s24 = func_80090424(g->s174);
    g->s26 = func_800904F4(g->s174);
    g->s28 = func_80090424(g->s176);
    g->s2A = func_800904F4(g->s176);
    if (g->s174 != g->s176) {
        g->w38 = 1;
        g->s162 = s1->s1C;
        g->w3C = g->s162;
        g->s160 = s1->s12 + 0x400;
    } else {
        g->w38 = 0;
        g->s162 = (s1->s1C * g->s26) >> 12;
        g->w3C = g->s162;
        g->w40 = (g->s24 * s1->s1C) >> 12;
        g->s160 = s1->s1E + 0x400;
    }
    g->sF8 = g->p0->w20 - s1->w8;
    g->sFC = g->p0->w28 - s1->wC;
    g->sFA = 0;
    g->w20 = func_800904F4(g->s160);
    g->w1C = func_80090424(g->s160);
    c = g->w20;
    s = g->w1C;
    g->sB8 = g->sC2 = c;
    g->sBE = -s;
    g->sBC = s;
    RR_GTE_SETROT(&g->sB8);
    RR_GTE_MVMVA_ROT_V0_NOCV(&g->sF8);
    g->s15C = (s2->s18 + s2->s2E) - (s1->s18 + s1->s2E);
    g->s15A = (s2->s1A + s2->s2C) - (s1->s1A + s1->s2C);
    RR_GTE_STORE_IR(&g->w100);
    g->w14 = g->w100;
    g->w18 = g->w104;
    if (g->w38 == 1) {
        g->w40 = (g->w18 * g->s24) / g->s26;
        g->w14 -= g->w40;
        g->w3C -= g->w40;
        g->w3C += (g->w18 * g->s28) / g->s2A;
        g->w14 = (g->w14 * g->s162) / g->w3C;
    } else {
        g->w18 -= (g->w40 * g->w14) / g->s162;
    }
    if (g->w14 < 0) {
        g->w14 = 0;
    } else if (g->w14 > g->s162) {
        g->w14 = g->s162;
    }
    g->w3C = 0;
    g->w40 = 0;
    g->s24 = 0;
    g->s26 = 0;
    g->w10 = (g->w14 << 10) / g->s162;
    for (i = 0; i < 4; i++) {
        g->w100 = g->sF8 + (g->t110[i].a >> 2);
        g->w108 = g->sFC + (g->t110[i].c >> 2);
        g->w30 = (-g->w1C * g->w100 + g->w20 * g->w108) >> 12;
        if (g->w30 > g->w3C) {
            g->w3C = g->w30;
            g->s24 = i + 1;
        } else if (g->w30 < g->w40) {
            g->w40 = g->w30;
            g->s26 = i + 1;
        }
    }
    g->s15C = ((g->s15C * g->w10) >> 10) + (s1->s18 + s1->s2E);
    g->s15A = ((g->s15A * g->w10) >> 10) + (s1->s1A + s1->s2C);
    if (g->w40 < -g->s15C) {
        g->sFC = g->w40 + g->s15C;
        g->w18 -= g->sFC;
        g->w2C = g->s26;
    } else if (g->w3C > g->s15A) {
        g->sFC = g->w3C - g->s15A;
        g->w18 -= g->sFC;
        g->w2C = g->s24;
    }
    if (g->w2C > 0) {
        g->sF8 = 0;
        g->sFA = 0;
        g->sB8 = g->w20;
        g->sBA = 0;
        g->sBC = -(s16)g->w1C;
        g->sBE = 0;
        g->sC0 = 0x1000;
        g->sC2 = 0;
        g->sC4 = g->w1C;
        g->sC6 = 0;
        g->sC8 = g->w20;
        RR_GTE_SETROT(&g->sB8);
        RR_GTE_MVMVA_ROT_V0_NOCV(&g->sF8);
        RR_GTE_STORE_IR(&g->w100);
        g->p0->w0 -= g->w100 << 8;
        g->p0->w8 -= g->w108 << 8;
        g->p0->w10 = g->p0->w0 >> 6;
        g->p0->w18 = g->p0->w8 >> 6;
    }
    g->s160 = s2->s12 - s1->s12;
    if (g->s160 > 0x400) {
        g->s160 -= 0x1000;
    } else if (g->s160 < -0x400) {
        g->s160 = -0x1000 - g->s160;
    }
    g->p0->s1EC = s1->s12 + ((g->s160 * g->w14) >> 10);
    g->p0->s170 = (g->w14 << 10) / g->s162;
    if (g->w18 < 0) {
        g->p0->s172 = (g->w18 << 10) / g->s15C;
    } else {
        g->p0->s172 = (g->w18 << 10) / g->s15A;
    }
    g->p0->wF4 = g->w18;
    g->p0->w188 = (D_800AD522 != 0) ? g->w14 : (g->s162 - g->w14);
    g->p0->w190 = g->p0->w18C;
    g->p0->w18C = (g->p0->w184 + g->p0->w188 + D_800F2EB8) % D_800F2EB8;
    if (g->p0->w18C < 0) {
        g->p0->w18C += D_800F2EB8;
    }
    g->p0->s194 = (s1->w24 + g->w14) >> 4;
    g->p0->s1A0 = g->w2C;
}

#undef D_800ABEA4
