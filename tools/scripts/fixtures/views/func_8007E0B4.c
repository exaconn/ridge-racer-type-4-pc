#include "rr/softfloat.h"

s32 func_8007E0B4(u64 value)
{
    /* Keep the input pair live across the error and shift calls. */
    register union {u64 bits;RrWide64 words;} input __asm__("$16");
    RrWide64 mantissa;
    s32 exponent,shift,result;
    /* Capture the mantissa before the exponent and sign gates. */
    s32 scaled;
    input.bits=value;
    if((input.words.hi&0x7FFFFFFF)||(result=0,input.words.lo!=0)) {
        exponent=(input.words.hi>>20)&0x7FF;
        shift=exponent-0x41D;
        if(shift>0) {
            func_8007E9C4(34,17);
            result=0x7FFFFFFF;
            if(input.words.hi<0) result=0x80000000;
        } else {
            mantissa.hi=(input.words.hi&0xFFFFF)|0x100000;
            mantissa.lo=input.words.lo;
            /* Retain the original initialized stack-pair argument read. */
            mantissa=_dbl_shift(0,*(volatile RrWide64 *)&mantissa,10);
            scaled=mantissa.hi;
            if((u32)(exponent-0x3FE)<32U&&scaled!=0) {
                /* The exponent gate proves that -shift is in0..31. */
                scaled=scaled>>-shift;
                goto signed_result;
            }
            result=0;
            goto done;
signed_result:
            result=scaled;
            if(input.words.hi<0) result=(s32)(0U-(u32)result);
        }
    }
done:
    return result;
}
