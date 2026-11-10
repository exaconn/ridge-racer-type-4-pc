#include "rr/overlay662_garage.h"
void ovl662_set_button_clip_rect(s32 index){
 GarageRectangle *p=ovl662_button_rect_lookup[index];
 ovl662_editor_cursor_state.halves[24]=(*p)[0];
 ovl662_editor_cursor_state.halves[25]=(*p)[1];
 ovl662_editor_cursor_state.halves[26]=(*p)[2];
 ovl662_editor_cursor_state.halves[27]=(*p)[3];
}
