#include "game/system/pad.h"
#include "rr/overlay662_modal.h"
#include "rr/overlay662_rotate.h"
#include "rr/overlay662_mirror.h"
#include "rr/overlay662_tool_action.h"
#include "rr/overlay662_confirm.h"

void ovl662_tool_undo_modal(s32 mode) {
    u8 phase;
    s32 selection;

    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[0] = 0;
        return;
    }
    if (mode > 0) {
        switch (mode) {
    case 1:
        if (ovl662_editor_workspace.selectorsB8[1] == 0) {
            ovl662_editor_workspace.optionIndices98.flat[2] = mode;
            ovl662_editor_workspace.selectorsB8[0] = 2;
            ovl662_editor_workspace.selectorsB8[1] = mode;
            ovl662_editor_workspace.selectorsB8[3] = 5;
        } else {
            if (ovl662_editor_workspace.optionIndices98.flat[5] == 4) {
                ovl662_editor_workspace.optionIndices98.flat[2] = 0;
            } else {
                ovl662_editor_workspace.optionIndices98.flat[2] = mode;
            }
            ovl662_editor_workspace.selectorsB8[0] = 2;
            ovl662_editor_workspace.selectorsB8[1] = 2;
            ovl662_editor_workspace.selectorsB8[3] = 8;
        }
        ovl662_select_option_target(0);
        return;
    case 2:
        ovl662_editor_workspace.optionIndices98.flat[0] = 1;
        ovl662_editor_workspace.selectorsB8[3] = 1;
        ovl662_select_option_target(0);
        return;
    case 4:
        ovl662_editor_workspace.selectorsB8[1] = mode;
        ovl662_editor_workspace.selectorsB8[2] = mode;
        ovl662_select_option_target(1);
        return;
    case 8:
        ovl662_editor_workspace.selectorsB8[1] = 3;
        ovl662_editor_workspace.selectorsB8[2] = 3;
        ovl662_select_option_target(1);
        return;
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
                        if (PlayerControllers[0].edges08.fields.repeat & 0x1000) {
                            if (ovl662_editor_workspace.stateD2 != 0) {
                                ovl662_editor_workspace.stateD2 = 0U;
                                goto block_36;
                            }
                        } else if (PlayerControllers[0].edges08.fields.repeat & 0x4000) {
                            if (ovl662_editor_workspace.stateD2 != 1) {
                                ovl662_editor_workspace.stateD2 = 1U;
block_36:
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
                    ovl662_editor_workspace.flagsD1 = 0;
                    return;
                }
                break;
            case 9:
                if (ovl662_editor_workspace.fade00 == 0) {
                    ovl662_emblem_edit_from_menu = 1;
                    ModeGroup = 1;
                    ModeIndex = 0x45;
                    SubmodeState = 0;
                    return;
                }
                break;
            }
        } else {
            if ((ovl662_point_outside_button(0) == 0) && (ovl662_editor_cursor_state.words[1] & 1)) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[0] = 0;
                    ovl662_editor_workspace.optionIndices98.flat[3] = 0;
                    ovl662_editor_workspace.saved64[0] = 0;
                    ovl662_editor_workspace.saved64[1] = 0;
                    ovl662_editor_workspace.saved64[2] = 0;
                    ovl662_editor_workspace.saved64[3] = 0;
                    ovl662_editor_workspace.saved64[4] = 0;
                }
                ovl662_editor_cursor_state.words[4] = 0;
                ovl662_editor_cursor_state.words[0] = (ovl662_editor_cursor_state.words[0] | 8);
                PlaySfx(1);
                ovl662_editor_cursor_state.words[0] = (ovl662_editor_cursor_state.words[0] | 1);
                CardDialogueStatePtr->b4 = 3;
                CardDialogueStatePtr->b5 = -1;
                CardDialogueStatePtr->st = 0;
                ovl662_editor_workspace.flagsD1 = 3U;
                BadgeRows->half02 = 1;
                BadgeRows->state10 = 0;
                UiRows[0].byte14 = 0U;
                UiRows[1].byte14 = 0U;
                ovl662_editor_workspace.stateD2 = 1U;
            }
            return;
        }
}

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

