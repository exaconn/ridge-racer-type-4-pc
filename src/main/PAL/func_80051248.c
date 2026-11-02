#include "types.h"
struct EE { s32 a, b, c; s16 d, e; s32 f; };
extern struct {
    char pad[0x26]; s16 h; char p1[0x1994 - 0x28]; s32 f1;
    struct EE e[2];
} D_800F4DB8;
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
