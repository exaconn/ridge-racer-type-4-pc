#include "rr/overlay662_scroll_actions.h"

void ovl662_scroll_canvas_left(void) {
    if (ovl662_editor_cursor_state.words[0] & 8) {
        if (ovl662_editor_cursor_state.words[1] & 4) {
            ovl662_editor_cursor_state.words[0] &= ~8;
            ovl662_set_clip_rect(13, 63, 294, 346);
            return;
        }
    } else {
        if (ovl662_point_outside_rect(81, 335, 9, 12) != 0) {
            return;
        }
        if (!(ovl662_editor_cursor_state.words[1] & 1)) {
            return;
        }
        ovl662_editor_cursor_state.words[4] = 38;
        ovl662_editor_cursor_state.words[0] |= 8;
        ovl662_set_button_clip_rect(38);
    }
    if (ovl662_editor_cursor_state.words[1] & 0x40) {
        ((GarageButton *)ovl662_button_rect_lookup[38])->timer0E = 7;
        if (--ovl662_editor_workspace.reset08[0] < 0) {
            ovl662_editor_workspace.reset08[0] = 0;
            PlaySfx(4);
        } else {
            PlaySfx(0);
        }
    }
}
