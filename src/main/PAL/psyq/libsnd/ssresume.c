#include "rr/library.h"
#include "psyq/libsnd/ssresume.h"
extern RrLibStateB0 *D_8010F068[];
extern s32 func_80089E1C();
void func_800831D4(s32 a0, s32 a1) {
    s32 t = a0 << 16;
    RrLibStateB0 **b = D_8010F068;
    RrLibStateB0 **q = (RrLibStateB0 **)((s32)b + (t >> 14));
    s32 o = (s16)a1 * 0xB0;
    s32 s = a1 << 8;
    RrLibStateB0 *p = (RrLibStateB0 *)((char *)*q + o);
    func_80089E1C((s16)(a0 | s), s);
    p->b14 = 0;
    ((RrLibStateB0 *)(o + (s32)*q))->w98 &= ~2;
}
