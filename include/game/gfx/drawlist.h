#ifndef RR_GAME_GFX_DRAWLIST_H
#define RR_GAME_GFX_DRAWLIST_H

#include "types.h"
#include "rr/animated_sprite_grid.h"
#include "rr/geometry.h"

/* Intro camera positions (0x8009E540): entry 0 is the opening position,
 * StepIntroScene copies others into the projection pose and interpolates
 * between neighbouring entries. */
extern VectorWords16 IntroCameraPosKeyframes[10];

void StepIntroScene(void);


void EmitSpriteGrid(u32 intensity, SpriteGridClock *state);

#endif /* RR_GAME_GFX_DRAWLIST_H */
