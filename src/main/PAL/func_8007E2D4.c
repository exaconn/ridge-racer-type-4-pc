/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/softfloat.h"

s32 func_8007E2D4(u64 left, u64 right) {
    /* ASM_BARRIER: Keep the left and right word pairs in their original registers. */
    register union {u64 bits; RrWide64 words;} a __asm__("$8"), b __asm__("$6");
    u32 magnitude = 0x7FFF0000;
    s32 leftExponent;
    s32 rightFraction;
    s32 leftSign;
    s32 rightExponent;
    s32 leftFraction;
    s32 result;
    /* ASM_BARRIER: Keep the high words in $t1 and $a3 across the zero and sign tests. */
    register s32 highA __asm__("$9"), highB __asm__("$7");

    a.bits = left; b.bits = right; highA = a.words.hi; highB = b.words.hi;
    if ((a.words.hi != b.words.hi) || ({
        /* ASM_BARRIER: Keep the initialized upper mask live through the low-word test. */
        __asm__("" : "+r"(magnitude));
        result = 0;
        a.words.lo != b.words.lo;
    })) {
        if (((highA & (magnitude | 0xFFFF)) == 0) && (a.words.lo == 0) && ((highB & (magnitude | 0xFFFF)) == 0) && (b.words.lo == 0)) {
            /* ASM_BARRIER: Prevent CSE from substituting $a2 for zero after the zero check. */
            __asm__("" : "+r"(b.bits));
            result = 0;
            goto return_result;
        }
        leftSign = a.words.hi & 0x80000000;
        if (leftSign != (b.words.hi & 0x80000000)) {
            result = (u32)leftSign < 1;
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
                result = (u32)({
                    u32 m = 0x80000000;
                    /* ASM_BARRIER: Materialize the sign mask in a register for the branch delay slots. */
                    __asm__("" : "+r"(m));
                    highA & m;
                }) < 1;
                goto return_result;
            }
            result = a.words.hi < zero;
        }
    }
return_result:
    /* ASM_BARRIER: Retain the initialized result and the intact input registers. */
    __asm__("" : "=r"(result) : "0"(result), "r"(b.bits), "r"(highA));
    return result;
}
