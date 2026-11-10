#ifndef RR_OVERLAY662_CAR_LAYOUT_H
#define RR_OVERLAY662_CAR_LAYOUT_H
#include "rr/overlay662_car_choice.h"
#include "rr/race.h"
#include "rr/projection_state.h"
#include "rr/menu_arenas.h"
#include "rr/renderer_setup.h"
#include "rr/menu_setup.h"
#include "rr/overlay_menu.h"
extern s16 ovl662_D_801272D4,ovl662_D_801272E0;
void InitPreviewImageRows(void);
void ResetCarState(RaceVehicle *vehicle);
s32 CountChoiceGlyphs(const CourseChoice4 *choices);
void SetMenuFrameOptions(s32,s32,s32);
/* Fresh choice index0..8. Vehicle setup may change published roots and choices.
 * Nine-entry course group and two complete vehicle owners remain accessible. */
#ifndef RR_PROJECTION_SCRATCH
#define RR_PROJECTION_SCRATCH ((ProjectionScratchPrefix *)0x1F800000U)
#endif
#endif
