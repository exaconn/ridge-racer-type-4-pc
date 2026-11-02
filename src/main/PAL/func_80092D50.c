#include "types.h"
#include "m2c_macros.h"

extern M2C_UNK D_80011E94;
extern void *D_800ABAB8;
extern M2C_UNK (*D_800ABABC)(M2C_UNK *, s32);
extern u8 D_800ABAC2;

void func_80092D50(s32 arg0) {
    if ((u8) D_800ABAC2 >= 2U) {
        D_800ABABC(&D_80011E94, arg0);
    }
    M2C_FIELD(D_800ABAB8, M2C_UNK (**)(s32), 0x3C)(arg0);
}
