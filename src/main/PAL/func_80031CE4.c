#include "types.h"
#include "m2c_macros.h"

extern int func_8002AF8C();
extern int func_80032A14();
extern int func_80032DF0();
extern int func_8003D6F0();
extern s32 D_800F3AFC;
extern void *D_800FF68C;

void func_80031CE4(void) {
    void *temp_v0;

    if (M2C_FIELD(((M2C_FIELD(D_800FF68C, s16 *, 0x16) * 0x1C) + D_800F3AFC), s16 *, 0x10) == 3) {
        func_8002AF8C(1, 1, 0);
        func_80032DF0(D_800FF68C);
        temp_v0 = (M2C_FIELD(D_800FF68C, s16 *, 0x16) * 0x1C) + D_800F3AFC;
        func_80032A14(M2C_FIELD(temp_v0, s16 *, 8) + 0xA, M2C_FIELD(temp_v0, s16 *, 0xA) + 0x28, D_800FF68C);
    }
    func_8003D6F0(M2C_FIELD(D_800FF68C, s16 *, 0x16));
}
