#include "types.h"
extern struct { char pad[0x269C]; s32 a; char pad2[0x50]; s32 b; } D_800F4DB8;
int func_800534B4(void) {
    if (D_800F4DB8.a == 1) return D_800F4DB8.b;
    return -1;
}
