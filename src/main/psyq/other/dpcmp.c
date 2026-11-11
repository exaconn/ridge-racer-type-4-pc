#include "rr/softfloat.h"
#include "psyq/other/dpcmp.h"

s32 __gtdf2(u64 left, u64 right) {
    union {u64 bits; Wide64 words;} a, b;
    u32 magnitude = 0x7FFF0000;
    s32 leftExponent;
    s32 rightFraction;
    s32 leftSign;
    s32 rightExponent;
    s32 leftFraction;
    s32 result;
    s32 highA, highB;

    a.bits = left; b.bits = right; highA = a.words.hi; highB = b.words.hi;
    if ((a.words.hi != b.words.hi) || ({
        result = 0;
        a.words.lo != b.words.lo;
    })) {
        if (((highA & (magnitude | 0xFFFF)) == 0) && (a.words.lo == 0) && ((highB & (magnitude | 0xFFFF)) == 0) && (b.words.lo == 0)) {
            result = 0;
            goto return_result;
        }
        leftSign = a.words.hi & 0x80000000;
        if (leftSign != (b.words.hi & 0x80000000)) {
            result = leftSign < 1U;
            goto return_result;
        }
        leftExponent = (a.words.hi >> 0x14) & 0x7FF;
        rightExponent = (b.words.hi >> 0x14) & 0x7FF;
        {
            s32 zero = 0;
            if ((leftExponent > rightExponent) ||
                ((leftExponent == rightExponent) &&
                 ((leftFraction = (a.words.hi & 0xFFFFF) | 0x100000,
                   rightFraction = (b.words.hi & 0xFFFFF) | 0x100000,
                   leftFraction > rightFraction) ||
                  ((leftFraction == rightFraction) && (a.words.lo > b.words.lo))))) {
                result = ({
                    u32 m = 0x80000000;
                    highA & m;
                }) < 1;
                goto return_result;
            }
            result = a.words.hi < zero;
        }
    }
return_result:
    return result;
}
