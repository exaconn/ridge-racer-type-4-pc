#include "types.h"
#include "m2c_macros.h"

extern void *D_800ACA28;

void func_8005B2E4(s32 arg0) {
    M2C_FIELD(D_800ACA28, s32 *, 0x50) = arg0;
}
