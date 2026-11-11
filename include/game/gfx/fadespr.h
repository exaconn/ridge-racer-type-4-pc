#ifndef RR_GAME_GFX_FADESPR_H
#define RR_GAME_GFX_FADESPR_H

#include "types.h"
#include "rr/layout.h"

void InitOptionsDefaults(void);
void InitDemoFadeContext(int a0);
void UpdateFadeTint(int a0);
void EmitFadeSprite(s32 index, s32 x, s32 y, s32 intensity);


#endif /* RR_GAME_GFX_FADESPR_H */
