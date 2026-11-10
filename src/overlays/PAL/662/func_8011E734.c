#include "rr/overlay662_garage.h"
s32 ovl662_point_outside_button(s32 index){
 GarageRectangle *p=ovl662_button_rect_lookup[index];
 s32 px=ovl662_editor_cursor_state.halves[10],py;
 s32 x=(s16)(*p)[0],y;
 if(px<x || (py=ovl662_editor_cursor_state.halves[11])<(y=(s16)(*p)[1]))return 1;
 return (px<x+(s16)(*p)[2])?!(py<y+(s16)(*p)[3]):1;
}
