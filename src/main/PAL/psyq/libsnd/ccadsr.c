#include "rr/library.h"
#include "psyq/libsnd/cc_nrpn3.h"
#include "psyq/libsnd/ccadsr.h"
void _SsSetNrpnVabAttr6(a0, a1, a2, s, c) s16 a0; s16 a1; s16 a2; LibBlock24Fields s; u8 c; {
    s32 cc = c;
    s16 loc[12];
    SsUtGetVagAtr(a0, a1, a2, &s);
    _SsUtResolveADSR(s.h10, s.h12, loc);
    
    loc[1] = cc;
    _SsUtBuildADSR(loc, &s.h10, &s.h12);
    SsUtSetVagAtr(a0, a1, a2, &s);
}
