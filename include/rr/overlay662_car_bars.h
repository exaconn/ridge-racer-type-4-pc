#ifndef RR_OVERLAY662_CAR_BARS_H
#define RR_OVERLAY662_CAR_BARS_H
#include "rr/badge_rows.h"
#include "rr/sprite_emit.h"
#include "rr/menu_choice_types.h"
extern s16 ovl662_car_slot_statuses[20];
extern UiUvRect ovl662_car_slot_bar_uv;
#include "game/data/selection.h"
/* Twenty availability bars; selected may lie outside the displayed range.
 * Services may replace the context and update subsequent status entries. */
void ovl662_draw_car_slot_bars(s32 selected);
#endif
