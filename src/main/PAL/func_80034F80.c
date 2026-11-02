#include "types.h"
#include "m2c_macros.h"

extern int func_8009AEAC();
extern s16 D_800FF688;

void func_80034F80(void) {
    if (D_800FF688 == 2) {
        D_800FF688 = 3;
        func_8009AEAC(1, 1, 2);
    }
}
