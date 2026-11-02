#include "types.h"
extern struct { char pad[0x18]; s16 a, b, c, d, e, f; } D_800F4DB8;
void func_8004FBF0(void) {
    D_800F4DB8.a = 0x80;
    D_800F4DB8.b = 0;
    D_800F4DB8.c = 0;
    D_800F4DB8.d = 0;
    D_800F4DB8.e = 0;
    D_800F4DB8.f = 0;
}
