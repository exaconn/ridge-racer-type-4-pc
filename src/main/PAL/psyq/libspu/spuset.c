/* GCC_VERSION: 2.7.2 */
#include "types.h"
#include "rr/spu.h"

extern s32 func_8007F414(s32, s32);
extern s32 D_800A4800;

u32 func_8008AFE4(s32 value) {
    u32 offset = (u32)value - 0x1010U;
    if (offset > 0x7EFE8U) return 0;
    D_800A47F0 = func_8007F414(-1, value);
    /* D_800A4800 is initialized to 3 in data and func_8007EAC4. */
    return (u32)D_800A47F0 << D_800A4800;
}
