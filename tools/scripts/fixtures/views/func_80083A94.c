#include "rr/library.h"

extern RrLibStateB0 *D_8010F068[];

void func_80083A94(s32 a0, s16 a1) {
    s32 t = a0 << 16;
    RrLibStateB0 **b = D_8010F068;
    RrLibStateB0 **q = (RrLibStateB0 **)((s32)b + (t >> 14));
    s32 o = a1 * 0xB0;
    ((RrLibStateB0 *)((char *)*q + o))->b14 = 1;
    ((RrLibStateB0 *)(o + (s32)*q))->w98 &= ~8;
}
