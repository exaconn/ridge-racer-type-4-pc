#include "rr/overlay662_pixel_dispatch.h"

void ovl662_dispatch_canvas_tool(s32 actionCode)
{
    s32 i;
    u32 *clear;

    ovl662_update_canvas_hover();
    clear = ovl662_editor_workspace.scratch34;
    for (i = 511; i >= 0; i--) {
        *clear++ = 0;
    }
    if (ovl662_editor_workspace.config20 != 2) {
        switch (ovl662_editor_workspace.config1E) {
        case 0: ovl662_apply_pen_stroke(); break;
        case 1: ovl662_apply_line_stroke(); break;
        case 2: ovl662_apply_flood_fill(); break;
        case 3: ovl662_apply_primary_pen_stroke(); break;
        case 4: ovl662_apply_move_drag(); break;
        }
    }
    if (ovl662_editor_workspace.config20 != 1) {
        switch (ovl662_editor_workspace.config1F) {
        case 0: ovl662_apply_color_picker(); break;
        case 1: ovl662_apply_eraser_stroke(); break;
        case 2: ovl662_toggle_preview_side(); break;
        }
    }
}
