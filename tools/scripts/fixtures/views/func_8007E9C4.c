/* CC1_FLAGS: -mno-split-addresses */

#include "types.h"

extern s32 D_800A47D0, D_800A47D4;

extern s32 func_80096E64(u32, u32);

s32 func_8007E9C4(s32 mode, s32 parameter) {
    /* ASM_BARRIER: Keep the final parameter store in the original PAL address register. */
    register s32 *page __asm__("$1");
    /* ASM_BARRIER: Materialize the first mode comparison before publishing the parameters. */
    register s32 first_mode=33;
    __asm__ volatile("" : : "r"(first_mode));
    D_800A47D0=mode;
    /* ASM_BARRIER: Publish the mode before materializing the final parameter address. */
    __asm__ volatile("" : : : "$1", "memory");
    page=(s32 *)0x800A0000U;
    /* ASM_BARRIER: Expose the final store for the mode branch delay slot. */
    __asm__("" : "+r"(page));
    page[0x47D4/4]=parameter;
    switch(mode) {
    case 33: func_80096E64(0xF4000002U,0x301); break;
    case 34: func_80096E64(0xF4000002U,0x302); break;
    }
    return 0;
}
