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

typedef struct {
    char p0[0xC]; int iC;
    char p10[0x8]; char *p18; T *p1C;
    char p20[4]; int i24;
    char p28[0x14]; short s3C; char p3E[2]; short s40;
    char p42[8]; unsigned short u4A;
    char p4C[0xC]; int i58; short s5C; short s5E; int i60;
    char p64[0x22]; short s86; int i88;
    char p8C[0x10]; int i9C;
    char pA0[4]; int iA4; short sA8; char pAA[2]; short sAC;
} D_1;

typedef struct {
    char p0[0x30]; int i30; char p34[4]; int i38;
    char p3C[0x18]; int i54;
    char p58[0x11A]; short s172;
    char p174[0x44]; short s1B8;
    char p1BA[0x1A]; short s1D4; char p1D6[2]; unsigned short u1D8;
    char p1DA[0x1A]; D_1 d;
} C_1;

typedef struct { char pad[0x24]; int s24; char p1[0x74 - 0x28]; int s74; char p2[0x86 - 0x78]; short s86; char p3[0x9C - 0x88]; int s9C; int sA0; int sA4; char p4[0xAC - 0xA8]; short sAC; short sAE; } Q;

typedef struct {
    char p0[0x30]; int s30; char p1[4]; int s38; char p2[0x54 - 0x3C]; int s54;
    char p3[0xE4 - 0x58]; short sE4; short sE6; char p4[0x1D4 - 0xE8]; short s1D4; char p5[2]; unsigned short s1D8;
} P;

extern int func_800247FC();
extern int func_80024BCC();
extern int func_80025F84();
extern int func_80025FBC();
extern int func_80050E94();
extern int func_80090424();
extern int func_800904F4();
extern char D_800ABEF8[];
extern E *D_800F3A08[];
extern short D_800F4D44;
extern int D_800F2DE4;
extern int func_8004B7E0();
extern int func_80096464();

void func_80024CD8(C *c);
void func_800252EC(C_1 *c0);
void func_80025954();
int func_80025C94(int a0, int a1, int a2);

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

void func_800252EC(C_1 *c0) {
    register C_1 *c __asm__("$18"); /* ASM_BARRIER: match saved reg assignment of original */
    D_1 *d;
    short *s6;
    int v;
    int s3;
    int s4;
    int s5;
    int a1;
    int a2;
    int a3;
    int x;
    int s0i;
    int pad[4];
    register int y __asm__("$4"); /* ASM_BARRIER: match a0 temp of original */
    int w;
    int z;

    c = c0;
    d = &c->d;
    s6 = (short *)(d->p18 + (d->s86 * 0x3C + 4));
    v = func_80025FBC(c->i54, d->i9C);
    if (v > 0x400) {
        d->i9C = c->i54 + 0x400;
    } else if (v < -0x400) {
        d->i9C = c->i54 - 0x400;
    }
    if (c->s1B8 == 0) {
        if ((unsigned)(v + 0x13) < 0x27) {
            c->i54 = c->i54 + (v >> 1);
        } else {
            c->i54 = c->i54 + v / 20;
        }
    }
    s3 = func_80025FBC(c->s1D4, c->i54);
    if (s3 > 0x600) {
        s3 = 0x600;
        s4 = 1;
    } else if (s3 < -0x600) {
        s3 = -0x600;
        s4 = 1;
    } else {
        s4 = 0;
    }
    s4 |= c->s1B8 > 0;
    if (c->s1B8 == 0) {
        s0i = func_80025F84(c->i54, c->s1D4);
        s5 = s0i;
        if (s0i < 0x201) {
            a2 = s0i / 8 + 0x40;
        } else {
            a2 = 0x7F;
        }
        if (s0i < 0x201) {
            a1 = s0i * 3 + 0x1800;
        } else {
            a1 = 0x1E00;
        }
        if (c == (C_1 *)D_800ABEF8 || c == (C_1 *)(D_800ABEF8 + 0x320)) {
            a3 = d->i24;
            if (D_800F3A08[a3]->bA == 1) {
                a2 = 0;
            }
            func_80050E94(1, a1, a2, a3 & 1);
        }
    } else {
        if (c == (C_1 *)D_800ABEF8 || c == (C_1 *)(D_800ABEF8 + 0x320)) {
            func_80050E94(0, 0, 0, d->i24 & 1);
        }
        s5 = 0x200;
    }
    x = d->p1C->s10;
    if (s4 == 0) {
        if (s3 < x && -x < s3) {
            c->s1D4 = c->s1D4 + (s3 >> 1);
        } else {
            c->s1D4 = c->s1D4 + s3 / x;
        }
    }
    s3 = func_80024BCC(c);
    s0i = (short)((short)d->u4A >> 8);
    if (s0i < 0) {
        s0i = s0i >> 2;
    }
    w = c->s172;
    if (w < 0) {
        w = -w;
    }
    s0i = w - s0i;
    if (s0i > 0x400) {
        s0i = 0x400;
    } else if (s0i < 0) {
        s0i = 0;
    }
    x = func_80025FBC(c->s1D4, s3);
    x = ((x * d->p1C->s2) >> 8) * s0i;
    y = x / 1024;
    if (s4 == 0) {
        c->s1D4 = c->s1D4 + y;
    }
    c->u1D8 = c->u1D8 - s0i / 256;
    c->i30 = ((short)c->u1D8 * func_80090424(c->s1D4)) >> 8;
    c->i38 = ((short)c->u1D8 * func_800904F4(c->s1D4)) >> 8;
    c->i54 = c->i54 + d->i60 / (d->s5E + s5 / 2);
    if ((short)c->u1D8 < 0x348) {
        s0i = (0x348 - (short)c->u1D8) * 8;
    } else {
        s0i = 0;
    }
    z = 0x180;
    d->i58 = d->i58 - ((s5 >> 2) + (s0i + z));
    d->i60 = d->i60 + d->iC / (d->p1C->s1E + 8);
    if (d->iC != 0) {
        if (d->p1C->s4 < d->s5E) {
            d->s5E = d->s5E - 8;
        }
    }
    if (d->i60 > 0x9600) {
        d->i60 = 0x9600;
    } else if (d->i60 < -0x9600) {
        d->i60 = -0x9600;
    }
    if (d->sAC < 0x80) {
        d->i58 = d->i58 - 0x80;
        d->i60 = d->i60 * 0x1F / 32;
    } else if (d->sA8 == 1) {
        d->i60 = d->i60 + (d->i60 >> 9);
    }
    if (d->i58 < 0) {
        if (c == (C_1 *)D_800ABEF8 || c == (C_1 *)(D_800ABEF8 + 0x320)) {
            func_80050E94(0, 0, 0, d->i24 & 1);
        }
        d->i9C = c->i54;
        d->iA4 = 0;
        d->s3C = 0x1E;
        d->s40 = (d->i88 - (short)c->u1D8 * 0x2710 / (s6[1] * 0x490 / 160)) / 30;
    }
    c->i54 = c->i54 & 0xFFF;
    c->s1D4 = c->s1D4 & 0xFFF;
}

