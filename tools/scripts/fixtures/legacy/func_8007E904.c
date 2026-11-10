#include "rr/wide.h"
u64 __negdf2(u64 value) {
    register u64 result=value;
    /* ASM_BARRIER: Retain the working words separately from the return pair. */
    register u64 bits __asm__("$6");
    union { u64 bits; Wide64 words; } work;
    bits=result;
    /* ASM_BARRIER: Keep the working pair before testing either word. */
    __asm__("" : "+r"(bits));
    work.bits=bits;
    if(work.words.hi || work.words.lo) {
        work.words.hi=(u32)work.words.hi ^ 0x80000000U;
        bits=work.bits;
        result=bits;
    }
    return result;
}
