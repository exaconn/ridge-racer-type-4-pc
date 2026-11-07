#include "rr/wide.h"
RrWide64 *_mainasu(RrWide64 *result,RrWide64 x) {
    RrWide64 one;
    one.hi=0;
    /* Qualified write views keep the real ABI-home stores and subsequent reloads. */
    *(volatile u32 *)&one.lo=1;
    *(volatile s32 *)&x.hi=~x.hi;
    *(volatile u32 *)&x.lo=~x.lo;
    _add_mant_d(&x,x,one);
    {
        u32 low=x.lo;
        s32 high=x.hi;
        result->lo=low;
        result->hi=high;
    }
    /* ASM_BARRIER: Publish both result words before forming the returned address. */
    __asm__("" : : "r"(result) : "$2", "$3", "memory");
    return result;
}
