#include "types.h"
extern int func_80085AEC();extern s16 D_800A043C[];
extern struct { char p0[0x19F8]; s16 x19F8; s16 x19FA; s32 x19FC; s32 x1A00; s32 x1A04; } D_800F4DB8;
void func_800517F8(int a0, int a1, int a2, int a3) {
    int x = D_800A043C[(s16)a3] * (s16)a2;
    int y = D_800A043C[(s16)(0x7F - a3)] * (s16)a2;
    D_800F4DB8.x19FC = a1;
    D_800F4DB8.x19F8 = y / 16129;
    D_800F4DB8.x19FA = x / 16129;
    if (a0 != D_800F4DB8.x1A04) {
        D_800F4DB8.x1A04 = a0;
        D_800F4DB8.x1A00 = 0;
    }
    if (a2 == 0) {
        func_80085AEC(6);
        D_800F4DB8.x1A00 = 0;
    }
}
