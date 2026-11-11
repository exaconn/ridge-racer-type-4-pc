#ifndef RR_OVERLAY662_SCROLL_ACTIONS_H
#define RR_OVERLAY662_SCROLL_ACTIONS_H
#include "rr/overlay662_tool_action.h"
#include "rr/overlay662_garage.h"
/* Slots36/38 decrement vertical/horizontal scroll. Input word1 is the
 * alias at8012766C. Table entries address genuine complete button owners.
 * PAL signed-halfword narrowing occurs before the negative clamp. */
void ovl662_scroll_canvas_up(void);
void ovl662_scroll_canvas_left(void);
/* Slots37/39 increment scroll and clamp after PAL signed-halfword
 * narrowing. Zoom is initialized/cycled through2,4,8,16; values2..255
 * also give a defined divisor. Limit is captured before input handling. */
/* The original symbol at 801276C4 is the workspace zoom byte (config1C). */
#define ovl662_editor_zoom_shift_alias (ovl662_editor_workspace.config1C)
void ovl662_scroll_canvas_down(void);
void ovl662_scroll_canvas_right(void);
#endif
