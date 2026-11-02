#ifndef RR_OVERLAY_659_H
#define RR_OVERLAY_659_H

#include "rr/menu_overlay_runtime.h"

extern u8 ovl659_finish_rank_limits[4];
extern s32 ovl659_result_labels[34];
extern s32 ovl659_preview_timer;

void ovl659_update_race(void);
void ovl659_update_results(void);
void ovl659_rank_standings(void);
void ovl659_init_preview(void);
void ovl659_update_preview(void);
void ovl659_exit_race(void);
void ovl659_init_results(void);

#endif
