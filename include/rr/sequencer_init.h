#ifndef RR_SEQUENCER_INIT_H
#define RR_SEQUENCER_INIT_H
#include "rr/library.h"

extern RrLibStateB0 *D_8010F068[];
extern u32 D_8010F060;
extern s16 D_8010F8E8, D_8010F8EA;

/* Low16 dimensions: banks0..32, rows0..32767. The table needs banks entries,
 * and base must cover banks*rows records (it may be null when banks==0).
 * Only the observed reset fields are initialized. Existing mask bits remain
 * set; bits banks..31 are added. State pointers and dimensions remain live
 * during initialization, including when state storage overlaps dimensions. */
void func_80083DE4(RrLibStateB0 *base, s32 banks, s32 rows);
#endif
