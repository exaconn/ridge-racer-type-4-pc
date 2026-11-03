#include "types.h"

extern struct { char pad[0x19BC]; s16 a[2]; s16 b[2]; s32 c, d; } D_800F4DB8;

void func_800514FC(void);
void func_80051538(int a0, s16 a1, s16 a2);

void func_800514FC(void) {
    int i;
    for (i = 0; i < 2; i++) {
        D_800F4DB8.a[i] = 0;
        D_800F4DB8.b[i] = 0;
    }
    D_800F4DB8.c = 0;
    D_800F4DB8.d = 5;
}

void func_80051538(int a0, s16 a1, s16 a2) {
    D_800F4DB8.a[a0] = a1;
    D_800F4DB8.b[a0] = a2;
}
