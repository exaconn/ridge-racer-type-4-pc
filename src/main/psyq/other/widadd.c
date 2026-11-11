#include "rr/wide.h"
Wide64 *_add_mant_d(Wide64 *result, Wide64 a, Wide64 b) {
    unsigned t[4];
    u32 low;
    s32 high;
    t[1] = a.lo >> 16; t[0] = a.lo & 0xFFFF;
    t[3] = b.lo >> 16; t[2] = b.lo & 0xFFFF;
    t[0] = t[0] + t[2];
    if (t[0] & 0x10000) t[1] = t[1] + 1;
    t[1] = t[1] + t[3];
    if (t[1] & 0x10000) a.hi = a.hi + 1U;
    a.lo = (t[1] << 16) | (t[0] & 0xFFFF);
    a.hi = (u32)a.hi + b.hi;
    low = a.lo;
    high = a.hi;
    result->lo = low;
    result->hi = high;
    return result;
}
