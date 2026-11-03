/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "m2c_macros.h"

extern int func_8008DCB4();
extern int func_8008B3C4();
extern int func_8008DA4C();

s32 func_8008C53C(void);
void func_8008C55C(a0);
void func_8008C580();

s32 func_8008C53C(void) {
    return func_8008DCB4() == 0;
}

void func_8008C55C(a0) { func_8008B3C4(3, a0); }

void func_8008C580() { func_8008DA4C(); }
