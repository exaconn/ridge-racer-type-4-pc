#include "rr/library.h"

extern LibStateB0 *_ss_score[];

void _SsSndNextSep(s32 a0, s16 a1) {
    s32 t = a0 << 16;
    LibStateB0 **b = _ss_score;
    LibStateB0 **q = (LibStateB0 **)((s32)b + (t >> 14));
    s32 o = a1 * 0xB0;
    LibStateB0 *p = (LibStateB0 *)((char *)*q + o);
    p->b20 = 1;
    p->b21 = 0;
    ((LibStateB0 *)(o + (s32)*q))->w98 &= ~0x100;
    ((LibStateB0 *)(o + (s32)*q))->w98 &= ~8;
    ((LibStateB0 *)(o + (s32)*q))->w98 &= ~2;
    ((LibStateB0 *)(o + (s32)*q))->w98 &= ~4;
    ((LibStateB0 *)(o + (s32)*q))->w98 &= ~0x200;
    p->b14 = 1;
    p->c0 = (u8 *)p->c4;
    ((LibStateB0 *)(o + (s32)*q))->w98 |= 1;
}
