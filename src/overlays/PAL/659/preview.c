#include "rr/overlay659.h"

void ovl659_init_preview(void) {
    s32 index;
    s32 *scratch;

    if (D_800F4D8A == 0) {
        D_800F2DE4 = 0;
        ScreenFrame = 0;
        MenuCursor = 0;
        MenuItemCount = 3;
        func_80053CB0();
        ResetScreenCounters();
        ClearScreenOverlayState();
        InitMenuArenas();
        LoadCursor = D_800F4D74;
        InitUiRow(0, 0xC, 0x40, 0x44, 0x20, 0, 0, 1, 0, 0x54);
        InitUiRow(1, 0xC, 0x68, 0x44, 0x20, 0, 0, 1, 1, 0xC);
        InitUiRow(2, 0xC, 0x90, 0x44, 0x20, 0, 0, 1, 0, 0x1D);
        InitUiRow(3, 0xC, 0x170, 0xD0, 0x20, 0, 0, 8, 4, (s32)D_800AD4D8.phase17);
        InitUiRow(4, 0x74, 0x58, 0x1C, 0x20, 0, 0, 1, 0, 0x37);
        InitUiRow(5, 0x74, 0x80, 0x1C, 0x20, 0, 0, 1, 0, 0x38);
        InitUiRow(6, 0x18, 0x58, 0x44, 0x20, 0, 0, 1, 0, 0x53);
        index = 0;
        do {
            if (index == D_800AD4D8.phase17) {
                UiRows[3].cells[index] = 2;
            } else {
                UiRows[3].cells[index] = 0;
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
        PlayerCars[1].pos20[0] = 0;
        PlayerCars[1].pos20[1] = 0;
        PlayerCars[1].pos20[2] = 0;
        ResetCarState(PlayerCars);
        PlayerCars[0].pos20[1] = 0xA;
        PlayerCars[0].pos20[2] = 0x134;
        PlayerCars[0].rot54 = 0xA00;
        D_800F4D8A = 1;
    } else if (D_800F4D8A == 1) {
        if (func_8003BAB0() == 1) {
            D_800F4D8A = 2;
        }
    } else if (D_800F4D8A == 2) {
        CdAudioInit();
        CdAudioSelectTrack(0, 2);
        D_800F4C6A = 1;
        D_800F3A26 = 0x12;
        D_800F4D8A = 0;
    }
    UpdateScreenFade(0x1002E);
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
    ScreenFrame = (u32)ScreenFrame + 1;
    PlayerCars[0].rot54 = ((u32)PlayerCars[0].rot54 + 8) & 0xFFF;
    phase = D_800AD4D8.phase17;
    switch (D_800F4D8A) {
    case 0:
    case 7:
        if (UiRows[0].byte14 == 2) {
            if (D_800F4D8A == 0) {
                CdAudioBeginPlay(1);
            }
            D_800F4D8A = 1;
            MenuCursor = 0;
            func_8002AF8C(1, 1, 0);
        }
        break;
    case 1:
        UiRows[3].cells[D_800AD4D8.phase17] = 2;
        UiRows[MenuCursor].cells[0] = 0;
        StepMenuValue(0, 2, (char *)&MenuCursor, 3, 0);
        UiRows[MenuCursor].cells[0] = 2;
        if (D_800F3A38[0].a != 0) {
            PlaySfx(1);
            func_8002AF8C(0, 1, 0);
            UiRows[MenuCursor].cells[0] = 1;
            if (MenuCursor == 0) {
                row = 0;

                do {
                    if (UiRows[row].byte14 == 2) {
                        UiRows[row].byte14 = 7;
                    }
                    row += 1;
                } while (row < 4);
                D_800AC5D0 = 0x40;
                D_800F4D8A = 3;
                CdAudioFade(0, 0x3C);
            } else if (MenuCursor == 1) {
                D_800F4D8A = 9;
                row = 0;

                do {
                    UiRows[row].byte14 = 3;
                    row += 1;
                } while (row < 4);
            } else if (MenuCursor == 2) {
                D_800AC5D0 = 0x40;
                row = 0;

                do {
                    UiRows[row].byte14 = 3;
                    row += 1;
                } while (row < 4);
                D_800F4D8A = 6;
                CdAudioFade(0, D_800AC5D0 - 4);
            }
        }
        break;
    case 9:
        if (UiRows[0].byte14 == 5) {
            D_800F4D8A = 2;
            func_80037110();
            UiRows[6].byte14 = 0;
            goto draw_vehicle_preview;
        }
        goto draw_rows;
    case 2:
        state6 = UiRows[6].byte14;
        if (state6 == 2) {
            func_8002AF8C(1, 1, 1);
            UiRows[6].cells[0] = state6;
            if (D_800F3A38[0].a != 0) {
                PlaySfx(1);
                UiRows[4].byte14 = 0;
                UiRows[5].byte14 = 0;
                UiRows[6].cells[0] = 1;
                D_800F4D8A = 4;
            } else if (D_800F3A38[0].b != 0) {
                PlaySfx(2);
                UiRows[6].byte14 = 3;
                ((s8 *)D_800F2AB0)[1] = 3;
                D_800F4D8A = 10;
                BadgeRows->state10 = 4;
            }
        }
        goto draw_vehicle_preview;
    case 4:
        state4 = UiRows[4].byte14;
        if (state4 == 2) {
            func_8002AF8C(1, 1, 0);
            UiRows[D_800AD4D8.copies[0].bytes[3] + 4].cells[0] = 0;
            StepMenuValue(0, 1, (char *)&D_800AD4D8.copies[0].bytes[3], 2, 0);
            UiRows[D_800AD4D8.copies[0].bytes[3] + 4].cells[0] = state4;
            D_800F4D98.choice = D_800AD4D8.copies[0];
            if (D_800F3A38[0].a != 0) {
                PlaySfx(1);
                UiRows[D_800AD4D8.copies[0].bytes[3] + 4].cells[0] = 1;
                ((s8 *)D_800F2AB0)[1] = 3;
                UiRows[4].byte14 = 3;
                UiRows[5].byte14 = 3;
                UiRows[6].byte14 = 3;
                D_800F4D8A = 5;
                BadgeRows->state10 = 4;
            }
        }
        goto draw_vehicle_preview;
    case 5:
    case 10:
        if (UiRows[6].byte14 == 5) {
            UiRows[0].byte14 = 0;
            UiRows[1].byte14 = 0;
            UiRows[2].byte14 = 0;
            UiRows[3].byte14 = 0;
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
        DrawUiRow(row);
    }
    goto end;
draw_vehicle_preview:
    func_8003FBA8(0);
    if (((s8 *)D_800F2AB0)[1] >= 2) {
        BadgeRows->half02 = 1;
        UpdateBadgeRowAnim(0);
        DrawUiRow(6);
        if ((u16)(D_800F4D8A - 4) < 2U) {
            DrawUiRow(4);
            DrawUiRow(5);
        }
    }
    if ((u32)((u8)((s8 *)D_800F2AB0)[1] - 1) < 2U) {
        func_80033A74(1);
        SetupViewport(6);
        func_80037198(0);
        func_8002E5A0(PlayerCars);
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
        func_8002CC88(PlayerCars, color.argument, scale, drawFlags);
    }
    renderMask = 0x10022;
end:
    UpdateScreenFade(renderMask);
}
