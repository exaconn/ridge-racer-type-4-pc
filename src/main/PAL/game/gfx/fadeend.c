#include "game/gfx/fadeset.h"
extern int RaceContext[];
#include "rr/audio_state_types.h"
#include "psyq/libsnd/vmcfg.h"

void StartDemoFadeOutAndCloseVab(void) {
    RaceContext[0x434 / 4] = 2;
    RaceContext[0x43C / 4] = 0;
    SetParticleBrightFade(0x20, 0x100, 0);
    RaceContext[0x41C / 4] = 3;
    RaceContext[0x420 / 4] = 0;
    if (SoundState.banks24.ids[7] >= 0) {
        SsVabClose(SoundState.banks24.ids[7]);
        SoundState.banks24.ids[7] = -1;
    }
}
