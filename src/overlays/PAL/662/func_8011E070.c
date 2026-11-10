#include "game/system/pad.h"
#include "rr/overlay662_cursor_control.h"

void ovl662_update_editor_cursor(void) {
    GaragePoint movement;
    GaragePoint destination;
    u32 events;
    s32 flags;
    u16 ySum;
    u16 xSum;

    PsyqMemset(&movement, 0, 4);
    events = 0;
    if (!(ovl662_editor_cursor_state.words[0] & 1)) {
        if (ovl662_editor_cursor_state.words[0] & 2) {
            ovl662_step_panel_fade_timers();
            if ((s32)ovl662_editor_cursor_state.points.word28 < 0x1000) {
                return;
            }
            ovl662_editor_cursor_state.points.cursor18 = ovl662_editor_cursor_state.points.cursor14;
            ovl662_editor_cursor_state.words[0] &= ~2U;
            ovl662_editor_cursor_state.words[0] &= ~4U;
        }
        if (ovl662_editor_cursor_state.words[0] & 4) {
            if (!(ovl662_editor_cursor_state.words[0] & 8)) {
                ovl662_step_panel_fade_timers();
                if ((s32)ovl662_editor_cursor_state.points.word28 < 0x1000) {
                    return;
                }
                ovl662_poll_stick_direction_repeat(&events);
                ovl662_poll_dpad_direction_repeat(&events);
                ovl662_handle_shortcut_buttons(events);
            }
        } else {
            ovl662_read_analog_cursor_delta(&movement);
            ovl662_read_mouse_cursor_delta(&movement);
            ovl662_read_dpad_cursor_delta(&movement);
            xSum = ovl662_editor_cursor_state.points.cursor18.x + movement.x;
            ySum = ovl662_editor_cursor_state.points.cursor18.y + movement.y;
            ovl662_editor_cursor_state.points.cursor18.x = xSum;
            ovl662_editor_cursor_state.points.cursor18.y = ySum;
            if ((s16) xSum < ovl662_editor_cursor_state.halves[24]) {
                ovl662_editor_cursor_state.points.cursor18.x = (u16) ovl662_editor_cursor_state.halves[24];
            }
            if (((ovl662_editor_cursor_state.halves[24] + ovl662_editor_cursor_state.halves[26]) - 1) < (s16) ovl662_editor_cursor_state.points.cursor18.x) {
                ovl662_editor_cursor_state.points.cursor18.x = (u16) (((u16) ovl662_editor_cursor_state.halves[24] + (u16) ovl662_editor_cursor_state.halves[26]) - 1);
            }
            if ((s16) ySum < ovl662_editor_cursor_state.halves[25]) {
                ovl662_editor_cursor_state.points.cursor18.y = (u16) ovl662_editor_cursor_state.halves[25];
            }
            if (((ovl662_editor_cursor_state.halves[25] + ovl662_editor_cursor_state.halves[27]) - 1) < (s16) ovl662_editor_cursor_state.points.cursor18.y) {
                ovl662_editor_cursor_state.points.cursor18.y = (u16) (((u16) ovl662_editor_cursor_state.halves[25] + (u16) ovl662_editor_cursor_state.halves[27]) - 1);
            }
            ovl662_editor_cursor_state.points.cursor14 = ovl662_editor_cursor_state.points.cursor18;
            flags = ovl662_editor_cursor_state.words[0];
            if (!(flags & 8) && (PlayerControllers[0].input00.fields.type != 0x12) && (PlayerControllers[0].edges08.fields.pressed & 4)) {
                ovl662_editor_cursor_state.words[0] = (flags | 4);
                ovl662_editor_cursor_state.words[1] = 0x30;
                ovl662_editor_cursor_state.points.first20 = ovl662_editor_cursor_state.points.cursor18;
                destination=ovl662_get_rect_center(ovl662_editor_workspace.saved64[0]);
                ovl662_editor_cursor_state.points.second24 = destination;
                ovl662_editor_cursor_state.points.word28 = 0;
                ovl662_editor_cursor_state.points.distance2C = ovl662_calc_point_distance(&ovl662_editor_cursor_state.points.first20,&ovl662_editor_cursor_state.points.second24);
                ovl662_editor_workspace.current60 = ovl662_editor_workspace.saved64[0];
                ovl662_editor_workspace.selectorsB8[0] = ovl662_editor_workspace.saved64[1];
                ovl662_editor_workspace.selectorsB8[1] = ovl662_editor_workspace.saved64[2];
                ovl662_editor_workspace.selectorsB8[2] = ovl662_editor_workspace.saved64[3];
                ovl662_editor_workspace.selectorsB8[3] = ovl662_editor_workspace.saved64[4];
                ovl662_action_lookup[((GarageButton *)ovl662_button_rect_lookup[ovl662_editor_workspace.saved64[0]])->action0A]->callback(-1);
                PlaySfx(3);
                return;
            }
        }
        ovl662_update_button_states();
    }
}
