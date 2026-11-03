#include "rr/menu_selection.h"

extern int func_80020074();

s32 func_80032E9C(s32 a0);
void func_80032EC8();

s32 func_80032E9C(s32 a0) {
    D_800AD522 = a0 >= 8;
    return D_8009F64C[a0 & 7];
}

void func_80032EC8(a0, a1, a2, a3, a4) int a0, a1, a2, a3, a4;
{
    int m = (a4 << 5) % 256;
    unsigned short s = (a4 << 6) + 0x3828;
    func_80020074(a0, a1, a2, a3, 0x20, 0x40, m, 0xC0, 0x9A, s, 0, 0x80);
}
