#include "types.h"
#include "m2c_macros.h"

extern int func_8005D40C();
extern void *D_800ACA28;

void func_8005D820(void) {
    s32 temp_a0;

    temp_a0 = M2C_FIELD(D_800ACA28, s32 *, 0x128);
    if (temp_a0 > 0) {
        func_8005D40C(temp_a0 - 1);
    }
}
