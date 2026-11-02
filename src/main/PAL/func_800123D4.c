#include "types.h"
#include "m2c_macros.h"

extern int func_80012290();
extern int func_8008C05C();
extern s32 D_800102F0;
extern s32 D_800ACC18;

void func_800123D4(void) {
    func_8008C05C(1);
    func_80012290();
    D_800ACC18 = D_800102F0;
}
