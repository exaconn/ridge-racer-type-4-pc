#include "rr/overlay665_menu.h"

void ovl665_func_80114A24(void) {
    ClearScreenOverlayState();
    ResetScreenCounters();
    D_800F4D70.second=CdDirectoryBuffer+0x2D000;
    InitMenuArenas();
    LoadCursor=D_800F4D70.second;
    SetupRenderer(5);
    InitUiRow(0,94,96,112,32,0,0,1,8,19);
    InitUiRow(1,94,136,112,32,0,0,1,9,19);
    InitUiRow(2,94,176,112,32,0,0,1,1,25);
    InitUiRow(3,94,216,112,32,0,0,1,1,20);
    InitUiRow(4,94,256,112,32,0,0,1,1,21);
    InitUiRow(5,94,296,112,32,0,0,1,1,18);
    InitUiRow(6,94,336,112,32,0,0,1,0,85);
    SetMenuFrameOptions(1,1,1);
    if(RaceContext.mode18==1) {
        ovl665_D_80119768.selection08=5;
        RaceContext.mode18=0;
    }
    D_800AC5D0=64;
    RaceLoadReset();
    func_80053BB4(0);
    D_800AC590->b4=-1;
}
