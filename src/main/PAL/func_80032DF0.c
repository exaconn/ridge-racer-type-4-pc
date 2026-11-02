#include "types.h"
#include "m2c_macros.h"

extern int func_8004F24C();
extern s32 D_800F3A60;

void func_80032DF0(void *arg0) {
    if (M2C_FIELD(arg0, s32 *, 0x18) < M2C_FIELD(arg0, s16 *, 0xC)) {
        if (D_800F3A60 != 0) {
            if (M2C_FIELD(arg0, s8 *, 6) != 4) {
                func_8004F24C(3);
                if (M2C_FIELD(arg0, s16 *, 0x12) < (M2C_FIELD(arg0, u8 *, 0xA) - M2C_FIELD(arg0, u8 *, 0xB))) {
                    M2C_FIELD(arg0, s16 *, 0x12) = (s16) (M2C_FIELD(arg0, u8 *, 0xA) - M2C_FIELD(arg0, u8 *, 0xB));
                    M2C_FIELD(arg0, s32 *, 0x18) = (s32) (M2C_FIELD(arg0, s16 *, 0xC) - 1);
                    return;
                }
                M2C_FIELD(arg0, s32 *, 0x18) = (s32) (M2C_FIELD(arg0, s16 *, 0xC) - 1);
            }
        }
    }
}
