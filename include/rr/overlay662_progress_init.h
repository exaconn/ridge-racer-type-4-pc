#ifndef RR_OVERLAY662_PROGRESS_INIT_H
#define RR_OVERLAY662_PROGRESS_INIT_H
#include "rr/overlay_menu_init.h"
#include "rr/save_progress.h"
/* Resident save prefix: the sixteen progress words start at offset10.
 * Only bits0..19 of each word contribute to these menu counts. */
extern u16 ovl662_team_maker_car_counts[4][4];
extern u16 ovl662_team_unlocked_counts[4],ovl662_total_unlocked_cars;
void ovl662_init_progress_menu(void);
#endif
