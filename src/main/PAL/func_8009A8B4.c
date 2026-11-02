/* CC1_FLAGS: -mno-split-addresses */
#include "types.h"
#include "m2c_macros.h"

extern int func_80096F04();
extern int func_80096F14();
extern int func_8009B9E4();
extern M2C_UNK D_800ABD50;

void func_8009A8B4(void) {
    s32 temp_s0;

    temp_s0 = func_80096F04();
    func_8009B9E4(&D_800ABD50);
    if (temp_s0 == 1) {
        func_80096F14();
    }
}
