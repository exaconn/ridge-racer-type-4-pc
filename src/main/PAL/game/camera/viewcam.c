#include "game/car/car.h"
#include "types.h"
#include "scratchpad.h"
#include "m2c_macros.h"
#include "game/camera/viewini.h"
#include "game/car/aipath.h"
#include "game/gfx/poly1.h"
#include "game/math/rotmat.h"
#include "psyq/libgte/gtemat3.h"
#include "psyq/libgte/gtemat7.h"
#include "psyq/libgte/gtetrig.h"
#include "rr/hud_number.h"
#include "game/data/globals.h"
#include "game/gfx/drawbuf.h"

#define G ((Scratch_8006EF8C *)SCRATCHPAD)

struct S8 { int w[8]; };

typedef struct {
    s16 f0;
    s16 f2;
    s16 f4;
    s16 f6;
} S_800A3C20;

typedef struct {
    s32 f00;
    s32 f04;
    s32 pad08[23];
    s32 f64;
    s32 f68;
    u16 f6C;
    u16 f6E;
    u16 f70;
    u16 f72;
} Scratch_8006EF8C;


extern int ScaleMatrix();
extern int D_8010E690;
extern struct S8 D_800FF690;
extern struct S8 D_800F39A8;
extern int SetLightMatrix();
extern M2C_UNK D_800AD570;
extern char D_800ABF48[];
extern short D_800F3914;
extern void func_80060C70();
extern unsigned short D_8010E280;
extern unsigned int D_8010E290;
extern int ApplyMatrix();
extern int RotMatrixYXZ();
extern short D_800ABED0[][3];
extern char D_800F38F8[][8];
extern unsigned short D_800AD5BC[];
extern s16 D_800A3BA4[];
extern s16 D_800A3BB4[];
extern s32 D_800A3BC4[];
extern u16 D_800A3BE0[];
extern s16 D_800A3BF0[];
extern u16 D_800A3C00[];
extern s16 D_800A3C10[];
extern S_800A3C20 D_800A3C20[][7];
extern void func_800912B4(s32, s32);
extern void func_800912D4(s32);
extern void SetDrawArea(s32);
extern void AddPrim(s32, s32);
extern int D_8010718C;
extern Car D_800ABEF8[];

void func_8006E9E8(int a0, int *a1);
void func_8006EAC8(void);
void func_8006EC64(void);
void func_8006ECE8(int ignoredPlayer);
void func_8006ED4C(void);
void func_8006EDA4(void);
void func_8006EE3C(short *a0, short *a1, int *a2);
void func_8006EED8(void);
void func_8006EF8C(s32 a0);
int func_8006F188(int *a0, int a1);

void func_8006E9E8(int a0, int *a1) {
    int v[4], w[4], x[4];
    int *g = (int *)(SCRATCHPAD + 0x7C);
    if (a0) {
        func_8006E300();
        v[0] = (0x8000 - a1[0]) << 2;
        v[1] = (0x60 - a1[1]) << 2;
        v[2] = (0x8000 - a1[2]) << 2;
        ApplyMatrixLV((SCRATCHPAD + 0x108), v, w);
        x[2] = 0x4000;
        x[1] = 0x4000;
        x[0] = 0x4000;
        g[0x28] = w[0];
        g[0x29] = w[1];
        g[0x2A] = w[2];
        ScaleMatrix((SCRATCHPAD + 0x108), x);
        func_8006E9D4(D_8010E690, a0, a1);
    }
}

void func_8006EAC8(void) {
    int v[4], w[4], x[4];
    int *h = (int *)SCRATCHPAD;
    int *g = (int *)(SCRATCHPAD + 0x8);
    v[0] = 0x8000 - (g[0] >> 2);
    v[1] = -(g[1] >> 2);
    v[2] = 0x8000 - (g[2] >> 2);
    ApplyMatrixLV(SCRATCH_ROT_MATRIX, v, w);
    D_800FF690.w[5] = w[0];
    D_800FF690.w[6] = w[1];
    D_800FF690.w[7] = w[2];
    D_800F39A8 = D_800FF690;
    v[0] = 0x20000 - g[0];
    v[1] = -g[1];
    v[2] = 0x20000 - g[2];
    D_800F39A8.w[5] >>= 1;
    D_800F39A8.w[6] >>= 1;
    D_800F39A8.w[7] >>= 1;
    ApplyMatrixLV(SCRATCH_ROT_MATRIX, v, w);
    h[15] = w[0];
    h[16] = w[1];
    h[17] = w[2];
    SetTransMatrix(SCRATCH_ROT_MATRIX);
    x[2] = 0x4000;
    x[1] = 0x4000;
    x[0] = 0x4000;
    ScaleMatrix(SCRATCH_ROT_MATRIX, x);
    SetRotMatrix(SCRATCH_ROT_MATRIX);
}

/* legacy typing of shared globals in this part of the unit */
#define D_800ABED0 (*(M2C_UNK (*))&D_800ABED0)

void func_8006EC64(void) {
    SetColorMatrix(&D_800AD570);
    func_8006EED8();
    SetLightMatrix(&D_800ABED0);
    SetBackColor(D_800ABE98, D_801093F8, D_80107194);
    SetFarColor(D_800F4D84, D_800F4D68, D_800F4D48);
    func_80090594(0x4E20, 0x122);
}

#undef D_800ABED0

void func_8006ECE8(int ignoredPlayer) {
    char *p = D_800ABF48;
    func_8006E834();
    func_80071348();
    func_8006EC64();
    func_8006E9E8(p, p - 0x30);
    func_8006EAC8();
    func_80060C70(SCRATCHPAD, D_8010E690, D_800F3914);
}

