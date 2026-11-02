/* CC1_FLAGS: -mno-split-addresses */
#include "types.h"
#include "m2c_macros.h"
M2C_UNK func_80096E14();                            /* extern */
s32 func_80096F04();                                /* extern */
M2C_UNK func_80096F14();                            /* extern */
M2C_UNK func_8009B9F4(M2C_UNK *);                   /* extern */
extern M2C_UNK D_80012104;

void func_8009A8F8(void) {
    s32 temp_s0;

    temp_s0 = func_80096F04();
    func_8009B9F4(&D_80012104);
    func_80096E14();
    if (temp_s0 == 1) {
        func_80096F14();
    }
}
