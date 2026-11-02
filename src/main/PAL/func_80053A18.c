#include "types.h"
extern int func_8008C51C();
extern char D_800F4DB8[];
void func_80053A18(u8 a0) {
    char *b;
    char *q;
    s32 *s;
    if (a0 == 1) {
        b = D_800F4DB8;
        s = (s32 *)(b + 0x8000);
        q = b + 0x28C8;
        func_8008C51C((s[0x28C8 / 4] << 11) + (int)q, 0x20);
        s[0x28CC / 4] = s[0x28C8 / 4];
        s[0x28C8 / 4] = (s[0x28C8 / 4] + 1) & 0xF;
    }
}
