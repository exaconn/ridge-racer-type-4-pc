#ifndef RR_UI_PANEL_H
#define RR_UI_PANEL_H

#include "rr/badge_rows.h"
#include "rr/sprite_emit.h"

typedef void *(*PanelDraw)(u32 *,void *,s32);
extern PanelDraw D_8009F984[];

/* The selected row and signed high-byte callback index must name accessible
 * entries; index*row size must fit signed32. FFFF status emits nothing.
 * Captures layout and the ordering head before helpers, then reloads the row
 * table for its lower edge and final viewport. All helper returns are cursors. */
void DrawBadgeRow(s32 index);

#endif
