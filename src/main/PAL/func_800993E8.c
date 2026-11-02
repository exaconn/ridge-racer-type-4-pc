#include "types.h"
#include "m2c_macros.h"

void func_800993E8(void *arg0) {
    M2C_FIELD(arg0, s8 *, 0x37) = 0x4D;
    M2C_FIELD(arg0, s8 *, 0x36) = 6;
    M2C_FIELD(arg0, s32 *, 0x2C) = (s32) M2C_FIELD(arg0, s32 *, 0x20);
}
