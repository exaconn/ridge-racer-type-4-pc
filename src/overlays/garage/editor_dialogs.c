#include "rr/overlay662_tool_action.h"
#include "game/system/pad.h"
#include "rr/overlay662_save_dialog.h"
#include "rr/overlay662_exit_dialog.h"

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
                        if (MenuReturnSubmodeIndex != 0) {
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

void ovl662_tool_exit_editor_dialog(s32 mode) {
    u8 capturedFlags;
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
        ovl662_editor_workspace.selectorsB8[1] = 3;
        ovl662_editor_workspace.selectorsB8[2] = 8;
        ovl662_select_option_target(1);
        return;
    case 8:
        ovl662_editor_workspace.optionIndices98.flat[4] = 0;
        ovl662_editor_workspace.selectorsB8[1] = 1;
        ovl662_editor_workspace.selectorsB8[2] = 0;
        ovl662_select_option_target(1);
        return;
        default:
            return;
        }
    }
        if (ovl662_editor_cursor_state.words[0] & 8) {
            capturedFlags = ovl662_editor_workspace.flagsD1;
            if (capturedFlags == 0) {
                if (ovl662_editor_workspace.fade00 == 0) {
                    ovl662_editor_cursor_state.words[0] = (ovl662_editor_cursor_state.words[0] & ~9);
                    ovl662_editor_workspace.fade00 = -1;
                    ModeGroup=1;
                    ModeIndex=0x27;
                    SubmodeState=0;
                    return;
                }
            } else {
                phase = UiRows[0].byte14;
                switch (phase) {
                case 2:
                    if ((BadgeRows->state10 == 3) && (ovl662_editor_workspace.flagsD1 = (u8) (capturedFlags | 4), (CardDialogueStatePtr->st == 4))) {
                        if (PlayerControllers[0].input00.fields.type != 0x12) {
                            if (PlayerControllers[0].edges08.fields.repeat & 0x1000) {
                                if (ovl662_editor_workspace.stateD2 != 0) {
                                    ovl662_editor_workspace.stateD2 = 0U;
                                    goto block_32;
                                }
                            } else if (PlayerControllers[0].edges08.fields.repeat & 0x4000) {
                                if (ovl662_editor_workspace.stateD2 != 1) {
                                    ovl662_editor_workspace.stateD2 = 1U;
block_32:
                                    PlaySfx(0);
                                }
                            }
                            if (MenuReturnSubmodeIndex != 0) {
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
                            UiRows[0].byte14 = 7U;
                            UiRows[1].byte14 = 7U;
                            ovl662_editor_workspace.fade00 = 0x40;
                            return;
                        case 3:
                            UiRows[0].cells[0] = 0;
                            UiRows[1].cells[0] = 1;
                            UiRows[0].byte14 = selection;
                            UiRows[1].byte14 = selection;
                            BadgeRows->state10 = 4;
                            ovl662_editor_workspace.flagsD1 = (u8) (ovl662_editor_workspace.flagsD1 & 0xFB);
                            return;
                        }
                    }
                    break;
                case 5:
                case 6:
                    if (BadgeRows->state10 == 0xB) {
                        ovl662_editor_cursor_state.words[0] = (ovl662_editor_cursor_state.words[0] & ~9);
                        ovl662_editor_workspace.flagsD1 = 0U;
                        return;
                    }
                    break;
                case 9:
                    if (ovl662_editor_workspace.fade00 == 0) {
                        ModeGroup = 1;
                        ModeIndex = 0x27;
                        SubmodeState = 0;
                        return;
                    }
                    break;
                }
            }
        } else if ((ovl662_point_outside_button(0x12) == 0) && (ovl662_editor_cursor_state.words[1] & 1)) {
            if (ovl662_editor_cursor_state.words[0] & 4) {
                ovl662_restore_saved_selection();
            } else {
                ovl662_editor_workspace.saved64[0] = 0x12;
                ovl662_editor_workspace.optionIndices98.flat[2] = 0;
                ovl662_editor_workspace.optionIndices98.flat[7] = 0;
                ovl662_editor_workspace.saved64[1] = 2;
                ovl662_editor_workspace.saved64[2] = 4;
                ovl662_editor_workspace.saved64[3] = 9;
                ovl662_editor_workspace.saved64[4] = 2;
            }
            ovl662_editor_cursor_state.words[4] = 0x12;
            ovl662_editor_cursor_state.words[0] = (ovl662_editor_cursor_state.words[0] | 8);
            PlaySfx(1);
            ovl662_editor_cursor_state.words[0] = (ovl662_editor_cursor_state.words[0] | 1);
            if (ovl662_editor_workspace.dirtyD3 == 0) {
                ovl662_editor_workspace.fade00 = 0x40;
                return;
            }
            CardDialogueStatePtr->b4 = 7;
            CardDialogueStatePtr->b5 = -1;
            CardDialogueStatePtr->st = 0;
            ovl662_editor_workspace.flagsD1 = 3U;
            BadgeRows->half02 = 1;
            BadgeRows->state10 = 0;
            UiRows[0].byte14 = 0U;
            UiRows[1].byte14 = 0U;
            ovl662_editor_workspace.stateD2 = 1U;
        } else {
            return;
        }
}

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

