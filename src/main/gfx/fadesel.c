#include "rr/scene_camera_state.h"
#include "types.h"
#include "game/gfx/fadesel.h"
#include "game/gfx/fadespr.h"


void SelectDemoFadePreset(int a0)
{
    RaceContext.fade3E0.f404 = 0;
    RaceContext.fade3E0.f3F8 = 0;
    RaceContext.fade3E0.f434 = 1;
    RaceContext.fade3E0.f43C = 0;
    RaceContext.fade3E0.f440 = 0;
    switch (a0) {
    case 0:
        RaceContext.fade3E0.f438 = 0xB;
        break;
    case 1:
        RaceContext.fade3E0.f438 = 8;
        break;
    case 2:
        RaceContext.fade3E0.f438 = 0xC;
        break;
    }
    RaceContext.fade3E0.f3F4 = 1;
    RaceContext.fade3E0.f3E4 = 0xBB8;
    RaceContext.fade3E0.f41C = 2;
}

void EnableDemoFadeSkip(void) {
    RaceContext.fade3E0.f428 = 1;
}
