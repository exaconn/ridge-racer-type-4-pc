#include "types.h"
#include "m2c_macros.h"

extern int func_80022694();
extern int func_8002A344();
extern int func_8002AF8C();
extern int func_80034FC0();
extern int func_80035024();
extern int func_80035478();
extern s32 D_800ACC14;
extern s16 D_800AD508;
extern s32 D_800F2DE4;
extern s16 D_800F3A26;
extern s16 D_800F4BE8;
extern s16 D_800F4C6A;
extern s16 D_800F4D8A;

void func_80035844(void) {
    D_800F2DE4 += 1;
    if (func_80034FC0() != 1) {
        if (func_80035024(0) != 0) {
            D_800F4BE8 = 8;
            D_800F4C6A = 1;
            D_800F3A26 = 0x23;
            D_800F4D8A = 0;
        }
        func_80035478(D_800AD508);
        func_80022694(D_800ACC14 + 0x70, 8, 0xF0, 0x138, 0xF0, D_800F2DE4 * 8, 1, 8, 8, 8, 0);
        func_80022694(D_800ACC14 + 0x70, 8, 0x10C, 0x138, 0x10C, D_800F2DE4 * 8, 1, 8, 8, 8, 0);
        func_8002AF8C(1, 1, 1);
        func_8002A344(0x1003E);
    }
}
