#include "types.h"
#include "m2c_macros.h"

extern int func_8002A344();
extern int func_8003A198();
extern u32 D_800F2DE4;
extern s16 D_800F3A26;
extern s32 D_800F3A60;
extern s16 D_800F4C6A;
extern s16 D_800F4D8A;
extern s32 D_8010C0E0;

void func_80036270(void) {
    s32 temp_s0;

    D_800F2DE4 += 1;
    func_8002A344(1);
    if ((u32) D_800F2DE4 < 0x1C2U) {
        if (D_800F3A60 != 0) {
            goto block_2;
        }
        goto block_5;
    }
block_2:
    if (D_8010C0E0 <= 0) {
        D_8010C0E0 += 1;
        D_800F2DE4 = 0;
        return;
    }
block_5:
    temp_s0 = D_8010C0E0;
    if (temp_s0 == 1) {
        if (D_800F3A60 != 0) {
            D_800F2DE4 = 0;
            func_8003A198();
            D_800F4C6A = (s16) temp_s0;
            D_800F3A26 = 0xB;
            D_800F4D8A = 0;
        }
    }
}
