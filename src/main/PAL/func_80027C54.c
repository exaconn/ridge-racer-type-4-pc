#include "types.h"
#include "rr_asm.h"

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
    union {
        struct Q4 q20;
        struct {
            s32 w20;
            s32 w24;
            s32 w28;
            s32 w2C;
        } s;
    } u20;
    char pad30[0x88 - 0x30];
    struct Q4 q88;
    struct Q4 q98;
    struct Q4 qA8;
    struct Q4 qB8;
    char padC8[0xF0 - 0xC8];
    s32 wF0;
    s32 wF4;
    s16 sF8[4];
    char pad100[0x10C - 0x100];
    s16 s10C;
    char pad10E[0x1F0 - 0x10E];
    union {
        s32 w1F0;
        u8 b1F0[4];
    } u1F0;
} Car;

typedef struct Z52 {
    s32 w0;
    s32 w4;
    s16 s8;
    s16 sA;
    s16 sC;
    s16 sE;
    s16 s10;
    s16 s12;
    s16 s14;
    s16 s16;
    s16 s18;
    s16 s1A;
    s32 w1C;
    struct {
        u8 b0;
        u8 b1;
    } a20[2];
    s16 s24[2];
    s16 s28[2];
    s16 s2C[2];
    s16 s30[2];
} Z52;

typedef struct Node {
    s32 pad0[2];
    Z52 z8;
} Node;

typedef struct T16 {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
} T16;

typedef struct SP {
    Car *p0;
    s32 w4;
    s32 w8;
    s32 wC;
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
    char padCA[0xDA - 0xCA];
    s16 sDA;
    s16 sDC;
    s16 sDE;
    s16 sE0;
    s16 sE2;
    s16 sE4;
    s16 sE6;
    char padE8[0xF8 - 0xE8];
    s16 sF8;
    s16 sFA;
    s16 sFC;
    char padFE[2];
    s32 w100;
    s32 w104;
    s32 w108;
    s32 w10C;
    T16 t110[4];
    s16 s150[4];
    s16 s158;
    s16 s15A;
    s16 s15C;
    s16 s15E;
    s16 s160;
    s16 s162;
    s16 s164;
    s16 s166;
    s16 s168;
    s16 s16A;
    s16 s16C;
    s16 s16E;
    s16 s170;
    s16 s172;
    s16 s174;
    s16 s176;
    char pad178[8];
    Z52 z180[4];
    s16 s250[2][4];
} SP;

extern void func_80027998(void);
extern s32 func_80090424(s16);
extern s32 func_800904F4(s16);
extern Node *D_800ABEA4;
extern s32 D_800ABECC;

