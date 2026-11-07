#include "rr/overlay662_transition.h"

void ovl662_func_80115474(void)
{
    ResetScreenCounters();
    LoadCursor=D_800F4D70.first;
    UnpackChoiceGlyph((s8 *)&SelectionRows,D_800ABE2C);
    D_800F2AB0[1]=6;
}
