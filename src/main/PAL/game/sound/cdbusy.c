#include "types.h"
extern struct { char pad[0x2674]; s32 a, b; char p[4]; s32 c; } D_800F4DB8;
int func_80052D6C(void) {
    if (D_800F4DB8.a != 0 || D_800F4DB8.b != 0) return 1;
    return D_800F4DB8.c;
}
