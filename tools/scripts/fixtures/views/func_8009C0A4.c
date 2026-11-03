/* GCC_VERSION: 2.7.2 */

#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define D_800ABDE4 (*(s32 (*))&D_800ABDE4)

#undef D_800ABDE4

extern volatile int D_800ABDE4;

extern int D_800ABDE8;

extern s32 D_800ABDF4;

extern s32 D_800ABDF8;

extern s32 D_80114730;

s32 func_8009C0A4(s32 value) {
    s32 result;
    if (!(D_800ABDE4 & 1)) return 0;
    D_800ABDF4 = 0;
    D_800ABDF8 = 0;
    D_80114730 = 0;
    D_800ABDE8 = 0;
    {
        /* ASM_BARRIER: retain the original assembler temporary for the final state store. */
        register s32 *base __asm__("$1");
        result = 1;
        /* ASM_BARRIER: preserve the shared return path and preceding stores. */
        __asm__("" : "=r"(result) : "0"(result) : "$1", "memory");
        /* PAL RAM mapping: D_800ABDEC is base minus 0x4214. */
        base = (s32 *)0x800B0000U;
        /* ASM_BARRIER: expose the register-relative store in the branch delay slot. */
        __asm__("" : "=r"(base) : "0"(base));
        base[-0x4214 / 4] = value;
    }
    return result;
}
