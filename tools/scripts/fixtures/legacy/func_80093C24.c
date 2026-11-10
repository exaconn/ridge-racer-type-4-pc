#include "rr/draw_environment.h"
#include "m2c_macros.h"


void SetDrawMode(void *arg0, s32 arg1, s32 arg2, s32 arg3, GpuRectangleFields *arg4) {
    M2C_FIELD(arg0, s8 *, 3) = 2;
    M2C_FIELD(arg0, s32 *, 4) = get_mode(arg1, arg2, arg3 & 0xFFFF);
    M2C_FIELD(arg0, s32 *, 8) = get_tw(arg4);
}
