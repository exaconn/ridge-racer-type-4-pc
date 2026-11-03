#include "types.h"
#include "m2c_macros.h"

extern int func_80095CE4();
extern int func_80095CF4();

void func_80095CA4(s32 arg0) {
    func_80095CF4();
    func_80095CE4(arg0, 0x3F, 0);
}
