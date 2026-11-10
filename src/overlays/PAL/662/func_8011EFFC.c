#include "rr/overlay662_mirror.h"
void ovl662_tool_mirror_layer_h(s32 arg0) {
    s32 last;
    s32 temp_a1;
    s32 var_a2;
    u32 *var_t0;
    s32 var_t2;
    u32 *var_a3;
    u32 *var_t1;
    u32 temp_a0;

    if(arg0<0) {
        ovl662_editor_workspace.optionIndices98.flat[0] = 1;
        ovl662_editor_workspace.optionIndices98.flat[3] = 1;
        return;
    }
    if (arg0 > 0) {
        switch (arg0) {
        case 1:
            ovl662_editor_workspace.optionIndices98.flat[0] = 0;
            ovl662_editor_workspace.selectorsB8[3] = 0;
            ovl662_select_option_target(0);
            return;
        case 2:
            ovl662_editor_workspace.optionIndices98.flat[0] = 2;
            ovl662_editor_workspace.selectorsB8[3] = 2;
            ovl662_select_option_target(0);
            return;
        case 4:
            ovl662_editor_workspace.optionIndices98.flat[3] = 0;
            ovl662_editor_workspace.selectorsB8[2] = 0;
            ovl662_select_option_target(1);
            return;
        case 8:
            ovl662_editor_workspace.selectorsB8[1] = 2;
            ovl662_editor_workspace.selectorsB8[2] = 2;
            ovl662_select_option_target(1);
            return;
        default:
            return;
        }
    }
        if ((ovl662_point_outside_button(2) == 0) && (ovl662_editor_cursor_state.words[1] & 1)) {
            if (ovl662_editor_cursor_state.words[0] & 4) {
                ovl662_restore_saved_selection();
            } else {
                ovl662_editor_workspace.optionIndices98.flat[0] = 0;
                ovl662_editor_workspace.optionIndices98.flat[3] = 1;
                ovl662_editor_workspace.saved64[0] = 2;
                ovl662_editor_workspace.saved64[1] = 0;
                ovl662_editor_workspace.saved64[2] = 0;
                ovl662_editor_workspace.saved64[3] = 1;
                ovl662_editor_workspace.saved64[4] = 1;
            }
            ovl662_backup_layer_and_palette();
            ((GarageButton *)ovl662_button_rect_lookup[2])->timer0E = 7;
            PlaySfx(1);
            var_t2 = 0;
            if (ovl662_editor_workspace.layer23 == 0) {
                var_t1 = ovl662_editor_workspace.image38[ovl662_editor_workspace.active21 == 0];
                var_t0 = ovl662_editor_workspace.image38[ovl662_editor_workspace.active21];
            } else {
                var_t1 = ovl662_editor_workspace.image40[ovl662_editor_workspace.active21 == 0];
                var_t0 = ovl662_editor_workspace.image40[ovl662_editor_workspace.active21];
            }
            last=7;
            do {
                var_a2 = 0;
                var_a3 = var_t1;
loop_23:
                temp_a0 = *var_a3;
                var_a3 += 1;
                temp_a1 = last - var_a2;
                var_a2 += 1;
                var_t0[temp_a1] = (temp_a0 << 0x1C) | (((temp_a0 >> 4) & 0xF) << 0x18) | (((temp_a0 >> 8) & 0xF) << 0x14) | (((temp_a0 >> 0xC) & 0xF) << 0x10) | (((temp_a0 << 12) >> 16) & 0xF000) | (((temp_a0 << 8) >> 20) & 0xF00) | ((temp_a0 >> 0x14) & 0xF0) | (temp_a0 >> 0x1C);
                if (var_a2 < 8) {
                    goto loop_23;
                }
                var_t2 += 1;
                var_t1 += 8;
                var_t0 += 8;
            } while (var_t2 < 0x40);
        }
}
