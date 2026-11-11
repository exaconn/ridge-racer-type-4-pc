#include "types.h"
#include "game/gfx/fadesel.h"
#include "game/gfx/fadespr.h"

extern FadeRaceContext RaceContext;
extern s32 DemoFadeSkipEnabled;

void SelectDemoFadePreset(int a0)
{
    RaceContext.f404 = 0;
    RaceContext.f3F8 = 0;
    RaceContext.f434 = 1;
    RaceContext.f43C = 0;
    RaceContext.f440 = 0;
    switch (a0) {
    case 0:
        RaceContext.f438 = 0xB;
        break;
    case 1:
        RaceContext.f438 = 8;
        break;
    case 2:
        RaceContext.f438 = 0xC;
        break;
    }
    RaceContext.f3F4 = 1;
    RaceContext.f3E4 = 0xBB8;
    RaceContext.f41C = 2;
}

void EnableDemoFadeSkip(void) {
    DemoFadeSkipEnabled = 1;
}
