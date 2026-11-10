#include "rr/overlay663.h"

u32 ovl663_module_id __attribute__((section(".ovl663.header"))) = 13;

const char ovl663_scene_name[16] __attribute__((section(".ovl663.scene_name"))) = "recode0\n";

u8 ovl663_course_order[9]
    __attribute__((section(".ovl663.course_order"))) = {1, 0, 2, 3, 8, 4, 6, 7, 5};

u8 ovl663_time_text[10]
    __attribute__((section(".ovl663.time_text"))) = {48, 48, 39, 48, 48, 34, 48, 48, 48, 0};

UiUvRect ovl663_record_badges[2]
    __attribute__((section(".ovl663.record_badges"))) = {{0, 216, 16, 32}, {16, 216, 16, 32}};

u8 ovl663_race_column_widths[4]
    __attribute__((section(".ovl663.race_column_widths"))) = {20, 8, 60, 8};

u8 ovl663_lap_column_widths[4]
    __attribute__((section(".ovl663.lap_column_widths"))) = {20, 8, 60, 8};

UiUvRect ovl663_record_label __attribute__((section(".ovl663.record_label"))) = {176, 16, 48, 16};

s32 ovl663_selected_record __attribute__((section(".ovl663.selected_record"))) = 0;

s32 ovl663_course_count __attribute__((section(".ovl663.course_count"))) = 0;

s32 ovl663_available_courses[9]
    __attribute__((section(".ovl663.available_courses"))) = {0, 0, 0, 0, 0, 0, 0, 0, 0};

s32 ovl663_extra_courses_enabled __attribute__((section(".ovl663.extra_courses_enabled"))) = 0;

s32 ovl663_record_kind __attribute__((section(".ovl663.record_kind"))) = 0;

s32 ovl663_available_course_count __attribute__((section(".ovl663.available_course_count"))) = 0;

s32 ovl663_course_unlock_levels[9]
    __attribute__((section(".ovl663.course_unlock_levels"))) = {0, 0, 0, 0, 0, 0, 0, 0, 0};
