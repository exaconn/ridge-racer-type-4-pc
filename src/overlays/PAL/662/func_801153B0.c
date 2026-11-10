#include "rr/overlay662_car_choice.h"

void ovl662_init_team_car_choice(void) {
    SetDispMask(0);
    ovl662_selected_car_slot = 0;
    CopyStringClearTail(ovl662_saved_player_name, SaveProgress.prefix.progress.prefix00.fields.name);
    ovl662_init_team_car_layout();
    UnpackChoiceGlyph((s8*)SelectionRows.bytes, CourseChoiceTable[1][0].glyph);
    PlayerCourseChoices[0].first = CourseChoiceTable[1][ovl662_selected_car_slot].first;
    PlayerCourseChoices[0].second = CourseChoiceTable[1][ovl662_selected_car_slot].second;
    ModeGroup = 1;
    ModeIndex = 0x40;
    SubmodeState = 0;
    DebugPrintStub(ovl662_str_init_car_choice);
}
