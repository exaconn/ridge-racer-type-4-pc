#include "types.h"
#include "m2c_macros.h"
#include "psyq/libgpu/addprim4.h"

void CatPrim(s32 *arg0, s32 arg1) {
    *arg0 = (*arg0 & 0xFF000000) | (arg1 & 0xFFFFFF);
}
