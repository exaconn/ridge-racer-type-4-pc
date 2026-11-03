/* GCC_VERSION: 2.7.2 */

#include "rr/channels.h"

s32 func_80085AEC(s32 a0) {
    int r;
    int h = a0;
    int selected = a0;
    if (D_8010F05C == 1) { r = -1; goto out; }
    D_8010F05C = 1;
    if ((unsigned)(selected & 0xFFFF) >= 0x18U) { 
        /* ASM_BARRIER: Use the original assembler temporary for the failure cleanup. */
        register int *page __asm__("$1");
        r=-1;
        page=(int *)0x80110000U;
        /* ASM_BARRIER: Expose the cleanup store for the shared-return delay slot. */
        __asm__("" : "=r"(page) : "0"(page));
        page[-0xFA4/4]=0;
        goto out; }
    D_80110118 = h;
    /* ASM_BARRIER: Keep the selector in a1 until the reset call. */
    __asm__("" : : "r"(selected));
    func_80088DA4(0);
    D_8010F05C = 0;
    r = 0;
out:
    return r;
}
