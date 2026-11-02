#include "rr/overlay666.h"

u32 ovl666_module_id __attribute__((section(".ovl666.header"))) = 15;

const char ovl666_scene_name[24] __attribute__((section(".ovl666.scene_name"))) =
    "job_ex_crs_info0\n";

RrUiUvRect ovl666_selection_sprite
    __attribute__((section(".ovl666.selection_sprite"))) = {0, 120, 40, 80};

s32 ovl666_result_labels[33] __attribute__((section(".ovl666.result_labels"))) = {
    7702, 0,  -1, 3,  5,    23,   -1, -1,   -1, 7702, -1, 7702, 0,  -1,   19, -1, 28,
    29,   30, 31, -1, 7702, 7702, 17, 7702, -1, 6,    8,  9,    -1, 7702, -1, -1};

u16 ovl666_course_dispatch[4][4] __attribute__((section(".ovl666.course_dispatch"))) = {
    {6, 1, 6, 6}, {0, 0, 0, 3}, {4, 4, 7, 4}, {5, 2, 2, 2}};

s16 ovl666_course_counts[4] __attribute__((section(".ovl666.course_counts"))) = {0, 0, 0, 0};

s32 ovl666_preview_phase __attribute__((section(".ovl666.preview_phase"))) = 0;

s32 ovl666_result_kind __attribute__((section(".ovl666.result_kind"))) = 0;

RrMenuSelectionWord32 ovl666_selected_course
    __attribute__((section(".ovl666.selected_course"))) = {0};

RrCourseChoice4 ovl666_available_courses[4][9]
    __attribute__((section(".ovl666.available_courses"))) = {0};
