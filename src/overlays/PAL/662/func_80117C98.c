#include "rr/overlay662_history_labels.h"

void ovl662_func_80117C98(u32 *arg0, void *arg1, s32 arg2) {
    char buf[8];
    s32 i;
    s32 x;
    s32 y;

    x = D_800F3AFC[arg2].x04 + 8;
    y = D_800F3AFC[arg2].y06 + 32;
    for (i = 0; i < 8; i++) {
        buf[i] = 0;
    }
    for (i = 0; i < 3; i++) {
        arg1 = func_8001FF84(arg0, arg1, x, y, &D_8009FA1C[ovl662_D_80126228[i]], 0x7FF8, 0);
        x += D_8009FA1C[ovl662_D_80126228[i]].w + 2;
        if (i == 2) {
            func_8002B814(buf, ovl662_D_801272BC, 1);
            arg1 = func_8001F5B4(arg0, arg1, x, y, buf, 0x7FF8, 0, 0);
        }
    }
    func_8001FCC8(arg0, arg1, 56);
}
