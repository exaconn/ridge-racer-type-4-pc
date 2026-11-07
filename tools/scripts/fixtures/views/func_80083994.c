#include "rr/library.h"

extern RrLibStateB0 *_ss_score[];

void _SsSndNextSep(s32 a0, s16 a1) {
    s32 t = a0 << 16;
    RrLibStateB0 **b = _ss_score;
    RrLibStateB0 **q = (RrLibStateB0 **)((s32)b + (t >> 14));
    s32 o = a1 * 0xB0;
    RrLibStateB0 *p = (RrLibStateB0 *)((char *)*q + o);
    p->b20 = 1;
    p->b21 = 0;
    ((RrLibStateB0 *)(o + (s32)*q))->w98 &= ~0x100;
    ((RrLibStateB0 *)(o + (s32)*q))->w98 &= ~8;
    ((RrLibStateB0 *)(o + (s32)*q))->w98 &= ~2;
    ((RrLibStateB0 *)(o + (s32)*q))->w98 &= ~4;
    ((RrLibStateB0 *)(o + (s32)*q))->w98 &= ~0x200;
    p->b14 = 1;
    p->c0 = (u8 *)p->c4;
    ((RrLibStateB0 *)(o + (s32)*q))->w98 |= 1;
}
