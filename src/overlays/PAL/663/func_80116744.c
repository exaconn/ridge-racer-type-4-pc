#include "rr/overlay_extra_labels.h"

void *ovl663_func_80116744(u32 *arg0, void *arg1, s32 arg2)
{
    char buf[8];
    s32 i;
    s32 x;
    s32 y;

    x = D_800F3AFC[arg2].x04 + 8;
    y = D_800F3AFC[arg2].y06 + 32;
    for (i = 0; i < 8; i++) {
        buf[i] = 0;
    }
    arg1 = func_8001FF84(arg0, arg1, x, y, &ovl663_D_80116890, 0x7FF8, 0);
    x += ovl663_D_80116890.w;
    func_8002B814(buf, ovl663_D_801168D0[ovl663_D_80116898], 1);
    arg1 = func_8001F5B4(arg0, arg1, x, y, buf, 0x7FF8, 0, 0);
    return func_8001FCC8(arg0, arg1, 56);
}
