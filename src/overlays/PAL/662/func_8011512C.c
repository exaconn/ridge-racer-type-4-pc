#include "rr/overlay662_car_layout.h"
#include "rr/load_cursor.h"
void ovl662_func_8011512C(void) {
    s32 i;
    ProjectionScratchPrefix *scratch;
    void *arena;
    ClearScreenOverlayState();
    ModeFrame=0;
    ScreenFrame=0;
    MenuCursor=0;
    MenuItemCount=4;
    ResetScreenCounters();
    MenuArenas.second=CdDirectoryBuffer+0x2D000;
    InitMenuArenas();
    LoadCursor=MenuArenas.second;
    InitUiRow(0,24,88,68,32,0,0,1,0,21);
    InitUiRow(1,24,128,68,32,0,0,1,0,102);
    InitUiRow(2,24,168,68,32,0,0,1,0,22);
    InitUiRow(3,24,208,68,32,0,0,1,0,85);
    SetupRenderer(5);
    ResetScreenCounters();
    InitPreviewImageRows();
    i=0;
    ovl662_D_801272D4=0;
    scratch=RR_PROJECTION_SCRATCH;
    scratch->pose08.angles10.xyz.x=120;
    scratch->pose08.position00.words[0]=-16;
    scratch->pose08.position00.words[1]=-248;
    scratch->pose08.angles10.xyz.y=0;
    scratch->pose08.angles10.xyz.z=0;
    scratch->pose08.position00.words[2]=-96;
    arena=MenuArenaBase;
    LoadCursor=arena;
    MenuArenas.first=arena;
    for (i = 0; i < 2; i++) {
        ResetCarState(&PlayerCars[i]);
        PlayerCars[i].position20.words[1] = 10;
        PlayerCars[i].position20.words[2] = 308;
        PlayerCars[i].rot54 = 2048;
    }
    UnpackChoiceGlyph((s8*)SelectionRows.bytes,CourseChoiceTable[1][ovl662_D_801272E4].glyph);
    PlayerCourseChoices[0].first=CourseChoiceTable[1][ovl662_D_801272E4].first;
    PlayerCourseChoices[0].second=CourseChoiceTable[1][ovl662_D_801272E4].second;
    ovl662_D_801272E0=CountChoiceGlyphs(CourseChoiceTable[1]);
    SetMenuFrameOptions(0,1,1);
}
