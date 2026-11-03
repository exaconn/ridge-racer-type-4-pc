/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/cd.h"

s32 func_8008C0F8(s32 mode, u8 *status) {
    return CD_ready(mode, status);
}
