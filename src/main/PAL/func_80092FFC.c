/* CC1_FLAGS: -mno-split-addresses */
#include "types.h"
#include "m2c_macros.h"
M2C_UNK func_80092DB8(M2C_UNK *, s32);              /* extern */
extern M2C_UNK D_80011EE4;
extern void *D_800ABAB8;

void func_80092FFC(s32 arg0, M2C_UNK arg1) {
    func_80092DB8(&D_80011EE4, arg0);
    M2C_FIELD(D_800ABAB8, M2C_UNK (**)(s32, s32, M2C_UNK, M2C_UNK), 8)(M2C_FIELD(D_800ABAB8, s32 *, 0x20), arg0, 8, arg1);
}
