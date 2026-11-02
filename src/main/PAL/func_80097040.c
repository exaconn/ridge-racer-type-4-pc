/* GCC_VERSION: 2.7.2 */
#include "rr/counters.h"
s32 func_80097040(u32 id) {
    s32 index=id & 0xFFFF;
    s32 result;
    if(index<3) {
        u16 count=D_800ABC34[index].count;
        /* ASM_BARRIER: Preserve the zero-extended halfword load as the return word. */
        __asm__("" : "=r"(result) : "0"(count));
    }
    else result=0;
    return result;
}
