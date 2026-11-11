#include "rr/wide.h"
s32 _comp_mant(Wide64 left,Wide64 right) {
    s32 result;
    if((u32)right.hi<left.hi) goto greater;
    if((u32)left.hi<right.hi) { result=-1; goto done; }
    if(right.lo<left.lo) { result=1; goto done; }
    result=-1;
    if(left.lo<right.lo) goto done;
    result=0;
    goto done;
greater:
    result=1;
done:
    return result;
}

Wide64 *_dbl_shift_us(Wide64 *result,s32 right,Wide64 value,s32 raw_count) {
    s32 count=raw_count;
    Wide64 *out=result;
    s32 i;
    if(!right) {
        u32 mask;
        i=0;
        if(count<=0) goto publish;
        mask=0x80000000U;
        do {
            u32 high=(&value)->hi;
            u32 low=(&value)->lo;
            u32 shifted=high<<1;
            value.hi=shifted;
            if(low&mask) value.hi=shifted|1;
            value.lo=low<<1;
            i++;
        } while(i<count);
    } else {
        u32 mask;
        i=0;
        if(count<=0) goto publish;
        mask=0x80000000U;
        do {
            u32 low=(&value)->lo;
            u32 high=(&value)->hi;
            u32 shifted=low>>1;
            value.lo=shifted;
            if(high&1) value.lo=shifted|mask;
            value.hi=high>>1;
            i++;
        } while(i<count);
    }
publish:
    {
        u32 low=(&value)->lo;
        u32 high=(&value)->hi;
        out->lo=low;
        out->hi=high;
    }
    return out;
}
