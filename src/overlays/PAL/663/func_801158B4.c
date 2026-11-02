#include "rr/overlay663_preview_draw.h"
void ovl663_func_801158B4(void)
{
    RrPreviewMatrixWorkspace mat;
    s16 rot[4];
    RrGpuRectangleFields light;
    RrRaceVehicle *state;

    func_8006E1AC();
    func_8006EF8C(6);
    state = D_800ABEF8;
    state->rot54 = (s32)(((u32)state->rot54 + 8U) & 0xFFFU);
    rot[0] = state->rot50;
    rot[1] = state->rot54;
    rot[2] = state->rot58;
    func_80091844(rot, mat.halves);
    light.x = 100;
    light.y = 80;
    light.w = 120;
    light.h = 280;
    func_80019D5C(&light, 128, ((RrFrameOrderingPrefix *)D_800ACC14)->ordering70);
    func_80019C90(4096);
    func_800195BC((char *)state->pos20, mat.halves);
}
