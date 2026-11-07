#include "types.h"
#include "game/gfx/fadeset.h"
extern struct { char pad[8]; s32 a, b, c, d, e; } ParticleSystem;
void SetParticleBrightFade(int a0, int a1, int a2) {
    ParticleSystem.a = a0;
    ParticleSystem.b = a0;
    ParticleSystem.c = a1;
    ParticleSystem.d = a2;
    ParticleSystem.e = a1;
}
