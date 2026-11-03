/* RODATA 0x80010448 0x18 */
#include "types.h"
#include "m2c_macros.h"
#include "rr/geometry.h"
#include "rr/gpu.h"
#include "rr/race.h"
#include "game/sound/sndvab.h"
#include "game/ui/window1.h"

typedef struct { unsigned char c[4]; } B4;

typedef struct { unsigned char a, b, c, d; } S;

typedef struct {
    unsigned char u, v, w, h;
} __attribute__((packed)) R4;

typedef struct {
    unsigned char a, b, c, d;
} S_ACC68;

typedef struct {
    short s0;
    short s2;
    unsigned char b4;
    unsigned char b5;
    unsigned char b6;
    unsigned char b7;
    int w8;
    unsigned char bC;
    unsigned char bD;
    short sE;
    short s10;
    short s12;
    short s14;
    short s16;
    short s18;
    short s1A;
    short s1C;
    short s1E;
} S_F4CB0;

typedef struct {
    char pad[0x10];
    int w10;
    char pad14[0x48];
} S_F3A38;

typedef struct { u8 c[8]; } B8;

typedef struct { char p[0xA]; unsigned short b; char q[0x5C - 0xC]; } E;

typedef struct {
    char pad[0x2A8];
    short s2A8;
    char pad2[0x76];
} S_800ABEF8;

extern S D_800ACC68;
extern B4 D_8009EFD0[];
extern char *D_800ACC14;
extern short D_800F4D2C;
extern int func_8001FDD4();
extern int func_8005B2D4();
extern int func_8005B304();
extern int func_80092CB8();
extern int func_80096434();
extern unsigned short D_80010440[];
extern R4 D_8009EE70[];
extern R4 D_8009EEC8;
extern signed char D_8009EF3C[][4];
extern short D_8009EFD8[];
extern short D_8009EFE8[];
extern R4 D_8009EFF8;
extern int D_800AC5D4;
extern S_F3A38 D_800F3A38[];
extern void *func_8001FC28();
extern void *func_8001FCC8();
extern void *func_8001FD20();
extern void *func_8001FF84();
extern void *func_80020074();
extern void *func_80020508();
extern int func_80090424();
extern struct { char pad[0xC]; unsigned char a; unsigned char b; short c; } D_800F4CB0;
extern s8 D_8009EFFC[];
extern char D_80010460[];
extern s32 func_8002071C();
extern s32 D_8009FDA4;
extern int func_8004F24C();
extern char D_8009F008[];
extern int D_800F3A30;
extern int D_800F4C74;
extern int D_800F4C80[];
extern int func_80095A44();
extern char D_8009F014[];
extern unsigned short D_8009F01C[];
extern char D_8009F020[];
extern int D_800AD504;
extern int func_80025F84();
extern int func_800904F4();

void func_8002A344();
void func_8002A604(void);
void func_8002A63C(int arg0);
void func_8002AF8C();
void func_8002AFA4();
void func_8002B12C();
void func_8002B330();
int func_8002B574(int a, int b);
void func_8002B5D0();
void func_8002B75C(char *d, char *s);
void func_8002B7B4();
void func_8002B814();
void func_8002B934(char *a, int b);
void func_8002BA28(int a0);
char *func_8002BB88();
void func_8002BC7C(int a);
void func_8002BD18(int a, int b);
void func_8002BDB4(int t);
void func_8002BEF8(void *arg0);
void func_8002BF34();
void func_8002C11C(void *arg0, s16 arg1, s16 arg2);

/* legacy typing of shared globals in this part of the unit */
#define D_800F4CB0 (*(short (*))&D_800F4CB0)

