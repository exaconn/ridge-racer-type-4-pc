#include "types.h"
extern struct { char pad[0x19D0]; s32 a; char p1[4]; s32 b, c; char p2[8]; s16 arr[8]; } D_800F4DB8;
void func_8004F2A0(void) {
    s16 m = -1;
    int i = 7;
    D_800F4DB8.a = 0;
    D_800F4DB8.b = 0;
    D_800F4DB8.c = 0;
    do { D_800F4DB8.arr[i] = m; i--; } while (i >= 0);
}
