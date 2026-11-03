#include "types.h"
extern struct { char pad[0x1964]; struct { s32 a, pad, b, c; char pad2[12]; } e[1]; } D_800F4DB8;
void func_80050E94(int a0, int a1, int a2, int a3) {
    D_800F4DB8.e[a3].a = a0;
    if (a0 != 0) {
        D_800F4DB8.e[a3].b = a1;
        D_800F4DB8.e[a3].c = a2;
    }
}