void func_8002A344(a0) int a0; {
    B4 sp;
    int a, t, a1;
    S *st = &D_800ACC68;
    *(D_800ACC14 + 0x18) = 0;
    a = (a0 >> 1) & 1;
    st->c = a;
    if (a != st->d) D_800ACC68.a = 0;
    st->d = a;
    t = st->b;
    switch (D_800ACC68.a) {
    case 0:
        sp = D_8009EFD0[a ^ 1];
        st->b = 0;
        D_800ACC68.a = 1;
        break;
    case 1:
        {
            B4 *p = &D_8009EFD0[st->c];
            B4 *q = &D_8009EFD0[st->c ^ 1];
            a1 = 0x1E - t;
            sp.c[0] = (p->c[0] * t + q->c[0] * a1) / 30;
            sp.c[1] = (p->c[1] * t + q->c[1] * a1) / 30;
            sp.c[2] = (p->c[2] * t + q->c[2] * a1) / 30;
        }
        if (t >= 0x1E) {
            D_800ACC68.a = 2;
            st->b = 0;
        } else {
            if (t == 2) {
                if (D_800F4CB0 == 0) func_80092CB8(1);
            }
            D_800ACC68.b = D_800ACC68.b + 1;
        }
        break;
    case 2:
        sp = D_8009EFD0[st->c];
        break;
    }
    {
        int t = *(int *)0x1F800000;
        *(int *)0x1F800000 = func_8001FDD4(D_800ACC14 + 0xB6C, t, 0, 0, 0x140, 0x200, sp.c[0], sp.c[1], sp.c[2]);
    }
    func_8002A63C(a0 >> 2);
    if (D_800F4D2C != 6 && D_800F4D2C != 8) {
        func_8005B2D4(a0 & 1);
        func_8005B304();
    }
}

#undef D_800F4CB0

/* legacy typing of shared globals in this part of the unit */
#define D_800ACC68 (*(M2C_UNK (*))&D_800ACC68)
#define D_800F4CB0 (*(M2C_UNK (*))&D_800F4CB0)

void func_8002A604(void) {
    func_80096434(&D_800F4CB0, 0x20);
    func_80096434(&D_800ACC68, 0xC);
}

#undef D_800ACC68
#undef D_800F4CB0

/* legacy typing of shared globals in this part of the unit */
#define D_800ACC68 (*(S_ACC68 (*))&D_800ACC68)
#define D_800F4CB0 (*(S_F4CB0 (*))&D_800F4CB0)

