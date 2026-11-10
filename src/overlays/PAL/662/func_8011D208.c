#include "rr/overlay662_garage.h"
#include "rr/trig.h"
void ovl662_update_cursor_pulse(void)
{
    ovl662_editor_workspace.shadeD0=rsin(ovl662_editor_workspace.phaseCC)/64-65;
    ovl662_editor_workspace.phaseCC+=96U;
    ovl662_editor_workspace.phaseCC=(s32)ovl662_editor_workspace.phaseCC%4096;
}
