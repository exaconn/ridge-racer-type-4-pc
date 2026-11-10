#include "rr/library.h"
#include "rr/spu_tone_snapshot.h"
#include "rr/spu_tone_update.h"
#include "psyq/libsnd/utrev3.h"
#include "psyq/libsnd/cc_98.h"
int _SsSetNrpnVabAttr1(a0, a1, a2, s, c) s16 a0; s16 a1; s16 a2; LibBlock24 s; u8 c; {
    SsUtGetVagAtr(a0, a1, a2, &s);
    s.b[1] = c;
    SsUtSetVagAtr(a0, a1, a2, &s);
    if (c == 0) SsUtReverbOff();
    else if (c == 4) SsUtReverbOn();
}
