#include "rr/selection_initializer.h"
#include "rr/load_cursor.h"
void ovl667_func_801161A8(s32 arg0) {
    s16 *temp_s0;
    s16 *temp_t0_2;
    s32 temp_a0;
    s32 temp_a2;
    s32 temp_a2_2;
    s32 var_s1;
    MenuGridPoint *temp_a3_2;

    ModeFrame = 0;
    ScreenFrame = 0;
    MenuCursor = 0;
    MenuItemCount = 3;
    ResetScreenCounters();
    MenuArenas.second = CdDirectoryBuffer + 0x2D000;
    InitMenuArenas();
    ActivePlayerIndex = arg0;
    LoadCursor = MenuArenas.second;
    InitUiRow(0, 0x18, 0x58, 0x44, 0x20, 0, 0, 1, 0, 0x53);
    /* ASM_BARRIER: keep the published arena cell across the first UI service.
     * Read/write memory annotation retains its value and original address lifetime. */
    __asm__("" : "+m"(LoadCursor));
    InitUiRow(1, 0x18, 0x80, 0x44, 0x20, 0, 0, 1, 0, 0x36);
    InitUiRow(2, 0x18, 0xA8, 0x44, 0x20, 0, 0, 1, 0, 0x55);
    InitUiRow(3, 0x18, 0x58, 0x44, 0x20, 0, 0, 1, 0, 0x53);
    InitUiRow(4, 0x18, 0x80, 0x44, 0x20, 0, 0, 1, 0, 0x65);
    InitUiRow(5, 0x18, 0xA8, 0x44, 0x20, 0, 0, 1, 0, 0x35);
    InitUiRow(6, 0x18, 0xD0, 0x44, 0x20, 0, 0, 1, 0, 0x55);
    InitUiRow(7, 0x74, 0x58, 0x1C, 0x20, 0, 0, 1, 0, 0x37);
    InitUiRow(8, 0x74, 0x80, 0x1C, 0x20, 0, 0, 1, 0, 0x38);
    InitUiRow(9, 0x18, 0x58, 0x44, 0x20, 0, 0, 1, 0, 0x53);
    InitUiRow(0xA, 0x18, 0x80, 0x44, 0x20, 0, 0, 1, 0, 0x36);
    InitUiRow(0xB, 0x18, 0xA8, 0x44, 0x20, 0, 0, 1, 0, 0x55);
    InitUiRow(0xC, 0x18, 0x58, 0x44, 0x20, 0, 0, 1, 0, 0x53);
    InitUiRow(0xD, 0x18, 0x80, 0x44, 0x20, 0, 0, 1, 0, 0x65);
    InitUiRow(0xE, 0x18, 0xA8, 0x44, 0x20, 0, 0, 1, 0, 0x35);
    InitUiRow(0xF, 0x18, 0xD0, 0x44, 0x20, 0, 0, 1, 0, 0x55);
    InitUiRow(0x10, 0x74, 0x58, 0x1C, 0x20, 0, 0, 1, 0, 0x37);
    InitUiRow(0x11, 0x74, 0x80, 0x1C, 0x20, 0, 0, 1, 0, 0x38);
    InitUiRow(0x12, 0xC, 0x40, 0x44, 0x20, 0, 0, 1, 0, 0x54);
    InitUiRow(0x13, 0xC, 0x68, 0x44, 0x20, 0, 0, 1, 0, 3);
    InitUiRow(0x14, 0xC, 0x90, 0x44, 0x20, 0, 0, 1, 0, 0x55);
    InitUiRow(0x15, 0xC, 0x170, 0xD0, 0x20, 0, 0, 8, 4, SelectedCourseChoice);
    InitUiRow(0x17, 0x68, 0x68, 0x44, 0x20, 0, 0, 1, 5, 3);
    InitUiRow(0x18, 0x68, 0x40, 0x44, 0x20, 0, 0, 0, 0x7E, 0);
    SetupRenderer(5);
    MenuCursor = 0;
    MenuItemCount = 3;
    ResetScreenCounters();
    InitPreviewImageRows();
    D_8010C0B8 = 0;
    LoadCursor = MenuArenaBase;
    if (!(ActivePlayerIndex & 1)) {
        MenuArenas.first = MenuArenaBase;
    } else {
        LoadCursor = MenuArenas.first;
    }
    RR_PROJECTION_SCRATCH->pose08.angles10.xyz.x = 0x78;
    RR_PROJECTION_SCRATCH->pose08.position00.words[0] = -0x10;
    RR_PROJECTION_SCRATCH->pose08.position00.words[1] = -0xF8;
    RR_PROJECTION_SCRATCH->pose08.angles10.xyz.y = 0;
    RR_PROJECTION_SCRATCH->pose08.angles10.xyz.z = 0;
    RR_PROJECTION_SCRATCH->pose08.position00.words[2] = -0x60;
    for (var_s1 = 0; var_s1 < 2; var_s1++) {
        ResetCarState(&PlayerCars[var_s1]);
        PlayerCars[var_s1].position20.words[1] = 10;
        PlayerCars[var_s1].position20.words[2] = 308;
        PlayerCars[var_s1].rot54 = 0x800;
    }
    LinkCarChoiceCounts[0][0] = CountChoiceGlyphs(LinkCarChoiceTable[0][0]);
    LinkCarChoiceCounts[0][1] = CountChoiceGlyphs(LinkCarChoiceTable[0][1]);
    LinkCarChoiceCounts[1][0] = CountChoiceGlyphs(LinkCarChoiceTable[1][0]);
    LinkCarChoiceCounts[1][1] = CountChoiceGlyphs(LinkCarChoiceTable[1][1]);
    temp_a0 = arg0 & 1;
    if (LinkCarChoiceCounts[temp_a0][1] == 0) {
        LinkCarChoiceGroup[temp_a0] = 0;
    }
    temp_a2 = ActivePlayerIndex & 1;
    UnpackChoiceGlyph((s8 *)&SelectionRows + ActivePlayerIndex * 4,
        LinkCarChoiceTable[temp_a2][LinkCarChoiceGroup[temp_a2]][LinkCarChoiceIndex[temp_a2]].glyph);
    temp_a2_2 = ActivePlayerIndex & 1;
    temp_s0 = &LinkCarChoiceGroup[temp_a2_2];
    temp_t0_2 = &LinkCarChoiceIndex[temp_a2_2];
    temp_a3_2 = &PlayerCourseChoices[ActivePlayerIndex];
    temp_a3_2->first = LinkCarChoiceTable[temp_a2_2][*temp_s0][*temp_t0_2].first;
    temp_a3_2->second = LinkCarChoiceTable[temp_a2_2][*temp_s0][*temp_t0_2].second;
}