void func_8002A63C(int arg0) {
    R4 sp30;
    int s7;
    int y;
    int sp38;
    int s0;
    int s6;
    int v;
    int s1, j, m, k;
    int s4;
    int s5;
    unsigned short sp40;
    void *a1;
    int c;
    R4 *r;
    int s2;
    unsigned short *pc;
    int col1, col2, col3;

    s7 = 0x80;
    if (D_800F4CB0.b6 != arg0) {
        D_800F4CB0.b7 = D_800F4CB0.b6;
    }
    y = 0x1C0;
    D_800F4CB0.b6 = arg0;
    s0 = D_800F4CB0.s2;
    s6 = D_800F4CB0.b6;
    sp38 = D_800ACC68.c;

    switch (D_800F4CB0.s0) {
    case 0:
        D_800F4CB0.s12 = 0x38;
        D_800F4CB0.s1A = 0x1B8;
        D_800F4CB0.s16 = D_800F4CB0.s1E = 2;
        if (D_800ACC68.a == 2) {
            D_800F4CB0.s0 = 1;
            D_800F4CB0.b4 = D_800ACC68.c;
        }
        break;
    case 1:
        D_800F4CB0.s14 = D_800F4CB0.s1C = (func_80090424((s0 << 10) / 30) * 0x140) / 0x1000;
        if (s0 >= 30) {
            D_800F4CB0.s0 = 2;
            D_800F4CB0.s2 = 0;
        } else {
            D_800F4CB0.s2++;
        }
        break;
    case 2:
        v = (func_80090424((s0 << 10) / 20) * 0x20) / 0x1000;
        D_800F4CB0.s16 = D_800F4CB0.s1E = v * 2 + 2;
        D_800F4CB0.s12 = 0x38 - v;
        D_800F4CB0.s1A = 0x1B8 - v;
        if (s0 >= 20) {
            D_800F4CB0.s0 = 3;
            D_800F4CB0.s2 = 0;
        } else {
            D_800F4CB0.s2++;
        }
    case 3:
        if (D_800ACC68.a == 1) {
            D_800F4CB0.s0 = 4;
            D_800F4CB0.s2 = 0;
        }
        if (D_800ACC68.a != 2) {
            s6 = D_800F4CB0.b7;
            sp38 = D_800ACC68.c ^ 1;
        }
        break;
    case 4:
        if (s0 >= 20) {
            if (D_800ACC68.a == 2) {
                D_800F4CB0.s0 = 5;
                D_800F4CB0.s2 = 0;
                D_800F4CB0.b4 = D_800ACC68.c;
            }
        } else {
            D_800F4CB0.s2++;
        }
        s7 = ((20 - s0) * 0x80) / 20;
        sp38 = D_800ACC68.c ^ 1;
        s6 = D_800F4CB0.b7;
        break;
    case 5:
        if (s0 >= 20) {
            D_800F4CB0.s0 = 3;
            D_800F4CB0.s2 = 0;
        } else {
            D_800F4CB0.s2++;
        }
        s7 = (s0 * 0x80) / 20;
        break;
    }

    pc = D_80010440;
    sp40 = (((sp38 + 1) & 3) << 5) | 0x1E;
    a1 = func_8001FC28(D_800ACC14 + 0x70, *(void **)0x1F800000, 0, 0, 0x140, 0x200);
    a1 = func_80020074(D_800ACC14 + 0x70, a1, 0x10, 0x24, 0x20, 0x10, 0, 0x20, sp40, 0x7F38, 1, (unsigned short)s7);
    s1 = 0;
    s4 = 0x30;
    s5 = 0x24;
    s7++;

    c = D_8009EF3C[s6][0];
    while (c != -1) {
        r = &D_8009EE70[c];
        a1 = func_8001FF84(D_800ACC14 + 0x70, a1, s4, s5, r, 0x7F38, ((unsigned short)s7 << 1) | 1);
        c = D_8009EF3C[s6][++s1];
        { int t = s4 + 2; s4 = t + r->w; }
    }

    if (D_800F4CB0.w8 > 0) {
        a1 = func_8001FF84(D_800ACC14 + 0x70, a1, s4, s5, &D_8009EEC8, 0x7F38, ((unsigned short)s7 << 1) | 1);
        D_8009EFF8.u = D_800F4CB0.w8 * 8;
        { int t = s4 + 2; s4 = t + D_8009EEC8.w; }
        a1 = func_8001FF84(D_800ACC14 + 0x70, a1, s4, s5, &D_8009EFF8, 0x7F38, 1, ((unsigned short)s7 << 1) | 1);
    }

    a1 = func_80020508(
        D_800ACC14 + 0x70, a1, 8, 0x38, 0x130, 2,
        col1 = *pc, col1, col1
    );
    a1 = func_8001FCC8(D_800ACC14 + 0x70, a1, sp40);
    a1 = func_8001FC28(D_800ACC14 + 0x70, a1, D_800F4CB0.s10, D_800F4CB0.s12, D_800F4CB0.s14, D_800F4CB0.s16);

    for (j = 2; j < 7; j++) {
        s4 = D_8009EFD8[j];
        s5 = (j >= 2) ? 0x1BE : 0x1AA;
        sp30 = D_8009EE70[j];
        if ((unsigned int)(j - 3) < 2) {
            s2 = (D_800F3A38[D_800AC5D4 & 1].w10 == 1);
            if (s2 == 1) {
                sp30.u += 0x14;
                if (j == 4) {
                    s4 -= 4;
                } else {
                    s4 -= 2;
                }
            }
        } else if (j == 5) {
            if (D_800F4CB0.bC != 1) {
                sp30.w -= 0x14;
            }
        } else if (j == 6) {
            if (D_800F4CB0.bD != 1) {
                sp30.w -= 0x1E;
            }
        }
        a1 = func_8001FF84(D_800ACC14 + 0x70, a1, s4, s5, &sp30, 0x7F38, ((unsigned short)s7 << 1) | 1);
    }

    if (D_800F4CB0.sE == 0) {
        s2 = 0x32;
    } else {
        s2 = 0x31;
    }
    if (D_800ACC68.a == 2) {
        j = sp38 + 0x7DBA;
        a1 = func_8001FD20(D_800ACC14 + 0x70, a1, D_8009EFE8[7], y, 1, 0xC, 0xF0, 0x7A, 0x14);
        a1 = func_8001FF84(D_800ACC14 + 0x70, a1, D_8009EFE8[7] + 8, s5, &D_8009EE70[s2], j, 0);
    }

    col2 = *pc;
    a1 = func_8001FCC8(D_800ACC14 + 0x70, a1, sp40);
    {
        char *p;
        int w, w2, w3, d;
        for (j = 3; j < 7; j++) {
            a1 = func_80020508(D_800ACC14 + 0x70, a1, D_8009EFE8[j], (j >= 3) ? y : 0x1A8, 1, 0xC, col2, col2, col2);
            /* ASM_BARRIER: define p after last loop call so p is live-in to next block without crossing calls */
            __asm__("" : "=r"(p));
        }
        w = 0x130;
        /* ASM_BARRIER: prevent constant folding of 0x130 into stack store */
        __asm__("" : "+r"(w));
        /* ASM_BARRIER: create anti-dependency from p load to w2 birthing insn in sched1 */
        __asm__("" : "=r"(w2) : "0"(w), "r"(p));
        /* ASM_BARRIER: chain birthing insn w3 so 16(sp) store stall cycle is filled before a3=0x1B8 */
        __asm__("" : "=r"(w3) : "0"(w2));
        a1 = func_80020508(
            (p = D_800ACC14) + 0x70, a1, 8, 0x1B8,
            (col3 = *pc, w3), 2,
            col3, col3, ({ /* ASM_BARRIER: keep w3 live past 16(sp) store so combine preserves w3 birthing insn */ __asm__("" : "=r"(d) : "r"(w3), "m"(*(char *)col3)); col3; })
        );
        if (D_800F4CB0.bD != 1) {
            a1 = func_80020508(D_800ACC14 + 0x70, a1, 0xEE, 0x1C6, 0x16, 2, col3, col3, col3);
        }
        if (D_800F4CB0.bC != 1) {
            a1 = func_80020508(D_800ACC14 + 0x70, a1, 0xA8, 0x1C6, 0x12, 2, col3, col3, col3);
            /* ASM_BARRIER: make REG_N_SETS > 1 to prevent birthing_insn_p boost in sched1 */
            __asm__ volatile("" : "+r"(col3), "=r"(p) : "m"(D_800ACC14), "m"(D_800ACC14), "m"(D_800ACC14), "m"(D_800ACC14), "m"(D_800F4CB0.bC), "m"(D_800F4CB0.bC));
        }
        /* ASM_BARRIER: extend live range of d so global-alloc assigns %hi(D_800ACC14) to $s5 */
        __asm__ volatile("" : : "r"(d));
        a1 = func_8001FC28(D_800ACC14 + 0x70, a1, D_800F4CB0.s18, D_800F4CB0.s1A, D_800F4CB0.s1C, D_800F4CB0.s1E);
        *(void **)0x1F800000 = func_8001FCC8(D_800ACC14 + 0x70, a1, sp40);
    }
}

