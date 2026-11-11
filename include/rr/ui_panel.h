#ifndef RR_UI_PANEL_H
#define RR_UI_PANEL_H

#include "rr/badge_rows.h"
#include "rr/sprite_emit.h"

/* Row decoration callback: ordering head, packet cursor, row index; returns
 * the advanced cursor. */
typedef void *(*PanelDraw)(GpuDmaTag *,void *,s32);
extern PanelDraw UiRowDrawHandlers[];

/* The selected row and signed high-byte callback index must name accessible
 * entries; index*row size must fit signed32. FFFF status emits nothing.
 * Captures layout and the ordering head before helpers, then reloads the row
 * table for its lower edge and final viewport. All helper returns are cursors. */
void DrawBadgeRow(s32 index);

#endif
