#include "rr/overlay662_palette_edit.h"
#include "rr/overlay662_tool_action.h"

void ovl662_step_palette_blue_up(void) {
    u16 *pal = (u16 *)((PaletteAddress)(Palette16 *)ovl662_editor_workspace.palette48[ovl662_editor_workspace.active21]+(ovl662_editor_workspace.config22*2));
    u16 val = (*pal >> 10) & 0x1F;

    if (ovl662_editor_cursor_state.words[0] & 8) {
        if (ovl662_editor_cursor_state.words[1] & 4) {
            ovl662_editor_cursor_state.words[0] &= ~8;
            ovl662_set_clip_rect(13, 63, 294, 346);
            return;
        }
    } else {
        if (ovl662_point_outside_rect(213, 391, 8, 12) != 0) {
            return;
        }
        if (!(ovl662_editor_cursor_state.words[1] & 1)) {
            return;
        }
        ovl662_editor_cursor_state.words[4] = 45;
        ovl662_editor_cursor_state.words[0] |= 8;
        ovl662_set_button_clip_rect(45);
        ovl662_backup_layer_and_palette();
    }
    if (ovl662_editor_cursor_state.words[1] & 0x40) {
        val++;
        ((GarageButton *)ovl662_button_rect_lookup[45])->timer0E = 7;
        if (val >= 32) {
            val = 31;
            PlaySfx(4);
        } else {
            PlaySfx(0);
        }
        *pal = (*pal & 0x83FF) | (val << 10);
    }
}
