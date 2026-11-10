#include "rr/overlay662_tool_action.h"

void ovl662_tool_merge_layers(s32 mode) {
    if (mode < 0) {

    } else if (mode > 0) {
        switch (mode) {
        case 1:
        ovl662_editor_workspace.selectorsB8[0] = 0;
        ovl662_editor_workspace.selectorsB8[3] = 0;
        ovl662_select_option_target(0);
            break;
        case 2:
        ovl662_editor_workspace.selectorsB8[0] = 2;
        ovl662_editor_workspace.selectorsB8[3] = 2;
        ovl662_select_option_target(0);
            break;
        case 4:
        ovl662_editor_workspace.selectorsB8[1] = 3;
        ovl662_editor_workspace.selectorsB8[2] = 2;
        ovl662_select_option_target(1);
            break;
        case 8:
        ovl662_editor_workspace.selectorsB8[1] = 1;
        ovl662_editor_workspace.selectorsB8[2] = 0;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(0x10) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[1] = 1;
                    ovl662_editor_workspace.optionIndices98.flat[7] = 0;
                    ovl662_editor_workspace.saved64[0] = 0x10;
                    ovl662_editor_workspace.saved64[1] = 1;
                    ovl662_editor_workspace.saved64[2] = 4;
                    ovl662_editor_workspace.saved64[3] = 3;
                    ovl662_editor_workspace.saved64[4] = 1;
                }
                ovl662_backup_layer_and_palette();
                ((GarageButton *)ovl662_button_rect_lookup[16])->timer0E = 7;
                PlaySfx(1);
                ovl662_composite_emblem_layers(ovl662_editor_workspace.image38[ovl662_editor_workspace.active21], ovl662_editor_workspace.image40[ovl662_editor_workspace.active21]);
            }
        }
    }
}
