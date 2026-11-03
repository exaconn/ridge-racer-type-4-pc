#include "types.h"
#include "m2c_macros.h"

extern int CD_getsector2();

s32 func_8008C53C(void) {
    return CD_getsector2() == 0;
}
