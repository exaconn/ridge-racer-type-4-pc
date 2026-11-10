#ifndef RR_FADE_SPRITE_H
#define RR_FADE_SPRITE_H
#include "rr/frame_storage_types.h"
#include "rr/ui_bar.h"
#include "rr/animated_sprite_grid.h"

extern s32 FadeSpritePhaseAngle,FadeSpriteIntensityScale;
extern UiUvRect HudCourseMapScaleTable[];
extern u32 FadeSpriteDrawModes[];
/* Index names an accessible UV entry. Pointer slots and returned packet must
 * be accessible and aligned; helper return must provide eight writable bytes.
 * Products wrap to signed32 before each division by256 (truncation toward0).
 * Only positive scale emits a sprite and a linked draw-mode command. */
void EmitFadeSprite(s32 index,s32 x,s32 y,s32 intensity);
#endif
