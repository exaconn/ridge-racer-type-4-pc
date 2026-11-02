#include "types.h"
extern struct { char pad[0x19BC]; s16 a[2]; s16 b[2]; } D_800F4DB8;
void func_80051538(int a0, s16 a1, s16 a2) {
    D_800F4DB8.a[a0] = a1;
    D_800F4DB8.b[a0] = a2;
}
