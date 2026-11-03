/* GCC_VERSION: 970318 */
#include "types.h"
#include "m2c_macros.h"

void func_80098AC4(void *arg0, s32 arg1, s8 arg2);
void func_80098AD0(void *arg0, s8 arg1, s32 arg2, s8 arg3);

void func_80098AC4(void *arg0, s32 arg1, s8 arg2) {
    M2C_FIELD(arg0, s32 *, 0x28) = arg1;
    M2C_FIELD(arg0, s8 *, 0x34) = arg2;
}

void func_80098AD0(void *arg0, s8 arg1, s32 arg2, s8 arg3) {
    M2C_FIELD(arg0, s8 *, 0x37) = arg1;
    M2C_FIELD(arg0, s32 *, 0x2C) = arg2;
    M2C_FIELD(arg0, s8 *, 0x36) = arg3;
}
