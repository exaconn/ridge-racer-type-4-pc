#include "rr/overlay668_camera.h"

void ovl668_func_80115DFC(void) {
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
    ovl668_func_80115460(RaceContext.position450);
    for (i = 0; i < 16; i++) {
        ovl668_func_80115554();
    }
    ovl668_func_801156FC();
    ovl668_func_80115850();
    InitSceneOverlayState();
    SetSceneOverlayColor(0, 144, 144, 144);
    SetSceneOverlayEnabled(1);
    MaskBitSetting = 1;
    D_80127E90 = 16000;
    D_80127E94 = ovl668_D_80116CC0[(D_800AD564 * 4U + SelectedTeamIndex)];
}
