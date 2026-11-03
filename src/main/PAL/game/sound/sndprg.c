#include "types.h"

extern struct { char pad[0x26]; s16 h; char pad2[0x195C - 0x28]; struct { s32 a, b, c; char p[16]; } e[2]; } D_800F4DB8;

void func_80050E58(void);
void func_80050E94(int a0, int a1, int a2, int a3);

void func_80050E58(void) {
    int i;
    for (i = 0; i < 2; i++) {
        D_800F4DB8.e[i].c = 0;
        D_800F4DB8.e[i].a = D_800F4DB8.h;
        D_800F4DB8.e[i].b = i + 12;
    }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char pad[0x1964]; struct { s32 a, pad, b, c; char pad2[12]; } e[1]; } (*))&D_800F4DB8)

void func_80050E94(int a0, int a1, int a2, int a3) {
    D_800F4DB8.e[a3].a = a0;
    if (a0 != 0) {
        D_800F4DB8.e[a3].b = a1;
        D_800F4DB8.e[a3].c = a2;
    }
}

#undef D_800F4DB8
