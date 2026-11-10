#include "game/system/pad.h"
#include "rr/overlay662_save_dialog.h"

void ovl662_tool_save_emblem_dialog(s32 mode) {
    u8 phase;
    s32 selection;

    if (mode < 0) {
        return;
    }
    if (mode > 0) {
        switch (mode) {
    case 1:
        ovl662_editor_workspace.selectorsB8[0] = mode;
        ovl662_editor_workspace.selectorsB8[3] = mode;
        ovl662_select_option_target(0);
        return;
    case 2:
        ovl662_editor_workspace.selectorsB8[0] = 0;
        ovl662_editor_workspace.selectorsB8[3] = 0;
        ovl662_select_option_target(0);
        return;
    case 4:
        if (ovl662_editor_workspace.optionIndices98.flat[2] == 0) {
            ovl662_editor_workspace.optionIndices98.flat[5] = 4;
        } else {
            ovl662_editor_workspace.optionIndices98.flat[5] = 3;
        }
        ovl662_editor_workspace.selectorsB8[1] = 2;
        ovl662_editor_workspace.selectorsB8[2] = 7;
        ovl662_select_option_target(1);
        return;
    case 8:
        ovl662_editor_workspace.selectorsB8[1] = 4;
        ovl662_editor_workspace.selectorsB8[2] = 9;
        ovl662_select_option_target(1);return;
        default:
            return;
        }
    }
        if (ovl662_editor_cursor_state.words[0] & 8) {
            phase = UiRows[0].byte14;
            switch (phase) {
            case 2:
                if ((BadgeRows->state10 == 3) && (ovl662_editor_workspace.flagsD1 = (u8) (ovl662_editor_workspace.flagsD1 | 4), (CardDialogueStatePtr->st == 4))) {
                    if (PlayerControllers[0].input00.fields.type != 0x12) {
                        if ((u8) ovl662_editor_workspace.stateD2 < 2U) {
                            if (PlayerControllers[0].edges08.fields.repeat & 0x1000) {
                                if (ovl662_editor_workspace.stateD2 == 0) goto skip_sfx;
                                ovl662_editor_workspace.stateD2 = 0U;
                            } else if ((PlayerControllers[0].edges08.fields.repeat & 0x4000) && (ovl662_editor_workspace.stateD2 != 1)) {
                                ovl662_editor_workspace.stateD2 = 1U;
                            } else {
                                goto skip_sfx;
                            }
                            PlaySfx(0);
                        skip_sfx:;
                        }
                        if (D_800F3A60 != 0) {
                            ovl662_editor_workspace.stateD2 = (u8) (ovl662_editor_workspace.stateD2 + 2);
                            PlaySfx(1);
                        }
                    }
                    selection = ovl662_editor_workspace.stateD2;
                    switch (selection) {
                    case 0:
                        UiRows[0].cells[0] = 2;
                        UiRows[1].cells[0] = 0;
                        return;
                    case 1:
                        UiRows[0].cells[0] = 0;
                        UiRows[1].cells[0] = 2;
                        return;
                    case 2:
                        UiRows[0].cells[0] = 1;
                        UiRows[1].cells[0] = 0;
                        CardDialogueStatePtr->b4 = 6;
                        return;
                    case 3:
                        UiRows[0].cells[0] = 0;
                        UiRows[1].cells[0] = 1;
                        UiRows[0].byte14 = 3U;
                        UiRows[1].byte14 = 3;
                        BadgeRows->state10 = 4;
                        ovl662_editor_workspace.flagsD1 = (u8) (ovl662_editor_workspace.flagsD1 & 0xFB);
                        return;
                    case 4:
                        if (CardDialogueStatePtr->st == 4) {
                            ovl662_pack_emblem_tims();
                            UiRows[0].byte14 = 7U;
                            UiRows[1].byte14 = 7;
                            ovl662_editor_workspace.fade00 = 0x40;
                            return;
                        }
                        break;
                    }
                }
                break;
            case 5:
            case 6:
                if (BadgeRows->state10 == 0xB) {
                    ovl662_editor_cursor_state.words[0] = (ovl662_editor_cursor_state.words[0] & ~9);
                    ovl662_editor_workspace.flagsD1 = 0;
                    return;
                }
                break;
            case 9:
                if (ovl662_editor_workspace.fade00 == 0) {
                    CardMenuOperation = 7;
                    ModeGroup = 1;
                    ModeIndex = 0x47;
                    SubmodeState = 0;
                    return;
                }
                break;
            }
        } else {
            if ((ovl662_point_outside_button(0x11) == 0) && (ovl662_editor_cursor_state.words[1] & 1)) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.saved64[0] = 0x11;
                    ovl662_editor_workspace.optionIndices98.flat[2] = 0;
                    ovl662_editor_workspace.optionIndices98.flat[6] = 0;
                    ovl662_editor_workspace.saved64[1] = 2;
                    ovl662_editor_workspace.saved64[2] = 3;
                    ovl662_editor_workspace.saved64[3] = 8;
                    ovl662_editor_workspace.saved64[4] = 2;
                }
                ovl662_editor_cursor_state.words[4] = 0x11;
                ovl662_editor_cursor_state.words[0] = (ovl662_editor_cursor_state.words[0] | 8);
                PlaySfx(1);
                ovl662_editor_cursor_state.words[0] = (ovl662_editor_cursor_state.words[0] | 1);
                CardDialogueStatePtr->b4 = 5;
                CardDialogueStatePtr->b5 = -1;
                CardDialogueStatePtr->st = 0;
                ovl662_editor_workspace.flagsD1 = 3U;
                BadgeRows->half02 = 1;
                BadgeRows->state10 = 0;
                UiRows[0].byte14 = 0U;
                UiRows[1].byte14 = 0;
                ovl662_editor_workspace.stateD2 = 1U;
            }
            return;
        }
}
