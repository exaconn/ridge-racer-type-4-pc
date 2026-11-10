#include "rr/overlay662_garage.h"
s32 ovl662_map_controller_buttons(s32 mode)
{
    if(mode){
        ovl662_editor_cursor_state.words[2]++;
        if(ovl662_editor_cursor_state.words[2]>=47)ovl662_editor_cursor_state.words[2]=40;
        if(ovl662_editor_cursor_state.words[2]==1 || ovl662_editor_cursor_state.words[2]==40)return 1;return 0;
    }else{
        ovl662_editor_cursor_state.words[3]++;
        if(ovl662_editor_cursor_state.words[3]>=47)ovl662_editor_cursor_state.words[3]=40;
        if(ovl662_editor_cursor_state.words[3]==1 || ovl662_editor_cursor_state.words[3]==40)return 1;return 0;
    }
}
