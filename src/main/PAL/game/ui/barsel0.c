#include "types.h"
#include "m2c_macros.h"
#include "game/ui/barsel0.h"
#include "rr/badge_rows.h"
#include "game/ui/barsel0.h"

extern int func_8002AF8C();
extern int func_8002B12C();
extern int func_80032A14();
extern int func_80032DF0();
extern int UpdateBadgeRowAnim();
extern int func_8003E13C();
extern s16 D_800F3826;
extern u8 D_800F4D99;
extern void *D_800FF68C;

s32 func_80036C08(void) {
    s32 var_s2;

    var_s2 = 0;
    if (M2C_FIELD(BadgeRows, s16 *, 0x10) == 3) {
        func_80032DF0(D_800FF68C);
        func_80032A14(M2C_FIELD(BadgeRows, s16 *, 8) + 0xA, M2C_FIELD(BadgeRows, s16 *, 0xA) + 0x28, D_800FF68C);
    }
    UpdateBadgeRowAnim(0);
    if (M2C_FIELD(D_800FF68C, s8 *, 4) >= 0x22) {
        if (M2C_FIELD(BadgeRows, s16 *, 0x10) == 0xB) {
            if (D_800AD4F2 == 1) {
                M2C_FIELD(BadgeRows, s16 *, 0x10) = 0;
            } else {
                goto block_6;
            }
        } else {
block_6:
            if (M2C_FIELD(BadgeRows, s16 *, 0x10) < 4) {
                func_8002B12C(0xA0, 0xCC, 0x30, 0, (s32) D_800F3826, 0, 4);
                var_s2 = func_8003E13C(D_800F4D99);
            }
        }
    }
    func_8002AF8C(1, 1, 0);
    return var_s2;
}
