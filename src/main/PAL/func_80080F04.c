#include "types.h"
#include "m2c_macros.h"

void func_80080F04(s32 arg0, u32 arg1, void *arg2) {
    u32 temp_v1;

    M2C_FIELD(arg2, s16 *, 0xA) = (s16) (arg0 & 0x8000);
    M2C_FIELD(arg2, s16 *, 0xC) = (s16) (arg1 & 0x8000);
    M2C_FIELD(arg2, s16 *, 0x10) = (s16) (arg1 & 0x4000);
    temp_v1 = arg0 & 0xFFFF;
    M2C_FIELD(arg2, s16 *, 0xE) = (s16) (arg1 & 0x20);
    M2C_FIELD(arg2, s16 *, 0) = (s16) ((temp_v1 >> 8) & 0x7F);
    M2C_FIELD(arg2, s16 *, 2) = (s16) ((temp_v1 >> 4) & 0xF);
    M2C_FIELD(arg2, s16 *, 4) = (s16) (arg0 & 0xF);
    M2C_FIELD(arg2, s16 *, 6) = (s16) ((arg1 >> 6) & 0x7F);
    M2C_FIELD(arg2, s16 *, 8) = (s16) (arg1 & 0x1F);
}
