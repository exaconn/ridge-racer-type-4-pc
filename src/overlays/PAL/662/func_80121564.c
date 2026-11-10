#include "rr/overlay662_tool_action.h"

void ovl662_tool_select_layer_0(s32 mode) {
    if (mode < 0) {

    } else if (mode > 0) {
        switch (mode) {
        case 1:
        ovl662_editor_workspace.selectorsB8[0] = 2;
        ovl662_editor_workspace.selectorsB8[3] = 2;
        ovl662_select_option_target(0);
            break;
        case 2:
        ovl662_editor_workspace.selectorsB8[0] = 1;
        ovl662_editor_workspace.selectorsB8[3] = 1;
        ovl662_select_option_target(0);
            break;
        case 4:
        ovl662_editor_workspace.selectorsB8[1] = 2;
        ovl662_editor_workspace.selectorsB8[2] = 2;
        ovl662_select_option_target(1);
            break;
        case 8:
        ovl662_editor_workspace.selectorsB8[1] = 4;
        ovl662_editor_workspace.selectorsB8[2] = 4;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(0x13) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[0] = 4;
                    ovl662_editor_workspace.optionIndices98.flat[6] = 0;
                    ovl662_editor_workspace.saved64[0] = 0x13;
                    ovl662_editor_workspace.saved64[1] = 0;
                    ovl662_editor_workspace.saved64[2] = 3;
                    ovl662_editor_workspace.saved64[3] = 3;
                    ovl662_editor_workspace.saved64[4] = 0;
                }
                ((GarageButton *)ovl662_button_rect_lookup[19])->timer0E = 7;
                ovl662_editor_workspace.layer23 = 0;
                PlaySfx(1);
            }
        }
    }
}
