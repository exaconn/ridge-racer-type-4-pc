/* GCC_VERSION: 2.7.2 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/wide.h"
extern u64 func_8007D7D4(u64 a, u64 b);
typedef union { double d; u64 bits; RrWide64 words; } DW;
/* Soft-float subtraction a - b: returns a for b == +-0, -b for a == +-0, else a + (-b). */
u64 func_8007E944(DW a, DW b) {
    DW x;
    u32 sign;
    x.d = a.d;
    if ((b.words.hi & 0x7FFFFFFF) == 0 && b.words.lo == 0) return a.bits;
    if ((x.words.hi & 0x7FFFFFFF) != 0) goto add;
    sign = 0x80000000;
    /* ASM_BARRIER: Keep the low-word branch from taking the sign constant, so the high-word branch delay slot gets it. */
    __asm__ volatile("");
    if (x.words.lo != 0) goto add_sign;
    return func_8007E904(b.bits);
add:
    sign = 0x80000000;
add_sign:
    b.words.hi ^= sign;
    return func_8007D7D4(a.bits, b.bits);
}
