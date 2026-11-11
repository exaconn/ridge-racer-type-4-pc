#include "game/gfx/fadectl.h"
#include "game/gfx/fadeset.h"
#include "game/gfx/fadein.h"
#include "game/gfx/fadespr.h"

extern FadeRaceContext RaceContext;

void StartDemoFadeIn(int a0) {
    if (RaceContext.f41C == 0) {
        ResetDemoFadeState();
    }
    RaceContext.f438 = a0 + 2;
    RaceContext.f434 = 1;
    RaceContext.f43C = 0;
    if (RaceContext.f408 == 1 || (RaceContext.f408 == 0 && RaceContext.f3EC == 0xFF)) {
        RaceContext.f408 = 2;
        RaceContext.f3E0 = 1;
        SetParticleBrightFade(0x20, 0, 0x100);
        RaceContext.f3F4 = 1;
    }
    RaceContext.f3F8 = 3;
    RaceContext.f41C = 2;
}
