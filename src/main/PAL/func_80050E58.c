#include "types.h"
extern struct { char pad[0x26]; s16 h; char pad2[0x195C - 0x28]; struct { s32 a, b, c; char p[16]; } e[2]; } D_800F4DB8;
void func_80050E58(void) {
    int i;
    for (i = 0; i < 2; i++) {
        D_800F4DB8.e[i].c = 0;
        D_800F4DB8.e[i].a = D_800F4DB8.h;
        D_800F4DB8.e[i].b = i + 12;
    }
}
