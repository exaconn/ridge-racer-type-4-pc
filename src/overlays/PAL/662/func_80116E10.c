#include "game/system/pad.h"
#include "rr/menu_overlay_runtime.h"
#include "rr/frame_storage_types.h"
#include "rr/load_cursor.h"

typedef struct {s16 value;u16 reserved;} GarageCarSlot;
extern s16 ovl662_saved_car_choice_idx;
extern s16 ovl662_menu_substate,ovl662_available_car_count,ovl662_selected_car_slot;
extern s16 ovl662_car_slot_statuses[20];
extern GarageCarSlot ovl662_available_car_slots[20];
void ovl662_init_team_car_layout(void);
void ovl662_draw_car_slot_bars(s32);

void ovl662_update_available_cars(void)
{
    PreviewColor color;
    s8 shown;
    s32 ready;
    s32 before;
    s32 status;
    s32 state;
    s32 cell;
    s32 i;
    s32 fadeMode;
    s16 steps;
    s32 fadeLevel;
    s8 shade;
    RaceVehicle *vehicle;

    shown = 0;
    ModeFrame++;
    vehicle = PlayerCars;
    ScreenFrame++;
    vehicle->rot54 = (vehicle->rot54 + 8) & 0xFFF;
    SetupMenuCarCamera(1);
    SetupViewport(6);
    ready = LoadCarRangeStep(0, 1);
    if (ready != 1) {
        if ((s8)ImageRows[1] == 1) {
            ImageRows[1] = 0;
        }
    } else {
        SetCarBounds(vehicle);
        if ((s8)ImageRows[1] == 7) {
            ImageRows[1] = ready;
        }
        shown = 1;
        switch (SubmodeState) {
        case 0:
            SetMenuFrameOptions(1, 1, 1);
            before = PackChoiceGlyph((s8 *)&SelectionRows);
            StepMenuValue(3, 2, (char *)&ovl662_selected_car_slot, ovl662_available_car_count, 0);
            UnpackChoiceGlyph((s8 *)&SelectionRows, ovl662_available_car_slots[ovl662_selected_car_slot].value);
            status = ovl662_car_slot_statuses[(SelectionRows.entry.parameter % 20) & 0xFF];
            if ((s16)before != (s16)PackChoiceGlyph((s8 *)&SelectionRows)) {
                ResetScreenCounters();
                LoadCursor = MenuArenas.first;
                UnpackChoiceGlyph((s8 *)&SelectionRows, ovl662_available_car_slots[ovl662_selected_car_slot].value);
                ImageRows[1] = 6;
                PlayerCourseChoices[0].first = 0;
                PlayerCourseChoices[0].second = 0;
            }
            UiRows[MenuCursor].cells[0] = 0;
            StepMenuValue(0, 2, (char *)&MenuCursor, 2, 0);
            cell = 2;
            if (status != 1) {
                UiRows[0].half20 = 1;
                if (MenuCursor == 0) {
                    cell = 4;
                }
            } else {
                UiRows[0].half20 = 0;
            }
            UiRows[MenuCursor].cells[0] = cell;
            if (UiRows[0].byte14 != 2) {
                break;
            }
            state = BadgeRows[0].state10;
            if (state != 3) {
                break;
            }
            if (PlayerControllers[0].action28) {
                SetMenuFrameOptions(0, 1, 1);
                UiRows[MenuCursor].cells[0] = 1;
                if (MenuCursor == 0) {
                    if (status == 1) {
                        PlaySfx(1);
                        SetModelTexturePage(-1);
                        SubmodeState = 3;
                        BadgeRows[0].state10 = 4;
                        BadgeRows[1].state10 = 4;
                        ImageRows[1] = 3;
                        for (i = 0; i < 4; i++) {
                            if (UiRows[i].byte14 == 2) {
                                UiRows[i].byte14 = 3;
                            }
                        }
                        FadeCountdown = 64;
                    } else {
                        PlaySfx(4);
                    }
                } else if (MenuCursor == 1) {
                    SetModelTexturePage(-1);
                    PlaySfx(1);
                    BadgeRows[0].state10 = 4;
                    BadgeRows[1].state10 = 4;
                    ImageRows[1] = state;
                    for (i = 0; i < UiRowCount; i++) {
                        if (UiRows[i].byte14 == 2) {
                            UiRows[i].byte14 = 3;
                        }
                    }
                    FadeCountdown = 64;
                    SubmodeState = 8;
                }
            } else if (PlayerControllers[0].action2C) {
                PlaySfx(2);
                SetModelTexturePage(-1);
                SetMenuFrameOptions(0, 1, 1);
                BadgeRows[0].state10 = 4;
                BadgeRows[1].state10 = 4;
                ImageRows[1] = state;
                for (i = 0; i < UiRowCount; i++) {
                    if (UiRows[i].byte14 == 2) {
                        UiRows[i].byte14 = 3;
                    }
                }
                FadeCountdown = 64;
                SubmodeState = 8;
            }
            break;
        case 3:
            FadeCountdown--;
            steps = 64;
            AddScreenFadeTiles(CurrentDrawBuf->ordering.ordering70.words, (steps - FadeCountdown) * 4, 69);
            if (FadeCountdown <= 0) {
                CourseChoiceTable[1][ovl662_saved_car_choice_idx].glyph = ovl662_available_car_slots[ovl662_selected_car_slot].value;
                CourseChoiceTable[1][ovl662_saved_car_choice_idx].first = 0;
                CourseChoiceTable[1][ovl662_saved_car_choice_idx].second = 0;
                ovl662_selected_car_slot = ovl662_saved_car_choice_idx;
                ovl662_init_team_car_layout();
                ModeGroup = ready;
                ModeIndex = steps;
                SubmodeState = 0;
                return;
            }
            break;
        case 8:
            FadeCountdown--;
            steps = 64;
            AddScreenFadeTiles(CurrentDrawBuf->ordering.ordering70.words, (steps - FadeCountdown) * 4, 69);
            if (FadeCountdown <= 0) {
                ovl662_selected_car_slot = ovl662_saved_car_choice_idx;
                ovl662_init_team_car_layout();
                ModeGroup = ready;
                ModeIndex = steps;
                SubmodeState = 0;
                return;
            }
            break;
        }
    }
    if (ovl662_menu_substate == 0 && (s8)ImageRows[1] == 2) {
        ovl662_menu_substate = 1;
        BadgeRows[0].half02 = 1;
        BadgeRows[1].half02 = 1;
    }
    for (i = 0; i < 2; i++) {
        DrawUiRow(i);
    }
    if (BadgeRows[1].state10 == 3) {
        ovl662_draw_car_slot_bars(SelectionRows.entry.parameter);
    }
    i = shown;
    if ((u8)(ImageRows[1] - 1) < 2 && i == 1) {
        DrawSelectionIconGrid(0);
        UpdateCarWorldTransform(PlayerCars);
        fadeLevel = 0x100;
        if ((s8)ImageRows[1] == i) {
            shade = ~((ImageRows[2] * 255) / 30);
            color.channels.r = shade;
            color.channels.g = shade;
            color.channels.b = shade;
            fadeMode = 0x102;
            fadeLevel = (ImageRows[2] << 8) / 30;
        } else {
            fadeMode = 1;
            color.channels.r = 0x80;
            color.channels.g = 0x80;
            color.channels.b = 0x80;
        }
        DrawPreviewCar(PlayerCars, color.argument, fadeLevel, fadeMode);
    }
    UpdateBadgeRowAnim(0);
    UpdateBadgeRowAnim(1);
    if (UpdateImageRow(0) > 0) {
        DrawMenuStepArrows(160, 144, 8, 1, ovl662_selected_car_slot, -1, ovl662_available_car_count + 1);
    }
    UpdateScreenFade(0x1008E);
}
