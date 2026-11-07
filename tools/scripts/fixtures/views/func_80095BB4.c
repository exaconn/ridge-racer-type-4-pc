#include "m2c_macros.h"

void SetTile(void *arg0) {
    M2C_FIELD(arg0, s8 *, 3) = 3;
    M2C_FIELD(arg0, s8 *, 7) = 0x60;
}