void ovl662_tool_select_layer_1(s32 mode) {
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
        ovl662_editor_workspace.selectorsB8[1] = 2;
        ovl662_editor_workspace.selectorsB8[2] = 1;
        ovl662_select_option_target(1);
            break;
        case 8:

ovl662_editor_workspace.selectorsB8[1] = 4;
        ovl662_editor_workspace.selectorsB8[2] = 3;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(0x14) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[1] = 1;
                    ovl662_editor_workspace.optionIndices98.flat[6] = 0;
                    ovl662_editor_workspace.saved64[0] = 0x14;
                    ovl662_editor_workspace.saved64[1] = 1;
                    ovl662_editor_workspace.saved64[2] = 3;
                    ovl662_editor_workspace.saved64[3] = 2;
                    ovl662_editor_workspace.saved64[4] = 1;
                }
                ((GarageButton *)ovl662_button_rect_lookup[20])->timer0E = 7;
                ovl662_editor_workspace.layer23 = 1;
                PlaySfx(1);
            }
        }
    }
}

void ovl662_tool_select_color_01(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[2] = 0;
        ovl662_editor_workspace.optionIndices98.flat[4] = 0;
    } else if (mode > 0) {
        switch (mode) {
        case 1:
        ovl662_editor_workspace.optionIndices98.flat[1] = 1;
        ovl662_editor_workspace.selectorsB8[0] = 1;
        ovl662_editor_workspace.selectorsB8[3] = 3;
        ovl662_select_option_target(0);
            break;
        case 2:
        ovl662_editor_workspace.optionIndices98.flat[2] = 1;
        ovl662_editor_workspace.selectorsB8[3] = 5;
        ovl662_select_option_target(0);
            break;
        case 4:
        ovl662_editor_workspace.selectorsB8[1] = 4;
        ovl662_editor_workspace.selectorsB8[2] = 9;
        ovl662_select_option_target(1);
            break;
        case 8:

ovl662_editor_workspace.optionIndices98.flat[4] = 1;
        ovl662_editor_workspace.selectorsB8[2] = 1;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(0x15) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[2] = 0;
                    ovl662_editor_workspace.optionIndices98.flat[4] = 0;
                    ovl662_editor_workspace.saved64[0] = 0x15;
                    ovl662_editor_workspace.saved64[1] = 2;
                    ovl662_editor_workspace.saved64[2] = 1;
                    ovl662_editor_workspace.saved64[3] = 0;
                    ovl662_editor_workspace.saved64[4] = 4;
                }
                ovl662_editor_workspace.config22 = 1;
                PlaySfx(1);
            }
        }
    }
}

