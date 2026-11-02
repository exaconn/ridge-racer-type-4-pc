#include "types.h"
#include "m2c_macros.h"

M2C_UNK func_8001E84C(M2C_UNK *);                   /* extern */
M2C_UNK func_80071440();                            /* extern */
M2C_UNK func_8007CDC4(M2C_UNK);                     /* extern */
M2C_UNK func_80092CB8(M2C_UNK);                     /* extern */
extern s32 D_800102F0;
extern M2C_UNK D_800103B8;
extern s32 D_800AC5DC;
extern s32 D_800ACC18;
extern s32 D_800F2DE4;
extern s16 D_800F3A26;
extern s16 D_800F4C5C;
extern s16 D_800F4C6A;
extern s8 D_800F4D7C;
extern s16 D_800F4D8A;
extern s16 D_800FFC18;
extern s32 D_8010BFBC;

void func_8001F2D4(void) {
    int pad[6];
    func_80092CB8(0);
    func_8001E84C(&D_800103B8);
    D_800ACC18 = D_800102F0;
    func_80071440();
    D_800FFC18 = 0;
    func_8007CDC4(0);
    D_800AC5DC = 0x80;
    D_800F2DE4 = 0;
    D_800F4D8A = 0;
    D_800F4D7C = 0;
    D_8010BFBC = 0;
    D_800F4C5C = 0x203;
    D_800F4C6A = 1;
    D_800F3A26 = 2;
    D_800F4D8A = 0;
}