void ovl662_tool_center_cursor(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[0] = 2;
        ovl662_editor_workspace.optionIndices98.flat[3] = 0;
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
            ovl662_editor_workspace.selectorsB8[1] = 4;
            ovl662_editor_workspace.selectorsB8[2] = 4;
            ovl662_select_option_target(1);
            break;
        case 8:
            ovl662_editor_workspace.optionIndices98.flat[3] = 1;
            ovl662_editor_workspace.selectorsB8[2] = 1;
            ovl662_select_option_target(1);
            break;
        }
    } else if (ovl662_point_outside_button(4) == 0 && (ovl662_editor_cursor_state.words[1] & 1)) {
        if (ovl662_editor_cursor_state.words[0] & 4) {
            ovl662_restore_saved_selection();
        } else {
            ovl662_editor_workspace.optionIndices98.flat[0] = 2;
            ovl662_editor_workspace.optionIndices98.flat[3] = 0;
            ovl662_editor_workspace.saved64[0] = 4;
            ovl662_editor_workspace.saved64[1] = 0;
            ovl662_editor_workspace.saved64[2] = 0;
            ovl662_editor_workspace.saved64[3] = 0;
            ovl662_editor_workspace.saved64[4] = 2;
        }
        ovl662_editor_workspace.config1E = 4;
        PlaySfx(3);
        ovl662_editor_cursor_state.words[0] |= 2;
        ovl662_editor_cursor_state.points.first20 = ovl662_editor_cursor_state.points.cursor14;
        ovl662_editor_cursor_state.points.second24.x = 146;
        ovl662_editor_cursor_state.points.second24.y = 199;
        ovl662_editor_cursor_state.points.word28 = 0;
        ovl662_editor_cursor_state.points.distance2C = ovl662_calc_point_distance(&ovl662_editor_cursor_state.points.first20, &ovl662_editor_cursor_state.points.second24);
    }
}

void ovl662_tool_clear_layer(s32 mode) {
    u32 *dst;
    s32 i;

    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[0] = 2;
        ovl662_editor_workspace.optionIndices98.flat[3] = 1;
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
            ovl662_editor_workspace.optionIndices98.flat[3] = 0;
            ovl662_editor_workspace.selectorsB8[2] = 0;
            ovl662_select_option_target(1);
            break;
        case 8:
            ovl662_editor_workspace.selectorsB8[1] = 2;
            ovl662_editor_workspace.selectorsB8[2] = 2;
            ovl662_select_option_target(1);
            break;
        }
    } else if (ovl662_point_outside_button(5) == 0 && (ovl662_editor_cursor_state.words[1] & 1)) {
        if (ovl662_is_active_layer_empty() != 0) {
            ((GarageButton *)ovl662_button_rect_lookup[5])->timer0E = 7;
            PlaySfx(4);
        } else {
            if (ovl662_editor_cursor_state.words[0] & 4) {
                ovl662_restore_saved_selection();
            } else {
                ovl662_editor_workspace.optionIndices98.flat[0] = 2;
                ovl662_editor_workspace.optionIndices98.flat[3] = 1;
                ovl662_editor_workspace.saved64[0] = 5;
                ovl662_editor_workspace.saved64[1] = 0;
                ovl662_editor_workspace.saved64[2] = 0;
                ovl662_editor_workspace.saved64[3] = 1;
                ovl662_editor_workspace.saved64[4] = 2;
            }
            ovl662_backup_layer_and_palette();
            ((GarageButton *)ovl662_button_rect_lookup[5])->timer0E = 7;
            PlaySfx(1);
            if (ovl662_editor_workspace.layer23 == 0) {
                dst = ovl662_editor_workspace.image38[ovl662_editor_workspace.active21];
            } else {
                dst = ovl662_editor_workspace.image40[ovl662_editor_workspace.active21];
            }
            for (i = 0; i < 512; i++) {
                *dst++ = 0;
            }
        }
    }
}

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

