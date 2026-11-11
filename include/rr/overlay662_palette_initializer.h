#ifndef RR_OVERLAY662_PALETTE_INITIALIZER_H
#define RR_OVERLAY662_PALETTE_INITIALIZER_H
#include "rr/overlay_menu_init.h"
#include "rr/save_progress.h"
#include "rr/course_choice_types.h"
extern CourseChoice4 CourseChoiceTable[2][9];
extern s16 ovl662_menu_substate,ovl662_emblem_choice_count,ovl662_emblem_preview_fade,ovl662_selected_emblem_id,ovl662_selected_car_slot,ovl662_selected_emblem_slot;
extern s32 ovl662_history_arrow_phase,ovl662_emblem_edit_from_menu;
extern u8 ovl662_emblem_choice_map[25];
#include "rr/emblem_images.h"
extern void *PresetEmblemTimPtr;
void UploadCarClut(void *,s32,s32);
/* Fresh course selector is0..8. Its signed second byte is at most25;
 * positive values select one of25 map slots. Palette bank1 covers25 TIMs.
 * UI and palette services may replace global roots and mutate shared state. */
void ovl662_init_emblem_selector(void);
#endif
