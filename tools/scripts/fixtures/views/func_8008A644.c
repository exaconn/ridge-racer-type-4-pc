/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/channels.h"

s32 SsSetReservedVoice(u32 count) {
    s32 result;
    if (((count - 1) & 0xFFU) < 24) {
        spuVmMaxVoice = count;
        result = (s8)count;
    } else {
        result = -1;
    }
    return result;
}
