/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/spu.h"

/* legacy typing of shared globals in this part of the unit */
#define D_800A47D8 ((struct H *)D_800A47D8)

#define D_800A47F0 (*(unsigned short (*))&D_800A47F0)

#undef D_800A47D8

#undef D_800A47F0

/* legacy typing of shared globals in this part of the unit */
#define D_800A47D8 ((unsigned short *)D_800A47D8)

#undef D_800A47D8

/* legacy typing of shared globals in this part of the unit */
#define D_800A47D8 ((unsigned short *)D_800A47D8)

#undef D_800A47D8

s32 func_8007F414(s32 index, s32 value) {
    u32 address = (u32)value;
    u32 units;
    /* ASM_BARRIER: Preserve the original copy of the shifted address in v1
     * and the shared return value in v0 across the selector branches. */
    register u32 copy __asm__("$3");
    register u32 result __asm__("$2");

    if (D_800A47FC && address % D_800A4804 != 0) {
        address += D_800A4804;
        address &= ~D_800A4808;
    }
    units = address >> D_800A4800;
    copy = units;
    if (index == -2) goto address_result;
    if (index != -1) goto store;
    result = copy & 0xFFFF;
    goto out;
address_result:
    result = address;
    goto out;
store:
    ((RrSpuRegisterStorage *)D_800A47D8)->halves[index] = units;
    result = address;
out:
    return (s32)result;
}
