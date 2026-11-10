#ifndef RR_GAME_EFFECT_PTINIT_H
#define RR_GAME_EFFECT_PTINIT_H

#include "types.h"

void InitParticles(void);
int FindFreeParticle(void);
void SpawnParticlePair(void);
void UpdateParticle(int a0);
void DrawParticle(int a0);

#endif /* RR_GAME_EFFECT_PTINIT_H */
