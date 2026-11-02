#include "types.h"
#include "m2c_macros.h"

extern int func_80096F64();
extern int func_8009AEAC();
extern s32 D_800F2AA8;
extern s32 D_800F2AAC;

void func_80034F18(void) {
    func_8009AEAC(2, 3, 0);
    func_8009AEAC(2, 0, 0);
    func_80096F64(D_800F2AAC);
    func_80096F64(D_800F2AA8);
    func_8009AEAC(4, 0, 0);
}
