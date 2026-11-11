/* libgte CordicSqrt: integer square root by hyperbolic CORDIC. PsyZ only
 * provides SquareRoot0/SquareRoot12, whose rounding differs. */
#include "types.h"
#include "psyq/libgte/gtecord.h"

/* GTE LZCS/LZCR: number of leading bits equal to the sign bit. */
static s32 LeadingSignBits(s32 value) {
    u32 bits = value < 0 ? ~(u32)value : (u32)value;
    s32 count = 0;
    if (bits == 0) {
        return 32;
    }
    while ((bits & 0x80000000u) == 0) {
        bits <<= 1;
        count++;
    }
    return count;
}

/* Six hyperbolic iterations on a mantissa scaled into [2^22, 2^24);
 * iteration four is repeated, as hyperbolic CORDIC requires. */
static s32 csqrt_1(s32 value) {
    s32 x = value + 0x5D50AD;
    s32 y = value - 0x5D50AD;
    s32 previous;
    s32 i;
    for (i = 1; i < 7; i++) {
        s32 repeat = i == 4 ? 2 : 1;
        while (repeat-- > 0) {
            previous = x;
            if (y >= 0) {
                x -= y >> i;
                y -= previous >> i;
            } else {
                x += y >> i;
                y += previous >> i;
            }
        }
    }
    return x;
}

s32 CordicSqrt(s32 value) {
    s32 shift;
    s32 mantissa;
    s32 exponent;
    if (value == 0) {
        return 0;
    }
    shift = 8 - LeadingSignBits(value);
    if (shift >= 0) {
        exponent = shift >> 1;
        mantissa = value >> (exponent * 2);
    } else {
        exponent = (shift >> 1) + 1;
        mantissa = (s32)((u32)value << -(exponent * 2));
    }
    exponent -= 6;
    if (exponent >= 0) {
        return (s32)((u32)csqrt_1(mantissa) << exponent);
    }
    return csqrt_1(mantissa) >> -exponent;
}
