#include "rr/overlay665_visual.h"
void ovl665_draw_twist_gauge_needle(s32 arg0)
{
    u16 pts[4];

    pts[0] = (rsin(arg0) / 128) + ovl665_options_state.centerX38;
    pts[1] = (-rcos(arg0) / 64) + ovl665_options_state.centerY3A;
    pts[2] = ((rsin(arg0) * 40) / 8192) + ovl665_options_state.centerX38;
    pts[3] = ((-rcos(arg0) * 40) / 4096) + ovl665_options_state.centerY3A;
    AddLineF2Prim((const s16 *)pts, ovl665_twist_needle_color, 0, 0);
}
