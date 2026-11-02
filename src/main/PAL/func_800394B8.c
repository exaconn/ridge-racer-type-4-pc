#include "types.h"
#include "m2c_macros.h"

extern int func_8004EE60();
extern int func_80055250();

void func_800394B8(void *arg0) {
    func_8004EE60(func_80055250(2, M2C_FIELD(arg0, s32 *, 0xF0)));
}