void ovl662_tool_select_color_02(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[2] = 0;
        ovl662_editor_workspace.optionIndices98.flat[4] = 1;
    } else if (mode > 0) {
        switch (mode) {
        case 1:
        ovl662_editor_workspace.optionIndices98.flat[1] = 1;
        ovl662_editor_workspace.selectorsB8[0] = 1;
        ovl662_editor_workspace.selectorsB8[3] = 3;
        ovl662_select_option_target(0);
            break;
        case 2:
        ovl662_editor_workspace.optionIndices98.flat[2] = 1;
        ovl662_editor_workspace.selectorsB8[3] = 5;
        ovl662_select_option_target(0);
            break;
        case 4:
        ovl662_editor_workspace.optionIndices98.flat[4] = 0;
        ovl662_editor_workspace.selectorsB8[2] = 0;
        ovl662_select_option_target(1);
            break;
        case 8:
        ovl662_editor_workspace.optionIndices98.flat[4] = 2;
        ovl662_editor_workspace.selectorsB8[2] = 2;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(0x16) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[2] = 0;
                    ovl662_editor_workspace.optionIndices98.flat[4] = 1;
                    ovl662_editor_workspace.saved64[0] = 0x16;
                    ovl662_editor_workspace.saved64[1] = 2;
                    ovl662_editor_workspace.saved64[2] = 1;
                    ovl662_editor_workspace.saved64[3] = 1;
                    ovl662_editor_workspace.saved64[4] = 4;
                }
                ovl662_editor_workspace.config22 = 2;
                PlaySfx(1);
            }
        }
    }
}

void ovl662_tool_select_color_03(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[2] = 0;
        ovl662_editor_workspace.optionIndices98.flat[4] = 2;
    } else if (mode > 0) {
        switch (mode) {
        case 1:
        ovl662_editor_workspace.optionIndices98.flat[1] = 1;
        ovl662_editor_workspace.selectorsB8[0] = 1;
        ovl662_editor_workspace.selectorsB8[3] = 3;
        ovl662_select_option_target(0);
            break;
        case 2:
        ovl662_editor_workspace.optionIndices98.flat[2] = 1;
        ovl662_editor_workspace.selectorsB8[3] = 5;
        ovl662_select_option_target(0);
            break;
        case 4:

ovl662_editor_workspace.optionIndices98.flat[4] = 1;
        ovl662_editor_workspace.selectorsB8[2] = 1;
        ovl662_select_option_target(1);
            break;
        case 8:

ovl662_editor_workspace.optionIndices98.flat[5] = 0;
        ovl662_editor_workspace.selectorsB8[1] = 2;
        ovl662_editor_workspace.selectorsB8[2] = 3;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(0x17) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[2] = 0;
                    ovl662_editor_workspace.optionIndices98.flat[4] = 2;
                    ovl662_editor_workspace.saved64[0] = 0x17;
                    ovl662_editor_workspace.saved64[1] = 2;
                    ovl662_editor_workspace.saved64[2] = 1;
                    ovl662_editor_workspace.saved64[3] = 2;
                    ovl662_editor_workspace.saved64[4] = 4;
                }
                ovl662_editor_workspace.config22 = 3;
                PlaySfx(1);
            }
        }
    }
}

