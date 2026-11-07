#ifndef RR_SEQUENCER_INIT_H
#define RR_SEQUENCER_INIT_H
#include "rr/library.h"

extern RrLibStateB0 *_ss_score[];
extern u32 _snd_openflag;
extern s16 _snd_seq_s_max, _snd_seq_t_max;

/* Low16 dimensions: banks0..32, rows0..32767. The table needs banks entries,
 * and base must cover banks*rows records (it may be null when banks==0).
 * Only the observed reset fields are initialized. Existing mask bits remain
 * set; bits banks..31 are added. State pointers and dimensions remain live
 * during initialization, including when state storage overlaps dimensions. */
void SsSetTableSize(RrLibStateB0 *base, s32 banks, s32 rows);
#endif
