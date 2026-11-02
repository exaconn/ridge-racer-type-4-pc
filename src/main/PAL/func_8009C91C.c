/* GCC_VERSION: 2.7.2 */
#include "rr/commands.h"
s32 func_8009C91C(s32 value) {
    s32 phase=D_800ABDF8;
    /* ASM_BARRIER: Keep the response result in the common return register. */
    /* ASM_BARRIER: Preserve the original register through address or return preparation. */
    register s32 result __asm__("$2");
    if(phase==9) D_80114730=(u32)D_80114728+1;
    else if(phase>=11) D_80114718[phase-11]=value;
    result=0;
    /* ASM_BARRIER: Keep the shared return after any response publication. */
    __asm__ volatile("" : : "r"(result));
    return result;
}
