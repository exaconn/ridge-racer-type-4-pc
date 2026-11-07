/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/interrupts.h"

/* legacy typing of shared globals in this part of the unit */
#define IntrDriverTablePtr (*(int (**(*))())&IntrDriverTablePtr)

#undef IntrDriverTablePtr

/* legacy typing of shared globals in this part of the unit */
#define IntrDriverTablePtr (*(int (**(*))())&IntrDriverTablePtr)

#undef IntrDriverTablePtr

/* legacy typing of shared globals in this part of the unit */
#define IntrDriverTablePtr (*(int (**(*))())&IntrDriverTablePtr)

#undef IntrDriverTablePtr

extern u16 *I_MASK;

s32 SetIntrMask(s32 value) {
    s32 previous=*I_MASK;
    *I_MASK=value;
    return previous;
}
