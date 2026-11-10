#include "rr/overlay662_scroll_drag.h"

void ovl662_drag_h_scrollbar(void) {
    s32 imageTravel;
    s32 trackTravel;
    s32 limit;
    s32 imageThumb;
    u32 trackThumb;
    s32 projected;
    s32 relative;
    u16 narrowed;
    s32 coordinate;
    GarageSelectionPrefix *selection;

    if (ovl662_editor_cursor_state.words[0] & 8) {
        if (ovl662_editor_cursor_state.words[1] & 4) {
            ovl662_editor_cursor_state.words[0] = (ovl662_editor_cursor_state.words[0] & ~8);
            ovl662_set_clip_rect(0xD, 0x3F, 0x126, 0x15A);
            return;
        }
    } else if (((u8) ovl662_editor_workspace.config1C >= 4U) && (ovl662_point_outside_rect(0x5A, 0x151, 0x70, 8) == 0) && (ovl662_editor_cursor_state.words[1] & 1)) {
        ovl662_editor_cursor_state.words[4] = 0x2E;
        ovl662_editor_cursor_state.words[0] = (ovl662_editor_cursor_state.words[0] | 8);
        ovl662_set_clip_rect(0x5A, 0x151, 0x70, 8);
        imageThumb = 0x40 / ((u8) ovl662_editor_workspace.config1C >> 1);
        /* ASM_BARRIER: prevent CSE of config1C >> 1 across divisions */
        __asm__("" : : : "memory");
        trackThumb = 0x70 / ((u8) ovl662_editor_workspace.config1C >> 1);
        imageTravel = 0x40 - imageThumb;
        trackTravel = 0x70 - trackThumb;
        projected = ((ovl662_editor_cursor_state.points.cursor14.x - 0x5A) * imageTravel) / trackTravel;
        ovl662_hscroll_image_thumb = imageThumb;
        ovl662_hscroll_image_travel = imageTravel;
        ovl662_hscroll_track_thumb = trackThumb;
        ovl662_hscroll_track_travel = trackTravel;
        relative = projected - (s16) ovl662_editor_workspace.reset08[0];
        if ((relative < 0) || (relative >= imageThumb)) {
            ovl662_hscroll_grab_offset = (u16) ((s32) ((trackThumb >> 1) * imageTravel) / trackTravel);
        } else {
            ovl662_hscroll_grab_offset = projected - ovl662_editor_workspace.reset08[0];
        }
    } else {
        return;
    }
    {
        coordinate=ovl662_editor_cursor_x_alias-0x5A;
        limit = ovl662_hscroll_image_travel;
        selection=&ovl662_editor_workspace;
        narrowed = ((coordinate * limit) / ovl662_hscroll_track_travel) - ovl662_hscroll_grab_offset;
        selection->reset08[0] = narrowed;
        if ((s16) narrowed < 0) {
            selection->reset08[0] = 0U;
            return;
        }
        if (limit < (s16) narrowed) {
            selection->reset08[0] = (u16) ovl662_hscroll_image_travel;
        }
    }
}
