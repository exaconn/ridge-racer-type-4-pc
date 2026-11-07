#include "rr/overlay663.h"

void ovl663_init_menu(void) {
    D_800F2DE4 = 0;
    ScreenFrame = 0;
    MenuCursor = 0;
    MenuItemCount = 3;
    ClearScreenOverlayState();
    ovl663_extra_courses_enabled = 0;
    ovl663_record_kind = 0;
    ResetScreenCounters();
    D_800F4D70.second = CdDirectoryBuffer + 0x2D000;
    InitMenuArenas();
    LoadCursor = D_800F4D70.second;
    InitUiRow(0, 118, 152, 68, 32, 0, 0, 1, 0, 122);
    InitUiRow(1, 118, 192, 68, 32, 0, 0, 1, 1, 16);
    InitUiRow(2, 118, 232, 68, 32, 0, 0, 1, 1, 17);
    InitUiRow(3, 118, 272, 68, 32, 0, 0, 1, 0, 85);
    if (!ovl663_available_course_count)
        UiRows->half20 = 1;
    SetupRenderer(5);
}

void ovl663_init_scene(void) {
    s32 i;
    s32 val;

    SetDispMask(0);
    func_8001ECE8(0, 0, 0, 2);
    D_800F4C6A = 1;
    D_800F3A26 = 74;
    D_800F4D8A = 0;
    func_8001E84C(ovl663_scene_name);
    ovl663_available_course_count = 0;
    for (i = 0; i < 9; i++) {
        if (D_800F3920.unlocked60[ovl663_course_order[i]] != 0) {
            ovl663_available_courses[ovl663_available_course_count] = ovl663_course_order[i];
            ovl663_course_unlock_levels[ovl663_available_course_count++] =
                D_800F3920.unlocked60[ovl663_course_order[i]];
        }
    }
    ovl663_init_menu();
    func_80053BB4(0);
    ovl663_course_count = 4;
    val = D_800F3920.prefix.progress.prefix00.fields.configuration0C;
    if (val >= 16) {
        ovl663_course_count = 8;
    }
    if (val & 0x100) {
        ovl663_course_count = 16;
    }
}

void ovl663_update_menu(void) {
    s32 cancelSound;
    s16 selected;
    s32 loaded;
    s16 closingTimer;
    s16 cancelTimer;
    s32 i;

    s32 closingMode;
    s32 highlight;
    u8 rowState;

    D_800F2DE4 = (s32)((u32)D_800F2DE4 + 1U);
    ScreenFrame = (s32)((u32)ScreenFrame + 1U);
    switch (D_800F4D8A) {
    case 0:
        i = 0;
        if (UiRows->byte14 == 2) {
            func_8002AF8C(1, 1, 1);
            UiRows[MenuCursor].cells[0] = 0;
            StepMenuValue(0, 2, (char *)&MenuCursor, 4, 0);
            highlight = 2;
            if (ovl663_available_course_count == 0) {
                if (MenuCursor == 0) {
                    highlight = 4;
                }
            }
            UiRows[MenuCursor].cells[0] = highlight;
            closingMode = 0;
            if (D_800F3A38[0].a != 0) {
                UiRows[MenuCursor].cells[0] = 1;
                selected = MenuCursor;
                if (selected == 0) {
                    if (ovl663_available_course_count > 0) {
                        PlaySfx(1);
                        closingMode = 2;
                        ResetScreenCounters();
                        D_800F4D8A = 3;
                        LoadCursor = D_800F4D40;
                    } else {
                        PlaySfx(4);
                    }
                    goto chosen;
                } else if (selected == 1) {
                    PlaySfx(1);
                    closingMode = 2;
                    ovl663_record_kind = selected;
                    D_800F4D8A = 4;
                    goto chosen;
                } else if (selected == 2) {
                    PlaySfx(1);
                    ovl663_record_kind = selected;
                    closingMode = 2;
                    D_800F4D8A = 5;
                    goto chosen;
                } else {
                    if (selected != 3)
                        goto chosen;
                    cancelSound = 1;
                    goto cancel;
                }
            } else {
                cancelSound = 2;
                if (D_800F3A38[0].b != 0) {
                cancel:
                    PlaySfx(cancelSound);
                    D_800F4D8A = 8;
                    closingMode = 1;
                }
            }
        chosen:
            if (closingMode > 0) {
                func_8002AF8C(0, 1, 1);
                D_800AC5D0 = 0x40;
                i = 0;

                do {
                    rowState = UiRows[i].byte14;
                    if (rowState == 2) {
                        if (closingMode == rowState) {
                            UiRows[i].byte14 = 7U;
                        } else {
                            UiRows[i].byte14 = 3U;
                            func_80053AF4(0, (s16)(D_800AC5D0 - 4));
                        }
                    }
                    i += 1;
                } while (i < 4);
                i = 0;
            }
        }
        break;
    case 3:
        if ((s16)D_800AC5D0 > 0) {
            D_800AC5D0 = (u16)D_800AC5D0 - 1;
        }
        func_800207D4(D_800ACC14->ordering.ordering70, (0x40 - (s16)D_800AC5D0) * 4, 0x45);
        i = 0;
        if ((s16)D_800AC5D0 <= 0) {
            loaded = func_8003BEA8(ovl663_available_courses[0]);
            if (loaded == 1) {
                ovl663_init_preview();
                D_800F4C6A = loaded;
                D_800F3A26 = 0x4C;
                D_800F4D8A = 0;
                return;
            }
            break;
        }
        break;
    case 4:
    case 5:
        closingTimer = (u16)D_800AC5D0 - 1;
        D_800AC5D0 = (u16)closingTimer;
        func_800207D4(D_800ACC14->ordering.ordering70, (0x40 - closingTimer) * 4, 0x45);
        i = 0;
        if ((s16)D_800AC5D0 <= 0) {
            ovl663_init_selection(ovl663_record_kind);
            D_800F4C6A = 1;
            D_800F3A26 = 75;
            D_800F4D8A = 0;
        }
        break;
    case 8:
        cancelTimer = (u16)D_800AC5D0 - 1;
        D_800AC5D0 = (u16)cancelTimer;
        func_800207D4(D_800ACC14->ordering.ordering70, (0x40 - cancelTimer) * 4, 0x45);
        i = 0;
        if ((s16)D_800AC5D0 <= 0) {
            func_80053CB0();
            D_800F4C6A = 1;
            D_800F3A26 = 3;
            D_800F4D8A = 0;
        }
        break;
    default:
        break;
    }
    i = 0;
    do {
        DrawUiRow(i);
        i += 1;
    } while (i < 4);
    UpdateScreenFade(0x10057);
}

void ovl663_init_selection(s32 mode) {
    s32 one = 1;
    D_800F2DE4 = 0;
    ScreenFrame = 0;
    D_800AC648 = 0;
    D_800F3914 = 0;
    MenuCursor = 0;
    ovl663_selected_record = 0;
    ClearScreenOverlayState();
    InitUiRow(0, 12, 64, 68, 32, 0, 0, one, one, 24);
    InitUiRow(1, 12, 104, 68, 32, 0, 0, one, 0, 85);
    InitUiRow(2, 12, 368, 208, 32, 0, 0, 8, 4, 0);
    InitUiRow(3, 24, 88, 68, 32, 0, 0, one, 0, 83);
    if (mode == 2)
        InitBadgeRow(one, 49, 188, 222, 152, 0, 0xB04);
    else
        InitBadgeRow(one, 49, 188, 222, 152, 0, 0xA04);
    BadgeRows[1].half02 = one;
}