/* legacy typing of shared globals in this part of the unit */
#define func_80060C70 ((int (*)())func_80060C70)
#define func_8006E834 ((int (*)())func_8006E834)
#define func_80071348 ((int (*)())func_80071348)

void func_8006ED4C(void) {
    func_8006E834();
    func_8006E8F8();
    func_80071348();
    func_8006EC64();
    func_8006EAC8();
    func_80060C70(SCRATCHPAD, D_8010E690, D_800F3914);
}

#undef func_80060C70
#undef func_8006E834
#undef func_80071348

/* legacy typing of shared globals in this part of the unit */
#define func_8006E834 ((int (*)())func_8006E834)
#define func_80060C70 ((int (*)())func_80060C70)

void func_8006EDA4(void) {
    short *g = (short *)SCRATCHPAD;
    unsigned int v; int pad[12];
    D_8010E280 = (unsigned short)g[0x2E];
    g[0x2E] = -1;
    func_8006E834();
    v = func_800713E0();
    if (v < D_8010E290) {
        D_8010E290 = v;
    }
    func_8006EC64();
    func_8006EAC8();
    func_80060C70(g, D_8010E690, D_800F3914);
    g[0x2E] = (short)D_8010E280;
}

#undef func_8006E834
#undef func_80060C70

void func_8006EE3C(short *a0, short *a1, int *a2) {
    short *g = (short *)SCRATCHPAD;
    a0[0] = a1[0] - g[4];
    a0[1] = a1[2] - g[6];
    a0[2] = a1[4] - g[8];
    ApplyMatrix(SCRATCH_ROT_MATRIX, a0, a0 + 4);
    *(int *)(a0 + 0x16) = *(int *)(a0 + 4);
    *(int *)(a0 + 0x18) = *(int *)(a0 + 6);
    *(int *)(a0 + 0x1A) = *(int *)(a0 + 8);
    SetRotMatrix(a2);
    SetTransMatrix(a0 + 12);
}

void func_8006EED8(void) {
    int i;
    int m[8];
    struct { short a, b, c, d; } s;
    struct { short x, p0, y, p1, z, p2, q[2]; } r;
    s.b = 0;
    s.a = 0;
    for (i = 0; i < 3; i++) {
        s.c = -D_800AD5BC[i];
        RotMatrixYXZ(D_800F38F8[i], m);
        SetRotMatrix(m);
        func_80090C74(&s, &r);
        D_800ABED0[i][0] = r.x;
        D_800ABED0[i][1] = r.y;
        D_800ABED0[i][2] = r.z;
    }
}

void func_8006EF8C(s32 a0) {
    /* ASM_BARRIER: Keep scratchpad base pointer in $s2 initialized in prologue. */
    register Scratch_8006EF8C *s2 asm("$18") = G;
    /* ASM_BARRIER: Keep s3 in $s3 so s3 += 0xC modifies $s3 in place before storing to SCRATCHPAD. */
    register s32 s3 asm("$19");
    s32 s1;
    s32 s5;
    s32 v1;
    S_800A3C20 *p;
    /* ASM_BARRIER: Keep a0_val in $a0 in the s5 != 0 branch so p is allocated to $a1. */
    register s32 a0_val asm("$4");
    /* ASM_BARRIER: Keep a2_val in $a2 in the s5 != 0 branch so p is allocated to $a1. */
    register s32 a2_val asm("$6");

    /* ASM_BARRIER: Materialize $s2 = SCRATCHPAD at start of prologue. */
    __asm__("" : "=r"(s2) : "0"(s2));
    s3 = s2->f00;
    D_800AC5D8 = a0;
    s1 = (a0 == 1);
    func_800912B4(D_800A3BA4[a0], D_800A3BB4[a0]);
    s5 = (a0 == 1);
    func_800912D4(D_800A3BC4[a0]);
    s2->f6C = D_800A3BE0[a0];
    v1 = D_800A3BF0[a0];
    if (s1) {
        v1 += D_800F4CE0;
    }
    s2->f6E = v1;
    s2->f70 = D_800A3C00[a0];
    v1 = D_800A3C10[a0];
    if (s1) {
        v1 += D_800F4CE0;
    }
    s2->f72 = v1;
    p = &D_800A3C20[D_800F4BF0][a0];
    if (!s5) {
        s2->f64 = 0;
    } else {
        s2->f64 = 1;
        a0_val = D_800F4CE0;
        if (a0_val < 0) {
            a0_val = 0;
        }
        p->f2 = a0_val;
        a2_val = s2->f72;
        /* ASM_BARRIER: Prevent combine from folding lhu into lh + duplicate lhu. */
        __asm__("" : "=r"(a2_val) : "0"(a2_val));
        if ((s16)a0_val < (s16)a2_val) {
            a0_val = a2_val - a0_val;
        } else {
            a0_val = 0;
        }
        p->f6 = a0_val;
        v1 = p->f2;
        if (D_800F4BF0 != 0) {
            v1 += 0x100;
        }
        p->f2 = v1;
    }
    if (a0 & 1) {
        s2->f04 = D_800ACC14 + 0xB70;
    } else {
        s2->f04 = D_800ACC14 + 0x70;
    }
    SetDrawArea(s3);
    AddPrim(s2->f04 + 0xAFC, s3);
    s3 += 0xC;
    G->f00 = s3;
}

int func_8006F188(int *a0, int a1) {
    if (D_8010718C == 0) {
        return func_80026998(a0[0], a0[2]);
    }
    return D_800ABEF8[a1].wF0;
}
