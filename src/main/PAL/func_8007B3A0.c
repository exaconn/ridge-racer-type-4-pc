#include "types.h"
#include "m2c_macros.h"

extern int func_8002A344();
extern int func_8002AF8C();
extern int func_80035024();
extern int func_8003D6F0();
extern int func_8007B460();
extern int func_8009AEAC();
extern s32 D_800A45B8;
extern s32 D_800F2DE4;
extern s16 D_800F4D44;
extern s16 D_800FF688;
extern s16 D_800FFC18;

void func_8007B3A0(void) {
    D_800F2DE4 += 1;
    if (D_800F4D44 == 4) {
        func_80035024(2);
        if (D_800FF688 == -1) {
            func_8009AEAC(1, 1, 0);
        }
    }
    func_8007B460();
    func_8003D6F0(0);
    if (D_800FFC18 != 3) {
        if (D_800FFC18 != 6) {
            if (D_800FFC18 == 7) {
                goto block_6;
            }
        } else {
            goto block_6;
        }
    } else {
block_6:
        func_8003D6F0(1);
    }
    func_8002A344(D_800A45B8);
    func_8002AF8C(1, 1, 1);
}
