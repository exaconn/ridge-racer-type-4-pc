#include "rr/library.h"
extern void func_800850C4(), func_800867E4();
void func_80080D34(s16 a0, s16 a1, s16 a2, RrLibBlock24 s, s32 c) {
    func_800850C4(a0, a1, a2, &s);
    s.b[6] = c;
    func_800867E4(a0, a1, a2, &s);
}
