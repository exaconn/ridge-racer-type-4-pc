/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define D_80112750 (*(char (*)[])&D_80112750)

#undef D_80112750

extern s32 func_8008C12C();

extern s32 func_8008C55C();

extern s32 func_80096DB8();

extern s32 func_80096DE0();

extern s32 func_80096F04();

extern s32 func_80096F14();

extern s32 D_800A64A8;

extern s8 *D_800A6800;

extern s8 *D_800A680C;

void func_8008F564(void) {
    func_80096F04();
    if (D_800A64A8 == 1) {
        func_80096DE0(0);
        func_80096DB8(0);
    } else {
        func_8008C55C(0);
        func_8008C12C(0);
    }
    *D_800A6800 = 0;
    {
        s8 *port = D_800A680C;
        /* ASM_BARRIER: preserve the pointer's load delay before the final call. */
        __asm__ volatile("nop" : : "r"(port));
        *port = 0;
    }
    func_80096F14();
}
