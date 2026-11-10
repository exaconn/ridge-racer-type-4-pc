#include "rr/overlay662_scroll_actions.h"

void ovl662_scroll_canvas_up(void) {
    if (ovl662_editor_cursor_state.words[0] & 8) {
        if (ovl662_editor_cursor_state.words[1] & 4) {
            ovl662_editor_cursor_state.words[0] &= ~8;
            ovl662_set_clip_rect(13, 63, 294, 346);
            return;
        }
    } else {
        if (ovl662_point_outside_rect(215, 69, 6, 18) != 0) {
            return;
        }
        if (!(ovl662_editor_cursor_state.words[1] & 1)) {
            return;
        }
        ovl662_editor_cursor_state.words[4] = 36;
        ovl662_editor_cursor_state.words[0] |= 8;
        ovl662_set_button_clip_rect(36);
    }
    if (ovl662_editor_cursor_state.words[1] & 0x40) {
        ((GarageButton *)ovl662_button_rect_lookup[36])->timer0E = 7;
        if (--ovl662_editor_workspace.reset08[1] < 0) {
            ovl662_editor_workspace.reset08[1] = 0;
            PlaySfx(4);
        } else {
            PlaySfx(0);
        }
    }
}
