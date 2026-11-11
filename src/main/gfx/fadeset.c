#include "types.h"
#include "rr/ui.h"
#include "game/gfx/fadeset.h"

void SetParticleBrightFade(int a0, int a1, int a2) {
    ParticleSystem.fadeDuration08 = a0;
    ParticleSystem.fadeTimer0C = a0;
    ParticleSystem.fadeStart10 = a1;
    ParticleSystem.fadeTarget14 = a2;
    ParticleSystem.brightness18 = a1;
}
