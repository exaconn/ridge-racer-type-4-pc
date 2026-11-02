#ifndef RR_OVERLAY666_PULSE_H
#define RR_OVERLAY666_PULSE_H
#include "rr/animated_sprite_grid.h"
#include "rr/frame_environment.h"
#include "rr/screen_tiles.h"
#include "rr/sprite_emit.h"
#include "rr/trig.h"
extern s32 D_800ABEB0;
extern RrUiUvRect ovl666_D_80116EE0;
/* The first emitter gets a captured cursor; the page emitter gets its return
 * and the freshly selected drawing context. Publish only the final cursor. */
void ovl666_func_801152B0(s32 x,s32 y);
#endif
