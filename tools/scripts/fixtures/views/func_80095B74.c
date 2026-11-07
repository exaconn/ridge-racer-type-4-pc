#include "m2c_macros.h"

void SetPolyFT3(void *arg0) {
    M2C_FIELD(arg0, s8 *, 3) = 7;
    M2C_FIELD(arg0, s8 *, 7) = 0x24;
}
