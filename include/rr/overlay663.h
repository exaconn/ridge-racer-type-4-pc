#ifndef RR_OVERLAY_663_H
#define RR_OVERLAY_663_H

#include "rr/menu_overlay_runtime.h"

extern const char ovl663_scene_name[];
extern u8 ovl663_course_order[9];
extern u8 ovl663_time_text[10];
extern UiUvRect ovl663_record_badges[2];
extern u8 ovl663_race_column_widths[4];
extern u8 ovl663_lap_column_widths[4];
extern UiUvRect ovl663_record_label;
extern s32 ovl663_selected_record;
extern s32 ovl663_course_count;
extern s32 ovl663_available_courses[9];
extern s32 ovl663_extra_courses_enabled;
extern s32 ovl663_record_kind;
extern s32 ovl663_available_course_count;
extern s32 ovl663_course_unlock_levels[9];

void ovl663_init_menu(void);
void ovl663_update_menu(void);
void ovl663_init_selection(s32);
void ovl663_update_selection(void);
void ovl663_init_preview(void);
void ovl663_draw_preview(void);
void ovl663_update_preview(void);
void ovl663_init_result_preview(void);
void ovl663_update_result_preview(void);
u8 *ovl663_format_time(s32 milliseconds);
void *ovl663_draw_race_records(u32 *, void *, s32);
void *ovl663_draw_lap_records(u32 *, void *, s32);
void *ovl663_draw_record_labels(u32 *, void *, s32);
void ovl663_init_scene(void);

#endif
