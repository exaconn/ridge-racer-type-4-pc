#include "rr/wide.h"
#include "psyq/other/dpadd.h"
#include "psyq/other/dpsub.h"
typedef union { double d; s64 bits; Wide64 words; } DW;
/* Soft-float subtraction a - b: returns a for b == +-0, -b for a == +-0, else a + (-b). */
s64 __subdf3(s64 a_in, s64 b_in) {
    DW a, b, x;
    u32 sign;
    a.bits = a_in;
    b.bits = b_in;
    x.d = a.d;
    if ((b.words.hi & 0x7FFFFFFF) == 0 && b.words.lo == 0) return a.bits;
    if ((x.words.hi & 0x7FFFFFFF) != 0) goto add;
    sign = 0x80000000;
    if (x.words.lo != 0) goto add_sign;
    return __negdf2(b.bits);
add:
    sign = 0x80000000;
add_sign:
    b.words.hi ^= sign;
    return __adddf3(a.bits, b.bits);
}
