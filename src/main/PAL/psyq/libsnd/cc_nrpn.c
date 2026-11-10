#include "rr/library.h"
#include "rr/spu_tone_snapshot.h"
#include "rr/spu_tone_update.h"
#include "psyq/libsnd/cc_nrpn.h"
void _SsSetNrpnVabAttr2(s16 a0, s16 a1, s16 a2, LibBlock24 s, s32 c) {
    SsUtGetVagAtr(a0, a1, a2, &s);
    s.b[6] = c;
    SsUtSetVagAtr(a0, a1, a2, &s);
}
