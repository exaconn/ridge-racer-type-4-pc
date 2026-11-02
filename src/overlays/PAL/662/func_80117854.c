#include "rr/overlay662_history_labels.h"

void ovl662_func_80117854(u32 *order, void *cursor, s32 row)
{
    s32 i, x, y;
    s32 flag = 0;

    x = D_800F3AFC[row].x04 + 8;
    y = D_800F3AFC[row].y06 + 32;
    for (i = 0; i < 4; i++) {
        if (D_800F4D98.bytes[0] == D_800F3920.history6C[i].selection) {
            flag = func_80090424(
                ((s32)(((u32)D_800ABEB0 << 7) - 682U)) & 0xFFE) < 0;
            break;
        }
    }
    for (i = 0; i < 7; i++) {
        if (ovl662_D_8012621C[i] == -1) {
            cursor = func_8001F5B4(
                order, cursor, x, y,
                func_80042D28((s16)ovl662_D_801272D8[D_800F4D98.bytes[0]], 80, 1),
                flag + 0x7FF8, 0, 0);
            x += 34;
            cursor = func_8001FD20(order, cursor, x, y - 2, 1, 20, 255, 255, 255);
            x += 8;
        } else if (ovl662_D_8012621C[i] == -2) {
            cursor = func_8001F5B4(
                order, cursor, x, y,
                func_80042D28((s16)ovl662_D_801272FC, 320, 2),
                flag + 0x7FF8, 0, 0);
        } else {
            cursor = func_8001FF84(order, cursor, x, y,
                &D_8009FA1C[ovl662_D_8012621C[i]], 0x7FF8, 0);
            x += D_8009FA1C[ovl662_D_8012621C[i]].w + 2;
        }
    }
    func_8001FCC8(order, cursor, 56);
}