void ovl662_tool_select_color_04(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[2] = 0;
        ovl662_editor_workspace.optionIndices98.flat[5] = 0;
    } else if (mode > 0) {
        switch (mode) {
        case 1:
        ovl662_editor_workspace.optionIndices98.flat[1] = 1;
        ovl662_editor_workspace.selectorsB8[0] = 1;
        ovl662_editor_workspace.selectorsB8[3] = 6;
        ovl662_select_option_target(0);
            break;
        case 2:
        ovl662_editor_workspace.optionIndices98.flat[2] = 1;
        ovl662_editor_workspace.selectorsB8[3] = 8;
        ovl662_select_option_target(0);
            break;
        case 4:
        ovl662_editor_workspace.optionIndices98.flat[4] = 2;
        ovl662_editor_workspace.selectorsB8[1] = 1;
        ovl662_editor_workspace.selectorsB8[2] = 2;
        ovl662_select_option_target(1);
            break;
        case 8:
        ovl662_editor_workspace.optionIndices98.flat[5] = 1;
        ovl662_editor_workspace.selectorsB8[2] = 4;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(0x18) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[2] = 0;
                    ovl662_editor_workspace.optionIndices98.flat[5] = 0;
                    ovl662_editor_workspace.saved64[0] = 0x18;
                    ovl662_editor_workspace.saved64[1] = 2;
                    ovl662_editor_workspace.saved64[2] = 2;
                    ovl662_editor_workspace.saved64[3] = 3;
                    ovl662_editor_workspace.saved64[4] = 7;
                }
                ovl662_editor_workspace.config22 = 4;
                PlaySfx(1);
            }
        }
    }
}

void ovl662_tool_select_color_05(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[2] = 0;
        ovl662_editor_workspace.optionIndices98.flat[5] = 1;
    } else if (mode > 0) {
        switch (mode) {
        case 1:
        ovl662_editor_workspace.optionIndices98.flat[1] = 1;
        ovl662_editor_workspace.selectorsB8[0] = 1;
        ovl662_editor_workspace.selectorsB8[3] = 6;
        ovl662_select_option_target(0);
            break;
        case 2:
        ovl662_editor_workspace.optionIndices98.flat[2] = 1;
        ovl662_editor_workspace.selectorsB8[3] = 8;
        ovl662_select_option_target(0);
            break;
        case 4:

ovl662_editor_workspace.optionIndices98.flat[5] = 0;
        ovl662_editor_workspace.selectorsB8[2] = 3;
        ovl662_select_option_target(1);
            break;
        case 8:

ovl662_editor_workspace.optionIndices98.flat[5] = 2;
        ovl662_editor_workspace.selectorsB8[2] = 5;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(0x19) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[2] = 0;
                    ovl662_editor_workspace.optionIndices98.flat[5] = 1;
                    ovl662_editor_workspace.saved64[0] = 0x19;
                    ovl662_editor_workspace.saved64[1] = 2;
                    ovl662_editor_workspace.saved64[2] = 2;
                    ovl662_editor_workspace.saved64[3] = 4;
                    ovl662_editor_workspace.saved64[4] = 7;
                }
                ovl662_editor_workspace.config22 = 5;
                PlaySfx(1);
            }
        }
    }
}

void ovl662_tool_select_color_06(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[2] = 0;
        ovl662_editor_workspace.optionIndices98.flat[5] = 2;
    } else if (mode > 0) {
        switch (mode) {
        case 1:

ovl662_editor_workspace.optionIndices98.flat[1] = 1;
        ovl662_editor_workspace.selectorsB8[0] = 1;
        ovl662_editor_workspace.selectorsB8[3] = 6;
        ovl662_select_option_target(0);
            break;
        case 2:
        ovl662_editor_workspace.optionIndices98.flat[2] = 1;
        ovl662_editor_workspace.selectorsB8[3] = 8;
        ovl662_select_option_target(0);
            break;
        case 4:
        ovl662_editor_workspace.optionIndices98.flat[5] = 1;
        ovl662_editor_workspace.selectorsB8[2] = 4;
        ovl662_select_option_target(1);
            break;
        case 8:
        ovl662_editor_workspace.optionIndices98.flat[5] = 3;
        ovl662_editor_workspace.selectorsB8[2] = 6;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(0x1A) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[2] = 0;
                    ovl662_editor_workspace.optionIndices98.flat[5] = 2;
                    ovl662_editor_workspace.saved64[0] = 0x1A;
                    ovl662_editor_workspace.saved64[1] = 2;
                    ovl662_editor_workspace.saved64[2] = 2;
                    ovl662_editor_workspace.saved64[3] = 5;
                    ovl662_editor_workspace.saved64[4] = 7;
                }
                ovl662_editor_workspace.config22 = 6;
                PlaySfx(1);
            }
        }
    }
}

