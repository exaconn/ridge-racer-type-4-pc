#include "game/race/objrec.h"
#include "game/car/aipath.h"
#include "psyq/libgte/gtemath4.h"
#include "psyq/libgte/gtemath5.h"
#include "game/race/objclr.h"
#include "game/gfx/polytri.h"
#include "rr/animated_glyph.h"
#include "rr/audio_loop.h"
typedef struct {
    int x;
} __attribute__((packed)) U4;

typedef struct {
    unsigned short v0[4];
    unsigned short v1[4];
    unsigned short v2[4];
    int pad[5];
    int c0;
    int c1;
    int c2;
} InTri;

typedef struct {
    char pad0[0x78];
    U4 sxy0;
    U4 sxy1;
    U4 sxy2;
    char pad84[0xB4 - 0x84];
    unsigned short v0[6];
    unsigned short v1[6];
    unsigned short v2[6];
} Sp7C;

typedef struct {
    unsigned int tag;
    int c0;
    U4 sxy0;
    int c1;
    U4 sxy1;
    int c2;
    U4 sxy2;
} PolyG3;

typedef struct {
    unsigned short v0[4];
    unsigned short v1[4];
    unsigned short v2[4];
    unsigned short v3[4];
    int pad[6];
    int c0;
    int c1;
    int c2;
    int c3;
} InQuad;

typedef struct {
    char pad0[0x78];
    U4 sxy0;
    U4 sxy1;
    U4 sxy2;
    U4 sxy3;
    char pad88[0xB4 - 0x88];
    unsigned short v0[6];
    unsigned short v1[6];
    unsigned short v2[6];
    unsigned short v3[6];
} Sp7C_1;

typedef struct {
    unsigned int tag;
    int c0;
    U4 sxy0;
    int c1;
    U4 sxy1;
    int c2;
    U4 sxy2;
    int c3;
    U4 sxy3;
} PolyG4;

/* size 0x30 */
typedef struct {
    char pad0[0x184];
    int f184;
    char pad1[0x1EA - 0x188];
    short f1EA;
    char pad2[0x2AA - 0x1EC];
    short f2AA;
    char pad3[0x320 - 0x2AC];
} S_ABEF8;
/* size 0x320 */
typedef struct {
    int f0;
    int f4;
    int f8;
} S_SP7C;
extern S_ABEF8 D_800ABEF8[];
extern int D_800AD504;
extern short D_800AD522;
extern int D_800F2EB8;
extern int D_800F3A30;
extern S_ECB0 *D_8010ECB0;
extern S_ECB4 *D_8010ECB4;

void func_80071978(InTri *a0, int a1, int a2);
void func_80071B94(InQuad *a0, int a1, int a2);

void func_80071978(InTri *a0, int a1, int a2) {
    int sp20;
    int sp24;
    int i;
    int otz;
    unsigned int *ot;
    unsigned int *ot_base;
    int *hw = (int *)0x1F800000;
    PolyG3 *pkt = (PolyG3 *)hw[0];
    Sp7C *sp = (Sp7C *)0x1F80007C;

    for (i = 0; i < a1; i++, a0++) {
        sp->v0[0] = a0->v0[0];
        sp->v0[1] = a0->v0[1];
        sp->v0[2] = a0->v0[2];
        sp->v1[0] = a0->v1[0];
        sp->v1[1] = a0->v1[1];
        sp->v1[2] = a0->v1[2];
        sp->v2[0] = a0->v2[0];
        sp->v2[1] = a0->v2[1];
        sp->v2[2] = a0->v2[2];
        otz = func_80091764(sp->v0, sp->v1, sp->v2, &sp->sxy0, &sp->sxy1, &sp->sxy2, &sp20, &sp24) >> (hw[0x18] + a2);
        if (otz > 0 && otz < 0x1C0) {
            if (a2 == 2) {
                otz = 0x1BF;
            }
            ot_base = (unsigned int *)hw[1];
            pkt->sxy0 = sp->sxy0;
            pkt->sxy1 = sp->sxy1;
            pkt->sxy2 = sp->sxy2;
            pkt->c0 = a0->c0;
            pkt->c1 = a0->c1;
            pkt->c2 = a0->c2;
            pkt->tag = 0x06000000;
            ((char *)pkt)[7] = 0x32;
            ot = ot_base + (otz + 0x80);
            func_80095A44(ot, pkt++);
            pkt->c0 = 0xE1000220;
            pkt->tag = *ot;
            ((char *)pkt)[3] = 1;
            *ot = ((unsigned int)pkt << 8) >> 8;
            pkt = (PolyG3 *)((char *)pkt + 8);
        }
    }
    hw[0] = (int)pkt;
}

void func_80071B94(InQuad *a0, int a1, int a2) {
    int sp28;
    int sp2C;
    int i;
    int otz;
    unsigned int *ot;
    unsigned int *ot_base;
    int *hw = (int *)0x1F800000;
    PolyG4 *pkt = (PolyG4 *)hw[0];
    Sp7C_1 *sp = (Sp7C_1 *)0x1F80007C;

    for (i = 0; i < a1; i++, a0++) {
        sp->v0[0] = a0->v0[0];
        sp->v0[1] = a0->v0[1];
        sp->v0[2] = a0->v0[2];
        sp->v1[0] = a0->v1[0];
        sp->v1[1] = a0->v1[1];
        sp->v1[2] = a0->v1[2];
        sp->v2[0] = a0->v2[0];
        sp->v2[1] = a0->v2[1];
        sp->v2[2] = a0->v2[2];
        sp->v3[0] = a0->v3[0];
        sp->v3[1] = a0->v3[1];
        sp->v3[2] = a0->v3[2];
        otz = func_800917C4(sp->v0, sp->v1, sp->v2, sp->v3, &sp->sxy0, &sp->sxy1, &sp->sxy2, &sp->sxy3, &sp28, &sp2C) >> (hw[0x18] + a2);
        if (otz > 0 && otz < 0x1C0) {
            if (a2 == 2) {
                otz = 0x1BF;
            }
            ot_base = (unsigned int *)hw[1];
            pkt->sxy0 = sp->sxy0;
            pkt->sxy1 = sp->sxy1;
            pkt->sxy2 = sp->sxy2;
            pkt->sxy3 = sp->sxy3;
            pkt->c0 = a0->c0;
            pkt->c1 = a0->c1;
            pkt->c2 = a0->c2;
            pkt->c3 = a0->c3;
            pkt->tag = 0x08000000;
            ((char *)pkt)[7] = 0x3A;
            ot = ot_base + (otz + 0x80);
            func_80095A44(ot, pkt++);
            pkt->c0 = 0xE1000220;
            pkt->tag = *ot;
            ((char *)pkt)[3] = 1;
            *ot = ((unsigned int)pkt << 8) >> 8;
            pkt = (PolyG4 *)((char *)pkt + 8);
        }
    }
    hw[0] = (int)pkt;
}

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
