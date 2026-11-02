#include "rr/overlay662_setup.h"

void ovl662_func_80115EC4(s32 arg0) {
    s32 i;
    ovl662_D_801272C0 = 0;
    D_800F3826 = 0;
    D_800F4D99 = 0;
    for (i = 0; i < 4; i++) {
        /* Unsigned addition wraps; the PAL signed-word conversion keeps its bits. */
        D_800F4D8C[arg0 + i].phase18 = (s32)((u32)D_800F4D8C[i].phase18 + 0x30000U);
        func_8003E040(arg0 + i, -2, 0, 1);
        func_8003DC9C(
            D_800F4D8C[arg0 + i].glyph07,
            D_800F4D8C[arg0 + i].x08,
            D_800F4D8C[arg0 + i].y0A,
            D_800F4D8C[arg0 + i].w0C,
            D_800F4D8C[arg0 + i].h0E,
            0, 0
        );
    }
    func_8003D228(0, 108, 336, 112, 34, 0, 0x401);
    func_8003E928(0, 40, 72, 192, 32, 0, 0, 4, 3, D_800F3826 + 4);
    D_800F3AFC[0].half02 = 1;
}
