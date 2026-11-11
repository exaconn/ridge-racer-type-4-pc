#include "rr/softfloat.h"
#include "psyq/other/dpadd.h"

typedef union {
    double d;
    s64 i;
    Wide64 s;
} DpUnion;

s64 __adddf3(s64 leftBits, s64 rightBits) {
    Wide64 leftMantissa;
    Wide64 rightMantissa;
    Wide64 rounding;
    DpUnion zeroResult;
    DpUnion left;
    DpUnion right;
    s64 incomingRight = rightBits;
    DpUnion result;
    s32 exponent;
    s32 rightExponent;
    u32 sign;

    zeroResult.s.lo = 0;
    zeroResult.s.hi = 0;
    sign = 0;
    left.i = leftBits;
    right.i = incomingRight;

    if ((left.s.hi & 0x7FFFFFFF) == 0 && left.s.lo == 0) {
        return incomingRight;
    }
    if ((right.s.hi & 0x7FFFFFFF) == 0 && right.s.lo == 0) {
        goto return_left;
    }

    exponent = (left.s.hi >> 20) & 0x7FF;
    rightExponent = (right.s.hi >> 20) & 0x7FF;

    if (rightExponent + 0x36 < exponent) {
        goto return_left;
    }
    if (exponent + 0x36 < rightExponent) {
        return right.i;
    }

    leftMantissa.hi = (left.s.hi & 0xFFFFF) | 0x100000;
    leftMantissa.lo = left.s.lo;
    rightMantissa.hi = (right.s.hi & 0xFFFFF) | 0x100000;
    rightMantissa.lo = right.s.lo;

    if (left.s.hi & 0x80000000) {
        _mainasu(&leftMantissa, leftMantissa);
    }
    if (right.s.hi & 0x80000000) {
        _mainasu(&rightMantissa, rightMantissa);
    }

    leftMantissa = _dbl_shift(0, leftMantissa, 9);
    rightMantissa = _dbl_shift(0, rightMantissa, 9);

    if (rightExponent < exponent) {
        rightMantissa = _dbl_shift(1, rightMantissa, exponent - rightExponent);
    } else {
        leftMantissa = _dbl_shift(1, leftMantissa, rightExponent - exponent);
        exponent = rightExponent;
    }

    _add_mant_d(&leftMantissa, leftMantissa, rightMantissa);

    if (leftMantissa.hi < 0) {
        _mainasu(&leftMantissa, leftMantissa);
        sign = 0x80000000;
    } else if (leftMantissa.hi == 0 && leftMantissa.lo == 0) {
        return zeroResult.i;
    }

    {
        s32 topBits;
        s32 highWord = leftMantissa.hi;
        topBits = 0xE0000000;
        if ((highWord & topBits) == 0) {
            s32 normalizationMask = 0xE0000000;
            do {
                leftMantissa = _dbl_shift(0, leftMantissa, 1);
                exponent--;
            } while ((leftMantissa.hi & normalizationMask) == 0);
        }
    }

    if (leftMantissa.hi & 0x40000000) {
        leftMantissa = _dbl_shift(1, leftMantissa, 1);
        exponent++;
    }

    rounding.hi = 0;
    rounding.lo = (leftMantissa.lo & 0x200) ? 0x100 : 0xFF;
    _add_mant_d(&leftMantissa, leftMantissa, rounding);

    if (leftMantissa.hi & 0x40000000) {
        leftMantissa = _dbl_shift(1, leftMantissa, 1);
        exponent++;
    }

    leftMantissa = _dbl_shift(1, leftMantissa, 9);
    leftMantissa.hi &= 0xFFEFFFFF;

    if (exponent >= 0x7FF) {
        _err_math(0x22, 0xB);
        left.s.hi = sign ? 0xFFF00000 : 0x7FF00000;
        left.s.lo = 0;
    } else {
        left.s.hi = sign | (exponent * 0x100000U) | leftMantissa.hi;
        left.s.lo = leftMantissa.lo;
    }

return_left:
    result.d = left.d;
    return result.i;
}
