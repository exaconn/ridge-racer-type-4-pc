#include "rr/overlay668_camera.h"

void ovl668_init_epilogue_credits(void) {
    s32 i;
    TrackSample3C *p;

    SetDispMask(0);
    SetupDrawDispEnvs(0, 0, 0, 1);
    VSyncThreshold = 384;
    InitCourseViewState();
    SetupRenderer(5);
    BuildCameraMatrices();
    p = TrackNodes;
    RaceContext.counter444 = 0;
    RaceContext.state44C = 0;
    RaceContext.counter4A4 = 0;
    RaceContext.mode4A8 = 1;
    RaceContext.phase4AC = 0;
    RaceContext.position450[0] = (p[320].x08 * 4U);
    RaceContext.position450[1] = p[320].y10 - 120;
    RaceContext.position450[2] = (p[320].z0C * 4U);
    RaceContext.pitch460 = 0;
    RaceContext.yaw462 = (s16)(0U-(u16)p[320].heading12);
    RaceContext.roll464 = 0;
    RaceContext.offset488[0] = 0;
    RaceContext.offset488[1] = 0;
    RaceContext.offset488[2] = 0;
    RaceContext.angle490 = -96;
    ViewUsesCarTrackNode = 0;
    RaceContext.timer492 = 0;
    RaceContext.timer494 = 0;
    RaceContext.node498 = 320;
    ovl668_project_camera_to_track(RaceContext.position450);
    for (i = 0; i < 16; i++) {
        ovl668_step_epilogue_camera_yaw();
    }
    ovl668_step_epilogue_camera_pitch();
    ovl668_apply_epilogue_camera();
    InitSceneOverlayState();
    SetSceneOverlayColor(0, 144, 144, 144);
    SetSceneOverlayEnabled(1);
    MaskBitSetting = 1;
    D_80127E90 = 16000;
    D_80127E94 = ovl668_credit_scroll_speeds[(D_800AD564 * 4U + SelectedTeamIndex)];
}
