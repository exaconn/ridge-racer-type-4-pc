#include "rr/wide.h"
u64 func_8007E1B4(s32 a0)
{
    unsigned s3 = 0;
    int s2;
    union { u64 bits; RrWide64 s; } r;
    RrWide64 t;
    /* ASM_BARRIER: Keep the sign accumulator distinct from literal zero results. */
    __asm__("" : "+r"(s3));
    s2 = 0x41D;
    if (a0 == 0) {
        r.s.lo = 0;
        /* ASM_BARRIER: Keep the two literal zero words independent during result construction. */
        __asm__("" : "+r"(r.s.lo));
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
            a0 = (s32)((u32)a0 << 4);
            s2 -= 4;
        }
        while (a0 <= 0x3FFFFFFF) {
            a0 = (s32)((u32)a0 << 1);
            s2 -= 1;
        }
        t.hi = a0;
        t.lo = 0;
        func_8007DFF4(&t, 1, t, 10);
        t.hi &= 0xFFEFFFFF;
        r.s.hi = t.hi | s3;
        r.s.hi |= (u32)s2 << 20;
        r.s.lo = t.lo;
    }
    return r.bits;
}
