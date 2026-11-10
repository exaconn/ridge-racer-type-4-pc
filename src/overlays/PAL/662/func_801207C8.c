#include "rr/overlay662_tool_action.h"

void ovl662_tool_swap_emblem_sides(s32 action)
{
    u32 *buffer;
    register u32 *image __asm__("$4");
    register u32 *palette __asm__("$3");
    register GarageTimBytes *tim __asm__("$5");
    u8 selected;
    volatile GarageSelectionPrefix *state;

    if (action < 0) {
        return;
    }
    if (action > 0) {
        switch (action) {
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
            ovl662_editor_workspace.selectorsB8[1] = 3;
            ovl662_editor_workspace.selectorsB8[2] = 3;
            ovl662_select_option_target(1);
            break;
        case 8:
            switch (ovl662_editor_workspace.optionIndices98.flat[0]) {
            case 1:
            case 2:
                ovl662_editor_workspace.optionIndices98.flat[3] = 0;
                /* Fall through: both paths select navigation row zero. */
            case 0:
            case 3:
                ovl662_editor_workspace.selectorsB8[1] = 0;
                break;
            case 4:
                ovl662_editor_workspace.selectorsB8[1] = 1;
                break;
            }
            ovl662_editor_workspace.selectorsB8[2] = 0;
            ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(15) == 0 &&
            (ovl662_editor_cursor_state.words[1] & 1)) {
            if (ovl662_editor_cursor_state.words[0] & 4) {
                ovl662_restore_saved_selection();
            } else {
                ovl662_editor_workspace.optionIndices98.flat[0] = 4;
                ovl662_editor_workspace.optionIndices98.flat[7] = 0;
                ovl662_editor_workspace.saved64[0] = 15;
                ovl662_editor_workspace.saved64[1] = 0;
                ovl662_editor_workspace.saved64[2] = 4;
                ovl662_editor_workspace.saved64[3] = 4;
                ovl662_editor_workspace.saved64[4] = 0;
            }
            ovl662_backup_layer_and_palette();
            ((GarageButton *)ovl662_button_rect_lookup[15])->timer0E = 7;
            PlaySfx(1);

            /* Preserve the observed pointer access order after sound.
             * Register declarations retain allocation, emitting no instructions. */
            state = &ovl662_editor_workspace;
            selected = state->active21;
            buffer = state->buffers24[selected];
            tim = state->tim2C[selected];
            image = state->image40[selected];
            palette = state->paletteBackup50[selected];
            state->tim2C[selected] = (GarageTimBytes *)buffer;
            state->image40[selected] = state->image38[selected];
            buffer = state->palette48[selected];
            state->buffers24[selected] = (u32 *)tim;
            state->image38[selected] = image;
            state->palette48[selected] = palette;
            state->paletteBackup50[selected] = buffer;
        }
    }
}
