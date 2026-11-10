#include "game/system/pad.h"
#include "rr/overlay662_confirm.h"

void ovl662_tool_load_card_dialog(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[0] = 2;
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
    } else if (ovl662_editor_cursor_state.words[0] & 8) {
        if (BadgeRows[0].state10 == 3) {
            if (ovl662_editor_workspace.fade00 < 0) {
                ovl662_editor_workspace.flagsD1 |= 4;
                if (CardDialogueStatePtr->st == 4 && PlayerControllers[0].input00.fields.type != 0x12 && PlayerControllers[0].action28 != 0) {
                    ovl662_editor_workspace.fade00 = 64;
                    PlaySfx(1);
                }
            } else if (ovl662_editor_workspace.fade00 == 0) {
                CardMenuOperation = 6;
                ModeGroup = 1;
                ModeIndex = 71;
                SubmodeState = 0;
            }
        }
    } else if (ovl662_point_outside_button(6) == 0 && (ovl662_editor_cursor_state.words[1] & 1)) {
        if (ovl662_editor_cursor_state.words[0] & 4) {
            ovl662_restore_saved_selection();
        } else {
            ovl662_editor_workspace.optionIndices98.flat[0] = 2;
            ovl662_editor_workspace.optionIndices98.flat[5] = 0;
            ovl662_editor_workspace.saved64[0] = 6;
            ovl662_editor_workspace.saved64[1] = 0;
            ovl662_editor_workspace.saved64[2] = 2;
            ovl662_editor_workspace.saved64[3] = 2;
            ovl662_editor_workspace.saved64[4] = 2;
        }
        ovl662_editor_cursor_state.words[4] = 6;
        ovl662_editor_cursor_state.words[0] |= 8;
        PlaySfx(1);
        ovl662_editor_cursor_state.words[0] |= 1;
        CardDialogueStatePtr->b4 = 4;
        CardDialogueStatePtr->b5 = -1;
        CardDialogueStatePtr->st = 0;
        ovl662_editor_workspace.flagsD1 = 1;
        BadgeRows[0].half02 = 1;
        BadgeRows[0].state10 = 0;
    }
}
