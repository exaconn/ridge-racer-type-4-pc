#include "rr/overlay659_preview_controller.h"

void ovl659_func_80115CE4(void) {
    /* Materialize the three real channels in their aligned argument object. */
    volatile RrPreviewRgb3 color;
    s32 renderMask;
    s32 drawFlags;
    s32 scale;
    s32 phase;
    u8 state4;
    u8 state6;
    s16 timer6;
    s16 timer3;
    register s32 row __asm__("$16");
    s32 idx;
    RrUiRow24 *rowPtr;

    renderMask = 0x1002E;
    D_800F2DE4 = (u32)D_800F2DE4 + 1;
    D_800ABEB0 = (u32)D_800ABEB0 + 1;
    D_800ABEF8[0].rot54 = ((u32)D_800ABEF8[0].rot54 + 8) & 0xFFF;
    phase = D_800AD4D8.phase17;
    switch (D_800F4D8A) {
    case 0:
    case 7:
        if (D_800F4D80[0].byte14 == 2) {
            if (D_800F4D8A == 0) {
                func_80052A30(1);
            }
            D_800F4D8A = 1;
            D_800AC64A = 0;
            func_8002AF8C(1, 1, 0);
        }
        break;
    case 1:
        D_800F4D80[3].cells[D_800AD4D8.phase17] = 2;
        D_800F4D80[D_800AC64A].cells[0] = 0;
        func_8002B330(0, 2, (char *)&D_800AC64A, 3, 0);
        D_800F4D80[D_800AC64A].cells[0] = 2;
        if (D_800F3A38[0].a != 0) {
            func_8004F24C(1);
            func_8002AF8C(0, 1, 0);
            D_800F4D80[D_800AC64A].cells[0] = 1;
            if (D_800AC64A == 0) {
                row = 0;
                idx = 0;
                do {
                    /* The compiler pointer width is retained throughout. */
                    rowPtr = (RrUiRow24 *)((unsigned long)(idx * sizeof(RrUiRow24)) + (unsigned long)D_800F4D80);
                    if (rowPtr->byte14 == 2) {
                        rowPtr->byte14 = 7;
                    }
                    row += 1;
                    idx++;
                } while (row < 4);
                D_800AC5D0 = 0x40;
                D_800F4D8A = 3;
                func_80052B8C(0, 0x3C);
            } else if (D_800AC64A == 1) {
                D_800F4D8A = 9;
                row = 0;
                idx = 0;
                do {
                    D_800F4D80[idx].byte14 = 3;
                    row += 1;
                    idx++;
                } while (row < 4);
            } else if (D_800AC64A == 2) {
                D_800AC5D0 = 0x40;
                row = 0;
                idx = 0;
                do {
                    D_800F4D80[idx].byte14 = 3;
                    row += 1;
                    idx++;
                } while (row < 4);
                D_800F4D8A = 6;
                func_80052B8C(0, D_800AC5D0 - 4);
            }
        }
        break;
    case 9:
        if (D_800F4D80[0].byte14 == 5) {
            D_800F4D8A = 2;
            func_80037110();
            D_800F4D80[6].byte14 = 0;
            goto block_6388;
        }
        goto draw_rows;
    case 2:
        state6 = D_800F4D80[6].byte14;
        if (state6 == 2) {
            func_8002AF8C(1, 1, 1);
            D_800F4D80[6].cells[0] = state6;
            if (D_800F3A38[0].a != 0) {
                func_8004F24C(1);
                D_800F4D80[4].byte14 = 0;
                D_800F4D80[5].byte14 = 0;
                D_800F4D80[6].cells[0] = 1;
                D_800F4D8A = 4;
            } else if (D_800F3A38[0].b != 0) {
                func_8004F24C(2);
                D_800F4D80[6].byte14 = 3;
                ((s8 *)D_800F2AB0)[1] = 3;
                D_800F4D8A = 10;
                D_800F3AFC->state10 = 4;
            }
        }
        goto block_6388;
    case 4:
        state4 = D_800F4D80[4].byte14;
        if (state4 == 2) {
            func_8002AF8C(1, 1, 0);
            D_800F4D80[D_800AD4D8.copies[0].bytes[3] + 4].cells[0] = 0;
            func_8002B330(0, 1, (char *)&D_800AD4D8.copies[0].bytes[3], 2, 0);
            D_800F4D80[D_800AD4D8.copies[0].bytes[3] + 4].cells[0] = state4;
            D_800F4D98 = D_800AD4D8.copies[0];
            if (D_800F3A38[0].a != 0) {
                func_8004F24C(1);
                D_800F4D80[D_800AD4D8.copies[0].bytes[3] + 4].cells[0] = 1;
                ((s8 *)D_800F2AB0)[1] = 3;
                D_800F4D80[4].byte14 = 3;
                D_800F4D80[5].byte14 = 3;
                D_800F4D80[6].byte14 = 3;
                D_800F4D8A = 5;
                D_800F3AFC->state10 = 4;
            }
        }
        goto block_6388;
    case 5:
    case 10:
        if (D_800F4D80[6].byte14 == 5) {
            D_800F4D80[0].byte14 = 0;
            D_800F4D80[1].byte14 = 0;
            D_800F4D80[2].byte14 = 0;
            D_800F4D80[3].byte14 = 0;
            D_800F4D8A = 7;
        }
        goto block_6388;
    case 6:
        timer6 = (u16)D_800AC5D0 - 1;
        D_800AC5D0 = timer6;
        func_800207D4(&D_800ACC14->head70, (0x40 - timer6) * 4, 0x45);
        if (D_800AC5D0 <= 0) {
            D_800F4C6A = 1;
            D_800F3A26 = 3;
            D_800F4D8A = 0;
        }
        goto draw_rows;
    case 3:
        timer3 = (u16)D_800AC5D0 - 1;
        D_800AC5D0 = timer3;
        func_800207D4(&D_800ACC14->head70, (0x40 - timer3) * 4, 0x45);
        if (D_800AC5D0 <= 0) {
            D_800F4D98 = D_800AD4D8.copies[0];
            D_800F4BE8 = 0;
            D_800F4C6A = 1;
            D_800F3A26 = 0x23;
            D_800F4D8A = 0;
        }
        break;
    }
    func_80033368(phase, 0);
    func_80032F44(phase, 0);
draw_rows:
    for (row = 0; row < 4; row++) {
        func_8003EC88(row);
    }
    goto end;
block_6388:
    func_8003FBA8(0);
    if (((s8 *)D_800F2AB0)[1] >= 2) {
        D_800F3AFC->half02 = 1;
        func_8003D6F0(0);
        func_8003EC88(6);
        if ((u16)(D_800F4D8A - 4) < 2U) {
            func_8003EC88(4);
            func_8003EC88(5);
        }
    }
    if ((u32)((u8)((s8 *)D_800F2AB0)[1] - 1) < 2U) {
        func_80033A74(1);
        func_8006EF8C(6);
        func_80037198(0);
        func_8002E5A0(D_800ABEF8);
        if (((s8 *)D_800F2AB0)[1] == 1) {
            s8 c = ~((D_800F2AB0[2] * 0xFF) / 30);
            color.r = c;
            color.g = c;
            color.b = c;
            drawFlags = 0x102;
            scale = (D_800F2AB0[2] << 8) / 30;
        } else if (((s8 *)D_800F2AB0)[1] == 2) {
            scale = 0x100;
            drawFlags = 1;
            color.r = 0x80;
            color.g = 0x80;
            color.b = 0x80;
        }
        func_8002CC88(D_800ABEF8, color, scale, drawFlags);
    }
    renderMask = 0x10022;
end:
    func_8002A344(renderMask);
}
