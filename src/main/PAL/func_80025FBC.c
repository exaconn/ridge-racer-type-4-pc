#include "types.h"
#include "m2c_macros.h"

s32 func_80025FBC(s32 arg0, s32 arg1) {
    s32 temp_a0;
    s32 temp_v0;
    s32 var_a0;
    s32 var_v0;
    s32 var_v1;

    temp_a0 = arg0 & 0xFFF;
    temp_v0 = arg1 & 0xFFF;
    var_v1 = temp_a0 < temp_v0;
    if (var_v1 != 0) {
        var_a0 = temp_v0 - temp_a0;
    } else {
        var_a0 = temp_a0 - temp_v0;
    }
    if (var_a0 >= 0x801) {
        var_a0 = 0x1000 - var_a0;
        var_v1 = var_v1 == 0;
    }
    var_v0 = var_a0;
    if (var_v1 == 0) {
        var_v0 = -var_v0;
    }
    return var_v0;
}
