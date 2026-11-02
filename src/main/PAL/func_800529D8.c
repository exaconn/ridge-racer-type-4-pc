#include "types.h"
extern int func_80052ADC();
extern struct { char pad[0x2664]; s32 a; char p0[4]; s32 b, c, d, e; char p1[8]; s32 g; char p3[0x14]; s32 h; char p2[0x26CE - 0x26A0]; u16 i; } D_800F4DB8;
void func_800529D8(int a0, int a1, int a2) {
    D_800F4DB8.a = a0;
    D_800F4DB8.b = a1;
    D_800F4DB8.d = 0;
    if (a2 == 0) D_800F4DB8.g = a0;
    D_800F4DB8.c = -1;
    D_800F4DB8.e = 0;
    if (D_800F4DB8.h == 2) func_80052ADC(D_800F4DB8.i);
}
