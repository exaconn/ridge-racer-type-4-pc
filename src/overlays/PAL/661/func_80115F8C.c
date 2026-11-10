#include "rr/course_choices.h"
#include "rr/load_cursor.h"
extern CourseChoice4 ovl661_car_choices[2][2][9];
extern s16 ovl661_selected_groups[2],ovl661_selected_cars[2];
void ovl661_reset_preview_selection(void)
{
    ResetScreenCounters();
    LoadCursor = MenuArenas.first;
    UnpackChoiceGlyph((s8 *)&SelectionRows + ActivePlayerIndex * 4, ovl661_car_choices[ActivePlayerIndex & 1][ovl661_selected_groups[ActivePlayerIndex & 1]][ovl661_selected_cars[ActivePlayerIndex & 1]].glyph);
    PlayerCourseChoices[ActivePlayerIndex].first = ovl661_car_choices[ActivePlayerIndex & 1][ovl661_selected_groups[ActivePlayerIndex & 1]][ovl661_selected_cars[ActivePlayerIndex & 1]].first;
    PlayerCourseChoices[ActivePlayerIndex].second = ovl661_car_choices[ActivePlayerIndex & 1][ovl661_selected_groups[ActivePlayerIndex & 1]][ovl661_selected_cars[ActivePlayerIndex & 1]].second;
    ImageRows[1] = 6;
}
