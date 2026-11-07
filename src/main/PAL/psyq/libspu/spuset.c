/* GCC_VERSION: 2.7.2 */
#include "types.h"
#include "rr/spu.h"

extern s32 _spu_FsetRXXa(s32, s32);
extern s32 _spu_mem_mode_plus;

u32 SpuSetTransferStartAddr(s32 value) {
    u32 offset = (u32)value - 0x1010U;
    if (offset > 0x7EFE8U) return 0;
    _spu_tsa = _spu_FsetRXXa(-1, value);
    /* _spu_mem_mode_plus is initialized to 3 in data and _spu_init. */
    return (u32)_spu_tsa << _spu_mem_mode_plus;
}