void ovl662_tool_select_color_07(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[2] = 0;
        ovl662_editor_workspace.optionIndices98.flat[5] = 3;
    } else if (mode > 0) {
        switch (mode) {
        case 1:
        ovl662_editor_workspace.optionIndices98.flat[1] = 1;
        ovl662_editor_workspace.selectorsB8[0] = 1;
        ovl662_editor_workspace.selectorsB8[3] = 6;
        ovl662_select_option_target(0);
            break;
        case 2:
        ovl662_editor_workspace.optionIndices98.flat[2] = 1;
        ovl662_editor_workspace.selectorsB8[3] = 8;
        ovl662_select_option_target(0);
            break;
        case 4:

        ovl662_editor_workspace.optionIndices98.flat[5] = 2;
        ovl662_editor_workspace.selectorsB8[2] = 5;
        ovl662_select_option_target(1);
            break;
        case 8:

        ovl662_editor_workspace.optionIndices98.flat[5] = 4;
        ovl662_editor_workspace.selectorsB8[2] = 7;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(0x1B) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[2] = 0;
                    ovl662_editor_workspace.optionIndices98.flat[5] = 3;
                    ovl662_editor_workspace.saved64[0] = 0x1B;
                    ovl662_editor_workspace.saved64[1] = 2;
                    ovl662_editor_workspace.saved64[2] = 2;
                    ovl662_editor_workspace.saved64[3] = 6;
                    ovl662_editor_workspace.saved64[4] = 7;
                }
                ovl662_editor_workspace.config22 = 7;
                PlaySfx(1);
            }
        }
    }
}

void ovl662_tool_select_color_08(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[2] = 0;
        ovl662_editor_workspace.optionIndices98.flat[5] = 4;
    } else if (mode > 0) {
        switch (mode) {
        case 1:
        ovl662_editor_workspace.optionIndices98.flat[1] = 1;
        ovl662_editor_workspace.selectorsB8[0] = 1;
        ovl662_editor_workspace.selectorsB8[3] = 6;
        ovl662_select_option_target(0);
            break;
        case 2:
        ovl662_editor_workspace.optionIndices98.flat[0] = 0;
        ovl662_editor_workspace.selectorsB8[0] = 0;
        ovl662_editor_workspace.selectorsB8[3] = 0;
        ovl662_select_option_target(0);
            break;
        case 4:
        ovl662_editor_workspace.optionIndices98.flat[5] = 3;
        ovl662_editor_workspace.selectorsB8[2] = 6;
        ovl662_select_option_target(1);
            break;
        case 8:
        ovl662_editor_workspace.selectorsB8[1] = 3;
        ovl662_editor_workspace.selectorsB8[2] = 8;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(0x1C) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[2] = 0;
                    ovl662_editor_workspace.optionIndices98.flat[5] = 4;
                    ovl662_editor_workspace.saved64[0] = 0x1C;
                    ovl662_editor_workspace.saved64[1] = 2;
                    ovl662_editor_workspace.saved64[2] = 2;
                    ovl662_editor_workspace.saved64[3] = 7;
                    ovl662_editor_workspace.saved64[4] = 7;
                }
                ovl662_editor_workspace.config22 = 8;
                PlaySfx(1);
            }
        }
    }
}

void ovl662_tool_select_color_09(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[2] = 1;
        ovl662_editor_workspace.optionIndices98.flat[4] = 0;
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
        ovl662_editor_workspace.selectorsB8[1] = 4;
        ovl662_editor_workspace.selectorsB8[2] = 9;
        ovl662_select_option_target(1);
            break;
        case 8:

        ovl662_editor_workspace.optionIndices98.flat[4] = 1;
        ovl662_editor_workspace.selectorsB8[2] = 1;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(0x1D) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[2] = 1;
                    ovl662_editor_workspace.optionIndices98.flat[4] = 0;
                    ovl662_editor_workspace.saved64[0] = 0x1D;
                    ovl662_editor_workspace.saved64[1] = 2;
                    ovl662_editor_workspace.saved64[2] = 1;
                    ovl662_editor_workspace.saved64[3] = 0;
                    ovl662_editor_workspace.saved64[4] = 5;
                }
                ovl662_editor_workspace.config22 = 9;
                PlaySfx(1);
            }
        }
    }
}

