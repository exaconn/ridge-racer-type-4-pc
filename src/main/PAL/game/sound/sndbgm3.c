#include "types.h"

struct EE { s32 a, b, c; s16 d, e; s32 f; };

struct EE_1 { s32 a, b, c; s16 d[2]; s32 f; };

extern struct {
    char pad[0x26]; s16 h; char p1[0x1994 - 0x28]; s32 f1;
    struct EE e[2];
} D_800F4DB8;
extern s16 D_800AD55A;

void func_80051248(int a0);
void func_800512C4(int a0, int a1, int a2);

void func_80051248(int a0) {
    int k, i;
    if (a0 == 0) {
        k = 0xB;
        D_800F4DB8.f1 = 1;
        D_800F4DB8.e[0].f = 0;
    } else {
        k = 0xE;
        D_800F4DB8.f1 = 1;
        D_800F4DB8.e[0].f = 1;
    }
    for (i = 0; i < 2; i++) {
        D_800F4DB8.e[i].e = 0;
        D_800F4DB8.e[i].d = 0;
        D_800F4DB8.e[i].a = 0;
        D_800F4DB8.e[i].b = D_800F4DB8.h;
        D_800F4DB8.e[i].c = k + i;
    }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char pad[0x1998]; struct EE_1 e[2]; } (*))&D_800F4DB8)

void func_800512C4(int a0, int a1, int a2) {
    int i;
    if (D_800AD55A == 0) {
        D_800F4DB8.e[a0].d[0] = a1;
        D_800F4DB8.e[a0].d[1] = a2;
    } else {
        D_800F4DB8.e[a0].d[0] = (a1 + a2) / 2;
        D_800F4DB8.e[a0].d[1] = (a1 + a2) / 2;
    }
    for (i = 0; i < 2; i++) {
        if (D_800F4DB8.e[a0].d[i] < 2) D_800F4DB8.e[a0].d[i] = 0;
    }
}

#undef D_800F4DB8
