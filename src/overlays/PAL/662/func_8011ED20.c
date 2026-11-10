#include "rr/overlay662_rotate.h"
void ovl662_tool_rotate_layer(s32 arg0) {
    u32 *destination;
    s32 var_t2;
    s32 var_t4;
    s32 var_t6;
    u32 *var_t7;
    u32 *var_t8;
    s32 var_v0;
    s32 last;
    u32 temp_a0;
    u32 temp_a1;
    u32 temp_a2;
    u32 temp_a3;
    u32 temp_t0;
    u32 temp_t1;
    u32 temp_v0;
    u32 temp_v1;
    u32 *temp_t3;

    if (arg0 < 0) {
        ovl662_editor_workspace.optionIndices98.flat[0] = 1;
        ovl662_editor_workspace.optionIndices98.flat[3] = 0;
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
            ovl662_editor_workspace.selectorsB8[1] = 4;
            ovl662_editor_workspace.selectorsB8[2] = 4;
            ovl662_select_option_target(1);
            return;
        case 8:
            ovl662_editor_workspace.optionIndices98.flat[3] = 1;
            ovl662_editor_workspace.selectorsB8[2] = 1;
            ovl662_select_option_target(1);
            return;
        default:
            return;
        }
    }
        if ((ovl662_point_outside_button(1) == 0) && (ovl662_editor_cursor_state.words[1] & 1)) {
            if (ovl662_editor_cursor_state.words[0] & 4) {
                ovl662_restore_saved_selection();
            } else {
                ovl662_editor_workspace.optionIndices98.flat[0] = 1;
                ovl662_editor_workspace.optionIndices98.flat[3] = 0;
                ovl662_editor_workspace.saved64[0] = 1;
                ovl662_editor_workspace.saved64[1] = 0;
                ovl662_editor_workspace.saved64[2] = 0;
                ovl662_editor_workspace.saved64[3] = 0;
                ovl662_editor_workspace.saved64[4] = 1;
            }
            ovl662_backup_layer_and_palette();
            ((GarageButton *)ovl662_button_rect_lookup[1])->timer0E = 7;
            PlaySfx(1);
            var_t6 = 0;
            if (ovl662_editor_workspace.layer23 == 0) {
                var_t8 = ovl662_editor_workspace.image38[ovl662_editor_workspace.active21 == 0];
                var_t7 = ovl662_editor_workspace.image38[ovl662_editor_workspace.active21];
            } else {
                var_t8 = ovl662_editor_workspace.image40[ovl662_editor_workspace.active21 == 0];
                var_t7 = ovl662_editor_workspace.image40[ovl662_editor_workspace.active21];
            }
            last=7;
            do {
                var_t4 = 0;
loop_24:
                var_v0 = last - var_t4;
                temp_t3 = var_t8 + ((var_v0 << 6) + var_t6);
                destination = (var_t7 + ((var_t6 << 6) + var_t4));
                var_t2 = 0;
loop_25:
                temp_v0 = temp_t3[0] >> var_t2;
                temp_v1 = temp_t3[8] >> var_t2;
                temp_a0 = temp_t3[16] >> var_t2;
                temp_a1 = temp_t3[24] >> var_t2;
                temp_a2 = temp_t3[32] >> var_t2;
                temp_a3 = temp_t3[40] >> var_t2;
                temp_t0 = temp_t3[48] >> var_t2;
                temp_t1 = temp_t3[56] >> var_t2;
                var_t2 += 4;
                *destination = (temp_v0 << 0x1C) | ((temp_v1 & 0xF) << 0x18) | ((temp_a0 & 0xF) << 0x14) | ((temp_a1 & 0xF) << 0x10) | ((temp_a2 & 0xF) << 0xC) | ((temp_a3 & 0xF) << 8) | ((temp_t0 & 0xF) * 0x10) | (temp_t1 & 0xF);
                if (var_t2 < 0x20) {
                    /* One packed 64-pixel row is eight words. */
                    destination += 8;
                    goto loop_25;
                }
                var_t4 += 1;
                if (var_t4 < 8) {
                    goto loop_24;
                }
                var_t6 += 1;
            } while (var_t6 < 8);
        }
}
