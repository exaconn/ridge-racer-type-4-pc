#include "types.h"
struct T { s32 k[9]; s32 v[9]; };
struct E { char pad[0x74]; struct T t[2][10]; char r[12]; };
extern struct E D_800F4DB8[];
int func_80050744(int a0, int x, int a2, int a3) {
    int i, lo, vlo, vhi, klo, khi;
    for (i = 1; i < 9; i++) {
        if (x < D_800F4DB8[a3].t[a2][a0].k[i]) break;
    }
    lo = i - 1;
    vlo = D_800F4DB8[a3].t[a2][a0].v[lo];
    vhi = D_800F4DB8[a3].t[a2][a0].v[i];
    klo = D_800F4DB8[a3].t[a2][a0].k[lo];
    khi = D_800F4DB8[a3].t[a2][a0].k[i];
    return (vhi - vlo) * (x - klo) / (khi - klo) + vlo;
}
