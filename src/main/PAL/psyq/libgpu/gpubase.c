/* GCC_VERSION: 970318 */
#include "types.h"
#include "rr/library.h"
#include "m2c_macros.h"

extern void func_80094E74(void);

void func_80095990(void);
void func_800959B8(s8 *arg0, s8 arg1, s32 arg2);

void func_80095990(void) {
    func_8008B3C4(2,func_80094E74);
}

void func_800959B8(s8 *arg0, s8 arg1, s32 arg2) {
    s32 var_v0;
    s8 *var_a0;

    var_a0 = arg0;
    var_v0 = arg2 - 1;
    if (arg2 != 0) {
        do {
            *var_a0 = arg1;
            var_v0 -= 1;
            var_a0 += 1;
        } while (var_v0 != -1);
    }
}
