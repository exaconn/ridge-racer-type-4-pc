#include "types.h"
#include "m2c_macros.h"

extern void *D_800ABAB8;

u32 func_80093AAC(void) {
    return M2C_FIELD(D_800ABAB8, u32 (**)(), 0x38)() >> 0x1F;
}
