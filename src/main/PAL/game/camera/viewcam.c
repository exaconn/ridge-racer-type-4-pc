#include "types.h"
#include "m2c_macros.h"

struct S8 { int w[8]; };

extern int func_8006E300();
extern int func_80090B14();
extern int func_80090F74();
extern int func_8006E9D4();
extern int D_8010E690;
extern int func_80091144();
extern int func_800910B4();
extern struct S8 D_800FF690, D_800F39A8;
extern int func_8006EED8();
extern int func_80090594();
extern int func_800910E4();
extern int func_80091114();
extern int func_80091274();
extern int func_80091294();
extern s32 D_800ABE98;
extern M2C_UNK D_800ABED0;
extern M2C_UNK D_800AD570;
extern s32 D_800F4D48;
extern s32 D_800F4D68;
extern s32 D_800F4D84;
extern s32 D_80107194;
extern s32 D_801093F8;

void func_8006E9E8(int a0, int *a1);
void func_8006EAC8(void);
void func_8006EC64(void);

void func_8006E9E8(int a0, int *a1) {
    int v[4], w[4], x[4];
    int *g = (int *)0x1F80007C;
    if (a0) {
        func_8006E300();
        v[0] = (0x8000 - a1[0]) << 2;
        v[1] = (0x60 - a1[1]) << 2;
        v[2] = (0x8000 - a1[2]) << 2;
        func_80090B14(0x1F800108, v, w);
        x[2] = 0x4000;
        x[1] = 0x4000;
        x[0] = 0x4000;
        g[0x28] = w[0];
        g[0x29] = w[1];
        g[0x2A] = w[2];
        func_80090F74(0x1F800108, x);
        func_8006E9D4(D_8010E690, a0, a1);
    }
}

void func_8006EAC8(void) {
    int v[4], w[4], x[4];
    int *h = (int *)0x1F800000;
    int *g = (int *)0x1F800008;
    v[0] = 0x8000 - (g[0] >> 2);
    v[1] = -(g[1] >> 2);
    v[2] = 0x8000 - (g[2] >> 2);
    func_80090B14(0x1F800028, v, w);
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
    func_80090B14(0x1F800028, v, w);
    h[15] = w[0];
    h[16] = w[1];
    h[17] = w[2];
    func_80091144(0x1F800028);
    x[2] = 0x4000;
    x[1] = 0x4000;
    x[0] = 0x4000;
    func_80090F74(0x1F800028, x);
    func_800910B4(0x1F800028);
}

void func_8006EC64(void) {
    func_80091114(&D_800AD570);
    func_8006EED8();
    func_800910E4(&D_800ABED0);
    func_80091274(D_800ABE98, D_801093F8, D_80107194);
    func_80091294(D_800F4D84, D_800F4D68, D_800F4D48);
    func_80090594(0x4E20, 0x122);
}