#undef D_800ACC68
#undef D_800F4CB0

void func_8002AF8C(a, b, c) short a; int b; int c; { D_800F4CB0.c = a; D_800F4CB0.a = b; D_800F4CB0.b = c; }

/* legacy typing of shared globals in this part of the unit */
#define D_800ACC14 (*(s32 (*))&D_800ACC14)

void func_8002AFA4(a0, a1, a2, a3, a4, a5) s32 a0; s32 a1; s32 a2; s32 a3; s32 a4; s32 a5; {
    s32 i, j, k, t0, sel, sg, off;
    register s32 s1 __asm__("$17"); /* ASM_BARRIER: pin i+1 to s1 so loop var i gets s2 (saved reg order) */
    s8 *tb;
    RrVec2s *p, *bp;
    char *ub;
    struct { RrVec2s v[3]; s32 pad; B8 u; } w;
    i = 0;
    bp = w.v;
    tb = D_8009EFFC;
    t0 = *(s32 *)0x1F800000;
    w.u = *(B8 *)D_80010460;
L0:
    j = 0;
L1:
    k = 0;
    sg = 1 - (i & 1) * 2;
    p = bp;
    off = j << 1;
    do {
        p->x = (*(s8 *)(off + (s32)tb) - a2) * sg + a0;
        p->y = *(s8 *)(off + (s32)tb + 1) + a1;
        off += 4;
        k++;
        p++;
    } while (k < 3);
    s1 = i + 1;
    sel = 0;
    if (a5 == s1) sel = 0 < j;
    j++;
    sg = D_800ACC14 + 0x70;
    ub = (char *)&w.u;
    t0 = func_8002071C(sg, t0, ((s32 *)w.v)[0], ((s32 *)w.v)[1], ((s32 *)w.v)[2], ub + sel * 4, 0);
    if (j < 2) goto L1;
    i = s1;
    if (i < 2) goto L0;
    *(s32 *)0x1F800000 = t0;
}

