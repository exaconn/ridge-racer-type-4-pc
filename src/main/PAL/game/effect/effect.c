#include "types.h"
#include "scratchpad.h"
#include "game/effect/ptinit.h"
#include "game/effect/effect.h"
#include "game/gfx/drawbuf.h"

typedef struct {
    s32 active00;
    s32 pad[7];
} RrParticleEntry;

typedef struct {
    s32 unk00;
    s32 f04;
    s32 f08;
    s32 f0C;
    s32 f10;
    s32 f14;
    s32 f18;
    s32 unk1C[4];
    RrParticleEntry e[24];
} RrParticleSys;

extern RrParticleSys ParticleSystem;

void SpawnParticlePair(void);
s32 func_80096464(void);
void SetDrawMode(u32 *, s32, s32, s32, s32);

void UpdateParticles(void) {
    u32 *p;
    u32 *ot;
    s32 i;

    if (ParticleSystem.f0C > 0) {
        ParticleSystem.f0C--;
        ParticleSystem.f18 = ParticleSystem.f14 - ((ParticleSystem.f14 - ParticleSystem.f10) * ParticleSystem.f0C) / ParticleSystem.f08;
    }
    if (ParticleSystem.f04 <= 0) {
        SpawnParticlePair();
        ParticleSystem.f04 = (func_80096464() % 16) * 2 + 8;
    }
    p = *(u32 **)SCRATCHPAD;
    ParticleSystem.f04--;
    SetDrawMode(p, 0, 1, 0xA0, 0);
    ot = (u32 *)(D_800ACC14 + 0x19C);
    *p = (*p & 0xFF000000) | (*ot & 0x00FFFFFF);
    *ot = (*ot & 0xFF000000) | ((u32)p & 0x00FFFFFF);
    p += 3;
    *(u32 **)SCRATCHPAD = p;
    for (i = 0; i < 0x18; i++) {
        if (ParticleSystem.e[i].active00 & 1) {
            UpdateParticle(i);
            DrawParticle(i);
        }
    }
}
