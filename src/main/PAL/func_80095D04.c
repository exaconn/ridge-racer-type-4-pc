#include "types.h"
#include "m2c_macros.h"

extern int func_80095DD4();
extern int func_80095E9C();
extern int func_80095F30();
extern int func_80095FA0();
extern int func_80096F04();
extern int func_80096F14();
extern int func_80096F84();
extern int func_800973D0();

void func_80095D04(s32 arg0) {
    s32 var_s0;

    var_s0 = arg0;
    func_80096F84(0);
    func_80096F04();
    if (func_800973D0() == 0) {
        var_s0 = 0;
    }
    func_80095DD4(var_s0);
    func_80095FA0();
    func_80095E9C();
    func_80095F30();
    func_80096F14();
}
