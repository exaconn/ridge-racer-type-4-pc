typedef struct {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
    unsigned short f1C;
    unsigned short f1E;
    unsigned short f20;
    unsigned short f22;
    int f24;
    int f28;
} S_ECB4; /* size 0x2C */

typedef struct {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
    int f1C;
    int f20;
    int f24;
    int f28;
    int f2C;
} S_ECB0; /* size 0x30 */

typedef struct {
    char pad0[0x184];
    int f184;
    char pad1[0x1EA - 0x188];
    short f1EA;
    char pad2[0x2AA - 0x1EC];
    short f2AA;
    char pad3[0x320 - 0x2AC];
} S_ABEF8; /* size 0x320 */

typedef struct {
    int f0;
    int f4;
    int f8;
} S_SP7C;

extern int func_80026998(int, int);

extern S_ABEF8 D_800ABEF8[];
extern int D_800AD504;
extern short D_800AD522;
extern int D_800F2EB8;
extern int D_800F3A30;
extern short D_800F4D44;
extern int D_8010ECA8;
extern S_ECB0 *D_8010ECB0;
extern S_ECB4 *D_8010ECB4;

void func_80071E04(void) {
    int i;
    S_SP7C *sp = (S_SP7C *)0x1F80007C;
    int *hw = (int *)0x1F800000;

#define b0 (&D_8010ECB0[i])
#define b4 (&D_8010ECB4[i])

    sp->f0 = func_80026998(hw[2], hw[4]);
    for (i = 0; i < D_8010ECA8; i++) {
        if (b0->f18 >= 0 && b0->f18 != D_800ABEF8[0].f2AA) {
            if (b0->f10 > 0) {
                b4->f24 = 0;
            }
            continue;
        }
        if (b0->f2C != 0x3F) {
            if (D_800F4D44 == 1 && D_800AD504 < 2) {
                if (!(b0->f2C & 1)) {
                    continue;
                }
            } else if (D_800F4D44 == 1 && D_800AD504 == 2) {
                if (!(b0->f2C & 2)) {
                    continue;
                }
            } else if (D_800F4D44 == 0 && D_800AD504 < 2) {
                if (!(b0->f2C & 4)) {
                    continue;
                }
            } else if (D_800F4D44 == 0 && D_800AD504 == 2) {
                if (!(b0->f2C & 8)) {
                    continue;
                }
            } else if (D_800F4D44 == 6) {
                if (b0->f2C == 0xA) {
                    if (D_800AD504 != 2) {
                        continue;
                    }
                } else if (!(b0->f2C & 0x10)) {
                    continue;
                }
            } else if (D_800F4D44 == 5) {
                if (b0->f2C != 0x20) {
                    continue;
                }
            } else {
                if (b0->f2C != 0xA || D_800AD504 != 2) {
                    continue;
                }
            }
        }
        sp->f4 = b0->f1C;
        sp->f8 = b0->f20;
        if (b0->f10 > 0) {
            if (b0->f18 == -3) {
                if ((D_800ABEF8[0].f2AA < 6 && (D_800F2EB8 * 4 + ((D_800F2EB8 * 59) >> 6)) >= D_800ABEF8[0].f184) ||
                    (sp->f4 < sp->f0 && sp->f0 <= sp->f8)) {
                    b4->f24 = 0;
                    b4->f8 = 0;
                } else {
                    b4->f24 = 1;
                }
                continue;
            }
            if (sp->f4 >= sp->f8) {
                if (sp->f4 >= sp->f0 && sp->f0 >= sp->f8) {
                    goto set_active;
                }
            } else {
                if (sp->f4 >= sp->f0 || sp->f0 > sp->f8) {
                    goto set_active;
                }
            }
            if (b0->f18 == -2) {
            set_active:
                b4->f24 = 1;
            } else {
                b4->f24 = 0;
                continue;
            }
        } else if (b0->f14 > 0 && b4->f24 == 0 && sp->f4 == sp->f0 && D_800ABEF8[0].f1EA == 0 && D_800AD522 == 0) {
            b4->f24 = 1;
        }
        if (D_800F4D44 == 2 || D_800F4D44 == 3) {
            if (b0->f0 == 0 && (b0->f4 % 10) == 0) {
                if (D_800ABEF8[0].f2AA == 0) {
                    if (D_800ABEF8[1].f2AA == 0) {
                        b4->f24 = 1;
                    } else {
                        b4->f24 = 0;
                    }
                } else {
                    b4->f24 = 0;
                }
            } else if (b0->f0 == 1 && (b0->f4 % 10) == 0) {
                if (((D_800F3A30 - 1) * D_800F2EB8 + ((D_800F2EB8 * 29) >> 5)) < D_800ABEF8[0].f184 ||
                    ((D_800F3A30 - 1) * D_800F2EB8 + ((D_800F2EB8 * 29) >> 5)) < D_800ABEF8[1].f184) {
                    b4->f24 = 1;
                } else {
                    b4->f24 = 0;
                }
            }
        } else if ((D_800F4D44 == 4 || D_800F4D44 == 5) &&
                   (((b0->f4 % 10) == 0 && (b0->f0 == 0 || b0->f0 == 1)) ||
                    (b0->f8 == 0xA && b0->f4 == 0) ||
                    b0->fC == 4)) {
            b4->f24 = 0;
        } else if (b0->f0 == 1 && (b0->f4 % 10) == 0) {
            if (((D_800F3A30 - 1) * D_800F2EB8 + ((D_800F2EB8 * 29) >> 5)) > D_800ABEF8[0].f184) {
                b4->f24 = 0;
            }
        }
    }
}
