/* CC1_FLAGS: -mno-split-addresses */
#include "rr/commands.h"
s32 func_8009C400(s32 value) {
    /* ASM_BARRIER: Keep the zero result in v0 before the shared return site. */
    register s32 result __asm__("$2");
    switch(D_800ABDF8) {
    case 3: D_80114730=value & 255; break;
    case 4: *(u32 *)D_8011471C=value & 255; break;
    case 5: *(u32 *)D_8011471C+=(u32)(value & 255)<<8; break;
    }
    result=0;
    /* ASM_BARRIER: Preserve the common return site after all response writes. */
    __asm__ volatile("" : : "r"(result));
    return result;
}