#undef D_800ACC14

/* legacy typing of shared globals in this part of the unit */
#define D_800ACC14 (*(s32 (*))&D_800ACC14)
#define func_8001FD20 ((s32 (*)())func_8001FD20)

void func_8002B12C(p0, p1, p2, p3, p4, a5, a6) s32 p0; s32 p1; s32 p2; s32 p3; s32 p4; s32 a5; s32 a6; {
    s32 i, k, t0, g, sel, sg;
    register s32 one __asm__("$18"); /* ASM_BARRIER: pin s-reg order (saved reg priority) */
    s32 a3; 
    register s32 a4 __asm__("$20"); /* ASM_BARRIER: pin s-reg order (saved reg priority) */
    s32 a0, a1;
    s32 a2; 
    RrVec2s *p, *bp;
    char *ub;
    s8 *tb, *nv;
    struct { RrVec2s v[3]; s32 pad; B8 u; } w;
    a0 = p0; a1 = p1; a2 = p2; a3 = p3;
    i = 0;
    bp = w.v;
    one = 1;
    a4 = p4;
    t0 = *(s32 *)0x1F800000;
    g = D_8009FDA4;
    nv = D_8009EFFC;
    w.u = *(B8 *)D_80010460;
L0:
    k = 0;
    sg = one - (i & 1) * 2;
    p = bp;
    tb = nv;
    do {
        p->x = (tb[0] - a2) * sg + a0;
        p->y = tb[1] + a1;
        tb += 4;
        k++;
        p++;
    } while (k < 3);
    if (a3 == one) g = 0;
    if (g == one) {
        sel = 1;
        if (i != 0) goto B8;
    } else {
B8:
        sel = 0;
        if (g == 2) sel = i == 1;
    }
    sel ^= a3;
    if (a4 == a5) {
        if (i != 0) goto B12;
    } else {
B12:
        if (a4 == a6 - 1) {
            if (i != one) goto B14;
        } else {
B14:
            ub = (char *)&w.u;
            t0 = func_8002071C(D_800ACC14 + 0x70, t0, ((s32 *)w.v)[0], ((s32 *)w.v)[1], ((s32 *)w.v)[2], ub + (sel << 2), 0);
        }
    }
    i++;
    if (i < 2) goto L0;
    *(s32 *)0x1F800000 = t0;
    if (a3 == 1) {
        *(s32 *)0x1F800000 = func_8001FD20(D_800ACC14 + 0x70, t0, a0, a1 - 0xC, a3, 0x18, 0xFF, 0xFF, 0xFF);
    }
}

#undef D_800ACC14
#undef func_8001FD20

/* legacy typing of shared globals in this part of the unit */
#define D_800F3A38 (*(E (*)[])&D_800F3A38)

