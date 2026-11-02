/* GCC_VERSION: 2.7.2 */
#include "rr/gpu.h"
s32 func_80094B70(s32 a) {
    *D_800ABBC8 = a | 0x10000000;
    return *D_800ABBC4 & 0xFFFFFF;
}
