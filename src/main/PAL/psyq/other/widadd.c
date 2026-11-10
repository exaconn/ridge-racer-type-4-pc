#include "rr/wide.h"
Wide64 *_add_mant_d(Wide64 *result, Wide64 a, Wide64 b) {
    unsigned t[4];
    u32 low;
    /* ASM_BARRIER: Keep the result words in the original copy registers. */
    s32 high;
    t[1] = a.lo >> 16; t[0] = a.lo & 0xFFFF;
    t[3] = b.lo >> 16; t[2] = b.lo & 0xFFFF;
    t[0] = t[0] + t[2];
    if (t[0] & 0x10000) t[1] = t[1] + 1;
    t[1] = t[1] + t[3];
    if (t[1] & 0x10000) a.hi = a.hi + 1U;
    a.lo = (t[1] << 16) | (t[0] & 0xFFFF);
    a.hi = (u32)a.hi + b.hi;
    /* ASM_BARRIER: Reload both parameter words before publishing the result. */
    __asm__ volatile("" : : : "memory");
    low = a.lo;
    high = a.hi;
    result->lo = low;
    result->hi = high;
    /* ASM_BARRIER: Complete the result stores before reusing v0 for the result pointer. */
    __asm__ volatile("" : : "r"(result) : "$2", "$3", "memory");
    return result;
}
