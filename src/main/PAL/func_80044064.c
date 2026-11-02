#include "types.h"
#include "m2c_macros.h"

extern void *D_800ACBA0;

s32 func_80044064(void) {
    s32 var_a1;
    void *var_a0;

    var_a0 = D_800ACBA0;
    var_a1 = 0;
    if (M2C_FIELD(var_a0, s16 *, 0x32) != -1) {
        do {
            var_a0 += 0x44;
            var_a1 += 1;
        } while (M2C_FIELD(var_a0, s16 *, 0x32) != -1);
    }
    return var_a1;
}
