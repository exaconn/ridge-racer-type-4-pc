/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "m2c_macros.h"

extern void *D_800ABAB8;
extern int func_8009424C();

u32 func_80093AAC(void);
void func_80093ADC(void *arg0, M2C_UNK arg1);

u32 func_80093AAC(void) {
    return M2C_FIELD(D_800ABAB8, u32 (**)(), 0x38)() >> 0x1F;
}

void func_80093ADC(void *arg0, M2C_UNK arg1) {
    M2C_FIELD(arg0, s8 *, 3) = 2;
    M2C_FIELD(arg0, s32 *, 4) = func_8009424C(arg1);
    M2C_FIELD(arg0, s32 *, 8) = 0;
}
