#include "rr/overlay663.h"

void ovl663_update_selection(void) {
    s32 car;
    s32 renderMask;
    s32 modelReady;
    s16 fadeTimer;
    s32 mirrorSet;
    s32 i;
    s32 glyphIndex;

    D_800F2DE4 = (u32)D_800F2DE4 + 1;
    ScreenFrame = (u32)ScreenFrame + 1;
    switch ((u16)D_800F4D8A) {
    case 0:
        i = 0;
        if (BadgeRows[1].state10 == 3) {
            D_800F4D8A = 1;
            func_8002AF8C(1, 1, 1);
            i = 0;
        }
        break;
    case 1:
        func_8002B12C(0xA0, 0x108, 0x76, 0, (s32)D_800AC648, 0, ovl663_course_count);
        StepMenuValue(1, 2, (char *)&D_800AC648, ovl663_course_count, 0);
        i = 0;
        D_800F3914 = DecodeCourseIndex(D_800AC648);
        while (i < (s32)UiRows[2].byte02) {
            if (i == ((u16)D_800AC648 & 7)) {
                UiRows[2].cells[i] = (s8)((u8)D_800AD522 + 2);
                UiRows[2].byte16 = i;
            } else if (i >= ovl663_course_count) {
                UiRows[2].cells[i] = 1;
            } else {
                UiRows[2].cells[i] = (u8)D_800AD522;
            }
            i += 1;
        }
        UiRows[MenuCursor].cells[0] = 0;
        StepMenuValue(0, 2, (char *)&MenuCursor, 2, 0);
        i = 0;
        UiRows[MenuCursor].cells[0] = 2;
        ovl663_selected_record = -1;
        if (D_800F3A38[0].a != 0) {
            PlaySfx(1);
            UiRows[MenuCursor].cells[0] = 1;
            if (MenuCursor == 0) {
                ovl663_selected_record = 0;
                D_800F4D8A = 2;
            } else if (MenuCursor == 1) {
                i = 1;
            }
        } else if (D_800F3A38[0].b != 0) {
            PlaySfx(2);
            i = 1;
        }
        if (i == 1) {
            func_8002AF8C(0, 1, 1);
            i = 0;
            D_800AC5D0 = 0x40;
            D_800F4D8A = 8;
            do {
                UiRows[i].byte14 = 3;
                i += 1;
            } while (i < 3);
            BadgeRows[1].state10 = 4;
        }
        break;
    case 2:
        StepMenuValue(0, 4, (char *)&ovl663_selected_record, 5, 0);
        if (D_800F3A38[0].b != 0) {
            PlaySfx(2);
            D_800F4D8A = 1;
        } else if (D_800F3A38[0].a != 0) {
            func_8002AF8C(0, 1, 1);
            D_800F4D8A = 3;
            ResetScreenCounters();
            LoadCursor = D_800F4D40;
            mirrorSet = D_800AC648 >= 8;
            if (ovl663_record_kind == 1) {
                car = D_800FF700[mirrorSet][D_800F3914][ovl663_selected_record].car;
            } else {
                car = D_800AD5C8[mirrorSet][D_800F3914][ovl663_selected_record].car;
            }
            UnpackChoiceGlyph((s8 *)&D_800F4D98, car);
            if (D_800F4D98.choice.bytes[2] == 0x27) {
                if (D_800F4D98.choice.bytes[0] != 2) {
                    glyphIndex = D_800F4D98.choice.bytes[1] + (D_800F4D98.choice.bytes[0] * 4);
                } else {
                    glyphIndex = 0;
                }
                D_800ACBD0[0].first = (s16)(s8)D_800ABE08[0][glyphIndex].first;
                D_800ACBD0[0].second = 0;
            } else {
                D_800ACBD0[0].first = 0;
                D_800ACBD0[0].second = 0;
            }
            i = 0;
            do {
                UiRows[i].byte14 = 3;
                i += 1;
            } while (i < 3);
            BadgeRows[1].state10 = 4;
        }
        break;
    case 3:
        modelReady = func_8003AE4C(0, 1);
        if ((modelReady == 1) && (UiRows->byte14 == 5)) {
            ovl663_init_result_preview();
            D_800F4C6A = modelReady;
            D_800F3A26 = 0x4D;
            D_800F4D8A = 0;
        }
        break;
    case 8:
        fadeTimer = (u16)D_800AC5D0 - 1;
        D_800AC5D0 = (u16)fadeTimer;
        func_800207D4(&D_800ACC14->ordering.ordering70[0], (0x40 - fadeTimer) * 4, 0x45);
        i = 0;
        if ((s16)D_800AC5D0 <= 0) {
            ovl663_init_menu();
            D_800F4C6A = 1;
            D_800F3A26 = 0x4A;
            D_800F4D8A = 0;
            return;
        }
        break;
    default:
        break;
    }
    i = 0;
    do {
        DrawUiRow(i);
        i += 1;
    } while (i < 3);
    UpdateBadgeRowAnim(1);
    renderMask = 0x10063;
    if (ovl663_record_kind == 1) {
        renderMask = 0x1005F;
    }
    UpdateScreenFade(renderMask);
}
