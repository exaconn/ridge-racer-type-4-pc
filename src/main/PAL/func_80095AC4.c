#include "types.h"
#include "m2c_macros.h"

void func_80095AC4(s32 *arg0, s32 arg1) {
    *arg0 = (*arg0 & 0xFF000000) | (arg1 & 0xFFFFFF);
}
