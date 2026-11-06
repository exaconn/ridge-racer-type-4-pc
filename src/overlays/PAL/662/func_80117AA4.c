#include "rr/overlay662_history_labels.h"

void ovl662_func_80117AA4(u32 *arg0, void *arg1, s32 arg2) {
    s32 i;
    s32 x;
    s32 y;
    s32 flag = 0;

    x = D_800F3AFC[arg2].x04 + 8;
    y = D_800F3AFC[arg2].y06 + 32;
    for (i = 0; i < 4; i++) {
        if (D_800F4D98.bytes[0] == D_800F3920.history6C[i].selection &&
            D_800F4D98.bytes[1] == D_800F3920.history6C[i].variant) {
            flag = rsin(((s32)(((u32)ScreenFrame << 7) - 682U)) & 0xFFE) < 0;
            break;
        }
    }
    for (i = 0; i < 3; i++) {
        if (ovl662_D_80126224[i] == -1) {
            arg1 = func_8001F5B4(
                arg0, arg1, x, y,
                func_80042D28(ovl662_D_80127328[D_800F4D98.bytes[0]][D_800F4D98.bytes[1]], 20, 1),
                flag + 0x7FF8, 0, 0);
            break;
        }
        arg1 = func_8001FF84(arg0, arg1, x, y, &D_8009FA1C[ovl662_D_80126224[i]], 0x7FF8, 0);
        /* ASM_BARRIER: boost arg1 register allocation priority to $s2 */
        __asm__("" : "+r"(arg1));
        x += D_8009FA1C[ovl662_D_80126224[i]].w + 2;
    }
    func_8001FCC8(arg0, arg1, 56);
}
