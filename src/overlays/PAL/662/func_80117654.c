#include "rr/overlay662_history_labels.h"

void ovl662_func_80117654(u32 *arg0, void *arg1, s32 arg2) {
    char buf[8];
    s32 i;
    s32 x;
    s32 y;
    s32 flag = 0;

    x = BadgeRows[arg2].x04 + 8;
    y = BadgeRows[arg2].y06 + 32;
    for (i = 0; i < 4; i++) {
        if (D_800F4D98.bytes[2] == D_800F3920.history6C[i].parameter &&
            D_800F4D98.bytes[0] == D_800F3920.history6C[i].selection &&
            D_800F4D98.bytes[1] == D_800F3920.history6C[i].variant) {
            flag = rsin(((s32)(((u32)ScreenFrame << 7) - 682U)) & 0xFFE) < 0;
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
            FormatDecimalDigits(buf, D_800F4D98.bytes[2] + 1, 1);
            arg1 = AddTextSprites(arg0, arg1, x, y, buf, flag + 0x7FF8, 0, 0);
        }
    }
    AddDrawModePrim(arg0, arg1, 56);
}
