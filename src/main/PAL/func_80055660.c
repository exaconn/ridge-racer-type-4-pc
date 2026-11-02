#include "types.h"
extern int func_80055358();
extern int D_800F4BF0;
extern struct { char pad[0xD4]; s32 a, b, c, d; u8 e[4]; } D_800AC650;
void func_80055660(void) {
    if (D_800AC650.a != 0) {
        func_80055358(D_800F4BF0, D_800AC650.b, D_800AC650.e, D_800AC650.c, D_800AC650.d);
    }
}
