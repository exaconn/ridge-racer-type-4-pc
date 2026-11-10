#include "rr/overlay662_image_stroke.h"

void ovl662_apply_pen_stroke(void) {
    u32 *img;

    if (ovl662_editor_cursor_state.words[0] & 8) {
        if (ovl662_editor_cursor_state.words[1] & 4) {
            ovl662_editor_workspace.config20 = 0;
            ovl662_editor_cursor_state.words[0] &= ~8;
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
        ovl662_editor_cursor_state.words[0] |= 8;
        ovl662_set_clip_rect(82, 71, 128, 256);
        ovl662_backup_layer_and_palette();
        ovl662_pen_stroke_prev_pt.x = ovl662_editor_workspace.reset08[0] + ovl662_editor_workspace.reset08[2];
        ovl662_pen_stroke_prev_pt.y = ovl662_editor_workspace.reset08[1] + ovl662_editor_workspace.reset08[3];
    }
    if (ovl662_editor_workspace.layer23 == 0) {
        img = ovl662_editor_workspace.image38[ovl662_editor_workspace.active21];
    } else {
        img = ovl662_editor_workspace.image40[ovl662_editor_workspace.active21];
    }
    ovl662_pen_stroke_curr_pt.x = ovl662_editor_workspace.reset08[0] + ovl662_editor_workspace.reset08[2];
    ovl662_pen_stroke_curr_pt.y = ovl662_editor_workspace.reset08[1] + ovl662_editor_workspace.reset08[3];
    ovl662_rasterize_brush_line(img, ovl662_pen_stroke_prev_pt.x, ovl662_pen_stroke_prev_pt.y, ovl662_pen_stroke_curr_pt.x, ovl662_pen_stroke_curr_pt.y, ovl662_editor_workspace.config22, ovl662_editor_workspace.config1D);
    ovl662_pen_stroke_prev_pt.x = ovl662_pen_stroke_curr_pt.x;
    ovl662_pen_stroke_prev_pt.y = ovl662_pen_stroke_curr_pt.y;
}
