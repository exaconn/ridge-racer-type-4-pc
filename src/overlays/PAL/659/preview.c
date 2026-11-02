#include "rr/overlay659.h"

void ovl659_init_preview(void) {
    s32 index;
    s32 *scratch;

    if (D_800F4D8A == 0) {
        D_800F2DE4 = 0;
        D_800ABEB0 = 0;
        D_800AC64A = 0;
        D_800AC5E0 = 3;
        func_80053CB0();
        func_8003A198();
        func_8002A604();
        func_8003A1B4();
        D_800ACC18 = D_800F4D74;
        func_8003E928(0, 0xC, 0x40, 0x44, 0x20, 0, 0, 1, 0, 0x54);
        func_8003E928(1, 0xC, 0x68, 0x44, 0x20, 0, 0, 1, 1, 0xC);
        func_8003E928(2, 0xC, 0x90, 0x44, 0x20, 0, 0, 1, 0, 0x1D);
        func_8003E928(3, 0xC, 0x170, 0xD0, 0x20, 0, 0, 8, 4, (s32)D_800AD4D8.phase17);
        func_8003E928(4, 0x74, 0x58, 0x1C, 0x20, 0, 0, 1, 0, 0x37);
        func_8003E928(5, 0x74, 0x80, 0x1C, 0x20, 0, 0, 1, 0, 0x38);
        func_8003E928(6, 0x18, 0x58, 0x44, 0x20, 0, 0, 1, 0, 0x53);
        index = 0;
        do {
            if (index == D_800AD4D8.phase17) {
                D_800F4D80[3].cells[index] = 2;
            } else {
                D_800F4D80[3].cells[index] = 0;
            }
            index += 1;
        } while (index < 8);
        scratch = RR_MENU_PREVIEW_SCRATCH_WORDS;
        scratch[6] = 0x200;
        scratch[7] = 0;
        scratch[8] = 0;
        scratch[2] = 0;
        scratch[3] = -0x1A0;
        scratch[4] = 0;
        D_800ABEF8[1].pos20[0] = 0;
        D_800ABEF8[1].pos20[1] = 0;
        D_800ABEF8[1].pos20[2] = 0;
        func_8002E4F4(D_800ABEF8);
        D_800ABEF8[0].pos20[1] = 0xA;
        D_800ABEF8[0].pos20[2] = 0x134;
        D_800ABEF8[0].rot54 = 0xA00;
        D_800F4D8A = 1;
    } else if (D_800F4D8A == 1) {
        if (func_8003BAB0() == 1) {
            D_800F4D8A = 2;
        }
    } else if (D_800F4D8A == 2) {
        func_80052800();
        func_800528A8(0, 2);
        D_800F4C6A = 1;
        D_800F3A26 = 0x12;
        D_800F4D8A = 0;
    }
    func_8002A344(0x1002E);
}

void ovl659_update_preview(void) {

    RrPreviewColor color;
    s32 renderMask;
    s32 drawFlags;
    s32 scale;
    s32 phase;
    u8 state4;
    u8 state6;
    s16 timer6;
    s16 timer3;
    s32 row;

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

                do {
                    if (D_800F4D80[row].byte14 == 2) {
                        D_800F4D80[row].byte14 = 7;
                    }
                    row += 1;
                } while (row < 4);
                D_800AC5D0 = 0x40;
                D_800F4D8A = 3;
                func_80052B8C(0, 0x3C);
            } else if (D_800AC64A == 1) {
                D_800F4D8A = 9;
                row = 0;

                do {
                    D_800F4D80[row].byte14 = 3;
                    row += 1;
                } while (row < 4);
            } else if (D_800AC64A == 2) {
                D_800AC5D0 = 0x40;
                row = 0;

                do {
                    D_800F4D80[row].byte14 = 3;
                    row += 1;
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
            goto draw_vehicle_preview;
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
        goto draw_vehicle_preview;
    case 4:
        state4 = D_800F4D80[4].byte14;
        if (state4 == 2) {
            func_8002AF8C(1, 1, 0);
            D_800F4D80[D_800AD4D8.copies[0].bytes[3] + 4].cells[0] = 0;
            func_8002B330(0, 1, (char *)&D_800AD4D8.copies[0].bytes[3], 2, 0);
            D_800F4D80[D_800AD4D8.copies[0].bytes[3] + 4].cells[0] = state4;
            D_800F4D98.choice = D_800AD4D8.copies[0];
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
        goto draw_vehicle_preview;
    case 5:
    case 10:
        if (D_800F4D80[6].byte14 == 5) {
            D_800F4D80[0].byte14 = 0;
            D_800F4D80[1].byte14 = 0;
            D_800F4D80[2].byte14 = 0;
            D_800F4D80[3].byte14 = 0;
            D_800F4D8A = 7;
        }
        goto draw_vehicle_preview;
    case 6:
        timer6 = (u16)D_800AC5D0 - 1;
        D_800AC5D0 = timer6;
        func_800207D4(&D_800ACC14->ordering.ordering70[0], (0x40 - timer6) * 4, 0x45);
        if (D_800AC5D0 <= 0) {
            D_800F4C6A = 1;
            D_800F3A26 = 3;
            D_800F4D8A = 0;
        }
        goto draw_rows;
    case 3:
        timer3 = (u16)D_800AC5D0 - 1;
        D_800AC5D0 = timer3;
        func_800207D4(&D_800ACC14->ordering.ordering70[0], (0x40 - timer3) * 4, 0x45);
        if (D_800AC5D0 <= 0) {
            D_800F4D98.choice = D_800AD4D8.copies[0];
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
draw_vehicle_preview:
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
            color.channels.r = c;
            color.channels.g = c;
            color.channels.b = c;
            drawFlags = 0x102;
            scale = (D_800F2AB0[2] << 8) / 30;
        } else if (((s8 *)D_800F2AB0)[1] == 2) {
            scale = 0x100;
            drawFlags = 1;
            color.channels.r = 0x80;
            color.channels.g = 0x80;
            color.channels.b = 0x80;
        }
        func_8002CC88(D_800ABEF8, color.argument, scale, drawFlags);
    }
    renderMask = 0x10022;
end:
    func_8002A344(renderMask);
}
