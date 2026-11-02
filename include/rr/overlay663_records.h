#ifndef RR_OVERLAY663_RECORDS_H
#define RR_OVERLAY663_RECORDS_H
#include "rr/menu_label_render.h"
#include "rr/overlay663_time.h"
#include "rr/time_record_types.h"
extern RrTimeRecord16 D_800AD5C8[2][8][5],D_800FF700[2][8][5];
extern s16 D_800AC648,D_800F3914;
extern s32 D_800ABEB0,ovl663_D_80116898;
extern RrUiUvRect D_8009FAA0[5],ovl663_D_80116880[2];
extern u8 ovl663_D_80116888[4],ovl663_D_8011688C[4];
s32 func_800904F4(s32);
void *func_80032EC8(u32 *,void *,s32,s32,s32);
/* Course index is0..7; row names a live badge. Group is captured once,
 * course and widths remain live across rendering services. The footer retains
 * its row coordinate across callbacks. Returned cursors keep native width. */
void *ovl663_func_8011602C(u32 *,void *,s32);
void *ovl663_func_801163B8(u32 *,void *,s32);
/* Three complete UI rows and two badge rows are available. UI cell count is
 *0..8,course selection0..15,current course0..7,record0..4,and menu choice0..1
 * whenever indexed. Packed record cars are0..479; decoder class2 selects entry0,
 * otherwise the special39 lookup uses group0,entry0..7 of the full2x9 owner.
 * Helpers may replace roots and selections within these bounds. Counters wrap32
 * and countdown wraps signed16; preview readiness requires full-word result1. */
void ovl663_func_801151B8(void);
#endif
