#ifndef RR_GAME_EFFECT_SPFADE_H
#define RR_GAME_EFFECT_SPFADE_H

#include "types.h"

void RampValueToLimit(unsigned short *a0, short *a1, int a2);
void RampValuePairTo4096(unsigned short *a0, unsigned short *a1, short *a2);
void RampValueTo4096(unsigned short *a0, short *a1);
void EmitTitleBeamFades(s32 intensity, s32 scale);
void EmitFadeTile(s32 intensity, s32 scale, u32 pos, s32 width);
enum {RR_TITLE_MASK_SPRITE_COUNT=11};
extern s16 SpinningWheelBaseVerts[RR_TITLE_MASK_SPRITE_COUNT];
/* Emits the selected low 11 mask bits; animation selects the four UV phases. */
void EmitTitleMaskSprites(s32 mask, s32 animation);
void EmitPresetSprite(s32 intensity,s32 index);


#endif /* RR_GAME_EFFECT_SPFADE_H */
