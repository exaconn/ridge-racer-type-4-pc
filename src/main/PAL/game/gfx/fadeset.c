#include "types.h"
#include "game/gfx/fadeset.h"
extern struct { char pad[8]; s32 a, b, c, d, e; } D_800F2AB8;
void func_80054D30(int a0, int a1, int a2) {
    D_800F2AB8.a = a0;
    D_800F2AB8.b = a0;
    D_800F2AB8.c = a1;
    D_800F2AB8.d = a2;
    D_800F2AB8.e = a1;
}
