#include "rr/course_choices.h"
extern RrCourseChoice4 ovl661_D_80118458[2][2][9];
extern s16 ovl661_D_80118450[2],ovl661_D_8011844C[2];
void ovl661_func_80115F8C(void)
{
    ResetScreenCounters();
    LoadCursor = MenuArenas.first;
    UnpackChoiceGlyph((s8 *)&SelectionRows + D_800AC5D4 * 4, ovl661_D_80118458[D_800AC5D4 & 1][ovl661_D_80118450[D_800AC5D4 & 1]][ovl661_D_8011844C[D_800AC5D4 & 1]].glyph);
    D_800ACBD0[D_800AC5D4].first = ovl661_D_80118458[D_800AC5D4 & 1][ovl661_D_80118450[D_800AC5D4 & 1]][ovl661_D_8011844C[D_800AC5D4 & 1]].first;
    D_800ACBD0[D_800AC5D4].second = ovl661_D_80118458[D_800AC5D4 & 1][ovl661_D_80118450[D_800AC5D4 & 1]][ovl661_D_8011844C[D_800AC5D4 & 1]].second;
    ImageRows[1] = 6;
}
