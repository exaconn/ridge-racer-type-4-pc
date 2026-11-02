#include "rr/library.h"
extern void func_800850C4(), func_800867E4(), func_80080F60();
void func_800815D4(s16 a0, s16 a1, s16 a2, RrLibBlock24Fields s, s32 c) {
    s16 loc[12];
    func_800850C4(a0, a1, a2, &s);
    s.b[9] = c;
    func_80080F60(loc, &s.h10, &s.h12);
    func_800867E4(a0, a1, a2, &s);
}
