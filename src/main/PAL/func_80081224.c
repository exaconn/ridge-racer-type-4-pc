#include "rr/library.h"
extern void func_800850C4(), func_800867E4(), func_80080F04(), func_80080F60();
void func_80081224(a0, a1, a2, s, c) s16 a0; s16 a1; s16 a2; RrLibBlock24Fields s; u8 c; {
    register s32 cc = c;
    s16 loc[12];
    func_800850C4(a0, a1, a2, &s);
    func_80080F04(s.h10, s.h12, loc);
    loc[6] = 0;
    loc[3] = cc;
    func_80080F60(loc, &s.h10, &s.h12);
    func_800867E4(a0, a1, a2, &s);
}
