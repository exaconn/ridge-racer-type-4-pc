#include "rr/sequencer_init.h"
#include "psyq/libsnd/ssresume.h"
#include "psyq/libsnd/vmkoff.h"
void _SsSndPause(s32 a0, s32 a1) {
    u32 bankBits = (u32)a0 << 16;
    LibStateB0 **table = _ss_score;
    LibStateB0 **q = &table[(s32)bankBits >> 16];
    LibStateB0 *p = &(*q)[(s16)a1];
    s32 s = (u32)a1 << 8;
    _SsVmSeqKeyOff((s16)(a0 | s));
    p->b14 = 0;
    (*q)[(s16)a1].w98 &= ~2;
}
