#include "types.h"
#include "m2c_macros.h"

void *func_80020508(s32 *arg0, void *arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    M2C_FIELD(arg1, s8 *, 3) = 3;
    M2C_FIELD(arg1, s16 *, 8) = arg2;
    M2C_FIELD(arg1, s16 *, 0xC) = (s16) arg4;
    M2C_FIELD(arg1, s8 *, 7) = 0x62;
    M2C_FIELD(arg1, s16 *, 0xA) = arg3;
    M2C_FIELD(arg1, s16 *, 0xE) = (s16) arg5;
    M2C_FIELD(arg1, s8 *, 4) = (s8) arg6;
    M2C_FIELD(arg1, s8 *, 5) = (s8) arg7;
    M2C_FIELD(arg1, s8 *, 6) = (s8) arg8;
    M2C_FIELD(arg1, s32 *, 0) = (s32) ((M2C_FIELD(arg1, s32 *, 0) & 0xFF000000) | (*arg0 & 0xFFFFFF));
    *arg0 = (*arg0 & 0xFF000000) | ((s32) arg1 & 0xFFFFFF);
    return arg1 + 0x10;
}
