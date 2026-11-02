typedef struct {
    short s0, s2, s4, s6, s8, sA, sC, sE, s10, s12, s14, s16, s18, s1A, s1C, s1E;
} T;

typedef struct {
    char p0[0x1C];
    T *p1C;
    char p20[4];
    int i24;
    char p28[4];
    int i2C;
    char p30[0xC];
    short s3C;
    char p3E[0xA];
    short s48;
    short s4A;
    char p4C[4];
    int i50;
    int i54;
    int i58;
    char p5C[2];
    short s5E;
    int i60;
    int i64;
    char p68[0x2C];
    int i94;
    char p98[4];
    int i9C;
    char pA0[4];
    int iA4;
    short sA8;
    short sAA;
    short sAC;
} D;

typedef struct {
    char p0[0x30];
    int i30;
    char p34[4];
    int i38;
    char p3C[0x18];
    int i54;
    char p58[0x94];
    int iEC;
    char pF0[4];
    int iF4;
    char pF8[0x7A];
    short s172;
    char p174[0x44];
    short s1B8;
    char p1BA[0x1A];
    short s1D4;
    char p1D6[2];
    short s1D8;
    char p1DA[2];
    short s1DC;
    short s1DE;
    char p1E0[0xA];
    short s1EA;
    char p1EC[8];
    D d;
} C;

typedef struct {
    char p0[0xA];
    unsigned char bA;
} E;

extern int func_800247FC();
extern int func_80024BCC();
extern int func_80025F84();
extern int func_80025FBC();
extern int func_80050E94();
extern int func_80090424();
extern int func_800904F4();
extern char D_800ABEF8[];
extern E *D_800F3A08[];

