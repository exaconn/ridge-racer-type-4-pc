#include "types.h"
#include "rr/audio_loop.h"
#include "m2c_macros.h"

extern int func_8004FF34();
extern s32 D_800ACBC8;
extern s32 D_800F4DC8;

void func_8004FE7C(s32 a0, int a1, int a2);
void func_8004FED4(void);

void func_8004FE7C(s32 a0, int a1, int a2) {
    int v = D_800F4DC0;
    func_80086B00((s16)a0, (s16)((s32)((u32)a1 * (u32)v) >> 7), (s16)((s32)((u32)a2 * (u32)v) >> 7));
}

void func_8004FED4(void) {
    M2C_UNK var_a0;

    func_8004FF34(0);
    if (D_800F4DC8 == 0) {
        if (D_800ACBC8 >= 0) {
            func_8004FF34(2);
            var_a0 = 3;
            goto block_4;
        }
    } else {
        var_a0 = 1;
block_4:
        func_8004FF34(var_a0);
    }
}
