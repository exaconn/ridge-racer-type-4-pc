#include "rr/wide.h"
RrWide64 *func_8007E818(RrWide64 *out,u32 a,u32 b) {
    RrWide64 r,t;
    u32 low_a=a&65535;
    /* ASM_BARRIER: Retain the output address across both additions. */
    register RrWide64 *result __asm__("$19")=out;
    u32 low_b=b&65535,high_a=a>>16,high_b=b>>16;
    u32 m;
    /* ASM_BARRIER: Retain the high half of each cross product in the original temporary. */
    register u32 cross_high __asm__("$3");
    /* ASM_BARRIER: Keep the output pointer independent of its incoming register value. */
    __asm__("" : "+r"(result));
    /* Each 16-bit factor is nonnegative; the signed 64-bit product cannot overflow. */
    m=(u32)((s64)(s32)high_a*(s32)low_b);
    r.hi=0;
    {
        /* ASM_BARRIER: Retain the low product in the original arithmetic temporary. */
        register u32 low_product __asm__("$3")=(u32)((s64)(s32)low_a*(s32)low_b);
        *(volatile u32 *)&r.lo=low_product;
    }
    cross_high=m>>16;
    t.hi=cross_high;*(volatile u32 *)&t.lo=m<<16;
    func_8007DB54(&r,r,t);
    m=(u32)((s64)(s32)low_a*(s32)high_b);
    cross_high=m>>16;
    t.hi=cross_high;*(volatile u32 *)&t.lo=m<<16;
    func_8007DB54(&r,r,t);
    {
        /* ASM_BARRIER: Retain the final high product in the original arithmetic temporary. */
        register u32 high_product __asm__("$3")=(u32)((s64)(s32)high_a*(s32)high_b);
        *(volatile s32 *)&r.hi=(u32)r.hi+high_product;
    }
    {
        u32 low=r.lo;s32 high=r.hi;
        result->lo=low;result->hi=high;
    }
    /* ASM_BARRIER: Complete the result stores before forming the returned pointer. */
    __asm__("" : : "r"(result) : "$2", "$3", "memory");
    return result;
}
