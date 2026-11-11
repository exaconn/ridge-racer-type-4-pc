#include "rr/softfloat.h"
s32 __ltdf2(u64 left, u64 right) {
    union {u64 bits; Wide64 words;} a,b;
    u32 magnitude=0x7FFF0000;
    s32 leftExponent;
    s32 rightFraction;
    s32 leftSign;
    s32 rightExponent;
    s32 leftFraction;
    s32 result;
    s32 highA,highB;

    a.bits=left;b.bits=right;highA=a.words.hi;highB=b.words.hi;
    if ((a.words.hi != b.words.hi) || ({
        /* Keep the initialized upper mask live through the low-word test. */
        result=0;
        a.words.lo != b.words.lo;
    })) {
        if ((highA & (magnitude|0xFFFF)) || (a.words.lo != 0) || (highB & (magnitude|0xFFFF)) || (b.words.lo != 0)) {
            leftSign = a.words.hi & 0x80000000;
            if (leftSign != (b.words.hi & 0x80000000)) {
                result = -1;
                if (leftSign == 0) {
                    result=0;
                    goto return_result;
                }
                
                goto return_result;
            }
            leftExponent = (a.words.hi >> 0x14) & 0x7FF;
            rightExponent = (b.words.hi >> 0x14) & 0x7FF;
            if ((leftExponent < rightExponent) || ((leftExponent == rightExponent) && ((leftFraction = (a.words.hi & 0xFFFFF) | 0x100000, rightFraction = (b.words.hi & 0xFFFFF) | 0x100000, ((leftFraction < rightFraction) != 0)) || ((leftFraction == rightFraction) && (a.words.lo < b.words.lo))))) {
                result = 0;
                if (a.words.hi >= 0) {
                    result=-1;
                    goto return_result;
                }
                
                goto return_result;
            }
            result = -1;
            if (a.words.hi >= 0) {
                goto equal_result;
            }
            
            goto return_result;
        }
equal_result:
        result = 0;
        
        goto return_result;
    }
return_result:
    /* Retain the initialized result and the intact right-hand word pair. */
    return result;
}
