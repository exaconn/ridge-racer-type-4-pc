#include "types.h"
#include "m2c_macros.h"

void func_8002679C(void *arg0) {
    s32 temp_v1;
    s32 var_v1;

    var_v1 = M2C_FIELD(arg0, s16 *, 0x1D8) * 3;
    if (var_v1 >= 0xC1) {
        var_v1 = 0xC0;
    }
    temp_v1 = M2C_FIELD(arg0, s32 *, 0x178);
    M2C_FIELD(arg0, s32 *, 0x17C) = (s32) ((M2C_FIELD(arg0, s32 *, 0x17C) - var_v1) & 0xFFF);
    if (temp_v1 >= 0x1000) {
        M2C_FIELD(arg0, s32 *, 0x178) = 0x1000;
        return;
    }
    if (temp_v1 < -0xFFF) {
        M2C_FIELD(arg0, s32 *, 0x178) = -0x1000;
    }
}
