/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/library.h"

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAC2 (*(unsigned char (*))&D_800ABAC2)

#define D_800ABABC (*(int (*(*))())&D_800ABABC)

#undef D_800ABAC2

#undef D_800ABABC

extern void func_80094E74(void);

void func_80095990(void) {
    func_8008B3C4(2,func_80094E74);
}
