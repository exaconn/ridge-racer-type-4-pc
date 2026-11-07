#include "rr/library.h"

extern RrLibStateB0 *_ss_score[];

extern s32 _SsVmSeqKeyOff();

void _SsSndPause(s32 a0, s32 a1) {
    s32 t = a0 << 16;
    RrLibStateB0 **b = _ss_score;
    RrLibStateB0 **q = (RrLibStateB0 **)((s32)b + (t >> 14));
    s32 o = (s16)a1 * 0xB0;
    s32 s = a1 << 8;
    RrLibStateB0 *p = (RrLibStateB0 *)((char *)*q + o);
    _SsVmSeqKeyOff((s16)(a0 | s), s);
    p->b14 = 0;
    ((RrLibStateB0 *)(o + (s32)*q))->w98 &= ~2;
}
