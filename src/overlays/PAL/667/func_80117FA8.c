#include "rr/overlay667_course_menu.h"

void ovl667_func_80117FA8(void) {
    s32 previewIndex;
    s32 waitResult;
    s32 backIndex;
    s32 close;
    s32 row;
    s32 cellCount;
    u16 previousCourse;
    u8 activeState;
    s32 detailState;
    RrUiRow24 *preview;
    RrUiRow24 *back;

    if (func_80034FC0() != 0) {
        func_80053CB0();
        return;
    }
    waitResult = func_80035024(2);
    D_800F2DE4 = (s32)((u32)D_800F2DE4 + 1U);
    ScreenFrame = (s32)((u32)ScreenFrame + 1U);
    if ((D_800F4D8A == 3) || (func_8003BAB0() == 1)) {
        switch (D_800F4D8A) {                       /* irregular */
        case 0:
            func_8002AF8C(1, 1, 1);
            previousCourse = D_800AC648;
            StepMenuValue(3, 2, (char *)&D_800AC648, D_8010C078, 0);
            if ((s16) previousCourse != (s16) D_800AC648) {
                ResetScreenCounters();
            } else {
                func_80033368((s16) previousCourse, 2);
            }
            D_800F3914 = DecodeCourseIndex((s16) D_800AC648);
            UiRows[MenuCursor+18].cells[0] = 0;
            StepMenuValue(0, 2, (char *)&MenuCursor, 3, 0);
            UiRows[MenuCursor+18].cells[0] = 2;
            row = 0;
            if (UiRows[21].byte02 != 0) {
                do {
                    if (row == (s16) D_800AC648) {
                        UiRows[21].cells[row] = 2;
                        UiRows[21].byte16 = row;
                    } else {
                        UiRows[21].cells[row] = 0;
                    }
                    cellCount=UiRows[21].byte02;
                    row += 1;
                } while (row < cellCount);
            }
            activeState = UiRows[18].byte14;
            close = 0;
            if (activeState == 2) {
                if (D_800F3A38[0].a != 0) {
                    PlaySfx(1);
                    UiRows[MenuCursor+18].cells[0] = 1;
                    if ((s16) MenuCursor == 0) {
                        row = 0;
                        previewIndex = 0;
                        do {
                            preview = (RrUiRow24 *)((unsigned long)(previewIndex*sizeof(RrUiRow24))+(unsigned long)UiRows+0x288);
                            if (preview->byte14 == 2) {
                                preview->byte14 = 7U;
                            }
                            row += 1;
                            previewIndex++;
                        } while (row < 4);
                        ResetScreenCounters();
                        D_800AC5D0 = 0x40;
                        LoadCursor = D_800F4D70.first;
                        func_80053AF4(0, 0x3C);
                        func_80034F80();
                        D_800F4D8A = 3;
                        func_8002AF8C(0, 1, 1);
                    } else if ((s16) MenuCursor == 1) {
                        UiRows[23].byte14 = 0U;
                        D_800F4D8A = 4;
                    } else if ((s16) MenuCursor == activeState) {
                        close = 1;
                    }
                } else if (D_800F3A38[0].b != 0) {
                    PlaySfx(2);
                    close = 1;
                }
                if (close == 1) {
                    func_8002AF8C(0, 1, 1);
                    row = 0;
                    backIndex = 0;
                    do {
                        back = (RrUiRow24 *)((unsigned long)(backIndex*sizeof(RrUiRow24))+(unsigned long)UiRows+0x288);
                        if (back->byte14 == 2) {
                            back->byte14 = 3U;
                        }
                        row += 1;
                        backIndex++;
                    } while (row < 5);
                    D_800F4D8A = 8;
                }
            }
            break;
        case 3:
            func_80033368((s16) D_800AC648, 2);
            if ((waitResult == 0) && (D_800AC5D0 == 0x40)) {
                DrawUiRow(0x18);
            } else {
                D_8010C0B8 = 1;
                if (D_800AC5D0 > 0) {
                    D_800AC5D0 = (s16)((u16) D_800AC5D0 - 1);
                }
                func_800207D4(((RrFrameOrderingPrefix *)(void *)D_800ACC14)->ordering70, (0x40 - D_800AC5D0) * 4, 0x45);
                if (D_800AC5D0 <= 0) {
                    D_8010C0C8 = -1;
                    func_80053CB0();
                    D_800F4C6A = 1;
                    D_800F3A26 = 0x29;
                    D_800F4D8A = 0;
                    D_800FF688 = 0;
                    _sio_control(3, 0, 0);
                }
            }
            break;
        case 4:
            detailState = UiRows[23].byte14;
            if (detailState == 2) {
                func_8002AF8C(1, 1, 0);
                UiRows[23].cells[0] = detailState;
                StepMenuValue(1, 4, (char *)&NumLaps, 0xA, (s32) detailState);
                if (D_800F3A60 != 0) {
                    PlaySfx(1);
                    UiRows[23].cells[0] = 1U;
                    UiRows[23].byte14 = 3U;
                }
            } else if (detailState == 5) {
                D_800F4D8A = 0;
            }
            func_80033368((s16) D_800AC648, 2);
            break;
        case 8:
            if (UiRows[18].byte14 == 5) {
                ovl667_func_801161A8((1 - D_800AD508) * 2);
                D_800F4C6A = 1;
                D_800F3A26 = 0x3C;
                D_800F4D8A = 0;
            }
            break;
        }
    }
    func_80032F44((s16) D_800AC648, 2);
    row = 0;
    do {
        DrawUiRow(row + 0x12);
        row += 1;
    } while (row < 4);
    if (UiRows[18].byte14 == 2) {
        if (D_800F4D8A != 4) {
            func_8002B12C(0xCE, 0xD0, 0x5E, 0, (s32) (s16) D_800AC648, -1, (s32)((u32)D_8010C078 + 1U));
            goto block_63;
        }
        goto block_64;
    }
block_63:
    if (D_800F4D8A == 4) {
block_64:
        DrawUiRow(0x17);
    }
    D_800F4CB8 = 0;
    UpdateScreenFade(0x10026);
}
