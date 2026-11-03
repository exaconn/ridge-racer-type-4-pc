#include "rr/badge_rows.h"

#include "m2c_macros.h"

/* legacy typing of shared globals in this part of the unit */
#define func_80095A44 ((void (*)(u32 *,u32 *))func_80095A44)

#undef func_80095A44

extern int func_80095A44();

extern int func_80095BB4();

void *func_8001FD20(u32 *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    func_80095BB4(arg1);
    M2C_FIELD(arg1, s16 *, 8) = arg2;
    M2C_FIELD(arg1, s16 *, 0xA) = arg3;
    M2C_FIELD(arg1, s16 *, 0xC) = (s16) arg4;
    M2C_FIELD(arg1, s16 *, 0xE) = (s16) arg5;
    M2C_FIELD(arg1, s8 *, 4) = (s8) arg6;
    M2C_FIELD(arg1, s8 *, 5) = (s8) arg7;
    M2C_FIELD(arg1, s8 *, 6) = (s8) arg8;
    func_80095A44(arg0, arg1);
    return arg1 + 0x10;
}
