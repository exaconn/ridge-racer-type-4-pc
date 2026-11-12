#include "rr/overlay_module.h"
#include "game/data/cdfile2.h"
#include "game/system/pad.h"
#include "rr/cd_directory_buffer.h"
#include "rr/frame_storage_types.h"
#include "rr/load_cursor.h"
#include "rr/menu_arena_base.h"
#include "rr/overlay662_course_controller.h"
#include "rr/renderer_setup.h"

extern s16 MenuItemCount;
extern u32 ScreenFrame;
extern s16 ovl662_selected_car_number, ovl662_selected_emblem_id;
extern s32 ovl662_emblem_edit_from_menu;
void InitMenuArenas(void);
void InitPreviewImageRows(void);
void InitBadgeRow(s32 index, s32 first, s16 x, s32 y, u16 width, u16 mode, u16 flags);
void InitUiRow(s32 index, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 count, s32 g, s32 h, ...);
void ovl662_init_team_car_layout(void);
void ovl662_init_garage_car_view(void);
void ovl662_select_garage_car(void);

void ovl662_select_garage_car(void) {
    s16 v0 = CourseChoiceTable[1][ovl662_selected_car_slot].first;
    s16 v1;
    ovl662_selected_car_number = v0;
    v1 = CourseChoiceTable[1][ovl662_selected_car_slot].second;
    PlayerCourseChoices[0].first = v0;
    ovl662_selected_emblem_id = v1;
    PlayerCourseChoices[0].second = v1;
    ovl662_init_garage_car_view();
}

void ovl662_init_garage_car_view(void) {
    ModeFrame = 0;
    ScreenFrame = 0;
    ovl662_menu_substate = 0;
    MenuCursor = 0;
    MenuItemCount = 3;
    ResetScreenCounters();
    MenuArenas.second = CdDirectoryBuffer + 0x2D000;
    InitMenuArenas();
    LoadCursor = MenuArenas.second;
    InitUiRow(0, 24, 88, 68, 32, 0, 0, 1, 0, 83);
    InitUiRow(1, 24, 128, 68, 32, 0, 0, 1, 0, 42);
    InitUiRow(2, 24, 168, 68, 32, 0, 0, 1, 0, 85);
    InitPreviewImageRows();
    InitBadgeRow(1, 24, 232, 84, 34, 0, 0x501);
    SetupRenderer(5);
    ModeGroup = 1;
    ModeIndex = 68;
    LoadCursor = MenuArenas.first = MenuArenaBase;
    SubmodeState = 0;
}

