/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/wide.h"
s32 _comp_mant(Wide64 left,Wide64 right) {
    /* ASM_BARRIER: Keep the selected result in the common return register. */
    register s32 result __asm__("$2");
    if((u32)right.hi<left.hi) goto greater;
    if((u32)left.hi<right.hi) { result=-1; goto done; }
    if(right.lo<left.lo) { result=1; goto done; }
    /* ASM_BARRIER: Keep the final low-word comparison after the greater-word branch. */
    __asm__ volatile("" : : : "$3");
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
    register s32 count=raw_count;
    register Wide64 *out=result;
    /* ASM_BARRIER: Keep the iteration variable in the original argument register. */
    register s32 i __asm__("$4");
    if(!right) {
        register u32 mask;
        i=0;
        if(count<=0) goto publish;
        /* ASM_BARRIER: Keep the carry mask after the nonpositive-count gate. */
        __asm__ volatile("" : : : "$7");
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
        register u32 mask;
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
    /* ASM_BARRIER: Publish both words before preparing the return pointer. */
    __asm__ volatile("" : : : "$2", "memory");
    return out;
}
