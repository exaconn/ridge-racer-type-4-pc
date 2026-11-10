#include "rr/overlay662_actions.h"
void ovl662_handle_shortcut_buttons(u32 flags){
 if(flags)PlaySfx(0);
 if(flags&1)ovl662_action_lookup[ovl662_editor_workspace.current60]->callback(1);
 else if(flags&2)ovl662_action_lookup[ovl662_editor_workspace.current60]->callback(2);
 if(flags&4)ovl662_action_lookup[ovl662_editor_workspace.current60]->callback(4);
 else if(flags&8)ovl662_action_lookup[ovl662_editor_workspace.current60]->callback(8);
}
