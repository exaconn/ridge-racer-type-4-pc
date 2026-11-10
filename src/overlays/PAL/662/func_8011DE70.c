#include "game/system/pad.h"
#include "rr/overlay662_garage.h"
#include "rr/tables.h"

s32 ovl662_map_controller_buttons(s32 mode);

/* Repeat-key handling for the two garage scroll directions. */
void ovl662_update_button_states(void)
{
    s32 first;
    s32 second;

    if (PlayerControllers[0].input00.fields.type == 0x23) {
        first = PlayerControllers[0].input00.fields.buttons & 0x840;
        second = PlayerControllers[0].input00.fields.buttons & 0x80;
    } else if (PlayerControllers[0].input00.fields.type != 0x12) {
        first = PlayerControllers[0].input00.fields.buttons & 0x820;
        second = PlayerControllers[0].input00.fields.buttons & 0x40;
    }
    if (PlayerControllers[1].input00.fields.type == 0x12) {
        first |= PlayerControllers[1].input00.fields.buttons & 8;
        second |= PlayerControllers[1].input00.fields.buttons & 4;
    }

    if (ovl662_editor_cursor_state.words[1] & 0x10) {
        if (first) {
            ovl662_editor_cursor_state.words[1] = (ovl662_editor_cursor_state.words[1] | 0x10) & ~5;
            if (ovl662_map_controller_buttons(1)) {
                ovl662_editor_cursor_state.words[1] |= 0x40;
            } else {
                ovl662_editor_cursor_state.words[1] &= ~0x40;
            }
        } else {
            ovl662_editor_cursor_state.words[1] = (ovl662_editor_cursor_state.words[1] & ~0x51) | 4;
        }
    } else {
        if (first) {
            ovl662_editor_cursor_state.words[1] = (ovl662_editor_cursor_state.words[1] | 0x11) & ~4;
            ovl662_editor_cursor_state.words[2] = 0;
            if (ovl662_map_controller_buttons(1)) {
                ovl662_editor_cursor_state.words[1] |= 0x40;
            } else {
                ovl662_editor_cursor_state.words[1] &= ~0x40;
            }
        } else {
            ovl662_editor_cursor_state.words[1] &= ~0x55;
        }
    }

    if (ovl662_editor_cursor_state.words[1] & 0x20) {
        if (second) {
            ovl662_editor_cursor_state.words[1] = (ovl662_editor_cursor_state.words[1] | 0x20) & ~0xA;
            if (ovl662_map_controller_buttons(0)) {
                ovl662_editor_cursor_state.words[1] |= 0x80;
            } else {
                ovl662_editor_cursor_state.words[1] &= ~0x80;
            }
        } else {
            ovl662_editor_cursor_state.words[1] = (ovl662_editor_cursor_state.words[1] & ~0xA2) | 8;
        }
    } else {
        if (second) {
            ovl662_editor_cursor_state.words[1] = (ovl662_editor_cursor_state.words[1] | 0x22) & ~8;
            ovl662_editor_cursor_state.words[3] = 0;
            if (ovl662_map_controller_buttons(0)) {
                ovl662_editor_cursor_state.words[1] |= 0x80;
            } else {
                ovl662_editor_cursor_state.words[1] &= ~0x80;
            }
        } else {
            ovl662_editor_cursor_state.words[1] &= ~0xAA;
        }
    }
}
