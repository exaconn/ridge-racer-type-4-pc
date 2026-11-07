#include "rr/library.h"

extern RrLibStateB0 *_ss_score[];

void _SsSndReplay(s32 a0, s16 a1) {
    s32 t = a0 << 16;
    RrLibStateB0 **b = _ss_score;
    RrLibStateB0 **q = (RrLibStateB0 **)((s32)b + (t >> 14));
    s32 o = a1 * 0xB0;
    ((RrLibStateB0 *)((char *)*q + o))->b14 = 1;
    ((RrLibStateB0 *)(o + (s32)*q))->w98 &= ~8;
}
