#include "types.h"
#include "m2c_macros.h"

extern int func_80036030();
extern int func_8004FC18();
extern s32 D_800F2EC8;

void func_80036068(void) {
    func_8004FC18(0, 0);
    func_80036030();
    D_800F2EC8 = 0;
}
