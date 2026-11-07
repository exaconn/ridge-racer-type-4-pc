#include "types.h"
#include "m2c_macros.h"

extern int CD_getsector2();

s32 CdGetSector2(void) {
    return CD_getsector2() == 0;
}
