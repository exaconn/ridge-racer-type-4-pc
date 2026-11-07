/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/library.h"

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAC2 (*(unsigned char (*))&D_800ABAC2)

#define GPU_printf (*(int (*(*))())&GPU_printf)

#undef D_800ABAC2

#undef GPU_printf

extern void _exeque(void);

void func_80095990(void) {
    DMACallback(2,_exeque);
}
