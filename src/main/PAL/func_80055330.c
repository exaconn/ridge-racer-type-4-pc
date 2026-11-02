#include "types.h"
extern struct { char pad[0xDC]; s32 a; char pad2[4]; u8 b, c, d; } D_800AC650;
void func_80055330(int a0, u8 a1, u8 a2, u8 a3) {
    D_800AC650.a = a0;
    D_800AC650.b = a1;
    D_800AC650.c = a2;
    D_800AC650.d = a3;
}
