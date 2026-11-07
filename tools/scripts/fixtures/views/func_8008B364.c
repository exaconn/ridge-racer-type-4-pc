/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/library.h"

/* legacy typing of shared globals in this part of the unit */
#define IntrDriverTablePtr (*(int (**(*))())&IntrDriverTablePtr)

#undef IntrDriverTablePtr

/* legacy typing of shared globals in this part of the unit */
#define IntrDriverTablePtr (*(int (**(*))())&IntrDriverTablePtr)

#undef IntrDriverTablePtr

/* legacy typing of shared globals in this part of the unit */
#define IntrDriverTablePtr (*(int (**(*))())&IntrDriverTablePtr)

#undef IntrDriverTablePtr

void ResetCallback(void) {
    IntrDriverTablePtr->reset();
}
