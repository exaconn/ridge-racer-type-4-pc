#include "types.h"
#include "rr/frame_storage_types.h"
#include "scratchpad.h"
#include "rr/ui.h"
#include "game/effect/ptinit.h"
#include "game/effect/effect.h"
#include "psyq/libc/rand.h"

#include "psyq/libgpu/gpudisp.h"

void UpdateParticles(void) {
    u32 *p;
    u32 *ot;
    s32 i;

    if (ParticleSystem.fadeTimer0C > 0) {
        ParticleSystem.fadeTimer0C--;
        ParticleSystem.brightness18 = ParticleSystem.fadeTarget14 - ((ParticleSystem.fadeTarget14 - ParticleSystem.fadeStart10) * ParticleSystem.fadeTimer0C) / ParticleSystem.fadeDuration08;
    }
    if (ParticleSystem.spawnTimer04 <= 0) {
        SpawnParticlePair();
        ParticleSystem.spawnTimer04 = (PsyqRand() % 16) * 2 + 8;
    }
    p = *(u32 **)SCRATCHPAD;
    ParticleSystem.spawnTimer04--;
    SetDrawMode(p, 0, 1, 0xA0, 0);
    ot = (u32 *)(&CurrentDrawBuf->buffer.ordering70.words[75]);
    *p = (*p & 0xFF000000) | (*ot & 0x00FFFFFF);
    *ot = (*ot & 0xFF000000) | ((u32)p & 0x00FFFFFF);
    p += 3;
    *(u32 **)SCRATCHPAD = p;
    for (i = 0; i < 0x18; i++) {
        if (ParticleSystem.entries2C[i].active00 & 1) {
            UpdateParticle(i);
            DrawParticle(i);
        }
    }
}
