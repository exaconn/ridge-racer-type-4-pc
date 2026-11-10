#include "rr/course_choices.h"
#include "rr/load_cursor.h"
extern CourseChoice4 LinkCarChoiceTable[2][2][9];
extern s16 LinkCarChoiceGroup[2],LinkCarChoiceIndex[2];
void ovl667_reset_preview_selection(void) {
    ResetScreenCounters();
    LoadCursor = MenuArenas.first;
    UnpackChoiceGlyph(
        (s8 *)&SelectionRows + ActivePlayerIndex * 4,
        LinkCarChoiceTable[ActivePlayerIndex & 1][LinkCarChoiceGroup[ActivePlayerIndex & 1]][LinkCarChoiceIndex[ActivePlayerIndex & 1]].glyph
    );
    PlayerCourseChoices[ActivePlayerIndex].first = LinkCarChoiceTable[ActivePlayerIndex & 1][LinkCarChoiceGroup[ActivePlayerIndex & 1]][LinkCarChoiceIndex[ActivePlayerIndex & 1]].first;
    PlayerCourseChoices[ActivePlayerIndex].second = LinkCarChoiceTable[ActivePlayerIndex & 1][LinkCarChoiceGroup[ActivePlayerIndex & 1]][LinkCarChoiceIndex[ActivePlayerIndex & 1]].second;
    ImageRows[1] = 6;
}
