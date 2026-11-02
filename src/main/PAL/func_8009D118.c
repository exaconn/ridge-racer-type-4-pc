/* CC1_FLAGS: -mno-split-addresses */
#include "rr/commands.h"
s32 func_8009D118(void) {
    /* ASM_BARRIER: Keep each response value in the register consumed by the common return. */
    register s32 result;
    switch(D_800ABDE8) {
    case 2: result=0x5D; break;
    case 4: result=D_80114724; break;
    case 5: result=D_8011472C.value; break;
    case 6: result=D_80114728; break;
    default: result=0; break;
    }
    /* ASM_BARRIER: Preserve the shared return site after selecting the command response. */
    __asm__ volatile("" : : "r"(result));
    return result;
}
