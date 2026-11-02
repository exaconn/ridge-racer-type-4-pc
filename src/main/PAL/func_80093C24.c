#include "rr/draw_environment.h"
#include "m2c_macros.h"


void func_80093C24(void *arg0, s32 arg1, s32 arg2, s32 arg3, RrGpuRectangleFields *arg4) {
    M2C_FIELD(arg0, s8 *, 3) = 2;
    M2C_FIELD(arg0, s32 *, 4) = func_800940E0(arg1, arg2, arg3 & 0xFFFF);
    M2C_FIELD(arg0, s32 *, 8) = func_8009424C(arg4);
}
