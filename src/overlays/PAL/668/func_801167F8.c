/* MASPSX_FLAGS: --assembler-profile=gnu */

/* GCC_VERSION: 2.7.2 */

#include "rr/overlay668_wait.h"
s32 ovl668_func_801167F8(void)
{
    volatile s32 counter = 0x100000;
    while (*(volatile u32 *)ovl668_D_80116E34 & 0x01000000) {
        if (--counter == -1) {
            ovl668_func_801168A4(ovl668_D_801148EC);
            return -1;
        }
    }
    return 0;
}
