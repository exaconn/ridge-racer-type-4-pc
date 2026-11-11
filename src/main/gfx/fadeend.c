#include "rr/scene_camera_state.h"
#include "game/gfx/fadeset.h"
#include "rr/audio_state_types.h"
#include "psyq/libsnd/vmcfg.h"

void StartDemoFadeOutAndCloseVab(void) {
    RaceContext.fade3E0.f434 = 2;
    RaceContext.fade3E0.f43C = 0;
    SetParticleBrightFade(0x20, 0x100, 0);
    RaceContext.fade3E0.f41C = 3;
    RaceContext.fade3E0.f420 = 0;
    if (SoundState.banks24.ids[7] >= 0) {
        SsVabClose(SoundState.banks24.ids[7]);
        SoundState.banks24.ids[7] = -1;
    }
}
