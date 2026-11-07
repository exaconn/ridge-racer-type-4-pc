/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/cd.h"

s32 CdReady(s32 mode, u8 *status) {
    return CD_ready(mode, status);
}
