#include "types.h"
#include "m2c_macros.h"

void func_8002C11C(void *arg0, s16 arg1, s16 arg2) {
    M2C_FIELD(arg0, s16 *, 0x19C) = 2;
    M2C_FIELD(arg0, s16 *, 0x198) = arg1;
    M2C_FIELD(arg0, s16 *, 0x19A) = arg2;
    M2C_FIELD(arg0, s16 *, 0x19E) = 0xF;
}
