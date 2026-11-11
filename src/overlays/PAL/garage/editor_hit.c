#include "rr/overlay662_garage.h"

s32 ovl662_point_outside_rect(s32 raw_x,s32 raw_y,s32 raw_w,s32 raw_h){
 s32 px=ovl662_editor_cursor_state.halves[10];
 s16 x=raw_x,y=raw_y;
 s32 py;
 if(px<x || (py=ovl662_editor_cursor_state.halves[11])<y)return 1;
 return (px<x+(s16)raw_w)?!(py<y+(s16)raw_h):1;
}


s32 ovl662_point_outside_button(s32 index){
 GarageRectangle *p=ovl662_button_rect_lookup[index];
 s32 px=ovl662_editor_cursor_state.halves[10],py;
 s32 x=(s16)(*p)[0],y;
 if(px<x || (py=ovl662_editor_cursor_state.halves[11])<(y=(s16)(*p)[1]))return 1;
 return (px<x+(s16)(*p)[2])?!(py<y+(s16)(*p)[3]):1;
}


void ovl662_set_clip_rect(s32 a, s32 b, s32 c, s32 d)
{
    ovl662_editor_cursor_state.halves[24] = a;
    ovl662_editor_cursor_state.halves[25] = b;
    ovl662_editor_cursor_state.halves[26] = c;
    ovl662_editor_cursor_state.halves[27] = d;
}


void ovl662_set_button_clip_rect(s32 index){
 GarageRectangle *p=ovl662_button_rect_lookup[index];
 ovl662_editor_cursor_state.halves[24]=(*p)[0];
 ovl662_editor_cursor_state.halves[25]=(*p)[1];
 ovl662_editor_cursor_state.halves[26]=(*p)[2];
 ovl662_editor_cursor_state.halves[27]=(*p)[3];
}

