#include "types.h"
#include "m2c_macros.h"

extern int func_80094230();

void func_80093B94(void *arg0, void *arg1) {
    M2C_FIELD(arg0, s8 *, 3) = 2;
    M2C_FIELD(arg0, s32 *, 4) = func_80094230(M2C_FIELD(arg1, s16 *, 0), M2C_FIELD(arg1, s16 *, 2));
    M2C_FIELD(arg0, s32 *, 8) = 0;
}
