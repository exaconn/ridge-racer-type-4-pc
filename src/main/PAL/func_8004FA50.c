#include "types.h"
#include "m2c_macros.h"

extern int func_80050028();
extern int func_80052C70();
extern s32 D_800F4DBC;

void func_8004FA50(void) {
    s32 var_a0;
    s32 var_s0;

    D_800F4DBC = 0;
    func_80052C70();
    var_s0 = 0;
    var_a0 = 0;
    do {
        func_80050028(var_a0, 0);
        var_s0 += 1;
        var_a0 = var_s0;
    } while (var_s0 < 4);
}
