/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/channels.h"

s32 func_8008A644(u32 count) {
    s32 result;
    if (((count - 1) & 0xFFU) < 24) {
        D_801100FC = count;
        result = (s8)count;
    } else {
        result = -1;
    }
    return result;
}
