#include "types.h"
#include "m2c_macros.h"

void func_8002E828(s32 arg0, void *arg1) {
    M2C_FIELD(arg1, s16 *, 6) = (s16) (M2C_FIELD(arg1, s16 *, 6) * arg0);
    M2C_FIELD(arg1, s16 *, 8) = (s16) (M2C_FIELD(arg1, s16 *, 8) * arg0);
    M2C_FIELD(arg1, s16 *, 0xA) = (s16) (M2C_FIELD(arg1, s16 *, 0xA) * arg0);
}
