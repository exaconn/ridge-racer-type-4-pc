#include "types.h"
extern struct { char pad[0x266C]; s32 a, b, c, d; } D_800F4DB8;
void func_80052D50(int a0) {
    D_800F4DB8.a = a0;
    D_800F4DB8.b = 6;
    D_800F4DB8.d = 0;
}
