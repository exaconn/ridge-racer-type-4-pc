#include "rr/scene_camera_state.h"
#include "game/gfx/fadectl.h"
#include "game/gfx/fadeset.h"
#include "game/gfx/fadein.h"
#include "game/gfx/fadespr.h"


void StartDemoFadeIn(int a0) {
    if (RaceContext.fade3E0.f41C == 0) {
        ResetDemoFadeState();
    }
    RaceContext.fade3E0.f438 = a0 + 2;
    RaceContext.fade3E0.f434 = 1;
    RaceContext.fade3E0.f43C = 0;
    if (RaceContext.fade3E0.f408 == 1 || (RaceContext.fade3E0.f408 == 0 && RaceContext.fade3E0.f3EC == 0xFF)) {
        RaceContext.fade3E0.f408 = 2;
        RaceContext.fade3E0.f3E0 = 1;
        SetParticleBrightFade(0x20, 0, 0x100);
        RaceContext.fade3E0.f3F4 = 1;
    }
    RaceContext.fade3E0.f3F8 = 3;
    RaceContext.fade3E0.f41C = 2;
}
