#include "types.h"
#include "m2c_macros.h"

extern int func_80096198();

void func_80078E1C(s32 arg0, s32 arg1) {
    func_80096198((arg0 * 0x10) + arg1);
}
