#include "types.h"
#include "m2c_macros.h"

extern int func_80086554();
extern s32 D_800F7650;

void func_8004EE60(s32 arg0) {
    D_800F7650 = arg0;
    func_80086554((s16) arg0, (s16) arg0);
}
