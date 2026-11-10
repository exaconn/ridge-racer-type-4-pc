#include "rr/overlay662_preview_drag.h"

void ovl662_drag_preview_viewport(void) {
    u16 *grab;
    s32 offsetX;
    s32 thumb;
    s32 center;
    u16 nextX;
    u32 deltaY;

    if (ovl662_editor_cursor_state.words[0] & 8) {
        if (ovl662_editor_cursor_state.words[1] & 4) {
            ovl662_editor_cursor_state.words[0] = (ovl662_editor_cursor_state.words[0] & ~8);
            ovl662_set_clip_rect(0xD, 0x3F, 0x126, 0x15A);
            return;
        }
    } else if (((u8) ovl662_editor_workspace.config1C >= 4U) && (ovl662_point_outside_rect(0xED, 0x47, 0x40, 0x80) == 0) && (ovl662_editor_cursor_state.words[1] & 1)) {
        ovl662_editor_cursor_state.words[4] = 0x33;
        ovl662_editor_cursor_state.words[0] = (ovl662_editor_cursor_state.words[0] | 8);
        ovl662_set_clip_rect(0xED, 0x47, 0x40, 0x80);
        thumb = 0x40 / ((u8) ovl662_editor_workspace.config1C >> 1);
        ovl662_preview_drag_thumb = thumb;
        ovl662_preview_drag_limit = 0x40 - thumb;
        if (ovl662_point_outside_rect((s16) (ovl662_editor_workspace.reset08[0] + 0xED), (s16) ((ovl662_editor_workspace.reset08[1] * 2) + 0x47), (s16) ovl662_preview_drag_thumb, thumb * 2) != 0) {
            grab=ovl662_preview_drag_grab;
            center = ovl662_preview_drag_thumb / 2;
            grab[1] = (u16) center;
            ovl662_preview_drag_grab[0] = (u16) center;
        } else {
            ovl662_preview_drag_grab[0] = (u16) ((ovl662_editor_cursor_state.points.cursor14.x - 0xED) - ovl662_editor_workspace.reset08[0]);
            ovl662_preview_drag_grab[1] = (u16) ((((s16) ovl662_editor_cursor_state.points.cursor14.y / 2) - 0x23) - (u16) ovl662_editor_workspace.reset08[1]);
        }
    } else {
        return;
    }
    {
        /* ASM_BARRIER: reload cursor and reset globals at the common drag join */
        __asm__("" : : "m"(ovl662_editor_workspace),"m"(ovl662_editor_cursor_state));
        offsetX=ovl662_preview_drag_grab[0]+0xED;
        
        nextX = ovl662_editor_cursor_state.points.cursor14.x - offsetX;
        ovl662_editor_workspace.reset08[0] = nextX;
        deltaY = (s16) ovl662_editor_cursor_state.points.cursor14.y - 0x47;
        ovl662_editor_workspace.reset08[1] = (s16) (((s32) deltaY / 2) - ovl662_preview_drag_grab[1]);
        if ((s16) nextX < 0) {
            ovl662_editor_workspace.reset08[0] = 0U;
        } else if (ovl662_preview_drag_limit < (s16) nextX) {
            ovl662_editor_workspace.reset08[0] = (u16) ovl662_preview_drag_limit;
        }
        if (ovl662_editor_workspace.reset08[1] < 0) {
            ovl662_editor_workspace.reset08[1] = 0;
            return;
        }
        if (ovl662_preview_drag_limit < ovl662_editor_workspace.reset08[1]) {
            ovl662_editor_workspace.reset08[1] = (s16) (u16) ovl662_preview_drag_limit;
        }
    }
}
