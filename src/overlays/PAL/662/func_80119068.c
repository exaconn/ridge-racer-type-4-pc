#include "game/system/pad.h"
#include "rr/overlay662_course_controller.h"
#include "rr/frame_storage_types.h"
#include "rr/load_cursor.h"
extern s16 ovl662_D_801272BC;
extern s32 ovl662_D_801272E8;
void ovl662_func_8011512C(void);

void ovl662_func_80119068(void) {
    PreviewColor color;
    s8 active;
    s32 ready;
    s32 i;
    s32 cell;
    s32 phase;
    s16 prev;
    s16 fade;
    s32 tint;
    s32 mode;
    s8 shade;
    u8 row;
    RaceVehicle *vehicle;
    UiRow24 *p;
    s32 *tick;

    active = 0;
    tick = &ModeFrame;
    vehicle = PlayerCars;
    *tick += 1;
    vehicle->rot54 = (vehicle->rot54 + 8) & 0xFFF;
    SetupMenuCarCamera(1);
    SetupViewport(6);
    ready = LoadCarRangeStep(0, 1);
    if (ready != 1) {
        if (((s8 *)ImageRows)[1] == 1) {
            ((s8 *)ImageRows)[1] = 0;
        }
    } else {
        SetCarBounds(vehicle);
        if (((s8 *)ImageRows)[1] == 7) {
            ((s8 *)ImageRows)[1] = ready;
        }
        active = 1;
        switch ((u16)D_800F4D8A) {
        case 0:
            SetMenuFrameOptions(1, 1, 1);
            prev = ovl662_D_801272BC;
            StepMenuValue(3, 2, (char*)&ovl662_D_801272BC, 17, 0);
            if (prev != ovl662_D_801272BC) {
                SelectionRows.pad02[0] %= 20;
                if (ovl662_D_801272BC > 0) {
                    SelectionRows.pad02[0] += 20;
                }
                PlayerCourseChoices[0].first = ovl662_D_801272BC;
                ResetScreenCounters();
                LoadCursor = MenuArenas.first;
                ((s8 *)ImageRows)[1] = 6;
            }
            UiRows[MenuCursor].cells[0] = 0;
            StepMenuValue(0, 2, (char*)&MenuCursor, 3, 0);
            cell = 2;
            if (ovl662_D_801272BC == 0) {
                UiRows[1].half20 = 1;
                if (MenuCursor == 1) {
                    cell = 4;
                }
            } else {
                UiRows[1].half20 = 0;
            }
            UiRows[MenuCursor].cells[0] = cell;
            row = UiRows[0].byte14;
            if (row != 2) break;
            phase = BadgeRows->state10;
            if (phase != 3) break;
            if (PlayerControllers[0].action28 != 0) {
                UiRows[MenuCursor].cells[0] = 1;
                if (MenuCursor == 0) {
                    SetModelTexturePage(-1);
                    PlaySfx(1);
                    D_800F4D8A = phase;
                    i = 0;
                    do {
                        p = (UiRow24 *)(i * sizeof(UiRow24) + (unsigned long)UiRows);
                        if (p->byte14 == 2) {
                            p->byte14 = 3;
                        }
                        i++;
                    } while (i < 4);
                    BadgeRows[0].state10 = 4;
                    BadgeRows[1].state10 = 4;
                    ((s8 *)ImageRows)[1] = 3;
                    D_800AC5D0 = 0x40;
                } else if (MenuCursor == 1) {
                    if (ovl662_D_801272BC > 0) {
                        SetModelTexturePage(-1);
                        PlaySfx(1);
                        D_800F4D8A = 4;
                        i = 0;
                        do {
                            p = (UiRow24 *)(i * sizeof(UiRow24) + (unsigned long)UiRows);
                            if (p->byte14 == 2) {
                                p->byte14 = 3;
                            }
                            i++;
                        } while (i < 4);
                        BadgeRows[0].state10 = 4;
                        BadgeRows[1].state10 = 4;
                        ((s8 *)ImageRows)[1] = 3;
                    } else {
                        PlaySfx(4);
                    }
                } else if (MenuCursor == row) {
                    SetModelTexturePage(-1);
                    PlaySfx(1);
                    i = 0;
                    BadgeRows[0].state10 = 4;
                    BadgeRows[1].state10 = 4;
                    ((s8 *)ImageRows)[1] = phase;
                    do {
                        p = (UiRow24 *)(i * sizeof(UiRow24) + (unsigned long)UiRows);
                        if (p->byte14 == 2) {
                            p->byte14 = 3;
                        }
                        i++;
                    } while (i < 4);
                    goto close;
                }
            } else if (PlayerControllers[0].action2C != 0) {
                PlaySfx(2);
                SetModelTexturePage(-1);
                i = 0;
                BadgeRows[0].state10 = 4;
                BadgeRows[1].state10 = 4;
                ((s8 *)ImageRows)[1] = phase;
                do {
                    p = (UiRow24 *)(i * sizeof(UiRow24) + (unsigned long)UiRows);
                    if (p->byte14 == 2) {
                        p->byte14 = 3;
                    }
                    i++;
                } while (i < 4);
close:
                D_800AC5D0 = 0x40;
                D_800F4D8A = 8;
            }
            break;
        case 3:
            fade = D_800AC5D0 - 1;
            D_800AC5D0 = fade;
            AddScreenFadeTiles(CurrentDrawBuf->ordering.ordering70.words, (0x40 - fade) * 4, 0x45);
            if ((s16)D_800AC5D0 > 0) break;
            CourseChoiceTable[1][ovl662_D_801272E4].glyph = PackChoiceGlyph(&SelectionRows);
            CourseChoiceTable[1][ovl662_D_801272E4].first = PlayerCourseChoices[0].first;
            CourseChoiceTable[1][ovl662_D_801272E4].second = PlayerCourseChoices[0].second;
            ovl662_func_8011512C();
            ModeGroup = 1;
            D_800F3A26 = 0x40;
            D_800F4D8A = 0;
            return;
        case 4:
            if (UiRows[0].byte14 == 5 && ((s8 *)ImageRows)[1] == 5) {
                ovl662_D_801272E8 = 0;
                ModeGroup = 1;
                D_800F3A26 = 0x45;
                D_800F4D8A = 0;
            }
            break;
        case 8:
            fade = D_800AC5D0 - 1;
            D_800AC5D0 = fade;
            AddScreenFadeTiles(CurrentDrawBuf->ordering.ordering70.words, (0x40 - fade) * 4, 0x45);
            if ((s16)D_800AC5D0 > 0) break;
            ovl662_func_8011512C();
            ModeGroup = 1;
            D_800F3A26 = 0x40;
            D_800F4D8A = 0;
            return;
        }
    }
    if (ovl662_D_801272D4 == 0 && ((s8 *)ImageRows)[1] == 2) {
        ovl662_D_801272D4 = 1;
        BadgeRows[0].half02 = 1;
        BadgeRows[1].half02 = 1;
        i = 0;
    }
    i = 0;
    do {
        DrawUiRow(i);
        i++;
    } while (i < 3);
    i = active;
    if ((((u8)((s8 *)ImageRows)[1] - 1) < 2U) && (i == 1)) {
        DrawSelectionIconGrid(0);
        UpdateCarWorldTransform((char *)(void *)PlayerCars);
        tint = 0x100;
        if (((s8 *)ImageRows)[1] == i) {
            shade = ~((ImageRows[2] * 0xFF) / 30);
            color.channels.r = shade;
            color.channels.g = shade;
            color.channels.b = shade;
            mode = 0x102;
            tint = (ImageRows[2] << 8) / 30;
        } else {
            mode = 1;
            color.channels.r = 0x80;
            color.channels.g = 0x80;
            color.channels.b = 0x80;
        }
        DrawPreviewCar(PlayerCars, color.argument, tint, mode);
    }
    UpdateBadgeRowAnim(0);
    UpdateBadgeRowAnim(1);
    if (UpdateImageRow(0) > 0) {
        DrawMenuStepArrows(0xA0, 0x90, 8, 1, ovl662_D_801272BC, -1, 18);
    }
    UpdateScreenFade(0x10046);
}
