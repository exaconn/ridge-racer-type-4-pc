#include "rr/overlay661_layout.h"
#include "rr/load_cursor.h"
void ovl661_func_80115598(s32 mode) {
    s32 i;
    ProjectionScratchPrefix *scratch;
    void *arena;
    ModeFrame=0;
    ScreenFrame=0;
    MenuCursor=0;
    MenuItemCount=3;
    ResetScreenCounters();
    MenuArenas.second=CdDirectoryBuffer+0x2D000;
    InitMenuArenas();
    ActivePlayerIndex=mode;
    LoadCursor=MenuArenas.second;
    InitUiRow(0,24,88,68,32,0,0,1,0,83);
    InitUiRow(1,24,128,68,32,0,0,1,0,54);
    InitUiRow(2,24,168,68,32,0,0,1,0,85);
    InitUiRow(3,24,88,68,32,0,0,1,0,83);
    InitUiRow(4,24,128,68,32,0,0,1,0,101);
    InitUiRow(5,24,168,68,32,0,0,1,0,53);
    InitUiRow(6,24,208,68,32,0,0,1,0,85);
    InitUiRow(7,116,88,28,32,0,0,1,0,55);
    InitUiRow(8,116,128,28,32,0,0,1,0,56);
    InitUiRow(9,24,88,68,32,0,0,1,0,83);
    InitUiRow(10,24,128,68,32,0,0,1,0,54);
    InitUiRow(11,24,168,68,32,0,0,1,0,85);
    InitUiRow(12,24,88,68,32,0,0,1,0,83);
    InitUiRow(13,24,128,68,32,0,0,1,0,101);
    InitUiRow(14,24,168,68,32,0,0,1,0,53);
    InitUiRow(15,24,208,68,32,0,0,1,0,85);
    InitUiRow(16,116,88,28,32,0,0,1,0,55);
    InitUiRow(17,116,128,28,32,0,0,1,0,56);
    InitUiRow(18,12,64,68,32,0,0,1,0,84);
    InitUiRow(19,12,104,68,32,0,0,1,0,3);
    InitUiRow(20,12,144,68,32,0,0,1,1,13);
    InitUiRow(21,12,184,68,32,0,0,1,0,85);
    InitUiRow(22,12,368,208,32,0,0,8,4,SelectedCourseChoice);
    InitUiRow(23,104,104,68,32,0,0,1,5,3);
    InitUiRow(24,104,144,68,32,0,0,1,6,23);
    InitUiRow(25,104,184,68,32,0,0,1,7,6);
    SetupRenderer(5);
    MenuCursor=0;
    MenuItemCount=3;
    ResetScreenCounters();
    InitPreviewImageRows();
    /* ASM_BARRIER: reload LoadCursor address after InitPreviewImageRows before storing MenuArenaBase */
    __asm__("" : "+m"(LoadCursor));
    arena=MenuArenaBase;
    ovl661_D_80118438=0;
    LoadCursor=arena;
    if(!(ActivePlayerIndex&1))MenuArenas.first=arena;
    else LoadCursor=MenuArenas.first;
    scratch=RR_PROJECTION_SCRATCH;
    scratch->pose08.angles10.xyz.x=120;
    scratch->pose08.position00.words[0]=-16;
    scratch->pose08.position00.words[1]=-248;
    scratch->pose08.angles10.xyz.y=0;
    scratch->pose08.angles10.xyz.z=0;
    scratch->pose08.position00.words[2]=-96;
    for (i = 0; i < 2; i++) {
        ResetCarState(&PlayerCars[i]);
        PlayerCars[i].position20.words[1] = 10;
        PlayerCars[i].position20.words[2] = 308;
        PlayerCars[i].rot54 = 2048;
    }
    ovl661_D_80118440[0][0]=CountChoiceGlyphs(ovl661_D_80118458[0][0]);
    ovl661_D_80118440[0][1]=CountChoiceGlyphs(ovl661_D_80118458[0][1]);
    ovl661_D_80118440[1][0]=CountChoiceGlyphs(ovl661_D_80118458[1][0]);
    ovl661_D_80118440[1][1]=CountChoiceGlyphs(ovl661_D_80118458[1][1]);
    if(ovl661_D_80118440[mode&1][1]==0)ovl661_D_80118450[mode&1]=0;
    if(ovl661_D_8011844C[ActivePlayerIndex&1]>=ovl661_D_80118440[ActivePlayerIndex&1][ovl661_D_80118450[ActivePlayerIndex&1]])ovl661_D_8011844C[ActivePlayerIndex&1]=0;
    UnpackChoiceGlyph((s8*)&SelectionRows+ActivePlayerIndex*4,ovl661_D_80118458[ActivePlayerIndex&1][ovl661_D_80118450[ActivePlayerIndex&1]][ovl661_D_8011844C[ActivePlayerIndex&1]].glyph);
    PlayerCourseChoices[ActivePlayerIndex].first=ovl661_D_80118458[ActivePlayerIndex&1][ovl661_D_80118450[ActivePlayerIndex&1]][ovl661_D_8011844C[ActivePlayerIndex&1]].first;
    PlayerCourseChoices[ActivePlayerIndex].second=ovl661_D_80118458[ActivePlayerIndex&1][ovl661_D_80118450[ActivePlayerIndex&1]][ovl661_D_8011844C[ActivePlayerIndex&1]].second;
}
