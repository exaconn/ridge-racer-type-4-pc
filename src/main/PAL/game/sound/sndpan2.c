#include "types.h"
extern s16 D_800AD55A;
struct EE { s32 a, b, c; s16 d[2]; s32 f; };
extern struct { char pad[0x1998]; struct EE e[2]; } D_800F4DB8;
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
