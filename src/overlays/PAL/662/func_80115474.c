#include "rr/overlay662_transition.h"
#include "rr/load_cursor.h"

void ovl662_reset_team_car_preview(void)
{
    ResetScreenCounters();
    LoadCursor=MenuArenas.first;
    UnpackChoiceGlyph((s8 *)&SelectionRows,GaragePackedChoiceGlyph);
    ImageRows[1]=6;
}
