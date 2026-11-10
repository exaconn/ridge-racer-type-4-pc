/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/spu.h"

/* legacy typing of shared globals in this part of the unit */
#define _spu_RXX ((struct H *)_spu_RXX)

#define _spu_tsa (*(unsigned short (*))&_spu_tsa)

#undef _spu_RXX

#undef _spu_tsa

/* legacy typing of shared globals in this part of the unit */
#define _spu_RXX ((unsigned short *)_spu_RXX)

#undef _spu_RXX

/* legacy typing of shared globals in this part of the unit */
#define _spu_RXX ((unsigned short *)_spu_RXX)

#undef _spu_RXX

s32 _spu_FsetRXXa(s32 index, s32 value) {
    u32 address = (u32)value;
    u32 units;
    /* ASM_BARRIER: Preserve the original copy of the shifted address in v1
     * and the shared return value in v0 across the selector branches. */
    register u32 copy __asm__("$3");
    register u32 result __asm__("$2");

    if (_spu_mem_mode && address % _spu_mem_mode_unit != 0) {
        address += _spu_mem_mode_unit;
        address &= ~_spu_mem_mode_unitM;
    }
    units = address >> _spu_mem_mode_plus;
    copy = units;
    if (index == -2) goto address_result;
    if (index != -1) goto store;
    result = copy & 0xFFFF;
    goto out;
address_result:
    result = address;
    goto out;
store:
    ((SpuRegisterStorage *)_spu_RXX)->halves[index] = units;
    result = address;
out:
    return (s32)result;
}
