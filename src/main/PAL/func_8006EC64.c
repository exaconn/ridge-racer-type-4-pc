#include "types.h"
#include "m2c_macros.h"

extern int func_8006EED8();
extern int func_80090594();
extern int func_800910E4();
extern int func_80091114();
extern int func_80091274();
extern int func_80091294();
extern s32 D_800ABE98;
extern M2C_UNK D_800ABED0;
extern M2C_UNK D_800AD570;
extern s32 D_800F4D48;
extern s32 D_800F4D68;
extern s32 D_800F4D84;
extern s32 D_80107194;
extern s32 D_801093F8;

void func_8006EC64(void) {
    func_80091114(&D_800AD570);
    func_8006EED8();
    func_800910E4(&D_800ABED0);
    func_80091274(D_800ABE98, D_801093F8, D_80107194);
    func_80091294(D_800F4D84, D_800F4D68, D_800F4D48);
    func_80090594(0x4E20, 0x122);
}