void ovl662_tool_select_color_10(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[2] = 1;
        ovl662_editor_workspace.optionIndices98.flat[4] = 1;
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
        ovl662_editor_workspace.optionIndices98.flat[4] = 0;
        ovl662_editor_workspace.selectorsB8[2] = 0;
        ovl662_select_option_target(1);
            break;
        case 8:
        ovl662_editor_workspace.optionIndices98.flat[4] = 2;
        ovl662_editor_workspace.selectorsB8[2] = 2;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(0x1E) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[2] = 1;
                    ovl662_editor_workspace.optionIndices98.flat[4] = 1;
                    ovl662_editor_workspace.saved64[0] = 0x1E;
                    ovl662_editor_workspace.saved64[1] = 2;
                    ovl662_editor_workspace.saved64[2] = 1;
                    ovl662_editor_workspace.saved64[3] = 1;
                    ovl662_editor_workspace.saved64[4] = 5;
                }
                ovl662_editor_workspace.config22 = 0xA;
                PlaySfx(1);
            }
        }
    }
}

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

void ovl662_tool_select_color_12(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[2] = 1;
        ovl662_editor_workspace.optionIndices98.flat[5] = 0;
    } else if (mode > 0) {
        switch (mode) {
        case 1:
        ovl662_editor_workspace.optionIndices98.flat[2] = 0;
        ovl662_editor_workspace.selectorsB8[3] = 7;
        ovl662_select_option_target(0);
            break;
        case 2:
        ovl662_editor_workspace.optionIndices98.flat[0] = 0;
        ovl662_editor_workspace.selectorsB8[0] = 0;
        ovl662_editor_workspace.selectorsB8[3] = 0;
        ovl662_select_option_target(0);
            break;
        case 4:
        ovl662_editor_workspace.optionIndices98.flat[4] = 2;
        ovl662_editor_workspace.selectorsB8[1] = 1;
        ovl662_editor_workspace.selectorsB8[2] = 2;
        ovl662_select_option_target(1);
            break;
        case 8:
        ovl662_editor_workspace.optionIndices98.flat[5] = 1;
        ovl662_editor_workspace.selectorsB8[2] = 4;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(0x20) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[2] = 1;
                    ovl662_editor_workspace.optionIndices98.flat[5] = 0;
                    ovl662_editor_workspace.saved64[0] = 0x20;
                    ovl662_editor_workspace.saved64[1] = 2;
                    ovl662_editor_workspace.saved64[2] = 2;
                    ovl662_editor_workspace.saved64[3] = 3;
                    ovl662_editor_workspace.saved64[4] = 8;
                }
                ovl662_editor_workspace.config22 = 0xC;
                PlaySfx(1);
            }
        }
    }
}

void ovl662_tool_select_color_13(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[2] = 1;
        ovl662_editor_workspace.optionIndices98.flat[5] = 1;
    } else if (mode > 0) {
        switch (mode) {
        case 1:
        ovl662_editor_workspace.optionIndices98.flat[2] = 0;
        ovl662_editor_workspace.selectorsB8[3] = 7;
        ovl662_select_option_target(0);
            break;
        case 2:
        ovl662_editor_workspace.optionIndices98.flat[0] = 0;
        ovl662_editor_workspace.selectorsB8[0] = 0;
        ovl662_editor_workspace.selectorsB8[3] = 0;
        ovl662_select_option_target(0);
            break;
        case 4:

        ovl662_editor_workspace.optionIndices98.flat[5] = 0;
        ovl662_editor_workspace.selectorsB8[2] = 3;
        ovl662_select_option_target(1);
            break;
        case 8:
        ovl662_editor_workspace.optionIndices98.flat[5] = 2;
        ovl662_editor_workspace.selectorsB8[2] = 5;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(0x21) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[2] = 1;
                    ovl662_editor_workspace.optionIndices98.flat[5] = 1;
                    ovl662_editor_workspace.saved64[0] = 0x21;
                    ovl662_editor_workspace.saved64[1] = 2;
                    ovl662_editor_workspace.saved64[2] = 2;
                    ovl662_editor_workspace.saved64[3] = 4;
                    ovl662_editor_workspace.saved64[4] = 8;
                }
                ovl662_editor_workspace.config22 = 0xD;
                PlaySfx(1);
            }
        }
    }
}

