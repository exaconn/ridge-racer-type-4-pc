#include "rr/overlay662_transition.h"
#include "rr/load_cursor.h"

void ovl662_func_80115474(void)
{
    ResetScreenCounters();
    LoadCursor=MenuArenas.first;
    UnpackChoiceGlyph((s8 *)&SelectionRows,D_800ABE2C);
    ImageRows[1]=6;
}
