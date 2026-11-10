#include "rr/overlay662_garage.h"
void PlaySfx(s32 mode);
void ovl662_toggle_preview_side(void)
{
    if(ovl662_editor_cursor_state.words[1]&2){
        PlaySfx(1);
        ovl662_editor_workspace.active21=!ovl662_editor_workspace.active21;
    }
}
