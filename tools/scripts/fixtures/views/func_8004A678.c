#include "m2c_macros.h"

extern M2C_UNK Options;

void func_8004A678(void) {
    func_8004A5C0(0, M2C_FIELD(&Options, s16 *, 0), M2C_FIELD(&Options, s16 *, 0x14), M2C_FIELD(&Options, s8 *, 0x2B));
    func_8004A5C0(1, M2C_FIELD(&Options, s16 *, 4), M2C_FIELD(&Options, s16 *, 0x24), M2C_FIELD(&Options, s8 *, 0x2F));
}
