#include "rr/library.h"
extern void func_800850C4();
extern void func_800867E4();
void func_80080DC4(s16 a0, s16 a1, s16 a2, RrLibBlock24 s, s32 c) {
    func_800850C4(a0, a1, a2, &s);
    s.b[7] = c;
    func_800867E4(a0, a1, a2, &s);
}