void ovl662_tool_select_color_14(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[2] = 1;
        ovl662_editor_workspace.optionIndices98.flat[5] = 2;
    } else if (mode > 0) {
        switch (mode) {
        case 1:
        ovl662_editor_workspace.optionIndices98.flat[2] = 0;
        ovl662_editor_workspace.selectorsB8[3] = 7;
        ovl662_select_option_target(0);
            break;
        case 2:
        ovl662_editor_workspace.optionIndices98.flat[0] = 0;
        ovl662_editor_workspace.selectorsB8[0] = 0;
        ovl662_editor_workspace.selectorsB8[3] = 0;
        ovl662_select_option_target(0);
            break;
        case 4:
        ovl662_editor_workspace.optionIndices98.flat[5] = 1;
        ovl662_editor_workspace.selectorsB8[2] = 4;
        ovl662_select_option_target(1);
            break;
        case 8:
        ovl662_editor_workspace.optionIndices98.flat[5] = 3;
        ovl662_editor_workspace.selectorsB8[2] = 6;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(0x22) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[2] = 1;
                    ovl662_editor_workspace.optionIndices98.flat[5] = 2;
                    ovl662_editor_workspace.saved64[0] = 0x22;
                    ovl662_editor_workspace.saved64[1] = 2;
                    ovl662_editor_workspace.saved64[2] = 2;
                    ovl662_editor_workspace.saved64[3] = 5;
                    ovl662_editor_workspace.saved64[4] = 8;
                }
                ovl662_editor_workspace.config22 = 0xE;
                PlaySfx(1);
            }
        }
    }
}

void ovl662_tool_select_color_15(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[2] = 1;
        ovl662_editor_workspace.optionIndices98.flat[5] = 3;
    } else if (mode > 0) {
        switch (mode) {
        case 1:
        ovl662_editor_workspace.optionIndices98.flat[2] = 0;
        ovl662_editor_workspace.selectorsB8[3] = 7;
        ovl662_select_option_target(0);
            break;
        case 2:
        ovl662_editor_workspace.optionIndices98.flat[0] = 0;
        ovl662_editor_workspace.selectorsB8[0] = 0;
        ovl662_editor_workspace.selectorsB8[3] = 0;
        ovl662_select_option_target(0);
            break;
        case 4:
        ovl662_editor_workspace.optionIndices98.flat[5] = 2;
        ovl662_editor_workspace.selectorsB8[2] = 5;
        ovl662_select_option_target(1);
            break;
        case 8:
        ovl662_editor_workspace.selectorsB8[1] = 3;
        ovl662_editor_workspace.selectorsB8[2] = 8;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(0x23) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[2] = 1;
                    ovl662_editor_workspace.optionIndices98.flat[5] = 3;
                    ovl662_editor_workspace.saved64[0] = 0x23;
                    ovl662_editor_workspace.saved64[1] = 2;
                    ovl662_editor_workspace.saved64[2] = 2;
                    ovl662_editor_workspace.saved64[3] = 6;
                    ovl662_editor_workspace.saved64[4] = 8;
                }
                ovl662_editor_workspace.config22 = 0xF;
                PlaySfx(1);
            }
        }
    }
}
