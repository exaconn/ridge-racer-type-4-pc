#include "rr/overlay662_tool_action.h"

void ovl662_tool_select_color_11(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[2] = 1;
        ovl662_editor_workspace.optionIndices98.flat[4] = 2;
    } else if (mode > 0) {
        switch (mode) {
        case 1:
        ovl662_editor_workspace.optionIndices98.flat[2] = 0;
        ovl662_editor_workspace.selectorsB8[3] = 4;
        ovl662_select_option_target(0);
            break;
        case 2:
        ovl662_editor_workspace.optionIndices98.flat[0] = 0;
        ovl662_editor_workspace.selectorsB8[0] = 0;
        ovl662_editor_workspace.selectorsB8[1] = 0;
        ovl662_editor_workspace.selectorsB8[3] = 0;
        ovl662_select_option_target(0);
            break;
        case 4:

        ovl662_editor_workspace.optionIndices98.flat[4] = 1;
        ovl662_editor_workspace.selectorsB8[2] = 1;
        ovl662_select_option_target(1);
            break;
        case 8:
        ovl662_editor_workspace.selectorsB8[1] = 2;
        ovl662_editor_workspace.optionIndices98.flat[5] = 0;
        ovl662_editor_workspace.selectorsB8[2] = 3;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(0x1F) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[2] = 1;
                    ovl662_editor_workspace.optionIndices98.flat[4] = 2;
                    ovl662_editor_workspace.saved64[0] = 0x1F;
                    ovl662_editor_workspace.saved64[1] = 2;
                    ovl662_editor_workspace.saved64[2] = 1;
                    ovl662_editor_workspace.saved64[3] = 2;
                    ovl662_editor_workspace.saved64[4] = 5;
                }
                ovl662_editor_workspace.config22 = 0xB;
                PlaySfx(1);
            }
        }
    }
}
