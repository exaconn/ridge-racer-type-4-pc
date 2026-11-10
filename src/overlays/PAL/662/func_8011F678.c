#include "rr/overlay662_tool_action.h"

void ovl662_tool_clear_layer(s32 mode) {
    u32 *dst;
    s32 i;

    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[0] = 2;
        ovl662_editor_workspace.optionIndices98.flat[3] = 1;
    } else if (mode > 0) {
        switch (mode) {
        case 1:
            ovl662_editor_workspace.optionIndices98.flat[0] = 1;
            ovl662_editor_workspace.selectorsB8[3] = 1;
            ovl662_select_option_target(0);
            break;
        case 2:
            ovl662_editor_workspace.optionIndices98.flat[0] = 3;
            ovl662_editor_workspace.selectorsB8[3] = 3;
            ovl662_select_option_target(0);
            break;
        case 4:
            ovl662_editor_workspace.optionIndices98.flat[3] = 0;
            ovl662_editor_workspace.selectorsB8[2] = 0;
            ovl662_select_option_target(1);
            break;
        case 8:
            ovl662_editor_workspace.selectorsB8[1] = 2;
            ovl662_editor_workspace.selectorsB8[2] = 2;
            ovl662_select_option_target(1);
            break;
        }
    } else if (ovl662_point_outside_button(5) == 0 && (ovl662_editor_cursor_state.words[1] & 1)) {
        if (ovl662_is_active_layer_empty() != 0) {
            ((GarageButton *)ovl662_button_rect_lookup[5])->timer0E = 7;
            PlaySfx(4);
        } else {
            if (ovl662_editor_cursor_state.words[0] & 4) {
                ovl662_restore_saved_selection();
            } else {
                ovl662_editor_workspace.optionIndices98.flat[0] = 2;
                ovl662_editor_workspace.optionIndices98.flat[3] = 1;
                ovl662_editor_workspace.saved64[0] = 5;
                ovl662_editor_workspace.saved64[1] = 0;
                ovl662_editor_workspace.saved64[2] = 0;
                ovl662_editor_workspace.saved64[3] = 1;
                ovl662_editor_workspace.saved64[4] = 2;
            }
            ovl662_backup_layer_and_palette();
            ((GarageButton *)ovl662_button_rect_lookup[5])->timer0E = 7;
            PlaySfx(1);
            if (ovl662_editor_workspace.layer23 == 0) {
                dst = ovl662_editor_workspace.image38[ovl662_editor_workspace.active21];
            } else {
                dst = ovl662_editor_workspace.image40[ovl662_editor_workspace.active21];
            }
            for (i = 0; i < 512; i++) {
                *dst++ = 0;
            }
        }
    }
}
