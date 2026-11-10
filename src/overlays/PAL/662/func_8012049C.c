#include "rr/overlay662_tool_action.h"

void ovl662_tool_cycle_brush_size(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[1] = 1;
    } else if (mode > 0) {
        switch (mode) {
        case 1:
        ovl662_editor_workspace.optionIndices98.flat[1] = 0;
        ovl662_editor_workspace.selectorsB8[3] = 2;
        ovl662_select_option_target(0);
            break;
        case 2:
        ovl662_editor_workspace.optionIndices98.flat[2] = 0;
        ovl662_editor_workspace.selectorsB8[0] = 2;
        ovl662_editor_workspace.selectorsB8[3] = 4;
        ovl662_select_option_target(0);
            break;
        case 4:
        ovl662_editor_workspace.selectorsB8[1] = 4;
        ovl662_editor_workspace.selectorsB8[2] = 3;
        ovl662_select_option_target(1);
            break;
        case 8:

ovl662_editor_workspace.selectorsB8[1] = 2;
        ovl662_editor_workspace.selectorsB8[2] = 1;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(0xD) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[1] = 1;
                    ovl662_editor_workspace.optionIndices98.flat[4] = 1;
                    ovl662_editor_workspace.saved64[0] = 0xD;
                    ovl662_editor_workspace.saved64[1] = 1;
                    ovl662_editor_workspace.saved64[2] = 1;
                    ovl662_editor_workspace.saved64[3] = 0;
                    ovl662_editor_workspace.saved64[4] = 3;
                }
                ((GarageButton *)ovl662_button_rect_lookup[13])->timer0E = 7;
                ovl662_editor_workspace.config1D <<= 1;
                if (ovl662_editor_workspace.config1D >= 9) {
                    ovl662_editor_workspace.config1D = 1U;
                }
                PlaySfx(1);
            }
        }
    }
}