void func_8002B330(a0, a1, a2, a3, a4) int a0; int a1; char *a2; int a3; int a4; {
    int s0;
    int a;
    unsigned short b;
    switch (a1) {
    case 2:
        s0 = *(short *)a2;
        a = (a0 >> 2) & 1;
        break;
    case 4:
        s0 = *(int *)a2;
        a = (a0 >> 2) & 1;
        break;
    case 1:
    case -1:
        s0 = *(unsigned char *)a2;
        a = (a0 >> 2) & 1;
        break;
    default:
        a = (a0 >> 2) & 1;
        break;
    }
    if ((a0 & 1) == 0) {
        b = D_800F3A38[a].b;
        if ((b & 0x8000) == 0) {
            if ((b & 0x2000) == 0) {
                if (b & 0x1000) {
                    s0 -= 1;
                    goto L;
                }
                if (b & 0x4000) {
                    s0 += 1;
                    goto L;
                }
            }
        }
    } else {
        b = D_800F3A38[a].b;
        if ((b & 0x1000) == 0) {
          if ((b & 0x4000) == 0) {
            if (b & 0x8000) {
                D_8009FDA4 = 1;
                s0 -= 1;
                goto L;
            }
            if (b & 0x2000) {
                D_8009FDA4 = 2;
                s0 += 1;
                goto L;
            }
          }
        }
        D_8009FDA4 = 0;
        goto M;
    }
    goto M;
L:
    if ((a0 & 2) == 0) {
        if (s0 < a3) {
            if (s0 >= a4) {
                func_8004F24C(0, a);
            } else {
                s0 = a4;
            }
        } else {
            s0 = a3 - 1;
        }
    } else {
        func_8004F24C(0, a);
        if (s0 >= a3) {
            s0 = a4;
        } else if (s0 < a4) {
            s0 = a3 - 1;
        }
    }
M:
    switch (a1) {
    case 2: *(short *)a2 = s0; break;
    case 4: *(int *)a2 = s0; break;
    case 1:
    case -1: *(char *)a2 = s0; break;
    }
}

#undef D_800F3A38

int func_8002B574(int a, int b)
{
    int q = a / 25;
    a -= q * 25;
    a = a * 40 + b;
    return q * 1000 + a;
}

void func_8002B5D0(a0, a1, b, a3, off)
int a0; int a1; int b; int a3; int off;
{
    if (b >= 0) {
        int m = b / 1000;
        int r = m % 60;
        int t;
        m = m / 60;
        t = b % 1000;
        D_8009F008[1] = m + 0x30;
        D_8009F008[6] = t / 100 + 0x30;
        D_8009F008[7] = t / 10 % 10 + 0x30;
        D_8009F008[8] = t % 10 + 0x30;
        D_8009F008[3] = r / 10 + 0x30;
        D_8009F008[4] = r % 10 + 0x30;
    } else {
        D_8009F008[1] = 0x3A;
        D_8009F008[3] = 0x3A;
        D_8009F008[4] = 0x3A;
        D_8009F008[6] = 0x3A;
        D_8009F008[7] = 0x3A;
        D_8009F008[8] = 0x3A;
    }
    func_80020CD4(a0, a1, D_8009F008 + off, a3);
}

void func_8002B75C(char *d, char *s)
{
    int i = 0;
    char *p;
    if (*s) {
        do {
            d[i] = *s++;
            i++;
        } while (*s);
    }
    while (*(p = d + i++)) {
        *p = 0;
    }
}

void func_8002B7B4(d, s, n) char *d; unsigned char *s; int n; {
    int i = 0;
    if (*s != 0) do { unsigned char c = *s; s++; d[i] = c; i++; } while (*s != 0);
    if (i < n) do { d[i] = 0x20; i++; } while (i < n);
    d[i] = 0;
}

void func_8002B814(a, b, c) char *a; int b; int c; {
    int t;
    a[c] = b % 10 + 0x30;
    t = b / 10;
    switch (c) {
    case 1:
        a[0] = t + 0x30;
        return;
    case 2: {
        int h = b / 100;
        a[0] = h + 0x30;
        a[1] = t - h * 10 + 0x30;
        return;
    }
    case 3: {
        int k = b / 1000;
        int h = b / 100;
        a[0] = k + 0x30;
        a[1] = h - k * 10 + 0x30;
        a[2] = t - h * 10 + 0x30;
        return;
    }
    }
}

