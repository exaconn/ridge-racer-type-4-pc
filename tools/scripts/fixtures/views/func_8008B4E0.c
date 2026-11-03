/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/interrupts.h"

/* legacy typing of shared globals in this part of the unit */
#define D_800A6350 (*(int (**(*))())&D_800A6350)

#undef D_800A6350

/* legacy typing of shared globals in this part of the unit */
#define D_800A6350 (*(int (**(*))())&D_800A6350)

#undef D_800A6350

/* legacy typing of shared globals in this part of the unit */
#define D_800A6350 (*(int (**(*))())&D_800A6350)

#undef D_800A6350

extern u16 *D_800A6358;

s32 func_8008B4E0(s32 value) {
    s32 previous=*D_800A6358;
    *D_800A6358=value;
    return previous;
}
