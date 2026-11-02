/* GCC_VERSION: 2.7.2 */
extern int func_8009A7F4(), func_8009A814();
extern int D_800ABCAC;
extern volatile unsigned *D_800ABCC8;
extern volatile unsigned char *D_800ABCCC;
extern unsigned D_801146B8, D_801146BC;
struct W { char p[0x3C]; unsigned char *f3c; char p2[8]; unsigned char f44, f45; char p3[0xE8 - 0x46]; unsigned char fE8; };
signed char func_80098788(a0, a1) struct W *a0; unsigned char a1; {
    short s1;
    unsigned char s2;
    unsigned v1, t;
    volatile unsigned short *r120, *r124, *r128;
    s1 = 0x88;
    if ((*a0->f3c >> 4) == 8) {
        if (a0->f44 >= 9) s1 = 0x22;
    }
    do { } while (!(*(volatile unsigned short *)(D_800ABCCC + 4) & 2));
    func_8009A7F4(0x190);
    s2 = D_800ABCCC[0];
    if (a0->f44 == 0) {
        if ((s2 >> 4) != 8) goto b7;
        *(volatile short *)(D_800ABCCC + 0xE) = 0x22;
    } else {
    b7:
        *(volatile short *)(D_800ABCCC + 0xE) = s1;
    }
    if (!(*D_800ABCC8 & 0x80)) {
        r120 = (volatile unsigned short *)0x1F801120;
        r128 = (volatile unsigned short *)0x1F801128;
        r124 = (volatile unsigned short *)0x1F801124;
    loop:
        v1 = *r120;
        if (v1 < D_801146B8) {
            if (*r128 != 0) v1 += *r128;
            else v1 += 0x10000;
        }
        t = v1 - D_801146B8;
        if (*r124 & 0x200) {
            if (t >= D_801146BC) return -2;
            goto b19;
        }
        if ((t >> 3) < D_801146BC) {
        b19:
            if (*D_800ABCC8 & 0x80) goto b20;
            goto loop;
        }
        return -2;
    }
b20:
    if (a0->fE8 != 8) {
        if (D_800ABCAC == 2) {
            func_8009A7F4(0x3C);
            do { } while (func_8009A814() == 0);
        }
    }
    D_800ABCCC[0] = a1;
    if (D_800ABCAC == 3) {
        if (s2 == 0x80) {
            *D_800ABCC8 = -0x81;
            *(volatile unsigned short *)(D_800ABCCC + 0xA) |= 0x10;
        }
    }
    a0->f45++;
    if (a0->f44 != 0xFF) a0->f3c[a0->f44] = s2;
    a0->f44++;
    return s2;
}
