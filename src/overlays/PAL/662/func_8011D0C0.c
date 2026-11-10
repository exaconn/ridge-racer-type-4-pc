#include "rr/overlay662_garage.h"
s32 ovl662_is_active_layer_empty(void)
{
    u32 mode=ovl662_editor_workspace.layer23;
    u32 selected=ovl662_editor_workspace.active21;
    u32 *cursor;
    s32 i;
    if(mode==0)
        cursor=ovl662_editor_workspace.image38[selected];
    else
        cursor=ovl662_editor_workspace.image40[selected];
    for(i=0;i<512;cursor++){
        i++;
        if(*cursor)return 0;
    }
    return 1;
}
