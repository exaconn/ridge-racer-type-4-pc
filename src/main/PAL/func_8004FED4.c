#include "types.h"
#include "m2c_macros.h"

extern int func_8004FF34();
extern s32 D_800ACBC8;
extern s32 D_800F4DC8;

void func_8004FED4(void) {
    M2C_UNK var_a0;

    func_8004FF34(0);
    if (D_800F4DC8 == 0) {
        if (D_800ACBC8 >= 0) {
            func_8004FF34(2);
            var_a0 = 3;
            goto block_4;
        }
    } else {
        var_a0 = 1;
block_4:
        func_8004FF34(var_a0);
    }
}
