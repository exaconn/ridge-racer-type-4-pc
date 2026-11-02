#include "rr/overlay_extra_labels.h"

void ovl662_func_80118238(u32 *arg0, void *arg1, s32 arg2) {
    char buf[4];
    s32 i;
    s32 offset;
    s32 x;
    s32 y;
    s32 clut;
    s32 color;
    s32 val;
    s32 x2;
    /* ASM_BARRIER: pin c to $v0 ($2) to match original v0/v1 register allocation */
    register s32 c __asm__("$2");

    x = D_800F3AFC[arg2].x04 + 8;
    y = D_800F3AFC[arg2].y06 + 72;
    for (i = 0; i < 4; i++) {
        buf[i] = 0;
    }
    for (i = 0, offset = i; i < 8; i++, offset += 12) {
        buf[0] = ovl662_D_801273A8[i];
        if (i == ovl662_D_801273A0) {
            clut = 0x7FF8;
            val = func_800904F4((s32)(((u32)D_800ABEB0 << 7) - 682U));
            c = 0xFF;
            if (val < 0) {
                /* ASM_BARRIER: prevent CSE from folding second val < 0 branch */
                __asm__("" : "+r"(val));
                clut = 0x7FF9;
            }
            if (val < 0) {
                c = 8;
            }
            color = c;
        } else {
            clut = 0x7FF8;
            color = 0xFF;
        }
        x2 = x + offset;
        arg1 = func_8001FD20(arg0, arg1, x2, y, 8, 2, color, color, color);
        arg1 = func_8001F5B4(arg0, arg1, x2 + 2, y - 20, buf, clut, 0, 0);
    }
    func_8001FCC8(arg0, arg1, 56);
}
