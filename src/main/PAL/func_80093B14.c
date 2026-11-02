#include "types.h"
#include "m2c_macros.h"

extern int func_80094100();
extern int func_80094198();

void func_80093B14(void *arg0, void *arg1) {
    M2C_FIELD(arg0, s8 *, 3) = 2;
    M2C_FIELD(arg0, s32 *, 4) = func_80094100(M2C_FIELD(arg1, s16 *, 0), M2C_FIELD(arg1, s16 *, 2));
    M2C_FIELD(arg0, s32 *, 8) = func_80094198((s16) (((u16) M2C_FIELD(arg1, s16 *, 0) + M2C_FIELD(arg1, u16 *, 4)) - 1), (s16) (((u16) M2C_FIELD(arg1, s16 *, 2) + M2C_FIELD(arg1, u16 *, 6)) - 1));
}
