#include "rr/overlay662_options.h"
#include "rr/overlay662_distance.h"
#include "rr/overlay662_garage.h"

void ovl662_select_option_target(s32 mode) {
    GarageOptionRecord *options;
    s32 row;
    s32 col;
    s32 target;

    if (mode != 0) {
        s32 sel = ovl662_editor_workspace.selectorsB8[0];
        options = ovl662_editor_workspace.options78.groups.first[sel];
        col = ovl662_editor_workspace.selectorsB8[2];
        row = ovl662_editor_workspace.optionIndices98.groups.first[sel];
    } else {
        s32 sel = ovl662_editor_workspace.selectorsB8[1];
        options = ovl662_editor_workspace.options78.groups.second[sel];
        col = ovl662_editor_workspace.selectorsB8[3];
        row = ovl662_editor_workspace.optionIndices98.groups.second[sel];
    }
    target = *(col + options[row].destinations);
    ovl662_editor_cursor_state.points.second24 = ovl662_get_rect_center(target);
    ovl662_editor_cursor_state.points.distance2C = ovl662_calc_point_distance(&ovl662_editor_cursor_state.points.first20, &ovl662_editor_cursor_state.points.second24);
    ovl662_editor_workspace.current60 = target;
}

void ovl662_restore_saved_selection(void) {
    GarageSelectionPrefix *p = &ovl662_editor_workspace;
    s32 a = p->current60, b = p->selectorsB8[0], c = p->selectorsB8[1], d = p->selectorsB8[2], e = p->selectorsB8[3];
    p->saved64[0] = a;
    p->saved64[1] = b;
    p->saved64[2] = c;
    p->saved64[3] = d;
    p->saved64[4] = e;
}
