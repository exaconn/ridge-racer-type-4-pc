/* GCC_VERSION: 2.7.2 */
#include "rr/gpu.h"
s32 func_800954D8(s32 a) {
    *D_800ABBC8 = 0x10000007;
    if ((*D_800ABBC4 & 0xFFFFFF) != 2) {
        *D_800ABBC4 = (*D_800ABBC8 & 0x3FFF) | 0xE1001000;
        *D_800ABBC4;
        return 0;
    }
    if (!(a & 8)) return 1;
    /* Ordinary final publication exposes the original branch-delay store. */
    *(u32 *)D_800ABBC8 = 0x09000001;
    return 2;
}
