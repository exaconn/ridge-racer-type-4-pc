#include "rr/overlay662_history_labels.h"

void ovl662_func_80117654(u32 *arg0, void *arg1, s32 arg2) {
    char buf[8];
    s32 i;
    s32 x;
    s32 y;
    s32 flag = 0;

    x = D_800F3AFC[arg2].x04 + 8;
    y = D_800F3AFC[arg2].y06 + 32;
    for (i = 0; i < 4; i++) {
        if (D_800F4D98.bytes[2] == D_800F3920.history6C[i].parameter &&
            D_800F4D98.bytes[0] == D_800F3920.history6C[i].selection &&
            D_800F4D98.bytes[1] == D_800F3920.history6C[i].variant) {
            flag = func_80090424(((s32)(((u32)D_800ABEB0 << 7) - 682U)) & 0xFFE) < 0;
            break;
        }
    }
    for (i = 0; i < 8; i++) {
        buf[i] = 0;
    }
    for (i = 0; i < 3; i++) {
        arg1 = func_8001FF84(arg0, arg1, x, y, &D_8009FA1C[ovl662_D_80126218[i]], 0x7FF8, 0);
        x += D_8009FA1C[ovl662_D_80126218[i]].w + 2;
        if (i == 2) {
            func_8002B814(buf, D_800F4D98.bytes[2] + 1, 1);
            arg1 = func_8001F5B4(arg0, arg1, x, y, buf, flag + 0x7FF8, 0, 0);
        }
    }
    func_8001FCC8(arg0, arg1, 56);
}
