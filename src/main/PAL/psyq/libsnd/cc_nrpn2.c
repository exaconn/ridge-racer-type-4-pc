#include "rr/library.h"
#include "rr/spu_tone_snapshot.h"
#include "rr/spu_tone_update.h"
void _SsSetNrpnVabAttr3(s16 a0, s16 a1, s16 a2, RrLibBlock24 s, s32 c) {
    SsUtGetVagAtr(a0, a1, a2, &s);
    s.b[7] = c;
    SsUtSetVagAtr(a0, a1, a2, &s);
}
