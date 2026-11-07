#include "rr/library.h"
#include "psyq/libsnd/cc_nrpn3.h"
#include "psyq/libsnd/cc_rpn.h"
void _SsSetNrpnVabAttr12(a0, a1, a2, s, c)
    s16 a0, a1, a2;
    RrLibBlock24Fields s;
    s32 c;
{
    s16 loc[12];
    SsUtGetVagAtr(a0, a1, a2, &s);
    if ((u8)(c - 1) < 0x3F) {
        loc[8] = 0;
    } else if ((u8)(c - 0x40) < 0x40) {
        loc[8] = 1;
    }
    _SsUtBuildADSR(loc, &s.h10, &s.h12);
    SsUtSetVagAtr(a0, a1, a2, &s);
}