void func_80025954(a0) P *a0; {
    Q *q = (Q *)((char *)a0 + 0x1F4);
    int t, a1, a2, a3, v;
    int s0, s2;
    int pad[4];
    t = func_80025FBC(a0->s54, q->s9C);
    v = t / 4;
    a1 = a0->s54 + v;
    a0->s54 = a1;
    t = func_80025FBC(a0->s1D4, a1);
    a0->s1D4 = a0->s1D4 + t / 10;
    a0->s1D8 = (short)a0->s1D8 / 10;
    a0->s30 = ((short)a0->s1D8 * func_80090424(a0->s1D4)) >> 8;
    a0->s38 = ((short)a0->s1D8 * func_800904F4(a0->s1D4)) >> 8;
    a2 = q->s74;
    if (a2 >= 0x3D) a2 = 0x3C;
    else if (a2 < 0) a2 = 0;
    a2 = (a2 << 1) * q->sAC;
    if (a2 < 0) a2 += 0xFF;
    a2 = a2 >> 8;
    if (q->s74 > 0) {
        if ((char *)a0 == D_800ABEF8 || (char *)a0 == D_800ABEF8 + 0x320) {
            a3 = q->s24;
            if (*((unsigned char *)D_800F3A08[a3] + 0xA) == 1) a2 = 0;
            func_80050E94(1, 0x1A80, a2, a3 & 1);
            func_8004B7E0(q->s24 & 1, 2, 1);
        }
        q->s74 = q->s74 - 0x30;
        q->s74 = q->s74 - q->sAE * 2;
        if (q->sAC < 0x80) q->s74 = q->s74 - 0x30;
        if (D_800F4D44 != 4) {
            s2 = q->s74 >> 10;
            s0 = q->s9C;
            if ((func_80096464() & 3) >= 2) q->s9C = s0 + s2;
            else q->s9C = s0 - s2;
            s0 = a0->sE4;
            if (func_80096464() & 1) s0 += 0x10;
            a0->sE4 = s0;
            s0 = a0->sE6;
            if (func_80096464() & 1) s0 += 0x10;
            a0->sE6 = s0;
        } else {
            int c;
            int z, w, u;
            int x;
            int y;
            y = q->s74;
            x = y >> 10;
            c = (unsigned)(D_800F2DE4 & 3) < 2;
            y = q->s9C;
            if (c) y = y - x; else y = y + x;
            z = D_800F2DE4 & 1;
            q->s9C = y;
            u = a0->sE4;
            if (z) u += 0x10;
            w = a0->sE6;
            a0->sE4 = u;
            if (z) w += 0x10;
            a0->sE6 = w;
        }
        if (q->s86 >= 2) {
            a0->s1D8 = 0;
            q->sA0 = 0;
            q->s74 = 0;
        }
    } else {
        q->sA4 = 0;
        if ((char *)a0 == D_800ABEF8 || (char *)a0 == D_800ABEF8 + 0x320) {
            func_80050E94(0, 0, 0, q->s24 & 1);
            func_8004B7E0(q->s24 & 1, 0, 1);
        }
    }
}

int func_80025C94(int a0, int a1, int a2) {
    int d;
    a0 &= 0xFFF;
    a1 &= 0xFFF;
    d = 0x400 - a2;
    if (a1 < a0) {
        if (a0 - a1 > 0x800) a1 += 0x1000;
    } else {
        if (a1 - a0 > 0x800) a0 += 0x1000;
    }
    return ((a0 * d + a1 * a2) / 1024) & 0xFFF;
}