void ovl662_tool_cycle_preview_bg(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[0] = 3;
    } else if (mode > 0) {
        switch (mode) {
        case 1:
        ovl662_editor_workspace.optionIndices98.flat[0] = 2;
        ovl662_editor_workspace.selectorsB8[1] = 0;
        ovl662_editor_workspace.selectorsB8[3] = 2;
        ovl662_select_option_target(0);
            break;
        case 2:
        ovl662_editor_workspace.optionIndices98.flat[0] = 4;
        ovl662_editor_workspace.selectorsB8[1] = 1;
        ovl662_editor_workspace.selectorsB8[3] = 1;
        ovl662_select_option_target(0);
            break;
        case 4:
        ovl662_editor_workspace.selectorsB8[1] = 4;
        ovl662_editor_workspace.selectorsB8[2] = 4;
        ovl662_select_option_target(1);
            break;
        case 8:
        ovl662_editor_workspace.selectorsB8[1] = 2;
        ovl662_editor_workspace.selectorsB8[2] = 2;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(7) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[0] = 3;
                    ovl662_editor_workspace.optionIndices98.flat[3] = 0;
                    ovl662_editor_workspace.saved64[0] = 7;
                    ovl662_editor_workspace.saved64[1] = 0;
                    ovl662_editor_workspace.saved64[2] = 0;
                    ovl662_editor_workspace.saved64[3] = 0;
                    ovl662_editor_workspace.saved64[4] = 3;
                }
                ((GarageButton *)ovl662_button_rect_lookup[7])->timer0E = 7;
                PlaySfx(1);
                ovl662_editor_workspace.config1F++;
                if (ovl662_editor_workspace.config1F >= 3) {
                    ovl662_editor_workspace.config1F = 0;
                }
            }
        }
    }
}

void ovl662_tool_cycle_zoom(s32 mode) {
    if (mode < 0) {
ovl662_editor_workspace.optionIndices98.flat[0] = 3;
    } else if (mode > 0) {
        switch (mode) {
        case 1:
        ovl662_editor_workspace.optionIndices98.flat[0] = 2;
        ovl662_editor_workspace.selectorsB8[3] = 2;
        ovl662_select_option_target(0);
            break;
        case 2:

        ovl662_editor_workspace.optionIndices98.flat[0] = 4;
        ovl662_editor_workspace.selectorsB8[3] = 4;
        ovl662_select_option_target(0);
            break;
        case 4:
        ovl662_editor_workspace.selectorsB8[1] = 0;
        ovl662_editor_workspace.selectorsB8[2] = 0;
        ovl662_select_option_target(1);
            break;
        case 8:
        ovl662_editor_workspace.selectorsB8[1] = 3;
        ovl662_editor_workspace.selectorsB8[2] = 3;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(8) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[0] = 3;
                    ovl662_editor_workspace.optionIndices98.flat[5] = 0;
                    ovl662_editor_workspace.saved64[0] = 8;
                    ovl662_editor_workspace.saved64[1] = 0;
                    ovl662_editor_workspace.saved64[2] = 2;
                    ovl662_editor_workspace.saved64[3] = 2;
                    ovl662_editor_workspace.saved64[4] = 3;
                }
                ((GarageButton *)ovl662_button_rect_lookup[8])->timer0E = 7;
                PlaySfx(1);
                ovl662_editor_workspace.config1C <<= 1;
                if (ovl662_editor_workspace.config1C >= 0x11) {
                    ovl662_editor_workspace.config1C = 2U;
                    ovl662_editor_workspace.reset08[1] = 0;
                    ovl662_editor_workspace.reset08[0] = 0;
                }
            }
        }
    }
}

void ovl662_tool_select_pen(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[0] = 4;
    } else if (mode > 0) {
        switch (mode) {
        case 1:
        ovl662_editor_workspace.optionIndices98.flat[0] = 3;
        ovl662_editor_workspace.selectorsB8[3] = 0;
        ovl662_select_option_target(0);
            break;
        case 2:
        ovl662_editor_workspace.optionIndices98.flat[1] = 0;
        ovl662_editor_workspace.selectorsB8[0] = 1;
        ovl662_editor_workspace.selectorsB8[3] = 2;
        ovl662_select_option_target(0);
            break;
        case 4:
        ovl662_editor_workspace.selectorsB8[1] = 4;
        ovl662_editor_workspace.selectorsB8[2] = 4;
        ovl662_select_option_target(1);
            break;
        case 8:
        ovl662_editor_workspace.selectorsB8[1] = 2;
        ovl662_editor_workspace.selectorsB8[2] = 2;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(9) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[0] = 4;
                    ovl662_editor_workspace.optionIndices98.flat[4] = 1;
                    ovl662_editor_workspace.saved64[0] = 9;
                    ovl662_editor_workspace.saved64[1] = 0;
                    ovl662_editor_workspace.saved64[2] = 1;
                    ovl662_editor_workspace.saved64[3] = 0;
                    ovl662_editor_workspace.saved64[4] = 1;
                }
                ovl662_editor_workspace.config1E = 0;
                PlaySfx(1);
            }
        }
    }
}

