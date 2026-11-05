/* RODATA 0x80010938 0x24c */
#include "types.h"
#include "scratchpad.h"
#include "game/ui/hudmap.h"
#include "game/ui/uiwin.h"
#include "psyq/libgpu/addprim.h"
#include "game/menu/mnrows.h"
#include "game/ui/uitext.h"
#include "rr/bios_devices.h"
#include "rr/course_choices.h"

typedef struct {
    s32 prim;
} Scratchpad;

typedef struct {
    s16 x, y, w, h;
} RectS16;

typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
    s16 s4;
    s16 s6;
    s16 s8;
    s16 sA;
    s8 c0C;
    u8 pad0D[7];
    u8 b14;
    u8 b15;
    u8 b16;
    u8 b17;
    RectS16 rect18;
    s16 s20;
    u8 pad22[2];
} UiElem24;

typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
} FontGlyph4;

typedef struct { char f0, f1, f2, f3; unsigned short s[6]; unsigned char b0, b1; char pad[2]; } E;

typedef struct { unsigned short s[6]; unsigned char b0, b1, pad[2]; } Q;

typedef struct {
    signed char f0, f1;
    unsigned char f2, f3;
    char pad[16];
} E_1;

typedef struct {
    char f0, f1, f2, f3;
    short x, y;
    unsigned short u, v, cx, cy;
    unsigned char b0, b1;
    char pad[2];
} E_2;

extern s32 func_8001F5B4();
extern s32 func_8001FC28();
extern s32 func_8001FCC8();
extern s32 func_8001FD20();
extern s32 func_8001FF84();
extern s32 rsin();
extern s32 rcos();
extern char D_80010924[];
extern char D_80010928[];
extern char D_8001092C[];
extern FontGlyph4 D_8009FA1C[];
extern FontGlyph4 D_8009FB80[];
extern FontGlyph4 D_8009FC0C[];
extern s8 D_8009FC14[][4];
extern s8 D_8009FC7C[][8];
extern FontGlyph4 D_8009FCDC[];
extern s8 D_8009FD00[];
extern s16 D_800ABEC8;
extern s16 D_800AC59C;
extern s32 D_800ACC14;
extern s16 D_800AD522;
extern s16 D_800F3826;
extern u8 D_800F3920[];
extern s32 D_800F3A30;
extern UiElem24 *D_800F4D80;
extern s32 D_80119754;
extern s32 D_80119758;
extern Q D_8009F9FC[];
extern int func_8003DCEC();
extern int func_80020074();
extern int func_8002039C();

int func_8003EC88(s32 arg0);
void func_8003FAE4(int i, char a, char b, int j);
int func_8003FBA8(int a0);
void func_8003FE78(int a0, int a1, int a2, int a3);

