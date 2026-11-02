#include "types.h"
#include "m2c_macros.h"

extern int func_8004F218();

void func_8004F274(s32 arg0) {
    func_8004F218(arg0 + 0x3C, 0x7F, 0x40, -1);
}
