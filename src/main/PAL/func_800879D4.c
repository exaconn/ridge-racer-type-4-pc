#include "types.h"
#include "m2c_macros.h"

extern s32 D_800A47D8;

void func_800879D4(s32 arg0, u16 *arg1) {
    *arg1 = M2C_FIELD(((arg0 * 0x10) + D_800A47D8), u16 *, 0xC);
}