s32 func_8003EC88(s32 arg0) {
    RectS16 sp28;
    s32 sp30;
    s32 sp34;
    s32 sp38;
    s32 s0;
    s32 s1;
    s32 s2;
    /* ASM_BARRIER: Keep s3 in $s3 while s3_init has single assignment in prologue */
    register s32 s3 asm("s3");
    /* ASM_BARRIER: Use $0 register operand so reload_cse_regs does not replace 0 with $fp */
    register s32 zero asm("$0");
    s32 s3_init;
    s32 s4;
    s32 s5_1;
    s32 s5_2;
    s32 s6;
    s32 s7;
    s32 fp;
    u32 a0_byte;
    s32 a0_arg;
    FontGlyph4 *glyph;

    fp = 0;
    s3_init = ((Scratchpad *)SCRATCHPAD)->prim;
    sp38 = D_800F4D80[arg0].s4;
    s2 = zero;
    sp30 = D_800F4D80[arg0].b14;
    s4 = sp38;
    a0_byte = D_800F4D80[arg0].b0;
    s7 = D_800F4D80[arg0].s6;
    s6 = D_800F4D80[arg0].b15;
    sp34 = D_800F4D80[arg0].b16;

    switch (sp30) {
    case 0:
        D_800F4D80[arg0].b14 = 1;
        D_800F4D80[arg0].rect18.w = 0;
        D_800F4D80[arg0].rect18.x = D_800F4D80[arg0].s4;
        D_800F4D80[arg0].rect18.y = D_800F4D80[arg0].s6;
        D_800F4D80[arg0].rect18.h = D_800F4D80[arg0].sA;
        D_800F4D80[arg0].b0 = 0;
        D_800F4D80[arg0].c0C = 0;
        break;
    case 1:
        D_800F4D80[arg0].rect18.w = (rsin((u16)a0_byte << 6) * D_800F4D80[arg0].s8) / 4096;
        if ((u16)a0_byte >= 16) {
            D_800F4D80[arg0].b0 = 0;
            D_800F4D80[arg0].b14 = 2;
        } else {
            D_800F4D80[arg0].b0++;
        }
        break;
    case 2:
        if (D_800F4D80[arg0].b2 == 1) {
            if (D_800F4D80[arg0].c0C & 2) {
                s2 = rcos((D_800F4D80[arg0].b17 << 4) - 0x2AA);
                fp = (u32)s2 >> 31;
                if (s2 >= 0) {
                    s2 = ((s2 >> 6) + 100) << 1;
                } else {
                    s2 = 0;
                }
            }
            if (D_800F4D80[arg0].s20 == 1) {
                fp = 2;
            }
        }
        break;
    case 3:
        D_800F4D80[arg0].rect18.w = (rcos((u16)a0_byte << 6) * D_800F4D80[arg0].s8) / 4096;
        if ((u16)a0_byte >= 16) {
            D_800F4D80[arg0].b0 = 0;
            D_800F4D80[arg0].b14 = 4;
        } else {
            D_800F4D80[arg0].b0++;
        }
        break;
    case 4:
    case 7:
        if (a0_byte >= 2) {
            D_800F4D80[arg0].b0 = 0;
            if (sp30 == 7) {
                D_800F4D80[arg0].b14 = 8;
            } else if (D_800F4D80[arg0].b1 == 0) {
                D_800F4D80[arg0].b14 = 5;
            } else {
                D_800F4D80[arg0].b14 = 6;
            }
        } else {
            D_800F4D80[arg0].b0++;
        }
        break;
    case 5:
    case 6:
        break;
    case 8:
        if (a0_byte >= 16) {
            D_800F4D80[arg0].b0 = 0;
            D_800F4D80[arg0].b14 = 9;
        } else {
            D_800F4D80[arg0].b0++;
        }
        /* fallthrough */
    case 9:
        if (D_800F4D80[arg0].b2 == 1) {
            fp = (D_800F4D80[arg0].c0C != 1) << 1;
        }
        break;
    }

    sp28 = D_800F4D80[arg0].rect18;
    s3 = func_8001FC28(D_800ACC14 + 0x70, s3_init, 0, 0, 0x140, 0x200);
    s4 += 7;

    switch (s6) {
    case 5:
    case 6:
    case 7:
        if (s6 == 5) {
            s0 = D_800F3A30;
        }
        if (s6 == 6) {
            s0 = D_800AC59C - 2;
        }
        fp = 0;
        if (s6 == 7) {
            s0 = D_800ABEC8 + 1;
        }
        s2 = 0;
        /* fallthrough */
    case 0:
        {
            FontGlyph4 *base_0 = D_8009FA1C;
            s3 = func_8001FF84(D_800ACC14 + 0x70, s3, s4, s7 + 8, &base_0[sp34], fp | 0x7FF8, s2);
            if (s6 != 0) {
                for (s1 = 0; s1 < 2; s1++) {
                    s3 = func_8001FF84(D_800ACC14 + 0x70, s3, s4 + 0x1E + s1 * 0x16, s7 + 8, &D_8009FC0C[s1], 0x7FF8, 0);
                    if (s1 == 0) {
                        D_8009FD00[0] = s0 / 10;
                    } else {
                        D_8009FD00[0] = (s0 % 10) * 8;
                    }
                    s3 = func_8001FF84(D_800ACC14 + 0x70, s3, s4 + 0x26 + s1 * 6, s7 + 8, D_8009FD00, 0x7FF8, 0);
                }
                /* ASM_BARRIER: Keep base_0 (D_8009FA1C) live across case 0 loop so reload rematerializes it into $t0 */
                __asm__("" : : "ri"(base_0));
            }
        }
        break;
    case 1:
    case 4:
    case 8:
    case 9:
        for (s1 = 0; s1 < 4; s1++) {
            s0 = D_8009FC14[sp34][s1];
            if (s0 == -1) {
                break;
            }
            s5_1 = fp | 0x7FF8;
            s3 = func_8001FF84(D_800ACC14 + 0x70, s3, s4, s7 + 8, &D_8009FA1C[s0], s5_1, s2);
            s4 += 2 + D_8009FA1C[s0].b2;
        }
        if (s6 == 4) {
            if (D_800AD522 == 1) {
                s0 = 0xD;
            } else {
                s0 = 0xC;
            }
            s3 = func_8001FF84(D_800ACC14 + 0x70, s3, s4, s7 + 8, &D_8009FA1C[s0], 0x7FF8, 0);
        } else if ((u32)(s6 - 8) < 2U) {
            s3 = func_8001F5B4(D_800ACC14 + 0x70, s3, s4, s7 + 8, (s6 == 8) ? D_80010924 : D_80010928, fp | 0x7FF8, 0, s2);
        }
        break;
    case 2:
        for (s1 = 0; s1 < 7; s1++) {
            s0 = D_8009FC7C[sp34][s1];
            if (s0 == -1) {
                break;
            }
            a0_arg = D_800ACC14 + 0x70;
            glyph = &D_8009FA1C[s0];
            s5_2 = fp | 0x7FF8;
            s3 = func_8001FF84(a0_arg, s3, s4, s7 + 8, glyph, s5_2, 0);
            s4 += 2 + D_8009FA1C[s0].b2;
        }
        s3 = func_8001FD20(D_800ACC14 + 0x70, s3, sp38 + 0xA9, s7 + 6, 1, 0x14, 0xFF, 0xFF, 0xFF);
        s4 = sp38 + 0xB0;
        for (s1 = 0; s1 < 2; s1++) {
            if (s1 == 0) {
                s0 = 0x45;
            } else {
                s0 = D_8009FC7C[sp34][7];
            }
            s3 = func_8001FF84(D_800ACC14 + 0x70, s3, s4, s7 + 8, &D_8009FA1C[s0], 0x7FF8, 0);
            s4 += 2 + D_8009FA1C[s0].b2;
        }
        break;
    case 3:
        for (s1 = 0; s1 < 7; s1++) {
            s0 = D_8009FC7C[sp34][s1];
            if (s0 == -1) {
                break;
            }
            s3 = func_8001FF84(D_800ACC14 + 0x70, s3, s4, s7 + 8, &D_8009FA1C[s0], 0x7FF8, 0);
            s4 += 2 + D_8009FA1C[s0].b2;
            if (s1 == 3) {
                s3 = func_8001FD20(D_800ACC14 + 0x70, s3, sp38 + 0x7E, s7 + 6, 1, 0x14, 0xFF, 0xFF, 0xFF);
                s4 = sp38 + 0x86;
            }
        }
        break;
    case 10:
    case 11:
        s3 = func_8001FF84(D_800ACC14 + 0x70, s3, s4, s7 + 8, &D_8009FB80[0], 0x7FF8, 0);
        s3 = func_8001FF84(D_800ACC14 + 0x70, s3, s4 + 0x44, s7 + 8, &D_8009FB80[1], 0x7FF8, 0);
        if (s6 == 10) {
            s0 = D_80119754;
        } else {
            s0 = D_80119758;
        }
        for (s1 = 0; s1 < s0; s1++) {
            s3 = func_8001FD20(D_800ACC14 + 0x70, s3, s4 + 0x12 + s1 * 3, s7 + 8, 2, 0x10, 0xFF, 0xFF, 0xFF);
        }
        break;
    case 12:
        if (D_800F3920[D_800F3826 + 0x5C] == 1) {
            s2 = 8;
        } else {
            s2 = 7;
        }
        if (s2 != 0) {
            s1 = 0;
            do {
                s0 = D_8009FC7C[sp34][s1];
                s3 = func_8001FF84(D_800ACC14 + 0x70, s3, s4, s7 + 8, &D_8009FA1C[s0], 0x7FF8, 0);
                s4 += 2 + D_8009FA1C[s0].b2;
                s1++;
            } while (s1 < s2);
        }
        break;
    case 0x7E:
        s3 = func_8001F5B4(D_800ACC14 + 0x70, s3, s4, s7 + 8, D_8001092C, 0x7FF8, 2, 0);
        break;
    case 0x7F:
        s3 = func_8001FF84(D_800ACC14 + 0x70, s3, s4, s7 + 8, &D_8009FCDC[sp34], fp | 0x4820, s2);
        break;
    }

    ((Scratchpad *)SCRATCHPAD)->prim = func_8001FCC8(D_800ACC14 + 0x70, s3, 0x38);
    func_8003F920(sp38, s7, D_800F4D80[arg0].s8, D_800F4D80[arg0].sA, 0x4A, 0x4A, 0x4A);
    ((Scratchpad *)SCRATCHPAD)->prim = func_8001FC28(D_800ACC14 + 0x70, ((Scratchpad *)SCRATCHPAD)->prim, sp28.x, sp28.y, sp28.w, sp28.h);
    sp38 += D_800F4D80[arg0].rect18.w;
    if ((u32)(sp30 - 4) >= 3U) {
        func_8003EA04(arg0, sp38, s7);
    }
    return (u32)~sp30 >> 31;
}

