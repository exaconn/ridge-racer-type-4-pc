#include "types.h"
#include "m2c_macros.h"

extern int func_8003BBBC();
extern int func_8004FA98();
extern int func_80071570();
extern int func_8007CDC4();
extern s16 D_800AD55A;
extern s32 D_800F2DE4;
extern s16 D_800F3A26;
extern u16 D_800F4C5C;
extern s16 D_800F4C6A;
extern s16 D_800F4D8A;

void func_8001F374(void) {
    s32 temp_v0;

    temp_v0 = func_8003BBBC();
    if (temp_v0 == 1) {
        D_800F2DE4 += 1;
        if (func_80071570(func_8007CDC4(1) == 1) == temp_v0) {
            func_8004FA98(D_800AD55A);
            D_800F4D8A = 0;
            D_800F4C6A = (s16) ((u16) D_800F4C5C >> 8);
            D_800F3A26 = D_800F4C5C & 0xFF;
        }
    }
}
