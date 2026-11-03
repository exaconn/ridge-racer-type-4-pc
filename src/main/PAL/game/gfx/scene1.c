#include "types.h"
extern struct { char pad[0xD4]; s32 a, b, c, d; u8 e, f, g; } D_800AC650;
void func_80055300(void) {
    D_800AC650.d = 1;
    D_800AC650.a = 0;
    D_800AC650.b = 0;
    D_800AC650.c = 0;
    D_800AC650.e = 0x90;
    D_800AC650.f = 0x90;
    D_800AC650.g = 0x90;
}