void func_80027C54(Car *arg0) {
    SP *g = (SP *)0x1F800080;
    register s32 t0 asm("$8"); /* ASM_BARRIER: pin inner loop counter to $t0 */
    register s32 *q asm("$19"); /* ASM_BARRIER: pin 0x1F80007C pointer to $s3 */

    arg0->u1F0.w1F0 = 0;
    arg0->u20.s.w20 = arg0->w10 >> 2;
    arg0->u20.s.w28 = arg0->w18 >> 2;
    *(struct Q4 *)0x1F800190 = arg0->q88;
    *(struct Q4 *)0x1F8001A0 = arg0->q98;
    *(struct Q4 *)0x1F8001B0 = arg0->qA8;
    *(struct Q4 *)0x1F8001C0 = arg0->qB8;
    g->sC0 = 0;
    g->sBA = 0;
    g->sC8 = 0;
    g->sC6 = 0;
    g->sC4 = 0;
    g->sE6 = 0;
    g->sE4 = 0;
    g->sE2 = 0;
    g->sDE = 0;
    g->sDC = 0;
    g->sDA = 0;
    *(struct Q4 *)0x1F800180 = arg0->u20.q20;
    q = (s32 *)0x1F80007C;
    {
        s32 a0 = arg0->wF0;
        s32 v0 = a0 - 2;
        s32 v1 = (v0 + D_800ABECC) % D_800ABECC;
        g->w14 = a0;
        g->w30 = arg0->s10C;
        g->w8 = 0;
        g->w20 = v1;
    }
    do {
        g->t110[g->w8].a = g->w100 + (g->t110[g->w8].a >> 2);
        g->t110[g->w8].c = g->w108 + (g->t110[g->w8].c >> 2);
        g->t110[g->w8 + 1].a = g->w100 + (g->t110[g->w8 + 1].a >> 2);
        g->t110[g->w8 + 1].c = g->w108 + (g->t110[g->w8 + 1].c >> 2);
        g->s150[g->w8] = arg0->sF8[g->w8];
        func_80027998();
        g->z180[g->w8] = D_800ABEA4[g->s150[g->w8]].z8;
        g->z180[g->w8 + 1] = D_800ABEA4[(g->s150[g->w8] + 1) % D_800ABECC].z8;
        arg0->sF8[g->w8 + 1] = arg0->sF8[g->w8] = g->s150[g->w8];
        g->w8 += 2;
    } while (g->w8 < 3);
    g->w8 = 0;
    do {
        s32 a0 = g->w8;
        g->wC = a0;
        if (g->s150[0] != g->s150[2] || g->w8 < 2) {
            g->s174 = g->z180[a0].s1A;
            g->s176 = g->z180[g->wC].s18;
            g->s24 = func_80090424(g->s174);
            g->s26 = func_800904F4(g->s174);
            g->s28 = func_80090424(g->s176);
            g->s2A = func_800904F4(g->s176);
            if (g->s174 != g->s176) {
                g->w38 = 1;
                g->s174 = g->s162 = g->z180[g->wC].s14;
                g->s160 = g->z180[g->wC].sA + 0x400;
            } else {
                g->w38 = 0;
                g->s174 = g->s162 = (g->z180[g->wC].s14 * g->s26) >> 12;
                g->s160 = g->z180[g->wC].s16 + 0x400;
                g->s176 = (g->s24 * g->z180[g->wC].s14) >> 12;
            }
            g->sB8 = g->sC2 = func_800904F4(g->s160);
            {
                s16 s = func_80090424(g->s160);
                g->sBE = -s;
                g->sBC = s;
            }
            RR_GTE_SETROT(&g->sB8);
            g->s16C = g->z180[g->wC + 1].s8 - g->z180[g->wC].s8;
            g->s168 = g->z180[g->wC + 1].sE - g->z180[g->wC].sE;
            g->s164 = g->z180[g->wC + 1].s12 - g->z180[g->wC].s12;
            g->s166 = g->z180[g->wC + 1].s10 - g->z180[g->wC].s10;
        }
        for (t0 = 0; t0 < 2; t0++) {
            g->sF8 = g->t110[g->w8 + t0].a - g->z180[g->wC].w0;
            g->sFC = g->t110[g->w8 + t0].c - g->z180[g->wC].w4;
            RR_GTE_MVMVA_ROT_V0_NOCV(&g->sF8);
            g->w34 = 0;
            g->s16E = 0;
            g->w3C = g->s174;
            g->w40 = g->s176;
            RR_GTE_STORE_IR(&g->w14);
            __asm__ volatile("" : : : "memory"); /* ASM_BARRIER: RR_GTE_STORE_IR writes g->w14..w1C in memory */
            if (g->w38 == 1) {
                g->w40 = (g->w18 * g->s24) / g->s26;
                g->w14 -= g->w40;
                g->w3C -= g->w40;
                g->w3C += (g->w18 * g->s28) / g->s2A;
                g->w14 = (g->w14 * g->s162) / g->w3C;
            } else {
                g->w18 -= (g->w40 * g->w14) / g->s162;
            }
            g->w10 = (g->w14 << 10) / g->s162;
            g->s158 = ((g->s168 * g->w10) >> 10) + g->z180[g->wC].sE;
            g->s15A = ((g->s164 * g->w10) >> 10) + g->z180[g->wC].s12;
            g->s15C = ((g->s166 * g->w10) >> 10) + g->z180[g->wC].s10;
            if (g->w18 > g->s15A) {
                g->s16A = g->w18 - g->s15A;
                g->w34 = 1;
                g->w18 = g->s15A;
            } else if (g->w18 < -g->s15C) {
                g->s16A = -(g->w18 + g->s15C);
                g->w34 = 2;
                g->w18 = -g->s15C;
            }
            if (g->w34 > 0) {
                g->w34--;
                g->s164 = g->z180[g->wC + 1].s2C[g->w34] - g->z180[g->wC].s2C[g->w34];
                g->s164 = ((g->s164 * g->w10) >> 10) + g->z180[g->wC].s2C[g->w34];
                g->s170 = g->z180[g->wC + 1].s30[g->w34] - g->z180[g->wC].s30[g->w34];
                g->s170 = ((g->s170 * g->w10) >> 10) + g->z180[g->wC].s30[g->w34];
                if (g->s16A < g->s164) {
                    g->s172 = g->s16A;
                    g->s16A = 0;
                } else {
                    g->s16A -= (g->s172 = g->s164);
                    g->s16E = g->z180[g->wC + 1].s28[g->w34] - g->z180[g->wC].s28[g->w34];
                    g->s16E = ((g->s16E * g->w10) >> 10) + g->z180[g->wC].s28[g->w34];
                    {
                        s32 v = (g->w34 != 0) ? -(((g->s16E * g->s16A) << 11) >> 16) : (((g->s16E * g->s16A) << 11) >> 16);
                        g->s16E = v;
                    }
                    arg0->u1F0.b1F0[g->w8 + t0] = g->z180[g->wC].a20[g->w34].b0;
                }
                {
                    s32 v = (g->w34 != 0) ? -(((g->s170 * g->s172) << 11) >> 16) : (((g->s170 * g->s172) << 11) >> 16);
                    g->s16E += (g->s170 = v);
                }
            }
            g->s250[*q][g->w8 + t0] = g->s16E + ((((g->s16C * g->w10) >> 10) + g->z180[g->wC].s8) + ((g->s158 * g->w18) >> 5));
        }
        g->w8 += 2;
    } while (g->w8 < 4);
}
