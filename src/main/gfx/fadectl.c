#include "game/gfx/fadeset.h"
#include "game/sound/sndvab.h"
#include "game/gfx/fadectl.h"
#include "game/gfx/fadespr.h"

extern FadeRaceContext RaceContext;

void ResetDemoFadeState(void) {
    int v, w, x;
    RaceContext.f434 = 0;
    RaceContext.f440 = 0;
    v = RaceContext.f408;
    if (v == 1 || (v == 0 && RaceContext.f3EC == 0)) {
        if (RaceContext.f3F4 == 0) {
            SetParticleBrightFade(1, 0, 0x100);
            RaceContext.f3F4 = 1;
        }
    }
    w = RaceContext.f408;
    if (w == 2 || (w == 0 && RaceContext.f3EC == 0xFF)) {
        if (RaceContext.f3F4) {
            SetParticleBrightFade(0, 0, 0);
            RaceContext.f3F4 = 0;
        }
    }
    RaceContext.f400 = 0x8C;
    RaceContext.f404 = 0x100;
    RaceContext.f3F8 = 0;
    RaceContext.f410 = 1;
    RaceContext.f412 = 0x10;
    RaceContext.f41A = 0x10;
    RaceContext.f416 = 0x10;
    RaceContext.f418 = 1;
    RaceContext.f414 = 1;
    RaceContext.f3E4 = 0xBB8;
    x = RaceContext.f430;
    RaceContext.f424 = 0;
    RaceContext.f41C = 1;
    if (x == 0) {
        PlaySfxBank2(RaceContext.f42C);
        RaceContext.f430 = 1;
    }
}
