#include "types.h"
#include "m2c_macros.h"

s32 func_80043080(s32 arg0, s32 arg1, s32 arg2) {
    s32 var_v0;

    if (arg2 == 0) {
        var_v0 = arg0 + arg1;
    } else {
        var_v0 = arg0 - arg1;
    }
    return var_v0 & 0xFF;
}
