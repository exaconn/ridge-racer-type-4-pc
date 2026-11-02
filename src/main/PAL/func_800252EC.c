typedef struct { short s0, s2, s4, s6, s8, sA, sC, sE, s10, s12, s14, s16, s18, s1A, s1C, s1E; } T;
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
} D;
typedef struct {
    char p0[0x30]; int i30; char p34[4]; int i38;
    char p3C[0x18]; int i54;
    char p58[0x11A]; short s172;
    char p174[0x44]; short s1B8;
    char p1BA[0x1A]; short s1D4; char p1D6[2]; unsigned short u1D8;
    char p1DA[0x1A]; D d;
} C;
typedef struct { char p0[0xA]; unsigned char bA; } E;

extern int func_80024BCC();
extern int func_80025F84();
extern int func_80025FBC();
extern int func_80050E94();
extern int func_80090424();
extern int func_800904F4();
extern char D_800ABEF8[];
extern E *D_800F3A08[];

void func_800252EC(C *c0) {
    register C *c __asm__("$18"); /* ASM_BARRIER: match saved reg assignment of original */
    D *d;
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
        if (c == (C *)D_800ABEF8 || c == (C *)(D_800ABEF8 + 0x320)) {
            a3 = d->i24;
            if (D_800F3A08[a3]->bA == 1) {
                a2 = 0;
            }
            func_80050E94(1, a1, a2, a3 & 1);
        }
    } else {
        if (c == (C *)D_800ABEF8 || c == (C *)(D_800ABEF8 + 0x320)) {
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
        if (c == (C *)D_800ABEF8 || c == (C *)(D_800ABEF8 + 0x320)) {
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
