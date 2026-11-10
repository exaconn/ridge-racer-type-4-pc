#include "rr/overlay662_scroll_drag.h"

void ovl662_drag_v_scrollbar(void) {
    s32 imageTravel;
    s32 trackTravel;
    s32 limit;
    s32 imageThumb;
    u32 trackThumb;
    s32 projected;
    s32 relative;
    u16 narrowed;
    GarageSelectionPrefix *selection;
    s32 coordinate;

    if (ovl662_editor_cursor_state.words[0] & 8) {
        if (ovl662_editor_cursor_state.words[1] & 4) {
            ovl662_editor_cursor_state.words[0] = (ovl662_editor_cursor_state.words[0] & ~8);
            ovl662_set_clip_rect(0xD, 0x3F, 0x126, 0x15A);
            return;
        }
    } else if (((u8) ovl662_editor_workspace.config1C >= 4U) && (ovl662_point_outside_rect(0xD8, 0x57, 4, 0xE0) == 0) && (ovl662_editor_cursor_state.words[1] & 1)) {
        ovl662_editor_cursor_state.words[4] = 0x2F;
        ovl662_editor_cursor_state.words[0] = (ovl662_editor_cursor_state.words[0] | 8);
        ovl662_set_clip_rect(0xD8, 0x57, 4, 0xE0);
        imageThumb = 0x40 / ((u8) ovl662_editor_workspace.config1C >> 1);
        /* ASM_BARRIER: prevent CSE of config1C >> 1 across divisions */
        __asm__("" : : : "memory");
        trackThumb = 0xE0 / ((u8) ovl662_editor_workspace.config1C >> 1);
        imageTravel = 0x40 - imageThumb;
        trackTravel = 0xE0 - trackThumb;
        projected = ((ovl662_editor_cursor_state.points.cursor14.y - 0x57) * imageTravel) / trackTravel;
        ovl662_vscroll_image_thumb = imageThumb;
        ovl662_vscroll_image_travel = imageTravel;
        ovl662_vscroll_track_thumb = trackThumb;
        ovl662_vscroll_track_travel = trackTravel;
        relative = projected - (s16) ovl662_editor_workspace.reset08[1];
        if ((relative < 0) || (relative >= imageThumb)) {
            ovl662_vscroll_grab_offset = (u16) ((s32) ((trackThumb >> 1) * imageTravel) / trackTravel);
        } else {
            ovl662_vscroll_grab_offset = projected - ovl662_editor_workspace.reset08[1];
        }
    } else {
        return;
    }
    {
        coordinate=ovl662_editor_cursor_y_alias-0x57;
        limit = ovl662_vscroll_image_travel;
        selection=&ovl662_editor_workspace;
        narrowed = ((coordinate * limit) / ovl662_vscroll_track_travel) - ovl662_vscroll_grab_offset;
        selection->reset08[1] = narrowed;
        if ((s16) narrowed < 0) {
            selection->reset08[1] = 0U;
            return;
        }
        if (limit < (s16) narrowed) {
            selection->reset08[1] = (u16) ovl662_vscroll_image_travel;
        }
    }
}