void func_80024CD8(C *c) {
    D *d;
    T *t;
    int s2;
    int s4;
    int s1;
    /* ASM_BARRIER: pin a0 to $4 for abs(s2) and c->s1DE shift register allocation */
    register int a0 __asm__("$4");
    int a2;
    int a3;
    /* ASM_BARRIER: pin v0 to $2 to match original expression temp register */
    register int v0 __asm__("$2");
    int v1;
    short x;
    int pad[4];

    d = &c->d;
    t = d->p1C;
    s2 = func_80025FBC(c->i54, d->i9C);
    if (s2 < d->s48 && -d->s48 < s2) {
        c->i54 = c->i54 + (s2 >> 1);
    } else {
        c->i54 = c->i54 + s2 / d->s48;
    }
    if (d->s3C > 0) {
        d->s3C = d->s3C - 1;
    } else {
        d->s3C = 0;
    }
    func_800247FC(c);
    c->i30 = (c->s1D8 * func_80090424(c->s1D4)) >> 8;
    v0 = func_800904F4(c->s1D4);
    v1 = c->s1D8;
    c->i38 = (v1 * v0) >> 8;
    if (t->sA == 0) {
        if (d->sA8 == 1) {
            v0 = v1;
            a0 = s2;
            if (s2 < 0) {
                a0 = -a0;
            }
            d->i58 = (v0 * a0 * d->i54) / 64;
            d->i54 = 0;
            if (c->s1D8 >= 0x3C1 && d->i58 > d->i94) {
                d->i2C = 1;
                d->s5E = 0x200;
                c->iEC = c->iF4;
                d->iA4 = 2;
                v1 = 1000 - (t->s0 - 1000) * 8;
                if (v1 < 1000) {
                    v1 = 1000;
                }
                v0 = ((s2 * v1) / 1000) << 6;
                /* ASM_BARRIER: prevent folding (v0 << 6) << 1 into << 7 */
                __asm__("" : "=r"(v0) : "0"(v0));
                d->i60 = v0;
                v1 = c->s1EA;
                d->i64 = v0 << 1;
                d->i50 = v1;
                v0 = c->s1EA;
                v1 = c->s172;
                if (v0 != 0) {
                    v0 = s2 * -v1;
                } else {
                    v0 = s2 * v1;
                }
                if (v0 < 0) {
                    v0 += 0xF;
                }
                d->s4A = (unsigned int)v0 >> 4;
            }
        } else if (d->sAC < 0x80) {
            a2 = d->sAA;
            if (a2 == 1) {
                a0 = __builtin_abs(s2);
                d->i58 = (c->s1D8 * a0) / 5;
                d->i54 = 0;
                if (c->s1D8 >= 0x3C1 && d->i58 > d->i94) {
                    d->i2C = a2;
                    d->s5E = 0x200;
                    c->iEC = c->iF4;
                    d->iA4 = 2;
                    v1 = 1000 - (d->p1C->s0 - 1000) * 8;
                    if (v1 < 1000) {
                        v1 = 1000;
                    }
                    v0 = ((s2 * v1) / 1000) << 6;
                    /* ASM_BARRIER: prevent folding (v0 << 6) << 1 into << 7 */
                    __asm__("" : "=r"(v0) : "0"(v0));
                    d->i60 = v0;
                    v1 = c->s1EA;
                    d->i64 = v0 << 1;
                    d->i50 = v1;
                    v0 = c->s1EA;
                    v1 = c->s172;
                    if (v0 != 0) {
                        v0 = s2 * -v1;
                    } else {
                        v0 = s2 * v1;
                    }
                    if (v0 < 0) {
                        v0 += 0xF;
                    }
                    d->s4A = (unsigned int)v0 >> 4;
                }
            } else {
                if (d->i54 < 0x3C && d->s3C == 0) {
                    d->i54 = d->i54 + 1;
                }
                d->i58 = 0;
            }
        }
    } else {
        if (c->s1B8 == 0) {
            v0 = *(unsigned short *)&c->s1DE;
            a0 = v0 << 16;
            v1 = a0 >> 16;
            if (v1 >= 0) {
                a0 = a0 >> 19;
                /* ASM_BARRIER: keep a0 = a0 >> 19 before c->s1DC load so reorg fills bltz slot */
                __asm__ volatile("");
                v0 = c->s1DC;
                if (v0 < 0) {
                    v0 = -v0;
                }
                v0 = v0 >> 2;
            } else {
                v1 = -v1;
                v0 = c->s1DC;
                a0 = v1 >> 1;
                /* ASM_BARRIER: stop reorg moving sra a0,v1,1 into bgez delay slot */
                __asm__ volatile("");
                if (v0 < 0) {
                    v0 = -v0;
                }
                v0 = v0 + 2;
            }
            a0 = a0 * v0;
            if (a0 < 0x80) {
                a2 = a0;
            } else {
                a2 = 0x7F;
            }
            if (c == (C *)D_800ABEF8 || c == (C *)(D_800ABEF8 + 0x320)) {
                a3 = d->i24;
                if (D_800F3A08[a3]->bA == 1) {
                    a2 = 0;
                }
                func_80050E94(1, 0x1418, a2, a3 & 1);
            }
        } else {
            if (c == (C *)D_800ABEF8 || c == (C *)(D_800ABEF8 + 0x320)) {
                func_80050E94(0, 0, 0, d->i24 & 1);
            }
        }
        s4 = func_80024BCC(c);
        if (c->s1DE > 0) {
            s1 = 0x190 - c->s1DE;
        } else {
            s1 = 0x21C;
        }
        s4 = (func_80025FBC(c->s1D4, s4) * t->s1A) / s1;
        /* ASM_BARRIER: keep c->s172 and t->sA loads after the division */
        __asm__ volatile("");
        v0 = c->s172;
        v1 = t->sA;
        if (v0 < 0) {
            v0 = -v0;
        }
        s1 = (v0 * v1) >> 7;
        if (s1 > 0x400) {
            s1 = 0x400;
        }
        s2 = func_80025F84(c->s1D4, c->i54);
        if (s2 > 0x400) {
            s2 = 0x400;
        }
        if (c->s1DE > 0) {
            d->i54 = d->i54 + (s2 >> 3);
        } else {
            d->i54 = (d->i54 * 2) / 3;
        }
        c->s1D4 = c->s1D4 + (((s4 * s2) / 1024) * s1 >> 10);
    }
    c->i54 = c->i54 & 0xFFF;
}
