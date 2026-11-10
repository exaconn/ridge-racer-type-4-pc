#include "rr/overlay662_tool_action.h"

void ovl662_tool_flip_layer_v(s32 mode) {
    s32 i, j;
    u32 *src, *dst;

    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[0] = 1;
    } else if (mode > 0) {
        switch (mode) {
        case 1:
            ovl662_editor_workspace.optionIndices98.flat[0] = 0;
            ovl662_editor_workspace.selectorsB8[3] = 0;
            ovl662_select_option_target(0);
            break;
        case 2:
            ovl662_editor_workspace.optionIndices98.flat[0] = 2;
            ovl662_editor_workspace.selectorsB8[3] = 2;
            ovl662_select_option_target(0);
            break;
        case 4:
            ovl662_editor_workspace.optionIndices98.flat[3] = 1;
            ovl662_editor_workspace.selectorsB8[1] = 0;
            ovl662_editor_workspace.selectorsB8[2] = 1;
            ovl662_select_option_target(1);
            break;
        case 8:
            ovl662_editor_workspace.selectorsB8[1] = 3;
            ovl662_editor_workspace.selectorsB8[2] = 3;
            ovl662_select_option_target(1);
            break;
        }
    } else if (ovl662_point_outside_button(3) == 0 && (ovl662_editor_cursor_state.words[1] & 1)) {
        if (ovl662_editor_cursor_state.words[0] & 4) {
            ovl662_restore_saved_selection();
        } else {
            ovl662_editor_workspace.optionIndices98.flat[0] = 1;
            ovl662_editor_workspace.optionIndices98.flat[5] = 0;
            ovl662_editor_workspace.saved64[0] = 3;
            ovl662_editor_workspace.saved64[1] = 0;
            ovl662_editor_workspace.saved64[2] = 2;
            ovl662_editor_workspace.saved64[3] = 2;
            ovl662_editor_workspace.saved64[4] = 1;
        }
        ovl662_backup_layer_and_palette();
        ((GarageButton *)ovl662_button_rect_lookup[3])->timer0E = 7;
        PlaySfx(1);
        if (ovl662_editor_workspace.layer23 == 0) {
            src = ovl662_editor_workspace.image38[ovl662_editor_workspace.active21 == 0];
            dst = ovl662_editor_workspace.image38[ovl662_editor_workspace.active21];
        } else {
            src = ovl662_editor_workspace.image40[ovl662_editor_workspace.active21 == 0];
            dst = ovl662_editor_workspace.image40[ovl662_editor_workspace.active21];
        }
        for (i = 0; i < 64; i++) {
            u32 *row;
            j = 0;
            row = (u32 *)(((63 - i) << 5) + (GarageImageAddress)src);
            do {
                *dst++ = *row++;
                j++;
            } while (j < 8);
        }
    }
}
