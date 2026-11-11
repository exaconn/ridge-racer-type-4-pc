#ifndef RR_OVERLAY662_CHARACTER_GRID_H
#define RR_OVERLAY662_CHARACTER_GRID_H
#include "rr/overlay662_history_labels.h"
extern u8 NameCharset[];
extern s8 ovl662_char_grid_cmd_glyphs[3];
extern s32 ovl662_name_grid_index;
s32 rcos(s32 angle);
/* Row names a complete badge. Character bytes1..40 and the three nonnegative
 * UV keys must be readable. Drawing services return full-width arena pointers
 * and may change selection, tick, row root and glyph widths between calls. */
void *ovl662_draw_character_grid(GpuDmaTag *order,void *cursor,s32 row);
typedef unsigned long CharacterGridAddress;
typedef char CharacterGridAddressWidth[
    sizeof(CharacterGridAddress)==sizeof(void*) ? 1 : -1];
#endif
