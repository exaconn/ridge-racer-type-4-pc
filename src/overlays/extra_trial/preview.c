#include "game/system/pad.h"
#include "rr/overlay666.h"
#include "rr/frame_storage_types.h"
#include "rr/load_cursor.h"

void ovl666_init_preview(void) {
    ProjectionScratchPrefix *sp;

    ModeFrame = 0;
    ScreenFrame = 0;
    ovl666_selected_course.word = 0;
    ovl666_preview_phase = 0;
    MenuCursor = 0;
    MenuItemCount = 2;
    InitUiRow(0, 24, 88, 68, 32, 0, 0, 1, 0, 84);
    InitUiRow(1, 24, 128, 68, 32, 0, 0, 1, 0, 85);
    SetupRenderer(5);
    ResetScreenCounters();
    InitPreviewImageRows();
    LoadCursor = MenuArenaBase;
    BadgeRows[0].half02 = 1;
    sp = RR_PROJECTION_SCRATCH;
    sp->pose08.angles10.xyz.x = 120;
    sp->pose08.position00.words[0] = -16;
    sp->pose08.position00.words[1] = -248;
    sp->pose08.angles10.xyz.y = 0;
    sp->pose08.angles10.xyz.z = 0;
    sp->pose08.position00.words[2] = -96;
    ResetCarState(PlayerCars);
    PlayerCars[0].position20.words[1] = 10;
    PlayerCars[0].position20.words[2] = 308;
    PlayerCars[0].rot54 = 2048;
    UnpackChoiceGlyph((s8 *)&SelectionRows,
                  ovl666_available_courses[SelectedMakerIndex][ovl666_selected_course.word].glyph);
    PlayerCourseChoices[0].first = ovl666_available_courses[SelectedMakerIndex][ovl666_selected_course.word].first;
    PlayerCourseChoices[0].second = ovl666_available_courses[SelectedMakerIndex][ovl666_selected_course.word].second;
    SetMenuFrameOptions(0, 1, 1);
}

