#ifndef RR_OVERLAY_666_H
#define RR_OVERLAY_666_H

#include "rr/menu_overlay_runtime.h"

extern const char ovl666_scene_name[];
extern RrUiUvRect ovl666_selection_sprite;
extern s32 ovl666_result_labels[33];
extern u16 ovl666_course_dispatch[4][4];
extern s16 ovl666_course_counts[4];
extern s32 ovl666_preview_phase;
extern s32 ovl666_result_kind;
extern RrMenuSelectionWord32 ovl666_selected_course;
extern RrCourseChoice4 ovl666_available_courses[4][9];

void ovl666_init_menu(void);
void ovl666_update_menu(void);
void ovl666_update_course_selection(void);
void ovl666_draw_selection_pulse(s32, s32);
s32 ovl666_update_retry_choice(s32);
void ovl666_update_results(void);
void ovl666_init_preview(void);
void ovl666_update_preview(void);
void ovl666_update_checkpoint(RrRaceCheckpointVehicle *vehicle);
void ovl666_update_race(void);
void ovl666_init_scene(void);
void ovl666_init_results(void);
void ovl666_exit_race(void);

#endif
