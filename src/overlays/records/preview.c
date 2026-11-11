#include "rr/overlay663.h"
#include "rr/frame_storage_types.h"

void ovl663_init_preview(void) {
    ProjectionScratchPrefix *scratch;
    RaceVehicle *state;

    ovl663_selected_record = 0;
    ClearScreenOverlayState();
    ScreenFadeState.phase = 2;
    ModeFrame = 0;
    SetupRenderer(3);
    scratch = RR_PROJECTION_SCRATCH;
    scratch->pose08.angles10.xyz.x = 0;
    scratch->pose08.angles10.xyz.y = 0;
    scratch->pose08.angles10.xyz.z = 0;
    scratch->pose08.position00.words[0] = 0;
    scratch->pose08.position00.words[1] = 0;
    scratch->pose08.position00.words[2] = 0;
    state = PlayerCars;
    state->position20.words[1] = -200;
    state->position20.words[2] = 2024;
    state->position20.words[0] = 0;
    state->rot50 = 40;
    state->rot54 = 0;
    state->rot58 = 0;
    InitUiRow(0, 12, 64, 68, 32, 0, 0, 1, 0, 83);
    InitBadgeRow(0, 12, 128, 76, 34, 0, 0xC01);
    BadgeRows[0].half02 = 1;
}

void ovl663_draw_preview(void) {
    PreviewMatrixWorkspace mat;
    Vec3sFields8 rot;
    GpuRectangleFields light;
    RaceVehicle *state;

    BuildCameraMatrices();
    SetupViewport(6);
    state = PlayerCars;
    state->rot54 = ((state->rot54 + 8U) & 0xFFFU);
    rot.x = state->rot50;
    rot.y = state->rot54;
    rot.z = state->rot58;
    RotMatrix(&rot, &mat.matrix);
    light.x = 100;
    light.y = 80;
    light.w = 120;
    light.h = 280;
    EmitTiledRect(&light, 128, CurrentDrawBuf->ordering.ordering70.tags);
    SetSineLightColors(4096);
    SetupSkyLightMatrices((char *)state->position20.words, mat.halves);
}

void ovl663_update_preview(void) {
    s32 old;
    if (LoadCoursePreviewStep(ovl663_available_courses[ovl663_selected_record])) {
        switch (SubmodeState) {
        case 0:
            if (UiRows->byte14 == 2) {
                SetMenuFrameOptions(1, 1, 0);
                SubmodeState = 1;
            }
            break;
        case 1:
            old = ovl663_selected_record;
            StepMenuValue(1, 4, (char *)&ovl663_selected_record, ovl663_available_course_count, 0);
            if (old != ovl663_selected_record)
                ResetScreenCounters();
            else
                ovl663_draw_preview();
            UiRows->cells[0] = 2;
            if (MenuReturnSubmodeIndex) {
                PlaySfx(1);
                SetMenuFrameOptions(0, 1, 0);
                SubmodeState = 8;
                UiRows->cells[0] = 1;
                FadeCountdown = 64;
                UiRows->byte14 = 3;
            }
            break;
        case 8:
            FadeCountdown--;
            AddScreenFadeTiles(CurrentDrawBuf->ordering.ordering70.tags, (64 - FadeCountdown) * 4, 69);
            if (FadeCountdown <= 0) {
                ovl663_init_menu();
                ModeGroup = 1;
                ModeIndex = 74;
                SubmodeState = 0;
                return;
            }
            break;
        }
    }
    if (ovl663_available_course_count >= 2)
        DrawMenuStepArrows(160, 384, 16, 1, ovl663_selected_record, 0, ovl663_available_course_count);
    DrawUiRow(0);
    UpdateScreenFade(0x10058);
}

void ovl663_init_result_preview(void) {
    RaceVehicle *state;
    ProjectionScratchPrefix *scratch;
    ModeFrame = 0;
    SetupRenderer(5);
    state = PlayerCars;
    scratch = RR_PROJECTION_SCRATCH;
    scratch->pose08.angles10.xyz.x = 120;
    scratch->pose08.position00.words[0] = -16;
    scratch->pose08.position00.words[1] = -248;
    scratch->pose08.angles10.xyz.y = 0;
    scratch->pose08.angles10.xyz.z = 0;
    scratch->pose08.position00.words[2] = -96;
    ResetCarState(state);
    state->position20.words[2] = 308;
    state->position20.words[1] = 10;
    state->rot54 = 2560;
    InitPreviewImageRows();
    BadgeRows->half02 = 1;
}

void ovl663_update_result_preview(void) {

    PreviewColor color;
    s32 scale;
    ModeFrame = (ModeFrame + 1U);
    switch ((u16)SubmodeState) {
    case 0:
        if (UiRows[3].byte14 == 2) {
            SetMenuFrameOptions(1, 1, 0);
            SubmodeState = 1;
        }
        break;
    case 1:
        UiRows[3].cells[0] = 2;
        if (MenuReturnSubmodeIndex) {
            PlaySfx(1);
            SetMenuFrameOptions(0, 1, 0);
            UiRows[3].cells[0] = 1;
            UiRows[3].byte14 = 3;
            SubmodeState = 2;
            BadgeRows->state10 = 4;
            ImageRows->f1 = 3;
        }
        break;
    case 2:
        if (BadgeRows->state10 == 11 && ImageRows->f1 == 5) {
            ovl663_init_selection(ovl663_record_kind);
            ModeGroup = 1;
            ModeIndex = 75;
            SubmodeState = 0;
        }
        break;
    }
    PlayerCars[0].rot54 = ((PlayerCars[0].rot54 + 8U) & 4095U);
    SetupMenuCarCamera(1);
    SetupViewport(6);
    if (ImageRows->f1 - 1U < 2U) {
        SelectionRows32 *selected = &SelectionRows.selection;
        selected->pad02[1] = 2;
        DrawSelectionIconGrid(0);
        selected->pad02[1] = 0;
        UpdateCarWorldTransform(PlayerCars);
        if (ImageRows->f1 == 1) {
            s32 level = ~(ImageRows->f2 * 255 / 30);
            color.channels.r = level;
            color.channels.g = level;
            color.channels.b = level;
            scale = ImageRows->f2 * 256 / 30;
        } else {
            color.channels.r = 128;
            color.channels.g = 128;
            color.channels.b = 128;
            scale = 256;
        }
        DrawPreviewCar(PlayerCars, color.argument, scale, 0);
    }
    DrawUiRow(3);
    UpdateBadgeRowAnim(0);
    UpdateImageRow(0);
    UpdateScreenFade(0x10022);
}