void ovl666_update_preview(void) {
    PreviewColor color;
    s32 drawFlags;
    s32 scale;
    s8 modelReady;
    s32 rowIndex;
    s32 drawModel;
    s32 loadResult;
    u32 previousCar;
    u8 selectionState;
    u8 confirmationState;
    u8 closingState;
    u16 course;
    s8 brightness;

    ModeFrame = ModeFrame + 1U;
    modelReady = 0;
    ScreenFrame = ScreenFrame + 1U;
    PlayerCars[0].rot54 = (PlayerCars[0].rot54 + 8U) & 0xFFF;
    SetupMenuCarCamera(1);
    SetupViewport(6);
    if (SubmodeState != 3) {
        loadResult = LoadCarRangeStep(0, 1);
        if (loadResult != 1) {
            if (ImageRows->f1 == 1) {
                ImageRows->f1 = 0;
            }
            rowIndex = 0;
            goto draw_selection_rows;
        }
        SetCarBounds(PlayerCars);
        if (ImageRows->f1 == 7) {
            ImageRows->f1 = loadResult;
        }
    }
    modelReady = 1;
    switch (SubmodeState) {
    case 0:
        SetMenuFrameOptions(1, 1, 1);
        previousCar = PackChoiceGlyph((s8 *)&SelectionRows);
        StepMenuValue(1, 2, (char *)&ovl666_selected_course.halves[0],
                      ovl666_course_counts[SelectedMakerIndex], 0);
        UnpackChoiceGlyph((s8 *)&SelectionRows,
                      ovl666_available_courses[SelectedMakerIndex][ovl666_selected_course.word].glyph);
        if ((previousCar << 16) != ((u32)PackChoiceGlyph((s8 *)&SelectionRows) << 16)) {
            ResetScreenCounters();
            LoadCursor = MenuArenaBase;
            UnpackChoiceGlyph((s8 *)&SelectionRows,
                          ovl666_available_courses[SelectedMakerIndex][ovl666_selected_course.word].glyph);
            PlayerCourseChoices[0].first =
                ovl666_available_courses[SelectedMakerIndex][ovl666_selected_course.word].first;
            PlayerCourseChoices[0].second =
                ovl666_available_courses[SelectedMakerIndex][ovl666_selected_course.word].second;
            ImageRows->f1 = 6;
        }
        UiRows[MenuCursor + 3].cells[0] = 0;
        StepMenuValue(0, 2, (char *)&MenuCursor, 2, 0);
        UiRows[MenuCursor + 3].cells[0] = 2;
        selectionState = UiRows[MenuCursor + 3].byte14;
        if (selectionState == 2) {
            if (PlayerControllers[0].action28 != 0) {
                UiRows[MenuCursor + 3].cells[0] = 1;
                if (MenuCursor == 0) {
                    PlaySfx(1);
                    ovl666_preview_phase = selectionState;
                    SubmodeState = 4;
                } else if (MenuCursor == 1) {
                    PlaySfx(1);
                    rowIndex = 0;

                    do {
                        if (UiRows[rowIndex + 3].byte14 == 2) {
                            UiRows[rowIndex + 3].byte14 = 3;
                        }
                        rowIndex += 1;
                    } while (rowIndex < 2);
                    goto close_selection;
                }
            } else if (PlayerControllers[0].action2C != 0) {
                PlaySfx(2);
                rowIndex = 0;

                do {
                    if (UiRows[rowIndex + 3].byte14 == 2) {
                        UiRows[rowIndex + 3].byte14 = 3;
                    }
                    rowIndex += 1;
                } while (rowIndex < 2);
            close_selection:
                ImageRows->f1 = 3;
                SubmodeState = 5;
                BadgeRows->state10 = 4;
            }
        }
        break;
    case 3:
        SetMenuFrameOptions(0, 1, 1);
        if (FadeCountdown > 0) {
            FadeCountdown = (u16)FadeCountdown - 1;
        }
        SetModelTexturePage(-1);
        AddScreenFadeTiles(&CurrentDrawBuf->ordering.ordering70.tags[0], (0x40 - FadeCountdown) * 4, 0x45);
        loadResult = LoadCarRangeStep(1, 1);
        if ((loadResult == 1) && (FadeCountdown <= 0)) {
            GameModeType = 6;
            course =
                ovl666_course_dispatch[SelectionRows.selection.primary][SelectionRows.selection.variant];
            ovl666_result_kind = loadResult;
            SelectedCourseChoice = course;
            CourseIndex = DecodeCourseIndex((s16)course);
            SeqMuteVolume();
            MenuScriptId = 0;
            ModeGroup = loadResult;
            ModeIndex = 0x23;
            SubmodeState = 0;
            goto save_preview_arena;
        }
        break;
    case 4:
        confirmationState = UiRows[5].byte14;
        if (confirmationState == 2) {
            UiRows[SelectionRows.selection.pad02[1] + 5].cells[0] = 0;
            StepMenuValue(0, 1, (char *)&SelectionRows.selection.pad02[1], 2, 0);
            UiRows[SelectionRows.selection.pad02[1] + 5].cells[0] = confirmationState;
            if (PlayerControllers[0].action28 != 0) {
                PlaySfx(1);
                SetMenuFrameOptions(0, 1, 1);
                UiRows[SelectionRows.selection.pad02[1] + 5].cells[0] = 1;
                UiRows[5].byte14 = 7;
                UiRows[6].byte14 = 7;
                SelectionRows.selection.rows[0] = SelectionRows.entry;
                SelectionRows.selection.rows[0].parameter = 0x13;
                CourseIndex = 0;
                SubmodeState = 3;
                ResetScreenCounters();
                FadeCountdown = 0x40;
                SeqFadeVolume(0, 0x3C);
            save_preview_arena:
                MenuArenas.first = LoadCursor;
                break;
            }
            if (PlayerControllers[0].action2C != 0) {
                PlaySfx(2);
                UiRows[5].byte14 = 3;
                UiRows[6].byte14 = 3;
                SubmodeState = 6;
            }
        }
        break;
    case 6:
        if (UiRows[5].byte14 == 5) {
            SubmodeState = 0;
            ovl666_preview_phase = 1;
            UiRows[5].byte14 = 0;
            UiRows[6].byte14 = 0;
        }
        break;
    case 5:
        SetMenuFrameOptions(0, 1, 1);
        closingState = UiRows[3].byte14;
        if ((closingState == 5) && (ImageRows->f1 == closingState)) {
            ResetScreenCounters();
            ovl666_init_menu();
            ModeGroup = 1;
            ModeIndex = 0x4F;
            SubmodeState = 0;
            goto end;
        }
        break;
    }
    rowIndex = 0;
draw_selection_rows:
    do {
        DrawUiRow(rowIndex + 3);
        rowIndex += 1;
    } while (rowIndex < 2);
    if (ovl666_preview_phase == 2) {
        DrawUiRow(5);
        DrawUiRow(6);
    }
    drawModel = modelReady;
    if ((((u8)ImageRows->f1 - 1) < 2U) && (drawModel == 1)) {
        DrawSelectionIconGrid(0);
        UpdateCarWorldTransform(PlayerCars);
        scale = 0x100;
        if (ImageRows->f1 == drawModel) {
            brightness = ~((ImageRows->f2 * 0xFF) / 30);
            color.channels.r = brightness;
            color.channels.g = brightness;
            color.channels.b = brightness;
            drawFlags = 0x102;
            scale = (ImageRows->f2 << 8) / 30;
        } else {
            drawFlags = 1;
            color.channels.r = 0x80;
            color.channels.g = 0x80;
            color.channels.b = 0x80;
        }
        DrawPreviewCar(PlayerCars, color.argument, scale, drawFlags);
    }
    UpdateBadgeRowAnim(0);
    if (UpdateImageRow(0) > 0) {
        if (ovl666_course_counts[SelectedMakerIndex] >= 2) {
            DrawMenuStepArrows(0xA0, 0x90, 8, 1, ovl666_selected_course.word, 0,
                          ovl666_course_counts[SelectedMakerIndex]);
        }
    }
end:
    UpdateScreenFade(0x10036);
}

/* Initial values of the globals this module owns. */
CurrentSelection SelectionRows;