int func_8003F920(a0, a1, a2, a3, a4, a5, a6) int a0; int a1; int a2; int a3; int a4; int a5; int a6;
{
    int *px;
    int *py;
    int t;
    int k;
    int j;
    int x[4];
    int g;
    int y[4];
    int i;

    x[0] = x[3] = a0;
    x[1] = x[2] = a0 + a2;
    y[0] = y[1] = a1;
    y[2] = y[3] = a1 + a3 - 2;
    i = 0;
    do {
        t = D_800ACC14 + 0x70;
        px = x + i;
        py = y + i;
        j = i + 1;
        k = j;
        if (k < 0) k = i + 4;
        k >>= 2;
        k <<= 2;
        k = j - k;
        func_80022694(t, *px, *py, x[k], y[k], 0x100, 1, 0xFF, 0xFF, 0xFF, -1);
        i++;
    } while (i < 4);
    x[0] = x[1] - x[3];
    y[0] = y[3] - y[1];
    g = *(int *)SCRATCHPAD;
    *(int *)SCRATCHPAD = func_8001FD20(D_800ACC14 + 0x70, g, a0, a1, x[0], y[0], a4, a5, a6);
}

int func_8003FAA0(int a) {
    int sz = D_800ACC24 * 28;
    D_800F2AB0 = a;
    func_80096434(a, sz);
    return sz;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F2AB0 ((E *)D_800F2AB0)

void func_8003FAE4(int i, char a, char b, int j) {
    if (i < D_800ACC24) {
        Q *q;
        E *p;
        D_800F2AB0[i].f1 = a;
        D_800F2AB0[i].f3 = b;
        p = (E *)(i * 20 + (int)D_800F2AB0);
        q = &D_8009F9FC[j];
        p->s[0] = q->s[0];
        p->s[1] = q->s[1];
        p->s[2] = q->s[2];
        p->s[3] = q->s[3];
        p->s[4] = q->s[4];
        p->s[5] = q->s[5];
        p->b0 = q->b0;
        D_800F2AB0[i].b1 = q->b1;
    }
}

#undef D_800F2AB0

/* legacy typing of shared globals in this part of the unit */
#define D_800F2AB0 ((E_1 *)D_800F2AB0)

int func_8003FBA8(int a0) {
    int s1;
    int s2;
    int state;
    unsigned short val;
    int pad[16];

    s2 = 0;
    state = D_800F2AB0[a0].f1;
    s1 = D_800F2AB0[a0].f2;
    switch (state) {
    case -1:
        if (func_8003DCEC(D_800F2AB0[a0].f3) == 1) {
            D_800F2AB0[a0].f1 = 0;
        }
        break;
    case 0:
        D_800F2AB0[a0].f2 = 0;
        D_800F2AB0[a0].f1 = 1;
    case 1:
        s2 = 1;
        val = (s1 << 7) / 30;
        func_8003FE78(a0, 0, val, 0);
        if (s1 >= 30) {
            D_800F2AB0[a0].f2 = 0;
            D_800F2AB0[a0].f1 = 2;
        } else {
            D_800F2AB0[a0].f2++;
        }
        break;
    case 2:
    case 4:
    default:
        s2 = 2;
        func_8003FE78(a0, 0, 0x80, 0);
        break;
    case 3:
        val = rcos((s1 << 10) / 30) / 32;
        func_8003FE78(a0, 1, val, 0);
        if (s1 >= 30) {
            D_800F2AB0[a0].f1 = 5;
            D_800F2AB0[a0].f2 = 0;
        } else {
            D_800F2AB0[a0].f2++;
        }
        break;
    case 5:
        break;
    case 6:
        s2 = 1;
        val = ((30 - s1) << 7) / 30;
        func_8003FE78(a0, 0, val, 0);
        if (s1 >= 30) {
            D_800F2AB0[a0].f2 = 0;
            D_800F2AB0[a0].f1 = 7;
        } else {
            D_800F2AB0[a0].f2++;
        }
        break;
    case 7:
        s2 = 1;
        func_8003FE78(a0, 0, 0, 0);
        break;
    }
    return s2;
}

#undef D_800F2AB0

/* legacy typing of shared globals in this part of the unit */
#define D_800F2AB0 ((E_2 *)D_800F2AB0)

void func_8003FE78(int a0, int a1, int a2, int a3) {
    int i, j, k;
    int tx, ty;
    int x[4], y[4], u[4], v[4];
    unsigned short tpage, clut;
    int s2;

    s2 = *(int *)SCRATCHPAD;
    for (i = 0; i < 18; i++) {
        tx = D_800F2AB0[a0].u + (i % 9) * 24;
        ty = D_800F2AB0[a0].v + (i / 9) * 96;
        tpage = ((ty & 0x100) >> 4) | 0x80 | ((tx & 0x3FF) >> 6);
        clut = func_80095A24(D_800F2AB0[a0].cx, D_800F2AB0[a0].cy + i / 2);
        for (j = 0; j < 4; j++) {
            if (a1 == 0) {
                u[0] = (tx % 64) * 2 + (j & 1) * 24;
                v[0] = (i / 9) * 96;
                if (j >= 2) {
                    v[0] += 48;
                }
                x[0] = D_800F2AB0[a0].x + (i % 6) * 48 + (j & 1) * 24;
                y[0] = D_800F2AB0[a0].y + (i / 6) * 96;
                if (j >= 2) {
                    y[0] += 48;
                }
                s2 = func_80020074(D_800ACC14 + 0xB48, s2, x[0], y[0], 24, 48, u[0], v[0], tpage, clut, a3, a2);
            } else if (a1 == 1) {
                for (k = 0; k < 4; k++) {
                    x[k] = D_800F2AB0[a0].x + (i % 6) * 48 + (k & 1) * 48;
                    y[k] = D_800F2AB0[a0].y + (i / 6) * 96 + j * 24;
                    if (k >= 2) {
                        y[k] += 24;
                    }
                    u[k] = (tx % 64) * 2 + (k & 1) * 48;
                    v[k] = (i / 9) * 96 + j * 24;
                    if (k >= 2) {
                        v[k] += 24;
                    }
                }
                y[0] = -(a2 * 12) / 128 + y[0] + 12;
                y[1] = -(a2 * 12) / 128 + y[1] + 12;
                y[2] = (a2 * 12) / 128 + y[2] - 12;
                y[3] = (a2 * 12) / 128 + y[3] - 12;
                s2 = func_8002039C(D_800ACC14 + 0xB48, s2, &x[0], &y[0], &u[0], &v[0], tpage, clut, 0, a2);
                s2 = func_8002039C(D_800ACC14 + 0xB48, s2, &x[1], &y[1], &u[1], &v[1], tpage, clut, 0, a2);
            }
        }
    }
    *(int *)SCRATCHPAD = s2;
}

#undef D_800F2AB0
