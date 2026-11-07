/* GCC_VERSION: 2.7.2 */
#include "rr/gpu.h"
s32 func_800954D8(s32 a) {
    *GPU_STATUS = 0x10000007;
    if ((*GPU_DATA & 0xFFFFFF) != 2) {
        *GPU_DATA = (*GPU_STATUS & 0x3FFF) | 0xE1001000;
        *GPU_DATA;
        return 0;
    }
    if (!(a & 8)) return 1;
    /* Ordinary final publication exposes the original branch-delay store. */
    *(u32 *)GPU_STATUS = 0x09000001;
    return 2;
}
