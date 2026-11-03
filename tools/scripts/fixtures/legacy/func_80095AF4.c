#include "types.h"
#include "m2c_macros.h"

void func_80095AF4(void *arg0, s32 arg1) {
    u8 var_v0;

    if (arg1 != 0) {
        var_v0 = M2C_FIELD(arg0, u8 *, 7) | 2;
    } else {
        var_v0 = M2C_FIELD(arg0, u8 *, 7) & 0xFD;
    }
    M2C_FIELD(arg0, u8 *, 7) = var_v0;
}
