#include "rr/library.h"
#include "psyq/libsnd/cc_nrpn3.h"
#include "psyq/libsnd/ccadsr4.h"

void func_80081394(a0, a1, a2, s, c) s16 a0; s16 a1; s16 a2; RrLibBlock24Fields s; u8 c; {
    register s32 cc = c;
    s16 loc[12];
    func_800850C4(a0, a1, a2, &s);
    func_80080F04(s.h10, s.h12, loc);
    loc[7] = 0;
    loc[4] = cc;
    func_80080F60(loc, &s.h10, &s.h12);
    func_800867E4(a0, a1, a2, &s);
}

void func_80081444(a0, a1, a2, s, c) s16 a0; s16 a1; s16 a2; RrLibBlock24Fields s; u8 c; {
    register s32 cc = c;
    s16 loc[12];
    func_800850C4(a0, a1, a2, &s);
    loc[7] = 1;
    loc[4] = cc;
    func_80080F60(loc, &s.h10, &s.h12);
    func_800867E4(a0, a1, a2, &s);
}
