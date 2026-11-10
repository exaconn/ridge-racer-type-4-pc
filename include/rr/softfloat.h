#ifndef RR_SOFTFLOAT_H
#define RR_SOFTFLOAT_H

#include "rr/wide.h"

/* This older shift helper returns its two-word structure by value. The PAL
 * structure-result pointer is implicit; native callers retain the real C ABI. */
Wide64 _dbl_shift(s32 direction, Wide64 value, s32 count);
s32 _err_math(s32 category, s32 operation);

/* Binary64 bit encoding to signed32. Both zero encodings produce zero;
 * exponents above1053 report(34,17) and saturate according to the sign bit.
 * The original encoding-based behavior is retained for infinities and NaNs. */
s32 __fixdfsi(u64 bits);

/* Encoding comparison: -1 when left precedes right, otherwise0. Both signed
 * zeros compare equal; NaN payloads follow the original magnitude ordering. */
s32 __ltdf2(u64 left, u64 right);

#endif
