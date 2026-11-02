#include "types.h"
#include "m2c_macros.h"

extern int func_80060C70();
extern int func_8006E834();
extern int func_8006E8F8();
extern int func_8006EAC8();
extern int func_8006EC64();
extern int func_80071348();
extern s16 D_800F3914;
extern s32 D_8010E690;

void func_8006ED4C(void) {
    func_8006E834();
    func_8006E8F8();
    func_80071348();
    func_8006EC64();
    func_8006EAC8();
    func_80060C70(0x1F800000, D_8010E690, D_800F3914);
}
