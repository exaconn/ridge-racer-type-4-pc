#ifndef RR_WIDE_H
#define RR_WIDE_H

#include "rr/layout.h"

/* Two-word value used by the software arithmetic helpers.
 * The explicit result pointer mirrors the PAL structure-return ABI: a0 in, v0 out. */
typedef struct {
    u32 lo;
    s32 hi;
} Wide64;

Wide64 *_add_mant_d(Wide64 *result, Wide64 left, Wide64 right);

/* Repeated logical one-bit shift: direction 0 is left, nonzero is right;
 * nonpositive counts copy the input unchanged. Returns the output pointer. */
Wide64 *_dbl_shift_us(Wide64 *result, s32 direction, Wide64 value, s32 count);

/* Compare both two-word inputs as unsigned 64-bit values: -1, 0, or 1. */
s32 _comp_mant(Wide64 left, Wide64 right);

/* Two's-complement negation modulo 2^64; returns the output pointer. */
Wide64 *_mainasu(Wide64 *result, Wide64 value);

/* Unsigned 32-by-32 multiplication, returning both product words. */
Wide64 *_mul_mant_d(Wide64 *result, u32 left, u32 right);

/* Signed integer to IEEE binary64 bits, returned as the PAL v0/v1 pair. */
u64 __floatsidf(s32 value);

/* Toggle the binary64 sign bit; both zero encodings produce positive zero. */
u64 __negdf2(u64 bits);

RR_ASSERT_SIZE(Wide64, 8);
RR_ASSERT_OFFSET(Wide64, hi, 4);

#endif
