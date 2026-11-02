#include "types.h"
#include "m2c_macros.h"

extern int func_80096434();
extern M2C_UNK D_800AC6D8;

void func_80045830(void) {
    func_80096434(&D_800AC6D8, 0x20);
    M2C_FIELD(&D_800AC6D8, s32 *, 0x1C) = -1;
}
