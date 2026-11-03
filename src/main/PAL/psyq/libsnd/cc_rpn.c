#include "rr/library.h"
extern void func_800850C4(), func_800867E4(), func_80080F60();
void func_800814F4(a0, a1, a2, s, c)
    s16 a0, a1, a2;
    RrLibBlock24Fields s;
    s32 c;
{
    s16 loc[12];
    func_800850C4(a0, a1, a2, &s);
    if ((u8)(c - 1) < 0x3F) {
        loc[8] = 0;
    } else if ((u8)(c - 0x40) < 0x40) {
        loc[8] = 1;
    }
    func_80080F60(loc, &s.h10, &s.h12);
    func_800867E4(a0, a1, a2, &s);
}