void ovl662_tool_toggle_emblem_side(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[0] = 4;
    } else if (mode > 0) {
        switch (mode) {
        case 1:

ovl662_editor_workspace.optionIndices98.flat[0] = 3;
        ovl662_editor_workspace.selectorsB8[3] = 3;
        ovl662_select_option_target(0);
            break;
        case 2:
        ovl662_editor_workspace.selectorsB8[0] = 1;
ovl662_editor_workspace.optionIndices98.flat[1] = 0;
        ovl662_editor_workspace.selectorsB8[3] = 5;
        ovl662_select_option_target(0);
            break;
        case 4:
        ovl662_editor_workspace.selectorsB8[1] = 1;
        ovl662_editor_workspace.selectorsB8[2] = 0;
        ovl662_select_option_target(1);
            break;
        case 8:
        ovl662_editor_workspace.selectorsB8[1] = 3;
        ovl662_editor_workspace.selectorsB8[2] = 3;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(0xA) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[0] = 4;
                    ovl662_editor_workspace.optionIndices98.flat[5] = 0;
                    ovl662_editor_workspace.saved64[0] = 0xA;
                    ovl662_editor_workspace.saved64[1] = 0;
                    ovl662_editor_workspace.saved64[2] = 2;
                    ovl662_editor_workspace.saved64[3] = 2;
                    ovl662_editor_workspace.saved64[4] = 4;
                }
                ((GarageButton *)ovl662_button_rect_lookup[10])->timer0E = 7;
                ovl662_editor_workspace.active21 = (u8) ((u8) ovl662_editor_workspace.active21 < 1U);
                PlaySfx(1);
            }
        }
    }
}

void ovl662_tool_select_line(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[1] = 0;
    } else if (mode > 0) {
        switch (mode) {
        case 1:
        ovl662_editor_workspace.optionIndices98.flat[0] = 4;
        ovl662_editor_workspace.selectorsB8[0] = 0;
        ovl662_editor_workspace.selectorsB8[3] = 1;
        ovl662_select_option_target(0);
            break;
        case 2:
        ovl662_editor_workspace.optionIndices98.flat[1] = 1;
        ovl662_editor_workspace.selectorsB8[3] = 3;
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
        if (ovl662_point_outside_button(0xB) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[1] = 0;
                    ovl662_editor_workspace.optionIndices98.flat[4] = 1;
                    ovl662_editor_workspace.saved64[0] = 0xB;
                    ovl662_editor_workspace.saved64[1] = 1;
                    ovl662_editor_workspace.saved64[2] = 1;
                    ovl662_editor_workspace.saved64[3] = 0;
                    ovl662_editor_workspace.saved64[4] = 2;
                }
                ovl662_editor_workspace.config1E = 1;
                PlaySfx(1);
            }
        }
    }
}

void ovl662_tool_select_fill(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[1] = 0;
    } else if (mode > 0) {
        switch (mode) {
        case 1:

ovl662_editor_workspace.optionIndices98.flat[0] = 4;
        ovl662_editor_workspace.selectorsB8[0] = 0;
        ovl662_editor_workspace.selectorsB8[3] = 4;
        ovl662_select_option_target(0);
            break;
        case 2:
        ovl662_editor_workspace.optionIndices98.flat[1] = 1;
        ovl662_editor_workspace.selectorsB8[3] = 6;
        ovl662_select_option_target(0);
            break;
        case 4:
        ovl662_editor_workspace.selectorsB8[1] = 1;
        ovl662_editor_workspace.selectorsB8[2] = 0;
        ovl662_select_option_target(1);
            break;
        case 8:
        ovl662_editor_workspace.selectorsB8[1] = 3;
        ovl662_editor_workspace.selectorsB8[2] = 2;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(0xC) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[1] = 0;
                    ovl662_editor_workspace.optionIndices98.flat[5] = 0;
                    ovl662_editor_workspace.saved64[0] = 0xC;
                    ovl662_editor_workspace.saved64[1] = 1;
                    ovl662_editor_workspace.saved64[2] = 2;
                    ovl662_editor_workspace.saved64[3] = 1;
                    ovl662_editor_workspace.saved64[4] = 5;
                }
                ovl662_editor_workspace.config1E = 2;
                PlaySfx(1);
            }
        }
    }
}

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

