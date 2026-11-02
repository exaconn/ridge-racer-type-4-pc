#include "rr/geometry.h"
#include "types.h"

s32 func_8005602C(RrPairS16 *t, s32 idx, s32 x) {
    RrPairS16 *p;
    if (idx == 0) return t[0].a;
    p = (RrPairS16 *)(idx * 4 + (s32)t);
    if (p->b < 0) return p->a;
    return (p->a - p[-1].a) * (x - p[-1].b * 10) / ((p->b - p[-1].b) * 10) + p[-1].a;
}
