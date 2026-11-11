#include "rr/wide.h"
u64 __floatsidf(s32 a0)
{
    unsigned s3 = 0;
    u32 s2;
    union { u64 bits; Wide64 s; } r;
    Wide64 t;
    s2 = 0x41D;
    if (a0 == 0) {
        r.s.lo = 0;
        r.s.hi = 0;
    } else if (a0 == (-2147483647 - 1)) {
        r.s.hi = 0xC1E00000;
        r.s.lo = 0;
    } else {
        if (a0 < 0) {
            s3 = 0x80000000;
            a0 = -a0;
        }
        while (a0 <= 0xFFFFFF) {
            a0 = (a0 * 16U);
            s2 -= 4;
        }
        while (a0 <= 0x3FFFFFFF) {
            a0 = (a0 * 2U);
            s2 -= 1;
        }
        t.hi = a0;
        t.lo = 0;
        _dbl_shift_us(&t, 1, t, 10);
        t.hi &= 0xFFEFFFFF;
        r.s.hi = t.hi | s3;
        r.s.hi |= s2 << 20;
        r.s.lo = t.lo;
    }
    return r.bits;
}