void ovl662_tool_select_move(s32 mode) {
    if (mode < 0) {
        ovl662_editor_workspace.optionIndices98.flat[1] = 1;
    } else if (mode > 0) {
        switch (mode) {
        case 1:

ovl662_editor_workspace.optionIndices98.flat[1] = 0;
        ovl662_editor_workspace.selectorsB8[3] = 5;
        ovl662_select_option_target(0);
            break;
        case 2:

ovl662_editor_workspace.optionIndices98.flat[2] = 0;
        ovl662_editor_workspace.selectorsB8[0] = 2;
        ovl662_editor_workspace.selectorsB8[3] = 7;
        ovl662_select_option_target(0);
            break;
        case 4:
        ovl662_editor_workspace.selectorsB8[1] = 1;
        ovl662_editor_workspace.selectorsB8[2] = 0;
        ovl662_select_option_target(1);
            break;
        case 8:
        ovl662_editor_workspace.selectorsB8[1] = 3;
        ovl662_editor_workspace.selectorsB8[2] = 2;
        ovl662_select_option_target(1);
            break;
        }
    } else {
        if (ovl662_point_outside_button(0xE) == 0) {
            if (ovl662_editor_cursor_state.words[1] & 1) {
                if (ovl662_editor_cursor_state.words[0] & 4) {
                    ovl662_restore_saved_selection();
                } else {
                    ovl662_editor_workspace.optionIndices98.flat[1] = 1;
                    ovl662_editor_workspace.optionIndices98.flat[5] = 0;
                    ovl662_editor_workspace.saved64[0] = 0xE;
                    ovl662_editor_workspace.saved64[1] = 1;
                    ovl662_editor_workspace.saved64[2] = 2;
                    ovl662_editor_workspace.saved64[3] = 1;
                    ovl662_editor_workspace.saved64[4] = 6;
                }
                ovl662_editor_workspace.config1E = 3;
                PlaySfx(1);
            }
        }
    }
}

void ovl662_tool_swap_emblem_sides(s32 action)
{
    u32 *buffer;
    union {
        u32 *ptr;
        intptr_t addr;
    } image, palette, out;
    union {
        GarageTimBytes *ptr;
        intptr_t addr;
    } tim;
    intptr_t t1, t2, t3;
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
            tim.ptr = state->tim2C[selected];
            image.ptr = state->image40[selected];
            palette.ptr = state->paletteBackup50[selected];
            state->tim2C[selected] = (GarageTimBytes *)buffer;
            state->image40[selected] = state->image38[selected];
            buffer = state->palette48[selected];
            t1 = tim.addr | 1;
            t1 &= tim.addr;
            t2 = tim.addr | 1;
            t2 &= tim.addr;
            t1 &= t2;
            t2 = tim.addr | 1;
            t2 &= tim.addr;
            out.addr = t1 & t2;
            state->buffers24[selected] = out.ptr;
            t1 = image.addr | 1;
            t1 &= image.addr;
            t2 = image.addr | 1;
            t2 &= image.addr;
            t1 &= t2;
            t2 = image.addr | 1;
            t2 &= image.addr;
            t3 = image.addr | 1;
            t3 &= image.addr;
            t2 &= t3;
            out.addr = t1 & t2;
            state->image38[selected] = out.ptr;
            t1 = palette.addr | 1;
            t1 &= palette.addr;
            t2 = palette.addr | 1;
            t2 &= palette.addr;
            t1 &= t2;
            t2 = palette.addr | 1;
            t2 &= palette.addr;
            t3 = palette.addr | 1;
            t3 &= palette.addr;
            t2 &= t3;
            t1 &= t2;
            t2 = palette.addr | 1;
            t2 &= palette.addr;
            out.addr = t1 & t2;
            state->palette48[selected] = out.ptr;
            state->paletteBackup50[selected] = buffer;
        }
    }
}
