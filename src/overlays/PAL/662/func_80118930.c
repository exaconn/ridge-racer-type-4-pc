#include "rr/overlay662_name_init.h"
void ovl662_init_name_entry(void) {
    ModeFrame = 0;
    ScreenFrame = 0;
    ovl662_name_grid_col = 0;
    ovl662_name_grid_row = 0;
    ovl662_name_entry_cursor = 0;
    ovl662_name_grid_index = 0;
    CopyStringClearTail((char *)ovl662_name_entry_buffer, (char *)&SaveProgress);
    ClearScreenOverlayState();
    InitBadgeRow(0, 4, 172, 312, 216, 0, 0x806);
    BadgeRows[0].half02 = 1;
    InitBadgeRow(1, 208, 84, 108, 80, 0, 0x905);
    BadgeRows[1].half02 = 1;
    SetMenuFrameOptions(0, 1, 1);
    ModeGroup = 1;
    ModeIndex = 28;
    SubmodeState = 0;
}
