#include "types.h"
#include "m2c_macros.h"

s32 func_8006F260(void *arg0) {
    s32 temp_v1;
    s32 var_v0;

    temp_v1 = M2C_FIELD(arg0, s32 *, 0x14) & 0xFFF;
    var_v0 = temp_v1 + 0x100;
    if (var_v0 < 0) {
        var_v0 = temp_v1 + 0x2FF;
    }
    return (var_v0 >> 9) & 7;
}
