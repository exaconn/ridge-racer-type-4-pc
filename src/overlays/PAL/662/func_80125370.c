#include "rr/overlay662_color_pick.h"


void ovl662_apply_color_picker(void) {
    s32 x, y;
    u8 *img0, *img1;
    u8 c;

    if (ovl662_point_outside_rect(82, 71, 128, 256) != 0) {
        return;
    }
    if (!(ovl662_editor_cursor_state.words[1] & 2)) {
        return;
    }
    img0 = (u8 *)(GarageImage512 *)ovl662_editor_workspace.image38[ovl662_editor_workspace.active21];
    img1 = (u8 *)(GarageImage512 *)ovl662_editor_workspace.image40[ovl662_editor_workspace.active21];
    x = ovl662_editor_workspace.reset08[0] + (ovl662_editor_cursor_state.halves[10] - 82) / ovl662_editor_workspace.config1C;
    y = ovl662_editor_workspace.reset08[1] + ((ovl662_editor_cursor_state.halves[11] - 71) / ovl662_editor_workspace.config1C) / 2;
    c = (img0 + y * 32)[x / 2];
    if (x & 1) {
        c >>= 4;
    } else {
        c &= 0xF;
    }
    if (c != 0) {
        ovl662_editor_workspace.config22 = c;
        PlaySfx(1);
    } else {
        c = (img1 + y * 32)[x / 2];
        if (x & 1) {
            c >>= 4;
        } else {
            c &= 0xF;
        }
        if (c != 0) {
            ovl662_editor_workspace.config22 = c;
            PlaySfx(1);
        } else {
            PlaySfx(4);
        }
    }
}
