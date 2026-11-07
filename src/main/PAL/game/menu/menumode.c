#include "game/data/cardchk.h"
#include "game/menu/loadmenu.h"
#include "game/menu/savemenu.h"
#include "game/data/cardini.h"
#include "game/menu/menumode.h"
#include "game/data/menuvars.h"
struct E { char pad[4]; unsigned char a; char b; unsigned char c; signed char d; };

extern int func_8002B330();
extern int func_8002B75C();
extern int PlaySfx();
extern short D_800FFC18;
extern short D_800FFC00;
extern short D_800AC594;
extern short D_800F3A1C;
extern short D_800F4D3C;
extern short D_800F3AF8;
extern char D_800F4D50[];
extern char D_800A45A0[];
extern char D_800A457C[];
extern struct { char a[4]; unsigned char b; char c[3]; } D_800A4258[];
extern unsigned char D_800A448C[][4];
extern struct { char pad[0x28]; int x28; int x2c; char pad2[0x2C]; } D_800F3A38[];
extern unsigned char D_800F3A24[];
extern char *D_800F4D80;
extern short D_80109478[];
extern short D_8010948E;
extern unsigned char D_800A44D8[][4];
extern unsigned char D_800ACBCC[];

void func_8007B774(void);
void func_8007B954();
void func_8007BB9C();

void func_8007B774(void) {
    /* ASM_BARRIER: pins value to s3 as in the original */
    register int s3 asm("$19");
    int i;
    i = D_800A4258[D_800F3A1C].b;
    s3 = D_800A448C[i][1];
    switch (D_800FFC18) {
    case 3:
    case 6:
    case 7:
        func_8002B330((D_800AC594 & 1) << 2, 2, &D_800F4D3C, 3, 0);
        if (D_800F3A38[D_800AC594].x28 != 0) {
            PlaySfx(1);
            if ((unsigned short)(D_800FFC18 - 6) < 2) {
                D_800F3AF8 = 0x11;
                func_8002B75C(D_800F4D50, D_800A4594[D_800F4D3C]);
            } else {
                func_8002B75C(D_800F4D50, D_800A457C);
            }
        } else {
            if (D_800F3A38[D_800AC594].x2c != 0) {
                D_800F3A1C = 0;
            }
            return;
        }
        break;
    default:
        if (D_800FFC18 == 0) {
            if (D_800F3A1C == 0x3A) {
                D_801094A0 = 1;
                D_800F3AF8 = 0x20;
                func_8002B75C(D_800F4D50, D_800A45A0);
            } else {
                func_8002B75C(D_800F4D50, D_800A457C);
            }
        } else {
            func_8002B75C(D_800F4D50, D_800A457C);
        }
        break;
    }
    D_800F3A1C = s3;
    func_80078E40(D_800FFC00, 0, 1);
}

/* legacy typing of shared globals in this part of the unit */
#define D_800A4258 (*(struct E (*)[])&D_800A4258)
#define D_800F3A38 (*(char (*)[])&D_800F3A38)

void func_8007B954()
{
    int a, s4, s3;
    /* ASM_BARRIER: pin q to a0 ($4) to reproduce original register allocation */
    register unsigned char *q asm("$4");
    unsigned char *r, *r2;
    char *b3;
    unsigned char *b2;
    unsigned char (*tb)[4];
    int t, c;
    int ia;
    struct E *base;
    struct E *p;
    signed char sb;

    base = D_800A4258;
    p = base + D_800F3A1C;
    q = (unsigned char *)(int)p->a;
    c = p->c;
    D_8010948E = c;
    if (D_800F4D8A == 0) {
        sb = p->d;
        if (sb >= 0) {
            D_80109478[c] = sb;
        }
        D_800F4D8A = 1;
    }
    tb = D_800A44A8;
    ia = (int)q * 4;
    b2 = D_800F3A24;
    r = b2 + D_800FFC00;
    q = (unsigned char *)tb + ia;
    a = q[0];
    s4 = q[1];
    s3 = q[2];
    if (*r < 0x10 || a == *r) {
        if (D_800F4D80[0x56C] == 2) {
            func_8002B330(((unsigned short)D_800AC594 & 1) << 2, 2, D_8010948E * 2 + (int)D_80109478, 2, 0);
            b3 = D_800F3A38;
        r2 = (unsigned char *)b3 + D_800AC594 * 0x5C;
            if (*(int *)(r2 + 0x28) != 0) {
                PlaySfx(1);
                if (D_80109478[D_8010948E] == 0) {
                    D_800F3A1C = s4;
                } else {
                    D_800F3A1C = s3;
                }
                D_800F4D8A = 0;
            } else if (*(int *)(r2 + 0x2C) != 0) {
                PlaySfx(2);
                D_800F3A1C = s3;
                D_800F4D8A = 0;
            }
        }
    } else {
        if (*r == 0x32) return;
        if (*r != 0x14) return;
        t = D_800F3A1C;
        if (t == 2 || t == 0x1A || t == 0x2D || t == 0x3C) {
            D_80109458 = D_800F3A1C;
            D_800F3A1C = 8;
        } else if (t == 0x10 || t == 0x23) {
            D_800F3A1C = 0x12;
        } else if (t == 0x34) {
            D_800F3A1C = 0x36;
        }
        D_800F4D8A = 0;
    }
}

#undef D_800A4258
#undef D_800F3A38

void func_8007BB9C()
{
    unsigned char *e;
    int s2, s4, s0, s3;
    int a0;
    unsigned char (*tab)[4];
    int i = D_800A4258[D_800F3A1C].b;
    tab = D_800A44D8;
    e = tab[i];
    s2 = e[0];
    s4 = e[1];
    s0 = e[2];
    s3 = e[3];
    if (D_800FFC18 != 0) goto l54;
    if (D_800F3A1C != 0x3D) goto l54;
    if (func_80078414(D_800ACBCC[D_800FFC00], D_800A457C) == 0) goto l48;
    D_80109490 = 1;
    goto l54;
l48:
    D_80109490 = 0;
l54:
    if (func_80078414(D_800ACBCC[D_800FFC00], D_800F4D50) != 1) goto ca0;
    D_800F3A1C = s2;
    if (s0 == 1) { a0 = 2; goto cf0; }
    func_8007A9CC(2, 0);
    return;
ca0:
    if (s0 != 1) goto ce0;
    if (D_800F3A24[D_800FFC00] + s3 < 0x10) goto ce0;
    D_800F3A1C = 0x16;
    return;
ce0:
    D_800F3A1C = s4;
    if (s0 != 1) return;
    a0 = 1;
cf0:
    func_8007A310(a0, 0);
}
