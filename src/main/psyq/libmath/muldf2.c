#include "rr/wide.h"

Wide64 *_mul_mant_d(Wide64 *out,u32 a,u32 b);

Wide64 *_mul_mant_d(Wide64 *out,u32 a,u32 b) {
    Wide64 r,t;
    u32 low_a=a&65535;
    Wide64 *result=out;
    u32 low_b=b&65535,high_a=a>>16,high_b=b>>16;
    u32 m;
    u32 cross_high;
    /* Each 16-bit factor is nonnegative; the signed 64-bit product cannot overflow. */
    m=((s64)(s32)high_a*(s32)low_b);
    r.hi=0;
    cross_high=((s64)(s32)low_a*(s32)low_b);
    r.lo=cross_high;
    cross_high=m>>16;
    t.hi=cross_high;*(volatile u32 *)&t.lo=m<<16;
    _add_mant_d(&r,r,t);
    m=((s64)(s32)low_a*(s32)high_b);
    cross_high=m>>16;
    t.hi=cross_high;*(volatile u32 *)&t.lo=m<<16;
    _add_mant_d(&r,r,t);
    cross_high=((s64)(s32)high_a*(s32)high_b);
    *(volatile s32 *)&r.hi=r.hi+cross_high;
    {
        u32 low=r.lo;s32 high=r.hi;
        result->lo=low;result->hi=high;
    }
    return result;
}

u64 __negdf2(u64 value) {
    u64 result=value;
    u64 bits;
    union { u64 bits; Wide64 words; } work;
    bits=result;
    work.bits=bits;
    if(work.words.hi || work.words.lo) {
        work.words.hi=work.words.hi ^ 0x80000000U;
        bits=work.bits;
        result=bits;
    }
    return result;
}
