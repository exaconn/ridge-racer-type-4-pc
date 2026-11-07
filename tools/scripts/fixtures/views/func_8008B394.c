/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/library.h"

/* legacy typing of shared globals in this part of the unit */
#define D_800A6350 (*(int (**(*))())&D_800A6350)

#undef D_800A6350

/* legacy typing of shared globals in this part of the unit */
#define D_800A6350 (*(int (**(*))())&D_800A6350)

#undef D_800A6350

/* legacy typing of shared globals in this part of the unit */
#define D_800A6350 (*(int (**(*))())&D_800A6350)

#undef D_800A6350

RrLibInterruptCallback InterruptCallback(s32 channel, RrLibInterruptCallback callback) {
    return D_800A6350->install(channel, callback);
}
