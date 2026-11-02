#include "types.h"
#include "m2c_macros.h"

extern int func_80055250();

void func_80039494(void *arg0) {
    func_80055250(3, M2C_FIELD(arg0, s32 *, 0xF0));
}