void func_8002B934(char *a, int b)
{
  int s;
  int t;
  int m;
  int r;
  if (b >= 0)
  {
    if (s)
    {
s = b / 1000; m = s / 60; r = s % 60; t = b % 1000;
      func_8002B814(a, m, 1);
      func_8002B814(a + 3, r, 1);
      func_8002B814(a + 6, t, 2);
      return;
    }
    else
    {
s = b / 1000; m = s / 60; r = s % 60; t = b % 1000;
      func_8002B814(a, m, 1);
      func_8002B814(a + 3, r, 1);
      func_8002B814(a + 6, t, 2);
      return;
    }
  }
  a[8] = 0x2D; a[7] = 0x2D; a[6] = 0x2D; a[4] = 0x2D; a[3] = 0x2D; a[1] = 0x2D; a[0] = 0x2D;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800ABEF8 (*(S_800ABEF8 (*)[])&D_800ABEF8)

void func_8002BA28(int a0) {
    /* ASM_BARRIER: pin &D_800ABEF8[a0] base pointer to $a0 */
    register S_800ABEF8 *p0 asm("a0");
    /* ASM_BARRIER: reserve $v0 across D_800F3A30 %hi address load so %hi(D_800F3A30) uses $v1 */
    register int dummy asm("v0");
    /* ASM_BARRIER: pin D_800F3A30 value load to $v1 */
    register int n asm("v1");
    int s7;
    int i;
    int s3;
    int v;
    int y;
    RrSpritePacket *p;

    s7 = D_800ABEF8[a0].s2A8;
    p0 = &D_800ABEF8[a0];
    /* ASM_BARRIER: combine D_800ABEF8[a0] address into $a0 and start $v0 live range before D_800F3A30 %hi */
    __asm__("" : "+r"(p0), "=r"(dummy));
    v = D_800F3A30;
    /* ASM_BARRIER: order prologue stack stores before D_800F3A30 %lo load and p0->s2A8 load in sched2 */
    __asm__("" : "=m"(*p0), "=m"(D_800F3A30) : "r"(dummy));
    s7 = p0->s2A8;
    n = D_800F3A30;
    if (n > 0) {
        i = 0;
        do {
            if (i == s7) {
                /* ASM_BARRIER: prevent jump_optimize from merging identical then/else branches and give i higher alloc priority than GIV */
                __asm__("" : : "r"(i));
                s3 = 0x7FF8;
            } else {
                s3 = 0x7FF8;
            }
            v = D_800F4C80[i];
            y = i * 10 + 0x2C;
            func_8002B5D0(0xF0, y, v, s3, 0);
            p = *(RrSpritePacket **)0x1F800000;
            p->len = 4;
            p->code = 0x64;
            p->x = 0xE7;
            p->y = y;
            p->u = i * 12 + 12;
            p->v = 0xA8;
            p->clut = s3;
            p->code |= 1;
            p->w = 0xC;
            p->h = 0xC;
            func_80095A44(D_800ACC14 + 0x70, p);
            p++;
            *(RrSpritePacket **)0x1F800000 = p;
            i++;
        } while (i < D_800F3A30);
    }
    func_8002B5D0(0xF0, 0x1C, D_800F4C74, s3, 0);
}

#undef D_800ABEF8

char *func_8002BB88(a) int a; {
    if (a >= 0) {
        int s = a / 25;
        int m = s / 60;
        int r = s % 60;
        D_8009F014[0] = m / 10 + 0x30;
        D_8009F014[1] = m % 10 + 0x30;
        D_8009F014[3] = r / 10 + 0x30;
        D_8009F014[4] = r % 10 + 0x30;
    } else {
        D_8009F014[0] = 0x2D; D_8009F014[1] = 0x2D; D_8009F014[3] = 0x2D; D_8009F014[4] = 0x2D;
    }
    return D_8009F014;
}

void func_8002BC7C(int a)
{
    char *p = D_800ACC14 + 0x226EC;
    int r = func_8002BB88(a);
    int i = a < 0x5DC;
    int t = D_8009F01C[i];
    int s = (D_800F3A30 - 3) * 10 + 0x50;
    func_80020E2C(D_800ACC14 + 0x70, 0x111, s, r, t);
    *(short *)(p + 0xA) = s;
}

void func_8002BD18(int a, int b)
{
    char *p = D_800ACC14 + (b * 120 + 0x226EC);
    char *q;
    int r;
    int i;
    int t;
    if (b == 0) {
        q = D_800ACC14 + 0x70;
    } else {
        q = D_800ACC14 + 0xB70;
    }
    r = func_8002BB88(a);
    i = a < 0x5DC;
    t = D_8009F01C[i];
    func_80020E2C(q, 0x111, *(short *)(p + 0xA), r, t);
}

void func_8002BDB4(int t)
{
    int a, b, c;
    /* ASM_BARRIER: pins the D_8009F020 base to $v0 (original allocates it before the digit temporaries) */
    register char *p asm("$2");

    a = t / 1000;
    b = a / 60;
    c = a % 60;
    a = t % 1000;
    p = D_8009F020;
    p[0] = b / 10 + '0';
    p[1] = b % 10 + '0';
    p[3] = c / 10 + '0';
    p[4] = c % 10 + '0';
    p[6] = a / 100 + '0';
    p[7] = a / 10 % 10 + '0';
    p[8] = a % 10 + '0';
}

void func_8002BEF8(void *arg0) {
    M2C_FIELD(arg0, s16 *, 0x1A0) = 0;
    M2C_FIELD(arg0, s8 *, 0x1A4) = 0;
    M2C_FIELD(arg0, s8 *, 0x1A5) = 0;
    M2C_FIELD(arg0, s16 *, 0x1A2) = 0;
    M2C_FIELD(arg0, s16 *, 0x19C) = 0;
    M2C_FIELD(arg0, s16 *, 0x19E) = 0;
    M2C_FIELD(arg0, s16 *, 0x198) = 0;
    M2C_FIELD(arg0, s16 *, 0x19A) = 0;
    M2C_FIELD(arg0, s16 *, 0x1B4) = 0;
    M2C_FIELD(arg0, s16 *, 0x1B6) = 0;
    M2C_FIELD(arg0, s16 *, 0x1B0) = 0;
    M2C_FIELD(arg0, s16 *, 0x1B2) = 0;
    M2C_FIELD(arg0, s16 *, 0x1B8) = 0;
    M2C_FIELD(arg0, s16 *, 0x1BA) = 0;
}

void func_8002BF34(a0, a1) RrRaceVehicle *a0; int a1; {
    short tv, s1, v1, s0;
    int w, w2, z, sx, sy, sc;
    unsigned short su;
    a0->shakeActive19C = 1;
    tv = 0x800 - a0->trackAngle1EC;
    s1 = (tv + (a0->lateralVelF4 < 0 ? -0x400 : 0x400)) & 0xFFF;
    if (a1 < 3) {
        v1 = a0->input;
        if (v1 > 0x320) {
            su = 0x708;
            if (v1 < 0x709) su = (unsigned short)a0->input;
            w = (short)su * func_80090424((short)func_80025F84(tv, a0->heading1D4));
            s0 = w / 65536 + 4;
        } else {
            s0 = 0xC;
            if (v1 < 0xF0) s0 = 0x1E;
        }
        sx = (func_80090424(s1) * s0) >> 4;
        sy = (func_800904F4(s1) * s0) >> 4;
        sc = 0x64;
        a0->shakeTimer19E = 0x1E;
    } else {
        sx = (func_80090424(s1) * 8) >> 4;
        sy = (func_800904F4(s1) * 8) >> 4;
        a0->shakeTimer19E = 0xF;
        sc = 0x32;
    }
    a0->shakeX198 = sx;
    a0->shakeY19A = sy;
    if (a0->flags1E4 & 0x10) {
        w2 = func_800904F4(func_80025F84(tv, a0->heading1D4)) >> 6;
        if (a0->lateralVelF4 < 0) w2 = -w2;
        z = w2 + 0x40;
        if (a0 == &D_800ABEF8[0] || a0 == &D_800ABEF8[1]) {
            if (D_800AD504 != 2) func_8004F218(0x21, sc, (short)z, -1);
        }
    }
}

void func_8002C11C(void *arg0, s16 arg1, s16 arg2) {
    M2C_FIELD(arg0, s16 *, 0x19C) = 2;
    M2C_FIELD(arg0, s16 *, 0x198) = arg1;
    M2C_FIELD(arg0, s16 *, 0x19A) = arg2;
    M2C_FIELD(arg0, s16 *, 0x19E) = 0xF;
}
