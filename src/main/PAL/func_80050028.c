#include "types.h"
extern struct { u32 flags; char pad[1568 - 4]; } D_800F4DF0[];
void func_80050028(int a0, int a1) {
    u32 *p = &D_800F4DF0[a0].flags;
    if (a1 == 1) *p |= 4;
    else *p &= ~4;
}
