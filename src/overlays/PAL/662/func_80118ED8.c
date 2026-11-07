#include "rr/overlay662_screen_init.h"
void ovl662_func_80118ED8(void) {
    ModeFrame = 0;
    ScreenFrame = 0;
    ovl662_D_801272D4 = 0;
    MenuCursor = 0;
    MenuItemCount = 3;
    ResetScreenCounters();
    D_800F4D70.second = CdDirectoryBuffer + 0x2D000;
    InitMenuArenas();
    LoadCursor = D_800F4D70.second;
    InitUiRow(0, 24, 88, 68, 32, 0, 0, 1, 0, 83);
    InitUiRow(1, 24, 128, 68, 32, 0, 0, 1, 0, 42);
    InitUiRow(2, 24, 168, 68, 32, 0, 0, 1, 0, 85);
    func_80037110();
    InitBadgeRow(1, 24, 232, 84, 34, 0, 0x501);
    SetupRenderer(5);
    ModeGroup = 1;
    D_800F3A26 = 68;
    LoadCursor = D_800F4D70.first = D_800F4D40;
    D_800F4D8A = 0;
}
