#include "rr/library.h"
#include "psyq/libsnd/cc_nrpn3.h"
void func_80081674(s16 a0, s16 a1, s16 a2, RrLibBlock24Fields s, s32 c) {
    s16 loc[12];
    func_800850C4(a0, a1, a2, &s);
    s.b[10] = c;
    func_80080F60(loc, &s.h10, &s.h12);
    func_800867E4(a0, a1, a2, &s);
}
