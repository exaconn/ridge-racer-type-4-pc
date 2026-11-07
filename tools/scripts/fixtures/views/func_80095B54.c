#include "m2c_macros.h"

void SetPolyF3(void *arg0) {
    M2C_FIELD(arg0, s8 *, 3) = 4;
    M2C_FIELD(arg0, s8 *, 7) = 0x20;
}
