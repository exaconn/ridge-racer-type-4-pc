#include "types.h"
#include "m2c_macros.h"

extern int func_80096F84();
extern int func_800976D4();
extern int func_80097724();

s32 func_80097500(void) {
    func_800976D4();
    func_80096F84(0);
    func_80097724();
    return 1;
}
