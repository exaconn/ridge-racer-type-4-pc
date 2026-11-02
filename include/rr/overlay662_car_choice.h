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
extern RrCourseChoice4 D_800ABE08[2][9];
extern s16 ovl662_D_801272E4,ovl662_D_801272BC,ovl662_D_801272E2;
extern char ovl662_D_801272C8[12];
extern const char ovl662_D_80114888[];
void ovl662_func_8011512C(void);
void ovl662_func_80118ED8(void);
/* Choice index0..8, signed byte coordinates, full nine-entry group. Saved and
 * destination names have accessible NUL terminators within12 bytes. */
void ovl662_func_801153B0(void);
void ovl662_func_80118E6C(void);
#endif
