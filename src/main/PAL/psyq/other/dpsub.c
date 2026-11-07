/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/wide.h"
#include "psyq/other/dpadd.h"
typedef union { double d; u64 bits; RrWide64 words; } DW;
/* Soft-float subtraction a - b: returns a for b == +-0, -b for a == +-0, else a + (-b). */
u64 __subdf3(DW a, DW b) {
    DW x;
    u32 sign;
    x.d = a.d;
    if ((b.words.hi & 0x7FFFFFFF) == 0 && b.words.lo == 0) return a.bits;
    if ((x.words.hi & 0x7FFFFFFF) != 0) goto add;
    sign = 0x80000000;
    if (x.words.lo != 0) goto add_sign;
    return func_8007E904(b.bits);
add:
    sign = 0x80000000;
add_sign:
    b.words.hi ^= sign;
    return __adddf3(a.bits, b.bits);
}
