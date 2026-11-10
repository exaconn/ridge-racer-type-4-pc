#include "rr/sequencer_init.h"
#include "psyq/libsnd/ssresume.h"
extern s32 _SsVmSeqKeyOff();
void _SsSndPause(s32 a0, s32 a1) {
    s32 t = a0 << 16;
    LibStateB0 **b = _ss_score;
    LibStateB0 **q = (LibStateB0 **)((s32)b + (t >> 14));
    s32 o = (s16)a1 * 0xB0;
    s32 s = a1 << 8;
    LibStateB0 *p = (LibStateB0 *)((char *)*q + o);
    _SsVmSeqKeyOff((s16)(a0 | s), s);
    p->b14 = 0;
    ((LibStateB0 *)(o + (s32)*q))->w98 &= ~2;
}
