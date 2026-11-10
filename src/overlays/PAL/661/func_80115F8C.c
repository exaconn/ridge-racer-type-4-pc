#include "rr/course_choices.h"
#include "rr/load_cursor.h"
extern CourseChoice4 ovl661_D_80118458[2][2][9];
extern s16 ovl661_D_80118450[2],ovl661_D_8011844C[2];
void ovl661_func_80115F8C(void)
{
    ResetScreenCounters();
    LoadCursor = MenuArenas.first;
    UnpackChoiceGlyph((s8 *)&SelectionRows + ActivePlayerIndex * 4, ovl661_D_80118458[ActivePlayerIndex & 1][ovl661_D_80118450[ActivePlayerIndex & 1]][ovl661_D_8011844C[ActivePlayerIndex & 1]].glyph);
    PlayerCourseChoices[ActivePlayerIndex].first = ovl661_D_80118458[ActivePlayerIndex & 1][ovl661_D_80118450[ActivePlayerIndex & 1]][ovl661_D_8011844C[ActivePlayerIndex & 1]].first;
    PlayerCourseChoices[ActivePlayerIndex].second = ovl661_D_80118458[ActivePlayerIndex & 1][ovl661_D_80118450[ActivePlayerIndex & 1]][ovl661_D_8011844C[ActivePlayerIndex & 1]].second;
    ImageRows[1] = 6;
}
