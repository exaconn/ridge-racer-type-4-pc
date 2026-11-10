#ifndef RR_ICON_SPRITES_H
#define RR_ICON_SPRITES_H
#include "rr/frame_storage_types.h"
#include "rr/animated_sprite_grid.h"

typedef struct {
    FrameStorage *currentDrawBuf;
} FrameStorageSlot;

extern u16 D_8001121C[];

/* Requires a valid nonnegative CLUT-table index, a four-byte-aligned cursor
 * with 28 writable bytes, and accessible context heads on both captures.
 * Emits a 24x16 sprite at x148,y120/128, then draw-mode command E100023E.
 * Color uses its low byte. Each insertion preserves the head's high byte. */
void EmitIconSpriteRow120(s32 index,s32 color);
void EmitIconSpriteRow128(s32 index,s32 color);
#endif
