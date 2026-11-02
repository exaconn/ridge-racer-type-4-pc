#include "types.h"
#include "m2c_macros.h"

extern int func_8009424C();

void func_80093ADC(void *arg0, M2C_UNK arg1) {
    M2C_FIELD(arg0, s8 *, 3) = 2;
    M2C_FIELD(arg0, s32 *, 4) = func_8009424C(arg1);
    M2C_FIELD(arg0, s32 *, 8) = 0;
}
