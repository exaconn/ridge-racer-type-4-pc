#include "rr/overlay662_car_choice.h"

void ovl662_select_garage_car(void) {
    s16 v0 = CourseChoiceTable[1][ovl662_selected_car_slot].first;
    s16 v1;
    ovl662_selected_car_number = v0;
    v1 = CourseChoiceTable[1][ovl662_selected_car_slot].second;
    PlayerCourseChoices[0].first = v0;
    ovl662_selected_emblem_id = v1;
    PlayerCourseChoices[0].second = v1;
    ovl662_init_garage_car_view();
}
