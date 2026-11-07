#include "rr/library.h"
#include "psyq/libsnd/cc_nrpn3.h"
void _SsSetNrpnVabAttr14(s16 a0, s16 a1, s16 a2, RrLibBlock24Fields s, s32 c) {
    s16 loc[12];
    SsUtGetVagAtr(a0, a1, a2, &s);
    s.b[10] = c;
    _SsUtBuildADSR(loc, &s.h10, &s.h12);
    SsUtSetVagAtr(a0, a1, a2, &s);
}
