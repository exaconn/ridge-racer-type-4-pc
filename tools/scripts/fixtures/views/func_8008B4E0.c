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

extern u16 *I_MASK;

s32 SetIntrMask(s32 value) {
    s32 previous=*I_MASK;
    *I_MASK=value;
    return previous;
}
