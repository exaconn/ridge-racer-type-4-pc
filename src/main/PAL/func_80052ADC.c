#include "types.h"
extern int func_80083CD4();
extern u8 D_800AD558;
extern struct { char pad[0x26C4]; u16 v; } D_800F4DB8;
void func_80052ADC(u16 a0) {
    s16 s;
    if (D_800F4DB8.v != a0) {
        D_800F4DB8.v = (u8)a0;
        s = (D_800F4DB8.v * D_800AD558) / 16;
        func_80083CD4(0, s, s);
    }
}
