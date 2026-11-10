#include "rr/overlay662_image_drag.h"

void ovl662_apply_move_drag(void) {
    u32 *src;
    u32 *dst;

    if (ovl662_editor_cursor_state.words[0] & 8) {
        if (ovl662_editor_cursor_state.words[1] & 4) {
            ovl662_editor_workspace.config20 = 0;
            ovl662_editor_cursor_state.words[0] &= ~8;
            ovl662_editor_cursor_state.words[14] = 0;
            ovl662_set_clip_rect(13, 63, 294, 346);
            return;
        }
    } else {
        if (ovl662_point_outside_rect(82, 71, 128, 256) != 0) {
            return;
        }
        if (!(ovl662_editor_cursor_state.words[1] & 1)) {
            return;
        }
        ovl662_editor_workspace.config20 = 1;
        ovl662_editor_cursor_state.words[4] = 52;
        ovl662_editor_cursor_state.words[14] = 1;
        ovl662_editor_cursor_state.words[0] |= 8;
        ovl662_set_clip_rect(82, 71, 128, 256);
        ovl662_backup_layer_and_palette();
        ovl662_move_drag_start_pt.x = ovl662_editor_workspace.reset08[0] + ovl662_editor_workspace.reset08[6];
        ovl662_move_drag_start_pt.y = ovl662_editor_workspace.reset08[1] + ovl662_editor_workspace.reset08[7];
    }
    if (ovl662_editor_workspace.layer23 == 0) {
        src = ovl662_editor_workspace.image38[!ovl662_editor_workspace.active21];
        dst = ovl662_editor_workspace.image38[ovl662_editor_workspace.active21];
    } else {
        src = ovl662_editor_workspace.image40[!ovl662_editor_workspace.active21];
        dst = ovl662_editor_workspace.image40[ovl662_editor_workspace.active21];
    }
    ovl662_scroll_layer_image(
        src,
        dst,
        ovl662_editor_workspace.reset08[0] + ovl662_editor_workspace.reset08[6] - ovl662_move_drag_start_pt.x,
        ovl662_editor_workspace.reset08[1] + ovl662_editor_workspace.reset08[7] - ovl662_move_drag_start_pt.y);
}
