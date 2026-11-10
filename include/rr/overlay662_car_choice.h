#ifndef RR_OVERLAY662_CAR_CHOICE_H
#define RR_OVERLAY662_CAR_CHOICE_H
#include "rr/course_choice_types.h"
#include "rr/menu_choice.h"
#include "rr/menu_selection.h"
#include "rr/save_progress.h"
#include "rr/menu_transition.h"
#include "rr/string_tail_copy.h"
#include "rr/menu_glyph.h"
#include "rr/diagnostics.h"
extern CourseChoice4 CourseChoiceTable[2][9];
extern s16 ovl662_selected_car_slot,ovl662_selected_car_number,ovl662_selected_emblem_id;
extern char ovl662_saved_player_name[12];
extern const char ovl662_str_init_car_choice[];
void ovl662_init_team_car_layout(void);
void ovl662_init_garage_car_view(void);
/* Choice index0..8, signed byte coordinates, full nine-entry group. Saved and
 * destination names have accessible NUL terminators within12 bytes. */
void ovl662_init_team_car_choice(void);
void ovl662_select_garage_car(void);
#endif
