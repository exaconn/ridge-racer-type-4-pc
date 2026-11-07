#include "types.h"
#include "m2c_macros.h"
#include "psyq/libgpu/primatt3.h"

void SetPolyFT3(void *arg0) {
    M2C_FIELD(arg0, s8 *, 3) = 7;
    M2C_FIELD(arg0, s8 *, 7) = 0x24;
}
