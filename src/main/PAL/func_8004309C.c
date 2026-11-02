#include "types.h"
#include "m2c_macros.h"

void func_8004309C(s32 arg0, void *arg1) {
    M2C_FIELD(arg1, s8 *, 0) = (s8) ((arg0 & 0xF) + 0x41);
    M2C_FIELD(arg1, s8 *, 1) = (s8) (((u32) (arg0 & 0xFF) >> 4) + 0x4B);
}
