#include "rr/overlay662_palette_edit.h"

void ovl662_drag_palette_red_bar(void) {
    u16 val;
    u16 tmp;

    if (ovl662_editor_cursor_state.words[0] & 8) {
        if (ovl662_editor_cursor_state.words[1] & 4) {
            ovl662_editor_cursor_state.words[0] &= ~8;
            ovl662_set_clip_rect(13, 63, 294, 346);
            return;
        }
    } else {
        if (ovl662_point_outside_rect(149, 355, 65, 13) != 0) {
            return;
        }
        if (!(ovl662_editor_cursor_state.words[1] & 1)) {
            return;
        }
        ovl662_editor_cursor_state.words[4] = 48;
        ovl662_editor_cursor_state.words[0] |= 8;
        ovl662_set_clip_rect(149, 355, 64, 12);
        ovl662_red_bar_dragging = 0;
        ovl662_red_bar_saved_color = *(u16 *)((ovl662_editor_workspace.config22*2)+(PaletteAddress)(Palette16 *)ovl662_editor_workspace.palette48[ovl662_editor_workspace.active21]) & 0x1F;
    }
    val = (ovl662_editor_cursor_state.halves[10] - 149) / 2;
    if (ovl662_red_bar_saved_color != val) {
        if (ovl662_red_bar_dragging == 0) {
            ovl662_red_bar_dragging = 1;
            ovl662_backup_layer_and_palette();
        }
    }
    tmp = *(u16 *)((ovl662_editor_workspace.config22*2)+(PaletteAddress)(Palette16 *)ovl662_editor_workspace.palette48[ovl662_editor_workspace.active21]) & 0xFFE0;
    *(u16 *)((ovl662_editor_workspace.config22*2)+(PaletteAddress)(Palette16 *)ovl662_editor_workspace.palette48[ovl662_editor_workspace.active21]) = tmp | val;
}
