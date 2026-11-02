#include "types.h"
#include "m2c_macros.h"

extern int func_80096434();
extern s32 D_800ACA28;

void func_8005D500(s32 arg0) {
    void *temp_s1;

    temp_s1 = D_800ACA28 + 0xE4;
    func_80096434(temp_s1, 0x50);
    M2C_FIELD(temp_s1, s32 *, 4) = arg0;
}