void ovl662_update_garage_car_view(void) {
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
        if (ImageRows->f1 == 1) {
            ImageRows->f1 = 0;
        }
    } else {
        SetCarBounds(vehicle);
        if (ImageRows->f1 == 7) {
            ImageRows->f1 = ready;
        }
        active = 1;
        switch ((u16)SubmodeState) {
        case 0:
            SetMenuFrameOptions(1, 1, 1);
            prev = ovl662_selected_car_number;
            StepMenuValue(3, 2, (char*)&ovl662_selected_car_number, 17, 0);
            if (prev != ovl662_selected_car_number) {
                SelectionRows.selection.pad02[0] %= 20;
                if (ovl662_selected_car_number > 0) {
                    SelectionRows.selection.pad02[0] += 20;
                }
                PlayerCourseChoices[0].first = ovl662_selected_car_number;
                ResetScreenCounters();
                LoadCursor = MenuArenas.first;
                ImageRows->f1 = 6;
            }
            UiRows[MenuCursor].cells[0] = 0;
            StepMenuValue(0, 2, (char*)&MenuCursor, 3, 0);
            cell = 2;
            if (ovl662_selected_car_number == 0) {
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
                    SubmodeState = phase;
                    i = 0;
                    do {
                        p = (UiRow24 *)(i * sizeof(UiRow24) + (uintptr_t)UiRows);
                        if (p->byte14 == 2) {
                            p->byte14 = 3;
                        }
                        i++;
                    } while (i < 4);
                    BadgeRows[0].state10 = 4;
                    BadgeRows[1].state10 = 4;
                    ImageRows->f1 = 3;
                    FadeCountdown = 0x40;
                } else if (MenuCursor == 1) {
                    if (ovl662_selected_car_number > 0) {
                        SetModelTexturePage(-1);
                        PlaySfx(1);
                        SubmodeState = 4;
                        i = 0;
                        do {
                            p = (UiRow24 *)(i * sizeof(UiRow24) + (uintptr_t)UiRows);
                            if (p->byte14 == 2) {
                                p->byte14 = 3;
                            }
                            i++;
                        } while (i < 4);
                        BadgeRows[0].state10 = 4;
                        BadgeRows[1].state10 = 4;
                        ImageRows->f1 = 3;
                    } else {
                        PlaySfx(4);
                    }
                } else if (MenuCursor == row) {
                    SetModelTexturePage(-1);
                    PlaySfx(1);
                    i = 0;
                    BadgeRows[0].state10 = 4;
                    BadgeRows[1].state10 = 4;
                    ImageRows->f1 = phase;
                    do {
                        p = (UiRow24 *)(i * sizeof(UiRow24) + (uintptr_t)UiRows);
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
                ImageRows->f1 = phase;
                do {
                    p = (UiRow24 *)(i * sizeof(UiRow24) + (uintptr_t)UiRows);
                    if (p->byte14 == 2) {
                        p->byte14 = 3;
                    }
                    i++;
                } while (i < 4);
close:
                FadeCountdown = 0x40;
                SubmodeState = 8;
            }
            break;
        case 3:
            fade = FadeCountdown - 1;
            FadeCountdown = fade;
            AddScreenFadeTiles(CurrentDrawBuf->ordering.ordering70.tags, (0x40 - fade) * 4, 0x45);
            if ((s16)FadeCountdown > 0) break;
            CourseChoiceTable[1][ovl662_selected_car_slot].glyph = PackChoiceGlyph(&SelectionRows.selection);
            CourseChoiceTable[1][ovl662_selected_car_slot].first = PlayerCourseChoices[0].first;
            CourseChoiceTable[1][ovl662_selected_car_slot].second = PlayerCourseChoices[0].second;
            ovl662_init_team_car_layout();
            ModeGroup = 1;
            ModeIndex = 0x40;
            SubmodeState = 0;
            return;
        case 4:
            if (UiRows[0].byte14 == 5 && ImageRows->f1 == 5) {
                ovl662_emblem_edit_from_menu = 0;
                ModeGroup = 1;
                ModeIndex = 0x45;
                SubmodeState = 0;
            }
            break;
        case 8:
            fade = FadeCountdown - 1;
            FadeCountdown = fade;
            AddScreenFadeTiles(CurrentDrawBuf->ordering.ordering70.tags, (0x40 - fade) * 4, 0x45);
            if ((s16)FadeCountdown > 0) break;
            ovl662_init_team_car_layout();
            ModeGroup = 1;
            ModeIndex = 0x40;
            SubmodeState = 0;
            return;
        }
    }
    if (ovl662_menu_substate == 0 && ImageRows->f1 == 2) {
        ovl662_menu_substate = 1;
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
    if ((((u8)ImageRows->f1 - 1) < 2U) && (i == 1)) {
        DrawSelectionIconGrid(0);
        UpdateCarWorldTransform((char *)(void *)PlayerCars);
        tint = 0x100;
        if (ImageRows->f1 == i) {
            shade = ~((ImageRows->f2 * 0xFF) / 30);
            color.channels.r = shade;
            color.channels.g = shade;
            color.channels.b = shade;
            mode = 0x102;
            tint = (ImageRows->f2 << 8) / 30;
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
        DrawMenuStepArrows(0xA0, 0x90, 8, 1, ovl662_selected_car_number, -1, 18);
    }
    UpdateScreenFade(0x10046);
}

/* Initial values of the overlay globals this module owns. */
s16 ovl662_selected_car_number;

/* Writable state of this unit; ovl662_reset_module restores it. */
static const OverlayStateRegion ovl662_car_view_regions[] = {
    RR_OVERLAY_STATE(ovl662_selected_car_number)};
const OverlayStateTable ovl662_car_view_state = {ovl662_car_view_regions, sizeof(ovl662_car_view_regions) / sizeof(ovl662_car_view_regions[0])};
