#include "types.h"
#include "m2c_macros.h"

extern int func_80091734();

u32 func_800384B4(void *arg0, M2C_UNK arg1) {
    u32 var_v0;

    var_v0 = 0;
    if (func_80091734(M2C_FIELD(arg0, s32 *, 8), M2C_FIELD(arg0, s32 *, 0xC), arg1) >= 0) {
        var_v0 = 0;
        if (func_80091734(M2C_FIELD(arg0, s32 *, 0xC), M2C_FIELD(arg0, s32 *, 4), arg1) >= 0) {
            var_v0 = 0;
            if (func_80091734(M2C_FIELD(arg0, s32 *, 4), M2C_FIELD(arg0, s32 *, 0), arg1) >= 0) {
                var_v0 = (u32) ~func_80091734(M2C_FIELD(arg0, s32 *, 0), M2C_FIELD(arg0, s32 *, 8), arg1) >> 0x1F;
            }
        }
    }
    return var_v0;
}
