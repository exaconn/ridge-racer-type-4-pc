#include "types.h"
#include "m2c_macros.h"

void func_80098AC4(void *arg0, s32 arg1, s8 arg2) {
    M2C_FIELD(arg0, s32 *, 0x28) = arg1;
    M2C_FIELD(arg0, s8 *, 0x34) = arg2;
}
