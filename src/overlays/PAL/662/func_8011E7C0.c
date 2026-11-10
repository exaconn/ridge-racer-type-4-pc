#include "rr/overlay662_garage.h"
/* Overlay-local storage; the same RAM can have another meaning in another image. */
void ovl662_set_clip_rect(s32 a, s32 b, s32 c, s32 d)
{
    ovl662_editor_cursor_state.halves[24] = a;
    ovl662_editor_cursor_state.halves[25] = b;
    ovl662_editor_cursor_state.halves[26] = c;
    ovl662_editor_cursor_state.halves[27] = d;
}
