#include "types.h"
#include "m2c_macros.h"

void func_800430D8(u8 *arg0, s32 arg1) {
    s32 var_v1;
    u8 *var_a0;

    var_a0 = arg0;
    var_v1 = 0;
    if (arg1 > 0) {
        do {
            var_v1 += 1;
            *var_a0 ^= 0x37;
            var_a0 += 1;
        } while (var_v1 < arg1);
    }
}
