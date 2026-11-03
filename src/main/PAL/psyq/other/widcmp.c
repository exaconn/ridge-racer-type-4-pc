/* CC1_FLAGS: -mno-split-addresses */
#include "rr/wide.h"
s32 func_8007DFA4(RrWide64 left,RrWide64 right) {
    /* ASM_BARRIER: Keep the selected result in the common return register. */
    register s32 result __asm__("$2");
    if((u32)right.hi<(u32)left.hi) goto greater;
    if((u32)left.hi<(u32)right.hi) { result=-1; goto done; }
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
    /* ASM_BARRIER: Keep comparison results before the common return instruction. */
    __asm__ volatile("" : : "r"(result));
    return result;
}

#define RR_SHIFT_VALUE (*(RrWide64 *)&value)
RrWide64 *func_8007DFF4(RrWide64 *result,s32 right,RrWide64 value,s32 raw_count) {
    register s32 count=raw_count;
    register RrWide64 *out=result;
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
            u32 high=(u32)RR_SHIFT_VALUE.hi;
            /* ASM_BARRIER: Preserve the lower input word through the carry update. */
            register u32 low __asm__("$5")=RR_SHIFT_VALUE.lo;
            u32 shifted=high<<1;
            value.hi=shifted;
            if(low&mask) value.hi=shifted|1;
            value.lo=low<<1;
            /* ASM_BARRIER: Preserve the input word through its final shift store. */
            __asm__ volatile("" : : "r"(low));
            i++;
        } while(i<count);
    } else {
        register u32 mask;
        i=0;
        if(count<=0) goto publish;
        mask=0x80000000U;
        do {
            u32 low=RR_SHIFT_VALUE.lo;
            /* ASM_BARRIER: Preserve the upper input word through the carry update. */
            register u32 high __asm__("$5")=(u32)RR_SHIFT_VALUE.hi;
            u32 shifted=low>>1;
            value.lo=shifted;
            if(high&1) value.lo=shifted|mask;
            value.hi=high>>1;
            /* ASM_BARRIER: Preserve the input word through its final shift store. */
            __asm__ volatile("" : : "r"(high));
            i++;
        } while(i<count);
    }
publish:
    {
        u32 low=RR_SHIFT_VALUE.lo;
        u32 high=(u32)RR_SHIFT_VALUE.hi;
        out->lo=low;
        out->hi=high;
    }
    /* ASM_BARRIER: Publish both words before preparing the return pointer. */
    __asm__ volatile("" : : : "$2", "memory");
    return out;
}

#undef RR_SHIFT_VALUE
