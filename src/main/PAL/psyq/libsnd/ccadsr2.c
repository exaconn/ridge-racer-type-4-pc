#include "rr/library.h"
#include "psyq/libsnd/cc_nrpn3.h"
void func_80081174(a0, a1, a2, s, c) s16 a0; s16 a1; s16 a2; RrLibBlock24Fields s; u8 c; {
    register s32 cc = c;
    s16 loc[12];
    func_800850C4(a0, a1, a2, &s);
    func_80080F04(s.h10, s.h12, loc);
    
    loc[2] = cc;
    func_80080F60(loc, &s.h10, &s.h12);
    func_800867E4(a0, a1, a2, &s);
}
