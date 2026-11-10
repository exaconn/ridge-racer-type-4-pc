#include "rr/overlay662_garage.h"
void ovl662_backup_layer_and_palette(void)
{
    s32 i;
    u8 selected=ovl662_editor_workspace.active21;
    u8 other;
    u32 *source,*destination;
    ovl662_editor_workspace.dirtyD3=1;
    other=!selected;
    source=ovl662_editor_workspace.image38[selected];
    destination=ovl662_editor_workspace.image38[other];
    for(i=0;i<512;){
        i++;
        *destination++=*source++;
    }
    source=ovl662_editor_workspace.image40[selected];
    destination=ovl662_editor_workspace.image40[other];
    for(i=0;i<512;){
        i++;
        *destination++=*source++;
    }
    source=ovl662_editor_workspace.palette48[selected];
    destination=ovl662_editor_workspace.palette48[other];
    for(i=0;i<8;){
        i++;
        *destination++=*source++;
    }
}
